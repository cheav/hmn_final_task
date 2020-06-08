#include "game_model.h"

//--------------------------------------------------------------------------------
GameModel::GameModel(int nRows, int nColumns, int nLowRandomNumber, int nHighRandomNumber,
                     QObject *pParent)
    : QAbstractListModel(pParent), m_pGameModel(this), m_nRows(nRows), m_nColumns(nColumns),
      m_nLowRandomNumber(nLowRandomNumber), m_nHighRandomNumber(nHighRandomNumber)
{
}
//-------------------------------------------------------------------------------------------------
GameModel* GameModel::gameModel() const
{
    return m_pGameModel;
}
void GameModel::setGameModel(GameModel* pGameModel)
{
    m_pGameModel = pGameModel;
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

int GameModel::newRandomValue() const
{
    return m_nNewRandomValue;
}
void GameModel::setNewRandomValue(int nNewRandomValue)
{
    m_nNewRandomValue = nNewRandomValue;
}

void GameModel::fillModel()
{
    srand(time(0));

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
    return m_Numbers.count();
}
int GameModel::rows() const
{
    return m_nRows;
}
int GameModel::columns() const
{
    return m_nColumns;
}

void GameModel::setRows(int nRows)
{
    m_nRows = nRows;
    emit rowsChanged();

    fillModel();
}
void GameModel::setColumns(int nColumns)
{
    m_nColumns = nColumns;
    emit columnsChanged();

    fillModel();
}


QVariant GameModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < rowCount())
        switch (role)
        {
        case ValueRole: return m_Numbers.at(index.row()).number();
        case VisibleRole: return m_Numbers.at(index.row()).visible();
        case ColorRole: return m_Numbers.at(index.row()).color();
        case IndexRole: return index.row();
        default: return QVariant();
        }
    return QVariant();
}
bool GameModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool result = false;

    switch(role)
    {
    case ValueRole:
    {
        int nValue = newRandomValue(); // before must set in game logic
        Number& rNumber = getItem(index);
        rNumber.setNumber(nValue);
        rNumber.setVisible(false);
        result = true;
    }
    case ColorRole:
    {
        QString strValue = value.toString();
        QColor color(strValue);
        Number& rNumber = getItem(index);
        rNumber.setColor(color);
        result = true;
    }
    default:{}
    }

    if (result) emit dataChanged(index, index);
    return result;
}
Number& GameModel::getItem(const QModelIndex &index) const
{
    Number& rcItem = const_cast<Number&>(m_Numbers[index.row()]);
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
    int row = 0;
    while (row < m_Numbers.count())
        ++row;
    beginInsertRows(QModelIndex(), row, row);
    m_Numbers.insert(row, number);
    endInsertRows();
}

void GameModel::set(int row, Number number)
{
    if (row < 0 || row >= m_Numbers.count())
        return;

    m_Numbers.replace(row, number);
    dataChanged(index(row, 0), index(row, 0), { ValueRole, VisibleRole, ColorRole, IndexRole });
}

void GameModel::remove(int row)
{
    if (row < 0 || row >= m_Numbers.count())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_Numbers.removeAt(row);
    endRemoveRows();
}

QModelIndex GameModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    QModelIndex index = createIndex(row, column);
    return index;
}


