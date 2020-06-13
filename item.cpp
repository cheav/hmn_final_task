#include "item.h"

Item::Item(int nValue, bool bVisible, const QString &color)
    : m_nValue(nValue), m_bVisible(bVisible), m_strColor(color)
{
    //
}

int Item::value() const
{
    return m_nValue;
}
void Item::setValue(int nValue)
{
    m_nValue = nValue;
}

bool Item::visible() const
{
    return m_bVisible;
}
void Item::setVisible(bool bVisible)
{
    m_bVisible = bVisible;
}

QString Item::color() const
{
    return m_strColor;
}
void Item::setColor(const QString &color)
{
    m_strColor = color;
}
