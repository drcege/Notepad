#include "data.h"
#include <string>
#include <list>
#include <iostream>

using namespace std;

Data::Data()
{
    clip = "";
}

Data::~Data()
{

}

bool Data::Load(string s)
{
    int i=0;
    int t=0;
    bool flag=true;
    Line *currentline;
    Block *currentblock=NULL;
    if(s[0]!='\0')
    {
                  firstline=new Line;
                  currentline=firstline;
                  currentline->nextLine=NULL;
                  currentline->nextBlock=NULL;

    }
    while(s[i]!='\0')
    {
            if(t==0)
            {
                    if(flag)
                    {
                            currentline->nextBlock=new Block;
                            currentblock=currentline->nextBlock;
                            currentblock->nextBlock=NULL;
                            currentblock->l=0;
                            flag=false;
                    }
                    else
                    {
                            currentblock->nextBlock=new Block;
                            currentblock=currentblock->nextBlock;
                            currentblock->nextBlock=NULL;
                            currentblock->l=0;
                    }
            }
            int pos=currentblock->l;
            currentblock->block[pos]=s[i];
            currentblock->l++;
            t++;
            if(s[i]=='\n')
            {
                          t=0;
                          flag=true;
                          currentline->nextLine=new Line;
                          currentline=currentline->nextLine;
                          currentline->nextLine=NULL;
                          currentline->nextBlock=NULL;
            }
            else if(t==length)
            {
                 t=0;
            }
            i++;
    }
    delete(currentline);
    delete(currentblock);
    return true;
}

bool Data::Copy(int beg, int end)
{
    Line *currentline=firstline;
    Block *currentblock=currentline->nextBlock;
    int t=0;
    int i=0;
    while(1)
    {
            if((i+currentblock->l-1)>=beg)
            {
                                       i=beg-i;
                                       break;
            }
            else
            {
                                       i+=currentblock->l;
                                       if(currentblock->block[currentblock->l-1]=='\n')
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
                      if(i==currentblock->l)
                      {
                                            i=0;
                                            if(currentblock->nextBlock==NULL)
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
    delete currentline;
    delete currentblock;
    return true;
}

bool Data::Cut(int beg, int end)
{

    return true;
}

bool Data::Paste(int l, int c)
{
    return true;
}

/*bool Data::Find(int &l, int &c, string s)
{
    return true;
}*/

bool Data::Enter(int l, int c)
{
    return true;
}

bool Data::Delete(int l, int c)
{
    return true;
}

bool Data::Backspace(int l, int r)
{
    return true;
}


bool Data::Update(string s, int l, int c)
{
    return true;
}

bool Data::Clear()
{
    return true;
}
string Data::Clip()
{
    return clip;
}

string Data::Text()
{
    return s;
}

