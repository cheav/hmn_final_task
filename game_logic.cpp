#include "game_logic.h"

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

    bool bFound_invisible_button = false;

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
            bFound_invisible_button = true;
            editModelItem(it, nIndex);
            //qDebug() << "target";
            break;
        }
        // find to right of random index
        else if((*it).visible() == true && nRandomIndex == nIndex)
        {
            it = it + 1;
            nIndex += 1;
            it_after_random_index_it = it;
            for(; it != end; ++it)
                if((*it).visible() == false)
                {
                    bFound_invisible_button = true;
                    editModelItem(it, nIndex);
                    //qDebug() << "right";
                    break;
                }
            break;
        }
    }
    // find to left of random index
    if(bFound_invisible_button == false)
    {
        it = begin;
        for(nIndex = 0; it != it_after_random_index_it; ++it, ++nIndex)
            if((*it).visible() == false)
            {
                editModelItem(it, nIndex);
                //qDebug() << "left";
                break;
            }
    }

    //qDebug() << nRandomIndex;
}
void GameLogic::editModelItem(model_iterator& it, int nIndex)
{
    (*it).setVisible(true);

    // slowly:
    //QModelIndex modelIndex = m_pGameModel->index(nIndex);
    //m_pGameModel->setData(modelIndex, true, GameModel::VisibleRole);
    //m_pGameModel->setData(modelIndex, QString("white"), GameModel::ColorRole);

    m_pGameModel->incrementVisibleButtonsCount();
}

bool GameLogic::GameOverCondition()
{
    int nGameOverCondition = 2 * m_pGameModel->size() / 3;
    //int nGameOverCondition = m_pGameModel->size();

    //qDebug() << "buttons count = " << m_pGameModel->visibleButtonsCount();

    if(m_pGameModel->visibleButtonsCount() > nGameOverCondition)
        return true;
    return false;
}
void GameLogic::runGameOver()
{
    m_pTimer->stop();
    m_UserSelectedNumbers.clear();
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
    if(GameOverCondition() == true)
    {
        qDebug() << "Game Over";
        runGameOver();
        return;
    }

    m_pGameModel->beginResetModel();

    gameFieldRandomFilling();

    m_pGameModel->endResetModel();
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

    m_UserSelectedNumbers.clear();
    m_pGameModel->clearModel();
    m_pGameModel->resetVisibleButtonsCount();
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

void GameLogic::onUserAction(int nUserSelectedNumber, int nIndex, const QString& strButtonColor)
{
    // Calculation of sum of numbers selected by user on game field
    // and according logic:

    if(nUserSelectedNumber >= targetNumber()) return;

    QModelIndex modelIndex = m_pGameModel->index(nIndex);

    if(m_UserSelectedNumbers.empty())
    {
        m_UserSelectedNumbers.append(SelectedNumber(nUserSelectedNumber, modelIndex));
        m_pGameModel->setData(modelIndex, strButtonColor, GameModel::ColorRole);
        qDebug() << nUserSelectedNumber;
    }
    else
    {
        int nSum = 0;
        auto it = m_UserSelectedNumbers.begin();
        for(; it != m_UserSelectedNumbers.end(); ++it)
        {
            int nValue = (*it).value;
            nSum += nValue;
        }
        nSum += nUserSelectedNumber;

        if(nSum < targetNumber())
        {
            m_UserSelectedNumbers.append(SelectedNumber(nUserSelectedNumber, modelIndex));
            m_pGameModel->setData(modelIndex, strButtonColor, GameModel::ColorRole);
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
                QString strButtonWhite("white");
                m_pGameModel->setData(modelIndex, strButtonWhite, GameModel::ColorRole);

                m_pGameModel->decrementVisibleButtonsCount();

                // Buttons pressed by user before last button above:
                auto it = m_UserSelectedNumbers.begin();
                for(; it != m_UserSelectedNumbers.end(); ++it)
                {
                    int nNewRandomValue = generateFieldNumber();
                    m_pGameModel->setData((*it).modelIndex, nNewRandomValue, GameModel::ValueRole);
                    m_pGameModel->setData((*it).modelIndex, bButtonVisible, GameModel::VisibleRole);
                    m_pGameModel->setData((*it).modelIndex, strButtonWhite, GameModel::ColorRole);

                    m_pGameModel->decrementVisibleButtonsCount();
                }

                incrementUserHitCount();
                m_UserSelectedNumbers.clear();

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

