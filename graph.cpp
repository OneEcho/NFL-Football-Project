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
    graphSize = 31;
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

QVector< std::pair<QString, int> > Graph::shortestPathAtVertex(QString label)
{
    Vertex *start = getVertexFromString(label);

    QVector< std::pair<QString, int> > shortestPathInfo = shortestPath(start);

    return shortestPathInfo;
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

QVector< std::pair<QString, int> > Graph::shortestPath(Vertex *startVertex)
{
    Vertex *vertex = NULL;
    //Used to represent weight and vertex id in that order
    typedef std::pair<int, int> iPair;

    //Priority queue to store vertices that are being preprocessed
    std::priority_queue< iPair, QVector<iPair>, std::greater<iPair> > pq;

    //Creating a vector for distances and initialize all distances as infinite
    QVector<int> dist(graphSize, std::numeric_limits<int>::max());

    //Insert source itself in pq and initialize its distance as 0
    pq.push(std::make_pair(0, startVertex->id));
    dist[startVertex->id] = 0;

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
                pq.push(std::make_pair(dist[connVertexId], connVertexId));
            }
        }
    }
    QVector< std::pair<QString, int> > vertexDistance;

    for(int i = 0; i < graphSize; i++)
    {
        Vertex* temp = getVertexFromId(i);
        vertexDistance.push_back(std::make_pair(temp->label, dist[i]));
        temp = NULL;
    }

    vertex = NULL;

    return vertexDistance;
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
