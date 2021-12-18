#ifndef BOT_H
#define BOT_H

#include "figures.h"

class Bot
{
public:
    explicit Bot(const CellStatus& player, Figures& figures)
        :m_bot(player), m_enemy(player == X ? O : X), m_figures(figures)
    {}

    Index makeMove()
    {
        winAttack() ||
        loseAttack() ||
        emergencyDefence() ||
        emergencyAttack() ||
        attack() +
        defence() +
        weakMove();

        return m_figures.getMove();
    }

    void swapPlayers(const CellStatus player)
    {
        m_bot = player;
        m_enemy = m_bot == X ? O : X;
    }

    CellStatus getBotPlayer() const
    {
        return m_bot;
    }

private:

    bool winAttack()
    {
        return m_figures.openFour(m_bot) ||
               m_figures.closedFour1(m_bot) ||
               m_figures.closedFour2(m_bot) ||
               m_figures.closedFour3(m_bot);
    }

    bool loseAttack()
    {
        return m_figures.openFour(m_enemy);
    }

    bool emergencyDefence()
    {
        return m_figures.closedFour1(m_enemy) ||
               m_figures.closedFour2(m_enemy) ||
               m_figures.closedFour3(m_enemy);
    }

    bool emergencyAttack()
    {
        return m_figures.openThree1(m_bot) ||
               m_figures.openThree2(m_bot);
    }

    bool attack()
    {
        return
               m_figures.closedThree1(m_bot, 2) +
               m_figures.closedThree2(m_bot, 2) +
               m_figures.closedThree3(m_bot, 2) +
               m_figures.closedThree4(m_bot, 2) +
               m_figures.openDouble1(m_bot, 2) +
               m_figures.openDouble2(m_bot, 2) +
               m_figures.openDouble3(m_bot, 2);
    }

    bool defence()
    {
        return m_figures.openThree1(m_enemy) +
               m_figures.openThree2(m_enemy) +
               m_figures.closedThree1(m_enemy) +
               m_figures.closedThree2(m_enemy) +
               m_figures.closedThree3(m_enemy) +
               m_figures.closedThree4(m_enemy) +
               m_figures.openDouble1(m_enemy) +
               m_figures.openDouble2(m_enemy) +
               m_figures.openDouble3(m_enemy);

    }

    bool weakMove()
    {
        return m_figures.secondSimpleFigure(m_bot, m_enemy) ||
              (m_figures.closedDouble1(m_bot, 1) +
               m_figures.singles1(m_bot, 1) +
               m_figures.singles2(m_bot, 1) ||
              (m_figures.closedDouble1(m_enemy)) ||
               m_figures.singles1(m_enemy) +
               m_figures.singles2(m_enemy));
    }

    CellStatus m_bot;
    CellStatus m_enemy;
    Figures& m_figures;
};

#endif // BOT_H
