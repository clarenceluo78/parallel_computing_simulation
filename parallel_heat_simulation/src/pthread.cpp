#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#ifdef GUI
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "./headers/physics.h"

int size; // problem size
int n_thd; // number of threads

void initialize(float *data) {
    // intialize the temperature distribution
    int len = size * size;
    for (int i = 0; i < len; i++) {
        data[i] = wall_temp;
    }
}

void generate_fire_area(bool *fire_area) {
    // generate the fire area
    int len = size * size;
    for (int i = 0; i < len; i++) {
        fire_area[i] = 0;
    }

    float fire1_r2 = fire_size * fire_size;
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            int a = i - size / 2;
            int b = j - size / 2;
            int r2 = 0.5 * a * a + 0.8 * b * b - 0.5 * a * b;
            if (r2 < fire1_r2) fire_area[i * size + j] = 1;
        }
    }
    float fire2_r2 = (fire_size / 2) * (fire_size / 2);
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            int a = i - 1 * size / 3;
            int b = j - 1 * size / 3;
            int r2 = a * a + b * b;
            if (r2 < fire2_r2) fire_area[i * size + j] = 1;
        }
    }
}

void update(float *data, float *new_data, int begin, int end) {
    // update the temperature of each point, and store the result in `new_data` to avoid data racing
    for (int i = begin; i < end; i++) {
        for (int j = 1; j < size - 1; j++) {
            int idx = i * size + j;
            float up = data[idx - size];
            float down = data[idx + size];
            float left = data[idx - 1];
            float right = data[idx + 1];
            float new_val = (up + down + left + right) / 4;
            new_data[idx] = new_val;
        }
    }
}

void maintain_fire(float *data, bool* fire_area, int begin, int end) {
    // maintain the temperature of fire
    for (int i = begin; i < end; i++) {
        for (int j = 0; j < size; j++) {
            int idx = i * size + j;
            if (fire_area[idx]) {
                data[idx] = fire_temp;
            } 
        }
    }
}

void maintain_wall(float *data) {
    // TODO: maintain the temperature of the wall
    for (int i = 0; i < size; i++) {
        data[i] = wall_temp;
        data[i * size] = wall_temp;
        data[i * size + size - 1] = wall_temp;
        data[size * size - 1 - i] = wall_temp;
    }
}

#ifdef GUI
void data2pixels(float *data, GLubyte* pixels) {
    // convert rawdata (large, size^2) to pixels (small, resolution^2) for faster rendering speed
    float factor_data_pixel = (float) size / resolution;
    float factor_temp_color = (float) 255 / fire_temp;
    for (int x = 0; x < resolution; x++){
        for (int y = 0; y < resolution; y++){
            int idx = x * resolution + y;
            int idx_pixel = idx * 3;
            int x_raw = x * factor_data_pixel;
            int y_raw = y * factor_data_pixel;
            int idx_raw = y_raw * size + x_raw;
            float temp = data[idx_raw];
            int color =  ((int) temp / 5 * 5) * factor_temp_color;
            pixels[idx_pixel] = color;
            pixels[idx_pixel + 1] = 255 - color;
            pixels[idx_pixel + 2] = 255 - color;
        }
    }
}
void plot(GLubyte* pixels) {
    // visualize temprature distribution
    #ifdef GUI
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(resolution, resolution, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glutSwapBuffers();
    #endif
}
#endif

typedef struct {

    int id_thread;
    int num_thread;
    float* data;
    float* new_data;
    bool* fire_area;

} Args;

void* worker(void* args) {

    Args* my_arg = (Args*) args;
    int id_thread = my_arg->id_thread;
    int num_thread = my_arg->num_thread;

    float* data = my_arg->data;
    float* new_data = my_arg->new_data;
    bool* fire_area = my_arg->fire_area;

    int num_my_row = size / num_thread;
    int remain = size % num_thread;
    if (id_thread < remain) num_my_row++;
    int displacement = num_my_row * id_thread;

    int begin = displacement;
    int end = displacement + num_my_row;
    if(begin == 0) begin++;
    if(end == size) end--;
    
    update(data, new_data, begin, end);
    maintain_fire(new_data, fire_area, begin, end);
    maintain_wall(new_data);
}

void master() {
    
    #ifdef GUI
    GLubyte* pixels = new GLubyte[resolution * resolution * 3];
    #endif

    float* data_odd = new float[size * size];
    float* data_even = new float[size * size];
    bool* fire_area = new bool[size * size];

    generate_fire_area(fire_area);
    initialize(data_odd);
    initialize(data_even);

    int count = 1;
    double total_time = 0;

    while (count <= max_iter) {
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        
        pthread_t thds[n_thd]; // thread pool
        Args args[n_thd]; // arguments for all threads
        
        if (count % 2 == 1) {
            for (int thd = 0; thd < n_thd; thd++) {
                args[thd].id_thread = thd;
                args[thd].num_thread = n_thd;
                args[thd].data = data_odd;
                args[thd].new_data = data_even;
                args[thd].fire_area = fire_area;
            }
        } 
        else {
            for (int thd = 0; thd < n_thd; thd++) {
                args[thd].id_thread = thd;
                args[thd].num_thread = n_thd;
                args[thd].data = data_even;
                args[thd].new_data = data_odd;
                args[thd].fire_area = fire_area;
            }
        }
        for (int thd = 0; thd < n_thd; thd++) pthread_create(&thds[thd], NULL, worker, &args[thd]);
        for (int thd = 0; thd < n_thd; thd++) pthread_join(thds[thd], NULL);
        
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        double this_time = std::chrono::duration<double>(t2 - t1).count();
        total_time += this_time;
        // printf("Iteration %d, elapsed time: %.6f\n", count, this_time);
        count++;
        
        #ifdef GUI
        if (count % 2 == 1) {
            data2pixels(data_even, pixels);
        } else {
            data2pixels(data_odd, pixels);
        }
        plot(pixels);
        #endif
        
    }
    printf("Converge after %d iterations, elapsed time: %.6f, average computation time: %.6f\n", count-1, total_time, (double) total_time / (count-1));
    
    delete[] data_odd;
    delete[] data_even;
    delete[] fire_area;
    
    #ifdef GUI
    delete[] pixels;
    #endif
  
}
int main(int argc, char* argv[]) {
    size = atoi(argv[1]);
    n_thd = atoi(argv[2]);
    
    #ifdef GUI
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(resolution, resolution);
    glutCreateWindow("Heat Distribution Simulation Pthread Implementation");
    gluOrtho2D(0, resolution, 0, resolution);
    #endif
    
    master();
    
    printf("Student ID: 119010221\n"); // replace it with your student id
    printf("Name: Haoyan Luo\n"); // replace it with your name
    printf("Assignment 4: Heat Distribution Pthread Implementation\n");
    return 0;
}

