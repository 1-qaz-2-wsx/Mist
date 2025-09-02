class Player {
private:
    Stats characterStats;
    int max_experience;      // �������辭��
    int level;               // �ȼ�
    int current_health;      // ��ǰ����ֵ
    int current_mana;        // ��ǰ����ֵ

    //�޸�����ֵ
    bool validateAttributeValue(Attribute attribute, int value);  // ��֤����ֵ�Ƿ���Ч
    void updateCurrentValues(Attribute attribute);  // ���µ�ǰֵ�������޸ı�ʱ��
public:
    Player(); // ���캯��
    int getLevel() const; // ��ȡ��ǰ�ȼ�
    int getCurrentHealth() const; // ��ȡ��ǰ����ֵ
    int getCurrentMana() const; // ��ȡ��ǰ����ֵ
    int getMaxExperience() const; // ��ȡ�����ֵ���������裩
    const Stats& getStats() const;// ��ȡ��ɫ����
    void setCurrentHealth(int health); // ���õ�ǰ����ֵ
    void setCurrentMana(int mana); // ���õ�ǰ����ֵ
    void addExperience(int exp); // ���Ӿ���ֵ���������
    void levelUp();// ������ɫ
    void heal(int amount);// �ָ�����ֵ
    void restoreMana(int amount); // �ָ�����ֵ
    bool useMana(int amount); // ���ķ���ֵ
    int  makeDemage();//�ܷ�����˺�
    void takeDamage(int damage); // �ܵ��˺�
    void addMoney(int amount); // ���ӽ�Ǯ
    bool spendMoney(int amount);// ���ѽ�Ǯ
    bool isAlive() const;
    void displayStatus() const; // ��ʾ��ɫ״̬
    void displayDetailedStats() const;// ��ʾ��ϸ����
    int getAttribute(Attribute attribute) const;  // ��ȡ����ֵ
    bool setAttribute(Attribute attribute, int value);//���ڵ�������
    bool increaseAttribute(Attribute attribute, int increment); // ��������ֵ
};