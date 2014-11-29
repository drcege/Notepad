#include "data.h"
#include <string>
#include <list>
#include <iostream>
#include <QFile>
#include <QTextStream>

using namespace std;
//---------------------------构造函数-----------------------------------------//
Data::Data()
{
    firstline = NULL;
    clip = "";
}
//----------------------------------------------------------------------------//

//---------------------------析构函数-----------------------------------------//
Data::~Data()
{
}
//----------------------------------------------------------------------------//

//--------------------------打开文档------------------------------------------//
bool Data::Load(string s)
{
    Block *currentblock = NULL;
    Line *currentline = NULL;
    int i = 0;//记录字符串下标
    int t = 0;//记录当前块内字符数量
    bool change_line = true;//判断是否换行

    if (s[i] != '\0')//非空文档建立头指针
    {
        firstline = new Line;
        firstline->nextBlock = NULL;
        firstline->nextLine = NULL;
        currentline = firstline;
    }

    while (s[i] != '\0')//依次遍历每一个字符
    {
        if (change_line)//进行换行操作，新建一行，并初始化
        {
            change_line = false;
            currentline->nextBlock = new Block;
            currentblock = currentline->nextBlock;
            currentblock->L = 0;
            currentblock->nextBlock = NULL;
        }
        else if (t == 0)//当前块为新的块，新建块，初始化
        {
            Data::create_block(currentblock);
            currentblock = currentblock->nextBlock;
        }

        currentblock->block[currentblock->L] = s[i];//将当前字符存入块内
        currentblock->L++;//块内元素数量加一
        t++;//当前块内元素数量加一

        if (s[i] == '\n')//遇到回车符
        {
            t = 0;//换行致使形成新的块
            change_line = true;//修正换行标志

            if (s[i + 1] != '\0')//若不为文件末尾的回撤符则进行换行操作
            {
                Data::create_line(currentline);//新建一行，初始化
                currentline = currentline->nextLine;//修正指针
            }
        }
        else if (t == 80)//当前块内已满
        {
            t = 0;//形成新块
        }

        i++;//字符串数组下标累加
    }

    return true;
}
//----------------------------------------------------------------------------//

//----------------------------拷贝函数----------------------------------------//
bool Data::Copy(int beg, int end)
{
    int sl = 0, sc = 0, el = 0, ec = 0;//分别存储开始位置行号，列号，结束位置行号列号
    find_pos(beg, sl, sc);//获得开始位置
    find_pos(end - 1, el, ec); //获得结束位置
    Line *currentline = get_line(sl); //获得开始位置的行指针
    clip = "";//剪切板清空
    Block *currentblock = get_block(currentline, sc); //活的开始位置的块指针

    if (sl == el) //复制的内容在同一行
    {
        int t = get_block_pos(sc); //获得开始位置在块内的下标

        for (int i = sc; i <= ec; i++) //逐个拷贝
        {
            clip += currentblock->block[t];
            t++;//判断是否已遍历到最后一个位置

            if (t == 80) //遍历到最后一个位置则遍历下一个块
            {
                t = 0;
                currentblock = currentblock->nextBlock;
            }
        }
    }
    else//要复制的内容跨行
    {
        int length = 0;
        clip += copy_line(currentblock, sc, length); //现将第一行从起始到末尾复制到剪切板中
        currentline = currentline->nextLine; //指针跳到下一行
        currentblock = currentline->nextBlock; //块指针只想新一行的第一个块

        for (int i = sl + 1; i <= (el - 1); i++) //遍历中间整行需要复制的部分
        {
            while (currentblock != NULL) //遍历每个块
            {
                for (int j = 0; j < currentblock->L; j++)
                {
                    clip += currentblock->block[j];    //复制块内所有内容
                }

                currentblock = currentblock->nextBlock; //遍历下一块
            }

            currentline = currentline->nextLine; //遍历下一行
            currentblock = currentline->nextBlock; //修正块指针
        }

        int t = 0; //判断当前块内元素是否全部复制完

        for (int i = 0; i <= ec; i++) //复制结束行的元素
        {
            clip += currentblock->block[t];
            t++;

            if (t == 80) //当前块已复制完，进入下一块
            {
                t = 0;
                currentblock = currentblock->nextBlock;
            }
        }
    }

    return true;
}
//----------------------------------------------------------------------------//

//------------------------查找第pos位置元素所在行列---------------------------//
void Data::find_pos(int pos, int &x, int &y)
{
    Line *currentline = firstline;
    Block *currentblock = firstline->nextBlock;
    int t = 0;//记录已遍历多少个元素
    int before_t = 0;//记录截止到上一行共有多少个元素
    pos++;
    bool flag = true;

    while (flag)
    {
        while (currentblock != NULL)//当前块非空，遍历
        {
            t += currentblock->L;//累加

            if (t >= pos)//已找到
            {
                y = pos - before_t - 1;//得到列号
                flag = false;
                break;
            }

            currentblock = currentblock->nextBlock;//遍历下一块
        }

        if (!flag)
        {
            break;
        }

        if (currentline->nextLine == NULL) //特判，当前位置为文本最后一个位置
        {
            y = t + 1;
            break;
        }

        x++;//遍历下一行
        before_t = t;//修正
        currentline = currentline->nextLine;
        currentblock = currentline->nextBlock;
    }
}
//----------------------------------------------------------------------------//

//---------------------------剪切函数-----------------------------------------//
bool Data::Cut(int beg, int end)
{
    bool flag = Copy(beg, end); //复制要剪切的内容
    int l = 0, c = 0;
    find_pos(beg, l, c); //得到起始位置的行号列号

    for (int i = beg; i < end; i++)
    {
        flag = flag && Delete(l, c); //逐个删除要剪切的元素
    }

    return flag;
}
//----------------------------------------------------------------------------//

//-----------------------------------粘贴函数---------------------------------//
bool Data::Paste(int l, int c)
{
    int i = 0; //记录剪切板内元素下标
    bool flag = true;

    while (clip[i] != '\0') //剪切板非空1
    {
        if (clip[i] == '\n') //剪切板当前元素为回车符
        {
            flag = flag && Enter(l, c); //插入回车
            l++;//更新行号
            c = 0; //更新列号
        }
        else
        {
            string s = "";
            s += clip[i];
            flag = flag && Update(s, l, c); //在当前位置拷贝当前元素
            c++;
        }

        i++;//下标后移
    }

    return flag;
}
//----------------------------------------------------------------------------//

//----------------------------查找函数----------------------------------------//
bool Data::Find(int &pos, string s)
{
    string CLIP = "";//用于存放待查找串
    int l = 0, c = 0;//记录行号列号
    int length = 0;
    int POS = pos;//记录查找位置
    string ::size_type r = 0;//查找函数返回值
    find_pos(pos, l, c);//定为起始位置行列
    Line *currentline = NULL;
    Block *currentblock = NULL;
    currentline = get_line(l);//定位起始位置行指针

    while (currentline != NULL)
    {
        currentblock = get_block_no_create(currentline, c);//定位块指针
        CLIP = "";

        if (currentblock != NULL)
        {
            CLIP = copy_line(currentblock, c, length);    //将这一行之后的部分复制到CLIP中，用于整行查找
        }

        if (CLIP == "")
        {
            r = string::npos;    //空串
        }
        else
        {
            r = CLIP.find(s, 0);    //查找
        }

        if (r != string::npos)//找到，修正pos返回
        {
            POS += int(r);
            pos = POS;
            return true;
        }

        POS = Data::find_start(l);//未找到，修正POS
        c = 0;//从下一行第0列开始
        l++;//下一行
        length = 0;
        currentline = currentline->nextLine;
    }

    //至文本末尾均未找到，则返回开头开始查找，内容通同上部分相同
    c = 0;
    l = 0;
    POS = 0;
    currentline = firstline;

    while (POS < pos)
    {
        currentblock = get_block_no_create(currentline, c);
        CLIP = "";

        if (currentblock != NULL)
        {
            CLIP = copy_line(currentblock, c, length);
        }

        if (CLIP == "")
        {
            r = string::npos;
        }
        else
        {
            r = CLIP.find(s, 0);
        }

        if (r != string::npos)
        {
            POS += int(r);
            pos = POS;
            return true;
        }

        POS = find_start(l);
        c = 0;
        l++;
        length = 0;
        currentline = currentline->nextLine;
    }

    return false;
}
//----------------------------------------------------------------------------//

//-------------------从第0行到第l行元素个数-----------------------------------//
int Data::find_start(int l)
{
    int s = 0;
    Line *currentline = firstline;
    Block *currentblock = currentline->nextBlock;

    for (int i = 0; i <= l; i++)//便利每一行
    {
        currentblock = currentline->nextBlock;

        while (currentblock != NULL)//遍历当前行每一块
        {
            s += currentblock->L;
            currentblock = currentblock->nextBlock;
        }

        currentline = currentline->nextLine;
    }

    return s;
}
//----------------------------------------------------------------------------//

//---------------------------------替换函数-----------------------------------//
bool Data::Replace(int beg, int end, string s)
{
    int l = 0, c = 0;
    bool flag = true;
    find_pos(beg, l, c);//得到替换串起始位置行列号

    for (int i = beg; i < end; i++)//先将代替还部分删除
    {
        flag = flag && Delete(l, c);
    }

    if (s != "")//非空则粘贴
    {
        string CLIP = clip;
        clip = s;
        flag = flag && Paste(l, c);
        clip = CLIP;
    }

    return flag;
}
//----------------------------------------------------------------------------//

//--------------------------删除函数------------------------------------------//
bool Data::Delete(int l, int c)
{
    Block *currentblock = NULL;
    Line *currentline = NULL;
    currentline = get_line(l);//得到当前行
    currentblock = get_block(currentline, c);//得到当前列
    int C = get_block_pos(c);//得到块内位置

    if (currentblock->nextBlock == NULL && currentline->nextLine == NULL && currentblock->L == (C + 1))//删除位置为文本末尾
    {
        if (currentblock->L != 0)
        {
            currentblock->L--;
        }

        return true;
    }

    if (currentblock->nextBlock == NULL && currentblock->L == (C + 1))//删除位置为行末回车符
    {
        Line *temp = currentline->nextLine;//临时指针存放下一行地址
        int length = 0;
        string CLIP = "";

        if (temp->nextBlock != NULL)
        {
            CLIP = copy_line(temp->nextBlock, 0, length);    //复制下一行
        }

        currentblock->L = C; //修正当前块内元素个数
        insert_line(currentblock, CLIP, length);//将下一行插入
        currentline->nextLine = temp->nextLine;//修正指针
    }
    else//正常删除
    {
        int length = 0;
        Block *temp = get_block(currentline, c + 1);//得到后续字符块
        string CLIP = copy_line(temp, c + 1, length); //拷贝后续字符
        delete_block(currentblock->nextBlock);//释放内存
        currentblock->nextBlock = NULL; //初始化
        (currentblock->L) = C; //修正
        insert_line(currentblock, CLIP, length);//重新插入
    }

    return true;
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
bool Data::Backspace(int l, int c)
{
    Line *currentline = NULL;

    if (l == 0 && c == 0)//位置在文本最开始，无效操作
    {
        return true;
    }

    if (c != 0)//正常转化为delete
    {
        return Data::Delete(l, c - 1);
    }

    //行首backspace，需要合并两行
    l--;
    currentline = get_line(l);
    int C = 0;

    for (Block* temp = currentline->nextBlock; temp != NULL; temp = temp->nextBlock)
    {
        C += temp->L;
    }

    return Data::Delete(l, C - 1);
}
//----------------------------------------------------------------------------//

//-------------------------------回车函数-------------------------------------//
bool Data::Enter(int l, int c)
{
    Block *currentblock = NULL;
    Line *currentline = NULL;
    int length = 0;
    currentline = get_line(l);
    currentblock = get_block(currentline, c);
    string CLIP = copy_line(currentblock, c, length);//复制回车键后续内容
    delete_block(currentblock->nextBlock);//释放内存
    currentblock->nextBlock = NULL;
    currentblock->L = get_block_pos(c) + 1; //修正块内元素数量
    currentblock->block[currentblock->L - 1] = '\n';//添加回车符
    Line *temp = currentline->nextLine;
    currentline->nextLine = NULL;
    create_line(currentline);
    currentline = currentline->nextLine;
    currentline->nextLine = temp;
    currentblock = currentline->nextBlock;

    if (currentblock == NULL && length != 0)//文本末尾回车，需要建立新的行
    {
        currentline->nextBlock = new Block;
        currentblock = currentline->nextBlock;
        currentblock->L = 0;
        currentblock->nextBlock = NULL;
    }

    insert_line(currentblock, CLIP, length);//插入后续内容
    return true;
}
//----------------------------------------------------------------------------//

//---------------------------------更新函数-----------------------------------//
bool Data::Update(string s, int l, int c)
{
    int length = 0;
    string CLIP = "";
    Block *currentblock = NULL;
    Line *currentline = NULL;
    currentline = get_line(l);
    currentblock = get_block(currentline, c);
    CLIP = copy_line(currentblock, c, length);//保存更新位置的后续内容
    delete_block(currentblock->nextBlock);//释放内存
    currentblock->nextBlock = NULL;
    currentblock->L = get_block_pos(c);//修正数量
    CLIP = s + CLIP;//连接内容

    if (length == 0)//后续内容为空
    {
        CLIP = s;
    }

    length++;
    insert_line(currentblock, CLIP, length);//将全部内容插入
    return true;
}
//----------------------------------------------------------------------------//

//------------------------------------释放函数--------------------------------//
bool Data::Clear()
{
    delete_line(firstline);
    firstline = NULL;
    clip = "";
    return true;
}
//----------------------------------------------------------------------------//

//-----------------------------------释放行-----------------------------------//
void Data::delete_line(Line *currentline)
{
    if (currentline != NULL)//回溯法释放
    {
        delete_block(currentline->nextBlock);
        delete_line(currentline->nextLine);
        delete(currentline);
    }
}
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
string Data::Clip()                          //返回剪切板数据
{
    return clip;
}
//----------------------------------------------------------------------------//

//--------------------------------生成文本函数--------------------------------//
string Data::Text()
{
    string CLIP;

    //依次遍历每行没列，生成一个字符串
    for (Line *currentline = firstline; currentline != NULL; currentline = currentline->nextLine)
    {
        for (Block *currentblock = currentline->nextBlock; currentblock != NULL; currentblock = currentblock->nextBlock)
        {
            for (int i = 0; i < currentblock->L; ++i)
            {
                CLIP += currentblock->block[i];
            }
        }
    }

    return CLIP;
}
//----------------------------------------------------------------------------//

//---------------------------新建行-------------------------------------------//
void Data::create_line(Line * &currentline)
{
    currentline->nextLine = new Line;
    currentline->nextLine->nextBlock = NULL;
    currentline->nextLine->nextLine = NULL;
}
//----------------------------------------------------------------------------//

//---------------------------得到第l行的指针----------------------------------//
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
//----------------------------------------------------------------------------//

//------------------------复制整行--------------------------------------------//
string Data::copy_line(Block *currentblock, int beg, int &le)
{
    string CLIP = "";
    beg = get_block_pos(beg);//得到块内位置

    for (int i = beg; i < currentblock->L; i++)//首先复制当前块内内容
    {
        le++;
        CLIP += currentblock->block[i];
    }

    currentblock = currentblock->nextBlock;

    while (currentblock != NULL)//复制后续整块的内容
    {
        for (int i = 0; i < currentblock->L; i++)
        {
            le++;
            CLIP += currentblock->block[i];
        }

        currentblock = currentblock->nextBlock;
    }

    return CLIP;
}
//----------------------------------------------------------------------------//

//------------------------在当前块后插入CLIP----------------------------------//
void Data::insert_line(Block *currentblock, string CLIP, int length)
{
    int i = 0;

    while (i < length)//逐个插入
    {
        if (currentblock->L == 80)//当前块已满，需新建
        {
            if (currentblock->nextBlock == NULL)
            {
                create_block(currentblock);
            }

            currentblock = currentblock->nextBlock;
        }

        currentblock->block[currentblock->L] = CLIP[i];//插入
        currentblock->L++;//修正
        i++;
    }
}
//----------------------------------------------------------------------------//

//--------------------------建立块链表----------------------------------------//
void Data::create_block(Block * &currentblock)
{
    currentblock->nextBlock = new Block;
    currentblock->nextBlock->L = 0;
    currentblock->nextBlock->nextBlock = NULL;
}
//----------------------------------------------------------------------------//

//-------------------------得到当前块地址-------------------------------------//
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

    for (int i = 79; i < c; i += 80)//逐个块遍历
    {
        if (currentblock->nextBlock == NULL)
        {
            create_block(currentblock);
        }

        currentblock = currentblock->nextBlock;
    }

    return currentblock;
}
//----------------------------------------------------------------------------//

//----------------得到当前块地址，并且不进行新建操作--------------------------//
Block* Data::get_block_no_create(Line *currentline, int c)
{
    Block *currentblock = currentline->nextBlock;

    if (currentblock == NULL)
    {
        return NULL;
    }

    for (int i = 79; i < c; i += 80)
    {
        if (currentblock->nextBlock == NULL)
        {
            return NULL;
        }

        currentblock = currentblock->nextBlock;
    }

    return currentblock;
}
//----------------------------------------------------------------------------//

//--------------------------回溯释放一行中的全部块----------------------------//
void Data::delete_block(Block *currentblock)
{
    if (currentblock != NULL)
    {
        delete_block(currentblock->nextBlock);
        delete(currentblock);
    }
}
//----------------------------------------------------------------------------//

//-------------------------------得到块内位置---------------------------------//
int Data::get_block_pos(int x)
{
    while (x >= 80)
    {
        x -= 80;
    }

    return x;
}
//----------------------------------------------------------------------------//
