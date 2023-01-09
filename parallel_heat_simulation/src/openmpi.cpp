#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <mpi.h>
#include <omp.h>

#ifdef GUI
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "./headers/physics.h"

int size; // problem size
int my_rank;
int world_size;
int local_size;
int n_omp_threads;
MPI_Request request;

void initialize(float *data) {
    // intialize the temperature distribution
    int len = size * size;
    for (int i = 0; i < len; i++) {
        data[i] = wall_temp;
    }
}

void generate_fire_area(bool *fire_area){
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
    // TODO: update the temperature of each point, and store the result in `new_data` to avoid data racing
    omp_set_num_threads(n_omp_threads);
    #pragma omp parallel for
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

void maintain_fire(float *data, bool* fire_area) {
    // TODO: maintain the temperature of fire
    int len = local_size * size;
    omp_set_num_threads(n_omp_threads);
    #pragma omp parallel for
    for (int i = 0; i < len; i++) {
        if (fire_area[i]) data[i] = fire_temp;
    }
}

void maintain_wall(float *data) {
    // TODO: maintain the temperature of the wall
    omp_set_num_threads(n_omp_threads);
    #pragma omp parallel for
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
    for (int x = 0; x < resolution; x++) {
        for (int y = 0; y < resolution; y++) {
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

void plot(GLubyte* pixels){
    // visualize temprature distribution
    #ifdef GUI
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(resolution, resolution, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glutSwapBuffers();
    #endif
}
#endif

void slave(){
    
    int *row_count = (int*)malloc(sizeof(int) * world_size);
    int *row_displacements = (int*)malloc(sizeof(int)* world_size);
    MPI_Ibcast(row_count, world_size, MPI_INT, 0, MPI_COMM_WORLD, &request);
    MPI_Ibcast(row_displacements, world_size, MPI_INT, 0, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, MPI_STATUS_IGNORE);
    
    local_size = row_count[my_rank];  // get local size

    int sendbuffer[world_size];
    int sendbuffer_displacements[world_size];
    MPI_Ibcast(sendbuffer, world_size, MPI_INT, 0, MPI_COMM_WORLD, &request);
    MPI_Ibcast(sendbuffer_displacements, world_size, MPI_INT, 0, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, MPI_STATUS_IGNORE);
    
    float* local_data = new float[local_size * size];
    float* global_data;
    float* input_data = new float[sendbuffer[my_rank]];
    float* output_data = new float[sendbuffer[my_rank]];
    bool* local_fire_area = new bool[local_size * size];
    bool* fire_area;
    
    int count = 1;

    // omp_set_num_threads(n_omp_threads);
    // #pragma omp parallel for
    for (int i = 0; i < max_iter; i++) {
        
        int begin = 1;
        int end = row_count[my_rank];

        // distribute data and calculate temperature
        MPI_Scatterv(global_data, sendbuffer, sendbuffer_displacements, MPI_FLOAT, input_data, sendbuffer[my_rank], MPI_FLOAT, 0, MPI_COMM_WORLD);
        // update the temperature of each point, don't need to update the neighborhood buffer
        update(input_data, output_data, begin, end);
        float * output_data_shifted = &output_data[size];
        if(my_rank == 0) output_data_shifted = &output_data[0];

        MPI_Gather(output_data_shifted, local_size*size, MPI_FLOAT, global_data, local_size*size, MPI_FLOAT, 0, MPI_COMM_WORLD);

        // distribute data and maintain fire     
        MPI_Scatter(global_data, local_size*size, MPI_FLOAT, local_data, local_size*size, MPI_FLOAT, 0, MPI_COMM_WORLD);
        MPI_Scatter(fire_area, local_size*size, MPI_CHAR, local_fire_area, local_size*size, MPI_CHAR, 0, MPI_COMM_WORLD);
        
        maintain_fire(local_data, local_fire_area);
        
        MPI_Gather(local_data, local_size*size, MPI_FLOAT, global_data, local_size*size, MPI_FLOAT, 0, MPI_COMM_WORLD);
        
        count++;
    }

    // TODO: Remember to delete[] local_data and local_pixcels.
    delete[] input_data;
    delete[] output_data;
    delete[] local_data;
    delete[] local_fire_area;
}

void master() {
    #ifdef GUI
    GLubyte* pixels;
    pixels = new GLubyte[resolution * resolution * 3];
    #endif

    // TODO: MPI routine (one possible solution, you can use another partition method)
    // calculate the number of rows each process will handle
    int row_sum = 0;
    int row_remainder = size % world_size;
    int *row_count = (int*)malloc(sizeof(int) * world_size);
    int *row_displacements = (int*)malloc(sizeof(int)* world_size);
    for (int idx = 0; idx < world_size; idx++) {
        row_count[idx] = size / world_size;
        if (row_remainder > 0) {
            row_count[idx]++;
            row_remainder--;
        }
        row_displacements[idx] = row_sum;
        row_sum += row_count[idx];
    }
    MPI_Ibcast(row_count, world_size, MPI_INT, 0, MPI_COMM_WORLD, &request);
    MPI_Ibcast(row_displacements, world_size, MPI_INT, 0, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, MPI_STATUS_IGNORE);
    
    local_size = row_count[my_rank];
    
    // calculate the number of data points each process will handle
    int sendbuffer_sum = 0;
    int sendbuffer[world_size];
    int sendbuffer_displacements[world_size];
    // allocate memory for two neighborhood rows
    for(int i = 1; i < world_size - 1; i++) {
        sendbuffer[i] = (local_size + 2) * size;
    }
    sendbuffer[0] = (local_size + 1) * size;
    sendbuffer[world_size - 1] = (local_size + 1) * size;
    // calculate data point displacements
    for(int i = 1; i < world_size; i++) {
        sendbuffer_displacements[i] = row_displacements[i] * size - size;
    }
    sendbuffer_displacements[0] = 0;

    MPI_Ibcast(sendbuffer, world_size, MPI_INT, 0, MPI_COMM_WORLD, &request);
    MPI_Ibcast(sendbuffer_displacements, world_size, MPI_INT, 0, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, MPI_STATUS_IGNORE);

    // use global data to record the overall states (and for visualization)
    float* global_data = new float[size * size];
    bool* fire_area = new bool[size * size];
    float* input_data = new float[sendbuffer[my_rank]];
    float* output_data = new float[sendbuffer[my_rank]];
    float* local_data = new float[local_size * size];
    bool* local_fire_area = new bool[local_size * size];

    initialize(global_data);
    generate_fire_area(fire_area);

    int count = 1;
    double total_time = 0;

    for (int i = 0; i < max_iter; i++) {
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        
        int begin = 1;
        int end = row_count[my_rank];

        // distribute data and calculate temperature
        MPI_Scatterv(global_data, sendbuffer, sendbuffer_displacements ,MPI_FLOAT, input_data, sendbuffer[my_rank], MPI_FLOAT, 0, MPI_COMM_WORLD);
        update(input_data, output_data, begin, end);
        // when gathering the data, the first row of each process is not needed
        float* output_data_shifted = &output_data[size];
        if(my_rank == 0) output_data_shifted = &output_data[0];
        MPI_Gather(output_data_shifted, local_size * size, MPI_FLOAT, global_data, local_size * size, MPI_FLOAT, 0, MPI_COMM_WORLD);

        // distribute data and maintain fire
        MPI_Scatter(global_data, local_size*size, MPI_FLOAT, local_data, local_size * size, MPI_FLOAT, 0, MPI_COMM_WORLD);
        MPI_Scatter(fire_area, local_size * size, MPI_CHAR, local_fire_area, local_size*size, MPI_CHAR, 0, MPI_COMM_WORLD);
        maintain_fire(local_data, local_fire_area);
        MPI_Gather(local_data, local_size*size, MPI_FLOAT, global_data, local_size*size, MPI_FLOAT, 0, MPI_COMM_WORLD);
        
        // maintain wall in root process
        maintain_wall(global_data);
        
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        double this_time = std::chrono::duration<double>(t2 - t1).count();
        total_time += this_time;
        printf("Iteration %d, elapsed time: %.6f\n", count, this_time);
        
        count++;
        
        #ifdef GUI
        if (count % 2 == 1) {
            // TODO: Gather pixels of slave processes
            data2pixels(global_data, pixels);
        } else {
            // TODO: Gather pixels of slave processes
            data2pixels(global_data, pixels);
        }
        plot(pixels);
        #endif
    }
    printf("Converge after %d iterations, elapsed time: %.6f, average computation time: %.6f\n", count-1, total_time, (double) total_time / (count-1));
    
    delete[] global_data;
    delete[] input_data;
    delete[] output_data;
    delete[] fire_area;
    delete[] local_data;
    delete[] local_fire_area;
    
    #ifdef GUI
    delete[] pixels;
    #endif
}
int main(int argc, char *argv[]) {
    size = atoi(argv[1]);
    n_omp_threads = atoi(argv[2]);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
    if (my_rank == 0) {
        #ifdef GUI
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
        glutInitWindowPosition(0, 0);
        glutInitWindowSize(resolution, resolution);
        glutCreateWindow("Heat Distribution Simulation OPenMP + MPI Implementation");
        gluOrtho2D(0, resolution, 0, resolution);
        #endif
        master();
	} else {
        slave();
    }
	if (my_rank == 0){
        printf("Student ID: 119010221\n"); // replace it with your student id
        printf("Name: Haoyan Luo\n"); // replace it with your name
        printf("Assignment 4: Heat Distribution OPenMP + MPI Implementation\n");
	}
	MPI_Finalize();
	return 0;
}