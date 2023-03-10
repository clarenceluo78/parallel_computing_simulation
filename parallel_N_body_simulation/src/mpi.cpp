#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <mpi.h>

#ifdef GUI
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "./headers/physics.h"
#include "./headers/logger.h"


int n_body;
int n_iteration;
MPI_Request request;
std::chrono::high_resolution_clock::time_point start;
std::chrono::high_resolution_clock::time_point end;
int my_rank;
int world_size;


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
    for(int i = 0; i < n; i++) {
        x[i] += vx[i] * dt;
        y[i] += vy[i] * dt;

        // bound collision check
        if (x[i] <= sqrt(radius2) || x[i] >= bound_x - sqrt(radius2)) vx[i] = -vx[i];
        if (y[i] <= sqrt(radius2) || y[i] >= bound_y - sqrt(radius2)) vy[i] = -vy[i];
    }
}


void update_velocity(double *total_m, double *x, double *y, double *vx, double *vy, double *total_x, double *total_y, int n) {
    //TODO: calculate force and acceleration, update velocity
    double deltaX, deltaY, distance, acceleration;
    for (int i = 0; i < n; i++) {

        for (int j = 0; j < n_body; j++){

            if (i == j) continue;

            deltaX = total_x[j] - x[i];
            deltaY = total_y[j] - y[i];

            distance = sqrt((deltaX * deltaX) + (deltaY * deltaY));

            // body collision check
            if (distance <= 2 * sqrt(radius2)) {
                vx[i] = -vx[i];
                vy[i] = -vy[i];
                continue;
            }
            
            acceleration = gravity_const * total_m[j] / (distance * distance + err);
            vx[i] = vx[i] + dt * acceleration * deltaX/distance;
            vy[i] = vy[i] + dt * acceleration * deltaY/distance;
        }
    }
}


void slave() {
    // wait and get common data from root process
    double* total_m = new double[n_body];
    double* total_x = new double[n_body];
    double* total_y = new double[n_body];
    int *element_count = (int*)malloc(sizeof(int) * world_size);
    int *displacements = (int*)malloc(sizeof(int)* world_size);
	MPI_Ibcast(element_count, world_size, MPI_INT, 0, MPI_COMM_WORLD, &request);
	MPI_Ibcast(displacements, world_size, MPI_INT, 0, MPI_COMM_WORLD, &request);
    MPI_Ibcast(total_m, n_body, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);
    MPI_Ibcast(total_x, n_body, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);
    MPI_Ibcast(total_y, n_body, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, MPI_STATUS_IGNORE);
    
    // initialize local data
    int num_my_body = element_count[my_rank];
    double* local_x = new double[num_my_body];
    double* local_y = new double[num_my_body];
    double* local_vx = new double[num_my_body];
    double* local_vy = new double[num_my_body];

    for (int i = 0; i < n_iteration; i++) {
        // wait and update local data
        MPI_Iscatterv(NULL, NULL, NULL, MPI_DOUBLE, local_x, num_my_body, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);
        MPI_Iscatterv(NULL, NULL, NULL, MPI_DOUBLE, local_y, num_my_body, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);
        MPI_Iscatterv(NULL, NULL, NULL, MPI_DOUBLE, local_vx, num_my_body, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);
        MPI_Iscatterv(NULL, NULL, NULL, MPI_DOUBLE, local_vy, num_my_body, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);

        // perform calculation
        update_velocity(total_m, local_x, local_y, local_vx, local_vy, total_x, total_y, num_my_body);
        update_position(local_x, local_y, local_vx, local_vy, num_my_body);

        // wait and send local data
        MPI_Igatherv(local_x, num_my_body, MPI_DOUBLE, NULL, NULL, NULL, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);
        MPI_Igatherv(local_y, num_my_body, MPI_DOUBLE, NULL, NULL, NULL, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);
        MPI_Igatherv(local_vx, num_my_body, MPI_DOUBLE, NULL, NULL, NULL, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);
        MPI_Igatherv(local_vy, num_my_body, MPI_DOUBLE, NULL, NULL, NULL, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
    }
    
    
}



void master() {
    double* total_m = new double[n_body];
    double* total_x = new double[n_body];
    double* total_y = new double[n_body];
    double* total_vx = new double[n_body];
    double* total_vy = new double[n_body];

    // calculate the number of elements each process will receive
    int num_my_body = 0;
    int sum = 0;
    int remainder = n_body % world_size;
    int *body_count = (int*)malloc(sizeof(int) * world_size);
    int *displacements = (int*)malloc(sizeof(int)* world_size);
    for (int idx = 0; idx < world_size; idx++) {
        body_count[idx] = n_body / world_size;
        if (remainder > 0) {
            body_count[idx]++;
            remainder--;
        }
        displacements[idx] = sum;
        sum += body_count[idx];
    }

    // initialize states of bodies
    generate_data(total_m, total_x, total_y, total_vx, total_vy, n_body);

    // send common data to all processes
    MPI_Ibcast(body_count, world_size, MPI_INT, 0, MPI_COMM_WORLD, &request);
    MPI_Ibcast(displacements, world_size, MPI_INT, 0, MPI_COMM_WORLD, &request);
    MPI_Ibcast(total_m, n_body, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);
    MPI_Ibcast(total_x, n_body, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);
    MPI_Ibcast(total_y, n_body, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);

    // initialize local data
    num_my_body = body_count[my_rank];
    double *local_x = new double[num_my_body];
    double *local_y = new double[num_my_body];
    double *local_vx = new double[num_my_body];
    double *local_vy = new double[num_my_body];

    // Logger l = Logger("sequential", n_body, bound_x, bound_y);

    for (int i = 0; i < n_iteration; i++) {
        // std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

        // scatter data to all processes
        MPI_Iscatterv(total_x, body_count, displacements, MPI_DOUBLE, local_x, num_my_body, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);
        MPI_Iscatterv(total_y, body_count, displacements, MPI_DOUBLE, local_y, num_my_body, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);
        MPI_Iscatterv(total_vx, body_count, displacements, MPI_DOUBLE, local_vx, num_my_body, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);
        MPI_Iscatterv(total_vy, body_count, displacements, MPI_DOUBLE, local_vy, num_my_body, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);
        // MPI_Wait(&request, MPI_STATUS_IGNORE);

        // perform calculation
        update_velocity(total_m, local_x, local_y, local_vx, local_vy, total_x, total_y, num_my_body);
        update_position(local_x, local_y, local_vx, local_vy, num_my_body);

        // wait and get local data
        MPI_Igatherv(local_x, num_my_body, MPI_DOUBLE, total_x, body_count, displacements, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);
        MPI_Igatherv(local_y, num_my_body, MPI_DOUBLE, total_y, body_count, displacements, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);
        MPI_Igatherv(local_vx, num_my_body, MPI_DOUBLE, total_vx, body_count, displacements, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);
        MPI_Igatherv(local_vy, num_my_body, MPI_DOUBLE, total_vy, body_count, displacements, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);


        // std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<double> time_span = t2 - t1;

        // printf("Iteration %d, elapsed time: %.3f\n", i, time_span);

        // l.save_frame(total_x, total_y);

        #ifdef GUI
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0f, 0.0f, 0.0f);
        glPointSize(2.0f);
        glBegin(GL_POINTS);
        double xi;
        double yi;
        for (int i = 0; i < n_body; i++){
            xi = total_x[i];
            yi = total_y[i];
            glVertex2f(xi, yi);
        }
        glEnd();
        glFlush();
        glutSwapBuffers();
        #else

        #endif
    }

    delete[] total_m;
    delete[] total_x;
    delete[] total_y;
    delete[] total_vx;
    delete[] total_vy;

}


int main(int argc, char *argv[]) {
    n_body = atoi(argv[1]);
    n_iteration = atoi(argv[2]);

	MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	if (my_rank == 0) {
		#ifdef GUI
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
		glutInitWindowSize(500, 500); 
		glutInitWindowPosition(0, 0);
		glutCreateWindow("N Body Simulation MPI Implementation");
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glMatrixMode(GL_PROJECTION);
		gluOrtho2D(0, bound_x, 0, bound_y);
		#endif
        start = std::chrono::high_resolution_clock::now();
        master();
	} else {
        slave();
    }

	if (my_rank == 0){
        end = std::chrono::high_resolution_clock::now();
		printf("Student ID: 119010221\n"); // replace it with your student id
		printf("Name: Haoyan Luo\n"); // replace it with your name
		printf("Assignment 3: N Body Simulation MPI Implementation\n");
        std::chrono::duration<double> total_time = end - start;
        printf("Total time: %.3f\n", total_time);
	}

	MPI_Finalize();

	return 0;
}

