#!/bin/bash
#SBATCH --job-name=par_sort # Job name
#SBATCH --nodes=1                    # Run all processes on a single node	
#SBATCH --ntasks=20                   # number of processes = 20
#SBATCH --cpus-per-task=1            # Number of CPU cores allocated to each process
#SBATCH --partition=Project            # Partition name: Project or Debug (Debug is default)

cd /nfsmnt/119010221/project1/
# 20
mpirun -np 2 ./psort 20 ./test_data/20a.in
./check 20 ./test_data/20a.in.parallel.out
mpirun -np 3 ./psort 20 ./test_data/20a.in
./check 20 ./test_data/20a.in.parallel.out
mpirun -np 4 ./psort 20 ./test_data/20a.in
./check 20 ./test_data/20a.in.parallel.out
mpirun -np 5 ./psort 20 ./test_data/20a.in
./check 20 ./test_data/20a.in.parallel.out
mpirun -np 6 ./psort 20 ./test_data/20a.in
./check 20 ./test_data/20a.in.parallel.out
mpirun -np 7 ./psort 20 ./test_data/20a.in
./check 20 ./test_data/20a.in.parallel.out
mpirun -np 8 ./psort 20 ./test_data/20a.in
./check 20 ./test_data/20a.in.parallel.out
mpirun -np 9 ./psort 20 ./test_data/20a.in
./check 20 ./test_data/20a.in.parallel.out
mpirun -np 10 ./psort 20 ./test_data/20a.in
./check 20 ./test_data/20a.in.parallel.out
mpirun -np 11 ./psort 20 ./test_data/20a.in
./check 20 ./test_data/20a.in.parallel.out
mpirun -np 12 ./psort 20 ./test_data/20a.in
./check 20 ./test_data/20a.in.parallel.out
mpirun -np 13 ./psort 20 ./test_data/20a.in
./check 20 ./test_data/20a.in.parallel.out
mpirun -np 14 ./psort 20 ./test_data/20a.in
./check 20 ./test_data/20a.in.parallel.out
mpirun -np 15 ./psort 20 ./test_data/20a.in
./check 20 ./test_data/20a.in.parallel.out
mpirun -np 16 ./psort 20 ./test_data/20a.in
./check 20 ./test_data/20a.in.parallel.out
mpirun -np 17 ./psort 20 ./test_data/20a.in
./check 20 ./test_data/20a.in.parallel.out
mpirun -np 18 ./psort 20 ./test_data/20a.in
./check 20 ./test_data/20a.in.parallel.out
mpirun -np 19 ./psort 20 ./test_data/20a.in
./check 20 ./test_data/20a.in.parallel.out
mpirun -np 20 ./psort 20 ./test_data/20a.in
./check 20 ./test_data/20a.in.parallel.out
# 100
mpirun -np 2 ./psort 100 ./test_data/100a.in
./check 100 ./test_data/100a.in.parallel.out
mpirun -np 3 ./psort 100 ./test_data/100a.in
./check 100 ./test_data/100a.in.parallel.out
mpirun -np 4 ./psort 100 ./test_data/100a.in
./check 100 ./test_data/100a.in.parallel.out
mpirun -np 5 ./psort 100 ./test_data/100a.in
./check 100 ./test_data/100a.in.parallel.out
mpirun -np 6 ./psort 100 ./test_data/100a.in
./check 100 ./test_data/100a.in.parallel.out
mpirun -np 7 ./psort 100 ./test_data/100a.in
./check 100 ./test_data/100a.in.parallel.out
mpirun -np 8 ./psort 100 ./test_data/100a.in
./check 100 ./test_data/100a.in.parallel.out
mpirun -np 9 ./psort 100 ./test_data/100a.in
./check 100 ./test_data/100a.in.parallel.out
mpirun -np 10 ./psort 100 ./test_data/100a.in
./check 100 ./test_data/100a.in.parallel.out
mpirun -np 11 ./psort 100 ./test_data/100a.in
./check 100 ./test_data/100a.in.parallel.out
mpirun -np 12 ./psort 100 ./test_data/100a.in
./check 100 ./test_data/100a.in.parallel.out
mpirun -np 13 ./psort 100 ./test_data/100a.in
./check 100 ./test_data/100a.in.parallel.out
mpirun -np 14 ./psort 100 ./test_data/100a.in
./check 100 ./test_data/100a.in.parallel.out
mpirun -np 15 ./psort 100 ./test_data/100a.in
./check 100 ./test_data/100a.in.parallel.out
mpirun -np 16 ./psort 100 ./test_data/100a.in
./check 100 ./test_data/100a.in.parallel.out
mpirun -np 17 ./psort 100 ./test_data/100a.in
./check 100 ./test_data/100a.in.parallel.out
mpirun -np 18 ./psort 100 ./test_data/100a.in
./check 100 ./test_data/100a.in.parallel.out
mpirun -np 19 ./psort 100 ./test_data/100a.in
./check 100 ./test_data/100a.in.parallel.out
mpirun -np 20 ./psort 100 ./test_data/100a.in
./check 100 ./test_data/100a.in.parallel.out
# 1000
mpirun -np 2 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 3 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 4 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 5 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 6 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 7 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 8 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 9 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 10 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 11 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 12 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 13 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 14 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 15 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 16 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 17 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 18 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 19 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 20 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
# 500
mpirun -np 2 ./psort 500 ./test_data/500a.in
./check 500 ./test_data/500a.in.parallel.out
mpirun -np 3 ./psort 500 ./test_data/500a.in
./check 500 ./test_data/500a.in.parallel.out
mpirun -np 4 ./psort 500 ./test_data/500a.in
./check 500 ./test_data/500a.in.parallel.out
mpirun -np 5 ./psort 500 ./test_data/500a.in
./check 500 ./test_data/500a.in.parallel.out
mpirun -np 6 ./psort 500 ./test_data/500a.in
./check 500 ./test_data/500a.in.parallel.out
mpirun -np 7 ./psort 500 ./test_data/500a.in
./check 500 ./test_data/500a.in.parallel.out
mpirun -np 8 ./psort 500 ./test_data/500a.in
./check 500 ./test_data/500a.in.parallel.out
mpirun -np 9 ./psort 500 ./test_data/500a.in
./check 500 ./test_data/500a.in.parallel.out
mpirun -np 10 ./psort 500 ./test_data/500a.in
./check 500 ./test_data/500a.in.parallel.out
mpirun -np 11 ./psort 500 ./test_data/500a.in
./check 500 ./test_data/500a.in.parallel.out
mpirun -np 12 ./psort 500 ./test_data/500a.in
./check 500 ./test_data/500a.in.parallel.out
mpirun -np 13 ./psort 500 ./test_data/500a.in
./check 500 ./test_data/500a.in.parallel.out
mpirun -np 14 ./psort 500 ./test_data/500a.in
./check 500 ./test_data/500a.in.parallel.out
mpirun -np 15 ./psort 500 ./test_data/500a.in
./check 500 ./test_data/500a.in.parallel.out
mpirun -np 16 ./psort 500 ./test_data/500a.in
./check 500 ./test_data/500a.in.parallel.out
mpirun -np 17 ./psort 500 ./test_data/500a.in
./check 500 ./test_data/500a.in.parallel.out
mpirun -np 18 ./psort 500 ./test_data/500a.in
./check 500 ./test_data/500a.in.parallel.out
mpirun -np 19 ./psort 500 ./test_data/500a.in
./check 500 ./test_data/500a.in.parallel.out
mpirun -np 20 ./psort 500 ./test_data/500a.in
./check 500 ./test_data/500a.in.parallel.out
# 1000
mpirun -np 2 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 3 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 4 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 5 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 6 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 7 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 8 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 9 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 10 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 11 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 12 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 13 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 14 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 15 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 16 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 17 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 18 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 19 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out
mpirun -np 20 ./psort 1000 ./test_data/1000a.in
./check 1000 ./test_data/1000a.in.parallel.out

# 2000
mpirun -np 2 ./psort 2000 ./test_data/2000a.in
./check 2000 ./test_data/2000a.in.parallel.out
mpirun -np 3 ./psort 2000 ./test_data/2000a.in
./check 2000 ./test_data/2000a.in.parallel.out
mpirun -np 4 ./psort 2000 ./test_data/2000a.in
./check 2000 ./test_data/2000a.in.parallel.out
mpirun -np 5 ./psort 2000 ./test_data/2000a.in
./check 2000 ./test_data/2000a.in.parallel.out
mpirun -np 6 ./psort 2000 ./test_data/2000a.in
./check 2000 ./test_data/2000a.in.parallel.out
mpirun -np 7 ./psort 2000 ./test_data/2000a.in
./check 2000 ./test_data/2000a.in.parallel.out
mpirun -np 8 ./psort 2000 ./test_data/2000a.in
./check 2000 ./test_data/2000a.in.parallel.out
mpirun -np 9 ./psort 2000 ./test_data/2000a.in
./check 2000 ./test_data/2000a.in.parallel.out
mpirun -np 10 ./psort 2000 ./test_data/2000a.in
./check 2000 ./test_data/2000a.in.parallel.out
mpirun -np 11 ./psort 2000 ./test_data/2000a.in
./check 2000 ./test_data/2000a.in.parallel.out
mpirun -np 12 ./psort 2000 ./test_data/2000a.in
./check 2000 ./test_data/2000a.in.parallel.out
mpirun -np 13 ./psort 2000 ./test_data/2000a.in
./check 2000 ./test_data/2000a.in.parallel.out
mpirun -np 14 ./psort 2000 ./test_data/2000a.in
./check 2000 ./test_data/2000a.in.parallel.out
mpirun -np 15 ./psort 2000 ./test_data/2000a.in
./check 2000 ./test_data/2000a.in.parallel.out
mpirun -np 16 ./psort 2000 ./test_data/2000a.in
./check 2000 ./test_data/2000a.in.parallel.out
mpirun -np 17 ./psort 2000 ./test_data/2000a.in
./check 2000 ./test_data/2000a.in.parallel.out
mpirun -np 18 ./psort 2000 ./test_data/2000a.in
./check 2000 ./test_data/2000a.in.parallel.out
mpirun -np 19 ./psort 2000 ./test_data/2000a.in
./check 2000 ./test_data/2000a.in.parallel.out
mpirun -np 20 ./psort 2000 ./test_data/2000a.in
./check 2000 ./test_data/2000a.in.parallel.out
# 5000
mpirun -np 2 ./psort 5000 ./test_data/5000a.in
./check 5000 ./test_data/5000a.in.parallel.out
mpirun -np 3 ./psort 5000 ./test_data/5000a.in
./check 5000 ./test_data/5000a.in.parallel.out
mpirun -np 4 ./psort 5000 ./test_data/5000a.in
./check 5000 ./test_data/5000a.in.parallel.out
mpirun -np 5 ./psort 5000 ./test_data/5000a.in
./check 5000 ./test_data/5000a.in.parallel.out
mpirun -np 6 ./psort 5000 ./test_data/5000a.in
./check 5000 ./test_data/5000a.in.parallel.out
mpirun -np 7 ./psort 5000 ./test_data/5000a.in
./check 5000 ./test_data/5000a.in.parallel.out
mpirun -np 8 ./psort 5000 ./test_data/5000a.in
./check 5000 ./test_data/5000a.in.parallel.out
mpirun -np 9 ./psort 5000 ./test_data/5000a.in
./check 5000 ./test_data/5000a.in.parallel.out
mpirun -np 10 ./psort 5000 ./test_data/5000a.in
./check 5000 ./test_data/5000a.in.parallel.out
mpirun -np 11 ./psort 5000 ./test_data/5000a.in
./check 5000 ./test_data/5000a.in.parallel.out
mpirun -np 12 ./psort 5000 ./test_data/5000a.in
./check 5000 ./test_data/5000a.in.parallel.out
mpirun -np 13 ./psort 5000 ./test_data/5000a.in
./check 5000 ./test_data/5000a.in.parallel.out
mpirun -np 14 ./psort 5000 ./test_data/5000a.in
./check 5000 ./test_data/5000a.in.parallel.out
mpirun -np 15 ./psort 5000 ./test_data/5000a.in
./check 5000 ./test_data/5000a.in.parallel.out
mpirun -np 16 ./psort 5000 ./test_data/5000a.in
./check 5000 ./test_data/5000a.in.parallel.out
mpirun -np 17 ./psort 5000 ./test_data/5000a.in
./check 5000 ./test_data/5000a.in.parallel.out
mpirun -np 18 ./psort 5000 ./test_data/5000a.in
./check 5000 ./test_data/5000a.in.parallel.out
mpirun -np 19 ./psort 5000 ./test_data/5000a.in
./check 5000 ./test_data/5000a.in.parallel.out
mpirun -np 20 ./psort 5000 ./test_data/5000a.in
./check 5000 ./test_data/5000a.in.parallel.out
# 8000
mpirun -np 2 ./psort 8000 ./test_data/8000a.in
./check 8000 ./test_data/8000a.in.parallel.out
mpirun -np 3 ./psort 8000 ./test_data/8000a.in
./check 8000 ./test_data/8000a.in.parallel.out
mpirun -np 4 ./psort 8000 ./test_data/8000a.in
./check 8000 ./test_data/8000a.in.parallel.out
mpirun -np 5 ./psort 8000 ./test_data/8000a.in
./check 8000 ./test_data/8000a.in.parallel.out
mpirun -np 6 ./psort 8000 ./test_data/8000a.in
./check 8000 ./test_data/8000a.in.parallel.out
mpirun -np 7 ./psort 8000 ./test_data/8000a.in
./check 8000 ./test_data/8000a.in.parallel.out
mpirun -np 8 ./psort 8000 ./test_data/8000a.in
./check 8000 ./test_data/8000a.in.parallel.out
mpirun -np 9 ./psort 8000 ./test_data/8000a.in
./check 8000 ./test_data/8000a.in.parallel.out
mpirun -np 10 ./psort 8000 ./test_data/8000a.in
./check 8000 ./test_data/8000a.in.parallel.out
mpirun -np 11 ./psort 8000 ./test_data/8000a.in
./check 8000 ./test_data/8000a.in.parallel.out
mpirun -np 12 ./psort 8000 ./test_data/8000a.in
./check 8000 ./test_data/8000a.in.parallel.out
mpirun -np 13 ./psort 8000 ./test_data/8000a.in
./check 8000 ./test_data/8000a.in.parallel.out
mpirun -np 14 ./psort 8000 ./test_data/8000a.in
./check 8000 ./test_data/8000a.in.parallel.out
mpirun -np 15 ./psort 8000 ./test_data/8000a.in
./check 8000 ./test_data/8000a.in.parallel.out
mpirun -np 16 ./psort 8000 ./test_data/8000a.in
./check 8000 ./test_data/8000a.in.parallel.out
mpirun -np 17 ./psort 8000 ./test_data/8000a.in
./check 8000 ./test_data/8000a.in.parallel.out
mpirun -np 18 ./psort 8000 ./test_data/8000a.in
./check 8000 ./test_data/8000a.in.parallel.out
mpirun -np 19 ./psort 8000 ./test_data/8000a.in
./check 8000 ./test_data/8000a.in.parallel.out
mpirun -np 20 ./psort 8000 ./test_data/8000a.in
./check 8000 ./test_data/8000a.in.parallel.out
# 10000
mpirun -np 2 ./psort 10000 ./test_data/10000a.in
./check 10000 ./test_data/10000a.in.parallel.out
mpirun -np 3 ./psort 10000 ./test_data/10000a.in
./check 10000 ./test_data/10000a.in.parallel.out
mpirun -np 4 ./psort 10000 ./test_data/10000a.in
./check 10000 ./test_data/10000a.in.parallel.out
mpirun -np 5 ./psort 10000 ./test_data/10000a.in
./check 10000 ./test_data/10000a.in.parallel.out
mpirun -np 6 ./psort 10000 ./test_data/10000a.in
./check 10000 ./test_data/10000a.in.parallel.out
mpirun -np 7 ./psort 10000 ./test_data/10000a.in
./check 10000 ./test_data/10000a.in.parallel.out
mpirun -np 8 ./psort 10000 ./test_data/10000a.in
./check 10000 ./test_data/10000a.in.parallel.out
mpirun -np 9 ./psort 10000 ./test_data/10000a.in
./check 10000 ./test_data/10000a.in.parallel.out
mpirun -np 10 ./psort 10000 ./test_data/10000a.in
./check 10000 ./test_data/10000a.in.parallel.out
mpirun -np 11 ./psort 10000 ./test_data/10000a.in
./check 10000 ./test_data/10000a.in.parallel.out
mpirun -np 12 ./psort 10000 ./test_data/10000a.in
./check 10000 ./test_data/10000a.in.parallel.out
mpirun -np 13 ./psort 10000 ./test_data/10000a.in
./check 10000 ./test_data/10000a.in.parallel.out
mpirun -np 14 ./psort 10000 ./test_data/10000a.in
./check 10000 ./test_data/10000a.in.parallel.out
mpirun -np 15 ./psort 10000 ./test_data/10000a.in
./check 10000 ./test_data/10000a.in.parallel.out
mpirun -np 16 ./psort 10000 ./test_data/10000a.in
./check 10000 ./test_data/10000a.in.parallel.out
mpirun -np 17 ./psort 10000 ./test_data/10000a.in
./check 10000 ./test_data/10000a.in.parallel.out
mpirun -np 18 ./psort 10000 ./test_data/10000a.in
./check 10000 ./test_data/10000a.in.parallel.out
mpirun -np 19 ./psort 10000 ./test_data/10000a.in
./check 10000 ./test_data/10000a.in.parallel.out
mpirun -np 20 ./psort 10000 ./test_data/10000a.in
./check 10000 ./test_data/10000a.in.parallel.out
# 20000
mpirun -np 2 ./psort 20000 ./test_data/20000a.in
./check 20000 ./test_data/20000a.in.parallel.out
mpirun -np 3 ./psort 20000 ./test_data/20000a.in
./check 20000 ./test_data/20000a.in.parallel.out
mpirun -np 4 ./psort 20000 ./test_data/20000a.in
./check 20000 ./test_data/20000a.in.parallel.out
mpirun -np 5 ./psort 20000 ./test_data/20000a.in
./check 20000 ./test_data/20000a.in.parallel.out
mpirun -np 6 ./psort 20000 ./test_data/20000a.in
./check 20000 ./test_data/20000a.in.parallel.out
mpirun -np 7 ./psort 20000 ./test_data/20000a.in
./check 20000 ./test_data/20000a.in.parallel.out
mpirun -np 8 ./psort 20000 ./test_data/20000a.in
./check 20000 ./test_data/20000a.in.parallel.out
mpirun -np 9 ./psort 20000 ./test_data/20000a.in
./check 20000 ./test_data/20000a.in.parallel.out
mpirun -np 10 ./psort 20000 ./test_data/20000a.in
./check 20000 ./test_data/20000a.in.parallel.out
mpirun -np 11 ./psort 20000 ./test_data/20000a.in
./check 20000 ./test_data/20000a.in.parallel.out
mpirun -np 12 ./psort 20000 ./test_data/20000a.in
./check 20000 ./test_data/20000a.in.parallel.out
mpirun -np 13 ./psort 20000 ./test_data/20000a.in
./check 20000 ./test_data/20000a.in.parallel.out
mpirun -np 14 ./psort 20000 ./test_data/20000a.in
./check 20000 ./test_data/20000a.in.parallel.out
mpirun -np 15 ./psort 20000 ./test_data/20000a.in
./check 20000 ./test_data/20000a.in.parallel.out
mpirun -np 16 ./psort 20000 ./test_data/20000a.in
./check 20000 ./test_data/20000a.in.parallel.out
mpirun -np 17 ./psort 20000 ./test_data/20000a.in
./check 20000 ./test_data/20000a.in.parallel.out
mpirun -np 18 ./psort 20000 ./test_data/20000a.in
./check 20000 ./test_data/20000a.in.parallel.out
mpirun -np 19 ./psort 20000 ./test_data/20000a.in
./check 20000 ./test_data/20000a.in.parallel.out
mpirun -np 20 ./psort 20000 ./test_data/20000a.in
./check 20000 ./test_data/20000a.in.parallel.out
# 50000
mpirun -np 2 ./psort 50000 ./test_data/50000a.in
./check 50000 ./test_data/50000a.in.parallel.out
mpirun -np 3 ./psort 50000 ./test_data/50000a.in
./check 50000 ./test_data/50000a.in.parallel.out
mpirun -np 4 ./psort 50000 ./test_data/50000a.in
./check 50000 ./test_data/50000a.in.parallel.out
mpirun -np 5 ./psort 50000 ./test_data/50000a.in
./check 50000 ./test_data/50000a.in.parallel.out
mpirun -np 6 ./psort 50000 ./test_data/50000a.in
./check 50000 ./test_data/50000a.in.parallel.out
mpirun -np 7 ./psort 50000 ./test_data/50000a.in
./check 50000 ./test_data/50000a.in.parallel.out
mpirun -np 8 ./psort 50000 ./test_data/50000a.in
./check 50000 ./test_data/50000a.in.parallel.out
mpirun -np 9 ./psort 50000 ./test_data/50000a.in
./check 50000 ./test_data/50000a.in.parallel.out
mpirun -np 10 ./psort 50000 ./test_data/50000a.in
./check 50000 ./test_data/50000a.in.parallel.out
mpirun -np 11 ./psort 50000 ./test_data/50000a.in
./check 50000 ./test_data/50000a.in.parallel.out
mpirun -np 12 ./psort 50000 ./test_data/50000a.in
./check 50000 ./test_data/50000a.in.parallel.out
mpirun -np 13 ./psort 50000 ./test_data/50000a.in
./check 50000 ./test_data/50000a.in.parallel.out
mpirun -np 14 ./psort 50000 ./test_data/50000a.in
./check 50000 ./test_data/50000a.in.parallel.out
mpirun -np 15 ./psort 50000 ./test_data/50000a.in
./check 50000 ./test_data/50000a.in.parallel.out
mpirun -np 16 ./psort 50000 ./test_data/50000a.in
./check 50000 ./test_data/50000a.in.parallel.out
mpirun -np 17 ./psort 50000 ./test_data/50000a.in
./check 50000 ./test_data/50000a.in.parallel.out
mpirun -np 18 ./psort 50000 ./test_data/50000a.in
./check 50000 ./test_data/50000a.in.parallel.out
mpirun -np 19 ./psort 50000 ./test_data/50000a.in
./check 50000 ./test_data/50000a.in.parallel.out
mpirun -np 20 ./psort 50000 ./test_data/50000a.in
./check 50000 ./test_data/50000a.in.parallel.out
# 80000
mpirun -np 2 ./psort 80000 ./test_data/80000a.in
./check 80000 ./test_data/80000a.in.parallel.out
mpirun -np 3 ./psort 80000 ./test_data/80000a.in
./check 80000 ./test_data/80000a.in.parallel.out
mpirun -np 4 ./psort 80000 ./test_data/80000a.in
./check 80000 ./test_data/80000a.in.parallel.out
mpirun -np 5 ./psort 80000 ./test_data/80000a.in
./check 80000 ./test_data/80000a.in.parallel.out
mpirun -np 6 ./psort 80000 ./test_data/80000a.in
./check 80000 ./test_data/80000a.in.parallel.out
mpirun -np 7 ./psort 80000 ./test_data/80000a.in
./check 80000 ./test_data/80000a.in.parallel.out
mpirun -np 8 ./psort 80000 ./test_data/80000a.in
./check 80000 ./test_data/80000a.in.parallel.out
mpirun -np 9 ./psort 80000 ./test_data/80000a.in
./check 80000 ./test_data/80000a.in.parallel.out
mpirun -np 10 ./psort 80000 ./test_data/80000a.in
./check 80000 ./test_data/80000a.in.parallel.out
mpirun -np 11 ./psort 80000 ./test_data/80000a.in
./check 80000 ./test_data/80000a.in.parallel.out
mpirun -np 12 ./psort 80000 ./test_data/80000a.in
./check 80000 ./test_data/80000a.in.parallel.out
mpirun -np 13 ./psort 80000 ./test_data/80000a.in
./check 80000 ./test_data/80000a.in.parallel.out
mpirun -np 14 ./psort 80000 ./test_data/80000a.in
./check 80000 ./test_data/80000a.in.parallel.out
mpirun -np 15 ./psort 80000 ./test_data/80000a.in
./check 80000 ./test_data/80000a.in.parallel.out
mpirun -np 16 ./psort 80000 ./test_data/80000a.in
./check 80000 ./test_data/80000a.in.parallel.out
mpirun -np 17 ./psort 80000 ./test_data/80000a.in
./check 80000 ./test_data/80000a.in.parallel.out
mpirun -np 18 ./psort 80000 ./test_data/80000a.in
./check 80000 ./test_data/80000a.in.parallel.out
mpirun -np 19 ./psort 80000 ./test_data/80000a.in
./check 80000 ./test_data/80000a.in.parallel.out
mpirun -np 20 ./psort 80000 ./test_data/80000a.in
./check 80000 ./test_data/80000a.in.parallel.out
# 100000
mpirun -np 2 ./psort 100000 ./test_data/100000a.in
./check 100000 ./test_data/100000a.in.parallel.out
mpirun -np 3 ./psort 100000 ./test_data/100000a.in
./check 100000 ./test_data/100000a.in.parallel.out
mpirun -np 4 ./psort 100000 ./test_data/100000a.in
./check 100000 ./test_data/100000a.in.parallel.out
mpirun -np 5 ./psort 100000 ./test_data/100000a.in
./check 100000 ./test_data/100000a.in.parallel.out
mpirun -np 6 ./psort 100000 ./test_data/100000a.in
./check 100000 ./test_data/100000a.in.parallel.out
mpirun -np 7 ./psort 100000 ./test_data/100000a.in
./check 100000 ./test_data/100000a.in.parallel.out
mpirun -np 8 ./psort 100000 ./test_data/100000a.in
./check 100000 ./test_data/100000a.in.parallel.out
mpirun -np 9 ./psort 100000 ./test_data/100000a.in
./check 100000 ./test_data/100000a.in.parallel.out
mpirun -np 10 ./psort 100000 ./test_data/100000a.in
./check 100000 ./test_data/100000a.in.parallel.out
mpirun -np 11 ./psort 100000 ./test_data/100000a.in
./check 100000 ./test_data/100000a.in.parallel.out
mpirun -np 12 ./psort 100000 ./test_data/100000a.in
./check 100000 ./test_data/100000a.in.parallel.out
mpirun -np 13 ./psort 100000 ./test_data/100000a.in
./check 100000 ./test_data/100000a.in.parallel.out
mpirun -np 14 ./psort 100000 ./test_data/100000a.in
./check 100000 ./test_data/100000a.in.parallel.out
mpirun -np 15 ./psort 100000 ./test_data/100000a.in
./check 100000 ./test_data/100000a.in.parallel.out
mpirun -np 16 ./psort 100000 ./test_data/100000a.in
./check 100000 ./test_data/100000a.in.parallel.out
mpirun -np 17 ./psort 100000 ./test_data/100000a.in
./check 100000 ./test_data/100000a.in.parallel.out
mpirun -np 18 ./psort 100000 ./test_data/100000a.in
./check 100000 ./test_data/100000a.in.parallel.out
mpirun -np 19 ./psort 100000 ./test_data/100000a.in
./check 100000 ./test_data/100000a.in.parallel.out
mpirun -np 20 ./psort 100000 ./test_data/100000a.in
./check 100000 ./test_data/100000a.in.parallel.out