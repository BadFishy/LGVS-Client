#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QVector>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QMessageBox>
#include <QSettings>
#include <QPixmap>

class GameBoard : public QWidget {
    Q_OBJECT
public:

    explicit GameBoard(QWidget *parent = 0);
    ~GameBoard();
    bool eventFilter(QObject *obj, QEvent *ev);
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *mouseEv);
    bool isWin(int row, int col);
    bool isDangerous(int row, int col);

    enum class State {
        NONE = 0,
        WHITE = 1,
        BLACK = 2,
    };

    QColor playerColor;
    QColor enemyColor;
    bool inRound;
    bool isHint;

    struct Chess {
        int row;
        int col;
        QColor color;
        Chess() { }
        Chess(int row, int col, QColor color): row(row), col(col), color(color) { }

        int getX() {
            return GameBoard::chessSpace * (col);
        }

        int getY() {
            return GameBoard::chessSpace * (row);
        }

        QString toString() {
            return QString("%1,%2").
                   arg(QString::number(row),
                       QString::number(col));
        }
    };

    const std::vector<Chess> &getPlayerChesses() const;
    const std::vector<Chess> &getEnemyChesses() const;

signals:
    void addChess(QString chess);
    void win();

private slots:
    void addEnemysChess(int row, int col);

private:
    bool isRunning;
    State states[19][19] = {{(State)0}};
    static int sideLength;
    static int boardSize;
    static int sideMargin;
    static int chessSpace;

    std::vector<Chess> playerChesses;
    std::vector<Chess> enemyChesses;

    inline int toIndex(int pixel) const;
};

#endif // GAMEBOARD_H
