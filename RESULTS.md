# **RESULTS**

### Outcome

Our team’s project is called “**Voyager**.” Voyager is a project which tries to find worldwide airports which have important effect on the transition. Heatmap is the visualization method we choose to show the results of our project. 

### Implementation

By using the algorithm **betweenness centrality**, our teammate finds the importance and centrality of every airport in the world. The unique ID for each airport is used as a key, and the value is all the neighbor airports around it. Centrality of all the airports is stored in the double array for “DrawGraph” function to use later.

Since we need to **project the position of each airport on to the map based on the data** we retrieve from dataset, we convert the longitude and latitude of every airport into XY coordinate. Then we use the graph **traversal** to change the target pixels’ hue. The range of hue is from 0 to 270. Redder (0) the pixel, then centrality of airport is higher. On the contrary, more purple (270) the pixel be, the less of centrality of this airport is. 

We also write **test case** to ensure the accuracy of every function. We seprately test airport database size, airport read accuracy, adjacency matrix, centrality and coordinate conversion.

### Discovery

Upon the heatmap we generate, we find that Asia and Europe have the reddest points, which means that airports have very important roles on the transition. In addition, comparing to the airports in the Asia, Europe, North America, South America, Africa, and Australia, almost all the important airports along the coastline.