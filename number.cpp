#include "number.h"

Number::Number(int nNum, bool bVisible)
    : m_nNumber(nNum), m_bVisible(bVisible), m_Color(Qt::white)
{
    //
}

int Number::number() const
{
    return m_nNumber;
}
bool Number::visible() const
{
    return m_bVisible;
}
QColor Number::color() const
{
    return m_Color;
}

void Number::setNumber(int nNum)
{
    m_nNumber = nNum;
}
void Number::setVisible(bool bVisible)
{
    m_bVisible = bVisible;
}
void Number::setColor(QColor& color)
{
    m_Color = color;
}
