#!/bin/bash
#SBATCH --job-name=seq # Job name
#SBATCH --nodes=1                    # Run all processes on a single node	
#SBATCH --ntasks=1                   # number of processes = 40
#SBATCH --cpus-per-task=1            # Number of CPU cores allocated to each process
#SBATCH --partition=Project            # Partition name: Project or Debug (Debug is default)

cd /nfsmnt/119010221/project3/
size=(50 100 200 400 800 1000 1600 3200 6400)
for i in "${size[@]}";
do  
    ./seq $i 500
    echo "Size = $i"
done