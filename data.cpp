#include "data.h"
#include <string>
#include <list>
#include <iostream>
#include <QFile>
#include <QTextStream>

using namespace std;

Data::Data()
{
    firstline=NULL;
}

Data::~Data()
{

}

bool Data::Load(string s)
{
    Block *currentblock = NULL;
    Line *currentline = NULL;
    int i = 0;
    int t = 0;
    bool change_line = true;

    if (s[i] != '\0')
    {
        firstline = new Line;
        firstline->nextBlock = NULL;
        firstline->nextLine = NULL;
        currentline = firstline;
    }

    while (s[i] != '\0')
    {
        if (change_line)
        {
            change_line = false;
            currentline->nextBlock = new Block;
            currentblock = currentline->nextBlock;
            currentblock->L = 0;
            currentblock->nextBlock = NULL;
        }
        else if (t == 0)
        {
            Data::create_block(currentblock);
            currentblock = currentblock->nextBlock;
        }

        currentblock->block[currentblock->L] = s[i];
        currentblock->L++;
        t++;

        if (s[i] == '\n')
        {
            t = 0;
            change_line = true;

            if (s[i + 1] != '\0')
            {
                Data::create_line(currentline);
                currentline = currentline->nextLine;
            }
        }
        else if (t == 80)
        {
            t = 0;
        }
        i++;
    }

    return true;
}

bool Data::Copy(int beg, int end)            //从文件开头的字符数，左闭右开
{
    return true;
}

bool Data::Cut(int beg, int end)             //从文件开头的字符数，左闭右开
{
    return true;
}

bool Data::Paste(int l, int c)               //行数和列数
{
    return true;
}

bool Data::Find(int& pos, string src, string dest)    //从l，c处开始查找字符串，引用返回查找到位置
{
    return true;
}

bool Data::Replace(int beg,int end, string s)
{
    return true;
}

bool Data::Delete(int l, int c)              //在l，c处删除后一个字符
{
    return true;
}

bool Data::Backspace(int l, int c)           //在l，c处删除前一个字符
{
    return true;
}

bool Data::Enter(int l, int c)               //在l，c处截断形成新行
{
    return true;
}

bool Data::Clear()                           //清空所有数据
{
    return true;
}

string Data::Clip()                          //返回剪切板数据
{
    return clip;
}

string Data::Text()
{
    string CLIP;

    for(Line *currentline = firstline; currentline!=NULL; currentline=currentline->nextLine)
    {
        for(Block *currentblock=currentline->nextBlock; currentblock!=NULL; currentblock=currentblock->nextBlock)
        {
            for(int i=0; i<currentblock->L; ++i)
            {
                CLIP+=currentblock->block[i];
            }
        }
    }
    return CLIP;
}


void Data::create_line(Line * &currentline)
{
    currentline->nextLine = new Line;
    currentline->nextLine->nextBlock = NULL;
    currentline->nextLine->nextLine = NULL;
}

Line* Data::get_line(int l)
{
    if (firstline == NULL)
    {
        firstline = new Line;
        firstline->nextLine = NULL;
        firstline->nextBlock = NULL;
    }

    Line *currentline = firstline;

    for (int i = 0; i < l; i++)
    {
        if (currentline->nextLine == NULL)
        {
            create_line(currentline);
        }

        currentline = currentline->nextLine;
    }

    return currentline;
}

void Data::create_block(Block * &currentblock)
{
    currentblock->nextBlock = new Block;
    currentblock->nextBlock->L = 0;
    currentblock->nextBlock->nextBlock = NULL;
}

Block* Data::get_block(Line *currentline, int c)
{
    Block *currentblock = currentline->nextBlock;

    if (currentline->nextBlock == NULL)
    {
        currentline->nextBlock = new Block;
        currentblock = currentline->nextBlock;
        currentblock->L = 0;
        currentblock->nextBlock = NULL;
    }

    for (int i = 79; i < c; i += 80)
    {
        if (currentblock->nextBlock == NULL)
        {
            create_block(currentblock);
        }

        currentblock = currentblock->nextBlock;
    }

    return currentblock;
}

int Data::get_block_pos(int x)
{
    while (x >= 80)x -= 80;

    return x;
}

string Data::copy_line(Block *currentblock, int beg, int &le)
{
    string CLIP = "";
    beg = get_block_pos(beg);

    for (int i = beg; i < currentblock->L; i++)
    {
        le++;
        CLIP += currentblock->block[i];
    }

    currentblock = currentblock->nextBlock;

    while (currentblock != NULL)
    {
        for (int i = 0; i < currentblock->L; i++)
        {
            le++;
            CLIP += currentblock->block[i];
        }
    }
    //second_statusLabel->setText(tr(CLIP));
    //ui->textEdit->setText(CLIP);
    return CLIP;
}

void Data::insert_line(Block *currentblock,string CLIP,int length)
{

    int i = 0;

    while (i < length)
    {

        currentblock->block[currentblock->L] = CLIP[i];

        currentblock->L++;

        if (currentblock->L == 80)
        {
            if (currentblock->nextBlock == NULL)
            {
                create_block(currentblock);
            }

            currentblock = currentblock->nextBlock;
        }

        i++;
    }

}

void Data::delete_block(Block *currentblock)
{
    if (currentblock != NULL)
    {
        delete_block(currentblock->nextBlock);
        delete(currentblock);
    }
}
bool Data::Update(string s, int l, int c)
{
    int length = 0;
    string CLIP="";
    Block *currentblock = NULL;
    Line *currentline = NULL;
    currentline = get_line(l);
    currentblock = get_block(currentline, c);
    CLIP = copy_line(currentblock, c, length);
    delete_block(currentblock->nextBlock);
    currentblock->L = get_block_pos(c);

    CLIP = s + CLIP;
    if(length==0)CLIP=s;
    length++;
    insert_line(currentblock, CLIP,length);

    return true;
}
