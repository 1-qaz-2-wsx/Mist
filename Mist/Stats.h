class Stats {
private:
    int values[NUMATTRIBUTES];  // �洢��������ֵ������
public:
    Stats(); //Ĭ�Ϲ��캯�� 
    int& operator[](Attribute attr); // ����[]����� - �����޸�����ֵ
    const int& operator[](Attribute attr) const; // const�汾 - ���ڶ�ȡ����ֵ
    void display() const; // ��ʾ��������
};