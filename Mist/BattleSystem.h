#pragma once

// 前向声明
class Player;
class Enemy;

class BattleSystem {
public:
    // 返回true表示玩家胜利, false表示玩家失败
    bool startBattle(Player& player, Enemy& enemy);


private:
	// 私有方法和成员变量来支持复杂的战斗逻辑
    // 检查战斗是否正在进行
    //bool isBattleOngoing() const;

    //// 玩家攻击回合
    //void playerAttack();

    //// 怪物攻击回合
    //void monsterAttack();
};