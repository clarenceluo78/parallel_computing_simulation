#include <cuda.h>
#include <cuda_runtime.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#ifdef GUI
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "./headers/physics.h"
#include "./headers/logger.h"


int block_size = 512;

std::chrono::high_resolution_clock::time_point start;
std::chrono::high_resolution_clock::time_point end;
int n_body;
int n_iteration;


__global__ void update_position(double *x, double *y, double *vx, double *vy, int n) {

    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i < n) {
        x[i] = x[i] + vx[i]*dt;
        y[i] = y[i] + vy[i]*dt;

        // bound collision check
        if (x[i] <= sqrt(radius2) || x[i] >= bound_x - sqrt(radius2)) vx[i] = -vx[i];
        if (y[i] <= sqrt(radius2) || y[i] >= bound_y - sqrt(radius2)) vy[i] = -vy[i];
    }
}

__global__ void update_velocity(double *m, double *x, double *y, double *vx, double *vy, int n) {
    
    int i = blockDim.x * blockIdx.x + threadIdx.x;

    if (i < n) {  
        double deltaX, deltaY, distance, acceleration;

        for (int j = 0; j < n; j++) {
            
            deltaX = x[j] - x[i];
            deltaY = y[j] - y[i];
            distance = sqrt((deltaX * deltaX) + (deltaY * deltaY));

            if (distance < 2 * sqrt(radius2)) {
                vx[i] = -vx[i];
                vy[i] = -vy[i];
                continue;
            }

            acceleration = gravity_const * m[j] / (distance * distance);
            vx[i] += acceleration * deltaX / distance * dt;
            vy[i] += acceleration * deltaY / distance * dt;
            
        }

    }
}


void generate_data(double *m, double *x,double *y,double *vx,double *vy, int n) {
    // TODO: Generate proper initial position and mass for better visualization
    srand((unsigned)time(NULL));
    for (int i = 0; i < n; i++) {
        m[i] = rand() % max_mass + 1.0f;
        x[i] = 1500.0f + rand() % (bound_x / 4);
        y[i] = 1500.0f + rand() % (bound_y / 4);
        vx[i] = 0.0f;
        vy[i] = 0.0f;
    }
}



void master() {
    double* m = new double[n_body];
    double* x = new double[n_body];
    double* y = new double[n_body];
    double* vx = new double[n_body];
    double* vy = new double[n_body];

    generate_data(m, x, y, vx, vy, n_body);

    // Logger l = Logger("cuda", n_body, bound_x, bound_y);

    double *device_m;
    double *device_x;
    double *device_y;
    double *device_vx;
    double *device_vy;

    cudaMalloc(&device_m, n_body * sizeof(double));
    cudaMalloc(&device_x, n_body * sizeof(double));
    cudaMalloc(&device_y, n_body * sizeof(double));
    cudaMalloc(&device_vx, n_body * sizeof(double));
    cudaMalloc(&device_vy, n_body * sizeof(double));

    cudaMemcpy(device_m, m, n_body * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(device_x, x, n_body * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(device_y, y, n_body * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(device_vx, vx, n_body * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(device_vy, vy, n_body * sizeof(double), cudaMemcpyHostToDevice);

    int n_block = n_body / block_size + 1;

    for (int i = 0; i < n_iteration; i++){
        // std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        update_velocity<<<n_block, block_size>>>(device_m, device_x, device_y, device_vx, device_vy, n_body);
        update_position<<<n_block, block_size>>>(device_x, device_y, device_vx, device_vy, n_body);

        cudaMemcpy(x, device_x, n_body * sizeof(double), cudaMemcpyDeviceToHost);
        cudaMemcpy(y, device_y, n_body * sizeof(double), cudaMemcpyDeviceToHost);

        // std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<double> time_span = t2 - t1;
        
        // printf("Iteration %d, elapsed time: %.3f\n", i, time_span);

        // l.save_frame(x, y);

        #ifdef GUI
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0f, 0.0f, 0.0f);
        glPointSize(2.0f);
        glBegin(GL_POINTS);
        double xi;
        double yi;
        for (int i = 0; i < n_body; i++){
            xi = x[i];
            yi = y[i];
            glVertex2f(xi, yi);
        }
        glEnd();
        glFlush();
        glutSwapBuffers();
        #else

        #endif

    }

    cudaFree(device_m);
    cudaFree(device_x);
    cudaFree(device_y);
    cudaFree(device_vx);
    cudaFree(device_vy);

    delete[] m;
    delete[] x;
    delete[] y;
    delete[] vx;
    delete[] vy;
    
}


int main(int argc, char *argv[]){
    
    n_body = atoi(argv[1]);
    n_iteration = atoi(argv[2]);

    #ifdef GUI
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(500, 500);
    glutCreateWindow("N Body Simulation CUDA Implementation");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gluOrtho2D(0, bound_x, 0, bound_y);
    #endif
    start = std::chrono::high_resolution_clock::now();
    master();
    end = std::chrono::high_resolution_clock::now();
    printf("Student ID: 119010221\n"); // replace it with your student id
    printf("Name: Haoyan Luo\n"); // replace it with your name
    printf("Assignment 3: N Body Simulation CUDA Implementation\n");
    std::chrono::duration<double> total_time = end - start;
    printf("Total time: %.3f\n", total_time);
    return 0;

}


