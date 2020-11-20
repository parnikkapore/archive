/*
    TASK: Route and block (variant a)
    LANG: CPP
    AUTHOR: PeaTT~
    SCHOOL: KVIS
*/
#include<bits/stdc++.h>
using namespace std;

struct step{
    int x,y,dist,len;
    bool operator < (const step& a) const{
        return dist > a.dist;
    }
};

char inp[100][100], inp_show[100][100];
int w,h,i,j, lastLen=9999;
int dw[4]={1,0,-1,0},dh[4]={0,1,0,-1};
priority_queue<step> todo; step pick;
int bt[100][100][2], lo_len=0;
bool banned[100][100], rand_picked=false;

bool branchable(int x, int y){
    //int cnt = 0;
    if(banned[x][y]) return false;
    else return true;
    /*for(int i=0; i<4; i++){
        if(x+dw[i]<0||x+dw[i]>w||y+dh[i]<0||y+dh[i]>h){cnt++; continue;}
        if(inp[x+dw[i]][y+dh[i]]!='*' && inp[x+dw[i]][y+dh[i]]!='.'){cnt++;}
    }
    return cnt<4;*/
}

void add_node(int x, int y, int len, int oldx, int oldy){
    if(x<0||x>w||y<0||y>h) return;
    if(inp[x][y] != '.')   return;
    inp[x][y]='*';
    int dist = len+abs((w-1)-x)+abs((h-1)-y);
    //printf("++\t%d, %d\t/ %d\n", x, y, dist);
    step nu = {x, y, dist, len+1};
    todo.push(nu);
    bt[x][y][0]=oldx; bt[x][y][1]=oldy;
}

void pingback(int x, int y, int r){
    inp[x][y]='%';
    if(x!=0||y!=0) pingback(bt[x][y][0],bt[x][y][1], r);
    inp[x][y]=(j%26)+65; j++;
    if(!rand_picked && branchable(x,y) && r<=j && (x!=(w-1) || y!=(h-1))){
        inp[x][y]='$'; rand_picked=true;
        pick.x=x; pick.y=y;
    }
}

void fail_reporter(){
    if(banned[pick.x][pick.y]==false) lo_len++;
    banned[pick.x][pick.y]=true;
    printf("We're on a roll! %d positions banned\n", lo_len);
    for(i=0; i<w; i++){
        if(i*w/(w-1) <= (lo_len+1)*w/lastLen) printf("#");
        else                              printf("_");
    }
    printf("\n");
}

int main()
{
    srand(time(NULL));
    printf("Block the clear path (master)\n");
    printf("Map size [w h]\n>");
    scanf("%d %d", &w, &h);
    printf("Map ['.'/'#']\n");
    for(i=0; i<h; i++){
        printf(">");
        for(j=0; j<w; j++)
            inp[j][i]='.';
    }

    printf("\033[2J");
    while(true){
    while(!todo.empty()) todo.pop();
    add_node(0,0,0,0,0);
    while(true){
        /*for(i=0; i<h; i++){
            for(j=0; j<w; j++)
                printf("%c", inp[j][i]);
            printf("\n");
        }*/

        if(todo.empty()){fail_reporter(); break;}
        step cur = todo.top();
        //printf("#%d\t%d, %d\t| %d\n",cur.len, cur.x, cur.y, cur.dist);
        if(cur.x==(w-1)&&cur.y==(h-1)){printf("\033[1;1HCalculating...\n"); break;} //The first part clears the screen
        add_node(cur.x-1, cur.y, cur.len,cur.x,cur.y);
        add_node(cur.x+1, cur.y, cur.len,cur.x,cur.y);
        add_node(cur.x, cur.y-1, cur.len,cur.x,cur.y);
        add_node(cur.x, cur.y+1, cur.len,cur.x,cur.y);
        todo.pop();
        //return 2; //debug
    }

    if(!todo.empty()){
    printf("Length: %d\nDifficulty: %ld\n\n", todo.top().dist, todo.size());
    lastLen = todo.top().dist;
    j=0; rand_picked=false;
    pingback(w-1, h-1, rand()%(lastLen));

    for(i=0; i<h; i++){
        for(j=0; j<w; j++){
            inp_show[j][i] = inp[j][i];
            if(inp[j][i]=='.') inp_show[j][i]=' ';
            if(inp[j][i]=='#') inp_show[j][i]='+';
            if(inp[j][i]=='*') inp_show[j][i]='.';
            printf("%c", inp_show[j][i]);
        }
        printf("\n");
    }
    if(!rand_picked&&(lo_len+1>=lastLen)){
        printf("\nNo forks possible. Done.\n\n%d %d\n", w, h);
        for(i=0; i<h; i++){
            for(j=0; j<w; j++){
                if(inp_show[j][i]=='+') printf("#");
                else                    printf(".");
            }
            printf("\n");
        }
    return 0;}
    for(i=0; i<h; i++){
        for(j=0; j<w; j++){
            if(inp_show[j][i]==' ') inp[j][i]='.';
            else if(inp_show[j][i]=='+'||inp_show[j][i]=='$') inp[j][i]='#';
            else inp[j][i]='.';
        }
    }
    }else{ // FAIL - cannot reach goal
    for(i=0; i<h; i++){ //cancel all modifications
        for(j=0; j<w; j++){
            if(inp_show[j][i]=='$'||inp[j][i]!='#'){inp[j][i]='.';}
        }
    }
    }
    }
    return 1;
}
