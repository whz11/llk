#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

int n;
int wordRandomStd=0;
int MAX_X;
int MAX_Y;
int a[100][100];
int b[100]; //桶排序记录表中是否还有相同数；
int rmove(int x1, int y1, int x2, int y2);

struct users
{
    int score;
    char name[20];
    int level;
}user,usr[100];

int isBlocked(int x,int y)    //1是 0不是；
{
    if(a[x][y]==32)
        return 0;
    else
        return 1;
}

static int xCheck(int x1, int y1, int x2, int y2)
{
    if (x1 == x2 && y1 == y2)
    {
        return 0;
    }
 
    if (x1 != x2)
    {
        return 0;
    }
 
    int start_y = y1<y2?y1: y2;
    int end_y = y1>y2?y1: y2;
 
    for (int j = start_y+1; j < end_y; j++)
    {
        if (isBlocked(x1, j))     //障碍
        {
            return 0;
        }
    }
 
    return 1;
}
static int yCheck(int x1, int y1, int x2, int y2)
{
    if (x1 == x2 && y1 == y2)
    {
        return 0;
    }
 
    if (y1 != y2)
    {
        return 0;
    }
 
    int start_x = x1<y2?x1: x2;
    int end_x = x1>x2?x1: x2;
 
    for (int i = start_x+1; i < end_x; i++)
    {
        if (isBlocked(i, y1))
        {
            return 0;
        }
    }
 
    return 1;
}
static int turn_once(int x1, int y1, int x2, int y2)
{
    if (x1 == x2 && y1 == y2)
    {
        return 0;
    }
 
    int c_x = x1, c_y = y2;
    int d_x = x2, d_y = y1;
 
    int ret = 0;
    if (!isBlocked(c_x, c_y))
    {
        ret |= yCheck(x1, y1, c_x, c_y) && xCheck(c_x, c_y, x2, y2);
    }
 
    if (!isBlocked(d_x, d_y))
    {
        ret |= yCheck(x1, y1, d_x, d_y) && xCheck(d_x, d_y, x2, y2);
    }
    if (ret)
    {
        return 1;
    }
 
    return 0;
}
static int turn_twice(int x1, int y1, int x2, int y2)
{
    if (x1 == x2 && y1 == y2)
    {
        return 0;
    }
 
    for (int i = 0; i <MAX_X; i++)
    {
        for (int j = 0; j <MAX_Y; j++)
        {
            if (i != x1 && i != x2 && j != y1 && j != y2)
            {
                continue;
            }
 
            if ((i == x1 && j == y1) || (i == x2 && j == y2))
            {
                continue;
            }
 
            if (isBlocked(i, j))
            {
                continue;
            }
 
            if (turn_once(x1, y1, i, j) && (yCheck(i, j,x2, y2) || xCheck(i,j, x2, y2)))
            {
                return 1;
            }
            if (turn_once(i, j, x2, y2) && (yCheck(x1,y1, i, j) || xCheck(x1,y1, i, j)))
            {
                return 1;
            }
 
        }
    }
 
    return 0;
}
int rmove(int x1, int y1, int x2, int y2)
{
    int ret = 0;
 
    ret = yCheck(x1, y1, x2, y2);
    if (ret)
    {
        return 1;
    }
    ret = xCheck(x1, y1, x2, y2);
    if (ret)
    {
        return 1;
    }
    ret = turn_once(x1, y1, x2, y2);
    if (ret)
    {
        return 1;
    }
    ret = turn_twice(x1, y1, x2, y2);
    if (ret)
    {
        return 1;
    }
 
    return 0;
}
/**
 *以上判断能否消除
 
 */
void wordStd(int x)
{
    if(x*x<100)
        wordRandomStd=x*x/5;
    else
        wordRandomStd=26;
}
void startMap(int x)
{
    int i,j;
    srand(time(NULL));
    for (i=0;i<x+2;i++)
    {
        for(j=0;j<x+2;j++)
        {
            if(i==0||j==0||j==x+1||i==x+1)
                a[i][j]=32;
            else
            a[i][j]=rand()%wordRandomStd+65;
            printf("%c ",a[i][j]);   //一个空格
        }
        printf("\n");
    }
    
}
void updateMap(int x)
{
    int i,j;
    for(i=0;i<x+2;i++)
    {
        for (j=0;j<x+2;j++)
        {
            printf("%c ",a[i][j]);   //一个空格
        }
        printf("\n");
    }
}
int search(int x)
{
    memset(b,0,sizeof b);
    int i,j;
    for(i=0;i<x+2;i++)
    {
        for(j=0;j<x+2;j++)
        {
            b[a[i][j]]++;
        }
    }
    if(b[32]==(x+2)*(x+2))
        return 1;

    return 0;
}
void addScore()
{
    
    FILE *p =fopen(".txt", "w");
    if(p==NULL)
    {
        printf("error");
    }
    fputc(user.score, p);
    fputc('-',p);
    fputs(user.name, p);
    fputc('-',p);
    fputc(user.level, p);
    fputs("\n", p);
    fclose(p);
    printf("榜单更新\n");
}
void updateRank()   //根据分数排名
{
    int i=0;
    FILE *pr =fopen(".txt", "r");
    FILE *pw =fopen(".txt", "w");
    if(pr==NULL||pw==NULL)
    {
        printf("error");
    }
    while (!feof(pr))
    {
        fscanf(pr, "%d-%s-%d",&usr[i].score,usr[i].name,&usr[i].level);
        i++;
    }
    int j,k;
    int m;
    int t;
    for(j=0;j<i;j++)
    {
        m=0;
        for(k=j;k<i;k++)
        {
            if(usr[m].score<usr[k].score)
                m=k;
        }
        t=usr[m].score;
        usr[m].score=usr[j].score;
        usr[j].score=t;
    }
    fclose(pr);
    for(j=0;j<i;j++)
    {
        fprintf(pw, "%d-%s-%d\n",usr[i].score,usr[i].name,usr[i].level);
    }
    fclose(pw);
    
}
void showRank()
{
    char ch;
    FILE *p =fopen(".txt", "r");
    if(p==NULL)
    {
        printf("error");
    }
    while (!feof(p))
    {
        ch=fgetc(p);
        printf("%c",ch);
    }
    fclose(p);
}
int main()
{
    printf("------连连看------\n");
    printf("\n");
    printf("------操作说明----\n");
    printf("\n");
    printf("-----------输入规则是x表示行，y表示列\n");
    printf("-----------退出请输入q\n");
    printf("-----------查看榜单\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");

    int s=1;
    int x1,y1,x2,y2;//x是行y是列
    int flag =1;//判断是否继续游戏
    int lg =0; //判断输赢
    char quit;
while (s){
    printf("输入生成棋盘大小（一个偶数>=4）\n");
    scanf("%d",&n);
    wordStd(n);
    startMap(n);
    printf("想重新生成按1，否则0\n");
    scanf("%d",&s);
    }
    user.level=wordRandomStd;
    MAX_X=n+2;
    MAX_Y=n+2;
    while(flag){
        
        printf("输入要删除的两点坐标：\n");
        quit=getchar();
        if(quit=='q')
            break;
        scanf("%d%d%d%d",&x1,&y1,&x2,&y2);
        if(rmove(x1,y1,x2,y2))
        {
            user.score +=100;
            a[x1][y1]=32;
            a[x2][y2]=32;
            updateMap(n);
        }
        else
        {
            user.score-=50;
            printf("抱歉这两个不能消，再试试\n");
        }
        
        if(search(n))
        {
            lg=1;
            break;
        }
    }
    if(lg==1)
        printf("you win\n");
    else
        printf("gg\n");
    printf("你的分数是%d,难度是%d，是否上榜(输入1是）\n",user.score,user.level);
    int ju;
    scanf("%d",&ju);
    if(ju)
        addScore();
    return 0;
}
