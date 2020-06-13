#ifndef ITEM_H
#define ITEM_H

#include <QString>

class Item
{
public:
    Item(int nValue = 0, bool bVisible = false, const QString& color = "white");

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

#endif // ITEM_H
