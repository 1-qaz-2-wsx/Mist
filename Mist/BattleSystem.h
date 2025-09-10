#pragma once

class Player;
class Enemy;

class BattleSystem {
public:
    bool startBattle(Player& player, Enemy& enemy);

private:
    void playerTurn(Player& player, Enemy& enemy);
    void enemyTurn(Player& player, Enemy& enemy);
};