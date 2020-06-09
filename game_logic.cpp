#include "game_logic.h"
#include "game_model.h"

#include <cstdlib>
#include <ctime>

#include <QTimer>
#include <QDebug>

GameLogic::GameLogic(QObject *pParent) : QObject(pParent), m_nTargetNumber(0),
    m_nGameWinCondition(2)
{
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(editModel()));
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

void GameLogic::NewRandomNumberVisible()
{
    int nRandomNumber = generateFieldNumber();

    srand(time(0));
    int nRandomIndex = rand() % (m_pGameModel->size() - 1);
    int nIndex = 0;
    bool bVisible = false;

    auto it = m_pGameModel->begin();
    auto end = m_pGameModel->end();

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
int GameLogic::targetNumber() const
{
    return m_nTargetNumber;
}

void GameLogic::onUserAction(int nUserSelectedNumber, int nIndex, QString strColor)
{
    // Definition of sum of numbers selected by user on game field
    // and according logic:

    if(nUserSelectedNumber >= targetNumber()) return;

    QModelIndex modelIndex = m_pGameModel->index(nIndex, 0);

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

            static int nCount = 0;

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
                ++ nCount;
            }
            m_UserSelectedNumbers.clear();
            emit targetNumberChanged();

            // game win
            if(nCount == m_nGameWinCondition)
            {
                nCount = 0;
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

