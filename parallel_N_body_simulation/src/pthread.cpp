#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <pthread.h>

#ifdef GUI
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "./headers/physics.h"
#include "./headers/logger.h"

int n_thd; // number of threads

int n_body;
int n_iteration;
std::chrono::high_resolution_clock::time_point start;
std::chrono::high_resolution_clock::time_point end;

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


void update_position(double *x, double *y, double *vx, double *vy, int n) {
    //TODO: update position 
    for (int i = 0; i < n; i++) {
        x[i] = x[i] + vx[i] * dt;
        y[i] = y[i] + vy[i] * dt;

        // bound collision check
        if (x[i] <= sqrt(radius2) || x[i] >= bound_x - sqrt(radius2)) vx[i] = -vx[i];
        if (y[i] <= sqrt(radius2) || y[i] >= bound_y - sqrt(radius2)) vy[i] = -vy[i];
    
    }
}


void update_velocity(double *m, double *x, double *y, double *vx, double *vy, int n) {
    
    double deltaX, deltaY, distance, acceleration;
    for (int i = 0; i < n; i++) {

        for (int j = 0; j < n_body; j++) {

            if (i == j) continue;

            deltaX = x[j] - x[i];
            deltaY = x[j] - y[i];
            distance = sqrt((deltaX * deltaX) + (deltaY * deltaY));

            if(distance <= 2 * sqrt(radius2)) {
                vx[i] = -vx[i];
                vy[i] = -vy[i];
                continue;
            }

            acceleration = gravity_const * m[j] / (distance * distance + err);
            vx[i] = vx[i] + dt * acceleration * deltaX/distance;
            vy[i] = vy[i] + dt * acceleration * deltaY/distance;
        }
    }
}


typedef struct {

    int id_thread;
    int num_thread;
    double* m;
    double* x;
    double* y;
    double* vx;
    double* vy;

} Args;


void* worker(void* args) {

    Args* my_arg = (Args*) args;
    int id_thread = my_arg->id_thread;
    int num_thread = my_arg->num_thread;

    int num_my_body = n_body / num_thread;
    int remain = n_body % num_thread;
    if (id_thread < remain) num_my_body++;
    int displacement = num_my_body * id_thread;
    
    double* local_m = my_arg->m + displacement;
    double* local_x = my_arg->x + displacement;
    double* local_y = my_arg->y + displacement;
    double* local_vx = my_arg->vx + displacement;
    double* local_vy = my_arg->vy + displacement;
    
    update_velocity(local_m, local_x, local_y, local_vx, local_vy, num_my_body);
    update_position(local_x, local_y, local_vx, local_vy, num_my_body);

}


void master(){
    double* m = new double[n_body];
    double* x = new double[n_body];
    double* y = new double[n_body];
    double* vx = new double[n_body];
    double* vy = new double[n_body];

    generate_data(m, x, y, vx, vy, n_body);

    // Logger l = Logger("pthread", n_body, bound_x, bound_y);

    for (int i = 0; i < n_iteration; i++){
        // std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        pthread_t thds[n_thd]; // thread pool
        Args args[n_thd]; // arguments for all threads
        for (int thd = 0; thd < n_thd; thd++){
            args[thd].id_thread = thd;
            args[thd].num_thread = n_thd;
            args[thd].m = m;
            args[thd].x = x;
            args[thd].y = y;
            args[thd].vx = vx;
            args[thd].vy = vy;
        }
        for (int thd = 0; thd < n_thd; thd++) pthread_create(&thds[thd], NULL, worker, &args[thd]);
        for (int thd = 0; thd < n_thd; thd++) pthread_join(thds[thd], NULL);

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

    delete[] m;
    delete[] x;
    delete[] y;
    delete[] vx;
    delete[] vy;


}


int main(int argc, char *argv[]) {
    n_body = atoi(argv[1]);
    n_iteration = atoi(argv[2]);
    n_thd = atoi(argv[3]);

    #ifdef GUI
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Pthread");
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, bound_x, 0, bound_y);
    #endif
    start = std::chrono::high_resolution_clock::now();
    master();
    end = std::chrono::high_resolution_clock::now();
    printf("Student ID: 119010221\n"); // replace it with your student id
    printf("Name: Haoyan Luo\n"); // replace it with your name
    printf("Assignment 3: N Body Simulation Pthread Implementation\n");
    std::chrono::duration<double> total_time = end - start;
    printf("Total time: %.3f\n", total_time);

	return 0;
}

