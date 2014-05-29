#include "data.h"
#include <sstream>
#include <string>
#include <list>
#include <iostream>

using namespace std;

//bool Copy(int,int);

Data::Data()
{
    firstline=new Line;
    firstline->nextBlock=new Block;
    firstline->nextBlock->L=0;
    firstline->nextBlock->nextBlock=NULL;
    firstline->nextLine=NULL;
    clip="";
}
Data::~Data()
{

}

void Data::DELETE_block(Block *currentblock)
{
    if(currentblock!=NULL)
    {
        DELETE_block(currentblock->nextBlock);
        delete(currentblock);
    }
}

void Data::INSERT(int t,bool flag,int i,string s,Line* &currentline,Block* &currentblock)
{
    while(s[i]!='\0')
    {
        if(t==0)
        {
            if(flag)
            {
                if(currentline->nextBlock!=NULL)
                {
                    DELETE_block(currentline->nextBlock);
                }
                currentline->nextBlock=new Block;
                currentblock=currentline->nextBlock;
                currentblock->nextBlock=NULL;
                currentblock->L=0;
                flag=false;
            }
            else
            {
                if(currentblock->nextBlock!=NULL)
                {
                    DELETE_block(currentblock);
                }
                currentblock->nextBlock=new Block;
                currentblock=currentblock->nextBlock;
                currentblock->nextBlock=NULL;
                currentblock->L=0;
            }
        }
        int pos=currentblock->L;
        currentblock->block[pos]=s[i];
        currentblock->L++;
        t++;
        if(s[i]=='\n' && s[i+1]!='\0')
        {
            t=0;
            flag=true;
            currentline->nextLine=new Line;
            currentline=currentline->nextLine;
            currentline->nextLine=NULL;
            currentline->nextBlock=NULL;
        }
        else if(t==80)
        {
            t=0;
        }
        i++;
    }
}

int Data::get_pos_block(int c)
{
    while(c>=80)c-=80;
    return  c;
}

Line* Data::get_line(int l)
{
    Line *currentline = firstline;
    for(int i=0;i<l;i++)
    {
        currentline=currentline->nextLine;
    }
    return currentline;
}

Block* Data::get_block(Block *currentblock,int c)
{
    for(int i=79;i<c;i+=80)
    {
        currentblock=currentblock->nextBlock;
    }
    return currentblock;
}

int Data::get_pos(int l,int c)
{
    int t=0;
    Line *currentline=firstline;
    Block *currentblock=currentline->nextBlock;
    for(int i=0;i<l;i++)
    {
        while(currentblock!=NULL)
        {
            t+=currentblock->L;
            currentblock=currentblock->nextBlock;
        }
        currentline=currentline->nextLine;
        currentblock=currentline->nextBlock;
    }
    t+=c;
    return t;
}

void Data::DELETE(Line *deleteline,Block *deleteblock)
{
    if(deleteblock!=NULL)
    {
        DELETE(deleteline,deleteblock->nextBlock);
        delete(deleteblock);
    }
    else if(deleteline->nextLine!=NULL)
    {
        DELETE(deleteline->nextLine,deleteline->nextLine->nextBlock);
        delete(deleteline->nextLine);
    }
}

void Data::DELETE_INSERT(int C,int l,int c,int add,Line * &currentline,Block * &currentblock)
{
    int st=0;
    int END=0;
    bool flag=false;
    if(C==0)flag=true;
    st=get_pos(l,c);
    if(add==1)
    {
        for(Block *i=currentline->nextBlock;i!=NULL;i=i->nextBlock)
        {
            END+=i->L;
        }
    }
    else
    {
        for(Block *i=currentline->nextLine->nextBlock;i!=NULL;i=i->nextBlock)
        {
            END+=i->L;
        }
    }
    END+=st-c;
    string CLIP=clip;
    clip="";
    Data::Copy(st+add,END);
    currentblock->L=C;
    Block *temporaryBlock=currentblock->nextBlock;
    INSERT(C,flag,0,clip,currentline,currentblock);
    DELETE_block(temporaryBlock);
    clip=CLIP;
}

bool Data::Load(string s)
{

    Line *currentline;
    Block *currentblock=NULL;
    if(s[0]!='\0')
    {
        firstline=new Line;
        currentline=firstline;
        currentline->nextLine=NULL;
        currentline->nextBlock=NULL;

    }
    INSERT(0,true,0,s,currentline,currentblock);
    return true;
}

bool Data::Copy(int beg, int end)
{
    clip="";
    Line *currentline=firstline;
    Block *currentblock=currentline->nextBlock;
    int t=0;
    int i=0;
    while(beg<end)
    {
        if((i+currentblock->L-1)>=beg)
        {
            i=beg-i;
            break;
        }
        else
        {
            i+=currentblock->L;
            if(currentblock->block[currentblock->L-1]=='\n')
            {
                currentline=currentline->nextLine;
                currentblock=currentline->nextBlock;
            }
            else
            {
                currentblock=currentblock->nextBlock;
            }
        }
    }
    while(t<(end-beg))
    {
        clip+=currentblock->block[i];
        i++;
        t++;
        if(i==currentblock->L)
        {
            i=0;
            if(currentblock->nextBlock==NULL)
            {
                currentline=currentline->nextLine;
                if(currentline!=NULL)currentblock=currentline->nextBlock;
            }
            else
            {
                currentblock=currentblock->nextBlock;
            }
        }
    }
    return true;
}

bool Data::Paste(int l, int c)
{
    //---------------------将后续内容同样复制到剪切板-----------------------//
    int END=0;
    int pos_st;
    string CLIP=clip;
    clip="";
    for(Line *i=firstline;i!=NULL;i=i->nextLine)
    {
        for(Block *j=i->nextBlock;j!=NULL;j=j->nextBlock)
        {
            END+=j->L;
        }
    }
    pos_st=get_pos(l,c);
    Data::Copy(pos_st,END);
    clip=CLIP+clip;
    //---------------------------------------------------------------------//

    //-------------------------定位并释放后续空间--------------------------//
    Line *currentline=get_line(l);
    Block *currentblock=get_block(currentline->nextBlock,c);
    DELETE(currentline,currentblock->nextBlock);
    //--------------------------------------------------------------------//

    //-----------------------------更新-----------------------------------//
    int t=0;
    int C=get_pos_block(c);
    t=C;
    currentblock->L=t;
    INSERT(0,false,0,clip,currentline,currentblock);
    //--------------------------------------------------------------------//

    //-------------------恢复剪切板内容-----------------------------------//
    clip=CLIP; 
    //--------------------------------------------------------------------//
    return true;
}
bool Data::Find(int& pos, string src, string dest)
{
    return true;

}

bool Data::Replace(int beg,int end, string s)
{
    return true;
}

bool Data::Enter(int l, int c)
{
    //--------------------------------定位---------------------------------//
    Line *currentline=get_line(l);
    Block *currentblock=get_block(currentline->nextBlock,c);
    //DELETE(currentline,currentblock->nextBlock);
    //---------------------------------------------------------------------//

    //------------将该行插入回车以后的字符复制到剪切板中-------------------//
    int END;
    int pos_st=get_pos(l,c);
    END=pos_st-c;
    string CLIP=clip;
    clip="";
    for(Block *i=currentline->nextBlock;i!=NULL;i=i->nextBlock)
    {
        END+=i->L;
    }
    Data::Copy(pos_st,END);
    DELETE_block(currentblock->nextBlock);
    //--------------------------------------------------------------------//

    //---------------------------------插入回车---------------------------//
    int C=get_pos_block(c);
    Line *temporaryline=currentline->nextLine;
    currentline->nextLine=new Line;
    currentline=currentline->nextLine;
    currentline->nextLine=temporaryline;
    currentblock->L=C+1;
    currentblock->block[C]='\n';
    currentblock=currentline->nextBlock;
    INSERT(0,true,0,clip,currentline,currentblock);
    //--------------------------------------------------------------------//

    //-------------------恢复剪切板内容-----------------------------------//
    clip=CLIP; 
    //--------------------------------------------------------------------//
    return true;
}

bool Data::Delete(int l, int c)
{
    //--------------------------------定位---------------------------------//
    Line *currentline=get_line(l);
    //cout<<currentline->nextBlock->block<<endl;
    Block *currentblock=get_block(currentline->nextBlock,c);
    //---------------------------------------------------------------------//
    //int END=0,st=0;
    int C;
    C=get_pos_block(c);
    //cout<<C<<endl;
    if((currentblock->L==C && currentblock->nextBlock==NULL)||(currentblock->block[C]=='\n'))
    {
        if(currentline->nextLine!=NULL)
        {

            Line *temporaryline=currentline;
            //cout<<currentline->nextBlock->block<<endl;
            //Block *temporaryblock=temporaryline->nextBlock;
            DELETE_INSERT(C,l+1,0,0,currentline,currentblock);
            temporaryline->nextLine=currentline->nextLine->nextLine;
            //cout<<temporaryline->nextBlock->block<<endl;
            //delete(temporaryline->nextLine);
        }
    }
    else
    {
        DELETE_INSERT(C,l,c,1,currentline,currentblock);
    }
    return true;
}
bool Data::Backspace(int l, int c)
{
    if(c==0)
    {
        if(l!=0)
        {
            Line *currentline=get_line(l-1);
            for(Block *i=currentline->nextBlock;i!=NULL;i=i->nextBlock)
            {
                c+=i->L;
            }
            currentline=currentline->nextLine;
            //cout<<l-1<<" "<<c-1<<endl;
            Delete(l-1,c-1);
        }
    }
    else
    {
        Delete(l,c-1);
    }
    return true;
}

bool Data::Cut(int beg,int end)
{
    clip="";
    Data::Copy(beg,end);
    Line *currentline=firstline;
    Block *currentblock=currentline->nextBlock;
    int i=0;
    int l=0;
    while(1)
    {
        if((i+currentblock->L-1)>=beg)
        {
            i=beg-i;
            break;
        }
        else
        {
            i+=currentblock->L;
            if(currentblock->block[currentblock->L-1]=='\n')
            {
                currentline=currentline->nextLine;
                l++;
                if(currentline!=NULL)
                {
                    currentblock=currentline->nextBlock;
                }
            }
            else
            {
                currentblock=currentblock->nextBlock;
            }
        }
    }
    for(Block *j=currentline->nextBlock;j!=currentblock;j=j->nextBlock)
    {
        i+=j->L;
    }
    for(int j=0;j<(end-beg);j++)
    {
        Delete(l,i);
    }
    return true;
}

bool Data::Update(string s, int l, int c)
{
    string CLIP=clip;
    clip=s;
    Paste(l,c);
    clip=CLIP;
    return true;
}

bool Data::Clear()
{
    DELETE(firstline,firstline->nextBlock);
    delete(firstline);
    firstline=NULL;
    return true;
}

string Data::Clip()
{
    return clip;
}

string Data::Text()
{
    string s="",CLIP="";
    int st=0,END=0;
    CLIP=clip;
    for(Line* i=firstline;i!=NULL;i=i->nextLine)
    {
        for(Block *j=i->nextBlock;j!=NULL;j=j->nextBlock)
        {
            END+=j->L;
        }
    }
    Data::Copy(st,END);
    s=clip;
    clip=CLIP;
    return s;
}

