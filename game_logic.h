#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <QObject>
#include <QModelIndex>
#include "number.h"

class QTimer;
class GameModel;

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

    Q_INVOKABLE int generateTargetNumber();
    Q_INVOKABLE int generateFieldNumber();
public slots:
    void displayRandomNumber();
    Q_INVOKABLE void onUserAction(int nUserSelectedNumber, int nIndex, const QString &strButtonColor);
private:
    void gameFieldRandomFilling();
    void editModelItem(int nIndex);
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
