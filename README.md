# Water Spreading Algorithm | Water Spreading Algorithm Parallel
Follows the implementation of the algorithm described in "Computing shortest paths from a source node to each other node" -  Liu, Guisong et. al., and a parallel implementation that parallelizes the algorithm, using OpenMP.

[Article](https://link.springer.com/article/10.1007/s00500-014-1434-2)

The algorithm is related to the K-shortest-path problem (KSP), which is summarized in finding the k-shortest paths in a directed weighted graph in a non-decreasing order.



# Execution

1. Execute "sh compile.sh"
2. Execute "sh execute.sh"


# Parallel Version vs Sequential Version
Some images comparing both versions

# Memory Usage
![alt text](https://github.com/lucaspacifico/wsaParallel/blob/master/memory3Versions.png)

# Time
![alt text](https://github.com/lucaspacifico/wsaParallel/blob/master/time3Versions.png)

# Dataset
[New-York -- ~250k Nodes -- ~750k Arcs](http://users.diag.uniroma1.it/challenge9/download.shtml)





