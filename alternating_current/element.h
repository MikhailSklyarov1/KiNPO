#ifndef ELEMENT_H
#define ELEMENT_H
#include "enum.h"


class Element
{
public:
    int type;
    double value;

    Element()
    {

    }

    Element(TypeElement e, double valueElement)
    {
        this->type = e;
        this->value = valueElement;
    }

    Element(const Element & other)
    {
        type = other.type;
        value = other.value;
    }

    ~Element()
    {

    }

    Element operator=(const Element & other)
    {
        type = other.type;
        value = other.value;
        return *this;
    }

    bool operator==(const Element & other) const
    {
        return true;
    }
};

#endif // ELEMENT_H
