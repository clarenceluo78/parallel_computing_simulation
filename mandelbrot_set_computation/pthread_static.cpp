#include "asg2.h"
#include <stdio.h>
#include <pthread.h>


int n_thd; // number of threads
pthread_mutex_t mutex;

typedef struct {
    //TODO: specify your arguments for threads
    int id_thread;
    int num_thread;
    //TODO END
} Args;


void* worker(void* args) {
    //TODO: procedure in each threads
    // the code following is not necessary, you can replace it.
    Args* my_arg = (Args*) args;
    int id_thread = my_arg->id_thread;
    int num_thread = my_arg->num_thread;

    int num_my_element = total_size / num_thread;
    int remain = total_size % num_thread;
    if (id_thread < remain) num_my_element++;
    int displacement = num_my_element * id_thread;
    Point* p = data + displacement;
    for (int index = 0; index < num_my_element; index++) {
        compute(p);
        p++;
    }
    //TODO END

}


int main(int argc, char *argv[]) {

	if ( argc == 5 ) {
		X_RESN = atoi(argv[1]);
		Y_RESN = atoi(argv[2]);
		max_iteration = atoi(argv[3]);
        n_thd = atoi(argv[4]);
	} else {
		X_RESN = 1000;
		Y_RESN = 1000;
		max_iteration = 100;
        n_thd = 4;
	}

    #ifdef GUI
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Pthread");
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, X_RESN, 0, Y_RESN);
	glutDisplayFunc(plot);
    #endif

    /* computation part begin */
    t1 = std::chrono::high_resolution_clock::now();

    initData();


    //TODO: assign jobs
    pthread_t thds[n_thd]; // thread pool
    Args args[n_thd]; // arguments for all threads
    for (int thd = 0; thd < n_thd; thd++){
        args[thd].id_thread = thd;
        args[thd].num_thread = n_thd;
    }
    for (int thd = 0; thd < n_thd; thd++) pthread_create(&thds[thd], NULL, worker, &args[thd]);
    for (int thd = 0; thd < n_thd; thd++) pthread_join(thds[thd], NULL);
    //TODO END

    t2 = std::chrono::high_resolution_clock::now();  
    time_span = t2 - t1;
    /* computation part end */

    printf("Student ID: 119010221\n"); // replace it with your student id
    printf("Name: Haoyan Luo\n"); // replace it with your name
    printf("Assignment 2 Pthread\n");
    printf("Run Time: %f seconds\n", time_span.count());
    printf("Problem Size: %d * %d, %d\n", X_RESN, Y_RESN, max_iteration);
    printf("Thread Number: %d\n", n_thd);

    #ifdef GUI
	glutMainLoop();
    #endif

	return 0;
}

