# Probabilistic roadmap algorithm 2d implementation
This implementation of [probabilistic roadmap](https://en.wikipedia.org/wiki/Probabilistic_roadmap) consists of two steps. At first we create a sample of points on the [configuration space](https://en.wikipedia.org/wiki/Configuration_space) and connect it in to the graph. All what we need in order to find a path through the [configuration space](https://en.wikipedia.org/wiki/Configuration_space) is just find the [shortest path](https://en.wikipedia.org/wiki/Shortest_path_problem) from start point to finish in the given graph. The second step is not a subject of this work. It's implemented as a [Dijkstra's algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm).
## Graph creation
To create the graph we must take a sample of points firstly. Given points shold play a part of vertexes.
### Vertexes sample
![Points shuffle](https://raw.githubusercontent.com/BOPOHOB/ProbabilisticRoadmap/master/exemplification/shuffle.png)

The sample of vertices consist of two part. First of it has a few vertices [dirtributed uniformly](https://en.wikipedia.org/wiki/Uniform_distribution_(continuous)) in the avalible [configuration space](https://en.wikipedia.org/wiki/Configuration_space).

![Points close to obstacle](https://raw.githubusercontent.com/BOPOHOB/ProbabilisticRoadmap/master/exemplification/circle.png) 

The second vertices sample are [dirtributed](https://en.wikipedia.org/wiki/Uniform_distribution_(continuous)) in the circler around each obstacle. D in the picture is the doubled distance from the center of obstacle to outermost vertex of this obstacle.

### Edges
![Graph](https://raw.githubusercontent.com/BOPOHOB/ProbabilisticRoadmap/master/exemplification/graph.png) 

Edges come out from each point to the visible (have direct way without obstacles on it) points in the not big area of source point. There is all. Graph ready to use.

## results for few source data
![solve to spars obstacles set](https://raw.githubusercontent.com/BOPOHOB/ProbabilisticRoadmap/master/exemplification/spars.png) 
![solve to regular obstacles set](https://raw.githubusercontent.com/BOPOHOB/ProbabilisticRoadmap/master/exemplification/regular.png) 
![solve to dense obstacles set](https://raw.githubusercontent.com/BOPOHOB/ProbabilisticRoadmap/master/exemplification/dense.png) 

Each picture display 15 ways.
