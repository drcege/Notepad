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
    bool Load(string s);                    //读取文件，将s内容写入数据结构
    bool Copy(int beg, int end);            //从文件开头的字符数，左闭右开
    bool Cut(int beg, int end);             //从文件开头的字符数，左闭右开
    bool Paste(int l, int c);               //行数和列数
    bool Find(int& pos, string s);          //从文件开头的字符数,开始查找字符串，引用返回查找到位置
    bool Replace(int beg, int end, string s);
    bool Delete(int l, int c);              //在l，c处删除后一个字符
    bool Backspace(int l, int c);           //在l，c处删除前一个字符
    bool Enter(int l, int c);               //在l，c处截断形成新行
    bool Update(string s, int l, int c);    //在l，c处插入字符 (单字符string)
    bool Clear();                           //清空所有数据
    string Clip();                          //返回剪切板数据
    string Text();                          //返回所有数据

private:
    Line *firstline;
    string clip;       //剪切板

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
