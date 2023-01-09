#include "asg2.h"
#include <stdio.h>
#include <mpi.h>


int rank;
int world_size;
MPI_Datatype PointType;
MPI_Request request;

void master();
void slave();

void master() {
	int num_my_element = 0;
    int sum = 0;
    int remainder = total_size % world_size;
	int *element_count = (int*)malloc(sizeof(int) * world_size);
    int *displacements = (int*)malloc(sizeof(int)* world_size);
	for (int idx = 0; idx < world_size; idx++) {
        element_count[idx] = total_size / world_size;
        if (remainder > 0) {
            element_count[idx]++;
            remainder--;
        }
        displacements[idx] = sum;
        sum += element_count[idx];
	}
	MPI_Ibcast(element_count, world_size, MPI_INT, 0, MPI_COMM_WORLD, &request);
	MPI_Ibcast(displacements, world_size, MPI_INT, 0, MPI_COMM_WORLD, &request);
	
	num_my_element = element_count[rank];
	// printf("rank %d: num_my_element = %d, displacements = %d\n", rank, num_my_element, displacements[rank]);

	Point my_element[num_my_element];
	Point* p = my_element;

	MPI_Iscatterv(data, element_count, displacements, PointType, my_element, num_my_element, PointType, 0, MPI_COMM_WORLD, &request);
	// printf("Process %d received %d values.\n", rank, my_element);
	MPI_Wait(&request, MPI_STATUS_IGNORE);
	for (int index = 0; index < num_my_element; index++) {
		compute(p);
		p++;
	}

	MPI_Igatherv(my_element, num_my_element, PointType, data, element_count, displacements, PointType, 0, MPI_COMM_WORLD, &request);
	MPI_Wait(&request, MPI_STATUS_IGNORE);
}

void slave() {
	int *element_count = (int*)malloc(sizeof(int) * world_size);
    int *displacements = (int*)malloc(sizeof(int)* world_size);
	MPI_Ibcast(element_count, world_size, MPI_INT, 0, MPI_COMM_WORLD, &request);
	MPI_Ibcast(displacements, world_size, MPI_INT, 0, MPI_COMM_WORLD, &request);
	MPI_Wait(&request, MPI_STATUS_IGNORE);
	int num_my_element = element_count[rank];
	// printf("rank %d: num_my_element = %d, displacements = %d\n", rank, num_my_element, displacements[rank]);
	
	Point my_element[num_my_element];
	Point* p = my_element;

	MPI_Iscatterv(NULL, NULL, NULL, PointType, my_element, num_my_element, PointType, 0, MPI_COMM_WORLD, &request);
	MPI_Wait(&request, MPI_STATUS_IGNORE);
	for (int index = 0; index < num_my_element; index++) {
		compute(p);
		p++;
	}

	MPI_Igatherv(my_element, num_my_element, PointType, NULL, NULL, NULL, PointType, 0, MPI_COMM_WORLD, &request);
	MPI_Wait(&request, MPI_STATUS_IGNORE);
}

int main(int argc, char *argv[]) {
	if ( argc == 4 ) {
		X_RESN = atoi(argv[1]);
		Y_RESN = atoi(argv[2]);
		max_iteration = atoi(argv[3]);
	} else {
		X_RESN = 1000;
		Y_RESN = 1000;
		max_iteration = 100;
	}

	if (rank == 0) {
		#ifdef GUI
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
		glutInitWindowSize(500, 500); 
		glutInitWindowPosition(0, 0);
		glutCreateWindow("MPI");
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glMatrixMode(GL_PROJECTION);
		gluOrtho2D(0, X_RESN, 0, Y_RESN);
		glutDisplayFunc(plot);
		#endif
	}

	/* computation part begin */
	MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	// define MPI data structure
	int lengths[3] = { 1, 1, 1 };
	MPI_Aint displacements[3];
	Point dummy_point;
	MPI_Aint base_address;
    MPI_Get_address(&dummy_point, &base_address);
    MPI_Get_address(&dummy_point.x, &displacements[0]);
    MPI_Get_address(&dummy_point.y, &displacements[1]);
    MPI_Get_address(&dummy_point.color, &displacements[2]);
	displacements[0] = MPI_Aint_diff(displacements[0], base_address);
    displacements[1] = MPI_Aint_diff(displacements[1], base_address);
    displacements[2] = MPI_Aint_diff(displacements[2], base_address);

	MPI_Datatype types[3] = { MPI_INT, MPI_INT, MPI_FLOAT };
    MPI_Type_create_struct(3, lengths, displacements, types, &PointType);
    MPI_Type_commit(&PointType);

	if (rank == 0){
		initData();
		t1 = std::chrono::high_resolution_clock::now();
	}

	if (rank == 0) {
		master();
	} else {
		slave();
	}
	
	if (rank == 0){
		t2 = std::chrono::high_resolution_clock::now();  
		time_span = t2 - t1;
	}

	if (rank == 0){
		printf("Student ID: 119010221\n"); // replace it with your student id
		printf("Name: Haoyan Luo\n"); // replace it with your name
		printf("Assignment 2 MPI\n");
		printf("Run Time: %f seconds\n", time_span.count());
		printf("Problem Size: %d * %d, %d\n", X_RESN, Y_RESN, max_iteration);
		printf("Process Number: %d\n", world_size);
	}

	MPI_Finalize();
	/* computation part end */

	if (rank == 0){
		#ifdef GUI
		glutMainLoop();
		#endif
	}

	return 0;
}

