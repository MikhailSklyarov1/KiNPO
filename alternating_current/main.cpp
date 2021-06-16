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
    if(element.value < 0.000001 || element.value > 1000000)
    {
        QString mes =  QString(namerus) + "(" + QString::number(element.value, 'f') + " " + QString(nomrus) + ")";
        throw mes;
    }
}


void findIndependentContours(Graph & graph, QMap<int, Branch> & branches, QVector<QMap<int, int>> & contours)
{
    Graph tree;
    int edgeNum;
    Branch current;

    //Выделить остовное дерево графа
    QVector<int> links;
    graph.getSkeletalTree(tree, links);

    //Для каждой хорды (s, v) графа
    QVectorIterator<int> link(links);
    while(link.hasNext())
    {
        edgeNum = link.next();
        current = branches.value(edgeNum);
        QString v = current.getStart();
        int b;
        QVector<int> path;
        QMap<int, int> contour;

        //Найти путь от узла s до узла v
        if(tree.findPath(current.getEnd(), v, path))
        {
            //Добавить ветвь (s, v) в начало найденного пути
            path.prepend(edgeNum);

            //Добавить найденный путь в список контуров

            //..Для каждой ветви, входящей в список пути
            for(int i = 0; i < path.count(); i++)
            {
                b = path[i];
                //Если направление тока совпадает с направлением контурного тока
                current = branches.value(path[i]);	//Берем ветвь из списка вевтей с номером хорды
                if(v == current.getStart())
                {
                    contour.insert(path[i], 1); //Присвоить единицу
                    v = current.getEnd();
                }
                else
                {
                    contour.insert(path[i], -1); //Присвоить минус один
                    v = current.getStart();
                }
            }
            contours.append(contour);
        }
    }
}


void cyclicCurrentsCalculation(QVector<QMap<int, int>> & contours, QMap<int, Branch> & branches, QVector<ComplexVal> & currents)
{
    QVector<QVector<ComplexVal>> multnum;
    QVector<ComplexVal> constnum;

    //Составить уравнения по второму законам Кирхгофа
    compilationEquations(multnum, constnum, branches, contours);

    //Решить систему уравнений
    solveEquations(multnum, constnum, currents);
}


void compilationEquations(QVector<QVector<ComplexVal>> & resistance, QVector<ComplexVal> & voltage,QMap<int, Branch> & branches, QVector<QMap<int, int>> & contours)
{
    Branch tmp;
    resistance.resize(contours.count());
    for(int i = 0; i < contours.count(); i++)
        resistance[i].resize(contours.count());

    voltage.resize(contours.count());

    //Для каждого элемента матрицы коэффициентов и матрицы свободных членов
    for(int i = 0; i < contours.count(); i++)
    {
        for(int j = i; j < contours.count(); j++)
        {
            resistance[i][j] = ComplexVal(0, 0);

            //Заполнить i-ю строку матрицы коэффициентов.
            if(i == j)	//Элемент матрицы находится на главной диагонали...
            {
                //..Элемент равен сумме сопротивлений в ветвях, через которые протекает i-й контурный
                QMapIterator<int, int> contour(contours[i]);
                while(contour.hasNext())
                {
                    tmp = branches.value(contour.next().key());
                    resistance[i][j]+=tmp.getComplexResistance();
                }
            }


            bool hasIncidentBranches = false;
            QMapIterator<int, int> k(contours[i]);
            while(k.hasNext())
                if(contours[j].contains(k.next().key()))
                {
                    hasIncidentBranches = true;
                }

            else if(!hasIncidentBranches) //Иначе если контур i не имеет смежных ветвей с контуром j
            {
                resistance[i][j] = resistance[j][i] = 0;
            }
            else
            {
                QVector<int> br;


               QMapIterator<int, int> q(contours[i]);
                while(q.hasNext())
                {
                    if(contours[j].contains(q.next().key()))
                    {
                        br.append(q.key());
                    }
                }

                for(int k = 0; k < br.count(); k++)
                {
                    tmp = branches.value(br[k]);
                    resistance[i][j] = resistance[j][i] += tmp.getComplexResistance() *	ComplexVal(contours[i].key(br[k]), 0) * ComplexVal(contours[j].key(br[k]), 0);
                }
            }
        }

        //Заполнить i-ый элемент матрицы свободных членов
        QMapIterator<int, int> contour(contours[i]);
        while(contour.hasNext())
        {
            tmp = branches.value(contour.next().key());
            voltage[i]+=tmp.getVoltage()* ComplexVal(contour.value(), 0);
        }
    }
}


void solveEquations(QVector<QVector<ComplexVal>> & multnum, QVector<ComplexVal> & constnum, QVector<ComplexVal> & variables)
{
    QVector<QVector<ComplexVal>> tmp;

    //Рассчитать определитель матрицы коэффициентов
    ComplexVal det = determinantOfMatrix(multnum);
    if(det == ComplexVal(0, 0))
        return;
    int count = multnum.count();

    //Для каждого i-го столбца матрицы коэффициентов
    for(int i = 0; i < count; i++)
    {
        tmp = multnum;

        //Заменить i-й столбец матрицы коэффициентов матрицей-столбцом
        for(int j = 0; j < count; j++)
            tmp[j][i] = constnum[j];

        //Добавить в конец вектора с неизвестными переменными отношение определителей
        variables.append(determinantOfMatrix(tmp)/det);
    }
}



ComplexVal additionMatrix(const QVector<QVector<ComplexVal>> & matrix, int row, int column)
{
    QVector<QVector<ComplexVal>> add(matrix.count()-1);
    for(int i = 0; i < matrix.count()-1; i++)
        add[i].resize(matrix.count()-1);

    for(int i = 0; i <  matrix.count(); i++)
        for(int j = 0; j <  matrix.count(); j++)
        {
            if(i < row && j < column)
            {
                add[i][j] = matrix[i][j];
            }
            else if(i > row && j < column)
            {
                add[i-1][j] = matrix[i][j];
            }
            else if(i < row && j > column)
            {
                add[i][j-1] = matrix[i][j];
            }
            else if(i > row && j > column)
            {
                add[i-1][j-1] = matrix[i][j];
            }
        }
    return determinantOfMatrix(add);
}

