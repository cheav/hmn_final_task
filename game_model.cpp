#include "game_model.h"
#include <QDebug>
//--------------------------------------------------------------------------------
GameModel::GameModel(int nRows, int nColumns, int nLowRandomNumber, int nHighRandomNumber,
                     QObject *pParent)
    : QAbstractListModel(pParent), m_pGameModel(this), m_nRows(nRows), m_nColumns(nColumns),
      m_nLowRandomNumber(nLowRandomNumber), m_nHighRandomNumber(nHighRandomNumber),
      m_nVisibleButtonsCount(0)
{
    m_roles[ValueRole] = "buttonValue";
    m_roles[VisibleRole] = "buttonVisible";
    m_roles[ColorRole] = "buttonColor";
    m_roles[IndexRole] = "buttonIndex";

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
    QVector<Button>::iterator it = m_Numbers.begin();
    return it;
}
model_iterator GameModel::end()
{
    QVector<Button>::iterator it = m_Numbers.end();
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

int GameModel::visibleButtonsCount() const
{
    return m_nVisibleButtonsCount;
}
void GameModel::incrementVisibleButtonsCount()
{
    ++ m_nVisibleButtonsCount;
}
void GameModel::decrementVisibleButtonsCount()
{
    -- m_nVisibleButtonsCount;
}
void GameModel::resetVisibleButtonsCount()
{
    m_nVisibleButtonsCount = 0;
}

void GameModel::fillModel()
{
    beginResetModel();

    m_Numbers.clear();

    for(int i = 0; i < m_nRows; ++i)
        for(int j = 0; j < m_nColumns; ++j)
        {
            int nRandomNumber = m_nLowRandomNumber + rand() % m_nHighRandomNumber;
            m_Numbers.append(Button(nRandomNumber, false));
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
bool GameModel::setData(const QModelIndex &rcIndex, const QVariant &rcValue, int nRole)
{
    bool result = false;

    switch(nRole)
    {
    case ValueRole:
    {
        int nValue = rcValue.toInt();
        m_Numbers[rcIndex.row()].setValue(nValue);
        result = true;
        emit dataChanged(rcIndex, rcIndex, { ValueRole });
        break;
    }
    case VisibleRole:
    {
        bool bVisible = rcValue.toBool();
        m_Numbers[rcIndex.row()].setVisible(bVisible);
        result = true;
        emit dataChanged(rcIndex, rcIndex, { VisibleRole });
        break;
    }
    case ColorRole:
    {
        QString strColor = rcValue.toString();
        m_Numbers[rcIndex.row()].setColor(strColor);
        result = true;
        emit dataChanged(rcIndex, rcIndex, { ColorRole });
        break;
    }
    default: break;
    }

    return result;
}
Button& GameModel::getItem(const QModelIndex &rcIndex) const
{
    Button& rcItem = const_cast<Button&>(m_Numbers[rcIndex.row()]);
    return rcItem;
}
QHash<int, QByteArray> GameModel::roleNames() const
{
    return m_roles;
}

void GameModel::append(const Button &rcButton)
{
    int nRow = 0;
    while (nRow < m_Numbers.count())
        ++nRow;
    beginInsertRows(QModelIndex(), nRow, nRow);
    //m_Numbers.insert(nRow, number);
    m_Numbers.push_back(rcButton);
    endInsertRows();
}

void GameModel::set(int nRow, const Button& rcButton)
{
    if (nRow < 0 || nRow >= m_Numbers.count())
        return;
    m_Numbers.replace(nRow, rcButton);
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
    return createIndex(nRow, nColumn);
}


