#!/bin/bash
#SBATCH --job-name=mpi # Job name
#SBATCH --nodes=1                    # Run all processes on a single node	
#SBATCH --ntasks=4                   # number of processes = 40
#SBATCH --cpus-per-task=4      # Number of CPU cores allocated to each process (please use 1 here, in comparison with pthread)
#SBATCH --partition=Project            # Partition name: Project or Debug (Debug is default)

cd /nfsmnt/119010221/project3/
size=(50 100 200 400 800 1000 1600 3200)
echo "Thread = 4, Process = 4"
for i in "${size[@]}";
do
    echo "Size = $i"
    mpirun -np 4 ./openmpi $i 500 4
done