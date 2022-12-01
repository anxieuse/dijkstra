# Dijkstra Algorithm using different heap implementations

The goal of this project is to implement the Dijkstra algorithm using different heap implementations and compare the performance of the different implementations.

## Usage

To run the program, you need to have the following installed:
- Boost
- Make

To compile the program, run the following command:
```
make
```

To run the program, run the following command:
```
./main < in.txt
```

## Input

The input file should be in the following format:
```
<Number of vertices> <Number of edges>
<Source vertex> <Destination vertex> <Weight>
...
```

## Modify the program

If you want to add another heap implementation, you need to:
1) Add it's header file in main.cpp
2) Modify heapMetrics in main.cpp to include the new heap implementation metrics
3) Add the new heap implementation call in the main for loop in main.cpp as follows

# Results

On my machine, the results are as follows:
```
$ make; ./main < ../tests/2000v2585e20000d679c/in.txt
g++ -o main main.cpp -Ofast
Reading edges
Running global Dijkstra 10 times

4_ary_heap
time: 0.012418s
pop: 75150 operations
push: 55150 operations

stl_priority_queue
time: 0.014747s
pop: 75150 operations
push: 55150 operations
```