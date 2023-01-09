#!/bin/bash
#SBATCH --job-name=mpi # Job name
#SBATCH --nodes=1                    # Run all processes on a single node	
#SBATCH --ntasks=40                   # number of processes = 40
#SBATCH --cpus-per-task=1            # Number of CPU cores allocated to each process
#SBATCH --partition=Project            # Partition name: Project or Debug (Debug is default)

cd /nfsmnt/119010221/project2/
size=(100 400 800 1000 2000 3000 4000 8000 10000)
for i in "${size[@]}";
do
    ./seq $i $i 100
done