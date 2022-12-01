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
<Number of vertices> <Number of edges> <Number of demands>
<Source vertex> <Destination vertex> <Weight>
...
```

Number of demands and the demands themselves are ignored in this implementation. The program will run the Dijkstra algorithm on every vertex in the graph.

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
Running global Dijkstra 1000 times

3_ary_heap
time: 0.049959s
pop: 7503000 operations
push: 5503000 operations

4_ary_heap
time: 0.047678s
pop: 7503000 operations
push: 5503000 operations

2_ary_heap
time: 0.04719s
pop: 7503000 operations
push: 5503000 operations

skew_heap
time: 0.093982s
pop: 7503000 operations
push: 5503000 operations

8_ary_heap
time: 0.047185s
pop: 7503000 operations
push: 5503000 operations

binomial_heap
time: 0.290924s
pop: 7503000 operations
push: 5503000 operations

pairing_heap
time: 0.165247s
pop: 7503000 operations
push: 5503000 operations

fibonacci_heap
time: 0.265638s
pop: 7503000 operations
push: 5503000 operations

boost_priority_queue
time: 0.067497s
pop: 7503000 operations
push: 5503000 operations

stl_priority_queue
time: 0.100738s
pop: 7503000 operations
push: 5503000 operations
```