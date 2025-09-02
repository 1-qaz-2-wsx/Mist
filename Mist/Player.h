class Player {
private:
    Stats characterStats;
    int max_experience;      // 升级所需经验
    int level;               // 等级
    int current_health;      // 当前生命值
    int current_mana;        // 当前法力值

    //修改属性值
    bool validateAttributeValue(Attribute attribute, int value);  // 验证属性值是否有效
    void updateCurrentValues(Attribute attribute);  // 更新当前值（当上限改变时）
public:
    Player(); // 构造函数
    int getLevel() const; // 获取当前等级
    int getCurrentHealth() const; // 获取当前生命值
    int getCurrentMana() const; // 获取当前法力值
    int getMaxExperience() const; // 获取最大经验值（升级所需）
    const Stats& getStats() const;// 获取角色属性
    void setCurrentHealth(int health); // 设置当前生命值
    void setCurrentMana(int mana); // 设置当前法力值
    void addExperience(int exp); // 增加经验值并检查升级
    void levelUp();// 升级角色
    void heal(int amount);// 恢复生命值
    void restoreMana(int amount); // 恢复法力值
    bool useMana(int amount); // 消耗法力值
    int  makeDemage();//能否造成伤害
    void takeDamage(int damage); // 受到伤害
    void addMoney(int amount); // 增加金钱
    bool spendMoney(int amount);// 消费金钱
    bool isAlive() const;
    void displayStatus() const; // 显示角色状态
    void displayDetailedStats() const;// 显示详细属性
    int getAttribute(Attribute attribute) const;  // 获取属性值
    bool setAttribute(Attribute attribute, int value);//用于调整属性
    bool increaseAttribute(Attribute attribute, int increment); // 增加属性值
};