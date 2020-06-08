#ifndef NUMBER_H
#define NUMBER_H

#include <QColor>

class Number
{
public:
    Number(int nNum, bool bVisible);
    int number() const;
    bool visible() const;
    QColor color() const;

    void setNumber(int nNum);
    void setVisible(bool bVisible);
    void setColor(QColor& color);
private:
    int m_nNumber;
    bool m_bVisible;
    QColor m_Color;
};

#endif // NUMBER_H
