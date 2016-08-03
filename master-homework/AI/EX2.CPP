#include <stdio.h>
#include <stdlib.h>

#define SUCCESS     1
#define FAILURE     0

class JCell
{
    char state;
    int  score;
public:
    JCell() { };
    char State();
    int Score();
    ~JCell() { };
};

class JRule
{
    JCell cell[11][11];
public:
    JRule() { };
    JCell Cell(int aI, int aJ);
    ~JRule() { };
};

class JPlayer
{
    int ruleNo;
    JRule **rule;
public:
    JPlayer() { };
    void ReadRules(FILE *aStream);
    int RuleNo();
    JRule *Rule(int aI);
    ~JPlayer() { };
};

class JPlayerRec
{
    long gameNo;
    long winNo;
public:
    JPlayerRec() { };
    long GameNo();
    long WinNo();
    ~JPlayerRec() { };
};

class JGod
{
private:
    JPlayerRec player[50];/*
    JPlayer *SelectFather();
    JPlayer *SelectMother(JPlayer *aFather);
    JPlayer *SelectDier(JPlayer *aFather,JPlayer *aMother);*/
public:
    JGod() { };
    void ReadData(FILE *aStream);
    JPlayerRec Player(int aI);
/*    JPlayer *CrossOver();*/
    ~JGod() { };
};

inline int Random(int aX,int aY)
{
    return  aX + ( rand() / 32767 ) * ( aY - aX );
};

inline char JCell::State()
{
    return state;
}

inline int JCell::Score()
{
    return score;
}

inline JCell JRule::Cell(int aI, int aJ)
{
    return cell[aI][aJ];
}

void JPlayer::ReadRules(FILE *aStream)
{
    int i, numRead;

    numRead = fread(&ruleNo,sizeof(int),1,aStream);
    if (numRead < 1)
    {
        printf("Read playerNo error!\n");
        exit(1);
    }
    rule = new JRule*[ruleNo];
    for (i = 0; i < this->RuleNo(); i++)
    {
        rule[i] = new JRule;
        numRead = fread(rule[i],sizeof(JRule),1,aStream);
        if (numRead < 1)
        {
            printf("read player[%d]-Rule error!\n",i);
            exit(1);
        }
    }
}

inline JRule *JPlayer::Rule(int aI)
{
    return rule[aI];
}

inline int JPlayer::RuleNo()
{
    return ruleNo;
}

inline long JPlayerRec::GameNo()
{
    return gameNo;
}

inline long JPlayerRec::WinNo()
{
    return winNo;
}

void JGod::ReadData(FILE *aStream)
{
    fread(player,sizeof(JPlayerRec),50,aStream);
}

JPlayerRec JGod::Player(int aI)
{
    return player[aI];
}

/*
JPlayer *JGod::SelectFather()
{
    int min,i;
    long totalScore;

    min = player[i].WinNo;
    for (i = 1; i < 50; i++)
    {
        if (player[i].WinNo() < min)
        {
            min = player[i].WinNo;
        }
    }
    for (totalScore = 0, i = 0; i < 50; i++)
    {
        if (player[i].GameNo() > 20)
        {
            totalScore += (player[i].WinNo - min);
        }
    }
    x = random.Random(0,totalScore);
    for (sum = 0, i = 0; x <= sum; i++)
    {
        sum += player[i].WinNo();
    }
    return i;
}*/

void main()
{
    FILE *stream;
    int i;
    JGod god;
    JPlayer player[50];

    stream=fopen("ex2.inp","rb");
    god.ReadData(stream);
    for (i = 0; i< 50; i++)
    {
        player[i].ReadRules(stream);
    }
    fclose(stream); 
}