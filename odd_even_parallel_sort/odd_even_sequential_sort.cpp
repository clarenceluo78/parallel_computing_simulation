#include <cstdlib>
#include <fstream>
#include <iostream>
#include <chrono>


void odd_even_sort_one_iter(int *arr, int start, int end, int is_odd, int *is_sorted);
void swap(int *arr, int i, int j);

int main (int argc, char **argv){

    int num_elements; // number of elements to be sorted
    num_elements = atoi(argv[1]); // convert command line argument to num_elements

    int elements[num_elements]; // store elements
    int sorted_elements[num_elements]; // store sorted elements

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

    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;
    std::chrono::duration<double> time_span;
    t1 = std::chrono::high_resolution_clock::now(); // record time

    int is_sorted = 0;  // 2 indicates unchanged for two consecutive iterations
    // odd even sort
    for (int i = 1; i <= num_elements; i++) {
        odd_even_sort_one_iter(elements, 0, num_elements - 1, i % 2, &is_sorted);
        if (is_sorted == 2) break;  // break the loop when the number's positions keep unchanged after two consecutive iterations
    }
    // move the sorted elements to a new array and print it out
    for (int i = 0; i < num_elements; i++) {
        sorted_elements[i] = elements[i];
    }

    t2 = std::chrono::high_resolution_clock::now();  
    time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << "Student ID: " << "119010221" << std::endl; // replace it with your student id
    std::cout << "Name: " << "Haoyan Luo" << std::endl; // replace it with your name
    std::cout << "Assignment 1" << std::endl;
    std::cout << "Run Time: " << time_span.count() << " seconds" << std::endl;
    std::cout << "Input Size: " << num_elements << std::endl;
    std::cout << "Process Number: " << 1 << std::endl;   
    
    std::ofstream output(argv[2]+std::string(".seq.out"), std::ios_base::out);
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

