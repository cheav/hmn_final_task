#include "game_model.h"
#include <cstdlib>
#include <ctime>

#include <QTimer>
#include <QDebug>

Number::Number(int nNum, bool bVisible)
    : m_nNumber(nNum), m_bVisible(bVisible) {}
int Number::number() const
{
    return m_nNumber;
}
void Number::setNumber(int nNum)
{
    m_nNumber = nNum;
}
bool Number::visible() const
{
    return m_bVisible;
}
void Number::setVisible(bool bVisible)
{
    m_bVisible = bVisible;
}
//--------------------------------------------------------------------------------
GameModel::GameModel(int nRows, int nColumns, int nLowRandomNumber, int nHighRandomNumber,
                     QObject *parent)
    : QAbstractListModel(parent), m_nRows(nRows), m_nColumns(nColumns),
      m_nLowRandomNumber(nLowRandomNumber), m_nHighRandomNumber(nHighRandomNumber)
{
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(editModel()));
    m_pTimer->setInterval(700);
}
//-------------------------------------------------------------------------------------------------
// Game Logic
void GameModel::fillModel()
{
    srand(time(0));

    beginResetModel();

    m_Numbers.clear();

    for(int i = 1; i < m_nRows + 1; ++i)
        for(int j = 1; j < m_nColumns + 1; ++j)
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
void GameModel::editModel()
{
    if(GameOverCondition() == true)
    {
        qDebug() << "Game Over";
        GameStop();
        return;
    }

    beginResetModel();

    srand(time(0));

    int nRandomNumber = m_nLowRandomNumber + rand() % m_nHighRandomNumber;
    int nRandomIndex = rand() % (size() - 1);
    int nIndex = 0;
    bool bFound = false;
    //
    auto it = m_Numbers.begin();
    for(; it != m_Numbers.end(); ++it)
    {
        if((*it).visible() == false && nRandomIndex == nIndex)
        {
            *it = Number(nRandomNumber, true);
            bFound = true;
            break;
        }
        else if((*it).visible() == true && nRandomIndex == nIndex)
        {
            // find to right of random index
            auto it_new = it + 1;
            for(; it_new != m_Numbers.end(); ++it_new)
                if((*it_new).visible() == false)
                {
                    *it_new = Number(nRandomNumber, true);
                    bFound = true;
                    break;
                }
            break;
        }
        ++nIndex;
    }
    // find to left of random index
    if(bFound == false)
    {
        it = m_Numbers.begin();
        for(; it != m_Numbers.end(); ++it)
            if((*it).visible() == false)
            {
                *it = Number(nRandomNumber, true);
                break;
            }
    }

    qDebug() << nRandomIndex;

    endResetModel();
}
bool GameModel::GameOverCondition()
{
    bool bGameFieldFull = false;

    int nCount = 0;
    auto it = m_Numbers.begin();
    for(; it != m_Numbers.end(); ++it)
        if((*it).visible() == true)
            ++nCount;

    int nGameOverCondition = 2 * size();
    nGameOverCondition /= 3;

    if(nCount == nGameOverCondition)
        bGameFieldFull = true;

    return bGameFieldFull;
}
void GameModel::GameStop()
{
    m_pTimer->stop();
}

void GameModel::startGame()
{
    fillModel();
    m_pTimer->start();
}
void GameModel::stopGame()
{
    clearModel();
    m_pTimer->stop();
}
void GameModel::pauseGame()
{
    m_pTimer->stop();
}
//-------------------------------------------------------------------------------------------------
int GameModel::rowCount(const QModelIndex &) const
{
    return m_Numbers.count();
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
    //
    fillModel();
}
void GameModel::setColumns(int nColumns)
{
    m_nColumns = nColumns;
    emit columnsChanged();
    //
    fillModel();
}


QVariant GameModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < rowCount())
        switch (role)
        {
        case DisplayRole: return m_Numbers.at(index.row()).number();
        case VisibleRole: return m_Numbers.at(index.row()).visible();
        default: return QVariant();
        }
    return QVariant();
}

QHash<int, QByteArray> GameModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DisplayRole] = "number";
    roles[VisibleRole] = "visibleNumber";
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
    dataChanged(index(row, 0), index(row, 0), { DisplayRole, VisibleRole });
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
int GameModel::randomNumber(int row, int column)
{
    QModelIndex idx = index(row, column);
    return (data(idx)).toInt();
}


