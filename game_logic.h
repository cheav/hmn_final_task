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

    void setTargetNumber(int nNum);
    void setUserSelectedNumber(int nNum);

    int targetNumber() const;
    int userSelectedNumber() const;

    bool generateTargetNumber();
    bool generateFieldNumber();

    void NewRandomNumberVisible();
    bool GameOverCondition();
    void runGameOver();
public slots:
    void editModel();
    void onModelChanged();
    Q_INVOKABLE void onUserAction(int nUserSelectedNumber, int nIndex, QString strColor);
private:
    GameModel *m_pGameModel;

    struct SelectedNumber
    {
        SelectedNumber(int value, QModelIndex index) : value(value), modelIndex(index) {}
        int value;
        QModelIndex modelIndex;
    };

    QList<SelectedNumber> m_UserSelectedNumbers;
    int m_nTargetNumber;
    int m_nUserSelectedNumber;
    QTimer *m_pTimer;
signals:
    void modelChanged();
    void targetNumberChanged();
    void gameStarted();
    void gameStopped();
    void gamePaused();
    void gameOver();
};

#endif // GAME_LOGIC_H
