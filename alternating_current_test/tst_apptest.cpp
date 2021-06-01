#include <QtTest>
#include <graph.h>
#include <branch.h>
#include <QObject>
#include <declaration.h>


Q_DECLARE_METATYPE(Element);
Q_DECLARE_METATYPE(ComplexVal);
Q_DECLARE_METATYPE(QVector<Element>);
Q_DECLARE_METATYPE(Branch);
Q_DECLARE_METATYPE(Graph);
Q_DECLARE_METATYPE(Link);


typedef QMap<int, Branch> branchList;
typedef QVector<QMap<int, int>> contourList;
typedef QVector<QVector<ComplexVal>> coefMatr;



class AppTest : public QObject
{
    Q_OBJECT

public:
    AppTest(QObject *parent);
    AppTest();
    ~AppTest();

private slots:
    void initTestCase();
    void initTestCase();
    void cleanupTestCase();
    void test_getSpanningTree();
    void test_getSpanningTree_data();
    void test_findPath();
    void test_findPath_data();


    void test_calculateResistance();
    void test_calculateResistance_data();
    void test_cyclicCurrentsCalculation();
    void test_cyclicCurrentsCalculation_data();
    void test_currentCalculation();
    void test_currentCalculation_data();
    void test_solveEquations();
    void test_solveEquations_data();
    void test_findIndependentContours();
    void test_findIndependentContours_data();


private:
    Branch b;
    QVector<branchList> b_list1; //списки ветвей для 8 тестовых ситуаций
    QVector<branchList> b_list2;
    QVector<contourList> c_list; //списки контуров для 8 тестовых ситуаций
    QVector<QVector<ComplexVal>> ccurr_list;
    QMap<int, int> contour;

    void doData();
};


AppTest::AppTest(QObject *parent)
    : QObject(parent)
{

}

AppTest::AppTest()
{
    b_list1.resize(8);
    b_list2.resize(7);
    c_list.resize(8);
    ccurr_list.resize(8);
    doData();
}

AppTest::~AppTest()
{

}


void AppTest::initTestCase()
{

}

void AppTest::cleanupTestCase()
{

}


template<typename T>
bool compareVects(const QVector<T> & first, const QVector<T> & second)
{
    if(first.count() != second.count())
        return false;
    for(int i = 0; i < first.count(); i++)
        if(!second.contains(first[i]) || second.count(first[i]) != first.count(first[i]))
            return false;
    return true;
}




void AppTest::test_getSpanningTree()
{
    Graph t;
    QVector<int> l;

    QFETCH(Graph, in_graph);
    QFETCH(Graph, tree);
    QFETCH(QVector<int>, links);

    in_graph.getSkeletalTree(t, l);
    bool b = compareVects(l, links);
    QVERIFY2(t==tree && b, "Result checking failed");
}

void AppTest::test_getSpanningTree_data()
{
    QTest::addColumn<Graph>("in_graph");
    QTest::addColumn<Graph>("tree");
    QTest::addColumn<QVector<int>>("links");

    Graph graph;
    QVector<int> links;
    Graph exp_graph;

    graph.insertEdge(QString("a"), 1, QString("b"));
    graph.insertEdge(QString("a"), 2, QString("b"));
    graph.insertEdge(QString("a"), 3, QString("b"));
    graph.insertEdge(QString("a"), 4, QString("b"));

    exp_graph.insertEdge(QString("a"), 1, QString("b"));

    links.append(2);
    links.append(3);
    links.append(4);

    QTest::newRow("Все ребра кратные") << graph << exp_graph << links;

    graph.clear();
    exp_graph.clear();
    links.clear();

    graph.insertEdge(QString("a"), 1, QString("b"));
    graph.insertEdge(QString("a"), 2, QString("c"));
    graph.insertEdge(QString("a"), 3, QString("d"));
    graph.insertEdge(QString("b"), 4, QString("c"));
    graph.insertEdge(QString("b"), 5, QString("d"));
    graph.insertEdge(QString("c"), 6, QString("d"));

    exp_graph.insertEdge(QString("a"), 1, QString("b"));
    exp_graph.insertEdge(QString("a"), 2, QString("c"));
    exp_graph.insertEdge(QString("a"), 3, QString("d"));

    links.append(4);
    links.append(5);
    links.append(6);

    QTest::newRow("Граф полный") << graph << exp_graph << links;

    graph.clear();
    exp_graph.clear();
    links.clear();

    graph.insertEdge(QString(), 1, QString());
    exp_graph.insertNode(QString());

    links.append(1);

    QTest::newRow("Граф состоит из одной петли") << graph << exp_graph << links;


    graph.clear();
    exp_graph.clear();
    links.clear();


    graph.insertEdge(QString(), 1, QString());
    graph.insertEdge(QString(), 2, QString());
    graph.insertEdge(QString(), 3, QString());

    exp_graph.insertNode(QString());

    links.append(1);
    links.append(2);
    links.append(3);

    QTest::newRow("Граф состоит из несольких петель") << graph << exp_graph << links;

    graph.clear();
    exp_graph.clear();
    links.clear();

    graph.insertEdge(QString("a"), 1, QString("b"));
    graph.insertEdge(QString("a"), 2, QString("c"));
    graph.insertEdge(QString("a"), 3, QString("a"));
    graph.insertEdge(QString("b"), 4, QString("c"));
    graph.insertEdge(QString("b"), 5, QString("d"));
    graph.insertEdge(QString("c"), 6, QString("d"));

    exp_graph.insertEdge(QString("a"), 1, QString("b"));
    exp_graph.insertEdge(QString("a"), 2, QString("c"));
    exp_graph.insertEdge(QString("b"), 5, QString("d"));

    links.append(3);
    links.append(4);
    links.append(6);

    QTest::newRow("Граф содержит петли") << graph << exp_graph << links;

    graph.clear();
    exp_graph.clear();
    links.clear();

    graph.insertEdge(QString("a"), 1, QString("b"));
    graph.insertEdge(QString("a"), 2, QString("g"));
    graph.insertEdge(QString("b"), 3, QString("c"));
    graph.insertEdge(QString("b"), 4, QString("d"));
    graph.insertEdge(QString("c"), 5, QString("d"));
    graph.insertEdge(QString("c"), 6, QString("g"));
    graph.insertEdge(QString("d"), 7, QString("g"));

    exp_graph.insertEdge(QString("a"), 1, QString("b"));
    exp_graph.insertEdge(QString("a"), 2, QString("g"));
    exp_graph.insertEdge(QString("b"), 3, QString("c"));
    exp_graph.insertEdge(QString("b"), 4, QString("d"));

    links.append(5);
    links.append(6);
    links.append(7);

    QTest::newRow("Граф простой") << graph << exp_graph << links;

    graph.clear();
    exp_graph.clear();
    links.clear();

    graph.insertEdge(QString("a"), 1, QString("b"));
    graph.insertEdge(QString("a"), 2, QString("g"));
    graph.insertEdge(QString("b"), 3, QString("c"));
    graph.insertEdge(QString("b"), 4, QString("d"));
    graph.insertEdge(QString("c"), 5, QString("d"));
    graph.insertEdge(QString("c"), 6, QString("g"));
    graph.insertEdge(QString("d"), 7, QString("g"));
    graph.insertEdge(QString("a"), 8, QString("b"));
    graph.insertEdge(QString("a"), 9, QString("g"));

    exp_graph.insertEdge(QString("a"), 1, QString("b"));
    exp_graph.insertEdge(QString("a"), 2, QString("g"));
    exp_graph.insertEdge(QString("b"), 3, QString("c"));
    exp_graph.insertEdge(QString("b"), 4, QString("d"));

    links.append(5);
    links.append(6);
    links.append(7);
    links.append(8);
    links.append(9);

    QTest::newRow("Граф содержит кратные ребра") << graph << exp_graph << links;

    graph.clear();
    exp_graph.clear();
    links.clear();

    graph.insertEdge(QString("a"), 1, QString("b"));
    graph.insertEdge(QString("a"), 2, QString("g"));
    graph.insertEdge(QString("b"), 3, QString("c"));
    graph.insertEdge(QString("b"), 4, QString("d"));
    graph.insertEdge(QString("c"), 5, QString("d"));
    graph.insertEdge(QString("c"), 6, QString("g"));
    graph.insertEdge(QString("d"), 7, QString("g"));
    graph.insertEdge(QString("a"), 8, QString("b"));
    graph.insertEdge(QString("a"), 9, QString("g"));
    graph.insertEdge(QString("c"), 10, QString("c"));

    exp_graph.insertEdge(QString("a"), 1, QString("b"));
    exp_graph.insertEdge(QString("a"), 2, QString("g"));
    exp_graph.insertEdge(QString("b"), 3, QString("c"));
    exp_graph.insertEdge(QString("b"), 4, QString("d"));

    links.append(5);
    links.append(6);
    links.append(7);
    links.append(8);
    links.append(9);
    links.append(10);

    QTest::newRow("Граф содержит кратные ребра и петли") << graph << exp_graph << links;

    graph.clear();
    exp_graph.clear();
    links.clear();

    graph.insertEdge(QString("a"), 1, QString("b"));
    graph.insertEdge(QString("a"), 2, QString("c"));
    graph.insertEdge(QString("a"), 3, QString("d"));
    graph.insertEdge(QString("b"), 4, QString("c"));
    graph.insertEdge(QString("b"), 5, QString("d"));
    graph.insertEdge(QString("c"), 6, QString("d"));
    graph.insertEdge(QString("a"), 7, QString("b"));
    graph.insertEdge(QString("c"), 8, QString("c"));

    exp_graph.insertEdge(QString("a"), 1, QString("b"));
    exp_graph.insertEdge(QString("a"), 2, QString("c"));
    exp_graph.insertEdge(QString("a"), 3, QString("d"));

    links.append(4);
    links.append(5);
    links.append(6);
    links.append(7);
    links.append(8);

    QTest::newRow("Полный граф с кратными ребрами и петлями") << graph << exp_graph << links;

    graph.clear();
    exp_graph.clear();
    links.clear();

    graph.insertEdge(QString("a"), 1, QString("b"));

    exp_graph.insertEdge(QString("a"), 1, QString("b"));

    QTest::newRow(" Граф состоит из одного ребра") << graph << exp_graph << links;

    graph.clear();
    exp_graph.clear();
    links.clear();

    graph.insertEdge(QString("a"), 1, QString("b"));
    graph.insertEdge(QString("a"), 2, QString("c"));
    graph.insertEdge(QString("a"), 3, QString("d"));
    graph.insertEdge(QString("b"), 4, QString("c"));
    graph.insertEdge(QString("d"), 5, QString("d"));
    graph.insertEdge(QString("b"), 6, QString("d"));
    graph.insertEdge(QString("c"), 7, QString("d"));
    graph.insertEdge(QString("a"), 8, QString("b"));
    graph.insertEdge(QString("c"), 9, QString("c"));

    exp_graph.insertEdge(QString("a"), 1, QString("b"));
    exp_graph.insertEdge(QString("a"), 2, QString("c"));
    exp_graph.insertEdge(QString("a"), 3, QString("d"));

    links.append(4);
    links.append(5);
    links.append(6);
    links.append(7);
    links.append(8);
    links.append(9);

    QTest::newRow("Комплексный тест") << graph << exp_graph << links;

    graph.clear();
    exp_graph.clear();
    links.clear();

}



void AppTest::test_findPath()
{
    QVector<int> p;
    QFETCH(Graph, graph);
    QFETCH(QString, begin);
    QFETCH(QString, goal);
    QFETCH(bool, isFind);
    QFETCH(QVector<QVector<int>>, path);

    bool retval = graph.findPath(begin, goal, p);
    bool res = false;
    for(int i = 0; i < path.count(); i++)
    {
        if(path[i] == p)
        {
            res = true;
            i=path.count();
        }
    }

    QVERIFY2((retval==isFind) && res, "Result checking failed");
}

void AppTest::test_findPath_data()
{
    QString begin, goal;
    Graph g;
    QVector<QVector<int>> path(4);
    bool isFind;

    QTest::addColumn<Graph>("graph");
    QTest::addColumn<QString>("begin");
    QTest::addColumn<QString>("goal");
    QTest::addColumn<bool>("isFind");
    QTest::addColumn<QVector<QVector<int>>>("path");


    begin = "k";
    goal="f";

    g.insertEdge("a", 1, "b");
    g.insertEdge("a", 2, "c");
    g.insertEdge("a", 3, "d");
    g.insertEdge("b", 4, "h");
    g.insertEdge("b", 5, "g");
    g.insertEdge("c", 6, "f");
    g.insertEdge("c", 7, "e");

    isFind = false;

    QTest::newRow("Начальный узел отсутствует в графе") << g << begin
        <<goal << isFind << path;

    g.clear();


    begin = "a";
    goal="k";

    g.insertEdge("a", 1, "b");
    g.insertEdge("a", 2, "c");
    g.insertEdge("a", 3, "d");
    g.insertEdge("b", 4, "h");
    g.insertEdge("b", 5, "g");
    g.insertEdge("c", 6, "f");
    g.insertEdge("c", 7, "e");

    isFind = false;

    QTest::newRow("Конечный узел отсутствует в графе") << g << begin
        <<goal << isFind << path;

    g.clear();


    begin = "k";
    goal="p";

    g.insertEdge("a", 1, "b");
    g.insertEdge("a", 2, "c");
    g.insertEdge("a", 3, "d");
    g.insertEdge("b", 4, "h");
    g.insertEdge("b", 5, "g");
    g.insertEdge("c", 6, "f");
    g.insertEdge("c", 7, "e");

    isFind = false;

    QTest::newRow("Начальный и конечный узел отсутствуют в графе") << g
        << begin <<goal << isFind << path;

    g.clear();


    begin = "a";
    goal="b";

    g.insertEdge("a", 1, "b");
    g.insertEdge("a", 2, "c");
    g.insertEdge("a", 3, "d");
    g.insertEdge("b", 4, "h");
    g.insertEdge("b", 5, "g");
    g.insertEdge("c", 6, "f");
    g.insertEdge("c", 7, "e");

    isFind = true;

    path[0].append(1);

    QTest::newRow("Узлы смежные") << g << begin <<goal << isFind << path;

    g.clear();
    path[0].clear();


    begin = "a";
    goal="f";

    g.insertEdge("a", 1, "b");
    g.insertEdge("a", 2, "c");
    g.insertEdge("a", 3, "d");
    g.insertEdge("b", 4, "h");
    g.insertEdge("b", 5, "g");
    g.insertEdge("c", 6, "f");
    g.insertEdge("c", 7, "e");

    isFind = true;

    path[0].append(2);
    path[0].append(6);

    QTest::newRow("Граф ацикличный") << g << begin <<goal << isFind << path;

    g.clear();
    path[0].clear();


    begin = "a";
    goal="f";

    g.insertEdge("a", 1, "b");
    g.insertEdge("a", 2, "c");
    g.insertEdge("a", 3, "d");
    g.insertEdge("b", 4, "h");
    g.insertEdge("b", 5, "f");
    g.insertEdge("c", 6, "f");
    g.insertEdge("c", 7, "e");

    isFind = true;

    path[0].append(1);
    path[0].append(5);
    path[1].append(2);
    path[1].append(6);

    QTest::newRow("Граф содержит циклы") << g << begin <<goal << isFind << path;

    g.clear();
    path[0].clear();
    path[1].clear();


    begin = "a";
    goal="f";

    g.insertEdge("a", 1, "b");
    g.insertEdge("a", 2, "c");
    g.insertEdge("a", 3, "d");
    g.insertEdge("b", 4, "h");
    g.insertEdge("b", 5, "f");
    g.insertEdge("c", 6, "f");
    g.insertEdge("c", 7, "e");
    g.insertEdge("f", 8, "f");

    isFind = true;

    path[0].append(1);
    path[0].append(5);
    path[1].append(2);
    path[1].append(6);

    QTest::newRow("Граф содержит петли") << g << begin <<goal << isFind << path;

    g.clear();
    path[0].clear();
    path[1].clear();


    begin = "a";
    goal="f";

    g.insertEdge("a", 1, "b");
    g.insertEdge("a", 2, "c");
    g.insertEdge("a", 3, "d");
    g.insertEdge("b", 4, "h");
    g.insertEdge("b", 5, "f");
    g.insertEdge("c", 6, "f");
    g.insertEdge("c", 7, "e");
    g.insertEdge("c", 8, "f");

    isFind = true;

    path[0].append(1);
    path[0].append(5);
    path[1].append(2);
    path[1].append(6);
    path[2].append(2);
    path[2].append(8);

    QTest::newRow("Граф содержит кратные ребра") << g << begin <<goal << isFind << path;

    g.clear();
    path[0].clear();
    path[1].clear();
    path[2].clear();


    begin = "a";
    goal="f";

    g.insertEdge("a", 1, "b");
    g.insertEdge("a", 2, "c");
    g.insertEdge("a", 3, "d");
    g.insertEdge("b", 4, "h");
    g.insertEdge("b", 5, "f");
    g.insertEdge("c", 6, "f");
    g.insertEdge("c", 7, "e");
    g.insertEdge("c", 8, "f");
    g.insertEdge("f", 9, "f");

    isFind = true;

    path[0].append(1);
    path[0].append(5);
    path[1].append(2);
    path[1].append(6);
    path[2].append(2);
    path[2].append(8);

    QTest::newRow("Граф содержит кратные ребра и петли") << g << begin <<goal << isFind << path;

    g.clear();
    path[0].clear();
    path[1].clear();
    path[2].clear();


    begin = "a";
    goal="d";

    g.insertEdge("a", 1, "b");
    g.insertEdge("a", 2, "c");
    g.insertEdge("a", 3, "d");
    g.insertEdge("b", 4, "c");
    g.insertEdge("b", 5, "d");
    g.insertEdge("c", 6, "d");

    isFind = true;

    path[0].append(3);
    path[1].append(1);
    path[1].append(5);
    path[2].append(1);
    path[2].append(4);
    path[2].append(6);
    path[3].append(2);
    path[3].append(6);

    QTest::newRow("Граф полный") << g << begin <<goal << isFind << path;

    g.clear();
    path[0].clear();
    path[1].clear();
    path[2].clear();
    path[3].clear();


    begin = "a";
    goal="f";

    g.insertEdge("a", 1, "b");
    g.insertEdge("a", 2, "c");
    g.insertEdge("a", 3, "d");
    g.insertEdge("b", 4, "h");
    g.insertEdge("b", 5, "f");
    g.insertEdge("c", 6, "f");
    g.insertEdge("c", 7, "e");
    g.insertEdge("c", 8, "k");
    g.insertEdge("k", 9, "k");
    g.insertEdge("c", 10, "f");
    g.insertEdge("f", 12, "f");

    isFind = true;

    path[0].append(1);
    path[0].append(5);
    path[1].append(2);
    path[1].append(6);
    path[2].append(2);
    path[2].append(8);

    QTest::newRow("Комплексный тест") << g << begin <<goal << isFind << path;
}







#include "tst_apptest.moc"

//Точка входа для тестов из main
int runTests(int argc, char *argv[]) {
    AppTest t;
    return QTest::qExec(&t, argc, argv);
}

#define TEST_MODE
#include "../alternating_current/main.cpp"
