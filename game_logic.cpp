#include "game_logic.h"
#include "game_model.h"

#include <cstdlib>
#include <ctime>

#include <QTimer>
#include <QDebug>

GameLogic::GameLogic(QObject *pParent) : QObject(pParent),
    m_nTargetNumber(0), m_nGameWinCondition(2), m_nUserHitCount(0), m_nGameLevel(1)
{
    srand(time(0));

    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(displayRandomNumber()));
    m_pTimer->setInterval(500);

    connect(this, SIGNAL(gameLevelChanged()), this, SLOT(onGameLevelChanged()));
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

void GameLogic::initIndexesContainer()
{
    int nLastIndex = m_pGameModel->count() - 1;
    for(int nIndex = 0; nIndex <= nLastIndex; ++nIndex)
        m_ItemIndexes.append(nIndex);
}

void GameLogic::gameFieldRandomFilling()
{
    // generate random index (normal distribution):

    int nRandomIndex = rand() % m_ItemIndexes.size();
    int nIndex = m_ItemIndexes.at(nRandomIndex);

    if(!m_ItemIndexes.isEmpty())
        m_ItemIndexes.removeAt(nRandomIndex);

    // edit model item:

    bool bButtonVisible = true;
    QString strButtonColor("white");
    QModelIndex modelIndex = m_pGameModel->index(nIndex);

    m_pGameModel->setData(modelIndex, bButtonVisible, GameModel::VisibleRole);
    m_pGameModel->setData(modelIndex, strButtonColor, GameModel::ColorRole);

    m_pGameModel->incrementVisibleItemsCount();

    //qDebug() << nRandomIndex << "; index = " << nIndex;
}
void GameLogic::setNewInvisibleItem(int nIndex, const QString& strItemColor)
{    
    QModelIndex modelIndex = m_pGameModel->index(nIndex);

    // set new random value in game field button
    int nNewRandomValue = generateFieldNumber();
    m_pGameModel->setData(modelIndex, nNewRandomValue, GameModel::ValueRole);

    // set game field button invisible
    bool bButtonVisible = false;
    m_pGameModel->setData(modelIndex, bButtonVisible, GameModel::VisibleRole);

    // set game field button color
    m_pGameModel->setData(modelIndex, strItemColor, GameModel::ColorRole);

    // for normal distribution filling of game field:
    m_ItemIndexes.append(nIndex);

    m_pGameModel->decrementVisibleItemsCount();
}

bool GameLogic::gameOverCondition()
{
    int nGameOverCondition = 2 * m_pGameModel->count() / 3;
    //int nGameOverCondition = m_pGameModel->count();

    //qDebug() << "buttons count = " << m_pGameModel->visibleItemsCount();

    if(m_pGameModel->visibleItemsCount() > nGameOverCondition)
        return true;
    return false;
}
void GameLogic::runGameOver()
{
    m_pTimer->stop();
    m_ItemIndexes.clear();
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
    if(m_ItemIndexes.isEmpty())
    {
        initIndexesContainer();
    }

    m_pTimer->start();
    emit gameStarted();
}
void GameLogic::stopGame()
{
    m_pTimer->stop();

    m_ItemIndexes.clear();
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

            // Disappearance of buttons from game field:

            // Last button pressed by user:
            setNewInvisibleItem(nIndex, strWhiteItemColor);

            // Buttons pressed by user before last button:

            auto it = m_UserSelectedItems.begin();
            for(; it != m_UserSelectedItems.end(); ++it)
            {
                nIndex = (*it).m_nIndex;
                setNewInvisibleItem(nIndex, strWhiteItemColor);
            }

            incrementUserHitCount();
            m_UserSelectedItems.clear();
            generateTargetNumber();


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

void GameLogic::onGameLevelChanged()
{
    stopGame();

    typedef std::pair<int,int> p;
    static std::array<p, 5> gameLevels = { p{1,9}, p{1,14}, p{1,19}, p{1,24}, p{1,29} };

    m_pGameModel->setLowRandomNumber( gameLevels[m_nGameLevel - 1].first );
    m_pGameModel->setHighRandomNumber( gameLevels[m_nGameLevel - 1].second );

//    qDebug() << "low = " << m_pGameModel->lowRandomNumber() <<
//                ", high = " << m_pGameModel->highRandomNumber();
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

int GameLogic::gameLevel() const
{
    return m_nGameLevel;
}
void GameLogic::setGameLevel(int nLevel)
{
    m_nGameLevel = nLevel;
    emit gameLevelChanged();
}
