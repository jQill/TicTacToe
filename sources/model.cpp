#include "model.h"

Model::Model(QObject *parent) :QAbstractTableModel(parent),
    m_game_board(std_board_size, std::vector<Cell>(std_board_size, EMPTY)),
    m_figures(m_game_board),
    m_bot(O, m_figures),
    m_game_status(GAME_BEGINNING),
    m_vs_bot(true)
{}

int Model::rowCount(const QModelIndex &parent) const
{
    return m_game_board.size();
}

int Model::columnCount(const QModelIndex &parent) const
{
    return m_game_board.size();
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
    case DisplayCell: return m_game_board.at(index.row()).at(index.column()).getCellStatus();
    case PreviousMove: return m_game_board.at(index.row()).at(index.column()) != EMPTY &&
                !m_moves.empty() && m_moves.back() == index;
    case CellNumber: return m_game_board.at(index.row()).at(index.column()).getCellNumber();
    case DangerousCell: return !m_dangerous_cells.empty() &&
                m_dangerous_cells.find(std::make_pair(index.row(), index.column())) != m_dangerous_cells.cend();
    default:
        return QVariant();
    }
}

bool Model::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (m_game_status == XWIN || m_game_status == OWIN ||
        m_game_status == DEAD_END)
        return false;

    int i = index.row();
    int j = index.column();
    if (!index.isValid() || m_game_board.at(i).at(j) != EMPTY)
        return false;

    if (role == Qt::EditRole)
    {
        m_figures.setBorders(std::make_pair(i, j));
        m_game_board.at(i).at(j) = m_moves.size() + 1;
        m_moves.emplace_back(index);
        emit dataChanged(index, index);

        int prev = m_moves.size() - 2;
        if (prev >= 0)
            emit dataChanged(m_moves.at(prev), m_moves.at(prev), { PreviousMove });

        std::pair<int, int> direction = m_figures.five(i, j, m_game_board.at(i).at(j));
        if (direction.first != 0 || direction.second != 0)
            paintWinner(i, j, direction);
        else if (m_moves.size() == m_game_board.size() * m_game_board.size())
            m_game_status = DEAD_END;
        else
        {
            if (m_game_board.at(i).at(j) == m_bot.getBotPlayer())
                paintDangerousCells();
            m_game_status = m_game_board.at(i).at(j).getCellStatus() % 2 + 1;
        }

        emit game_statusChanged();
        return true;
    }

    return false;
}

QHash<int, QByteArray> Model::roleNames() const
{
    QHash<int, QByteArray> names;
    names[DisplayCell] = "cell";
    names[PreviousMove] = "color";
    names[CellNumber] = "number";
    names[DangerousCell] = "dangerous";

    return names;
}

void Model::clear()
{
    if (m_game_status == GAME_BEGINNING)
        return;

    for (auto it = m_moves.cbegin(); it != m_moves.cend(); ++it)
    {
        m_game_board.at(it->row()).at(it->column()) = EMPTY;
        emit dataChanged(*it, *it, { DisplayCell });
    }
    emit dataChanged(m_moves.back(), m_moves.back(), { PreviousMove });
    m_moves.clear();

    Indices dangerous_cells = m_dangerous_cells;
    m_dangerous_cells.clear();
    QModelIndex index;
    for (auto it = dangerous_cells.cbegin(); it != dangerous_cells.cend(); ++it)
    {
        index = createIndex(it->first.first, it->first.second);
        emit dataChanged(index, index, { DangerousCell });
    }

    m_figures.clearBorders();

    m_game_status = GAME_BEGINNING;
    emit game_statusChanged();
}

bool Model::resizeBoard(int k)
{
    int new_size = m_game_board.size() + k;
    if ((k != 2 && k != -2) || new_size < 15 || new_size > 45)
        return false;

    beginResetModel();
    m_game_board = GameBoard(new_size, std::vector<Cell>(new_size, EMPTY));
    endResetModel();

    return true;
}

bool Model::botMove()
{
    Index bot_move = m_bot.makeMove();
    QModelIndex index = createIndex(bot_move.first, bot_move.second);
    return setData(index, QVariant());
}

bool Model::undoMove()
{
    if (m_moves.empty())
        return false;

    QModelIndex index = m_moves.back();
    if (index.isValid())
    {
        int i = index.row();
        int j = index.column();
        if (m_game_board.at(i).at(j) == EMPTY)
            return false;

        m_game_status = m_game_board.at(i).at(j);
        m_game_board.at(i).at(j) = EMPTY;
        emit dataChanged(index, index);
        m_moves.pop_back();
        if (!m_moves.empty())
            emit dataChanged(m_moves.back(), m_moves.back(), { PreviousMove });
        else m_game_status = GAME_BEGINNING;

        paintDangerousCells();

        emit game_statusChanged();
        return true;
    }

    return false;
}

void Model::setBotPlayer(int player)
{
    m_bot.swapPlayers(static_cast<CellStatus>(player));
}

void Model::paintWinner(int &i, int &j, const std::pair<int, int> &direction)
{
    QModelIndex index;

    m_game_status = m_game_status == X ? XWIN : OWIN;

    for (int t = 0; t < 5; ++t)
    {
        i += direction.first;
        j += direction.second;
        m_game_board.at(i).at(j).setWinnerStatus();
        index = createIndex(i, j);
        emit dataChanged(index, index, { DisplayCell });
    }

    paintDangerousCells();
}

void Model::paintDangerousCells()
{
    if (!m_vs_bot)
        return;

    Indices tmp = m_dangerous_cells;
    QModelIndex tmp_index;
    m_figures.closedFour1(m_bot.getBotPlayer());
    m_figures.closedFour2(m_bot.getBotPlayer());
    m_figures.closedFour3(m_bot.getBotPlayer());
    m_figures.openFours(m_bot.getBotPlayer());
    m_dangerous_cells = m_figures.getMoves();

    for (auto it = tmp.cbegin(); it != tmp.cend(); ++it)
    {
        tmp_index = createIndex(it->first.first, it->first.second);
        emit dataChanged(tmp_index, tmp_index, { DangerousCell });
    }

    for (auto it = m_dangerous_cells.cbegin(); it != m_dangerous_cells.cend(); ++it)
    {
        tmp_index = createIndex(it->first.first, it->first.second);
        emit dataChanged(tmp_index, tmp_index, { DangerousCell });
    }
}

int Model::game_status() const
{
    return m_game_status;
}

bool Model::vs_bot() const
{
    return m_vs_bot;
}

void Model::setVs_bot(bool newVs_bot)
{
    if (m_vs_bot == newVs_bot)
        return;

    m_vs_bot = newVs_bot;
    emit vs_botChanged();
}
