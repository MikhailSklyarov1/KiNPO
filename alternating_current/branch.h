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
    Branch(QObject *parent);
    Branch();
    Branch(const Branch &);
    Branch(const QVector<Element> & elements, const QString & begin, const QString & end, double volt);
    ~Branch();

    /*! Возвращает значение комплексного сопротивления на ветви
        \return - комплексное сопротивление
    */
    const ComplexVal & getComplexResistance();
    /*! Устанавливает величину тока на ветви
        \param[in] величина тока
        \return - ссылка на ветвь
    */
    Branch & setCurrent(double Amper);
    /*!	Получить величину ЭДС на ветви
        \return - значение ЭДС
    */
    double getVoltage();
    /*! Получить величину тока на ветви
        \return - значение тока
    */
    double getCurrent() const;
    /*! Получить имя начального узла ветви
        \return - константная ссылка на имя начального узла ветви
    */
    const QString & getStart() const;
    /*! Получить имя конечного узла ветви
        \return - константная ссылка на имя конечного узла ветви
    */
    const QString & getEnd() const;
    /*! Задать имена узлов ветви
        \param[in] имя начального узла
        \param[in] имя конечного узла
        \return - ссылка на ветвь текущую
    */
    Branch & setNodes(const QString & begin, const QString & end);
    /*! Задать комплексное сопротивление на ветви
        \param[in] значение сопротивления в ом
        \return - ссылка на ветвь текущую
    */
    Branch & setResistance(const ComplexVal & Om);
    /*! Задать величину ЭДС на ветви
        \param[in] значение ЭДС в вольт
        \return - ссылка на ветвь текущую
    */
    Branch & setVoltage(double Volt);
    /*! Перегрузка оператора присваивания
        \param[in] other -
        \return
    */
    Branch & operator=(const Branch & other);
    /*! Перегрузка оператора присваивания
        \param[in] other -
        \return
    */
    bool operator==(const Branch & other) const;


private:
    QString begin;
    QString end;
    ComplexVal resistance;
    double current;
    double voltage;

    /*! Рассчитывает комплексное сопротивление на ветви
        \param[in] elements - список элементов, принадлежащих ветви
        \return - компексное значение сопротивления
    */
    ComplexVal calcResistanceBranch(const QVector<Element>& elements);

};



#endif // BRANCH_H
