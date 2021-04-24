#include <QCoreApplication>
#include <functions.h>

int main(int argc, char *argv[])
{

#ifdef TEST_MODE
    runTests(argc, argv);
#else
    //Основные действия программы
    int res = sum(1, 2);
#endif

    return 0;
}
