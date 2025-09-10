#include "Enemy.h"

Enemy::Enemy(std::string name, int health, int attack, int defense, Item loot)
    : name(name), health(health), attack(attack), defense(defense), loot(loot) {}

bool Enemy::isAlive() const {
    return health > 0;
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}