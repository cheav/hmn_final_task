#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include <QAbstractListModel>

class Number
{
public:
    Number(int num);
    int number() const;
    void setNumber(int num);
private:
    int m_number;
};

class GameModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int rows READ rows WRITE setRows NOTIFY rowsChanged)
    Q_PROPERTY(int columns READ columns WRITE setColumns NOTIFY columnsChanged)
public:
    enum NumberRoles
    {
        NumberRole = Qt::UserRole + 1
    };

    GameModel(QObject *parent = nullptr);
    Q_INVOKABLE void fill();

    int rowCount(const QModelIndex & = QModelIndex()) const;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;

    Q_INVOKABLE void append(Number number);
    Q_INVOKABLE void set(int row, Number number);
    Q_INVOKABLE void remove(int row);

    Q_INVOKABLE int size() const;
    Q_INVOKABLE int rows() const;
    Q_INVOKABLE int columns() const;

    Q_INVOKABLE void setRows(int nRows);
    Q_INVOKABLE void setColumns(int nColumns);

    Q_INVOKABLE int randomNumber(int row, int column);
protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<Number> m_Numbers;
    int m_nRows;
    int m_nColumns;
    //
    int m_nLowRandomNumber;
    int m_nHighRandomNumber;
signals:
    void rowsChanged();
    void columnsChanged();
};

#endif // GAME_MODEL_H
