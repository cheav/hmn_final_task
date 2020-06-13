#include "game_logic.h"
#include "game_model.h"

#include <cstdlib>
#include <ctime>

#include <QTimer>
#include <QDebug>

GameLogic::GameLogic(QObject *pParent) : QObject(pParent), m_nTargetNumber(0),
    m_nGameWinCondition(2), m_nUserHitCount(0)
{
    srand(time(0));

    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(displayRandomNumber()));
    m_pTimer->setInterval(500);
}
GameLogic::SelectedItem::SelectedItem(int nValue, int nIndex)
    : m_nValue(nValue), m_nIndex(nIndex) {}

GameModel* GameLogic::model() const
{
    return m_pGameModel;
}
void GameLogic::setModel(GameModel *pModel)
{
    m_pGameModel = pModel;   
    emit modelChanged();
}

void GameLogic::gameFieldRandomFilling()
{
    // generate random index:
    int nLastIndex = m_pGameModel->size() - 1;
    int nRandomIndex = rand() % nLastIndex;

    bool bInvisiblItem_Found = false;

    auto begin = m_pGameModel->begin();
    auto end = m_pGameModel->end();
    auto it = begin;
    auto it_after_random_index_it = it;

    int nIndex = 0;
    for(; it != end; ++it, ++nIndex)
    {
        // find random index
        if((*it).visible() == false && nRandomIndex == nIndex)
        {
            bInvisiblItem_Found = true;
            editModelItem(nIndex);
            //qDebug() << "target";
            break;
        }
        // find to right of random index
        else if((*it).visible() == true && nRandomIndex == nIndex)
        {
            it = it + 1;
            nIndex += 1;
            it_after_random_index_it = it;
            for(; it != end; ++it, ++nIndex)
                if((*it).visible() == false)
                {
                    bInvisiblItem_Found = true;
                    editModelItem(nIndex);
                    //qDebug() << "right";
                    break;
                }
            break;
        }
    }
    // find to left of random index
    if(bInvisiblItem_Found == false)
    {
        it = begin;
        for(nIndex = 0; it != it_after_random_index_it; ++it, ++nIndex)
            if((*it).visible() == false)
            {
                editModelItem(nIndex);
                //qDebug() << "left";
                break;
            }
    }

    //qDebug() << nRandomIndex;
}
void GameLogic::editModelItem(int nIndex)
{
    bool bButtonVisible = true;
    QString strButtonColor("white");
    QModelIndex modelIndex = m_pGameModel->index(nIndex);

    m_pGameModel->setData(modelIndex, bButtonVisible, GameModel::VisibleRole);
    m_pGameModel->setData(modelIndex, strButtonColor, GameModel::ColorRole);

    m_pGameModel->incrementVisibleItemsCount();
}

bool GameLogic::gameOverCondition()
{
    int nGameOverCondition = 2 * m_pGameModel->size() / 3;
    //int nGameOverCondition = m_pGameModel->size();

    //qDebug() << "buttons count = " << m_pGameModel->visibleItemsCount();

    if(m_pGameModel->visibleItemsCount() > nGameOverCondition)
        return true;
    return false;
}
void GameLogic::runGameOver()
{
    m_pTimer->stop();
    m_UserSelectedItems.clear();
    emit gameStopped();
    emit gameOver();
}

int GameLogic::userHitCount() const
{
    return m_nUserHitCount;
}
void GameLogic::incrementUserHitCount()
{
    ++ m_nUserHitCount;
}
void GameLogic::resetUserHitCount()
{
    m_nUserHitCount = 0;
}


void GameLogic::displayRandomNumber()
{
    if(gameOverCondition() == true)
    {
        qDebug() << "Game Over";
        runGameOver();
        return;
    }

    gameFieldRandomFilling();
}

void GameLogic::startGame()
{
    if(m_pGameModel->empty())
    {
        m_pGameModel->fillModel();
        generateTargetNumber();
    }

    m_pTimer->start();
    emit gameStarted();
}
void GameLogic::stopGame()
{
    m_pTimer->stop();

    m_UserSelectedItems.clear();
    m_pGameModel->clearModel();
    m_pGameModel->resetVisibleItemsCount();
    resetUserHitCount();

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
int GameLogic::targetNumber() const
{
    return m_nTargetNumber;
}

void GameLogic::onUserAction(int nIndex, const QString& strUserSelectedNumber)
{
    // Calculation of sum of numbers selected by user on game field
    // and according logic:

    QModelIndex modelIndex = m_pGameModel->index(nIndex);
    int nUserSelectedNumber = strUserSelectedNumber.toInt();

    QString strItemColor("lightblue");
    QString strWhiteItemColor("white");

    if(nUserSelectedNumber >= targetNumber())
    {
        m_pGameModel->setData(modelIndex, strWhiteItemColor, GameModel::ColorRole);
        return;
    }

    if(m_UserSelectedItems.empty())
    {
        m_UserSelectedItems.append(SelectedItem(nUserSelectedNumber, nIndex));
        m_pGameModel->setData(modelIndex, strItemColor, GameModel::ColorRole);
        qDebug() << nUserSelectedNumber;
    }
    else
    {
        int nSum = 0;
        auto it = m_UserSelectedItems.begin();
        for(; it != m_UserSelectedItems.end(); ++it)
        {
            int nValue = (*it).m_nValue;
            nSum += nValue;
        }
        nSum += nUserSelectedNumber;

        if(nSum < targetNumber())
        {
            m_UserSelectedItems.append(SelectedItem(nUserSelectedNumber, nIndex));
            m_pGameModel->setData(modelIndex, strItemColor, GameModel::ColorRole);
            qDebug() << nUserSelectedNumber;
        }
        else if(nSum == targetNumber())
        {
            qDebug() << "sum = " << nSum << " !";

            if(modelIndex.isValid())
            {
                // Disappearance of buttons from game field:

                // Last button pressed by user:

                // set new random value in game field button
                int nNewRandomValue = generateFieldNumber();
                m_pGameModel->setData(modelIndex, nNewRandomValue, GameModel::ValueRole);

                // set game field button invisible
                bool bButtonVisible = false;
                m_pGameModel->setData(modelIndex, bButtonVisible, GameModel::VisibleRole);

                // set game field button white
                m_pGameModel->setData(modelIndex, strWhiteItemColor, GameModel::ColorRole);

                m_pGameModel->decrementVisibleItemsCount();

                // Buttons pressed by user before last button:

                auto it = m_UserSelectedItems.begin();
                for(; it != m_UserSelectedItems.end(); ++it)
                {
                    int nNewRandomValue = generateFieldNumber();
                    QModelIndex modelIndex = m_pGameModel->index((*it).m_nIndex);

                    m_pGameModel->setData(modelIndex, nNewRandomValue, GameModel::ValueRole);
                    m_pGameModel->setData(modelIndex, bButtonVisible, GameModel::VisibleRole);
                    m_pGameModel->setData(modelIndex, strWhiteItemColor, GameModel::ColorRole);

                    m_pGameModel->decrementVisibleItemsCount();
                }

                incrementUserHitCount();
                m_UserSelectedItems.clear();
                generateTargetNumber();
            }

            // game win
            if(m_nGameWinCondition == userHitCount())
            {
                resetUserHitCount();
                pauseGame();
                emit gameWin();
            }
            //
        } // else if(nSum == targetNumber())
    } // else

    return;
}

int GameLogic::generateTargetNumber()
{
    // game field numbers limits:
    int nMinFieldNumber = m_pGameModel->lowRandomNumber();
    int nMaxFieldNumber = m_pGameModel->lowRandomNumber() + m_pGameModel->highRandomNumber();

    // formulas for target number limits based on game field numbers limits:
    int nLowRandomNumber = (nMinFieldNumber + nMaxFieldNumber) / 2;
    int nHighRandomNumber = 3 * nMaxFieldNumber / 2;

    m_nTargetNumber = nLowRandomNumber + rand() % nHighRandomNumber;
    emit targetNumberChanged();

    return m_nTargetNumber;
}
int GameLogic::generateFieldNumber()
{
    int nLowRandomNumber = m_pGameModel->lowRandomNumber();
    int nHighRandomNumber = m_pGameModel->highRandomNumber();

    int nFieldNumber = nLowRandomNumber + rand() % nHighRandomNumber;
    return nFieldNumber;
}

