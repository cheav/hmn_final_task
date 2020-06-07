#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include <QAbstractListModel>
#include <QColor>

class QTimer;

class Number
{
public:
    Number(int nNum, bool bVisible);
    int number() const;
    bool visible() const;
    QColor color() const;

    void setNumber(int nNum);
    void setVisible(bool bVisible);
    void setColor(QColor& color);
private:
    int m_nNumber;
    bool m_bVisible;
    QColor m_Color;
};

class GameModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int rows READ rows WRITE setRows NOTIFY rowsChanged)
    Q_PROPERTY(int columns READ columns WRITE setColumns NOTIFY columnsChanged)
    Q_PROPERTY(int targetNumber READ targetNumber WRITE setTargetNumber NOTIFY targetNumberChanged)
public:
    enum NumberRole
    {
        ValueRole = Qt::DisplayRole,
        VisibleRole = Qt::UserRole + 1,
        ColorRole = Qt::UserRole + 2,
    };
    Q_ENUM(NumberRole)

    GameModel(int nRows = 1, int nColumns = 1,
              int nLowRandomNumber = 1, int nHighRandomNumber = 9,
              QObject *parent = nullptr);
public:
    int rowCount(const QModelIndex & = QModelIndex()) const;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = ValueRole) const;
    Q_INVOKABLE bool setData(const QModelIndex &index, const QVariant &value, int role = ColorRole);
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;

    Number &getItem(const QModelIndex &index) const;

    void append(Number number);
    void set(int row, Number number);
    void remove(int row);

    Q_INVOKABLE int size() const;
    Q_INVOKABLE int rows() const;
    Q_INVOKABLE int columns() const;

    Q_INVOKABLE void setRows(int nRows);
    Q_INVOKABLE void setColumns(int nColumns);

    Q_INVOKABLE int randomNumber(int nRow, int nColumn);
    // game logic
    Q_INVOKABLE void startGame();
    Q_INVOKABLE void stopGame();
    Q_INVOKABLE void pauseGame();
    Q_INVOKABLE void setTargetNumber(int nNum);
    Q_INVOKABLE void setUserSelectedNumber(int nNum);
    //
    Q_INVOKABLE int targetNumber() const;
    Q_INVOKABLE int userSelectedNumber() const;
    //
    Q_INVOKABLE void reactionOnUserAction(int nUserSelectedNumber, int nIndex);
    bool testOnEquality() const;
    void generateTargetNumber();
protected:
    QHash<int, QByteArray> roleNames() const;
private slots:
    void fillModel();
    void clearModel();
    void editModel();
private:
    void RandomNumbersAppearance();
    bool GameOverCondition();
    void GameStop();
private:
    QList<Number> m_Numbers;
    QList<Number> m_UserSelectedNumbers;
    int m_nTargetNumber;
    int m_nUserSelectedNumber;
    QTimer *m_pTimer;
    //
    int m_nRows;
    int m_nColumns;
    int m_nLowRandomNumber;
    int m_nHighRandomNumber;
signals:
    void rowsChanged();
    void columnsChanged();
    void targetNumberChanged();
};

#endif // GAME_MODEL_H
