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


void parseFileXML(char * filename, QMap<int, Branch> & branches, Graph & graph)
{
    if(QString(filename).right(4) != QString(".xml"))
        throw QString("Неверный формат файла");
    TiXmlDocument doc(filename);
    if(!doc.LoadFile())
        throw QString("Неверный формат файла");
    Branch b;
    int number = 0;
    int count = 0;
    int id_num = 1;
    TiXmlElement* chain = doc.FirstChildElement( "chain" );
    if ( chain )
    {
        TiXmlElement* branch = chain->FirstChildElement( "branch" );
        try
        {
            //Для каждой ветви...
            while(branch)
            {
                //Сформировать ветвь и произвести их подсчет
                count += makingBranch(branch, b, &number, &id_num);
                id_num++;  //Инкрементировать id
                branches.insert(number, b);
                graph.insertEdge(b.getStart(), number, b.getEnd());
                branch = branch->NextSiblingElement( "branch" );
            }

            if(count < 2 || count > 40)
            {
                throw QString("Некорректное количество элементов. Оно должно быть в диапазоне от 2 до 40 элементов.");
            }
        }

        catch (QString str)
        {
                throw str;
        }

        //Проверка на односвязность графа
        if(!graph.isSimpleConnected())
            throw QString("В цепи присутствует разрыв, поэтому ток не может пройти через все ветви.");

        QVector<QString> list;
        graph.findLeafNode(list);

        if(!list.isEmpty())
        {
            throw QString ("Из каждого узла должны выходить хотя бы две ветви.");
        }
    }
}
