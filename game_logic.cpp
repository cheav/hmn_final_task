#include "game_logic.h"
#include "game_model.h"

#include <cstdlib>
#include <ctime>

#include <QTimer>
#include <QDebug>

GameLogic::GameLogic(QObject *pParent) : QObject(pParent)
{
    generateTargetNumber();

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
}

void GameLogic::RandomNumberVisible()
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
        it = m_pGameModel->begin();
        for(; it != end; ++it)
            if((*it).visible() == false)
            {
                *it = Number(nRandomNumber, true);
                break;
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

    RandomNumberVisible();

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
void GameLogic::reactionOnUserAction(int nUserSelectedNumber, int nIndex)
{
    qDebug() << "grid view index = " << nIndex;
    QModelIndex modelIndex = m_pGameModel->index(nIndex,0);

    setUserSelectedNumber(nUserSelectedNumber);

    if(testOnEquality())
    {
        generateTargetNumber();
        if(modelIndex.isValid())
        {
            //qDebug() << "index is valid";
            m_pGameModel->setData(modelIndex, "green", GameModel::ColorRole);

            int nNewRandomValue = generateFieldNumber();
            m_pGameModel->setNewRandomValue(nNewRandomValue);

            m_pGameModel->setData(modelIndex, nUserSelectedNumber, GameModel::ValueRole);
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
    m_nTargetNumber = 1 + rand() % 8;
    return m_nTargetNumber;
}
bool GameLogic::generateFieldNumber()
{
    srand(time(0));
    int nFieldNumber = 1 + rand() % 8;
    return nFieldNumber;
}

