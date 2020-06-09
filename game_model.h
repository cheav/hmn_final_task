#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include <QAbstractListModel>
#include "number.h"

using model_iterator = QList<Number>::iterator;

class GameModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(GameModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(int rows READ rows WRITE setRows NOTIFY rowsChanged)
    Q_PROPERTY(int columns READ columns WRITE setColumns NOTIFY columnsChanged)
public:
    enum NumberRole
    {
        ValueRole = Qt::DisplayRole,
        VisibleRole = Qt::UserRole + 1,
        ColorRole = Qt::UserRole + 2,
        IndexRole = Qt::UserRole + 3,
    };
    Q_ENUM(NumberRole)

    GameModel(int nRows = 1, int nColumns = 1,
              int nLowRandomNumber = 1, int nHighRandomNumber = 9,
              QObject *pParent = nullptr);
public:
    GameModel* model() const;
    void setModel(GameModel* pModel);

    void beginResetModel();
    void endResetModel();

    model_iterator begin();
    model_iterator end();

    int lowRandomNumber() const;
    void setLowRandomNumber(int lowRandomNum);

    int highRandomNumber() const;
    void setHighRandomNumber(int highRandomNum);
public slots:
    void fillModel();
    void clearModel();
public:
    int rowCount(const QModelIndex & = QModelIndex()) const;
    Q_INVOKABLE QVariant data(const QModelIndex &rcIndex, int nRole = ValueRole) const;
    Q_INVOKABLE bool setData(const QModelIndex &rcIndex, const QVariant &rValue, int nRole = ValueRole);
    QModelIndex index(int nRow, int nColumn = 0, const QModelIndex &parent = QModelIndex()) const;

    Number& getItem(const QModelIndex &rcIndex) const;

    void append(Number number);
    void set(int nRow, Number number);
    void remove(int nRow);

    bool empty() const;
    int size() const;

    Q_INVOKABLE int rows() const;
    Q_INVOKABLE void setRows(int nRows);

    Q_INVOKABLE int columns() const;
    Q_INVOKABLE void setColumns(int nColumns);
protected:
    QHash<int, QByteArray> roleNames() const;
private:
    GameModel *m_pGameModel;
    QList<Number> m_Numbers;

    int m_nNewRandomValue;
    int m_nRows;
    int m_nColumns;
    int m_nLowRandomNumber;
    int m_nHighRandomNumber;
signals:
    void modelChanged();
    void rowsChanged();
    void columnsChanged();
};

#endif // GAME_MODEL_H
