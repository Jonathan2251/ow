#include <stdlib.h>
#include <stdio.h>

#define ACCEPT  1
#define REJECT  0
#define TRUE    1
#define FALSE   0
#define SUCCESS 1
#define FAILURE 0

class Node
{
    int stateNo;
    int ***v;
public:
    Node(int aStateNo);
    int GetStateNo();
    void SetStateNo(int aStateNo);
    int GetV(int i, int j, int k);
    void SetV(int i, int j, int k, int m);
    ~Node() {};
};

Node::Node(int aStateNo)
{
    int i,j,k;

    stateNo = aStateNo;
  // 配置一個 node 空間, 大小為 v[stateNo][stateNo][stateNo], v[][][]=0
    v = new int**[stateNo];
    for (i=0;i<stateNo;i++)
    {
        v[i] = new int*[stateNo];
        for (j=0;j<stateNo;j++)
            v[i][j] = new int[stateNo];
    }
    for (i=0;i<stateNo;i++)
        for (j=0;j<stateNo;j++)
            for (k=0;k<stateNo;k++)
                v[i][j][k] = 0;
    
}

inline int Node::GetStateNo()
{
    return stateNo;
}

inline void Node::SetStateNo(int aStateNo)
{
	stateNo = aStateNo;
}

inline int Node::GetV(int i, int j, int k)
{
    return v[i][j][k];
}

inline void Node::SetV(int i, int j, int k, int m)
{
    v[i][j][k] = m;
}

class Relation
{
    int stateNo;
    int **R;
public:
    Relation(Node *aNode1, Node *aNode2);
    int GetR(int i, int j);
    ~Relation() {};
};

Relation::Relation(Node *aNode1, Node *aNode2)
{
    int *r1,*r2;
    int i,j,k,found;

    stateNo = aNode1->GetStateNo();

    r1 = new int[stateNo];
    r2 = new int[stateNo];

    R  = new int*[stateNo];
    for (i=0;i<stateNo;i++)
        R[i] = new int[stateNo];
    for (i=0;i<stateNo;i++)
        for (j=0;j<stateNo;j++)
            R[i][j] = 0;

    for (i=0;i<stateNo;i++)
    {

      // 找 aNode1 中 (x,[1..15,i]) 之所有 x, -> r1

        for (j=0;j<stateNo;j++)
        {
            found = FALSE;
            for (k=0; ((k<stateNo)&&(found==FALSE)); k++)
            {
                if (aNode1->GetV(j,k,i) == 1)
                   found = TRUE;
            }
            if (found==TRUE)
                r1[j] = 1;
            else
                r1[j] = 0;
        }

      // 找 aNode2 中 (y,[i,1..15]) 之所有 y, -> r2

        for (j=0;j<stateNo;j++)
        {
            found = FALSE;
            for (k=0; ((k<stateNo)&&(found==FALSE)); k++)
            {
                if (aNode2->GetV(j,i,k) == 1)
                   found = TRUE;
            }
            if (found==TRUE)
                r2[j] = 1;
            else
                r2[j] = 0;
        }

      // R = r1 x r2

        for (j=0;j<stateNo;j++)
        {
            for (k=0;k<stateNo;k++)
            {
                if ((r1[j]==1)&&(r2[k]==1))
                {
                    R[j][k] = 1;
                }
            }
        }
    }
    delete r1;
    delete r2;
}

inline int Relation::GetR(int i, int j)
{
    return R[i][j];
}

class Nta
{
private:
    int stateNo, acceptNo;
    int ***map;
    Node *Generate(int x, int y);
    Node *Generate(Node *node1, Node *node2);
public:
    Nta(int aStateNo, int aAcceptNo); 
    ~Nta() {};
    int Reader(FILE *aStream);
    int Feed(char *aSequence);
};

Nta::Nta(int aStateNo, int aAcceptNo)
{
    int i,j;

    stateNo  = aStateNo;
    acceptNo = aAcceptNo;
    map = new int**[stateNo];
    for (i=0; i<stateNo; i++)
    {
        map[i] = new int*[stateNo];
        for (j=0; j<stateNo; j++)
            map[i][j] = new int[stateNo];
    }
}

int Nta::Reader(FILE *aStream)
{
    int i,j,k,state;
    char str[20];
 
  // input nta 的 state
    for (i=0; i<stateNo; i++)
    {
        for (j=0; j<stateNo; j++)
        {
            for (k=0; k<stateNo; k++)
                map[i][j][k] = 0;
            fscanf(aStream,"%s\n",str);
            for (k=0;str[k]!='\0';k++)
            {
              // 檢查 input state 是否正確 
                state = (int)str[k] - (int)'a';
                if ( (state < 0) || (state >= stateNo) )
                {
                    printf("input state error %s\n",str);
                    return FAILURE;
                }

                map[i][j][(int)(str[k])-(int)'a'] = 1;
            }

        }
    }
    return SUCCESS;
}

Node *Nta::Generate(int x, int y)
{
    Node *p;
    int i;

    p = new Node(stateNo);
    p->SetStateNo(stateNo);  
    for (i=0;i<stateNo;i++)
    {
        // v = ( (x,y),[x,y] ) (x,y):表 x 與 y 可生成的狀態 , 為一狀態之集合
        p->SetV(i,x,y,map[x][y][i]);
    }
    return p;
}

Node *Nta::Generate(Node *node1, Node *node2)
{
    Relation *r;
    Node *p;
    int i,j,k;

    r = new Relation(node1,node2);
    p = new Node(stateNo);
    p->SetStateNo(node1->GetStateNo());

    // 依關連 r(表 node1 與 node2 有可能的配對) 來生成新 node p
    for (i=0;i<stateNo;i++)
    {
        for (j=0;j<stateNo;j++)
        {
            if (r->GetR(i,j)==1)
            {
               for (k=0;k<stateNo;k++)
                   p->SetV(k,i,j,map[i][j][k]);
            }
        }
    }
    delete r;
    return p;
}

int Nta::Feed(char *aSequence)
{
    int i,j,k,size;
    char *ptr;
    Node **A;
    Node *temp;

  // 檢查 input sequence 是否正確 
  // A[size] = 輸入字串長度 - 1
    for (i=0,ptr=aSequence; *ptr != '\0'; i++,ptr++)
    {
        j = (int)(*ptr)-(int)'a';
        if ( (j < 0) || (j >= stateNo) )
        {
            printf("input sequence error\n");
            return REJECT;
        }
    }
    size = i-1;
 
  // input 字串只有一位 => 直接判斷是否落於 accept state
    if (size == 0)
    {
       if (((int)(*aSequence)-(int)'a') < (stateNo-acceptNo))
           return REJECT;
       else
           return ACCEPT;
    }

    A = new Node*[size];

  // 倒數第二層
    for (i=0,ptr=aSequence;i<size;i++,ptr++)
    {
        A[i] = Generate((int)(*ptr)-(int)'a',(int)(*(ptr+1))-(int)'a');
    }

  // 倒數第三層...直到頂層
    for (i=size-1;i>0;i--)
    {
        for (j=0;j<i;j++)
        {
	    temp = A[j];
            A[j] = Generate(A[j],A[j+1]);
	    delete temp;
        }
        delete A[i];
    }
  // 判斷是 accept 或 reject
    for (i=stateNo-acceptNo;i<stateNo;i++)
    {
        for (j=0;j<stateNo;j++)
        {
            for (k=0;k<stateNo;k++)
            {
                if (A[0]->GetV(i,j,k)==1)
                {
                    delete A[0];
                    return ACCEPT;
                }
            }
        }
    }
    delete A[0];
    return REJECT;
}

void Output(int aResult, char *aSequence)
{
    if (aResult == REJECT)
       printf("reject %s\n",aSequence);
    else
       printf("accept %s\n",aSequence);
}

int main(int argc,char *argv[])
{
    FILE *stream;
    Nta *nta;
    int aStateNo, aAcceptNo, result, i;
    char sequence[20];

    stream=fopen(argv[1],"rt");

    fscanf(stream,"%d %d\n",&aStateNo,&aAcceptNo);
    for (i=1;aStateNo!=0;i++)
    {
       // 依 aStateNo(狀態數),決定轉換 table 大小
        nta = new Nta(aStateNo,aAcceptNo);

       // 造 NTA machine => 設定轉換 table 內容
        result = nta->Reader(stream);
        if (result == SUCCESS)
        {
            printf("\nNTA %d\n",i);
            fscanf(stream,"%s\n",sequence);
        }
        else
        {
            printf("\nNTA %d input state is error!\n",i);
            fclose(stream);
            return FAILURE;
        }
        while (*sequence != '#')
        {
           result=nta->Feed(sequence); // 餵 NTA 一字串, 決定該字串是否 accept
           Output(result,sequence);
           fscanf(stream,"%s\n",sequence);
        }
        delete nta;
        fscanf(stream,"%d %d\n",&aStateNo,&aAcceptNo);
    }
    fclose(stream);
    return SUCCESS;
}
