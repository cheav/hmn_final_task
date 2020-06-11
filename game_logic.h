#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <QObject>
#include <QModelIndex>
#include "number.h"

class GameModel;
class QTimer;

class GameLogic : public QObject
{
    Q_OBJECT
    Q_PROPERTY(GameModel* model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(int targetNumber READ targetNumber WRITE setTargetNumber NOTIFY targetNumberChanged)
public:
    GameLogic(QObject *pParent = nullptr);

    GameModel* model() const;
    void setModel(GameModel* pModel);

    Q_INVOKABLE void startGame();
    Q_INVOKABLE void stopGame();
    Q_INVOKABLE void pauseGame();

    int targetNumber() const;
    void setTargetNumber(int nNum);

    int generateTargetNumber();
    int generateFieldNumber();  
public slots:
    void displayRandomNumber();
    void onModelChanged();
    Q_INVOKABLE void onUserAction(int nUserSelectedNumber, int nIndex, QString strColor);
private:
    void findRandomNumber_in_model();
    bool GameOverCondition();
    void runGameOver();
    //
    int userHitCount() const;
    void incrementUserHitCount();
    void resetUserHitCount();
private:
    GameModel *m_pGameModel;

    struct SelectedNumber
    {
        SelectedNumber(int value, QModelIndex index) : value(value), modelIndex(index) {}
        int value;
        QModelIndex modelIndex;
    };
    // container of user selected numbers on game field:
    QList<SelectedNumber> m_UserSelectedNumbers;
    //
    QTimer *m_pTimer;
    int m_nTargetNumber;
    int m_nGameWinCondition;
    int m_nUserHitCount;
signals:
    void modelChanged();
    void targetNumberChanged();
    void gameStarted();
    void gameStopped();
    void gamePaused();
    void gameOver();
    void gameWin();
};

#endif // GAME_LOGIC_H
