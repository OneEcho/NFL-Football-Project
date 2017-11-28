#ifndef GRAPH_H
#define GRAPH_H

#include <QString>
#include <QVector>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QQueue>
#include <queue>

struct Edge;

//Used to represent a vertex in a graph
struct Vertex
{
    Vertex() {}

    QString       label;         //Labels the node
    int           id;            //Unique ID for the vertex
    QVector<Edge> incidentEdges; //Vector to represent adjacent nodes
};

//Used to represent an edge in the graph
struct Edge
{
    Edge() {}

    Vertex *connectedVertex; //The connected vertex
    int     weight;          //Weight of edge
};

class Graph
{
public:
    Graph();

    void createGraph();
    void printGraph();

    void dfsAtVertex(QString label);
    void bfsAtVertex(QString label);
    QVector< std::pair<QString, int> > shortestPathAtVertex(QString label);

private:
    int              graphSize; //Graph size
    QVector<Vertex>  graph;     //Vector to create adjacency list
    struct traversalInfo
    {
        QStringList traversal;
        int distanceTraveled;
    } traversalInfo;            //Holds the list of vertices that were traversed during DFS/BFS/Shortest Path and distance traveled
    QVector<QString> visited;   //Holds the visited nodes for the DFS/BFS

    //Returns the address of the vertex from the label passed in
    Vertex* getVertexFromString(QString searchLabel);
    //Returns the address of the vertex from the id passed in
    Vertex* getVertexFromId(int searchId);

    //Performs the dfs
    void dfs(Vertex *startVertex);
    void bfs(Vertex *startVertex);
    QVector <std::pair<QString, int>> shortestPath(Vertex *startVertex);

};


#endif // GRAPH_H
