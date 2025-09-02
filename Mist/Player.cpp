#include"Player'h"

// ���캯��
Player::Player() : level(1), max_experience(100),
current_health(characterStats[max_health]),
current_mana(characterStats[max_mana]) {}

// ��ȡ��ǰ�ȼ�
int Player::getLevel() const {
    return level;
}

// ��ȡ��ǰ����ֵ
int Player::getCurrentHealth() const {
    return current_health;
}

// ��ȡ��ǰ����ֵ
int Player::getCurrentMana() const {
    return current_mana;
}

// ��ȡ�����ֵ���������裩
int Player::getMaxExperience() const {
    return max_experience;
}

// ��ȡ��ɫ����
const Stats& Player::getStats() const {
    return characterStats;
}

// ���õ�ǰ����ֵ
void Player::setCurrentHealth(int health) {
    current_health = health;
    if (current_health < 0) current_health = 0;
    if (current_health > characterStats[max_health])
        current_health = characterStats[max_health];
}

// ���õ�ǰ����ֵ
void Player::setCurrentMana(int mana) {
    current_mana = mana;
    if (current_mana < 0) current_mana = 0;
    if (current_mana > characterStats[max_mana])
        current_mana = characterStats[max_mana];
}

// ���Ӿ���ֵ���������
void Player::addExperience(int exp) {
    Player::characterStats[experience_points] += exp;

    // ����Ƿ�����
    while (characterStats[experience_points] >= max_experience) {
        levelUp();
    }
}

// ������ɫ
void Player::levelUp() {
    level++;
    characterStats[experience_points] -= max_experience;
    max_experience = static_cast<int>(max_experience * 1.5); // ÿ���������辭������50%

    // ������������
    characterStats[max_health] += 20;
    characterStats[max_mana] += 5;
    characterStats[attack] += 5;
    characterStats[agility] += 2;
    characterStats[accuracy] += 5;
    characterStats[defence] += 3;

    // �ָ�����ֵ�ͷ���ֵ
    current_health = characterStats[max_health];
    current_mana = characterStats[max_mana];

    cout << "��ϲ����ɫ������ " << level << " ����" << endl;
}

// �ָ�����ֵ
void Player::heal(int amount) {
    current_health += amount;
    if (current_health > characterStats[max_health]) {
        current_health = characterStats[max_health];
    }
}

// �ָ�����ֵ
void Player::restoreMana(int amount) {
    current_mana += amount;
    if (current_mana > characterStats[max_mana]) {
        current_mana = characterStats[max_mana];
    }
}

// ���ķ���ֵ
bool Player::useMana(int amount) {
    if (current_mana >= amount) {
        current_mana -= amount;
        return true;
    }
    return false;
}

// �ܵ��˺�
void Player::takeDamage(int damage) {
    // ����ʵ���˺������Ƿ�����
    srand(time(0));
    int a = rand() % 100 + 1;  //�������һ��1-100��������С��miss�����ܳɹ�
    int actualDamage = damage - characterStats[defence];

    if (a <= characterStats[agility]) {
        cout << "��������ι���" << endl;
        return;
    }

    if (actualDamage < 1) actualDamage = 1;    //�˺�С��1ʱ��Ϊ1

    current_health -= actualDamage;
    printf("�ܵ�%d���˺�\n", actualDamage);
    if (isAlive()) cout << "��һ���" << current_health << "��Ѫ��" << endl;
    if (current_health < 0) current_health = 0;
}

int Player::makeDemage() {
    srand(time(0));
    int random = rand() % 100 + 1;
    int choice = 0;
    int demage = 1;
    while (choice == 0) {
        cout << "��ѡ����ļ��ܣ�1.��ͨ���� 2.ս�������ķ���ֵ��ɸ�ǿ�󹥻��� 3.������";
        cin >> choice;
        if (choice == 1) {
            demage = characterStats[attack];
        }
        else if (choice == 2) {
            if (useMana(2)) demage = characterStats[attack] * 1.5;
            else cout << "��ķ���ֵ���㣡" << endl;
        }
        else if (choice == 3) {
            //..............
        }
        else {
            choice = 0;
            cout << "Ŀǰû�и�������ˣ�" << endl;
        }
    }
    return demage;
}


// ���ӽ�Ǯ
void Player::addMoney(int amount) {
    characterStats[money] += amount;
}

// ���ѽ�Ǯ
bool Player::spendMoney(int amount) {
    if (characterStats[money] >= amount) {
        characterStats[money] -= amount;
        return true;
    }
    return false;
}

// ����Ƿ���
bool Player::isAlive() const {
    return current_health > 0;
}

// ��ʾ��ɫ״̬
void Player::displayStatus() const {
    cout << "=== ��ɫ״̬ ===" << endl;
    cout << "�ȼ�: " << level << endl;
    cout << "����ֵ: " << current_health << "/" << characterStats[max_health] << endl;
    cout << "����ֵ: " << current_mana << "/" << characterStats[max_mana] << endl;
    cout << "����ֵ: " << characterStats[experience_points] << "/" << max_experience << endl;
    cout << "��Ǯ: " << characterStats[money] << endl;
    cout << "=================" << endl << endl;
}

// ��ʾ��ϸ����
void Player::displayDetailedStats() const {
    cout << "=== ��ϸ���� ===" << endl;
    characterStats.display();
    cout << "================" << endl << endl;
}

// ��֤����ֵ�Ƿ���Ч
bool Player::validateAttributeValue(Attribute attribute, int value) {
    if (value < 0) {
        return false; // �������Զ�����Ϊ����
    }

    // ����ض����ԵĶ�����֤
    switch (attribute) {
    case max_health:
        return value >= 1; // ����ֵ����Ϊ1
    case max_mana:
        return value >= 0; // ����ֵ����Ϊ0
    case attack:
    case defence:
        return value >= 0;
    case agility:
    case accuracy:
        return value >= 1; // ���ݺ�׼ȷ������Ϊ1
    case money:
    case experience_points:
        return value >= 0;
    default:
        return true;
    }
}

// ���µ�ǰֵ�������޸ı�ʱ��
void Player::updateCurrentValues(Attribute attribute) {
    switch (attribute) {
    case max_health:
        // ȷ����ǰ����ֵ�������µ�����
        if (current_health > characterStats[max_health]) {
            current_health = characterStats[max_health];
        }
        break;
    case max_mana:
        // ȷ����ǰ����ֵ�������µ�����
        if (current_mana > characterStats[max_mana]) {
            current_mana = characterStats[max_mana];
        }
        break;
    default:
        break;
    }
}

// �޸�����ֵ����Ҫ��鲻Ϊ��/��Ч��
bool Player::setAttribute(Attribute attribute, int value) {
    // ��������Ƿ���Ч
    if (attribute < 0 || attribute >= NUMATTRIBUTES) {
        cout << "������Ч���������ͣ�" << endl;
        return false;
    }

    // ���ֵ�Ƿ���Ч�����ݲ�ͬ�����Կ����в�ͬ����֤����
    if (!validateAttributeValue(attribute, value)) {
        cout << "������Ч������ֵ��" << endl;
        return false;
    }

    // ��������ֵ
    characterStats[attribute] = value;

    // ����޸ĵ�������ֵ����ֵ���ޣ���Ҫͬ�����µ�ǰֵ
    updateCurrentValues(attribute);

    return true;
}

// ��ȡ����ֵ
int Player::getAttribute(Attribute attribute) const {
    if (attribute < 0 || attribute >= NUMATTRIBUTES) {
        cout << "������Ч���������ͣ�" << endl;
        return -1;
    }
    return characterStats[attribute];
}

// ��������ֵ
bool Player::increaseAttribute(Attribute attribute, int increment) {
    if (attribute < 0 || attribute >= NUMATTRIBUTES) {
        cout << "������Ч���������ͣ�" << endl;
        return false;
    }

    if (increment <= 0) {
        cout << "������������Ϊ������" << endl;
        return false;
    }

    characterStats[attribute] += increment;
    updateCurrentValues(attribute);
    return true;
}
