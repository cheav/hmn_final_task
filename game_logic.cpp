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

void GameLogic::onUserAction(int nUserSelectedNumber, int nIndex, QString strColor)
{
    if(nUserSelectedNumber >= targetNumber()) return;

    QModelIndex modelIndex = m_pGameModel->index(nIndex, 0);
    setUserSelectedNumber(nUserSelectedNumber);

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

                // set not visible new random value by current model index
                int nNewRandomValue = generateFieldNumber();
                m_pGameModel->setData(modelIndex, nNewRandomValue, GameModel::ValueRole);

                // set other not visible new random values by their model indexes
                auto it = m_UserSelectedNumbers.begin();
                for(; it != m_UserSelectedNumbers.end(); ++it)
                {
                    int nNewRandomValue = generateFieldNumber();
                    m_pGameModel->setData((*it).modelIndex, nNewRandomValue, GameModel::ValueRole);
                }
            }
            emit targetNumberChanged();
            m_UserSelectedNumbers.clear();
        }
    }
}

bool GameLogic::generateTargetNumber()
{
    srand(time(0));
    int nLowRandomNumber = 3 + m_pGameModel->lowRandomNumber();
    int nHighRandomNumber = 5 + m_pGameModel->highRandomNumber();

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

