#from platform import node
#import functools as ft
#import random
import math
import numpy as np
import pandas as pd
import random
import networkx as nx
import matplotlib.pyplot as plt
#import time
#import json
#import subprocess
import os, sys
import argparse 
from numpy.random import choice
from scipy.spatial import Delaunay
from datetime import datetime
from leo import misc
#import geom_funcs as gf
import tqdm

date_and_time = datetime.today().strftime("%Y-%m-%d %H:%M:%S")

def xyz_euclidian_distance(arr1: np.array, arr2: np.array):
    return np.linalg.norm(arr1 - arr2)

def create_graph_files(gr: nx.Graph, points, args, logger, argsinfo):
    edges_cnt = len(gr.edges)
    nodes_cnt = len(gr.nodes)

    logger.info("Create demands")
    sh_p_len = dict(nx.all_pairs_shortest_path_length(gr))
    source_dest_n_len = []
    for v1 in gr.nodes():
        for v2 in gr.nodes():
            if v1 >= v2: continue
            curlen = sh_p_len[v1][v2]
            if args.minlen <= curlen <= args.maxlen:
                source_dest_n_len.append((v1, v2, sh_p_len[v1][v2]))

    if args.demands != 0: 
        demands_num = args.demands
    else:    
        demands_num = nodes_cnt * 10
    
    demands = []
    wlen_sum = 0
    total_capacity = 0
    while len(demands) < demands_num:
        new_demand = random.choice(source_dest_n_len)
        rand_num = random.randint(1, 1000)
        demand_bitrate = ''
        if rand_num <= 600:
            demand_bitrate = '10G'
            total_capacity += 10
        elif rand_num > 800:
            demand_bitrate = '100G'
            total_capacity += 100
        else:
            demand_bitrate = '40G'
            total_capacity += 40
        demands.append((*new_demand, demand_bitrate))

    logger.info(f'Generated {len(demands)} demands') #, while desired {demands_num=}')
    demands_num = len(demands)
    
    total_capacity //= 1000
    logger.info("Write test input data in find_paths.py script format")
    logger.info(f"{nodes_cnt} nodes {edges_cnt} edges {demands_num} demands {total_capacity}Tb capacity\n")

    # Create plot
    plt.figure(figsize=(25.6 * 0.5, 19.2 * 0.5))
    plt.ylabel('y'), plt.xlabel('x')
    
    # Plot points
    plt.scatter(*zip(*points), c = 'black', s = 25 * 0.25)

    # Plot edges
    for v1, v2 in gr.edges():
        x_coords = [points[v1][0], points[v2][0]]
        y_coords = [points[v1][1], points[v2][1]]
        plt.plot(x_coords, y_coords, color='black', linewidth=0.5)
    
    # Create graph directory
    example_name = f'{nodes_cnt}v{edges_cnt}e{demands_num}d{total_capacity}c'
    graph_dirpath = args.output + '/' + example_name
    os.mkdir(graph_dirpath)
    print ("Create dir", graph_dirpath, "for the output")

    # Save plot
    plt.title(f'{args.graphtype}: v{nodes_cnt}e{edges_cnt}')
    plt.savefig(graph_dirpath + f'/graph_plot.png')
    plt.clf()

    with open('gen.log', 'at') as logfile:
        logfile.write(f"{argsinfo}")
        logfile.write(f"Output: {example_name}\n")

    with open(graph_dirpath + '/demands.csv', 'wt') as ofile:
        ofile.write("ID,src,dst,bitrate,length\n")
        ofile.write('\n'.join([f'{id},{d[0]},{d[1]},{d[3]},{d[2]}' for id, d in enumerate(demands)]))

    with open(graph_dirpath + '/nodes.csv', 'wt') as ofile:
        ofile.write("ID,x,y\n")
        ofile.write('\n'.join([f'{i},{points[i][0]},{points[i][1]}' for i in range(nodes_cnt)]))

    with open(graph_dirpath + '/links.csv', 'wt') as ofile:
        ofile.write("ID,node1,node2" + '\n')
        ofile.write('\n'.join([f"{id},{v1},{v2}" for id, (v1, v2) in enumerate(gr.edges())]))

    # calculate distance between nodes
    dist = np.zeros((nodes_cnt, nodes_cnt))
    for v1 in range(nodes_cnt):
      for v2 in range(nodes_cnt):
          dist[v1][v2] = xyz_euclidian_distance(points[v1], points[v2])

    logger.info("Write test input data in contest format")
    with open(graph_dirpath + '.txt', 'wt') as ofile:
       ofile.write(f"{nodes_cnt} {edges_cnt} {demands_num}\n")
       ofile.write("\n".join([f'{v1 + 1} {v2 + 1} {int(dist[v1][v2])}' for v1, v2 in gr.edges()]) + '\n')
       ofile.write("\n".join([f'{id + 1} {d[0] + 1} {d[1] + 1}' for id, d in enumerate(demands)]) + '\n')


def main():
    parser = argparse.ArgumentParser(description="Tests generator")
    parser.add_argument("-o", "--output", type=str, default=".",
                        help="Outut folder name")
    parser.add_argument("-n", "--nodes", type=int, default=50,
                        help="Number of nodes")
    parser.add_argument("-d", "--demands", type=int, default=0,
                        help="Number of demands")
    parser.add_argument("-W", "--width", type=int, default=64,
                        help="Maximum edge width")
    parser.add_argument("-minl", "--minlen", type=int, default=2,
                        help="Minimum path length")
    parser.add_argument("-maxl", "--maxlen", type=int, default=1000,
                        help="Maximum path length")
    parser.add_argument("-minw", "--minwidth", type=int, default=1,
                        help="Minimum path width")
    parser.add_argument("-maxw", "--maxwidth", type=int, default=8,
                        help="Maximum path width")
    parser.add_argument("-rw", "--rectanglewidth", type=int, default=2000,
                        help="Rectangle width")
    parser.add_argument("-rh", "--rectangleheight", type=int, default=1000,
                        help="Rectangle height")
    parser.add_argument("-MC", "--magicconst", type=float, default=0.5,
                        help="Magic constant: [0.4, 1.0]")
    parser.add_argument("-t", "--graphtype", type=str, default='UG',
                        help="Graph type: UG, GG, RNG, TG, grid")
    args = parser.parse_args()
    argsinfo = f"{date_and_time}\n"
    argsinfo += f"Cmdline args: {args.nodes} nodes, {args.demands} demands, {args.width} colors, length: {args.minlen}..{args.maxlen}, "
    argsinfo += f"dmd width: {args.minwidth}..{args.maxwidth}, rectangle: {args.rectanglewidth} x {args.rectangleheight}, "
    argsinfo += f"MC={args.magicconst}, graphtype={args.graphtype}.\n"

    # Create logger
    logger = misc.get_logger(__name__)
    
    # The size of the canvas
    width = args.rectanglewidth
    height = args.rectangleheight
    # The number of nodes
    nodes_num = args.nodes
    if args.graphtype == 'grid':
        nodes_num = width * height
    if nodes_num > 1000:
        logger.error (f"The number of nodes {nodes_num} is greater than 1000!")
        #exit()
        
    
    # Length scaling
    length_multiplier = 10

    if nodes_num > width * height * 0.5:
        if nodes_num > width * height:
            logger.error (f"The number of nodes {nodes_num} is greater than {width}x{height}!")
            exit()
        points_matrix = [ [x,y] for x in range(width) for y in range(height) ]
        choosen_points = set(choice(range(len(points_matrix)), nodes_num, replace=False))
        points = np.array(  [points_matrix[i] for i in choosen_points]  )
    else:
        choosen_points = set()    
        while len(choosen_points) < nodes_num:
            x = random.randint(1, width)
            y = random.randint(1, height)
            choosen_points.add((x,y))
        points = np.array([[x,y] for x,y in choosen_points])

    # Find the Delaunay diagram
    tri = Delaunay(points)

    # Collect all the edges of the Delaunay diagram
    all_edges = set()
    indptr = tri.vertex_neighbor_vertices[0]
    indices = tri.vertex_neighbor_vertices[1]
    length = {}
    for i in range(nodes_num):
        for j in indices[indptr[i]:indptr[i+1]]:
            if i < j:
                length[(i, j)] = float(length_multiplier * np.linalg.norm([points[i,0] - points[j,0], points[i,1] - points[j,1]]))
                all_edges.add((i, j))

    # Create the networkx graph
    edges_with_weight = [(e[0], e[1], {'len': length[e]}) for e in all_edges]
    TG = nx.Graph(edges_with_weight)
    for nd in TG.nodes:
        TG.nodes[nd]['x'] = points[nd][0]
        TG.nodes[nd]['y'] = points[nd][1]
        TG.nodes[nd]['users'] = 0

    logger.info(f"TG has {TG.number_of_nodes()} nodes and {TG.number_of_edges()} edges")

    if args.graphtype == 'UG' or args.graphtype == 'grid':
        logger.info("Create UG")
        edges_to_remove = []
        for v1 in TG.nodes:
            for v2 in TG.neighbors(v1):
                for v3 in TG.neighbors(v2):
                    if TG.has_edge(v1, v3) and v1 < v2 < v3:
                        max_edge_len = max(TG[v1][v2]['len'], TG[v1][v3]['len'], TG[v2][v3]['len'])
                        if TG[v1][v2]['len'] == max_edge_len:
                            edges_to_remove.append((v1, v2))
                        elif TG[v1][v3]['len'] == max_edge_len:
                            edges_to_remove.append((v1, v3))
                        elif TG[v2][v3]['len'] == max_edge_len:
                            edges_to_remove.append((v2, v3))
                        else: 
                            exit('Cant find edge with max length in triangle')

        UG = TG.copy()
        edges_to_remove = set(edges_to_remove)
        for v1, v2 in edges_to_remove:
            if UG.has_edge(v1, v2):
                UG.remove_edge(v1, v2)

        logger.info('Crete UG files')
        create_graph_files(UG, points, args, logger, argsinfo)
    elif args.graphtype == 'GG' or args.graphtype == 'RNG':
        logger.info('Create RNG and GG')
        RNG = nx.create_empty_copy(TG)
        GG = nx.create_empty_copy(TG)
        for v1 in tqdm.tqdm(TG.nodes()):
            pt1 = np.array(points[v1])
            for v2 in TG.nodes():
                if v2 <= v1: continue 
                pt2 = np.array(points[v2])
                R = xyz_euclidian_distance(pt1, pt2)
                center = 0.5 * (pt1 + pt2)
                RNG_connect = True
                GG_connect = True
                for v3 in TG.nodes():
                    if v3 == v1 or v3 == v2: continue
                    pt3 = np.array(points[v3])
                    r1 = xyz_euclidian_distance(pt1, pt3)
                    r2 = xyz_euclidian_distance(pt2, pt3)
                    if r1 <= R and r2 <= R: 
                        RNG_connect = False 
                    if xyz_euclidian_distance(center, pt3) <= 0.5 * R: 
                        GG_connect = False 
                if RNG_connect:
                    RNG.add_edge(v1, v2)
                if GG_connect:
                    GG.add_edge(v1, v2)

        if args.graphtype == 'RNG':
            logger.info('Create RNG files')
            create_graph_files(RNG, points, args, logger, argsinfo)
        else:
            logger.info('Create GG files')
            create_graph_files(GG, points, args, logger, argsinfo)
    elif args.graphtype == 'TG':
        logger.info('Crete TG files')
        create_graph_files(TG, points, args, logger, argsinfo)
    else:
        logger.error(f'Wrong graph type: {args.graphtype}')

    
if __name__ == '__main__':
    main() 