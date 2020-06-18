#include "game_model.h"
#include <QDebug>
//--------------------------------------------------------------------------------
GameModel::GameModel(int nRows, int nColumns, int nLowRandomNumber, int nHighRandomNumber,
                     QObject *pParent)
    : QAbstractListModel(pParent), m_pGameModel(this), m_nRows(nRows), m_nColumns(nColumns),
      m_nLowRandomNumber(nLowRandomNumber), m_nHighRandomNumber(nHighRandomNumber),
      m_nVisibleItemsCount(0)
{
    m_roles[ValueRole] = "itemValue";
    m_roles[VisibleRole] = "itemVisible";
    m_roles[ColorRole] = "itemColor";
    m_roles[IndexRole] = "itemIndex";

    srand(time(0));
    fillModel();
    connect(this, SIGNAL(rowsChanged()), this, SLOT(fillModel()));
    connect(this, SIGNAL(columnsChanged()), this, SLOT(fillModel()));
}
//-------------------------------------------------------------------------------------------------
GameModel* GameModel::model() const
{
    return m_pGameModel;
}
void GameModel::setModel(GameModel* pModel)
{
    m_pGameModel = pModel;
}

model_iterator GameModel::begin()
{
    QVector<Item>::iterator it = m_Items.begin();
    return it;
}
model_iterator GameModel::end()
{
    QVector<Item>::iterator it = m_Items.end();
    return it;
}

int GameModel::lowRandomNumber() const
{
    return m_nLowRandomNumber;
}
void GameModel::setLowRandomNumber(int lowRandomNum)
{
    m_nLowRandomNumber = lowRandomNum;
}

int GameModel::highRandomNumber() const
{
    return m_nHighRandomNumber;
}
void GameModel::setHighRandomNumber(int highRandomNum)
{
    m_nHighRandomNumber = highRandomNum;
}

int GameModel::visibleItemsCount() const
{
    return m_nVisibleItemsCount;
}
void GameModel::incrementVisibleItemsCount()
{
    ++ m_nVisibleItemsCount;
}
void GameModel::decrementVisibleItemsCount()
{
    -- m_nVisibleItemsCount;
}
void GameModel::resetVisibleItemsCount()
{
    m_nVisibleItemsCount = 0;
}

void GameModel::fillModel()
{
    beginResetModel();

    m_Items.clear();

    for(int i = 0; i < m_nRows; ++i)
        for(int j = 0; j < m_nColumns; ++j)
        {
            int nRandomNumber = m_nLowRandomNumber + rand() % m_nHighRandomNumber;
            m_Items.append(Item(nRandomNumber, false));
        }

    endResetModel();
}
void GameModel::clearModel()
{
    beginResetModel();
    m_Items.clear();
    endResetModel();
}
void GameModel::onRowsChanged()
{
    fillModel();
}
void GameModel::onColumnsChanged()
{
    fillModel();
}
//-------------------------------------------------------------------------------------------------
int GameModel::rowCount(const QModelIndex &) const
{
    return m_Items.count();
}
bool GameModel::empty() const
{
    return m_Items.empty();
}
int GameModel::count() const
{
    return m_Items.size();
}

int GameModel::rows() const
{
    return m_nRows;
}
void GameModel::setRows(int nRows)
{
    m_nRows = nRows;
    emit rowsChanged();
}

int GameModel::columns() const
{
    return m_nColumns;
}
void GameModel::setColumns(int nColumns)
{
    m_nColumns = nColumns;
    emit columnsChanged();
}

QVariant GameModel::data(const QModelIndex &rcIndex, int nRole) const
{
    if (rcIndex.row() < rowCount())
        switch (nRole)
        {
        case ValueRole: return m_Items.at(rcIndex.row()).value();
        case VisibleRole: return m_Items.at(rcIndex.row()).visible();
        case ColorRole: return m_Items.at(rcIndex.row()).color();
        case IndexRole: return rcIndex.row();
        default: return QVariant();
        }
    return QVariant();
}
bool GameModel::setData(const QModelIndex &rcIndex, const QVariant &rcValue, int nRole)
{
    bool result = false;

    switch(nRole)
    {
    case ValueRole:
    {
        int nValue = rcValue.toInt();
        m_Items[rcIndex.row()].setValue(nValue);
        result = true;
        emit dataChanged(rcIndex, rcIndex, { ValueRole });
        break;
    }
    case VisibleRole:
    {
        bool bVisible = rcValue.toBool();
        m_Items[rcIndex.row()].setVisible(bVisible);
        result = true;
        emit dataChanged(rcIndex, rcIndex, { VisibleRole });
        break;
    }
    case ColorRole:
    {
        QString strColor = rcValue.toString();
        m_Items[rcIndex.row()].setColor(strColor);
        result = true;
        emit dataChanged(rcIndex, rcIndex, { ColorRole });
        break;
    }
    default: break;
    }

    return result;
}
bool GameModel::setItemData(const QModelIndex &index, const QHash<int, QVariant> &roles)
{
    //
}

QHash<int, QByteArray> GameModel::roleNames() const
{
    return m_roles;
}

void GameModel::append(const Item &rcItem)
{
    int nRow = 0;
    while (nRow < m_Items.count())
        ++nRow;
    beginInsertRows(QModelIndex(), nRow, nRow);
    //m_Numbers.insert(nRow, number);
    m_Items.push_back(rcItem);
    endInsertRows();
}

void GameModel::set(int nRow, const Item& rcItem)
{
    if (nRow < 0 || nRow >= m_Items.count())
        return;
    m_Items.replace(nRow, rcItem);
}

void GameModel::remove(int nRow)
{
    if (nRow < 0 || nRow >= m_Items.count())
        return;

    beginRemoveRows(QModelIndex(), nRow, nRow);
    m_Items.removeAt(nRow);
    endRemoveRows();
}

QModelIndex GameModel::index(int nRow, int nColumn, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return createIndex(nRow, nColumn);
}


