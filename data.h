#ifndef DATA_H
#define DATA_H
#include <string>
#include <QFile>
#include <QTextStream>

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
    bool Find(int& pos, string s);          //���ļ���ͷ���ַ���,��ʼ�����ַ��������÷��ز��ҵ�λ��
    bool Replace(int beg, int end, string s);
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

    int find_start(int l);
    int get_block_pos(int x);
    char next_char(int l, int c);
    void delete_line(Line *currentline);
    void create_block(Block * &currentblock);
    void delete_block(Block *currentblock);
    void create_line(Line * &currentline);
    void insert_line(Block *currentblock,  string CLIP, int length);
    void find_pos(int pos, int &x, int &y);
    Line* get_line(int l);
    Block* get_block(Line *currentline, int c);
    Block* get_block_no_create(Line *currentline, int c);
    string copy_line(Block *currentblock, int beg, int &l);
};

#endif // DATA_H
