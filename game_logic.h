#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <QObject>
#include "number.h"

class GameModel;
class QTimer;

class GameLogic : public QObject
{
    Q_OBJECT
    Q_PROPERTY(GameModel* gameModel READ gameModel WRITE setGameModel NOTIFY gameModelChanged)
    Q_PROPERTY(int targetNumber READ targetNumber WRITE setTargetNumber NOTIFY targetNumberChanged)
public:
    GameLogic(QObject *pParent = nullptr);

    GameModel* gameModel() const;
    void setGameModel(GameModel* pGameModel);

    Q_INVOKABLE void startGame();
    Q_INVOKABLE void stopGame();
    Q_INVOKABLE void pauseGame();

    void setTargetNumber(int nNum);
    void setUserSelectedNumber(int nNum);

    int targetNumber() const;
    int userSelectedNumber() const;

    Q_INVOKABLE void reactionOnUserAction(int nUserSelectedNumber, int nIndex, QString strColor);
    bool testOnEquality() const;
    bool generateTargetNumber();
    bool generateFieldNumber();

    void NewRandomNumberVisible();
    bool GameOverCondition();
    void runGameOver();
public slots:
    void editModel();
    void onGameModelChanged();
private:
    GameModel *m_pGameModel;
    //
    //QList<Number> m_UserSelectedNumbers;
    int m_nTargetNumber;
    int m_nUserSelectedNumber;
    QTimer *m_pTimer;
signals:
    void gameModelChanged();
    void targetNumberChanged();
    void gameStarted();
    void gameStopped();
    void gamePaused();
    void gameOver();
};

#endif // GAME_LOGIC_H
