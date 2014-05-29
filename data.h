#ifndef DATA_H
#define DATA_H
#include <string>
using namespace std;

struct Block
{
       char block[80];
       struct Block *nextBlock;
       int L;
};
struct Line
{
       struct Line *nextLine;
       struct Block *nextBlock;
};



class Data
{
public:
    Data();
    ~Data();
    bool Load(string s);                    //��ȡ�ļ�����s����д�����ݽṹ
    bool Copy(int beg, int end);            //���ļ���ͷ���ַ���������ҿ�
    bool Cut(int beg, int end);             //���ļ���ͷ���ַ���������ҿ�
    bool Paste(int l, int c);               //����������
    bool Find(int& pos, string src, string dest);    //��l��c����ʼ�����ַ��������÷��ز��ҵ�λ��
    bool Replace(int beg,int end, string s);
    bool Delete(int l, int c);              //��l��c��ɾ����һ���ַ�
    bool Backspace(int l, int c);           //��l��c��ɾ��ǰһ���ַ�
    bool Enter(int l, int c);               //��l��c���ض��γ�����
    bool Update(string s, int l, int c);    //��l��c�������ַ� (���ַ�string)
    bool Clear();                           //�����������
    string Clip();                          //���ؼ��а�����
    string Text();                          //������������

private:
    Line *firstline;
    string clip;       //���а�

    void DELETE_block(Block *currentblock);
    void INSERT(int t,bool flag,int i,string s,Line* &currentline,Block* &currentblock);
    int get_pos_block(int c);
    Line * get_line(int l);
    Block * get_block(Block *currentblock,int c);
    int get_pos(int l,int c);
    void DELETE(Line *deleteline,Block *deleteblock);
    void DELETE_INSERT(int C,int l,int c,int add,Line * &currentline,Block * &currentblock);


};

#endif // DATA_H
