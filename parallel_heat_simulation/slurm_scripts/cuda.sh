#!/bin/bash

#SBATCH --job-name cuda  ## Job name
#SBATCH --gres=gpu:1                ## Number of GPUs required for job execution.
#SBATCH --output cuda.out         ## filename of the output
#SBATCH --partition=Project           ## the partitions to run in (Debug or Project)
#SBATCH --ntasks=1                  ## number of tasks (analyses) to run
#SBATCH --gpus-per-task=1           ## number of gpus per task
#SBATCH --time=0-00:02:00           ## time for analysis (day-hour:min:sec)

## Compile the cuda script using the nvcc compiler
## You can compile your codes out of the script and simply srun the executable file.
cd /nfsmnt/119010221/project4/
size=(100 200 400 800 1000 1600 3200)
for i in "${size[@]}";
do
    echo "Size = $i"
    ./cuda $i
    
done