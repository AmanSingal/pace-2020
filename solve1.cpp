/* Algorithm working:-
1) Pick the node which has maximum no of edges initially(let's say 'X')
2) Call Decompostion function for that node
3) This function will find out vertices that has max edges(let's say Y1, Y2,....as many as components are there) in their respected components. It
also marks those vertices with the vertex(Y1, Y2,......) to indicate to which component they belong to.
4) This function then stores these values (Y1,Y2,....) in a queue
5) The function is then called for these values
6) Lastly we will find the bottommost leaf node and by that point all nodes have been marked with
their parent node

Logic behind this approach - Picking max edge vertex everytime ensures that max no of components are obtained. This will
increase the width of the final tree and as the width increases the depth will decrease and we
can get a good approximation of true solution*/

#include <bits/stdc++.h>
using namespace std;

//....Making adjacency list and hashmap stores the value of edges connected to that vertex
void addEdge(map<unsigned long int,unsigned long int> &mymap,vector<int> adj[], int &u, int &v){
    adj[u].emplace_back(v);
    adj[v].emplace_back(u);
    mymap[u]++;
    mymap[v]++;
    }


//...This routine deletes the selected vertex passed by argument max(which includes from adjacency list and map as well)
void delDecomposition(int masti[],int &arg_max, vector<int> adj[], map<unsigned long int,unsigned long int> &mymap, int &V, int timepass[], queue <unsigned long int> &q);

//...This routine is used for connected components//....More about this will be in its definition
void Decomposition(int masti[],int &arg_max, vector<int> adj[], map<unsigned long int,unsigned long int> &mymap, int &V, int timepass[], queue <unsigned long int> &q);

//....This function is used performing DFS starting from the vertex passed as 'v'
void DFSUtil(int &v, bool visited[], int &max,int &nextNode, int timepass[],map<unsigned long int,unsigned long int> &mymap,vector<int> adj[],int masti[],int &arg_max);


//..........These two functions below are used to find out the depth of tree passed using parent[] array (which has the same format as asked in solution i.e., value at ith index is parent of i)
int fillHeight(int p[], int node, int visited_new[], int height[]) { 
	if (p[node] == 0) { 
		visited_new[node] = 1; 
		return 0; 
	}  
	if (visited_new[node]) 
	return height[node]; 
	visited_new[node] = 1;
    if(p[node]!=-1){
	height[node] = 1 + fillHeight(p, p[node], visited_new, height);} 
	return height[node]; 
} 
int findHeight(int parent[], int n) 
{ 
	int ma = 0; 
	int visited_new[n]; 
	int height[n]; 
	memset(visited_new, 0, sizeof(visited_new)); 
	memset(height, 0, sizeof(height)); 
	for (int i = 0; i < n; i++) { 
		if (!visited_new[i]) 
			height[i] = fillHeight(parent, i, 
							visited_new, height); 
		ma = max(ma, height[i]); 
	}
	return ma; 
} 
//...............................................................................


 int main(){
    int v,e;// v is used to store no of vertices, e is used to store no of edges
    string str,T;
    int a,b;
    stringstream Y;
    int z=0;
    while(1){
        getline(cin,str);
        if(str[0]=='p'){

            stringstream X(str);
            while(getline(X,T,' ')){

            if(z==2){
                v = stoi(T);
            }
            if(z==3){
                e = stoi(T);
            }
            z++;}

            break;
        }
    }
//......................UPTILL THIS Point VALUE OF VERTICES AND EDGES ARE OBTAINED (STORED IN v and e)

    vector<int> adj[v+1];

//..................Initializing the value of map 
    map<unsigned long int,unsigned long int> mymap;
        for(int i=0; i<=v;i++){
        mymap.insert(pair<int,int>(i,0));
    }

    for(int i =0; i<e;){
            getline(cin,str);

            if(str[0]!='c'){
                stringstream F(str);
                    int z1=0;
                while(getline(F,T,' ')){

                    if(z1==0){
                    a = stoi(T);
    
               }
                    if(z1==1){
                        b=stoi(T);
                    }
                    z1++;
                }
               
                addEdge(mymap,adj,a,b);
                i++;
            }
    }
//.......................Uptill this point both adjacency list and map are created
    
//.............This part finds the initial max value in hashmap i.e, the vertex that has max no of edges connected to it (returned by arg_max)
    int currentMax=0;
    int arg_max=0;
    for(auto it = mymap.cbegin(); it != mymap.cend(); ++it ) {
    if (it ->second > currentMax) {
        arg_max = it->first;
        currentMax = it->second;
    }
    }

//....masti is the final array that will contain info about parent of each index
//....timepass will be used as an indication to detect which all vertices belong to a particular component.......It's usage is explained later
    int masti[v+1],timepass[v+1];
 
//....Initializing the value for both masti and timepass
    for(int i=1; i<=v;i++){
        masti[i]=-1;
        timepass[i]=arg_max;// initial value of each index in time pass is made arg_max as initially we are given a connected graph in which all vertices are connected
    }

//...... q stores info about which vertex to be processed for connected component check//.. Explained later
    queue<unsigned long int> q;

//....This function is called by the initial value of vertex that has max no of edges connected
    Decomposition(masti,arg_max, adj,mymap,v,timepass,q);
    

    masti[arg_max]=0;// Set the value of root node to be zero(as of output format)
    masti[0]=-1;// as no vertex is marked by 0 so this assignment is just to separate it from rest vertex...Suggesting there is no vertex as '0'
   
// Below 2 statements print the final output
    cout<<findHeight(masti,v+1)+1<<endl;
    for(int i=1; i<sizeof(masti)/sizeof(masti[0]); i++){ 
        cout<<masti[i]<<endl;}   

    return 0;
}



void delDecomposition(int masti[],int &arg_max, vector<int> adj[], map<unsigned long int,unsigned long int> &mymap, int &V, int timepass[], queue <unsigned long int> &q){
    adj[arg_max].clear();//......Clears the vertex which is passed 'arg_max'
    mymap[arg_max]=0;//......Makes edges in edge map of vertex passed as 'arg_max' to be zero
    vector<int> :: iterator it;//......Removes that vertex 'arg_max' from all other adjacency lists and decrements count in hashmap of edges
    for(int i=1; i<=V;i++){
        it = find(adj[i].begin(),adj[i].end(),arg_max);
        if(it != adj[i].end()){
            adj[i].erase(it);
            mymap[i]--; // Decrements count in edge map of all vertices connected to 'arg_max' as the vertex is removed now
        }
    }
}

void Decomposition(int masti[],int &arg_max, vector<int> adj[], map<unsigned long int,unsigned long int> &mymap, int &V, int timepass[], queue <unsigned long int> &q){
 
 if(masti[arg_max]==-1 || (adj[arg_max].size()>0)) //  This condition checks whether already parent is assigned to arg_max or arg_max is deleted or not
 { 
    
    delDecomposition(masti,arg_max,adj,mymap,V,timepass,q); // Deletes arg_max node

//...This array stores info whether the node was visited or not while performing DFS taking 'arg_max' as the starting point
    bool* visited = new bool[V+1]; 
    for(int v = 1; v <= V; v++){ 
        //if(mymap[v]>0 || masti[v]==-1 ){
            visited[v] = 0;
        //}
    }

    //int marked[V]; // ... yeh galti se bann gya tha iska koi kaam nhi hai... Ignore kar dena isko

    for (int v=1; v<=V; v++) // Iterating through all nodes for checking connected components
    { 
        if ((visited[v] == false) && (timepass[v] == arg_max) && (masti[v] ==-1) ) // Checks if the node is visited or not, whether grouped in any category by 'timepass' and whether parent node is assigned to that or not
        {   int max=0; //max stores the max value of adjacent edges for a vertex
            int nextNode; // stores the info about vertex with max edge present in a particular component of connected components
            DFSUtil(v, visited, max,nextNode,timepass, mymap, adj,masti,arg_max); 
            if(timepass[v]==-10){
        // if the value of timepass changes for a particular vertex then make that vertex value as nextNode
        //This way all vertex present in a single component are marked uniquely by the value of the vertex present in that component with max no of edges
                timepass[v]=nextNode;
            }
// The max of each components are made the child for the vertex that was considered
            masti[nextNode]= arg_max;

            if(mymap[nextNode]>0){// If nextNode has still some adjacent edges only then consider it for further evaluation
               q.push(nextNode);
            }
        } 
    }
    delete [] visited;
    int aese;
    while(!q.empty()){ 
         if(arg_max==0){ // Not necessary // Ignore it
            break;
        } 
        aese= q.front();// It assigns the front element value to aese
        q.pop(); // removes the front element
        vector<int>::iterator it2; 
        if(mymap[aese]>0){
        for(it2 = adj[aese].begin(); it2 != adj[aese].end(); ++it2){
            //if(it2 != adj[aese].end())
            timepass[*it2]=aese; // This condition will mark all adjacent edges of aese as we did earlier
        }  
        }
        Decomposition(masti,aese,adj,mymap,V,timepass,q);// Recursively doing it for other vertices as well
       // delete aese;
        }
  
}
}

void DFSUtil(int &v, bool visited[], int& max,int& nextNode, int timepass[],  map<unsigned long int,unsigned long int> &mymap,vector<int> adj[],int masti[],int &arg_max) 
{   
    visited[v] = true; // mark the vertex as visited
    if(v!=arg_max){
        timepass[v]=-10; // assign some unique value to the connected vertices in that component
    }

    if(max<mymap[v]){
        max = mymap[v]; // this condition checks for the vertex which has maximum no of edges connected to it in that component
        nextNode=v;
    }
    if(mymap[v]>0){ // Recursively performing DFS for vertex v if it has some adjacent vertex (assured by mymap[] value >0 )
        vector<int>::iterator i; 
        for(i = adj[v].begin(); i != adj[v].end(); ++i) 
            if(!visited[*i] && mymap[*i]>0) 
                DFSUtil(*i, visited,max,nextNode,timepass,mymap,adj,masti,arg_max); 
    }
    else if(max==0){ // If only single vertex is found, make arg_max as its parent node
        masti[v] = arg_max;
        nextNode = v;
    }
}  