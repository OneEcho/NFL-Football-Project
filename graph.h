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

/*! \struct Vertex
    \brief Used to represent a Vertex for the graph class.

    This struct represents a vertex for the graph. It has a label,
    unique ID, and a vector of incident edges.
*/
struct Vertex
{
    Vertex() {}

    QString       label;         //Labels the node
    int           id;            //Unique ID for the vertex
    QVector<Edge> incidentEdges; //Vector to represent adjacent nodes
};

/*! \struct Edge
    \brief Used to represent a Edge for the graph class.

    This struct represents a edge for the graph. It uses a pointer variable
    to hold the connected vertex and integer variable for the weight.
*/struct Edge
{
    Edge() {}
    ~Edge() {connectedVertex = nullptr;}

    Vertex *connectedVertex; //The connected vertex
    int     weight;          //Weight of edge
};

/*! \class Graph
    \brief Used to represent a graph data structure.

    This class represents a graph data structure in memory. Using
    two structs to represent a vertex and edges. It can perform the
    BFS, DFS, and shortest path algorithm.
*/
class Graph
{
public:
    /**
     * @brief Default constructor
     */
    Graph();

    /**
     * @brief Creates the graph using information from the database
     */
    void createGraph();

    /**
     * @brief Prints the graph
     */
    void printGraph();

    /**
     * @brief Performs a DFS traversal
     * @param label Label of the start vertex
     */
    void dfsAtVertex(QString label);

    /**
     * @brief Performs a BFS traversal
     * @param label Label of the start vertex
     */
    void bfsAtVertex(QString label);

    /**
     * @brief Calculates the shortest path from startCity to endCity
     * @param startStadium The beginning stadium for the shortest path
     * @param endCity The end stadium for the shortest path
     */
    void shortestPathAtVertex(QString startStadium, QString endStadium);

    /**
     * @brief Resets the variable holding the shortest path
     */
    void resetShortestPath();

    /**
     * @brief Prints the shortest path vector (used for debugging purposes)
     */
    void printVector();

    /**
     * @brief Adds a stadium to the vector that holds the visited stadiums
     * @param stadiumName The name to add to visited vector
     */
    void visitStadium(QString stadiumName);

    /**
     * @brief Resets the visited vector
     */
    void resetVisitedVector();

    /**
     * @brief Updates the graph by pulling new information from the database
     */
    void updateGraph();

    /**
     * @brief Getter function for the shortest path vector
     * @return The shortest path vector
     */
    QVector<QString> getShortestPathTraversal() const;

    /**
     * @brief Getter function for the shortest path weight
     * @return The total weight of the shortest path
     */
    int getShortestPathWeight() const;

    /**
     * @brief Getter function for the graph size
     * @return The graph size
     */
    int getGraphSize() const;

    /**
     * @brief Getter function for the visited vector
     * @return The visited vector
     */
    QVector<QString> getVisited() const;

    /**
     * @brief Getter function for the vertices vector
     * @return The graph vector
     */
    QVector<Vertex> getGraph() const;

    /**
     * @brief Getter function for the BFS/DFS traversal
     * @return The most recent BFS/DFS traversal
     */
    QStringList getTraversalInfoTraversal();

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
