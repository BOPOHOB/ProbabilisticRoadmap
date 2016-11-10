# Probabilistic roadmap algorithm 2d implementation
In this implementation of [probabilistic roadmap](https://en.wikipedia.org/wiki/Probabilistic_roadmap) consists of two steps. In first we create a sample of points on the [configuration space](https://en.wikipedia.org/wiki/Configuration_space) and connect it to the graph. All what we need to find a path through the [configuration space](https://en.wikipedia.org/wiki/Configuration_space) is just find the [shortest path](https://en.wikipedia.org/wiki/Shortest_path_problem) from start point to finish in the given graph. The second step is not a subject of this work. It's implemented as a raw [Dijkstra's algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm) for undirected graph.
## Graph creation
To create the graph we must take a sample of points in first.
### Vertexes sample
![Points shuffle](https://raw.githubusercontent.com/BOPOHOB/ProbabilisticRoadmap/master/exemplification/shuffle.png)

The sample of vertexes consist of two parts. First of it have a few fertexes [uniform dirtributed](https://en.wikipedia.org/wiki/Uniform_distribution_(continuous)) on the avalible [configuration space](https://en.wikipedia.org/wiki/Configuration_space).

![Points close to obstacle](https://raw.githubusercontent.com/BOPOHOB/ProbabilisticRoadmap/master/exemplification/circle.png) 

The second vertexes sample [dirtributed](https://en.wikipedia.org/wiki/Uniform_distribution_(continuous)) in the circler around each obstacle.

### Edges
![Graph](https://raw.githubusercontent.com/BOPOHOB/ProbabilisticRoadmap/master/exemplification/graph.png) 

Edges come out from each point to the visible (have direct way without obstacles on it) points in the not big area of it. There is all. Graph ready to use.

## results for few source data
![solve to spars obstacles set](https://raw.githubusercontent.com/BOPOHOB/ProbabilisticRoadmap/master/exemplification/spars.png) 
![solve to regular obstacles set](https://raw.githubusercontent.com/BOPOHOB/ProbabilisticRoadmap/master/exemplification/regular.png) 
![solve to [dense obstacles set](https://raw.githubusercontent.com/BOPOHOB/ProbabilisticRoadmap/master/exemplification/dense.png) 

Each picture display 15 ways.
