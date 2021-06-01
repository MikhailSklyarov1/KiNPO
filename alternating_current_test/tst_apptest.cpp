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


void AppTest::test_calculateResistance()
{
    Branch branch;
    QFETCH(QVector<Element>, param);
    QFETCH(ComplexVal, expect);
    ComplexVal res = branch.calcResistanceBranch(param);
    QVERIFY2((expect.real()-0.1 < res.real() || expect.imag()-0.1 < res.imag()) && (res.real() < expect.real()+0.1 || res.imag() < expect.imag()+0.1), "Result checking failed");
}

void AppTest::test_calculateResistance_data()
{
    setlocale(LC_ALL, "Russian");

    ComplexVal results[7] = {
        ComplexVal((double)50, 0),
        ComplexVal(0, (double)40.82),
        ComplexVal(0, (double)-62.63),
        ComplexVal(0, (double)107.06),
        ComplexVal(65, (double)160.14),
        ComplexVal(65, (double)-0.32),
        ComplexVal(25, (double)107.06)
    };

    QVector<Element> test1;
    QVector<Element> test2;
    QVector<Element> test3;
    QVector<Element> test4;
    QVector<Element> test5;
    QVector<Element> test6;
    QVector<Element> test7;

    test1.append(Element(RESISTOR, (double)15.0));
    test1.append(Element(RESISTOR, (double)20.0));
    test1.append(Element(RESISTOR, (double)5.0));
    test1.append(Element(RESISTOR, (double)10.0));

    test2.append(Element(INDUCTOR, (double)0.015));
    test2.append(Element(INDUCTOR, (double)0.1));
    test2.append(Element(INDUCTOR, (double)0.01));
    test2.append(Element(INDUCTOR, (double)0.005));

    test3.append(Element(CAPACITOR, (double)0.00015));
    test3.append(Element(CAPACITOR, (double)0.0001));
    test3.append(Element(CAPACITOR, (double)0.001));
    test3.append(Element(CAPACITOR, (double)0.0005));

    test4.append(Element(CAPACITOR, (double)0.00015));
    test4.append(Element(CAPACITOR, (double)0.0001));
    test4.append(Element(INDUCTOR, (double)0.01));
    test4.append(Element(INDUCTOR, (double)0.5));

    test5.append(Element(RESISTOR, (double)15.0));
    test5.append(Element(RESISTOR, (double)50.0));
    test5.append(Element(INDUCTOR, (double)0.01));
    test5.append(Element(INDUCTOR, (double)0.5));

    test6.append(Element(RESISTOR, (double)15.0));
    test6.append(Element(RESISTOR, (double)50.0));
    test6.append(Element(CAPACITOR, (double)0.01));
    test6.append(Element(CAPACITOR, (double)0.5));

    test7.append(Element(RESISTOR, (double)15.0));
    test7.append(Element(RESISTOR, (double)10.0));
    test7.append(Element(CAPACITOR, (double)0.00015));
    test7.append(Element(CAPACITOR, (double)0.0001));
    test7.append(Element(INDUCTOR, (double)0.01));
    test7.append(Element(INDUCTOR, (double)0.5));

    QTest::addColumn<QVector<Element>>("param");
    QTest::addColumn<ComplexVal>("expect");

    QTest::newRow("Только резисторы") << test1 << results[0];
    QTest::newRow("Только катушки") << test2 << results[1];
    QTest::newRow("Только конденсаторы") << test3 <<results[2];
    QTest::newRow("Катушки и конденсаторы") << test4 << results[3];
    QTest::newRow("Резисторы и катушки") << test5 << results[4];
    QTest::newRow("Резисторы и конденсаторы") << test6 << results[5];
    QTest::newRow("Резисторы, катушки, конденсаторы") << test7 <<results[6];
}

void AppTest::doData()
{
    //ДАННЫЕ ДЛЯ ФУНКЦИЙ test_cyclicCurrentsCalculation
    //test_calculaterRealCurrents


    b_list1[0].insert(1, b.setResistance(ComplexVal(20.0, 4.0)).setVoltage(30.0).setNodes("", ""));
    b_list2[0].insert(1, b.setCurrent(1.04));
    contour.insert(1,1);
    c_list[0].append(contour);

    ccurr_list[0].append(ComplexVal(1.44, -0.29));

    contour.clear();


    b_list1[1].insert(1, b.setResistance(ComplexVal(10.0, 10.0)).setVoltage(0.0).setNodes("a", "b"));
    b_list2[1].insert(1, b.setCurrent(1.49977));
    b_list1[1].insert(2, b.setResistance(ComplexVal(15.0, -5.0)).setVoltage(0).setNodes("b", "c"));
    b_list2[1].insert(2, b.setCurrent(0.35778));
    b_list1[1].insert(3, b.setResistance(ComplexVal(5.0, 0.5)).setVoltage(0).setNodes("b", "c"));
    b_list2[1].insert(3, b.setCurrent(1.13));
    b_list1[1].insert(4, b.setResistance(ComplexVal(10.0, -5.0)).setVoltage(0).setNodes("c", "a"));
    b_list2[1].insert(4, b.setCurrent(1.46));
    b_list1[1].insert(5, b.setResistance(ComplexVal(0, 0)).setVoltage(30.0).setNodes("a", "b"));
    b_list2[1].insert(5, b.setCurrent(2.51));

    contour.insert(1, 1);
    contour.insert(5, -1);
    c_list[1].append(contour);
    contour.clear();

    contour.insert(4, -1);
    contour.insert(2, -1);
    contour.insert(5, -1);
    c_list[1].append(contour);
    contour.clear();

    contour.insert(3, 1);
    contour.insert(2, -1);
    c_list[1].append(contour);
    contour.clear();

    ccurr_list[1].append(ComplexVal(-1.5, 1.5));
    ccurr_list[1].append(ComplexVal(-1.95, -0.68));
    ccurr_list[1].append(ComplexVal(1.55, 0.37));


    b_list1[2].insert(1, b.setResistance(ComplexVal(20.0, 0)).setVoltage(0.0).setNodes("a", "b"));
    b_list2[2].insert(1, b.setCurrent(1.66145));
    b_list1[2].insert(2, b.setResistance(ComplexVal(25.0, 0)).setVoltage(0).setNodes("a", "b"));
    b_list2[2].insert(2, b.setCurrent(0.50904));
    b_list1[2].insert(3, b.setResistance(ComplexVal(20.0, 0)).setVoltage(0).setNodes("a", "c"));
    b_list2[2].insert(3, b.setCurrent(1.0605));
    b_list1[2].insert(4, b.setResistance(ComplexVal(15.0, 0)).setVoltage(0).setNodes("b", "c"));
    b_list2[2].insert(4, b.setCurrent(0.5656));
    b_list1[2].insert(5, b.setResistance(ComplexVal(5.0, 0)).setVoltage(0).setNodes("b", "c"));
    b_list2[2].insert(5, b.setCurrent(0.58681));
    b_list1[2].insert(6, b.setResistance(ComplexVal(0, 0)).setVoltage(30.0).setNodes("c", "a"));
    b_list2[2].insert(6, b.setCurrent(2.21291));

    contour.insert(1, 1);
    contour.insert(2, -1);
    c_list[2].append(contour);
    contour.clear();

    contour.insert(1, 1);
    contour.insert(4, 1);
    contour.insert(6, 1);
    c_list[2].append(contour);
    contour.clear();


    contour.insert(1, 1);
    contour.insert(5, 1);
    contour.insert(6, 1);
    c_list[2].append(contour);
    contour.clear();

    contour.insert(3, 1);
    contour.insert(6, 1);
    c_list[2].append(contour);
    contour.clear();

    ccurr_list[2].append(ComplexVal(0.72, 0));
    ccurr_list[2].append(ComplexVal(0.8, 0));
    ccurr_list[2].append(ComplexVal(0.83, 0));
    ccurr_list[2].append(ComplexVal(1.5, 0));


    b_list1[3].insert(1, b.setResistance(ComplexVal(30.0, 0)).setVoltage(0.0).setNodes("a", "c"));
    b_list2[3].insert(1, b.setCurrent(0.48076));
    b_list1[3].insert(2, b.setResistance(ComplexVal(20.0, 0)).setVoltage(0).setNodes("a", "c"));
    b_list2[3].insert(2, b.setCurrent(0.71407));
    b_list1[3].insert(3, b.setResistance(ComplexVal(10.0, 0)).setVoltage(0).setNodes("b", "a"));
    b_list2[3].insert(3, b.setCurrent(1.19483));
    b_list1[3].insert(4, b.setResistance(ComplexVal(10.0, 0)).setVoltage(30).setNodes("c", "b"));
    b_list2[3].insert(4, b.setCurrent(2.50278));
    b_list1[3].insert(5, b.setResistance(ComplexVal(20, 0)).setVoltage(0.0).setNodes("b", "c"));
    b_list2[3].insert(5, b.setCurrent(1.30795));

    contour.insert(1, -1);
    contour.insert(2, 1);
    c_list[3].append(contour);
    contour.clear();

    contour.insert(1, -1);
    contour.insert(3, -1);
    contour.insert(4, -1);
    c_list[3].append(contour);
    contour.clear();

    contour.insert(1, -1);
    contour.insert(3, -1);
    contour.insert(5, 1);
    c_list[3].append(contour);
    contour.clear();

    ccurr_list[3].append(ComplexVal(1.01, 0));
    ccurr_list[3].append(ComplexVal(-3.54, 0));
    ccurr_list[3].append(ComplexVal(1.85, 0));


    b_list1[4].insert(1, b.setResistance(ComplexVal(10.0, 10.0)).setVoltage(0.0).setNodes("a", "b"));
    b_list2[4].insert(1, b.setCurrent(1.49977));
    b_list1[4].insert(2, b.setResistance(ComplexVal(15.0, -5.0)).setVoltage(0).setNodes("b", "c"));
    b_list2[4].insert(2, b.setCurrent(0.36));
    b_list1[4].insert(3, b.setResistance(ComplexVal(5.0, 0.5)).setVoltage(0).setNodes("b", "c"));
    b_list2[4].insert(3, b.setCurrent(1.13));
    b_list1[4].insert(4, b.setResistance(ComplexVal(10.0, -5.0)).setVoltage(0).setNodes("c", "a"));
    b_list2[4].insert(4, b.setCurrent(1.46));
    b_list1[4].insert(5, b.setResistance(ComplexVal(0, 0)).setVoltage(30.0).setNodes("a", "b"));
    b_list2[4].insert(5, b.setCurrent(2.51));

    contour.insert(1, 1);
    contour.insert(5, -1);
    c_list[4].append(contour);
    contour.clear();

    contour.insert(4, 1);
    contour.insert(2, 1);
    contour.insert(5, 1);
    c_list[4].append(contour);
    contour.clear();

    contour.insert(3, 1);
    contour.insert(2, -1);
    c_list[4].append(contour);
    contour.clear();

    ccurr_list[4].append(ComplexVal(-1.5, 1.5));
    ccurr_list[4].append(ComplexVal(1.95, 0.68));
    ccurr_list[4].append(ComplexVal(1.55, 0.37));


    b_list1[5].insert(1, b.setResistance(ComplexVal(10.0, 10.0)).setVoltage(0.0).setNodes("a", "b"));
    b_list2[5].insert(1, b.setCurrent(1.49977));
    b_list1[5].insert(2, b.setResistance(ComplexVal(15.0, -5.0)).setVoltage(0).setNodes("b", "c"));
    b_list2[5].insert(2, b.setCurrent(0.36));
    b_list1[5].insert(3, b.setResistance(ComplexVal(5.0, 0.5)).setVoltage(0).setNodes("b", "c"));
    b_list2[5].insert(3, b.setCurrent(1.13));
    b_list1[5].insert(4, b.setResistance(ComplexVal(10.0, -5.0)).setVoltage(0).setNodes("c", "a"));
    b_list2[5].insert(4, b.setCurrent(1.46));
    b_list1[5].insert(5, b.setResistance(ComplexVal(0, 0)).setVoltage(30.0).setNodes("a", "b"));
    b_list2[5].insert(5, b.setCurrent(2.51));

    contour.insert(1, -1);
    contour.insert(5, 1);
    c_list[5].append(contour);
    contour.clear();

    contour.insert(4, -1);
    contour.insert(2, -1);
    contour.insert(5, -1);
    c_list[5].append(contour);
    contour.clear();

    contour.insert(3, -1);
    contour.insert(2, 1);
    c_list[5].append(contour);
    contour.clear();

    ccurr_list[5].append(ComplexVal(1.5, -1.5));
    ccurr_list[5].append(ComplexVal(-1.95, -0.68));
    ccurr_list[5].append(ComplexVal(-1.55, -0.37));


    b_list1[6].insert(1, b.setResistance(ComplexVal(10.0, 10)).setVoltage(0.0).setNodes("a", "c"));
    b_list2[6].insert(1, b.setCurrent(0.35));
    b_list1[6].insert(2, b.setResistance(ComplexVal(15.0, -5)).setVoltage(0).setNodes("a", "c"));
    b_list2[6].insert(2, b.setCurrent(0.9));
    b_list1[6].insert(3, b.setResistance(ComplexVal(5.0, 0.5)).setVoltage(0).setNodes("b", "a"));
    b_list2[6].insert(3, b.setCurrent(0.55));
    b_list1[6].insert(4, b.setResistance(ComplexVal(10.0, -5)).setVoltage(30).setNodes("c", "b"));
    b_list2[6].insert(4, b.setCurrent(1.29));
    b_list1[6].insert(5, b.setResistance(ComplexVal(10, 0)).setVoltage(0.0).setNodes("b", "c"));
    b_list2[6].insert(5, b.setCurrent(0.75));


    contour.insert(1, -1);
    contour.insert(2, 1);
    c_list[6].append(contour);
    contour.clear();

    contour.insert(2, 1);
    contour.insert(3, 1);
    contour.insert(4, 1);
    c_list[6].append(contour);
    contour.clear();

    contour.insert(4, 1);
    contour.insert(5, 1);
    c_list[6].append(contour);
    contour.clear();

    ccurr_list[6].append(ComplexVal(-0.46, 0.16));
    ccurr_list[6].append(ComplexVal(-0.77, 0.14));
    ccurr_list[6].append(ComplexVal(-0.99, 0.37));


    b_list1[7].insert(1, b.setResistance(ComplexVal(10.0, 10.0)).setVoltage(0.0).setNodes("a", "c"));
    b_list1[7].insert(2, b.setResistance(ComplexVal(10.0, 10.0)).setVoltage(0).setNodes("a", "c"));
    b_list1[7].insert(3, b.setResistance(ComplexVal(10.0, 10.0)).setVoltage(0).setNodes("b", "a"));
    b_list1[7].insert(4, b.setResistance(ComplexVal(10.0, 10.0)).setVoltage(30).setNodes("c", "b"));
    b_list1[7].insert(5, b.setResistance(ComplexVal(10.0, 10.0)).setVoltage(0.0).setNodes("b", "c"));


    contour.insert(1, -1);
    contour.insert(2, 1);
    c_list[7].append(contour);
    contour.clear();

    contour.insert(2, 1);
    contour.insert(3, 1);
    contour.insert(4, 1);
    c_list[7].append(contour);
    contour.clear();

    contour.insert(4, 1);
    contour.insert(5, 1);
    c_list[7].append(contour);
    contour.clear();

    ccurr_list[7].append(ComplexVal(0.19, -0.19));
    ccurr_list[7].append(ComplexVal(0.38, -0.38));
    ccurr_list[7].append(ComplexVal(0.56, -0.56));

}


void AppTest::test_cyclicCurrentsCalculation()
{
    QVector<ComplexVal> c;
    QFETCH(contourList, contours);
    QFETCH(branchList, branches);
    QFETCH(QVector<ComplexVal>, currents);

    cyclicCurrentsCalculation(contours, branches, c);
    bool b = true;
    for(int i = 0; i < c.count() && c.count() == currents.count(); i++)
    {

        //допустимая погрешность в ручных расчетах - 0.5
        if(!((c[i].real()-0.5 < currents[i].real() || c[i].imag()-0.5 < currents[i].imag())
            && (currents[i].real() < c[i].real()+0.5 || currents[i].imag() < c[i].imag()+0.5)))
            b = false;
    }
    QVERIFY2(compareVects(c, currents) || b, "Result checking failed");
}

void AppTest::test_cyclicCurrentsCalculation_data()
{
    QTest::addColumn<contourList>("contours");
    QTest::addColumn<branchList>("branches");
    QTest::addColumn<QVector<ComplexVal>>("currents");

    QTest::newRow("В цепи один контур") << c_list[0] << b_list1[0] << ccurr_list[0];
    QTest::newRow("Через каждую ветвь протекает не более двух контурных токов") << c_list[1] << b_list1[1] << ccurr_list[1];
    QTest::newRow("Цепь содержит ветви, через которые протекает более двух контурных токов")<< c_list[2] << b_list1[2] << ccurr_list[2];
    QTest::newRow("Цепь содержит ветви, через которые протекают все контурные токи") << c_list[3] << b_list1[3] << ccurr_list[3];
    QTest::newRow("Все контурные токи движутся по часовой стрелке") << c_list[4] << b_list1[4] << ccurr_list[4];
    QTest::newRow("Все контура движутся против часовой стрелки") << c_list[5] << b_list1[5] << ccurr_list[5];
    QTest::newRow("Все смежные контура однонаправленны") << c_list[6] << b_list1[6] << ccurr_list[6];
    QTest::newRow("Величина сопротивления на всех ветвях одинаковая") << c_list[7] << b_list1[7] << ccurr_list[7];

}




#include "tst_apptest.moc"

//Точка входа для тестов из main
int runTests(int argc, char *argv[]) {
    AppTest t;
    return QTest::qExec(&t, argc, argv);
}

#define TEST_MODE
#include "../alternating_current/main.cpp"
