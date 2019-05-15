// Jacob Stelzriede - 1436063
// CMPE 101 - UC Santa Cruz
// Fall 2018
// Programming Assignment 5: Graph Theory in C
//
// 
//
//
//
//
#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "List.h"

// Data structure
typedef struct GraphObj
{
	List* neighbors;
	int* color;
	int* parent;
	int* discover;
	int* finish;
	int order;
	int size;
	int source;
}GraphObj;

// helper function
void visit(Graph G, List S, int u, int *t);

/*** Constructors-Destructors ***/
// returns a new graph G with n vertices.
Graph newGraph(int n)
{
	Graph G = malloc(sizeof(GraphObj));
	G -> neighbors = calloc(n+1, sizeof(List));
	G -> color = calloc(n+1, sizeof(int));
	G -> parent = calloc(n+1, sizeof(int));
	G -> discover = calloc(n+1, sizeof(int));
	G -> finish = calloc(n+1, sizeof(int));
	G -> order = n;
	G -> size = 0;
	G -> source = NIL;
	//initialize Graph G to NIL, INF and WHITE on all Vertices
	for(int i = 1; i<=n; i++)
	{
		G -> neighbors[i] = newList();
		G -> color[i] = WHITE;
		G -> parent[i] = NIL;
		G -> discover[i] = UNDEF;
		G -> finish[i] = UNDEF;
	}
	return G;
}

//
void freeGraph(Graph* pG)
{
	for(int i = 0; i <= (*pG) -> order; i++)
	{
		freeList(&(*pG) -> neighbors[i]);
	}
	free((*pG) -> neighbors); 
	free((*pG) -> color); 
	free((*pG) -> parent); // G -> parent = calloc(n+1, sizeof(int));
	free((*pG) -> discover); // G -> distance = calloc(n+1, sizeof(int));
	free((*pG) -> finish);

	(*pG) -> neighbors = NULL;
	(*pG) -> color = NULL;
	(*pG) -> parent = NULL;
	(*pG) -> discover = NULL;
	(*pG) -> finish = NULL;	

	free(*pG);
	*pG = NULL;
}

/*** Access functions ***/
// returns the number of vertices in graph G
int getOrder(Graph G)
{
	if(G == NULL)
	{
		printf("Graph.getOrder.Error: NULL Graph");
		exit(1);
	}
	return G -> order;
}

// returns the number of edges in graph G
int getSize(Graph G)
{
	/*
	if(G == NULL)
	{
		printf("Graph.getSize.Error: NULL Graph");
		exit(1);
	}
	*/
	return G -> size;
}

// returns the source vertex of G
int getSource(Graph G)
{
	if(G == NULL)
	{
		printf("Graph.getSource.Error: NULL Graph");
		exit(1);
	}
	return G -> source;
}

// returns the parent of vertex u
int getParent(Graph G, int u)
{
	if(G == NULL)
	{
		printf("Graph.getParent.Error: NULL Graph");
		exit(1);
	}
	if(u < 1 || u > getOrder(G)) //|| u != NIL
	{
		printf("Graph.getParent.Error: No Parent Exists");
		exit(1);
	}
	return G -> parent[u];
}

// 
int getDiscover(Graph G, int u)
{
	if(G == NULL)
	{
		printf("Graph.getDiscover.Error: NULL Graph");
		exit(1);
	}
	if(u < 1 || u > getOrder(G))
	{
		printf("Graph.getDiscover.Error: No Parent Exists");
		exit(1);
	}
	return G -> discover[u];
}

//
int getFinish(Graph G, int u)
{
	if(G == NULL)
	{
		printf("Graph.getFinish.Error: NULL Graph");
		exit(1);
	}
	if(u < 1 || u > getOrder(G))
	{
		printf("Graph.getFinish.Error: No Parent Exists");
		exit(1);
	}
	return G -> finish[u];
}

/*** Manipulation procedures ***/
//
void makeNull(Graph G)
{
	if(G == NULL)
	{
		printf("Graph.makeNull.Error: NULL Graph");
		exit(1);
	}
	//for(int i = 0; i <= getSize(G); i++)
	for(int i = 0; i <= getOrder(G); i++)
	{
		clear(G -> neighbors[i]);
	}G-> size = 0;
}

// add an edge between u and v, add both to adj list.(run addArc on both)
void addEdge(Graph G, int u, int v)
{
	if(G == NULL)
	{
		printf("Graph.addEdge.Error: NULL Graph");
		exit(1);
	}
	if(u < 1 || u > getOrder(G))
	{
		printf("Graph.addEdge.Error: Path undiscovered, arg[1].");
		exit(1);
	}
	if(v < 1 || v > getOrder(G))
	{
		printf("Graph.addEdge.Error: Path undiscovered, arg[2].");
		exit(1);
	}
	// from u to v
	moveFront(G->neighbors[u]);
	while(index(G->neighbors[u]) > -1 && v > get(G->neighbors[u]))
	{
		moveNext(G->neighbors[u]);
	}
	if(index(G->neighbors[u]) == -1)
	{
		append(G->neighbors[u], v);
	}
	else
	{
		insertBefore(G->neighbors[u], v);
	}

	// from v to u
	moveFront(G->neighbors[v]);
	while(index(G->neighbors[v]) > -1 && u > get(G->neighbors[v]))
	{
		moveNext(G->neighbors[v]);
	}
	if(index(G->neighbors[v]) == -1)
	{
		append(G->neighbors[v], u);
	}
	else
	{
		insertBefore(G->neighbors[v], u);
	}
	G -> size++;
}

// add an directed edge between u and v, only add 1 to adj list.
void addArc(Graph G, int u, int v)
{
	if(G == NULL)
	{
		printf("Graph.addArc.Error: NULL Graph");
		exit(1);
	}
	if(u < 1 || u > getOrder(G))
	{
		printf("Graph.addArc.Error: Path undiscovered.");
		exit(1);
	}
	if(v < 1 || v > getOrder(G))
	{
		printf("Graph.addArc.Error: Path undiscovered.");
		exit(1);
	}
	// same as addEdge() but only one way.
	// from u to v
	moveFront(G->neighbors[u]);
	while(index(G->neighbors[u]) > -1 && v > get(G->neighbors[u]))
	{
		moveNext(G->neighbors[u]);
	}
	if(index(G->neighbors[u]) == -1)
	{
		append(G->neighbors[u], v);
	}
	else
	{
		insertBefore(G->neighbors[u], v);
	}
	G -> size++;	
}

// First part of DFS, visit to follow: List S = order of process
void DFS(Graph G, List S)
{
	//run error checks
	//DFS linen 1-4
	int t = 0; //time variable
	for(int i = 1; i <= getOrder(G); i++)
	{
		G -> color[i] =  WHITE;
		G -> parent[i] = NIL;
	}	
	moveFront(S);
	while(index(S) >= 0)
	{
		int x = get(S);
		if(G -> color[x] == WHITE)
		{
			visit(G, S, x, &t);
		}
		moveNext(S);
	}
	// not sure where to handle stack
	// remove back item from Stack here 
	// add them at the end of Visit
	for(int i = 0; i<getOrder(G); i++)
	{
		deleteBack(S);
	}
}
// DFS-Visit() using second method from 
void visit(Graph G, List S, int x, int *t)
{
	//error check
	G -> color[x] = GRAY;
	G -> discover[x] = ++*t;
	moveFront(G -> neighbors[x]);
	while(index(G -> neighbors[x]) >= 0)
	{
		int y = get(G -> neighbors[x]);
		if(G -> color[y] == WHITE)
		{
			G -> parent[y] = x;
			//visit(G, S, y, *t);
			visit(G, S, y, t);
		}
		moveNext(G -> neighbors[x]);
	}
	G -> color[x] = BLACK;
	G -> finish[x] = ++*t;
	// handle stack
	prepend(S,x);
}


/*** Other operations ***/
void printGraph(FILE* out, Graph G)
{
	for(int i = 1; i <= getOrder(G); i++)
	{
		List N = G -> neighbors[i];
		fprintf(out, "%d: ", i);
		moveFront(N);
		while(index(N) != -1)
		{
			fprintf(out, "%d ", get(N));
			moveNext(N);
		}
		//printList(out, G->neighbors[i]);
		fprintf(out, "\r\n");
	}
}

//
Graph transpose(Graph G)
{
	if(G == NULL)
	{
		fprintf(stdout, "G is null\n");
	}

	Graph primeG = newGraph(getOrder(G));
	//fprintf(stdout,"test1 in transpose: order passed %d\n",getOrder(G));
	for(int i = 1; i<=getOrder(primeG); i++)
	{
		moveFront(G -> neighbors[i]);
		if(length(G -> neighbors[i]) == 0)
		{
			continue;
			//return tempG;
		}
		else
		{
			while(index(G -> neighbors[i]) != -1)
			{
				addArc(primeG, get(G -> neighbors[i]), i);
				moveNext(G -> neighbors[i]);
			}
		}
	}
	return primeG;
}
/* CE13 code
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[j][i] = mat1[i][j];
        }
    }
*/

//
Graph copyGraph(Graph G)
{
	Graph tempA = newGraph(getOrder(G));
	for(int i = 1; i <= getOrder(G); i++)
	{
		moveFront(G -> neighbors[i]);
		if(length(G -> neighbors[i]) == 0)
		{
			continue;
			//return tempA;
		}
		else
		{
			while(index(G -> neighbors[i]) >= 0)
				{
					addArc(tempA, i, get(G -> neighbors[i]));
					moveNext(G -> neighbors[i]);
				}
		}
	}
	return tempA;
}