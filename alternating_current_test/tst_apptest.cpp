#include <QtTest>
#include <functions.h>

// add necessary includes here

class AppTest : public QObject
{
    Q_OBJECT

public:
    AppTest();
    ~AppTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();
    void test_case2();

};

AppTest::AppTest()
{

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

void AppTest::test_case1()
{
    QVERIFY(sum(1, 3) == 4);
}

void AppTest::test_case2()
{
    QVERIFY(sum(2, 3) == 5);
}

#include "tst_apptest.moc"

//Точка входа для тестов из main
int runTests(int argc, char *argv[]) {
    AppTest t;
    return QTest::qExec(&t, argc, argv);
}

#define TEST_MODE
#include "../alternating_current/main.cpp"
