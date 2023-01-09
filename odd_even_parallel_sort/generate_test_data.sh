mkdir test_data
g++ test_data_generator.cpp -o gen
./gen 20 ./test_data/20a.in
./gen 100 ./test_data/100a.in
./gen 200 ./test_data/200a.in
./gen 500 ./test_data/500a.in
./gen 800 ./test_data/800a.in
./gen 1000 ./test_data/1000a.in
./gen 2000 ./test_data/2000a.in
./gen 5000 ./test_data/5000a.in
./gen 8000 ./test_data/8000a.in
./gen 10000 ./test_data/10000a.in
./gen 50000 ./test_data/50000a.in
./gen 80000 ./test_data/80000a.in
./gen 100000 ./test_data/100000a.in