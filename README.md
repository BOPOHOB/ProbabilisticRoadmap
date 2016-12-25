# Probabilistic Roadmap algorithm 2D implementation
This implementation of [probabilistic roadmap](https://en.wikipedia.org/wiki/Probabilistic_roadmap) consists of two steps. At first we create a sample of points in the [configuration space](https://en.wikipedia.org/wiki/Configuration_space) and connect it in to the graph. All what we need in order to find a path through the [configuration space](https://en.wikipedia.org/wiki/Configuration_space) is just to find the [shortest path](https://en.wikipedia.org/wiki/Shortest_path_problem) between start and finish points in the given graph. The second step is not a subject of this work. It's implemented as the [Dijkstra's algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm).
## Graph creation
To create the graph we must take a sample of points firstly. The given points represent vertices.
### Vertices sample
![Points shuffle](https://raw.githubusercontent.com/BOPOHOB/ProbabilisticRoadmap/master/exemplification/shuffle.png)

The sample of vertices consists of two parts. First off, it has a few vertices [dirtributed uniformly](https://en.wikipedia.org/wiki/Uniform_distribution_(continuous)) in the avalible [configuration space](https://en.wikipedia.org/wiki/Configuration_space).

![Points close to obstacle](https://raw.githubusercontent.com/BOPOHOB/ProbabilisticRoadmap/master/exemplification/circle.png) 

The second vertices sample is [dirtributed](https://en.wikipedia.org/wiki/Uniform_distribution_(continuous)) in the circle around each obstacle. D in the picture is the doubled distance from the obstacle center to the outermost vertex of this obstacle.

### Edges
![Graph](https://raw.githubusercontent.com/BOPOHOB/ProbabilisticRoadmap/master/exemplification/graph.png) 

Edges come out from each point to the visible (they are connected by straight lines without ever crossing obstacles) points in the small area near the source point. That's it. The graph is ready to use.

## results for few source data
![solve to spars obstacles set](https://raw.githubusercontent.com/BOPOHOB/ProbabilisticRoadmap/master/exemplification/spars.png) 
![solve to regular obstacles set](https://raw.githubusercontent.com/BOPOHOB/ProbabilisticRoadmap/master/exemplification/regular.png) 
![solve to dense obstacles set](https://raw.githubusercontent.com/BOPOHOB/ProbabilisticRoadmap/master/exemplification/dense.png) 

Each picture displays 15 ways.
