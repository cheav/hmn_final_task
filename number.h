#ifndef NUMBER_H
#define NUMBER_H

#include <QString>

class Number
{
public:
    Number(int nNum = 0, bool bVisible = false);
    int number() const;
    bool visible() const;
    QString color() const;

    void setNumber(int nNum);
    void setVisible(bool bVisible);
    void setColor(QString& color);
private:
    int m_nNumber;
    bool m_bVisible;
    QString m_Color;
};

#endif // NUMBER_H
