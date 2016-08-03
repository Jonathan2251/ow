
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FREE        10
#define ASSIGNED    1
#define UNASSIGNED  0
#define EXIST       1
#define UNEXIST     0
#define NOTHING     10
#define TRUE        1
#define FALSE       0
#define RIGHT       1
#define FAULT       0
#define SUCCESS     1
#define FAILURE     0

class CharOccurList
{
public:
    char ch;
    int  frequence;
    CharOccurList *next;
};

class DigitPool;

class Letter
{
    char ch;
    int  digit;
    int  assigned[10];
    Letter *front,*back;
public:
    Letter(char aCh,DigitPool *aDigit,Letter *aF,Letter *aB);
    int GetCh();
    int GetDigit();
    int Assigned(int aI);
    Letter *Front();
    Letter *Back();
    void SetBack(Letter *aB);
    int NextDigit(DigitPool *aDigitPool);
    ~Letter() {};
};

class DigitPool
{
    int d[10];
public:
    DigitPool();
    void Return(int aD);
    int Request(Letter *aP);
    ~DigitPool() {};
};

class Ex1String
{
private:
    char str[20];
    int TranToDigit(char aCh,Letter *aH);
public:
    Ex1String(char *aStr);
    long TranToDigit(Letter *aH);
    ~Ex1String() {};
};

inline Letter::Letter(char aCh,DigitPool *aDigitPool,Letter *aF,Letter *aB)
{
    int i;

    ch    = aCh; 
    front = aF;
    back  = aB;
    for (i=0; i<10; i++)
    {
        assigned[i] = FALSE;
    }
    digit = aDigitPool->Request(this);
}

inline int Letter::GetCh()
{
    return ch;
}

inline int Letter::GetDigit()
{
    return digit;
}

inline int Letter::Assigned(int aI)
{
    return assigned[aI];
}

inline Letter *Letter::Front()
{
    return front;
}

inline Letter *Letter::Back()
{
    return back;
}

inline void Letter::SetBack(Letter *aB)
{
    back = aB;
}

int Letter::NextDigit(DigitPool *aDigitPool)
{
    int i;

    i = aDigitPool->Request(this);
    if (i != NOTHING)
    {
        aDigitPool->Return(digit);
        digit = i;
        assigned[digit] = TRUE;
        return SUCCESS;
    }
    else
    {
        for (i=0; i<10; i++)
        {
            if (assigned[i])
            {
                assigned[i] = FALSE;
            }
        }
        aDigitPool->Return(digit);
        digit = NOTHING;
       // 如果前面有字母,請它改派數字
        if (front != 0)
        {
            if (front->NextDigit(aDigitPool))
            {
                i = aDigitPool->Request(this);
                if (i != NOTHING)
                {
                    aDigitPool->Return(digit);
                    digit = i;
                    assigned[digit] = TRUE;
                }
                else
                {
                    printf("program has error! Why can't request\n");
                }
                return SUCCESS;
            }
            else
            {
                return FAILURE;
            }
        }
        else
        {
            return FAILURE;
        }
    }
}

DigitPool::DigitPool()
{
    int i;

    for (i=0; i<10; i++)
    {
        d[i] = EXIST;
    }
}

void DigitPool::Return(int aD)
{
    d[aD] = EXIST;
}

int DigitPool::Request(Letter *aP)
{
    int i;

    for (i=0; i<10; i++)
    {
        if ((!aP->Assigned(i))&&(d[i] == EXIST))
        {
            d[i] = UNEXIST;
            return i;
        }
    }
    return NOTHING;
}


inline Ex1String::Ex1String(char *aStr)
{
    strcpy(str,aStr);
}

int Ex1String::TranToDigit(char aCh,Letter *aH)
{
    Letter *p;

    for (p=aH; p!=0; p=p->Back())
    {
        if (aCh == p->GetCh())
        {
            return p->GetDigit();
        }
    }
    printf("TranToDigit has error !\n");
    return NOTHING;
}

long Ex1String::TranToDigit(Letter *aH)
{
    Letter* h=aH;
    char *p;
    long sum;

    sum = 0;
    for (p=str; *p != '\0'; p++)
    {
        sum = sum*10 + TranToDigit(*p,h);
    }
    return sum;
}


void AddToH( CharOccurList *aH, char aCh)
{
    for (;(aH->next != 0)&&(aH->ch != aCh); aH=aH->next);
    if (aH->ch == aCh)
    {
        aH->frequence++;
    }
    else
    {
        aH->next = new CharOccurList;
        aH = aH->next;
        aH->ch = aCh;
        aH->frequence = 1;
        aH->next = 0;
    }
}

void Swap(CharOccurList *p, CharOccurList *q)
{
    CharOccurList temp;

    temp.ch = p->ch;
    temp.frequence = p->frequence;
    p->ch = q->ch;
    p->frequence = q->frequence;
    q->ch = temp.ch;
    q->frequence = temp.frequence;
}

void Sort(CharOccurList *aH)
{
    CharOccurList *max, *p;

    if (aH != 0)
    {
        for (; aH->next != 0; aH=aH->next)
        {
            max = aH;
            for (p=aH->next; p != 0; p=p->next)
            {
                if (max->frequence < p->frequence)
                {
                    max = p;
                }
            }
            Swap(aH,max);
        }
    }
}

Letter *BuildLetterList(CharOccurList *aH,DigitPool *aDigit)
{
    CharOccurList *p;
    Letter *h, *t, *q;
    DigitPool* digit=aDigit;

    if (aH != 0)
    {

        h = new Letter(aH->ch,digit,0,0);
        q = h;
        for (p=aH->next; p != 0; p=p->next)
        {
            t = new Letter(p->ch,digit,q,0);
            q->SetBack(t); 
            q = t;
        }
    }
    return q;
}

void Release(CharOccurList *aH)
{
    CharOccurList *p;

    if (aH != 0)
    {
        for (p=aH,aH=aH->next; aH != 0; aH=aH->next)
        {
            delete p;
            p=aH;
        }
        delete p;
    }
}

int Ex1StringNo(char *aStr)
{
    int strNo;
    char *p;

    for (strNo=1,p=aStr; *p != '\0'; p++)
    {
        if ((*p == '+')||(*p == '='))
        {
            strNo++;
        }
    }
    return strNo;
}

Ex1String **BuildEx1String(char *aStr,int aStrNo)
{
    char *p,str[20];
    int strNo=aStrNo,i,j;
    Ex1String **a,*t;

    a = new Ex1String*[strNo];
    for (i=0,j=0,p=aStr; *p != '\0'; p++)
    {
        if ((*p == '+')||(*p == '='))
        {
            str[i] = '\0';
            t = new Ex1String(str);
            a[j] = t;
            j++;
            i = 0;
        }
        else
        {
            str[i] = *p;
            i++;
        }
    }
    str[i] = '\0';
    a[j] = new Ex1String(str);
    return a;
}

Letter *SetLetterOrder(char *aStr,DigitPool *aDigit)
{
    char *p;
    CharOccurList *h;
    Letter *tail;
    DigitPool* digit=aDigit;

// 將各字母出現的次數記錄於 h 串列中
    if ((*aStr != '\0')&&(*aStr != '+')&&(*aStr != '='))
    {
        h = new CharOccurList;
        h->ch = *aStr;
        h->frequence = 1;
        h->next = 0;
        for (p=(aStr+1); (*p != '\0'); p++)
        {
            if ((*p != '+')&&(*p != '='))
            {
                AddToH(h,*p);
            }
        }
    }
// 依字母出現的次數由多至少順序造 Letter 串列
    Sort(h);
    tail = BuildLetterList(h,digit);
    Release(h);
    return tail;
}

int CheckSolution(Letter *aP,Ex1String **aStr,int aStrNo)
{
    int i;
    long sum=0;
    Letter *h;
    Ex1String **str;

    h = aP;
    for (; h->Front() != 0; h=h->Front());
    str = aStr;
    for (i=0; i < aStrNo - 1; i++)
    {
        sum = sum + str[i]->TranToDigit(h);
    }
    if (sum == str[i]->TranToDigit(h))
    {
        return RIGHT;
    }
    else
    {
        return FAULT;
    }
}

void PrintSolution(Letter *aTail,FILE *aStream)
{
    Letter *p;
    FILE *stream=aStream;

    for (p=aTail; p!= 0; p=p->Front())
    {
        fprintf(stream,"%c=%d ",p->GetCh(),p->GetDigit());
    }
    printf ("\n");
}

int FindSolution(Letter *aTail, DigitPool *aDigitPool, Ex1String **aStr, int aStrNo,FILE *aStream)
{
    Letter *tail;
    DigitPool* digitPool=aDigitPool;
    Ex1String** str=aStr;
    int strNo=aStrNo;
    FILE *stream=aStream;
    int find=FALSE, next=1;

    tail = aTail;
    while (next)
    {
        if (CheckSolution(tail,str,strNo)==RIGHT)
        {
            PrintSolution(tail,stream);
            find = TRUE;
        }
        next = tail->NextDigit(digitPool);
    }
    return find;
}

 //---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "ex1.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OnKeyPress(TObject *Sender, char &Key)
{
    FILE *stream;
    int strNo;
    char str[30];
    Letter *tail;
    DigitPool digitPool;
    Ex1String **a;

    if ((int)Key != 13)
    {
        return;
    }
    remove("temp.txt");
    stream=fopen("temp.txt","w");
    strcpy(str,Edit1->Text.c_str());
    strNo = Ex1StringNo(str);
    a = BuildEx1String(str, strNo);
// 依字母出現的次數由多至少順序造 Letter 串列
    tail = SetLetterOrder(str,&digitPool);
    if (tail->GetDigit() == NOTHING)
    {
        fprintf(stream,"%s","無解\n");
    }
    else
    {
        if (!FindSolution(tail,&digitPool,a,strNo,stream))
        {
            fprintf(stream,"%s","無解\n");
        }
    }
    fclose(stream);
    Memo1->Lines->LoadFromFile("temp.txt");

}
//---------------------------------------------------------------------------
