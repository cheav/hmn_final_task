#include "game_model.h"
#include <cstdlib>
#include <ctime>

#include <QTimer>
#include <QDebug>

Number::Number(int nNum, bool bVisible)
    : m_nNumber(nNum), m_bVisible(bVisible), m_Color(Qt::white) {}
int Number::number() const
{
    return m_nNumber;
}
bool Number::visible() const
{
    return m_bVisible;
}
QColor Number::color() const
{
    return m_Color;
}
void Number::setNumber(int nNum)
{
    m_nNumber = nNum;
}
void Number::setVisible(bool bVisible)
{
    m_bVisible = bVisible;
}
void Number::setColor(QColor& color)
{
    m_Color = color;
}
//--------------------------------------------------------------------------------
GameModel::GameModel(int nRows, int nColumns, int nLowRandomNumber, int nHighRandomNumber,
                     QObject *parent)
    : QAbstractListModel(parent), m_nRows(nRows), m_nColumns(nColumns),
      m_nLowRandomNumber(nLowRandomNumber), m_nHighRandomNumber(nHighRandomNumber)
{
    generateTargetNumber();
    m_nUserSelectedNumber = 0;

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

    RandomNumbersAppearance();

    endResetModel();
}

void GameModel::RandomNumbersAppearance()
{
    srand(time(0));

    int nRandomNumber = m_nLowRandomNumber + rand() % m_nHighRandomNumber;
    int nRandomIndex = rand() % (size() - 1);
    int nIndex = 0;
    bool bVisible = false;
    //
    auto it = m_Numbers.begin();
    for(; it != m_Numbers.end(); ++it)
    {
        // find random index
        if((*it).visible() == false && nRandomIndex == nIndex)
        {
            *it = Number(nRandomNumber, true);
            bVisible = true;
            break;
        }
        // find to right of random index
        else if((*it).visible() == true && nRandomIndex == nIndex)
        {
            it = it + 1;
            for(; it != m_Numbers.end(); ++it)
                if((*it).visible() == false)
                {
                    *it = Number(nRandomNumber, true);
                    bVisible = true;
                    break;
                }
            break;
        }
        ++nIndex;
    }
    // find to left of random index
    if(bVisible == false)
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

    if(nCount > nGameOverCondition)
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
void GameModel::setTargetNumber(int nNum)
{
    m_nTargetNumber = nNum;
}
void GameModel::setUserSelectedNumber(int nNum)
{
    m_nUserSelectedNumber = nNum;
}

int GameModel::targetNumber() const
{
    return m_nTargetNumber;
}
int GameModel::userSelectedNumber() const
{
    return m_nUserSelectedNumber;
}
void GameModel::reactionOnUserAction(int nUserSelectedNumber, int nIndex)
{
    qDebug() << "grid view index = " << nIndex;
    QModelIndex index;

    setUserSelectedNumber(nUserSelectedNumber);

    if(testOnEquality())
    {
        generateTargetNumber();
        if(index.isValid())
        {
            qDebug() << "index is valid";
            setData(index, "green", ColorRole);
            setData(index, nUserSelectedNumber, ValueRole);
        }
        emit targetNumberChanged();
    }
}
bool GameModel::testOnEquality() const
{
    if(m_nUserSelectedNumber == m_nTargetNumber)
        return true;
    return false;
}
void GameModel::generateTargetNumber()
{
    srand(time(0));
    m_nTargetNumber = 1 + rand() % 8;
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
        case ValueRole: return m_Numbers.at(index.row()).number();
        case VisibleRole: return m_Numbers.at(index.row()).visible();
        case ColorRole: return m_Numbers.at(index.row()).color();
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
        int nValue = 0;//value.toInt() + 1;
        Number& rNumber = getItem(index);
        rNumber.setNumber(nValue);
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
    dataChanged(index(row, 0), index(row, 0), { ValueRole, VisibleRole, ColorRole });
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
int GameModel::randomNumber(int nRow, int nColumn)
{
    QModelIndex idx = index(nRow, nColumn);
    return (data(idx)).toInt();
}


