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
    void shortestPathAtVertex(QString startCity, QString endCity);
    void resetShortestPath();
    void printVector();
    void visitStadium(QString stadiumName);
    void resetVisitedVector();

    QVector<QString> getShortestPathTraversal() const;

    int getShortestPathWeight() const;

    int getGraphSize() const;

    QVector<QString> getVisited() const;

    QVector<Vertex> getGraph() const;

private:
    int              graphSize; //Graph size
    QVector<Vertex>  graph;     //Vector to create adjacency list
    struct traversalInfo
    {
        QStringList traversal;
        int distanceTraveled;
    } traversalInfo;            //Holds the list of vertices that were traversed during DFS/BFS/Shortest Path and distance traveled
    QVector<QString> visited;   //Holds the visited nodes for the DFS/BFS

    QVector<QString> shortestPathTraversal; //Holds the most recent shortest path traversal
    int shortestPathWeight;                 //Holds the weight of the most recent path traversal


    //Returns the address of the vertex from the label passed in
    Vertex* getVertexFromString(QString searchLabel);
    //Returns the address of the vertex from the id passed in
    Vertex* getVertexFromId(int searchId);

    //Performs the dfs
    void dfs(Vertex *startVertex);
    void bfs(Vertex *startVertex);
    void shortestPath(Vertex *startVertex, Vertex *endVertex);

    void printPath(int *parent, int startVertexId);

};


#endif // GRAPH_H
