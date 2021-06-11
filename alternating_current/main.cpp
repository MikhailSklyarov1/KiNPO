#define _SCL_SECURE_NO_WARNINGS
#include <QCoreApplication>
#include <QtCore/QCoreApplication>
#include "declaration.h"
#include <string.h>
#include <QString>
#include <QTextStream>

typedef QMapIterator<QString, Link> GraphIterator;



int main(int argc, char *argv[])
{
#ifdef TEST_MODE
    runTests(argc, argv);
#else


    //Получить данные из файла

    //..Подготовить пути файлов
    QCoreApplication a(argc, argv);

    QStringList params;
    params << " " << "-o" <<"..\\Win32\\Release\\testlog.txt";

    QMap<int, Branch> branches;
    Graph graph;

    char * path = argv[argc-1];;
    char * outpath = new char[strlen(path) * 2];

    try {
        //Рассмотреть ситуации в зависимости от количества аргументов...

        if(argc < 2) //Если аргументов меньше, чем 2
        {
            //Ввести еще один аргумент
            cout << "Enter path ";
            cin >> path;

            //Записать расположение файла
            strcpy(outpath, path);
            strcpy(strrchr(outpath, '.')+1, "dot");
        }

        else if(argc == 2)   //Иначе если 2 аргумента
        {
            //Записать расположение файла
            strcpy(outpath, path);
            strcpy(strrchr(outpath, '.')+1, "dot");
        }

        else if(argc > 2)  //Если аргументов больше, чем 2
        {
            delete [] outpath;
            throw QString("Слишком много аргументов");
        }

        //..Распарсить файл
        parseFileXML(path, branches, graph);

        //Найти независимые контуры в цепи
        QVector<QMap<int, int>> contours;
        findIndependentContours(graph, branches, contours);

        //Найти токи в цепи
        QVector<ComplexVal> currents;
        cyclicCurrentsCalculation(contours, branches, currents); //Рассчитать контурные токи
        currentCalculation(contours, currents, branches);   //Рассчитать силу тока в каждой ветви

        //Запись результата в файл
        writeResult(outpath, branches);

        //Сообщение об успешной записи
        cout << "Result recorded" << endl;

    } catch(QString str) {
        setlocale(LC_ALL, "Russian");
        cout << qPrintable(str) << endl;
        cout << "Press any key to exit";
        getch();
        a.exit(1);
    }
    a.exit(0);
    #endif
    return 0;
}
