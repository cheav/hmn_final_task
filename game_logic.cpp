#include "game_logic.h"
#include "game_model.h"

#include <cstdlib>
#include <ctime>

#include <QTimer>
#include <QDebug>

GameLogic::GameLogic(QObject *pParent) : QObject(pParent), m_nTargetNumber(0)
{
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(editModel()));
    m_pTimer->setInterval(700);
}
GameModel* GameLogic::gameModel() const
{
    return m_pGameModel;
}
void GameLogic::setGameModel(GameModel *pGameModel)
{
    m_pGameModel = pGameModel;
    onGameModelChanged();
}
void GameLogic::onGameModelChanged()
{
    generateTargetNumber();
    emit targetNumberChanged();
}

void GameLogic::NewRandomNumberVisible()
{
    srand(time(0));

    int nLowRandomNumber = m_pGameModel->lowRandomNumber();
    int nHighRandomNumber = m_pGameModel->highRandomNumber();

    int nRandomNumber = nLowRandomNumber + rand() % nHighRandomNumber;
    int nRandomIndex = rand() % (m_pGameModel->size() - 1);
    int nIndex = 0;
    bool bVisible = false;

    model_iterator it = m_pGameModel->begin();
    model_iterator end = m_pGameModel->end();

    for(; it != end; ++it)
    {
        //QColor color = (*it).color();
        //QModelIndex modelIndex = m_pGameModel->index(nIndex);
        //m_pGameModel->setData(modelIndex, color, GameModel::ColorRole);

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
            for(; it != end; ++it)
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
        //nIndex = 0;
        it = m_pGameModel->begin();
        for(; it != end; ++it)
        {
            //QColor color = (*it).color();
            //QModelIndex modelIndex = m_pGameModel->index(nIndex);
            //m_pGameModel->setData(modelIndex, color, GameModel::ColorRole);

            if((*it).visible() == false)
            {
                *it = Number(nRandomNumber, true);
                break;
            }
        }
    }

    qDebug() << nRandomIndex;
}
bool GameLogic::GameOverCondition()
{
    bool bGameFieldFull = false;

    int nCount = 0;
    auto it = m_pGameModel->begin();
    auto end = m_pGameModel->end();
    for(; it != end; ++it)
        if((*it).visible() == true)
            ++nCount;

    int nGameOverCondition = 2 * m_pGameModel->size();
    nGameOverCondition /= 3;

    if(nCount > nGameOverCondition)
        bGameFieldFull = true;

    return bGameFieldFull;
}
void GameLogic::runGameOver()
{
    m_pTimer->stop();
    emit gameStopped();
    emit gameOver();
}

void GameLogic::editModel()
{
    if(GameOverCondition() == true)
    {
        qDebug() << "Game Over";
        runGameOver();
        return;
    }
    m_pGameModel->beginResetModel();

    NewRandomNumberVisible();

    m_pGameModel->endResetModel();
}

void GameLogic::startGame()
{
    if(m_pGameModel->empty())
        m_pGameModel->fillModel();

    m_pTimer->start();
    emit gameStarted();
}
void GameLogic::stopGame()
{
    m_pTimer->stop();
    m_pGameModel->clearModel();
    emit gameStopped();
}
void GameLogic::pauseGame()
{
    m_pTimer->stop();
    emit gamePaused();
}

void GameLogic::setTargetNumber(int nNum)
{
    m_nTargetNumber = nNum;
}
void GameLogic::setUserSelectedNumber(int nNum)
{
    m_nUserSelectedNumber = nNum;
}

int GameLogic::targetNumber() const
{
    return m_nTargetNumber;
}
int GameLogic::userSelectedNumber() const
{
    return m_nUserSelectedNumber;
}

void GameLogic::reactionOnUserAction(int nUserSelectedNumber, int nIndex, QString strColor)
{
    qDebug() << "grid view index = " << nIndex;
    QModelIndex modelIndex = m_pGameModel->index(nIndex,0);

    setUserSelectedNumber(nUserSelectedNumber);

    m_pGameModel->setData(modelIndex, strColor, GameModel::ColorRole);

    if(testOnEquality())
    {
        generateTargetNumber();
        if(modelIndex.isValid())
        {
            //qDebug() << "index is valid";

            int nNewRandomValue = generateFieldNumber();
            m_pGameModel->setNewRandomValue(nNewRandomValue);

            m_pGameModel->setData(modelIndex, nUserSelectedNumber, GameModel::ValueRole);

            //m_pGameModel->setData(modelIndex, "green", GameModel::ColorRole);
        }
        emit targetNumberChanged();
    }
}
bool GameLogic::testOnEquality() const
{
    if(m_nUserSelectedNumber == m_nTargetNumber)
        return true;
    return false;
}
bool GameLogic::generateTargetNumber()
{
    srand(time(0));
    int nLowRandomNumber = m_pGameModel->lowRandomNumber();
    int nHighRandomNumber = m_pGameModel->highRandomNumber();

    m_nTargetNumber = nLowRandomNumber + rand() % nHighRandomNumber;
    return m_nTargetNumber;
}
bool GameLogic::generateFieldNumber()
{
    srand(time(0));
    int nLowRandomNumber = m_pGameModel->lowRandomNumber();
    int nHighRandomNumber = m_pGameModel->highRandomNumber();

    int nFieldNumber = nLowRandomNumber + rand() % nHighRandomNumber;
    return nFieldNumber;
}

