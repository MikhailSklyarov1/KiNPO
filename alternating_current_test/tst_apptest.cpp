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

    void test_findPath();
    void test_findPath_data();


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
