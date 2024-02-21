# this code is for R3
from networkx.generators.random_graphs import erdos_renyi_graph
import networkx as nx
import matplotlib.pyplot as plt
import random

N = 60
p = 0.05
T = 100
k = 3
f = 0.6

trips = {}
benefits = {}
highest_benefit = 0
road_dist = 0

G= nx.erdos_renyi_graph(N,p)

while not nx.is_connected(G):
    p += 0.01
    G = nx.erdos_renyi_graph(N,p)

for edge in G.edges():
    G[edge[0]][edge[1]]['weight'] = random.randint(5, 25)

def simulation(T):
    global trips
    for i in range(36000):
        for j in range(T):
            start = random.randint(0, N-1)
            end = random.randint(0, N-1)

            while start == end:
                end = random.randint(0, N-1)

            pair = (min(start,end), max(start,end))
            if pair in trips:
                trips[pair] += 1
            else:
                trips[pair] = 1

def find_benefits(G):
    global highest_benefit, node1, node2, benefits, road_dist
    visited_pairs = set()
    for a in G.nodes():
        for b in G.nodes():
            x = min(a, b)
            y = max(a, b)
            pair = (x,y)
            if x == y or pair in visited_pairs:
                continue
            if G.has_edge(x, y):
                continue
            visited_pairs.add(pair)
            spd = nx.astar_path_length(G,x,y)
            d = round((f*spd), 1)
            if pair in trips:
                nt = trips[pair]
            else:
                nt = 0
            benefits[pair] = round((spd-d)*nt, 1)
            #print(f"pair: {pair}; benefit: {benefits[pair]}; nt: {nt}; spd: {spd}; d: {d}")

            for n1 in G.neighbors(y):
                n1Benefit = nx.astar_path_length(G,x,n1) - (d + nx.astar_path_length(G,y,n1))
                n1Benefit = round(n1Benefit, 1)
                n1_pair = (min(x,n1), max(x,n1))
                if n1_pair in trips:
                    benefits[pair] += round(max(n1Benefit,0)*(trips[n1_pair]), 1)
                    #print(f"n1: {n1}; n1benefit: {n1Benefit}; new benefits: {benefits[pair]}")

            for n2 in G.neighbors(x):
                n2Benefit = nx.astar_path_length(G,y,n2) - (d + nx.astar_path_length(G,x,n2))
                n2Benefit = round(n2Benefit, 1)
                n2_pair = (min(y,n2), max(y,n2))
                if n2_pair in trips:
                    benefits[pair] += round(max(n2Benefit,0)*(trips[n2_pair]), 1)
                    #print(f"n2: {n2}; n2benefit: {n2Benefit}; new benefits: {benefits[pair]}")
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
            x = min(a,b)
            y = max(a,b)
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
            #print(f"pair: {pair}; benefit: {benefits[pair]}; nt: {nt}; spd: {spd}; d: {d}")

            for n1 in G.neighbors(y):
                n1Benefit = nx.astar_path_length(G,x,n1) - (d + nx.astar_path_length(G,y,n1))
                n1Benefit = round(n1Benefit, 2)
                n1_pair = (min(x,n1), max(x,n1))
                if n1_pair in trips:
                    benefits[pair] += round(max(n1Benefit,0)*(trips[n1_pair]), 2)
                    #print(f"n1: {n1}; n1benefit: {n1Benefit}; new benefits: {benefits[pair]}")

            for n2 in G.neighbors(x):
                n2Benefit = nx.astar_path_length(G,y,n2) - (d + nx.astar_path_length(G,x,n2))
                n2Benefit = round(n2Benefit, 2)
                n2_pair = (min(y,n2), max(y,n2))
                if n2_pair in trips:
                    benefits[pair] += round(max(n2Benefit,0)*(trips[n2_pair]), 2)
                    #print(f"n2: {n2}; n2benefit: {n2Benefit}; new benefits: {benefits[pair]}")
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
        #print(f"benefit is {highest_benefit}")
        #for pair, value in benefits.items():
            #print(f"Pair: {pair}, Benefit: {value}")
        G.add_edge(node1, node2, weight=road_dist)
        benefits[(min(node1,node2), max(node1,node2))] = 0
        highest_benefit=0
        edit_benefits(G)

pos = nx.spring_layout(G)

simulation(T)
#for pair, value in trips.items():
    #print(f"Pair: {pair}, Value: {value}")
find_benefits(G)
build_road(G)


