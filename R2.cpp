# this is for R2
from networkx.generators.random_graphs import erdos_renyi_graph
import networkx as nx
import matplotlib.pyplot as plt


G = nx.Graph()
G.add_edges_from([(0,1, {'weight': 6}), (1,3, {'weight': 11}), (3,4, {'weight': 7}), (4,2, {'weight': 10}), (4,0, {'weight': 9})])

new_roads = [(0,2), (0,3), (1,2), (1,4), (2,3)]
trip_list = [((0,2), 1), ((2,0),1), ((0,3),1), ((3,0),1), ((1,2),1), ((2,1),2), ((1,4), 1), ((4,1),2), ((2,3),3), ((3,2),1)]
trips = {}
benefits = {}
highest_benefit = 0
road_dist = 0
f = 0.6
k = 2

def simulation(trip_list):
    global trips
    for trip in trip_list:
        pair = (min(trip[0][0], trip[0][1]), max(trip[0][0], trip[0][1]))
        if pair in trips:
            trips[pair] += trip[1]
        else:
            trips[pair] = trip[1]

def find_benefits(G):
    global highest_benefit, node1, node2, benefits, road_dist
    visited_pairs = set()
    for node in new_roads:
        x = node[0]
        y = node[-1]
        pair = (min(x,y), max(x,y))
        if x == y or G.has_edge(x, y):
            continue
        if pair in visited_pairs:
            continue
        visited_pairs.add(pair)
        spd = nx.astar_path_length(G,x,y)
        d = round(f*spd, 1)
        if pair in trips:
            nt = trips[pair]
        else:
            nt = 0
        benefits[pair] = round((spd-d)*nt, 1)
        #print(f"initial benefit: {benefits[pair]} for {pair}")

        for n1 in G.neighbors(y):
            n1Benefit = nx.astar_path_length(G,x,n1) - (d + nx.astar_path_length(G,y,n1))
            n1Benefit = round(n1Benefit, 1)
            n1_pair = (min(x,n1), max(x,n1))
            if n1_pair in trips:
                benefits[pair] += round(max(n1Benefit,0)*(trips[n1_pair]), 1)
                #print(f"after addition to ben through n1(N of y):{n1} -> {benefits[pair]}")
                #print(f"n1 benefit: {n1Benefit} and nt: {trips[n1_pair]}")

        for n2 in G.neighbors(x):
            n2Benefit = nx.astar_path_length(G,y,n2) - (d + nx.astar_path_length(G,x,n2))
            n2Benefit = round(n2Benefit, 1)
            n2_pair = (min(y,n2), max(y,n2))
            if n2_pair in trips:
                benefits[pair] += round(max(n2Benefit,0)*(trips[n2_pair]), 1)
                #print(f"after addition to ben through n2(N of x):{n2} -> {benefits[pair]}")
                #print(f"n2 benefit: {n2Benefit} and nt: {trips[n2_pair]}")

    for key, value in benefits.items():
            #print(f"pair: {key} and benefit: {value}")
            if value > highest_benefit:
                highest_benefit = value
                node1 = key[0]
                node2 = key[1]
                spd = nx.astar_path_length(G,node1,node2)
                road_dist = round((f*spd), 1)

def edit_benefits(G):
    global road_dist, node1, node2, highest_benefit
    visited_pairs = set()
    affected_nodes = [node1]+list(G.neighbors(node1))+[node2]+list(G.neighbors(node2))
    for a in affected_nodes:
        for b in affected_nodes:
            x = min(a, b)
            y = max(a, b)
            if x == y or G.has_edge(x, y):
                continue
            pair = (x,y)
            if pair in visited_pairs:
                continue
            visited_pairs.add(pair)
            spd = nx.astar_path_length(G,x,y)
            d = round(f*spd, 2)
            if pair in trips:
                nt = trips[pair]
            else:
                nt = 0
            benefits[pair] = round((spd-d)*nt, 2)
            #print(f"initial benefit: {benefits[pair]} for {pair}")

            for n1 in G.neighbors(y):
                n1Benefit = nx.astar_path_length(G,x,n1) - (d + nx.astar_path_length(G,y,n1))
                n1Benefit = round(n1Benefit, 2)
                n1_pair = (min(x,n1), max(x,n1))
                if n1_pair in trips:
                    benefits[pair] += round(max(n1Benefit,0)*(trips[n1_pair]), 2)
                    #print(f"after addition to ben through n1(N of y):{n1} -> {benefits[pair]}")
                    #print(f"n1 benefit: {n1Benefit} and nt: {trips[n1_pair]}")

            for n2 in G.neighbors(x):
                n2Benefit = nx.astar_path_length(G,y,n2) - (d + nx.astar_path_length(G,x,n2))
                n2Benefit = round(n2Benefit, 2)
                n2_pair = (min(y,n2), max(y,n2))
                if n2_pair in trips:
                    benefits[pair] += round(max(n2Benefit,0)*(trips[n2_pair]), 2)
                    #print(f"after addition to ben through n2(N of x):{n2} -> {benefits[pair]}")
                    #print(f"n2 benefit: {n2Benefit} and nt: {trips[n2_pair]}")

    for key, value in benefits.items():
        #print(f"pair: {key} and benefit: {value}")
        if value > highest_benefit:
            highest_benefit = value
            node1 = key[0]
            node2 = key[1]
            spd = nx.astar_path_length(G,node1,node2)
            road_dist = round((f*spd), 1)


def build_road(G):
    global highest_benefit, benefits
    for i in range(k):
        print(f"build road between {node1} and {node2}. Benefit is {highest_benefit}")
        G.add_edge(node1, node2, weight=road_dist)
        benefits[(min(node1,node2), max(node1,node2))] = 0
        highest_benefit=0
        edit_benefits(G)



simulation(trip_list)
find_benefits(G)
build_road(G)


