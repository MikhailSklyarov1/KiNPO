#include <QCoreApplication>

int sum(int a, int b)
{
    int c = a + b;
    return c;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);




    return a.exec();
}
