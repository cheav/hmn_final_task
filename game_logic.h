#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <QObject>
#include <QVector>
#include <QList>

class QTimer;
class GameModel;

class GameLogic : public QObject
{
    Q_OBJECT
    Q_PROPERTY(GameModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(int targetNumber READ targetNumber WRITE setTargetNumber NOTIFY targetNumberChanged)
    Q_PROPERTY(int gameLevel READ gameLevel WRITE setGameLevel NOTIFY gameLevelChanged)
public:
    GameLogic(QObject *pParent = nullptr);

    GameModel* model() const;
    void setModel(GameModel* pModel);

    Q_INVOKABLE void initIndexesContainer();

    Q_INVOKABLE void startGame();
    Q_INVOKABLE void stopGame();
    Q_INVOKABLE void pauseGame();

    int targetNumber() const;
    void setTargetNumber(int nNum);

    Q_INVOKABLE int generateTargetNumber();
    Q_INVOKABLE int generateFieldNumber();

    int gameLevel() const;
    void setGameLevel(int nLevel);
public slots:
    void displayRandomNumber();
    Q_INVOKABLE void onUserAction(int nIndex, const QString& strUserSelectedNumber);
private slots:
    void onGameLevelChanged();
private:
    void gameFieldRandomFilling();
    void setNewInvisibleItem(int nIndex, const QString &strItemColor);
    bool gameOverCondition();
    void runGameOver();
    //
    int userHitCount() const;
    void incrementUserHitCount();
    void resetUserHitCount();
private:
    GameModel *m_pGameModel;
    // for game field filling by normal distribution:
    QList<int> m_ItemIndexes;

    struct SelectedItem
    {
        SelectedItem(int nValue = 0, int nIndex = 0);
        int m_nValue;
        int m_nIndex;
    };
    // container of user selected numbers on game field:
    QVector<SelectedItem> m_UserSelectedItems;
    //
    QTimer *m_pTimer;
    int m_nTargetNumber;
    int m_nGameWinCondition;
    int m_nUserHitCount;
    int m_nGameLevel;
signals:
    void modelChanged();
    void targetNumberChanged();
    void gameStarted();
    void gameStopped();
    void gamePaused();
    void gameOver();
    void gameWin();
    void gameLevelChanged();
};

#endif // GAME_LOGIC_H
