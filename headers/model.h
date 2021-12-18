#ifndef MODEL_H
#define MODEL_H

#include <QAbstractTableModel>
#include "bot.h"

class Model : public QAbstractTableModel
{
    Q_OBJECT

    enum Roles
    {
        DisplayCell = Qt::UserRole + 1,
        PreviousMove,
        DangerousCell,
        CellNumber
    };

    constexpr static int
    std_board_size = 15,
    GAME_BEGINNING = 0,
    DEAD_END = -3;

    Q_PROPERTY(int game_status READ game_status NOTIFY game_statusChanged)
    Q_PROPERTY(bool vs_bot READ vs_bot WRITE setVs_bot NOTIFY vs_botChanged)

public:
    explicit Model(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void clear();
    Q_INVOKABLE bool resizeBoard(int k);
    Q_INVOKABLE bool botMove();
    Q_INVOKABLE bool undoMove();
    Q_INVOKABLE void setBotPlayer(int player);

    int game_status() const;
    bool vs_bot() const;
    void setVs_bot(bool newVs_bot);

signals:
    void game_statusChanged();
    void vs_botChanged();

private:
    void paintWinner(int& i, int& j, const std::pair<int, int>& direction);
    void paintDangerousCells();

    GameBoard m_game_board;
    std::vector<QModelIndex> m_moves;
    Indices m_dangerous_cells;

    Figures m_figures;
    Bot m_bot;
    int m_game_status;
    bool m_vs_bot;
};

#endif // MODEL_H
