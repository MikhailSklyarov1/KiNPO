#include "graph.h"
#include <QVector>

Graph::Graph(const Graph & other)
{
    QString k;
    Link v;
    this->graph.clear();
    GraphIterator i(other.graph);
    while(i.hasNext())
    {
        k = i.next().key();
        v = i.value();
        this->graph.insert(k, v);
    }
}

Graph::Graph()
{

}

Graph::~Graph()
{

}







bool Graph::operator==(const Graph & other) const
{
    return this->graph == other.graph;
}

const QMap<QString, Link> & Graph::getGraph() const
{
    return graph;
}

void Graph::clear()
{
    graph.clear();
}

Graph & Graph::operator=(const Graph & right)
{
    this->graph = right.graph;
    return *this;
}
