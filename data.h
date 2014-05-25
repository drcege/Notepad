#ifndef DATA_H
#define DATA_H
#include <string>
#define length 80
using namespace std;

/*
typedef struct Block{
    char block[80];
    struct Block* nextBlock;
}Block;
typedef struct Line{
    struct Line* nextLine;
    struct Block* nextBlock;
}Line;
*/


struct Block
{
       char block[length];
       struct Block *nextBlock;
       int l;
};
struct Line
{
       struct Line *nextLine;
       struct Block *nextBlock;
};
Line *firstline=NULL;


class Data
{
public:
    Data();
    ~Data();
    bool Load(string s);                    //��ȡ�ļ�����s����д�����ݽṹ
    bool Copy(int beg, int end);            //���ļ���ͷ���ַ���������ҿ�
    bool Cut(int beg, int end);             //���ļ���ͷ���ַ���������ҿ�
    bool Paste(int l, int c);               //����������
//    bool Find(int& l, int& c, string s);    //��l��c����ʼ�����ַ��������÷��ز��ҵ�λ��
//    bool Replace(string );
    bool Delete(int l, int c);              //��l��c��ɾ����һ���ַ�
    bool Backspace(int l, int r);           //��l��c��ɾ��ǰһ���ַ�
    bool Enter(int l, int c);               //��l��c���ض��γ�����
    bool Update(string s, int l, int c);    //��l��c�������ַ� (���ַ�string)
    bool Clear();                           //�����������
    string Clip();                          //���ؼ��а�����
    string Text();                          //������������

    
private:
//    Line* line;
    string clip;       //���а�

};

#endif // DATA_H
