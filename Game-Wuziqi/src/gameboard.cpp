#include <src/gameboard.h>


namespace
{
GameBoard::State toState(QColor color)
{
    return (color == Qt::white)? GameBoard::State::WHITE: GameBoard::State::BLACK;
}

}

int GameBoard::sideLength = 540;
int GameBoard::boardSize = 19;
int GameBoard::sideMargin = 25;
int GameBoard::chessSpace = GameBoard::sideLength / (GameBoard::boardSize - 1);

GameBoard::GameBoard(QWidget *parent):
    QWidget(parent),
    inRound(false),
    isRunning(false),
    isHint(false)
{
    playerColor = Qt::white;
    enemyColor = Qt::black;
    installEventFilter(this);

    for (int i = 0; i < 19; i++)
        for (int j = 0; j < 19; j++)
            states[i][j] = State::NONE;
}

GameBoard::~GameBoard()
{

}

bool GameBoard::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj != this)
    {
        return false;
    }

    if (ev->type() == QEvent::MouseButtonDblClick)
    {
        return true;
    }

    if (ev->type() != QEvent::MouseButtonPress)
    {
        return false;
    }

    if (!inRound)
        return true;

    QMouseEvent *mouseEv = static_cast<QMouseEvent *>(ev);

    if (states[toIndex(mouseEv->y())][toIndex(mouseEv->x())] == State::WHITE || states[toIndex(mouseEv->y())][toIndex(mouseEv->x())] == State::BLACK)
    {
        return true;
    }
    if(toIndex(mouseEv->y()) > 18 || toIndex(mouseEv->x() > 18))
        return true;

    return false;
}

void GameBoard::paintEvent(QPaintEvent *ev) {
    QPainter p(this);
    p.setBrush(QColor(210,184,140));
    //212, 120, 12; 231, 134, 12; 210,184,140
    p.setPen(Qt::NoPen);
    p.drawRect(0, 0, sideLength + 2 * sideMargin, sideLength + 2 * sideMargin);
    p.translate(sideMargin, sideMargin);
    p.setPen(QPen(Qt::black, 1));

    for (int i = 0; i < boardSize; i++) {
        p.drawLine(0, i * chessSpace, sideLength, i * chessSpace);
        p.drawLine(i * chessSpace, 0, i * chessSpace, sideLength);
    }

    p.setPen(QPen(Qt::black, 5));
    p.drawPoint(3 * chessSpace, 3 * chessSpace);
    p.drawPoint(15 * chessSpace, 3 * chessSpace);
    p.drawPoint(3 * chessSpace, 15 * chessSpace);
    p.drawPoint(15 * chessSpace, 15 * chessSpace);
    p.drawPoint(3 * chessSpace, 9 * chessSpace);
    p.drawPoint(9 * chessSpace, 3 * chessSpace);
    p.drawPoint(9 * chessSpace, 15 * chessSpace);
    p.drawPoint(15 * chessSpace, 9 * chessSpace);
    p.drawPoint(9 * chessSpace, 9 * chessSpace);

    p.setPen(Qt::NoPen);
    p.setBrush(playerColor);

    for (int i = 0; i < playerChesses.size(); i++) {
        if (playerChesses[i].row == -1)
            continue;

        p.drawEllipse(QPoint(playerChesses[i].getX(), playerChesses[i].getY()), (int)(chessSpace * 0.4), (int)(chessSpace * 0.4));
    }

    p.setBrush(enemyColor);

    for (int i = 0; i < enemyChesses.size(); i++) {
        if (enemyChesses[i].row == -1)
            continue;

        p.drawEllipse(QPoint(enemyChesses[i].getX(), enemyChesses[i].getY()), (int)(chessSpace * 0.4), (int)(chessSpace * 0.4));
    }

    p.setRenderHint(QPainter::Antialiasing);
    //qDebug() << isHint;
    for(int i = 0; i < 19; i++)
        for(int j = 0; j < 19; j++)
        {
            if(isHint && isDangerous(j,i))
                p.drawPixmap((int)(i*chessSpace-0.4*chessSpace),(int)(j*chessSpace-0.4*chessSpace),(int)(0.8*chessSpace),(int)(0.8*chessSpace),QPixmap(":/rec/dilei2.png"));
        }
}

void GameBoard::mousePressEvent(QMouseEvent *mouseEv)
{
    //qDebug() << "chessssss";
    int row = toIndex(mouseEv->y());
    int col = toIndex(mouseEv->x());
    if(row > 18 || col > 18)
        return;

    states[row][col] = toState(playerColor);
    Chess chess(row,col,playerColor);
    playerChesses.push_back(chess);

    emit addChess(chess.toString());
    update();

    if(isWin(row,col))
        emit win();

    inRound = false;
}

bool GameBoard::isWin(int row, int col)
{
    State state = states[row][col];
    int cnt = 1;

    for (int i = row - 1; i >= 0; i--)
    {
        if (states[i][col] != state)
            break;

        ++cnt;
    }
    for (int i = row + 1; i < boardSize; i++)
    {
        if (states[i][col] != state)
            break;
        ++cnt;
    }
    if (cnt >= 5)
        return true;

    cnt = 1;

    for (int i = col - 1; i >= 0; i--)
    {
        if (states[row][i] != state)
            break;

        ++cnt;
    }
    for (int i = col + 1; i < boardSize; i++)
    {
        if (states[row][i] != state)
            break;
        ++cnt;
    }
    if (cnt >= 5)
        return true;

    cnt = 1;

    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--)
    {
        if (states[i][j] != state)
            break;
        ++cnt;
    }
    for (int i = row + 1, j = col + 1; i < boardSize && j < boardSize; i++, j++)
    {
        if (states[i][j] != state)
            break;
        ++cnt;
    }
    if (cnt >= 5)
        return true;

    cnt = 1;

    for (int i = row - 1, j = col + 1; i >= 0 && j < boardSize; i--, j++)
    {
        if (states[i][j] != state)
            break;
        ++cnt;
    }
    for (int i = row + 1, j = col - 1; i < boardSize && j >= 0; i++, j--)
    {
        if (states[i][j] != state)
            break;
        ++cnt;
    }
    if (cnt >= 5)
        return true;

    return false;
}

bool GameBoard::isDangerous(int row, int col)//现在是比较全面的安全提示,包括双三,四等,如需变成大作业要求,则space=0且令其等于3或四
{
    if(states[row][col] != State::NONE)
        return false;
    State state;

    if(playerColor == QColor(255,255,255))
        state = State::BLACK;
    else if(playerColor == QColor(0,0,0))
        state = State::WHITE;
    //Whether it is player or enemy

    int con = 1, block = 0, space = 0,po = 0, huo3 = 0, si4 = 0;

    for(int i = row - 1; i >= 0; i--)
    {
        if(states[i][col] == state)
        {
            con++;
            if(i == 0)
            {
                block++;
                break;
            }
        }
        if(states[i][col] != state && states[i][col] != State::NONE)
        {
            block++;
            break;
        }
        if(states[i][col] == State::NONE && states[i-1][col] == state && space == 0)
            space++;
        if(states[i][col] == State::NONE && states[i-1][col] != state)
        {
            if(states[i-1][col] != State::NONE || i==0)
                po++;
            break;
        }
        if(states[i][col] == State::NONE && states[i-1][col] == state && space > 0)
            break;
    }
    for(int i = row + 1; i < boardSize; i++)
    {
        if(states[i][col] == state)//如果是自己人就+1,遇见边界自动添加block
        {
            con++;
            if(i == boardSize-1)
            {
                block++;
                break;
            }
        }
        if(states[i][col] != state && states[i][col] != State::NONE)//遇见对方就添加block
        {
            block++;
            break;
        }
        if(states[i][col] == State::NONE && states[i+1][col] == state && space == 0)//如果当前为空且下一个是自己人,就加一个space,且只有过一个space
            space++;
        if(states[i][col] == State::NONE && states[i+1][col] != state)//如果当前为空,且下一个不是自己人
        {
            if(states[i+1][col] != State::NONE || i == boardSize-1)//下一个不是空的或者是边界就加po
                po++;
            break;
        }
        if(states[i][col] == State::NONE && states[i+1][col] == state && space > 0)
            break;
    }
    if(row == 0 || col == 0 || row == boardSize-1 || col == boardSize-1)
        block++;
    if(block == 0 && con == 3 && po < 2)
        huo3++;
    if(block < 2 && con >= 4)//这是普遍情况
        si4++;
//    if(block == 1 && con == 4)
//        si4++;
    //qDebug() << row << " " << col << " " << con << " " << block << " " << space << " " << po << " " << huo3 << " " << si4;

    con = 1, block = 0, space = 0, po = 0;

    for(int i = col - 1 ; i >= 0; i--)
    {
        if(states[row][i] == state)
        {
            con++;
            if(i == 0)
            {
                block++;
                break;
            }
        }
        if(states[row][i] != state && states[row][i] != State::NONE)
        {
            block++;
            break;
        }
        if(states[row][i] == State::NONE && states[row][i-1] == state && space == 0)
            space++;
        if(states[i][col] != State::NONE && states[row][i-1] != state)
        {
            if(states[row][i-1] != State::NONE || i == 0)
            {
                po++;
            }
            break;
        }
        if(states[row][i] == State::NONE && states[row][i-1] == state && space > 0)
            break;
    }
    for(int i = col + 1 ; i < boardSize; i++)
    {
        if(states[row][i] == state)
        {
            con++;
            if(i == boardSize-1)
            {
                block++;
                break;
            }
        }
        if(states[row][i] != state && states[row][i] != State::NONE)
        {
            block++;
            break;
        }
        if(states[row][i] == State::NONE && states[row][i+1] == state && space == 0)
            space++;
        if(states[row][i] == State::NONE && states[row][i+1] != state)
        {
            if(states[row][i+1] != State::NONE || i == boardSize)
                po++;
            break;
        }
        if(states[row][i] == State::NONE && states[row][i+1] == state && space > 0)
            break;
    }
    if(row == 0 || col == 0 || row == boardSize-1 || col == boardSize-1)
        block++;
    //qDebug() << row << " " << col << " " << con << " " << block << " " << space;
    if(block == 0 && con == 3 && po < 2)
        huo3++;
    if(block < 2 && con >= 4)
        si4++;
//    if(block == 1 && con == 4)
//        si4++;

    con = 1, block = 0, space = 0, po = 0;

    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--)
    {
        if(states[i][j] == state)
        {
            con++;
            if(i == 0 || j == 0)
            {
                block++;
                break;
            }
        }
        if(states[i][j] != state && states[i][j] != State::NONE)
        {
            block++;
            break;
        }
        if(states[i][j] == State::NONE && states[i-1][j-1] == state && space == 0)
            space++;
        if(states[i][j] == State::NONE && states[i-1][j-1] != state)
        {
            if(states[i-1][j-1] != State::NONE || (i == 0 || j == 0))
                po++;
            break;
        }
        if(states[i][j] == State::NONE && states[i-1][j-1] == state && space > 0)
            break;
    }
    for (int i = row + 1, j = col + 1; i < boardSize && j < boardSize; i++, j++)
    {
        if(states[i][j] == state)
        {
            con++;
            if(i == boardSize || j == boardSize)
            {
                block++;
                break;
            }
        }
        if(states[i][j] != state && states[i][j] != State::NONE)
        {
            block++;
            break;
        }
        if(states[i][j] == State::NONE && states[i+1][j+1] == state && space == 0)
            space++;
        if(states[i][j] == State::NONE && states[i+1][j+1] != state)
        {
            if(states[i+1][j+1] != State::NONE || (i == boardSize || j == boardSize))
                po++;
            break;
        }
        if(states[i][j] == State::NONE && states[i+1][j+1] == state && space > 0)
            break;
    }
    if(row == 0 || col == 0 || row == boardSize-1 || col == boardSize-1)
        block++;
    //qDebug() << row << " " << col << " " << con << " " << block << " " << space;
    if(block == 0 && con == 3 && po < 2)
        huo3++;
    if(block < 2 && con >= 4)
        si4++;
//    if(block == 1 && con == 4)
//        si4++;

    con = 1, block = 0, space = 0, po = 0;

    for (int i = row - 1, j = col + 1; i >= 0 && j < boardSize; i--, j++)
    {
        if(states[i][j] == state)
        {
            con++;
            if(i == 0 || j == boardSize)
            {
                block++;
                break;
            }
        }
        if(states[i][j] != state && states[i][j] != State::NONE)
        {
            block++;
            break;
        }
        if(states[i][j] == State::NONE && states[i-1][j+1] == state && space == 0)
            space++;
        if(states[i][j] == State::NONE && states[i-1][j+1] != state)
        {
            if(states[i-1][j+1] != State::NONE || (i == 0 || j == boardSize))
                po++;
            break;
        }
        if(states[i][j] == State::NONE && states[i-1][j+1] == state && space > 0)
            break;
    }

    for (int i = row + 1, j = col - 1; i < boardSize && j >= 0; i++, j--)
    {
        if(states[i][j] == state)
        {
            con++;
            if(i == boardSize || j == 0)
            {
                block++;
                break;
            }
        }
        if(states[i][j] != state && states[i][j] != State::NONE)
        {
            block++;
            break;
        }
        if(states[i][j] == State::NONE && states[i+1][j-1] == state && space == 0)
            space++;
        if(states[i][j] == State::NONE && states[i+1][j-1] != state)
        {
            if(states[i+1][j-1] != State::NONE || (i == boardSize || j == 0))
                po++;
            break;
        }
        if(states[i][j] == State::NONE && states[i+1][j-1] == state && space > 0)
            break;
    }
    if(row == 0 || col == 0 || row == boardSize-1 || col == boardSize-1)
        block++;
    //qDebug() << row << " " << col << " " << con << " " << block << " " << space;
    if(block == 0 && con == 3 && po < 2)
        huo3++;
    if(block < 2 && con >= 4)
        si4++;
//    if(block == 1 && con == 4)
//        si4++;

    if(huo3+si4 >= 2)
        return true;
//    if(huo3 == 2)
//        return true;
//    if(huo3 == 1 && si4 == 1)
//        return true;

    return false;
}

const std::vector<GameBoard::Chess> &GameBoard::getPlayerChesses() const
{
    return playerChesses;
}

const std::vector<GameBoard::Chess> &GameBoard::getEnemyChesses() const
{
    return enemyChesses;
}

void GameBoard::addEnemysChess(int row, int col)
{
    states[row][col] = toState(enemyColor);
    Chess chess(row,col,enemyColor);
    enemyChesses.push_back(chess);
    update();
    inRound = true;
}

inline int GameBoard::toIndex(int pixel) const
{
    return ((pixel - sideMargin + 0.5 * chessSpace) / chessSpace);
}

