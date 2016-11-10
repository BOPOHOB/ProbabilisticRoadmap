# Probabilistic roadmap algorithm 2d realization
In this implementation of [probabilistic roadmap](https://en.wikipedia.org/wiki/Probabilistic_roadmap) consists of two steps. In first we create a sample of points on the [configuration space](https://en.wikipedia.org/wiki/Configuration_space) and connect it to the graph. All what we need to find a path through the [configuration space](https://en.wikipedia.org/wiki/Configuration_space) is just find the [shortest path](https://en.wikipedia.org/wiki/Shortest_path_problem) from start point to finish in the given graph. The second step is not a subject of this work. It's implemented as a raw [Dijkstra's algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm) for undirected graph.
## Graph creation
To create the graph we must take a sample of points in first.
### Vertexes sample
