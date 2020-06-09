#ifndef NUMBER_H
#define NUMBER_H

#include <QString>

class Number
{
public:
    Number(int nValue = 0, bool bVisible = false);
    int value() const;
    bool visible() const;
    QString color() const;

    void setValue(int nValue);
    void setVisible(bool bVisible);
    void setColor(QString& color);
private:
    int m_nValue;
    bool m_bVisible;
    QString m_strColor;
};

#endif // NUMBER_H
