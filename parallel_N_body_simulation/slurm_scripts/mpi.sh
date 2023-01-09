#!/bin/bash
#SBATCH --job-name=mpi # Job name
#SBATCH --nodes=1                    # Run all processes on a single node	
#SBATCH --ntasks=40                   # number of processes = 40
#SBATCH --cpus-per-task=1      # Number of CPU cores allocated to each process (please use 1 here, in comparison with pthread)
#SBATCH --partition=Project            # Partition name: Project or Debug (Debug is default)

cd /nfsmnt/119010221/project3/
size=(50 100 200 400 800 1000 1600 3200 6400)
processes=(1 2 4 8 12 16 24 32 40)
for i in "${size[@]}";
do
    echo ">>>>>>>>>>>>> size = $i <<<<<<<<<<<<<<<"
    for j in "${processes[@]}";
    do
        echo "Process = $i"
        mpirun -np $j ./mpi $i 500
    done
done