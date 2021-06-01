#ifndef DECLARATION_H
#define DECLARATION_H

#include "branch.h"
#include "graph.h"
#include "../../tinyxml/tinyxml.h"
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTextCodec>
#include <iostream>
#include <conio.h>


/*! Функция формирует данные об элементе.
    \param[in] givenel - заданное в файле название
    \param[in, out] element - данные об элементе
    \param[in] type - тип элемента
    \param[in] namerus - название элемента
    \param[in] nomeng - размерность на английском
    \param[in] nomrus - размерность на русском
*/
void makingElement(const TiXmlElement * givenel, Element & element,  TypeElement type, char * namerus, char * nomeng, char * nomrus);

/*! Функция собирает данные и создает структуру ветви.
    \param[in] filename - имя файла с данными о цепи
    \param[in, out] branch - анализируемая ветвь
    \param[in, out] graph - граф, отображающий цепь
    \return - количество элементов цепи
*/
int makingBranch(TiXmlElement * xmlnode, Branch & branch, int * number, int * id_num);

/*! Функция парсит XML-файл с данными о цепи, создает ее структуру.
    \param[in] filename - имя файла с данными о цепи
    \param[in, out] branches - выделенные ветви цепи
    \param[in, out] graph - граф, отображающий цепь
*/
void parseFileXML(char * filename, QMap<int, Branch> & branches, Graph & graph);

/*! Функция рассчитывает силу тока в ветвях.
    \param[in] contours - список контуров
    \param[in] currents - значения контурных токов
    \param[in, out] branches - список ветвей цепи
*/
void currentCalculation(QVector<QMap<int, int>> & contours, QVector<complex<double>> & currents, QMap<int, Branch> & branches);

/*! Функция генерирует уравнения для нахождения силы тока.
    \param[out] resistance - матрица сопротивлений
    \param[out] voltage - напряжения
    \param[in] branches - список ветвей цепи
    \param[in] contours - список контуров
*/
void compilationEquations(QVector<QVector<ComplexVal>> & resistance, QVector<ComplexVal> & voltage, QMap<int, Branch> & branches, QVector<QMap<int, int>> & contours);

/*! Функция ищет независимые контуры.
    \param[in] graph - матрица инцидентности графа
    \param[in] branches - список ветвей цепи
    \param[out] contours - список контуров
*/
void findIndependentContours(Graph & graph, QMap<int, Branch> & branches, QVector<QMap<int, int>> & contours);

/*! Функция решает систему уравнений.
    \param[in] multnum - матрица коэффициентов
    \param[in] constnum - свободные члены уравнения
    \param[out] variables - искомые значения
*/
void solveEquations (QVector<QVector<ComplexVal>> & multnum, QVector<ComplexVal> & constnum, QVector<complex<double>> & variables);

/*! Функция рассчитывает контурные токи.
    \param[in] contours - список контуров
    \param[in] branches - список ветвей цепи
    \param[out] currents - искомые значений контурных токов
*/
void cyclicCurrentsCalculation (QVector<QMap<int, int>> & contours, QMap<int, Branch> & branches, QVector<complex <double>> & currents);

/*! Функция считает определитель матрицы.
    \param[in] matrix - матрица значений
    \return - определитель матрицы значений
*/
ComplexVal determinantOfMatrix(const QVector<QVector<ComplexVal>> & matrix);

/*! Функция считает дополнение элемента матрицы.
    \param[in] matrix - анализируемая матрица
    \param[in] row - строка элемента
    \param[in] column - столбец элемента
    \return - дополнение элемента
*/
ComplexVal additionMatrix(const QVector<QVector<ComplexVal>> & matrix, int row, int column);

/*! Функция записываеты результат в файл.
    \param[in] direct - путь для записи
    \param[in] branches - список ветвей
*/
void writeResult(char * direct, const QMap<int, Branch> & branches);


#endif // DECLARATION_H
