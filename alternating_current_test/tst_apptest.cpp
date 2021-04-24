#include <QtTest>

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

}

QTEST_APPLESS_MAIN(AppTest)

#include "tst_apptest.moc"
