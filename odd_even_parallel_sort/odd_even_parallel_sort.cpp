#include <mpi.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <chrono>

void odd_even_sort_one_iter(int *arr, int start, int end, int is_odd, int *is_sorted);
void swap(int *arr, int i, int j);

int main (int argc, char **argv) {

    MPI_Init(&argc, &argv); 

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
    
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int num_elements; // number of elements to be sorted
    
    num_elements = atoi(argv[1]); // convert command line argument to num_elements

    int elements[num_elements]; // store elements
    int sorted_elements[num_elements]; // store sorted elements

    if (rank == 0) { // read inputs from file (master process)
        std::ifstream input(argv[2]);
        int element;
        int i = 0;
        while (input >> element) {
            elements[i] = element;
            i++;
        }
        std::cout << "actual number of elements:" << i << std::endl;
        // print the original array
        if (num_elements == 20) {
            std::cout << "The input 20-dim array:" << std::endl; 
            for (int i = 0; i < num_elements; i++) {
                std::cout << elements[i] << ' ';
            }
            std::cout << '\n';
    }
    }

    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;
    std::chrono::duration<double> time_span;
    if (rank == 0){ 
        t1 = std::chrono::high_resolution_clock::now(); // record time
    }
    
    /* Broadcast the original array */
    MPI_Bcast(elements, num_elements, MPI_INT, 0, MPI_COMM_WORLD);

    /* Task distribution */
    int num_my_element = 0;
    int sum = 0;
    int remainder = num_elements % world_size;
    int *element_count = (int*)malloc(sizeof(int) * world_size);
    int *displacements = (int*)malloc(sizeof(int)* world_size);

    // calculate the number of elements each process should handle
    for (int idx = 0; idx < world_size; idx++) {
        element_count[idx] = num_elements / world_size;
        if (remainder > 0) {
            element_count[idx]++;
            remainder--;
        }
        displacements[idx] = sum;
        sum += element_count[idx];
    }

    for (int idx = 0; idx < world_size; idx++) {
        if (rank == idx) {
            num_my_element = element_count[idx];
            // std::cout << "Rank " << rank << " has " << num_my_element << " elements" << std::endl;
        }
    }
    printf("rank %d: num_my_element = %d, displacements = %d\n", rank, element_count[rank], displacements[rank]);
    int* my_element = (int*)malloc(sizeof(int) *num_my_element);  // allocate memory for each process

    MPI_Scatterv(elements, element_count, displacements, MPI_INT, my_element, num_my_element, MPI_INT, 0, MPI_COMM_WORLD); // distribute elements to each process

    int is_sorted = 0;
    int send_element;
    int recv_element;
    for (int itr = 1; itr <= num_elements; itr++) {
        /* Sorting in each process */
        odd_even_sort_one_iter(my_element, 0, num_my_element - 1, itr % 2, &is_sorted);

        /* Swapping between processes */
        if (rank != 0) {
            send_element = my_element[0];
            MPI_Send(&send_element, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&recv_element, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (recv_element > my_element[0]) {
                my_element[0] = recv_element;
                is_sorted = 0;
            }
        }
        if (rank != world_size - 1) {
            send_element = my_element[num_my_element - 1];
            MPI_Send(&send_element, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&recv_element, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (recv_element < my_element[num_my_element - 1]) {
                my_element[num_my_element - 1] = recv_element;
                is_sorted = 0;
            }
        }
        if (is_sorted == 2) break;
    }

    /* Gather the sorted elements in each process */
    MPI_Allgatherv(my_element, num_my_element, MPI_INT, elements, element_count, displacements, MPI_INT, MPI_COMM_WORLD);
    // MPI_Gather(my_element, num_my_element, MPI_INT, sorted_elements, num_my_element, MPI_INT, 0, MPI_COMM_WORLD); // collect result from each process
    
    // move the sorted elements to a new array
    for (int i = 0; i < num_elements; i++) {
        sorted_elements[i] = elements[i];
    }

    if (rank == 0){ // record time (only executed in master process)
        t2 = std::chrono::high_resolution_clock::now();  
        time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        std::cout << "Student ID: " << "119010221" << std::endl; // replace it with your student id
        std::cout << "Name: " << "Haoyan Luo" << std::endl; // replace it with your name
        std::cout << "Assignment 1" << std::endl;
        std::cout << "Run Time: " << time_span.count() << " seconds" << std::endl;
        std::cout << "Input Size: " << num_elements << std::endl;
        std::cout << "Process Number: " << world_size << std::endl; 
    }

    if (rank == 0){ // write result to file (only executed in master process)
        std::ofstream output(argv[2]+std::string(".parallel.out"), std::ios_base::out);
        for (int i = 0; i < num_elements; i++) {
            output << sorted_elements[i] << std::endl;
        }
        // print the sorted array
        if (num_elements == 20) {
            std::cout << "The sorted 20-dim array:" << std::endl; 
            for (int i = 0; i < num_elements; i++) {
                std::cout << sorted_elements[i] << ' ';
            }
            std::cout << '\n';
        }
    }
    
    MPI_Finalize();
    
    return 0;
}

// swap two elements in an array
void swap(int *arr, int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

// one iteration of odd-even sort implementation
void odd_even_sort_one_iter(int *arr, int start, int end, int is_odd, int *is_sorted) {
  if (is_odd) {
    for (int i = start; i < end; i += 2) {
        if (arr[i] > arr[i + 1]) {
            swap(arr, i, i + 1);
            is_sorted = 0;
        } else {
            is_sorted++;
        }
    }
  } else {
    for (int j = start + 1; j < end; j += 2) {
        if (arr[j] > arr[j + 1]) {
            swap(arr, j, j + 1);
            is_sorted = 0;
        } else {
            is_sorted++;
        }
    }
  }
}
