#include "branch.h"

Branch::Branch(QObject *parent)
    : QObject(parent)
{

}

Branch::Branch(const QVector<Element> & elements, const QString & begin=QString(), const QString & end=QString(), double volt=0)
{
    resistance = calcResistanceBranch(elements);
    this->begin = begin;
    this->end = end;
    this->voltage = volt;
}

Branch::Branch(const Branch & other)
{
    this->begin=other.begin;
    this->end = other.end;
    this->current = other.current;
    this->resistance = other.resistance;
    this->voltage = other.voltage;
}

Branch::Branch()
{

}

Branch::~Branch()
{

}

ComplexVal Branch::calcResistanceBranch(const QVector<Element>& elements)
{
    double activ = 0;
    double reactiv = 0;

    //Для каждого элемента вектора
    for(int i = 0; i < elements.size(); i++)
    {
        //Если резистор
        if(elements[i].type == RESISTOR)
        {
            activ+=elements[i].value;
        }

        //Иначе если индуктивный элемент
        else if(elements[i].type == INDUCTOR)
        {
            reactiv+= 2 * M_PI * FREQUENCY * elements[i].value;
        }

        //Иначе если конденсатор
        else if(elements[i].type == CAPACITOR)
        {
            reactiv-= 1/(2 * M_PI * FREQUENCY * elements[i].value);
        }
    }
    return ComplexVal(activ, reactiv);
}

const ComplexVal & Branch::getComplexResistance()
{
    return resistance;
}

Branch & Branch::setCurrent(double Amper)
{
    current = Amper;
    return *this;
}

double Branch::getVoltage()
{
    return voltage;
}

double Branch::getCurrent() const
{
    return current;
}

const QString & Branch::getStart() const
{
    return begin;
}

const QString & Branch::getEnd() const
{
    return end;
}

Branch & Branch::setNodes(const QString & begin, const QString & end)
{
    this->begin = begin;
    this->end = end;
    return *this;
}

Branch & Branch::setResistance(const ComplexVal & Om)
{
    resistance = Om;
    return *this;
}

Branch & Branch::setVoltage(double Volt)
{
    voltage = Volt;
    return *this;
}

Branch & Branch::operator=(const Branch & other)
{
    this->begin=other.begin;
    this->end = other.end;
    this->current = other.current;
    this->resistance = other.resistance;
    this->voltage = other.voltage;
    return *this;
}


bool Branch::operator==(const Branch & other) const
{
    if(this->begin != other.begin)
        return false;
    else if(this->end != other.end)
        return false;
    else if(this->current != other.current)
        return false;
    else if(this->resistance != other.resistance)
        return false;
    else if(this->voltage != other.voltage)
        return false;
    return true;
}
