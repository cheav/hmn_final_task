#include "number.h"

Number::Number(int nValue, bool bVisible)
    : m_nValue(nValue), m_bVisible(bVisible), m_strColor("white")
{
    //
}

int Number::value() const
{
    return m_nValue;
}
bool Number::visible() const
{
    return m_bVisible;
}
QString Number::color() const
{
    return m_strColor;
}

void Number::setValue(int nValue)
{
    m_nValue = nValue;
}
void Number::setVisible(bool bVisible)
{
    m_bVisible = bVisible;
}
void Number::setColor(const QString &color)
{
    m_strColor = color;
}
