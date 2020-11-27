# RESULTS

## Outcome

Our team’s project is called “**Voyager**.” Voyager is a project which tries to find worldwide airports which have important effect on the transition. Heatmap is the visualization method we choose to show the results of our project. ![outMap](result_high_resolution.png)



## Implementation

By using the algorithm **betweenness centrality**, our teammate finds the importance and centrality of every airport in the world. The unique ID for each airport is used as a key, and the value is all the neighbor airports around it. Centrality of all the airports is stored in the double array for “DrawGraph” function to use later.

First part is to find **betweenness centrality**, which is the number of shortest path of each pair of nodes that passes through each node. This task can be divided into two steps, first compute the length and number of shortest path between all pairs of nodes, and then sum all pair-dependencies. Using adjacency matrix to implement second step will take at least $O(n^3)$ time complexity, we choose to use Brande's algorithm to reduce run time to $O(m \cdot n)$.  Brande's algorithm primarily replies on the following theorem. 

1. A vertex $v ∈ V$ lies on a shortest path between vertices $s,t ∈ V$ , if and only if $d_G(s,t) = d_G(s, v) + d_G(v,t)$.
2. Combinatorial shortest-path counting. $For s \neq v ∈ V,  σ_{sv} = \sum_{u∈P_s(v)} σ_{su}$.
3. Accumulation of Pair-Dependencies. If there is exactly one shortest path from $s ∈ V$ to each $t ∈ V$ , the dependency of $s$ on any $v ∈ V$ obeys $δs•(v) = \sum_{w : v∈P_s(w)} (1 + δs•(w))$.
4. The dependency of $s ∈ V$ on any $v ∈ V$ obeys $δs•(v) = \sum_{w:v∈P_s(w)} \frac{σ_{sv}}{σ_{sw}} · (1 + δs•(w))$.

Combining these theorem, we have Brande's algorithm implemented by 

```
for each node v the graph:
	using queue to do BFS
	for each node q in queue:
		add to stack
		update minimum steps from v to each node and keep track of predecessor
	using stack to do DFS
		update centrality by pair-depencency accumulation.  
   
```



The second part is to present centrality onto world map as **heatmap**. This can be done in two steps. First,  project the position of each airport on to the map based on the data we retrieve from dataset by converting the longitude and latitude of every airport into $(X,Y)$ coordinate. Then we use the **BFS** to change the target pixels’ hue. The range of hue is from 0 to 270. Redder (0) the pixel, then centrality of airport is higher. On the contrary, more purple (270) the pixel be, the less of centrality of this airport is. 



We also write **test case** to ensure the accuracy of every function. We seprately test airport database size, airport read accuracy, adjacency matrix, centrality and coordinate conversion.

### Testing Strategy

1. Test Initialization 
   1. Test read airport database with correct size
   2. Test first/last/random airport read correctly
   3. Test Adjacency matrix is set up correctly.
   4. (Samll testing database used dir: ../dataset/test..*dat)
2. Test betweeness Centrality
   1. Test basic graph centrality 1
   2. Test basic graph centrality 2
   3. Test database with most connected/busiest airport
3. Test Visualization
   1. Test random airport coordinate conversion
   2. Test two near airport coordinate conversion
   3. Test write to png successful
   4. Test result graph hue with correct contrast

Testing groups:

[init] [centrality] [visualization] [coordinate] [basic] [complex]

### Discovery

Upon the heatmap we generate, we find that Asia and Europe have the reddest points, which means that airports have very important roles on the transition. In addition, comparing to the airports in the Asia, Europe, North America, South America, Africa, and Australia, almost all the important airports along the coastline.