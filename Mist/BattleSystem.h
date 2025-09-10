#pragma once

// 前向声明
class Player;
class Enemy;

class BattleSystem {
public:
    // 返回true表示玩家胜利, false表示玩家失败
    bool startBattle(Player& player, Enemy& enemy);
};