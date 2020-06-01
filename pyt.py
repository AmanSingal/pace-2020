# from __future__ import division, print_function

import networkx as nx
from collections import defaultdict
from collections import deque
import sys

sys.setrecursionlimit(10**9)
G = nx.Graph()
n, m = 0, 0
masti = defaultdict(lambda:0)
mymap = defaultdict(lambda:0)

def getGraph():
    global n,m,G,mymap
    edge =  0
    line = input()
    ch = line[0]
    while ch == 'c' or ch:
        if ch == 'p':
            n, m = map(int, line.split()[-2:])
            break
        line = input()
        ch = line[0]
    while edge != m:
        line = input()
        if len(line) ==0 or line[0] == 'c':
            continue
        u, v = map(int, line.split())
        G.add_node(u)
        G.add_node(v)
        G.add_edge(u,v)
        G.add_edge(v,u)
        mymap[u] = mymap[u] + 1
        mymap[v] = mymap[v] + 1
        edge += 1

def DFSUtil(i, visited, timepass, value):
    global mymap, n, G
    visited[i] = True
    if(i != value):
        timepass[i] = -10
    if(mymap[i]>0):
        for nbr in G[i]:
            if((visited[nbr] == False) and mymap[nbr] > 0 ):
                DFSUtil(nbr, visited, timepass, value)


def Decomposition(value, timepass,q):
    global masti, n, G, mymap
    a = 0
    if(masti[value]==-1 or G.has_node(value)):
        mymap[value] = 0
        for nbr in G[value]: #Not necessary but keeping it
            mymap[nbr] = mymap[nbr] - 1
        G.remove_node(value)
        visited = defaultdict(lambda:0)
        for i in range(1,n+1):
            visited[i] = False

        for i in range(1,n+1):
            if(mymap[i]==0 and masti[i]==-1):
                    masti[i] = value
            store = defaultdict(lambda:0)
            if((visited[i] == False) and timepass[i] == value and masti[i] == -1 and mymap[i] > 0):
                DFSUtil(i, visited, timepass, value) 
                baby = 0
                for j in range(1,n+1):
                    store[j] = -1
                for h in range(1,n+1):
                    if(timepass[h] == -10):
                        store[h] = 1
                        baby = baby + 1
                        baby1 = h
                if(baby==1):
                    masti[baby1]=value
                    break

                gr = nx.Graph()
                for g in range(1,n+1):
                    if(store[g]==1):
                        for f in range(g+1,n+1):
                            if(store[f]==1):
                                for d in G[g]:
                                    if(d==f):
                                        gr.add_node(g)
                                        gr.add_node(f)
                                        gr.add_edge(g,f)
                                        gr.add_edge(f,g)
                if(gr.number_of_edges() < 4*gr.number_of_nodes() and gr.number_of_nodes() < 800):
                    pr=nx.betweenness_centrality(gr)
                elif(gr.number_of_nodes() < 2000 and 4*gr.number_of_nodes() > gr.number_of_edges()):
                    pr = nx.betweenness_centrality(gr,k=max(1,gr.number_of_nodes()//8))
                elif(gr.number_of_nodes()<5000 and 10*gr.number_of_nodes()> gr.number_of_edges()):
                    pr = nx.betweenness_centrality(gr,k=max(1,gr.number_of_nodes()//32))
                elif(gr.number_of_nodes()<20000 and 10*gr.number_of_nodes()> gr.number_of_edges()):
                    pr = nx.betweenness_centrality(gr,k=max(1,gr.number_of_nodes()//2000))
                elif(gr.number_of_nodes()<50000 and 10*gr.number_of_nodes()> gr.number_of_edges()):
                    pr = nx.betweenness_centrality(gr,k=max(1,gr.number_of_nodes()//20000))
                elif(gr.number_of_nodes()<200000 and 1.5*gr.number_of_nodes()> gr.number_of_edges()):
                    pr = nx.betweenness_centrality(gr,k=max(1,gr.number_of_nodes()//80000))
                elif(gr.number_of_nodes()<600000 and 1.2*gr.number_of_nodes()> gr.number_of_edges()):
                    pr = nx.betweenness_centrality(gr,k=max(1,gr.number_of_nodes()//128000))
                elif(gr.number_of_nodes()<1200000 and 1.1*gr.number_of_nodes()> gr.number_of_edges()):
                    pr = nx.betweenness_centrality(gr,k=max(1,gr.number_of_nodes()//320000))
                else:
                    pr = nx.betweenness_centrality(gr,k=max(1,gr.number_of_nodes()//400000))

                nextNode = max(pr, key = pr.get)

                for m in range(1,n+1):
                    if(timepass[m]==-10):
                        timepass[m] = nextNode
                masti[nextNode] = value
                gr.clear()
                store.clear()
                if(mymap[nextNode]>0):
                    q.append(nextNode)
        visited.clear()
        while(q):
            if(value==0):
                break 
            aese= q[0] 
            q.popleft()
            Decomposition(aese,timepass,q)
                
def findHeight():
    global n, masti
    res = 0
    p=1
    s = ""
    for i in range(1,n+1): 
        p = i 
        current = 1
        #exc = 0
        
        while(masti[p] != -1 and p != masti[p]): 
            current = current + 1
          #  exc = exc + 1
            p = masti[p]
        res = max(res, current)  
    print(res) 

if __name__=='__main__':
# def main():
    # global mymap, masti,n,m,G
    for i in range(1,n+1):
        mymap[i] = 0
    getGraph()
    timepass = defaultdict(lambda:0)
    q = deque() 
    #pr=nx.pagerank(G,0.4)
    if(G.number_of_edges() < 4*G.number_of_nodes() and G.number_of_nodes() < 800):
        pr=nx.betweenness_centrality(G)
    elif(G.number_of_nodes() < 2000 and 4*G.number_of_nodes() > G.number_of_edges()):
        pr = nx.betweenness_centrality(G,k=max(1,G.number_of_nodes()//8))
    elif(G.number_of_nodes()<5000 and 10*G.number_of_nodes()> G.number_of_edges()):
        pr = nx.betweenness_centrality(G,k=max(1,G.number_of_nodes()//32))
    elif(G.number_of_nodes()<20000 and 10*G.number_of_nodes()> G.number_of_edges()):
        pr = nx.betweenness_centrality(G,k=max(1,G.number_of_nodes()//2000))
    elif(G.number_of_nodes()<50000 and 10*G.number_of_nodes()> G.number_of_edges()):
        pr = nx.betweenness_centrality(G,k=max(1,G.number_of_nodes()//20000))
    elif(G.number_of_nodes()<200000 and 1.5*G.number_of_nodes()> G.number_of_edges()):
        pr = nx.betweenness_centrality(G,k=max(1,G.number_of_nodes()//80000))
    elif(G.number_of_nodes()<600000 and 1.2*G.number_of_nodes()> G.number_of_edges()):
        pr = nx.betweenness_centrality(G,k=max(1,G.number_of_nodes()//128000))
    elif(G.number_of_nodes()<1200000 and 1.1*G.number_of_nodes()> G.number_of_edges()):
        pr = nx.betweenness_centrality(G,k=max(1,G.number_of_nodes()//320000))
    else:
        pr = nx.betweenness_centrality(G,k=max(1,G.number_of_nodes()//400000))
    # #pr=nx.closeness_centrality(G)
    # #pr=nx.current_flow_closeness_centrality(G,solver ='lu')
    value = max(pr, key = pr.get)
    #print(value)
    for i in range(1,n+1):
        masti[i] = -1
        timepass[i] = value
    Decomposition(value, timepass,q)
    #print(G.edges())
    masti[value] = -1
    #print("hello")
    q.clear() 
    timepass.clear()
    G.clear()
    #print("hello")
    findHeight()
    mymap.clear()
    # print("hello")
    # # height = height - 1
    # # print(height)
    # print(height)
    for i in range(1,n+1):
        if(masti[i]==-1):
            print('0')
        else:
            print(masti[i])
