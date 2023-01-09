#!/bin/bash
#SBATCH --job-name=pthread # Job name
#SBATCH --nodes=1                    # Run all processes on a single node	
#SBATCH --ntasks=1                   # number of processes = 1 
#SBATCH --cpus-per-task=40      # Number of CPU cores allocated to each process
#SBATCH --partition=Project            # Partition name: Project or Debug (Debug is default)

cd /nfsmnt/119010221/project3/
size=(50 100 200 400 800 1000 1600 3200 6400)
threads=(1 2 4 8 12 16 24 32 40)
for i in "${size[@]}";
do
    echo ">>>>>>>>>>>>> size = $i <<<<<<<<<<<<<<<"
    for j in "${threads[@]}";
    do
        ./pthread $i 500 $j
        echo "Thread: $j"
    done
done