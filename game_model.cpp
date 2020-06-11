#include "game_model.h"

//--------------------------------------------------------------------------------
GameModel::GameModel(int nRows, int nColumns, int nLowRandomNumber, int nHighRandomNumber,
                     QObject *pParent)
    : QAbstractListModel(pParent), m_pGameModel(this), m_nRows(nRows), m_nColumns(nColumns),
      m_nLowRandomNumber(nLowRandomNumber), m_nHighRandomNumber(nHighRandomNumber)
{
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

void GameModel::beginResetModel()
{
    QAbstractListModel::beginResetModel();
}
void GameModel::endResetModel()
{
    QAbstractListModel::endResetModel();
}

model_iterator GameModel::begin()
{
    QList<Number>::iterator it = m_Numbers.begin();
    return it;
}
model_iterator GameModel::end()
{
    QList<Number>::iterator it = m_Numbers.end();
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

void GameModel::fillModel()
{
    beginResetModel();

    m_Numbers.clear();

    for(int i = 0; i < m_nRows; ++i)
        for(int j = 0; j < m_nColumns; ++j)
        {
            int nRandomNumber = m_nLowRandomNumber + rand() % m_nHighRandomNumber;
            m_Numbers.append(Number(nRandomNumber, false));
        }

    endResetModel();
}
void GameModel::clearModel()
{
    beginResetModel();
    m_Numbers.clear();
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
    return m_Numbers.count();
}
bool GameModel::empty() const
{
    return m_Numbers.empty();
}
int GameModel::size() const
{
    return m_Numbers.size();
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
        case ValueRole: return m_Numbers.at(rcIndex.row()).value();
        case VisibleRole: return m_Numbers.at(rcIndex.row()).visible();
        case ColorRole: return m_Numbers.at(rcIndex.row()).color();
        case IndexRole: return rcIndex.row();
        default: return QVariant();
        }
    return QVariant();
}
bool GameModel::setData(const QModelIndex &rcIndex, const QVariant &rValue, int nRole)
{
    bool result = false;

    switch(nRole)
    {
    case ValueRole:
    {
        int nValue = rValue.toInt();
        Number number(nValue, false);
        set(rcIndex.row(), number);
        result = true;
    }
    case ColorRole:
    {
        QString strColor = rValue.toString();
        m_Numbers[rcIndex.row()].setColor(strColor);
        result = true;
    }
    default:{}
    }

    if (result) emit dataChanged(rcIndex, rcIndex);
    return result;
}
Number& GameModel::getItem(const QModelIndex &rcIndex) const
{
    Number& rcItem = const_cast<Number&>(m_Numbers[rcIndex.row()]);
    return rcItem;
}
QHash<int, QByteArray> GameModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ValueRole] = "numberValue";
    roles[VisibleRole] = "numberVisible";
    roles[ColorRole] = "numberColor";
    roles[IndexRole] = "numberIndex";
    return roles;
}

void GameModel::append(Number number)
{
    int nRow = 0;
    while (nRow < m_Numbers.count())
        ++nRow;
    beginInsertRows(QModelIndex(), nRow, nRow);
    m_Numbers.insert(nRow, number);
    endInsertRows();
}

void GameModel::set(int nRow, Number number)
{
    if (nRow < 0 || nRow >= m_Numbers.count())
        return;

    m_Numbers.replace(nRow, number);
    dataChanged(index(nRow, 0), index(nRow, 0), { ValueRole, VisibleRole, ColorRole, IndexRole });
}

void GameModel::remove(int nRow)
{
    if (nRow < 0 || nRow >= m_Numbers.count())
        return;

    beginRemoveRows(QModelIndex(), nRow, nRow);
    m_Numbers.removeAt(nRow);
    endRemoveRows();
}

QModelIndex GameModel::index(int nRow, int nColumn, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    QModelIndex index = createIndex(nRow, nColumn);
    return index;
}


