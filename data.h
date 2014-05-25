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
    bool Load(string s);                    //读取文件，将s内容写入数据结构
    bool Copy(int beg, int end);            //从文件开头的字符数，左闭右开
    bool Cut(int beg, int end);             //从文件开头的字符数，左闭右开
    bool Paste(int l, int c);               //行数和列数
//    bool Find(int& l, int& c, string s);    //从l，c处开始查找字符串，引用返回查找到位置
//    bool Replace(string );
    bool Delete(int l, int c);              //在l，c处删除后一个字符
    bool Backspace(int l, int r);           //在l，c处删除前一个字符
    bool Enter(int l, int c);               //在l，c处截断形成新行
    bool Update(string s, int l, int c);    //在l，c处插入字符 (单字符string)
    bool Clear();                           //清空所有数据
    string Clip();                          //返回剪切板数据
    string Text();                          //返回所有数据

    
private:
//    Line* line;
    string clip;       //剪切板

};

#endif // DATA_H
