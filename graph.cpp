#include "graph.h"

Graph::Graph()
{
    graph.clear();
    graphSize = 0;
}

/*!
 * \fn brief Graph::createGraph
 */
void Graph::createGraph()
{
    QSqlQuery beginningStadiumQuery; //Query to get the stadiums
    QSqlQuery stadiumInfoQuery;      //Query to get ending stadium and weight
    QString   currentStadium;        //Keeps track of the current stadium
    Vertex    newVertex;             //Used to make new vertexes
    Edge      newEdge;               //Used to make new edges
    int       newId;                 //Used to set the id of a vertex

    //Initialization
    newId     = 0;

    //Queries the database for distinct beginning stadiums
    beginningStadiumQuery.prepare("SELECT DISTINCT BeginningStadium FROM Distances ORDER BY BeginningStadium ASC");
    beginningStadiumQuery.exec();

    //Adds the beginning stadiums as vertices of the graph
    while(beginningStadiumQuery.next())
    {
        newVertex.label = beginningStadiumQuery.value(0).toString();
        newVertex.id    = newId;
        newVertex.incidentEdges.clear();
        graph.push_back(newVertex);
        newId++;
    }

    //Sets the size of the graph depending on how many id's were created
    graphSize = newId;

    //Loops over the vertices of the graph created earlier
    for(QVector<Vertex>::iterator graphIt = graph.begin(); graphIt != graph.end(); graphIt++)
    {
        //Gets the connected vertices from the database sorted by smallest to largest distance
        stadiumInfoQuery.prepare("SELECT EndingStadium, Distance FROM Distances WHERE BeginningStadium=:stadium ORDER BY Distance ASC");
        stadiumInfoQuery.bindValue(":stadium", graphIt->label);
        stadiumInfoQuery.exec();

        //Adds the connected vertices of the current node
        while(stadiumInfoQuery.next())
        {
            newEdge.connectedVertex = nullptr;
            newEdge.connectedVertex = getVertexFromString(stadiumInfoQuery.value(0).toString());
            newEdge.weight = stadiumInfoQuery.value(1).toInt();
            graphIt->incidentEdges.push_back(newEdge);
        }

        stadiumInfoQuery.clear();
    }
}

void Graph::printGraph()
{
    QVector<Vertex>::iterator graphIt;
    QVector<Edge>::iterator   edgeIt;

    qDebug() << "Printing graph...\n";

    for(graphIt = graph.begin(); graphIt != graph.end(); graphIt++)
    {
        qDebug() << graphIt->label << "(" << graphIt->id << ") -> ";

        for(edgeIt = graphIt->incidentEdges.begin(); edgeIt != graphIt->incidentEdges.end(); edgeIt++)
        {
            qDebug() << "(" << edgeIt->connectedVertex->label << ", " << edgeIt->connectedVertex->id << ", " << edgeIt->weight << ")\t";
        }
    }
}

void Graph::dfsAtVertex(QString label)
{
    Vertex* start = getVertexFromString(label);
    traversalInfo.distanceTraveled = 0;
    traversalInfo.traversal.clear();
    visited.clear();

    dfs(start);

    for(int i = 0; i < traversalInfo.traversal.length(); i++)
        qDebug() << traversalInfo.traversal[i];

    qDebug() << traversalInfo.distanceTraveled;
}

void Graph::bfsAtVertex(QString label)
{
    Vertex *start = getVertexFromString(label);
    traversalInfo.distanceTraveled = 0;
    traversalInfo.traversal.clear();
    visited.clear();

    bfs(start);

    for(int i = 0; i < traversalInfo.traversal.length(); i++)
        qDebug() << traversalInfo.traversal[i];

    qDebug() << traversalInfo.distanceTraveled;
}

void Graph::shortestPathAtVertex(QString startCity, QString endCity)
{
    Vertex *start = getVertexFromString(startCity);
    Vertex *end = getVertexFromString(endCity);

    this->shortestPathTraversal.push_back(startCity);

    shortestPath(start, end);
}

void Graph::resetShortestPath()
{
    this->shortestPathTraversal.clear();
    this->shortestPathWeight = 0;
}

void Graph::printVector()
{
    for(int i = 0; i < shortestPathTraversal.size(); i++)
    {
        qDebug() << shortestPathTraversal[i];
    }
}

void Graph::visitStadium(QString stadiumName)
{
    this->visited.push_back(stadiumName);
}

void Graph::resetVisitedVector()
{
    this->visited.clear();
}

void Graph::updateGraph()
{
    this->graph.clear();
    this->createGraph();
}

QVector<QString> Graph::getShortestPathTraversal() const
{
    return shortestPathTraversal;
}

int Graph::getShortestPathWeight() const
{
    return shortestPathWeight;
}

int Graph::getGraphSize() const
{
    return graphSize;
}

QVector<QString> Graph::getVisited() const
{
    return visited;
}

QVector<Vertex> Graph::getGraph() const
{
    return graph;
}

QStringList Graph::getTraversalInfoTraversal()
{
    return traversalInfo.traversal;
}

void Graph::dfs(Vertex *startVertex)
{
    visited.push_back(startVertex->label);
    traversalInfo.traversal.append(startVertex->label);

    //Loop over the incident edges of the vertex
    for(QVector<Edge>::iterator edgeIt = startVertex->incidentEdges.begin(); edgeIt != startVertex->incidentEdges.end(); edgeIt++)
    {
        if(!visited.contains(edgeIt->connectedVertex->label))
        {
            traversalInfo.distanceTraveled += edgeIt->weight;
            dfs(edgeIt->connectedVertex);
        }
    }
}

void Graph::bfs(Vertex *startVertex)
{
    QQueue<Vertex*> vertexQ;
    Vertex *currentVertex = startVertex;

    for(int i = 0; i < graphSize; i++)
    {
        if(!visited.contains(currentVertex->label))
        {
            vertexQ.push_back(currentVertex);
            visited.push_back(currentVertex->label);
            traversalInfo.traversal.append(currentVertex->label);

            while(!vertexQ.empty())
            {
                currentVertex = vertexQ.front();
                vertexQ.pop_front();

                for(QVector<Edge>::iterator edgeIt = currentVertex->incidentEdges.begin(); edgeIt != currentVertex->incidentEdges.end(); edgeIt++)
                {
                    if(!visited.contains(edgeIt->connectedVertex->label))
                    {
                        vertexQ.push_back(edgeIt->connectedVertex);
                        visited.push_back(edgeIt->connectedVertex->label);
                        traversalInfo.traversal.append(edgeIt->connectedVertex->label);
                        traversalInfo.distanceTraveled += edgeIt->weight;
                    }
                }
            }
        }
    }
}

void Graph::shortestPath(Vertex *startVertex, Vertex *endVertex)
{
    Vertex *vertex = NULL;

    //Parent array to store shortest path tree
    int parent[graphSize];

    //Used to represent weight and vertex id in that order
    typedef std::pair<int, int> iPair;

    //Priority queue to store vertices that are being preprocessed
    std::priority_queue< iPair, QVector<iPair>, std::greater<iPair> > pq;

    //Creating a vector for distances and initialize all distances as infinite
    QVector<int> dist(graphSize, std::numeric_limits<int>::max());

    //Insert source itself in pq and initialize its distance as 0
    pq.push(std::make_pair(0, startVertex->id));
    dist[startVertex->id] = 0;
    parent[startVertex->id] = -1;

    //Loops until pq becomes empty
    while(!pq.empty())
    {
        //Extracts the minumum distance vertex from priority queue
        int vertexId = pq.top().second;
        pq.pop();

        vertex = getVertexFromId(vertexId);

        //Loops over the connect vertices
        QVector<Edge>::iterator vectorIt;
        for(vectorIt = vertex->incidentEdges.begin(); vectorIt != vertex->incidentEdges.end(); vectorIt++)
        {
            //Get the current adjacent vertex id and the weight of the edge connecting them
            int connVertexId = vectorIt->connectedVertex->id;
            int weight       = vectorIt->weight;

            // If there is a shorter path to the connected vertex through the current vertex
            if(dist[connVertexId] > dist[vertexId] + weight)
            {
                //Updating distance of connected vertex
                dist[connVertexId] = dist[vertexId] + weight;
                parent[connVertexId] = vertexId;
                pq.push(std::make_pair(dist[connVertexId], connVertexId));
            }
        }
    }

//    for(int i = 0; i < graphSize; i++)
//    {
//        Vertex *current = getVertexFromId(i);
//        qDebug() << "A path from " << startVertex->label << " to " << current->label << ": ";

//        qDebug() << startVertex->label << " ";
    printPath(parent, endVertex->id);
    shortestPathWeight = dist[endVertex->id];
//        qDebug() << "cost: (" << dist[i] << ")\n";
//    }
}

Vertex *Graph::getVertexFromString(QString searchLabel)
{
    for(QVector<Vertex>::iterator graphIt = graph.begin(); graphIt != graph.end(); graphIt++)
    {
        if(graphIt->label == searchLabel)
        {
            return graphIt;
        }
    }
}

Vertex *Graph::getVertexFromId(int searchId)
{
    for(QVector<Vertex>::iterator graphIt = graph.begin(); graphIt != graph.end(); graphIt++)
    {
        if(graphIt->id == searchId)
        {
            return graphIt;
        }
    }
}

void Graph::printPath(int *parent, int startVertexId)
{
    //Base case : if ending vertex is the source
    if(parent[startVertexId] == -1)
        return;

    printPath(parent, parent[startVertexId]);

    Vertex* temp = getVertexFromId(startVertexId);
    shortestPathTraversal.push_back(temp->label);
}
