#include "button.h"

Button::Button(int nValue, bool bVisible)
    : m_nValue(nValue), m_bVisible(bVisible), m_strColor("white")
{
    //
}

int Button::value() const
{
    return m_nValue;
}
void Button::setValue(int nValue)
{
    m_nValue = nValue;
}

bool Button::visible() const
{
    return m_bVisible;
}
void Button::setVisible(bool bVisible)
{
    m_bVisible = bVisible;
}

QString Button::color() const
{
    return m_strColor;
}
void Button::setColor(const QString &color)
{
    m_strColor = color;
}
