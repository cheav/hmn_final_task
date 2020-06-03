#include "game_model.h"
#include <cstdlib>
#include <ctime>

Number::Number(int num) : m_number(num) { }
int Number::number() const
{
    return m_number;
}
void Number::setNumber(int num)
{
    m_number = num;
}
//--------------------------------------------------------------------------------
GameModel::GameModel(QObject *parent) : QAbstractListModel(parent), m_nRows(0), m_nColumns(0),
    m_nLowRandomNumber(1), m_nHighRandomNumber(9)
{
    fill();
}
void GameModel::fill()
{
    //srand(time(0));

    for(int i = 1; i < m_nRows + 1; ++i)
        for(int j = 1; j < m_nColumns + 1; ++j)
        {
            int nRandomNumber = m_nLowRandomNumber + rand() % m_nHighRandomNumber;
            m_Numbers.append(Number(nRandomNumber));
        }
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
    m_Numbers.clear();
    fill();
}
void GameModel::setColumns(int nColumns)
{
    m_nColumns = nColumns;
    emit columnsChanged();
    //
    m_Numbers.clear();
    fill();
}


QVariant GameModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < rowCount())
        switch (role)
        {
        case NumberRole: return m_Numbers.at(index.row()).number();
        //case Qt::UserRole: return m_numbers.at(index.row()).number();
        default: return QVariant();
        }
    return QVariant();
}

QHash<int, QByteArray> GameModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NumberRole] = "number";
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
    dataChanged(index(row, 0), index(row, 0), { NumberRole });
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
