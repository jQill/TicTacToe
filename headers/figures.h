#ifndef FIGURES_H
#define FIGURES_H

#include <vector>
#include <map>

enum CellStatus
{
    XWIN = -2,
    OWIN = -1,
    EMPTY = 0,
    O = 1,
    X = 2
};


struct Cell
{
    Cell(const int number = 0) :number(number)
    {}

    operator CellStatus() const { return static_cast<CellStatus>(getCellStatus()); }

    bool operator==(const CellStatus& other) const { return getCellStatus() == static_cast<int>(other); }
    bool operator!=(const CellStatus& other) const { return getCellStatus() != static_cast<int>(other); }

    bool operator==(const Cell& other) const { return getCellStatus() == other.getCellStatus(); }
    bool operator!=(const Cell& other) const { return getCellStatus() != other.getCellStatus(); }

    void setWinnerStatus() { number = -std::abs(number); }

    int getCellNumber() const { return std::abs(number); }

    int getCellStatus() const
    {
        if (number > 0) return number % 2 + 1;
        if (number < 0) return number % 2 - 1;

        return 0;
    }

private:
    int number;
};


using GameBoard = std::vector<std::vector<Cell>>;
using Index = std::pair<int, int>;
using Indices = std::map<Index, int>;


class Figures
{
    enum FigureComponent
    {
        Em, // Empty
        Me, // Me
        EA  // Empty, Adversary, Null
    };

    static constexpr int
    SINGLE1 = 1,
    SINGLE2 = 2,
    CLOSED_THREES = 5,
    DOUBLES = 5,
    WINING  = 100;

    using FigureBegin = std::pair<int, Index>;
    using Figure = std::vector<FigureComponent>;
    using Positions = std::vector<int>;

    const Figure open_four = { EA, Em, Me, Me, Me, Me, Em, EA };

    const Figure closed_four1 = { EA, Me, Me, Me, Me, Em, EA };
    const Figure closed_four2 = { EA, Me, Me, Me, Em, Me, EA };
    const Figure closed_four3 = { EA, Me, Me, Em, Me, Me, EA };

    const Figure open_three1 = { EA, Em, Me, Me, Me, Em, Em, EA };
    const Figure open_three2 = { EA, Em, Me, Me, Em, Me, Em, EA };

    const Figure closed_three1 = { EA, Me, Me, Me, Em, Em, EA };
    const Figure closed_three2 = { EA, Me, Me, Em, Me, Em, EA };
    const Figure closed_three3 = { EA, Em, Me, Me, Me, Em, EA };
    const Figure closed_three4 = { EA, Me, Em, Me, Em, Me, EA };

    const Figure open_double1 = { Em, Em, Me, Me, Em, Em };
    const Figure open_double2 = { Em, Em, Me, Em, Me, Em, Em };
    const Figure open_double3 = { Em, Me, Em, Em, Me, Em };

    const Figure closed_double1 = { EA, Me, Me, Em, Em };

    const Figure single1 = { Em, Me };
    const Figure single2 = { Em, Me, Em };

    const Positions open_four_pos = { 6 };

    const Positions closed_four1_pos = { 5 };
    const Positions closed_four2_pos = { 4 };
    const Positions closed_four3_pos = { 3 };

    const Positions open_three1_pos = { 5 };
    const Positions open_three2_pos = { 4 };

    const Positions closed_three1_pos = { 5,4 };
    const Positions closed_three2_pos = { 3 };
    const Positions closed_three3_pos = { 1,5 };
    const Positions closed_three4_pos = { 2,4 };

    const Positions open_double1_pos = { 1,4 };
    const Positions open_double2_pos = { 1,3,5 };
    const Positions open_double3_pos = { 2,3 };

    const Positions closed_double1_pos = { 4 };

    const Positions single1_pos = { 0 };
    const Positions single2_pos = { 0,2 };

    const std::vector<int> ki = { -1,1,0,0,-1,1,-1,1 };
    const std::vector<int> kj = { 0,0,-1,1,1,1,-1,-1 };

public:

    Figures(const GameBoard& board)
        :m_board(board), m_begin(std::make_pair(m_board.size(), m_board.size())), m_end(std::make_pair(0, 0))
    {}

    Indices getMoves()
    {
        return std::move(m_potential_moves);
    }

    Index getMove()
    {
        if (m_potential_moves.empty())
            return std::make_pair(m_board.size() / 2, m_board.size() / 2);

        Index res = maxPosistionRate()->first;
        m_potential_moves.clear();
        return res;
    }

    void clearBorders()
    {
        m_begin = std::make_pair(m_board.size(), m_board.size());
        m_end = std::make_pair(0, 0);
    }

    void setBorders(const Index& index)
    {
        if (index.first < m_begin.first)
            m_begin.first = index.first;
        if (index.second < m_begin.second)
            m_begin.second = index.second;

        if (index.first > m_end.first)
            m_end.first = index.first;
        if (index.second > m_end.second)
            m_end.second = index.second;
    }

    std::pair<int, int> five(int& i, int& j, const CellStatus& me)const { return hasFive(i, j, me); }

    bool openFour(const CellStatus& me) { return findFigure(me, open_four, open_four_pos); }
    bool openFours(const CellStatus& me) { return findFigures(me, open_four, open_four_pos); }

    bool closedFour1(const CellStatus& me) { return findFigure(me, closed_four1, closed_four1_pos); }
    bool closedFour2(const CellStatus& me) { return findFigure(me, closed_four2, closed_four2_pos); }
    bool closedFour3(const CellStatus& me) { return findFigure(me, closed_four3, closed_four3_pos); }

    bool openThree1(const CellStatus& me) { return findFigures(me, open_three1, open_three1_pos); }
    bool openThree2(const CellStatus& me) { return findFigures(me, open_three2, open_three2_pos); }

    bool closedThree1(const CellStatus& me, int rate = 0) { return findFigures(me, closed_three1, closed_three1_pos, CLOSED_THREES + rate); }
    bool closedThree2(const CellStatus& me, int rate = 0) { return findFigures(me, closed_three2, closed_three2_pos, CLOSED_THREES + rate); }
    bool closedThree3(const CellStatus& me, int rate = 0) { return findSymmetricFigures(me, closed_three3, closed_three3_pos, CLOSED_THREES + rate); }
    bool closedThree4(const CellStatus& me, int rate = 0) { return findSymmetricFigures(me, closed_three4, closed_three4_pos, CLOSED_THREES + rate); }

    bool openDouble1(const CellStatus& me, int rate = 0) { return findSymmetricFigures(me, open_double1, open_double1_pos, DOUBLES + rate); }
    bool openDouble2(const CellStatus& me, int rate = 0) { return findSymmetricFigures(me, open_double2, open_double2_pos, DOUBLES + rate); }
    bool openDouble3(const CellStatus& me, int rate = 0) { return findSymmetricFigures(me, open_double3, open_double3_pos, DOUBLES + rate); }

    bool closedDouble1(const CellStatus& me, int rate = 0) { return  findFigures(me, closed_double1, closed_double1_pos, SINGLE1 + rate); }

    bool singles1(const CellStatus& me, int rate = 0) { return findFigures(me, single1, single1_pos, SINGLE1 + rate); }
    bool singles2(const CellStatus& me, int rate = 0) { return findSymmetricFigures(me, single2, single2_pos, SINGLE2 + rate); }

    bool secondSimpleFigure(const CellStatus& me, const CellStatus& enemy)
    {
        int i, j, cnt_me = 0, cnt_enemy = 0, i_enemy, j_enemy, board_center = m_board.size() / 2;
        for (i = 0; i < m_board.size(); ++i)
            for (j = 0; j < m_board.size(); ++j)
            {
                if (m_board.at(i).at(j) == enemy)
                {
                    i_enemy = i;
                    j_enemy = j;
                    ++cnt_enemy;
                }
                if (m_board.at(i).at(j) == me)
                    ++cnt_me;
            }

        Index index;

        if (me == O && cnt_enemy == 1 && cnt_me == 0 &&
            m_board.at(board_center).at(board_center) == EMPTY)
            index = std::make_pair(board_center, board_center);
        else if (cnt_enemy != 1 || cnt_me != 1)
            return false;

        else if (i_enemy == board_center && j_enemy == board_center + 1)
            index = std::make_pair(board_center - 1, board_center + 1);
        else if (i_enemy == board_center && j_enemy == board_center - 1)
            index = std::make_pair(board_center - 1, board_center - 1);
        else if (j_enemy == board_center && i_enemy == board_center - 1)
            index = std::make_pair(board_center - 1, board_center - 1);
        else if (j_enemy == board_center && i_enemy == board_center + 1)
            index = std::make_pair(board_center + 1, board_center - 1);

        else if (i_enemy < board_center && j_enemy <= board_center)
            index = std::make_pair(board_center + 1, board_center - 1);
        else if (i_enemy <= board_center && j_enemy > board_center)
            index = std::make_pair(board_center - 1, board_center - 1);
        else if (i_enemy > board_center && j_enemy >= board_center)
            index = std::make_pair(board_center - 1, board_center + 1);
        else if (i_enemy >= board_center && j_enemy < board_center)
            index = std::make_pair(board_center + 1, board_center + 1);

         ++m_potential_moves[index];

        return true;
    }

private:
    bool findFigure(const CellStatus& me, const Figure& figure, const Positions& positions)
    {
        int i = m_begin.first - 4, j = m_begin.second - 4, direction = 0;
        return findFigureImpl(i, j, direction, me, figure, positions);
    }

    bool findSymmetricFigures(const CellStatus& me, const Figure& figure, const Positions& positions, const int rate)
    {
        int i = m_begin.first - 4, j = m_begin.second - 4, direction = 0;
        std::map<std::pair<Index, Index>, bool> prev_figures;
        Index move, figure_tail;

        while(hasFigure(i, j, direction, me, figure))
        {
            figure_tail = std::make_pair(i + ki.at(direction) * (figure.size() - 1),
                                         j + kj.at(direction) * (figure.size() - 1));
            if (prev_figures.find(std::make_pair(figure_tail, std::make_pair(i, j))) == prev_figures.end())
            {
                prev_figures[std::make_pair(std::make_pair(i, j), figure_tail)] = true;
                for (int k = 0; k < positions.size(); ++k)
                {
                    move.first = i + ki.at(direction) * positions.at(k);
                    move.second = j + kj.at(direction) * positions.at(k);
                    m_potential_moves[move] += rate;
                }
            }

            if (!indicesValid(i, ++j))
            {
                ++i;
                j = 0;
            }
        }

        return prev_figures.size();
    }

    bool findFigures(const CellStatus& me, const Figure& figure, const Positions& positions, const int rate = WINING)
    {
        int i =  m_begin.first - 4, j = m_begin.second - 4, direction = 0, finded = 0;
        while (findFigureImpl(i, j, direction, me, figure, positions, rate))
        {
            ++finded;
            if (!indicesValid(i, ++j))
            {
                ++i;
                j = 0;
            }
        }

        return finded;
    }

    bool findFigureImpl(int& i, int& j, int& direction, const CellStatus& me, const Figure& figure,
                        const Positions& positions, const int rate = WINING)
    {
        if (hasFigure(i, j, direction, me, figure))
        {
            Index move;
            for (int k = 0; k < positions.size(); ++k)
            {
                move.first = i + ki.at(direction) * positions.at(k);
                move.second = j + kj.at(direction) * positions.at(k);
                m_potential_moves[move] += rate;
            }
            return true;
        }
        return false;
    }

    bool hasFigure(int& i, int& j, int& direction, const CellStatus& me, const Figure& figure) const
    {
        int ii = 0, jj = 0, cnt = 0;
        for (; i < m_end.first + 4; ++i, j = m_begin.second - 4)
            for (; j < m_end.second + 4; ++j)
                for (direction = 0; direction < ki.size(); ++direction)
                {
                    for (int k = 0; k < figure.size(); ++k)
                    {
                        ii = i + k * ki.at(direction);
                        jj = j + k * kj.at(direction);
                        if (indicesValid(ii, jj))
                        {
                            if ((figure.at(k) == Me && m_board.at(ii).at(jj) == me) ||
                                (figure.at(k) == EA && m_board.at(ii).at(jj) != me) ||
                                (figure.at(k) == Em && m_board.at(ii).at(jj) == EMPTY))
                                ++cnt;
                        }
                        else if (figure.at(k) == EA)
                            ++cnt;
                    }
                    if (cnt == figure.size())
                        return true;
                    cnt = 0;
                }

        return false;
    }

    std::pair<int, int> hasFive(int& i, int& j, const CellStatus& me) const
    {
        int ii = i, jj = j;
        for (int d = 0; d < ki.size(); d += 2)
        {
            robot(i, j, ki.at(d), kj.at(d), me);
            if (5 == robot(i -= ki.at(d), j -= kj.at(d), -ki.at(d), -kj.at(d), me))
                return std::make_pair(ki.at(d), kj.at(d));
            i = ii;
            j = jj;
        }

        return std::make_pair(0, 0);
    }

    int robot(int& i, int& j, int direction_i, int direction_j, const CellStatus& me) const
    {
        int cnt = 0;
        while (indicesValid(i, j) && m_board.at(i).at(j) == me)
        {
            i += direction_i;
            j += direction_j;
            ++cnt;
        }

        return cnt;
    }

    Indices::const_iterator maxPosistionRate() const
    {
        auto max = m_potential_moves.cbegin();
        for (auto it = ++m_potential_moves.cbegin(); it != m_potential_moves.cend(); ++it)
            if (max->second < it->second)
                max = it;

        return max;
    }

    bool indicesValid(int i, int j) const
    {
        return i < m_board.size() && j >= 0 && i >= 0 && j < m_board.size();
    }

    const GameBoard& m_board;
    Indices m_potential_moves;

    Index m_begin;
    Index m_end;
};

#endif // FIGURES_H
