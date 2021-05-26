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
    /*! Добавляет ветвь в граф
        \param[in] begin - узел, из которого исходит ветвь
        \param[in] edge - номер ветви
        \param[in] end - узел, в который заходит ветвь
    */
    void insertEdge(const QString & begin, int edge, const QString & end);
    /*! Добавляет узел в граф
        \param[in] node -
    */
    void insertNode(const QString & node);
    /*! Функция проверяет, содержит ли граф узел node
        \param[in] node -
        \return -
    */
    bool containsNode(const QString & node) const;
    /*! Функция проверяет, содержит ли граф ветвь edge
        \param[in] edge -
        \return -
    */
    bool containsEdge(int edge) const;
    /*! Функция выделяет в графе остовное дерево
        \param[in, out] tree -
        \param[in, out] links -
    */
    void getSkeletalTree(Graph & tree, QVector<int> & links);
    /*! Функция ищет путь в графе из узла begin в узел goal
        \param[in] begin -
        \param[in] goal -
        \param[in, out] path -
        \param[in, out] visited -
        \return -
    */
    bool findPath(const QString & begin, const QString & goal,
        QVector <int> & path, QVector<QString> visited = QVector<QString>()) const;
    /*! Функция возвращает все ветви, исходящие из узла node и узлы на другом конце этих ветвей
        \param[in] node -
        \return
    */
    Link getLinks(const QString & node) const;
    /*! Функция проверяет односвязность графа
        \return -
    */
    bool isSimpleConnected() const;
    /*! Ищет висячие узлы в графе, то есть те узлы, которые инцидентны одной ветви (за исключением петли)
        \param[in, out] list -
    */
    void findLeafNode(QVector<QString> & list);
    /*! Функция возвращает матрицу смежности графа
        \return -
    */
    const QMap<QString, Link> & getGraph() const;
    /*! Перегрузка оператора ==
        \param[in]
        \return
    */
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
