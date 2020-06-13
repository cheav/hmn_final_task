#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include <QAbstractListModel>
#include <QVector>
#include "item.h"

using model_iterator = QVector<Item>::iterator;

class GameModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(GameModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(int rows READ rows WRITE setRows NOTIFY rowsChanged)
    Q_PROPERTY(int columns READ columns WRITE setColumns NOTIFY columnsChanged)
public:
    enum ItemRole
    {
        ValueRole = Qt::DisplayRole,
        VisibleRole = Qt::UserRole,
        ColorRole,
        IndexRole
    };
    Q_ENUM(ItemRole)

    GameModel(int nRows = 6, int nColumns = 10,
              int nLowRandomNumber = 1, int nHighRandomNumber = 9,
              QObject *pParent = nullptr);
public:
    GameModel* model() const;
    void setModel(GameModel* pModel);

    model_iterator begin();
    model_iterator end();

    int lowRandomNumber() const;
    void setLowRandomNumber(int lowRandomNum);

    int highRandomNumber() const;
    void setHighRandomNumber(int highRandomNum);

    int visibleItemsCount() const;
    void incrementVisibleItemsCount();
    void decrementVisibleItemsCount();
    void resetVisibleItemsCount();
public slots:
    void fillModel();
    void clearModel();
    void onRowsChanged();
    void onColumnsChanged();
public:
    int rowCount(const QModelIndex & = QModelIndex()) const;
    QVariant data(const QModelIndex &rcIndex, int nRole = ValueRole) const;
    bool setData(const QModelIndex &rcIndex, const QVariant &rcValue, int nRole = ValueRole);
    QModelIndex index(int nRow, int nColumn = 0, const QModelIndex &parent = QModelIndex()) const;

    Item& getItem(const QModelIndex &rcIndex) const;

    void append(const Item& rcItem);
    void set(int nRow, const Item &rcItem);
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
    QHash<int, QByteArray> m_roles;
    GameModel *m_pGameModel;
    QVector<Item> m_Items;

    int m_nRows;
    int m_nColumns;
    int m_nLowRandomNumber;
    int m_nHighRandomNumber;
    int m_nVisibleItemsCount;
signals:
    void modelChanged();
    void rowsChanged();
    void columnsChanged();
};

#endif // GAME_MODEL_H
