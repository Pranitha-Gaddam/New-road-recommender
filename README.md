# New-road-recommender
This project aims to maximize the benefits of shorter travel times when building new roads within a network graph. The process entails trip simulation, estimating the possible benefits of building new roads, and repeatedly adding the road that offers the greatest benefit to the graph.

The networkX module is utilized to create a random graph with 60 nodes and an initial connectivity of 0.05. The connectivity is incremented by 0.01 until the graph is connected. The size of the edges between two nodes in the graph are randomly assigned with a range of 5-25. The program runs a simulation of 10 hours (36000 seconds), where 100 trips are generated every second.

The program then finds the benefit of building a road between each set of unconnected nodes within the graph and the set of nodes with the highest benefit value is recommended to build a road. The "benefit" is a numeric value calculated based on the distances and the number of trips between two nodes as well as their negihbors. After a new road is built, the program reconsiders the benefits of building a road between the neighbors of the nodes between which the new road was built. Then, a new road is recommended to be built again after taking the updated benefits into consideration, and this process is repeated until the budget for new roads (represented by k) runs out.

R2 - does not include trip simulation; a graph with pre-defined edges and vertices is used along with a pre-defined list of trips between nodes. 2 new roads are recommended.
R3 - Graph is generated randomly and a trip simulation is run. The program then recommends 3 new roads for the randomly generated graph.
