class Stats {
private:
    int values[NUMATTRIBUTES];  // 存储所有属性值的数组
public:
    Stats(); //默认构造函数 
    int& operator[](Attribute attr); // 重载[]运算符 - 用于修改属性值
    const int& operator[](Attribute attr) const; // const版本 - 用于读取属性值
    void display() const; // 显示所有属性
};