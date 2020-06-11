#include "game_logic.h"
#include "game_model.h"

#include <cstdlib>
#include <ctime>

#include <QTimer>
#include <QDebug>

GameLogic::GameLogic(QObject *pParent) : QObject(pParent), m_nTargetNumber(0),
    m_nGameWinCondition(2), m_nUserHitCount(0)
{
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(displayRandomNumber()));
    m_pTimer->setInterval(700);
}
GameModel* GameLogic::model() const
{
    return m_pGameModel;
}
void GameLogic::setModel(GameModel *pModel)
{
    m_pGameModel = pModel;
    onModelChanged();
}
void GameLogic::onModelChanged()
{
    generateTargetNumber();
}

void GameLogic::findRandomNumber_in_model()
{
    srand(time(0));

    // generate random index:
    int nLastIndex = m_pGameModel->size() - 1;
    int nRandomIndex = rand() % nLastIndex;

    // generate random bias from random index:
    int nRandomBias = 1;
    int nDiffIndex = nLastIndex - nRandomIndex;
    if(nDiffIndex != 0)
        nRandomBias = rand() % nDiffIndex;


    bool bFound_invisible_number = false;
    QString strButtonColor("white");

    auto begin = m_pGameModel->begin();
    auto end = m_pGameModel->end();
    auto it = begin;

    for(int nIndex = 0; it != end; ++it, ++nIndex)
    {
        // find random index
        if((*it).visible() == false && nRandomIndex == nIndex)
        {
            bFound_invisible_number = true;
            (*it).setVisible(true);
            (*it).setColor(strButtonColor);
            //qDebug() << "target";
            break;
        }
        // find to right of random index
        else if((*it).visible() == true && nRandomIndex == nIndex)
        {
            it = it + nRandomBias;
            for(; it != end; ++it)
                if((*it).visible() == false)
                {
                    bFound_invisible_number = true;
                    (*it).setVisible(true);
                    (*it).setColor(strButtonColor);
                    //qDebug() << "right";
                    break;
                }
            break;
        }
    }
    // find from begin + random bias
    if(bFound_invisible_number == false && nRandomIndex < 2 * nLastIndex / 3)
    {
        it = begin + nRandomBias;
        for(; it != end; ++it)
            if((*it).visible() == false)
            {
                bFound_invisible_number = true;
                (*it).setVisible(true);
                (*it).setColor(strButtonColor);
                //qDebug() << "from begin";
                break;
            }
    }
    // find from end (all elems iteration)
    if(bFound_invisible_number == false)
    {
        it = end - 1;
        for(; it != begin - 1; --it)
            if((*it).visible() == false)
            {
                bFound_invisible_number = true;
                (*it).setVisible(true);
                (*it).setColor(strButtonColor);
                //qDebug() << "from end";
                break;
            }
    }

    //qDebug() << nRandomIndex;
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

    findRandomNumber_in_model();

    m_pGameModel->endResetModel();
}

void GameLogic::startGame()
{
    if(m_pGameModel->empty())
    {
        m_pGameModel->fillModel();
        generateTargetNumber();
        emit targetNumberChanged();
    }

    m_pTimer->start();
    emit gameStarted();
}
void GameLogic::stopGame()
{
    m_pTimer->stop();
    m_pGameModel->clearModel();
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

void GameLogic::onUserAction(int nUserSelectedNumber, int nIndex, QString strColor)
{
    // Calculation of sum of numbers selected by user on game field
    // and according logic:

    if(nUserSelectedNumber >= targetNumber()) return;

    QModelIndex modelIndex = m_pGameModel->index(nIndex);

    if(m_UserSelectedNumbers.empty())
    {
        m_UserSelectedNumbers.append(SelectedNumber(nUserSelectedNumber, modelIndex));
        m_pGameModel->setData(modelIndex, strColor, GameModel::ColorRole);
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
            m_pGameModel->setData(modelIndex, strColor, GameModel::ColorRole);
            qDebug() << nUserSelectedNumber;
        }
        else if(nSum == targetNumber())
        {
            generateTargetNumber();
            qDebug() << "sum = " << nSum << " !";

            if(modelIndex.isValid())
            {
                // Disappearance of numbers from game field:

                // set new random value (like invisible) by current model index
                int nNewRandomValue = generateFieldNumber();
                m_pGameModel->setData(modelIndex, nNewRandomValue, GameModel::ValueRole);

                // set other new random values (like invisibles) by their model indexes
                auto it = m_UserSelectedNumbers.begin();
                for(; it != m_UserSelectedNumbers.end(); ++it)
                {
                    int nNewRandomValue = generateFieldNumber();
                    m_pGameModel->setData((*it).modelIndex, nNewRandomValue, GameModel::ValueRole);
                }
                //
                incrementUserHitCount();
            }
            m_UserSelectedNumbers.clear();
            emit targetNumberChanged();

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
    srand(time(0));

    // game field numbers limits:
    int nMinFieldNumber = m_pGameModel->lowRandomNumber();
    int nMaxFieldNumber = m_pGameModel->lowRandomNumber() + m_pGameModel->highRandomNumber();

    // formulas for target number limits based on game field numbers limits:
    int nLowRandomNumber = (nMinFieldNumber + nMaxFieldNumber) / 2;
    int nHighRandomNumber = 3 * nMaxFieldNumber / 2;

    m_nTargetNumber = nLowRandomNumber + rand() % nHighRandomNumber;
    return m_nTargetNumber;
}
int GameLogic::generateFieldNumber()
{
    srand(time(0));
    int nLowRandomNumber = m_pGameModel->lowRandomNumber();
    int nHighRandomNumber = m_pGameModel->highRandomNumber();

    int nFieldNumber = nLowRandomNumber + rand() % nHighRandomNumber;
    return nFieldNumber;
}

