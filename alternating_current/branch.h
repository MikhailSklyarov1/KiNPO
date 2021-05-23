#ifndef BRANCH_H
#define BRANCH_H
#define _USE_MATH_DEFINES

#include <QObject>
#include <QVector>
#include <complex>
#include <math.h>
#include "enum.h"
#include "element.h"

#define FREQUENCY 50   //Частота переменного тока

using namespace std;
typedef complex<double> ComplexVal;  //Тип данных для комплексных значений


class Branch : public QObject
{
    Q_OBJECT
    friend class AppTest;

public:



private:
    QString begin;
    QString end;
    ComplexVal resistance;
    double current;
    double voltage;


};



#endif // BRANCH_H
