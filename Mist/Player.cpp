#include"Player'h"

// 构造函数
Player::Player() : level(1), max_experience(100),
current_health(characterStats[max_health]),
current_mana(characterStats[max_mana]) {}

// 获取当前等级
int Player::getLevel() const {
    return level;
}

// 获取当前生命值
int Player::getCurrentHealth() const {
    return current_health;
}

// 获取当前法力值
int Player::getCurrentMana() const {
    return current_mana;
}

// 获取最大经验值（升级所需）
int Player::getMaxExperience() const {
    return max_experience;
}

// 获取角色属性
const Stats& Player::getStats() const {
    return characterStats;
}

// 设置当前生命值
void Player::setCurrentHealth(int health) {
    current_health = health;
    if (current_health < 0) current_health = 0;
    if (current_health > characterStats[max_health])
        current_health = characterStats[max_health];
}

// 设置当前法力值
void Player::setCurrentMana(int mana) {
    current_mana = mana;
    if (current_mana < 0) current_mana = 0;
    if (current_mana > characterStats[max_mana])
        current_mana = characterStats[max_mana];
}

// 增加经验值并检查升级
void Player::addExperience(int exp) {
    Player::characterStats[experience_points] += exp;

    // 检查是否升级
    while (characterStats[experience_points] >= max_experience) {
        levelUp();
    }
}

// 升级角色
void Player::levelUp() {
    level++;
    characterStats[experience_points] -= max_experience;
    max_experience = static_cast<int>(max_experience * 1.5); // 每次升级所需经验增加50%

    // 提升各项属性
    characterStats[max_health] += 20;
    characterStats[max_mana] += 5;
    characterStats[attack] += 5;
    characterStats[agility] += 2;
    characterStats[accuracy] += 5;
    characterStats[defence] += 3;

    // 恢复生命值和法力值
    current_health = characterStats[max_health];
    current_mana = characterStats[max_mana];

    cout << "恭喜！角色升级到 " << level << " 级！" << endl;
}

// 恢复生命值
void Player::heal(int amount) {
    current_health += amount;
    if (current_health > characterStats[max_health]) {
        current_health = characterStats[max_health];
    }
}

// 恢复法力值
void Player::restoreMana(int amount) {
    current_mana += amount;
    if (current_mana > characterStats[max_mana]) {
        current_mana = characterStats[max_mana];
    }
}

// 消耗法力值
bool Player::useMana(int amount) {
    if (current_mana >= amount) {
        current_mana -= amount;
        return true;
    }
    return false;
}

// 受到伤害
void Player::takeDamage(int damage) {
    // 计算实际伤害（考虑防御）
    srand(time(0));
    int a = rand() % 100 + 1;  //随机生成一个1-100的正数，小于miss代表躲避成功
    int actualDamage = damage - characterStats[defence];

    if (a <= characterStats[agility]) {
        cout << "你躲避了这次攻击" << endl;
        return;
    }

    if (actualDamage < 1) actualDamage = 1;    //伤害小于1时设为1

    current_health -= actualDamage;
    printf("受到%d的伤害\n", actualDamage);
    if (isAlive()) cout << "玩家还有" << current_health << "点血量" << endl;
    if (current_health < 0) current_health = 0;
}

int Player::makeDemage() {
    srand(time(0));
    int random = rand() % 100 + 1;
    int choice = 0;
    int demage = 1;
    while (choice == 0) {
        cout << "请选择你的技能：1.普通攻击 2.战技（消耗法力值造成更强大攻击） 3.背包：";
        cin >> choice;
        if (choice == 1) {
            demage = characterStats[attack];
        }
        else if (choice == 2) {
            if (useMana(2)) demage = characterStats[attack] * 1.5;
            else cout << "你的法力值不足！" << endl;
        }
        else if (choice == 3) {
            //..............
        }
        else {
            choice = 0;
            cout << "目前没有更多操作了！" << endl;
        }
    }
    return demage;
}


// 增加金钱
void Player::addMoney(int amount) {
    characterStats[money] += amount;
}

// 消费金钱
bool Player::spendMoney(int amount) {
    if (characterStats[money] >= amount) {
        characterStats[money] -= amount;
        return true;
    }
    return false;
}

// 检查是否存活
bool Player::isAlive() const {
    return current_health > 0;
}

// 显示角色状态
void Player::displayStatus() const {
    cout << "=== 角色状态 ===" << endl;
    cout << "等级: " << level << endl;
    cout << "生命值: " << current_health << "/" << characterStats[max_health] << endl;
    cout << "法力值: " << current_mana << "/" << characterStats[max_mana] << endl;
    cout << "经验值: " << characterStats[experience_points] << "/" << max_experience << endl;
    cout << "金钱: " << characterStats[money] << endl;
    cout << "=================" << endl << endl;
}

// 显示详细属性
void Player::displayDetailedStats() const {
    cout << "=== 详细属性 ===" << endl;
    characterStats.display();
    cout << "================" << endl << endl;
}

// 验证属性值是否有效
bool Player::validateAttributeValue(Attribute attribute, int value) {
    if (value < 0) {
        return false; // 所有属性都不能为负数
    }

    // 针对特定属性的额外验证
    switch (attribute) {
    case max_health:
        return value >= 1; // 生命值至少为1
    case max_mana:
        return value >= 0; // 法力值可以为0
    case attack:
    case defence:
        return value >= 0;
    case agility:
    case accuracy:
        return value >= 1; // 敏捷和准确度至少为1
    case money:
    case experience_points:
        return value >= 0;
    default:
        return true;
    }
}

// 更新当前值（当上限改变时）
void Player::updateCurrentValues(Attribute attribute) {
    switch (attribute) {
    case max_health:
        // 确保当前生命值不超过新的上限
        if (current_health > characterStats[max_health]) {
            current_health = characterStats[max_health];
        }
        break;
    case max_mana:
        // 确保当前法力值不超过新的上限
        if (current_mana > characterStats[max_mana]) {
            current_mana = characterStats[max_mana];
        }
        break;
    default:
        break;
    }
}

// 修改属性值（需要检查不为空/有效）
bool Player::setAttribute(Attribute attribute, int value) {
    // 检查属性是否有效
    if (attribute < 0 || attribute >= NUMATTRIBUTES) {
        cout << "错误：无效的属性类型！" << endl;
        return false;
    }

    // 检查值是否有效（根据不同的属性可能有不同的验证规则）
    if (!validateAttributeValue(attribute, value)) {
        cout << "错误：无效的属性值！" << endl;
        return false;
    }

    // 设置属性值
    characterStats[attribute] = value;

    // 如果修改的是生命值或法力值上限，需要同步更新当前值
    updateCurrentValues(attribute);

    return true;
}

// 获取属性值
int Player::getAttribute(Attribute attribute) const {
    if (attribute < 0 || attribute >= NUMATTRIBUTES) {
        cout << "错误：无效的属性类型！" << endl;
        return -1;
    }
    return characterStats[attribute];
}

// 增加属性值
bool Player::increaseAttribute(Attribute attribute, int increment) {
    if (attribute < 0 || attribute >= NUMATTRIBUTES) {
        cout << "错误：无效的属性类型！" << endl;
        return false;
    }

    if (increment <= 0) {
        cout << "错误：增量必须为正数！" << endl;
        return false;
    }

    characterStats[attribute] += increment;
    updateCurrentValues(attribute);
    return true;
}
