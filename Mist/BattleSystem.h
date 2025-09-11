#pragma once

class Player;
class Enemy;
// 用于表示战斗的最终结果
enum class BattleResult {
    WIN,
    LOSS,
    FLED
};

class BattleSystem {
public:
    BattleResult startBattle(Player& player, Enemy& enemy);

private:
    void playerTurn(Player& player, Enemy& enemy);
    void enemyTurn(Player& player, Enemy& enemy);
};