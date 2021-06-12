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



int makingBranch(TiXmlElement * xmlnode, Branch & branch, int * number,int * id_num)
{
    int count = 0;
    QStringList wrongElements;
    QString mes;
    if ( xmlnode )
    {
        const char * begin = xmlnode->Attribute("begin");
        const char * end = xmlnode->Attribute("end");

        //Если начало ветви не указано
        if(begin == NULL)
        {
            //Считать узел по умолчанию a
            begin = "a";
        }

        //Если конец ветви не указан
        if(end == NULL)
        {
            //Считать узел по умолчанию a
            end = "a";
        }

        QString allow("qwertyuiopasdfghjklzxcvbnm0123456789"); //Допустимые элементы в названии узла

        bool beginNodeCorrect =allow.contains(begin) || allow.isEmpty(); //Проверить узел, из которого ветвь исходит
        bool endNodeCorrect =allow.contains(end) || allow.isEmpty(); //Проверить узел, в который ветвь заходит

        //Если узлы некорректно названы
        if(!(beginNodeCorrect && endNodeCorrect))
        {
            throw QString("В названии узлов должны присутствовать только латинские буквы нижнего регистра и цифры.");
        }

        //Считать с файла id
        char  *_id = (char *)xmlnode->Attribute("id");

        //Если id элемена не указано
        if(_id == NULL)
        {
            //Считать индекс первого лемента эдиницей
            _id = "1";
        }

        //Проиндексировать последующие элементы
        char buff[20];
        _id = itoa(*id_num, buff, 10);


        double volt = 0;
        Element e;
        QVector<Element> elements;

        //Считать резисторы
        TiXmlElement* resistor = xmlnode->FirstChildElement( "resistor" );
        while ( resistor )
        {
            try {
                makingElement(resistor, e, RESISTOR, "резистор", "om", "Ом");
            } catch (QString mes) {
                wrongElements << mes;
            }
            elements.append(e);
            resistor = resistor->NextSiblingElement( "resistor" );
            count++;
        }

        //Считать катушки индуктивности
        TiXmlElement* inductor = xmlnode->FirstChildElement( "inductor" );
        while ( inductor )
        {
            try {
                string str0 = "катушка индуктивности";
                makingElement(inductor, e, INDUCTOR, "катушка индуктивности", "henry", "Генри");
            } catch (QString mes) {
                wrongElements << mes;
            }
            elements.append(e);
            inductor = inductor->NextSiblingElement( "inductor" );
            count++;
        }

        //Считать конденсаторы
        TiXmlElement* capacitor = xmlnode->FirstChildElement( "capacitor" );
        while ( capacitor )
        {
            try {
                makingElement(capacitor, e, CAPACITOR, "конденсатор", "farad", "Фарад");
            } catch (QString mes) {
                wrongElements << mes;
            }
            elements.append(e);
            capacitor = capacitor->NextSiblingElement( "capacitor" );
            count++;
        }

        //Считать источники питания
        TiXmlElement* source = xmlnode->FirstChildElement( "source" );
        while ( source )
        {
            try {
                makingElement(source, e, SOURCE, "источник ЭДС", "volt", "Вольт");
            } catch (QString mes) {
                wrongElements << mes;
            }
            volt = e.value;
            elements.append(e);
            source = source->NextSiblingElement( "source" );
            count++;
        }
        branch = Branch(elements, begin, end, volt);	//узлы
        *number = atoi(_id);
    }

    if(!wrongElements.isEmpty())
    {
        QString exception("Числовые значения для элементов: ");
        QStringListIterator i(wrongElements);
        while(i.hasNext())
            exception += i.next() + ", ";
        exception += QString(" некорректны. Должны быть в диапазоне [10^-6, 10^6].");
        throw exception;
    }
    return count;
}



void makingElement(const TiXmlElement * givenel, Element & element,  TypeElement type,
    char * namerus, char * nomeng, char * nomrus)
{
    QTextCodec * c = QTextCodec::codecForLocale();
    element.type = type;
    const char * value = givenel->Attribute(nomeng);
    element.value = QString(c->toUnicode(value)).toDouble();
    if(element.value < 0.0000001 || element.value > 10000000)
    {
        QString mes =  QString(namerus) + "(" + QString::number(element.value, 'f') + " " + QString(nomrus) + ")";
        throw mes;
    }
}
