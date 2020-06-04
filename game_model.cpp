#include "game_model.h"
#include <cstdlib>
#include <ctime>
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
    //fill();
}
void GameModel::fill()
{
    srand(time(0));

    int nRandomRow_visible = 1 + rand() % m_nRows;
    int nRandomColumn_visible = 1 + rand() % m_nColumns;
    bool bVisibleNumber = false;
    int nVisibleNumber = 0;

    beginResetModel();

    m_Numbers.clear();

    for(int i = 1; i < m_nRows + 1; ++i)
        for(int j = 1; j < m_nColumns + 1; ++j)
        {
            int nRandomNumber = m_nLowRandomNumber + rand() % m_nHighRandomNumber;
            //
            bVisibleNumber = false;
            if(nRandomRow_visible == i && nRandomColumn_visible == j)
            {
                bVisibleNumber = true;
                nVisibleNumber = Number(nRandomNumber, bVisibleNumber).number();
            }
            //
            m_Numbers.append(Number(nRandomNumber, bVisibleNumber));
        }

    qDebug() << nRandomRow_visible << ' ' << nRandomColumn_visible << ": " << nVisibleNumber;

    endResetModel();
}

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
    fill();
}
void GameModel::setColumns(int nColumns)
{
    m_nColumns = nColumns;
    emit columnsChanged();
    //
    fill();
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
