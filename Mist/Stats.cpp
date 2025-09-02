#include"Stats.h"

Stats::Stats() {
    values[0] = 100;
    values[1] = 10;
    values[2] = 50;
    values[3] = 5;
    values[4] = 80;
    values[5] = 50;
    values[6] = 0;
    values[7] = 20;
}

int& Stats::operator[](Attribute attr) {
    return values[attr];
}

const int& Stats::operator[](Attribute attr) const {
    return values[attr];
}

void Stats::display() const {
    const char* attributeNames[] = {
        "生命值上限", "法力值上限", "攻击力", "敏捷", "速度", "金钱", "经验", "防御"
    };

    for (int i = 0; i < NUMATTRIBUTES; i++) {
        cout << attributeNames[i] << ": " << values[i] << endl;
    }
}