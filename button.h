#ifndef NUMBER_H
#define NUMBER_H

#include <QString>

// game field button:

class Button
{
public:
    Button(int nValue = 0, bool bVisible = false);

    int value() const;
    void setValue(int nValue);

    bool visible() const;
    void setVisible(bool bVisible);

    QString color() const;
    void setColor(const QString& color);
private:
    int m_nValue;
    bool m_bVisible;
    QString m_strColor;
};

#endif // NUMBER_H
