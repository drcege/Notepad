#include "data.h"
#include <string>
#include <list>
#include <iostream>
#include <QFile>
#include <QTextStream>

using namespace std;

Data::Data()
{
    firstline = NULL;
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

bool Data::Copy(int beg, int end)
{/*
    QFile f("debug.txt");
    f.open(QFile::Append | QFile::Text);
    QTextStream debug(&f);
*/
    int sl = 0, sc = 0, el = 0, ec = 0;
    find_pos(beg, sl, sc);
    find_pos(end-1, el, ec);
    Line *currentline=get_line(sl);
    clip = "";
    Block *currentblock=get_block(currentline,sc);
    if(sl==el)
    {
        int t=get_block_pos(sc);
        for(int i=sc;i<=ec;i++)
        {
            clip+=currentblock->block[t];
            t++;
            if(t==80)
            {
                t=0;
                currentblock=currentblock->nextBlock;
            }
        }
    }
    else
    {
        int length=0;
        clip+=copy_line(currentblock,sc,length);
        currentline=currentline->nextLine;
        currentblock=currentline->nextBlock;
        for(int i=sl+1;i<=(el-1);i++)
        {
            while(currentblock!=NULL)
            {
                for(int j=0;j<currentblock->L;j++)clip+=currentblock->block[j];
                currentblock=currentblock->nextBlock;
            }
            currentline=currentline->nextLine;
            currentblock=currentline->nextBlock;
        }
        int t=0;
        for(int i=0;i<=ec;i++)
        {
            clip+=currentblock->block[t];
            t++;
            if(t==80)
            {
                t=0;
                currentblock=currentblock->nextBlock;
            }
        }
    }

    //debug << beg << ' ' << end << ' ' << sl << ' ' << sc << ' ' << el << ' ' << ec <<' '<< '\n';
    //debug<<clip[0]<<clip[1]<<clip[2]<<clip[3]<<clip[4]<<clip[5]<<clip[6]<<clip[7]<<clip[8];
    //f.close();

    return true;
}

void Data::find_pos(int pos, int &x, int &y)
{
    Line *currentline = firstline;
    Block *currentblock = firstline->nextBlock;
    int t = 0;
    int before_t = 0;
    pos++;
    bool flag = true;

    while (flag)
    {
        while (currentblock != NULL)
        {
            t += currentblock->L;

            if (t >= pos)
            {
                y = pos - before_t - 1;
                flag = false;
                break;
            }

            currentblock = currentblock->nextBlock;
        }
        if(!flag)break;
        x++;
        before_t = t;
        currentline = currentline->nextLine;
        currentblock = currentline->nextBlock;
    }
}

bool Data::Cut(int beg,int end)
{
    bool flag=Copy(beg,end);
    int l=0,c=0;
    find_pos(beg,l,c);
    for(int i=beg;i<end;i++)
    {
        flag=flag&&Delete(l,c);
    }
    return flag;
}

bool Data::Paste(int l,int c)
{
    int i=0;
    bool flag=true;
    while(clip[i]!='\0')
    {
        if(clip[i]=='\n')
        {
            flag=flag&&Enter(l,c);
            l++;
            c=0;
        }
        else
        {
            string s="";
            s+=clip[i];
            flag=flag&&Update(s,l,c);
            c++;
        }
        i++;
    }
    return flag;
}


bool Data::Find(int &pos, string s)
{
    string CLIP = "";
    int l = 0, c = 0;
    int length = 0;
    int POS = pos;
    string ::size_type r = 0;
    find_pos(pos, l, c);
    Line *currentline = NULL;
    Block *currentblock = NULL;
    currentline = get_line(l);

    while (currentline != NULL)
    {
        currentblock = get_block(currentline, c);
        CLIP = "";
        CLIP = copy_line(currentblock, c, length);
        r = CLIP.find(s, 0);

        if (r != string::npos)
        {
            POS += int(r);
            pos = POS;
            return true;
        }

        POS = Data::find_start(l);
        c = 0;
        l++;
        length = 0;
        currentline = currentline->nextLine;
    }

    c = 0;
    l = 0;
    POS = 0;
    currentline = firstline;

    while (POS<pos)
    {
        currentblock = get_block(currentline, c);
        CLIP = "";
        CLIP = copy_line(currentblock, c, length);
        r = CLIP.find(s, 0);

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

int Data::find_start(int l)
{
    int s = 0;
    Line *currentline = firstline;
    Block *currentblock = currentline->nextBlock;

    for (int i = 0; i <= l; i++)
    {
        currentblock = currentline->nextBlock;

        while (currentblock != NULL)
        {
            s += currentblock->L;
            currentblock = currentblock->nextBlock;
        }

        currentline = currentline->nextLine;
    }

    return s;
}

bool Data::Replace(int beg, int end, string s)
{
    int l = 0, c = 0;
    bool flag = true;
    find_pos(beg, l, c);

    for (int i = beg; i < end; i++)
    {
        flag = flag && Delete(l, c);
    }

    if (s != "")
    {
        //find_pos(beg,l,c);
        string CLIP = clip;
        clip = s;
        flag = flag && Paste(l, c);
        clip = CLIP;
    }

    return flag;
}



bool Data::Delete(int l, int c)
{
    /*if(Data::END(l,c))
    {
        return true;
    }*/
    Block *currentblock = NULL;
    Line *currentline = NULL;
    currentline = get_line(l);
    currentblock = get_block(currentline, c);
    int C = get_block_pos(c);
    if (currentblock->nextBlock == NULL && currentline->nextLine == NULL && currentblock->L == (C + 1))
    {
        if(currentblock->L!=0)currentblock->L--;
        return true;
    }
    if (currentblock->nextBlock == NULL && currentblock->L == (C + 1))
    {
        Line *temp = currentline->nextLine;
        int length = 0;
        string CLIP = "";
        if(temp->nextBlock!=NULL)CLIP=copy_line(temp->nextBlock, 0, length);
        currentblock->L=C;
        insert_line(currentblock, CLIP, length);
        currentline->nextLine = temp->nextLine;
        //delete(temp);
    }
    else
    {
        int length = 0;
        Block *temp = get_block(currentline, c +1 );
        string CLIP = copy_line(temp, c +1, length);
        delete_block(currentblock->nextBlock);
        currentblock->nextBlock=NULL;
        (currentblock->L)=C;
        insert_line(currentblock, CLIP, length);
    }

    return true;
}

bool Data::Backspace(int l, int c)
{
    //Block *currentblock = NULL;
    Line *currentline = NULL;

    if (l == 0 && c == 0)
    {
        return true;
    }

    if (c != 0)
    {
        return Data::Delete(l, c - 1);
    }

    l--;
    currentline = get_line(l);
    int C = 0;

    for (Block* temp = currentline->nextBlock; temp != NULL; temp = temp->nextBlock)
    {
        C += temp->L;
    }

    return Data::Delete(l, C-1);
}


bool Data::Enter(int l, int c)
{
/*    QFile f("debug.txt");
    f.open(QFile::Append | QFile::Text);
    QTextStream debug(&f);
    debug << l << ' ' << c << '\n';
  */
    Block *currentblock = NULL;
    Line *currentline = NULL;
    int length = 0;
    currentline = get_line(l);
    currentblock = get_block(currentline, c);
    string CLIP = copy_line(currentblock, c, length);
    delete_block(currentblock->nextBlock);
    currentblock->nextBlock=NULL;
    currentblock->L = get_block_pos(c) +1;
    currentblock->block[currentblock->L - 1] = '\n';
    Line *temp = currentline->nextLine;
    currentline->nextLine = NULL;
    create_line(currentline);
    currentline = currentline->nextLine;
    currentline->nextLine = temp;
    currentblock = currentline->nextBlock;

    if (currentblock == NULL && length != 0)
    {
        currentline->nextBlock = new Block;
        currentblock = currentline->nextBlock;
        currentblock->L = 0;
        currentblock->nextBlock = NULL;
    }

    insert_line(currentblock, CLIP, length);
    return true;
}

bool Data::Update(string s, int l, int c)
{
    /*
        QFile f("debug.txt");
        f.open(QFile::Append | QFile::Text);
        QTextStream debug(&f);

        debug << l << ' ' << c << '\n';
    */
    int length = 0;
    string CLIP = "";
    Block *currentblock = NULL;
    Line *currentline = NULL;
    currentline = get_line(l);
    currentblock = get_block(currentline, c);
    CLIP = copy_line(currentblock, c, length);
    delete_block(currentblock->nextBlock);
    currentblock->nextBlock=NULL;
    currentblock->L = get_block_pos(c);
    CLIP = s + CLIP;

    if (length == 0)
    {
        CLIP = s;
    }

    length++;
    insert_line(currentblock, CLIP, length);
    //f.close();
    return true;
}

bool Data::Clear()
{
    delete_line(firstline);
    firstline = NULL;
    clip = "";
    return true;
}

void Data::delete_line(Line *currentline)
{
    if (currentline != NULL)
    {
        delete_block(currentline->nextBlock);
        delete_line(currentline->nextLine);
        delete(currentline);
    }
}

string Data::Clip()                          //返回剪切板数据
{
    return clip;
}

string Data::Text()
{
    string CLIP;

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

bool Data::Renew()
{
    int max_block = 79;
    bool flag = true;
    Line *currentline = firstline;
    Block *currentblock = NULL;

    while (currentline != NULL)
    {
        currentblock = currentline->nextBlock;
        int t = 0;
        int c = 0;
        int l = 0;

        while (currentblock != NULL)
        {
            for (int i = 0; i < currentblock->L; i++)
            {
                t++;

                if (t == max_block)
                {
                    //下一位空，下一位是符号，下一位是回车
                    if (currentblock->block[i] == '\n')
                    {
                        continue;
                    }
                    else if (next_char(l, c) == '\n')
                    {
                        break;
                    }
                    else
                    {
                        flag = flag && Enter(l, c + 1);
                    }

                    t = 0;
                }

                c++;
            }

            currentblock = currentblock->nextBlock;
        }

        l++;
        currentline = currentline->nextLine;
    }

    return flag;
}

char Data::next_char(int l, int c)
{
    c++;
    int C = get_block_pos(c);
    Line *currentline = get_line(l);
    Block *currentblock = get_block(currentline, c);
    return currentblock->block[C];
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
        currentblock=currentblock->nextBlock;
    }

    //second_statusLabel->setText(tr(CLIP));
    //ui->textEdit->setText(CLIP);
    return CLIP;
}

void Data::insert_line(Block *currentblock, string CLIP, int length)
{
    int i = 0;

    while (i < length)
    {
        if (currentblock->L == 80)
        {
            if (currentblock->nextBlock == NULL)
            {
                create_block(currentblock);
            }

            currentblock = currentblock->nextBlock;
        }
        currentblock->block[currentblock->L] = CLIP[i];
        currentblock->L++;
        i++;
    }
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

void Data::delete_block(Block *currentblock)
{
    if (currentblock != NULL)
    {
        delete_block(currentblock->nextBlock);
        delete(currentblock);
    }
}

int Data::get_block_pos(int x)
{
    while (x >= 80)
    {
        x -= 80;
    }

    return x;
}

/*bool Data::END(int l,int c)
{
    int finall=0;
    int finalb=0;
    Line *currentline=firstline;
    if(firstline==NULL)return true;
    while(currentline!=NULL)
    {
        finall++;
        currentline=currentline->nextLine;
    }
    if((l+1)>finall)return true;
    Block *currentblock=currentline->nextBlock;
    while(currentblock!=NULL)
    {
        finalb+=currentblock->L;
        currentblock=currentblock->nextBlock;
    }
    if((c+1)>finalb)return true;
    return false;
}*/
