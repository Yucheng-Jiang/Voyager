# Project Goals

### Summary

In this final project, the goal is aim at finding all the possible flight routes from airport of departure to destination. All the possible flight routes will be mapped on the world map and be sorted based on the betweenness centrality.

### Dataset

Openflights is the dataset our group choose to use. [Link of the datase](https://openflights.org/data.html).

This dataset contains over 10,000 airports, 5888 airlines and 67663 routes between 3321 airports on 548 airlines spreading around the world. Format of the sample entries in the route database as listed following: `BA, 1355, SIN, 3316, LHR, 507, , 0, 744 777`.

### Algorithms

###### **1.**   **Traversal: BFS (Breadth First Search)**

###### **2.**   **Shortest Path: [Dijkstra’s Algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm)**

###### **3.**   **[Between centrality](https://en.wikipedia.org/wiki/Betweenness_centrality)**

###### **4.**   **Graphic Output of Graph: Project on to map based on data**

First, our group’s strategy is to map all the flight routes based on the data provided by the Openflights datasets on the map. Then to compute the overall routes from origin airport to all the destination airports, we choose **BFS traversal** to search the graph from one specific node to the others. To retrieve the importance of airports based on the **betweenness centrality**, we need to calculate the shortest path for every pair of nodes in the graph by **Dijkstra’s Algorithm**. There exists at least one shortest path between the origin airport and other destination in the graph we mapped. The number of edges that the shortest path passes through the airport will show the degree of influence on the transfer network. The main purpose of the project is to find what airports are most important based on the **betweenness centrality**.

 