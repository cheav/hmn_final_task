#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include <QAbstractListModel>

class QTimer;

class Number
{
public:
    Number(int nNum, bool bVisible);
    int number() const;
    void setNumber(int nNum);
    //
    bool visible() const;
    void setVisible(bool bVisible);
private:
    int m_nNumber;
    bool m_bVisible;
};

class GameModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int rows READ rows WRITE setRows NOTIFY rowsChanged)
    Q_PROPERTY(int columns READ columns WRITE setColumns NOTIFY columnsChanged)
public:
    enum NumberRole
    {
        DisplayRole = Qt::DisplayRole,
        VisibleRole = Qt::UserRole + 1
    };
    Q_ENUM(NumberRole)

    GameModel(int nRows = 1, int nColumns = 1,
              int nLowRandomNumber = 1, int nHighRandomNumber = 9,
              QObject *parent = nullptr);
public:
    int rowCount(const QModelIndex & = QModelIndex()) const;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;

    Q_INVOKABLE void append(Number number);
    Q_INVOKABLE void set(int row, Number number);
    Q_INVOKABLE void remove(int row);

    Q_INVOKABLE int size() const;
    Q_INVOKABLE int rows() const;
    Q_INVOKABLE int columns() const;

    Q_INVOKABLE void setRows(int nRows);
    Q_INVOKABLE void setColumns(int nColumns);

    Q_INVOKABLE int randomNumber(int row, int column);
    // game logic
    Q_INVOKABLE void startGame();
    Q_INVOKABLE void stopGame();
    Q_INVOKABLE void pauseGame();
protected:
    QHash<int, QByteArray> roleNames() const;
private slots:
    void fillModel();
    void clearModel();
    void editModel();
private:
    bool GameOverCondition();
    void GameStop();
private:
    QList<Number> m_Numbers;
    QTimer *m_pTimer;
    //
    int m_nRows;
    int m_nColumns;
    int m_nLowRandomNumber;
    int m_nHighRandomNumber;
signals:
    void rowsChanged();
    void columnsChanged();
};

#endif // GAME_MODEL_H
