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





void Graph::insertEdge(const QString & begin, int edge, const QString & end)
{
    //если граф не содержит узел begin
    if(!this->containsNode(begin))
    {

        //добавить узел begin в граф
        graph.insert(begin, Link());
    }
    if(!this->containsNode(end))
    {
        graph.insert(end, Link());
    }

    //добавить в список инцидентных begin ветвей, ветвь edge,заканчивающуюся узлом end
    graph[begin].insert(edge, end);
    graph[end].insert(edge, begin);
}

void Graph::insertNode(const QString & node)
{
    if(!this->containsNode(node))
        //добавить узел begin в граф
        graph.insert(node, Link());
}


bool Graph::containsNode(const QString & node) const
{
    GraphIterator i(graph);
    //проверка по ключам
    while(i.hasNext())
        if(i.next().key() != node)
        {
            //проверка по значениям
            QMapIterator<int, QString> i1(i.value());
            while(i1.hasNext())
                if(i1.next().value() == node)
                    return true;
        }
        else
            return true;
    return false;
}

bool Graph::containsEdge(int edge) const
{
    GraphIterator i(graph);
    while(i.hasNext())
        if(i.next().value().contains(edge))
            return true;
    return false;
}

void Graph::getSkeletalTree(Graph & tree, QVector<int> & links)
{
    QQueue<QString> queue;	//очередь для узлов
    //Добавить первый узел в графе в очередь
    if(graph.isEmpty())
        return;
    queue.append(graph.firstKey());
    //Пока очередь не пустая:
    while(!queue.isEmpty())
    {
        //извлечь первый узел s в очереди и добавить в дерево;
        QString s = queue.first();
        queue.pop_front();

        //поочередно рассмотрим все ветви (s, v), не включенные в дерево
        LinksIterator g(graph.value(s));
        while(g.hasNext())
        {
            QString v = g.next().value();

            //Если узел v не принадлжеит дереву
            if(!tree.containsNode(v) && s != v)
            {
                    queue.append(v); //добавить узел в очередь
                    tree.insertEdge(s, g.key(), v); //добавить (s, v) к дереву
            }

            else if(s == v)
            {
                        links.append(g.key());
            }

            if(s==v && !tree.containsNode(v))
            {
                //добавить (s, v) к дереву
                tree.insertNode(s);
            }

            if(!tree.containsEdge(g.key()) && !links.contains(g.key()))
            {
                links.append(g.key());
            }
        }
    }
}

bool Graph::findPath(const QString & begin, const QString & goal,
    QVector <int> & path, QVector<QString> visited) const
{

    //отметить узел begin как пройденный
    visited.append(begin);
    Link neighbours = this->getLinks(begin);
    if(begin == goal)
    {
        return true;
    }
    //если узлы begin и goal смежны
    if(neighbours.key(goal)!= 0)
    {
        //добавить номер ветви (begin, node) к списку хорд
        path.append(neighbours.key(goal));
        //завершить поиск с результатом "успех"
        return true;
    }
    // Для каждого узла node, смежного узлу begin:
    LinksIterator link(this->getLinks(begin));
    while(link.hasNext())
    {
        QString node = link.next().value(); //смежный узел
        if(!visited.contains(node)) //если узел node не пройден
        {
            if(this->findPath(node, goal, path, visited)) //повторить поиск
            {
                //добавить ветвь (begin, node) в начало списка
                path.prepend(neighbours.key(node));
                //завершить поиск с результатом «успех»
                return true;
            }
        }
    }
    return false;
}

Link Graph::getLinks(const QString & node) const
{
    return graph.value(node);
}

bool Graph::isSimpleConnected() const
{
    QVector<int> path1;

    GraphIterator i(graph);
    GraphIterator j(graph);
    while(i.hasNext())
    {
        i.next();
        j = i;
        while(j.hasNext())

            if(!this->findPath(i.key(), j.next().key(), path1))
                return false;
    }
    return true;
}

void Graph::findLeafNode(QVector<QString> & list)
{
    GraphIterator i(graph);
    while(i.hasNext())
        if(i.next().value().count() == 1)
        {
            if(i.value().first()!=i.key())
            {
                list.append(i.key());
            }
        }
        else if(i.next().value().count() == 0)
        {
            list.append(i.key());
        }
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
