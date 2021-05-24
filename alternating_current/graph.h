#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include <QMap>
#include <QVector>
#include <QQueue>

typedef QMap<int, QString> Link;

typedef QMapIterator<int, QString> LinksIterator;
typedef QMutableMapIterator<int, QString> MutableLinksIterator;

typedef QMapIterator<QString, Link> GraphIterator;
typedef QMutableMapIterator<QString, Link> MutableGraphIterator;


class Graph : public QObject
{
    Q_OBJECT
    friend class AppTest;

public:
    Graph();
    Graph(const Graph &);
    ~Graph();

private:
    QMap<QString, Link> graph;

public:

    bool operator==(const Graph & other) const;
    /*! Функция удаляет все ветви графа
    */
    void clear();
    /*! Перегрузка оператора =
        \param[in]
        \return
    */
    Graph & operator=(const Graph & right);
};

#endif // GRAPH_H
