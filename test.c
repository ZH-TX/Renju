#include<math.h>

#include<stdlib.h>

#include<time.h>

#include<iostream>

#include<cstdio>

#include "jsoncpp/json.h" // C++编译时默认包含此库  

#define N 7 //每个节点的分支数

//以下为各棋型的识别码  //权重

#define WIN 1 //4000

#define LOSE 2 //-4000

#define FLEX4 3 //2000

#define flex4 4 //-2000

#define BLOCK4 5 //1000

#define block4 6 //-1000 

#define FLEX3 7 //1000 

#define flex3 8 //-1000

#define BLOCK3 9 //400 

#define block3 10 //-600  

#define FLEX2 11 //400 

#define flex2 12 //-600 

#define BLOCK2 13 //100 

#define block2 14 //-150 

#define FLEX1 15 //100 

#define flex1 16 //-150 

using namespace std;
using namespace Json;

char board[15][15]; //当前棋盘

//给棋盘board[15][15]赋值
void placeAt(int x,int y,int d) {
	if(x>=0&&y>=0)
		board[x][y]=d;
}

//复制棋盘A到B 
int copy_board(char A[15][15],char B[15][15])
{
	int i,j;
	
	for(i=0;i<15;i++)
	{
		for(j=0;j<15;j++)
		{
			B[i][j]=A[i][j];
		}
	}
	return 0;
}

//复制取反后的棋盘A到B
int inv_board(char A[15][15],char B[15][15])
{
	int i,j;
	
	int INV[3]={0,2,1};
	
	for(i=0;i<15;i++)
	{
		for(j=0;j<15;j++)
		{
			B[i][j]=INV[A[i][j]];
		}
	}
	return 0;
}

//棋型辨识数组  
int score[3][3][3][3][3][3]; 

//给棋型辨识数组赋值 
int init_score()
{
	//黑五连 AI胜 (这里的黑不是指先手，而是指AI)（棋型 1 //4000） 
	score[1][1][1][1][1][1]=WIN;
	score[1][1][1][1][1][0]=WIN;
	score[0][1][1][1][1][1]=WIN;
	score[1][1][1][1][1][2]=WIN;
	score[2][1][1][1][1][1]=WIN;
	
	//白五连 AI负  （棋型 2 // -4000） 
	score[2][2][2][2][2][2]=LOSE;
	score[2][2][2][2][2][0]=LOSE;
	score[0][2][2][2][2][2]=LOSE;
	score[2][2][2][2][2][1]=LOSE;
	score[1][2][2][2][2][2]=LOSE;
	
	
	//黑活四   （棋型 3 // 2000） 
	score[0][1][1][1][1][0]=FLEX4;
	
	//白活四	（棋型 4 // -2000） 
	score[0][2][2][2][2][0]=flex4;
	
	
	//黑活三	（棋型 7 // 1000） 
	score[0][1][1][1][0][0]=FLEX3;
	score[0][1][1][0][1][0]=FLEX3;
	score[0][1][0][1][1][0]=FLEX3;
	score[0][0][1][1][1][0]=FLEX3;
	
	//白活三	（棋型 8 // -1000） 
	score[0][2][2][2][0][0]=flex3;
	score[0][2][2][0][2][0]=flex3;
	score[0][2][0][2][2][0]=flex3;
	score[0][0][2][2][2][0]=flex3;
	
	
	//黑活二	（棋型 11 // 400） 
	score[0][1][1][0][0][0]=FLEX2;
	score[0][1][0][1][0][0]=FLEX2;
	score[0][1][0][0][1][0]=FLEX2;
	score[0][0][1][1][0][0]=FLEX2;
	score[0][0][1][0][1][0]=FLEX2; 
	score[0][0][0][1][1][0]=FLEX2;
	
	//白活二	（棋型 12 // -600） 
	score[0][2][2][0][0][0]=flex2;
	score[0][2][0][2][0][0]=flex2;
	score[0][2][0][0][2][0]=flex2;
	score[0][0][2][2][0][0]=flex2;
	score[0][0][2][0][2][0]=flex2; 
	score[0][0][0][2][2][0]=flex2;
	
	
	//黑活一	（棋型 15 // 100） 
	score[0][1][0][0][0][0]=FLEX1;
	score[0][0][1][0][0][0]=FLEX1;
	score[0][0][0][1][0][0]=FLEX1;
	score[0][0][0][0][1][0]=FLEX1;
	
	//白活一	（棋型 16 // -150） 
	score[0][2][0][0][0][0]=flex1;
	score[0][0][2][0][0][0]=flex1;
	score[0][0][0][2][0][0]=flex1;
	score[0][0][0][0][2][0]=flex1;
	
	
	int p1,p2,p3,p4,p5,p6,x,y,ix,iy;
	
	for(p1=0;p1<3;p1++)
	{
		for(p2=0;p2<3;p2++)
		{
			for(p3=0;p3<3;p3++)
			{
				for(p4=0;p4<3;p4++)
				{
					for(p5=0;p5<3;p5++)
					{
						for(p6=0;p6<3;p6++)
						{
							x=0;
							y=0;
							ix=0;
							iy=0;
							if(p1==1) x++;
							else if(p1==2) y++;
							
							if(p2==1) x++,ix++;
							else if(p2==2) y++,iy++;
							
							if(p3==1) x++,ix++;
							else if(p3==2) y++,iy++;
							
							if(p4==1) x++,ix++;
							else if(p4==2) y++,iy++;
							
							if(p5==1) x++,ix++;
							else if(p5==2) y++,iy++;
							
							if(p6==1) ix++;
							else if(p6==2) iy++;
							
							//黑冲四 
							if(x==4&&y==0||ix==4&&iy==0)
							{
								if(score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=BLOCK4;  //（棋型 5 // 1000）  
								}
							}
							
							//白冲四 
							else if(x==0&&y==4||ix==0&&iy==4)
							{
								if(score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=block4;  //（棋型 6 // -1000） 
								}
							}
							
							//黑眠三 
							else if(x==3&&y==0||ix==3&&iy==0)
							{
								if(score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=BLOCK3;  //（棋型 9 // 400） 
								}
							}
							
							//白眠三 
							else if(x==0&&y==3||ix==0&&iy==3)
							{
								if(score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=block3;  //（棋型 10 // -600） 
								}
							}
							
							//黑眠二
							else if(x==2&&y==0||ix==2&&iy==0)
							{
								if(score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=BLOCK2; //（棋型 13 // 100） 
								}
							}
							
							//白眠二
							else if(x==0&&y==2||ix==0&&iy==2)
							{
								if(score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=block2;  //（棋型 14 // -150） 
								}
							}
							
						}
					}
				}
			}
		}
	}
	
	return 0;
}

struct evaluation
{
	int score;
	int result;
};

//对棋盘A的局势进行评分 
struct evaluation evaluate(char A[][15])
{
	//各棋型权重 
	int weight[17]={0,4000,-4000,2000,-2000,1000,-1000,1000,-1000,400,-600,400,-600,100,-150,100,-150};
	
	int i,j,s;
	
	int stat[4][17]={0};

	int STAT[17];
	
	struct evaluation EVALUATION;
	
	//棋型统计
	/*
	0 1 2 ... 14
	1
	2
	.
	.
	.
	14 
	*/
	for(i=0;i<15;i++)
	{
		for(j=0;j<10;j++)
		{
			s=score[A[i][j]][A[i][j+1]][A[i][j+2]][A[i][j+3]][A[i][j+4]][A[i][j+5]];
			/*
			横向五子的情况 
			i=0,j=0 a[0][0] a[0][1] a[0][2] a[0][3] a[0][4] a[0][5]
				j=1 a[0][1,2,3,4,5,6]
				...
				j=9 a[0][9,10,11,12,13,14,15]
			...
			i=14,j=0 a[14][0,1,2,3,4,5]
				j=1 a[14][1,2,3,4,5,6]
				...
				j=9 a[14][9,10,11,12,13,14,15]				
			*/ 
			stat[1][s]++;
		}
	}
	
	for(j=0;j<15;j++)
	{
		for(i=0;i<10;i++)
		{
			s=score[A[i][j]][A[i+1][j]][A[i+2][j]][A[i+3][j]][A[i+4][j]][A[i+5][j]];
			/*
			竖向五子的情况 
			j=0,i=0 a[0][0]
					a[1][0]
					a[2][0]
					a[3][0]
					a[4][0]
					a[5][0]
				
			*/ 
			stat[2][s]++;
		}
	}
	
	for(i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			s=score[A[i][j]][A[i+1][j+1]][A[i+2][j+2]][A[i+3][j+3]][A[i+4][j+4]][A[i+5][j+5]];
			/*
			斜向五子的情况1 
			i=0,j=0 a[0][0]
						a[1][1]
							a[2][2]
							...
								a[5][5]
			*/ 
			stat[3][s]++;
		}
	}
	
	for(i=5;i<15;i++)
	{
		for(j=0;j<10;j++)
		{
			s=score[A[i][j]][A[i-1][j+1]][A[i-2][j+2]][A[i-3][j+3]][A[i-4][j+4]][A[i-5][j+5]];
			/*
			斜向五子的情况2
			i=5,j=0 a[5][0]
				a[4][1]
			a[3][2]
		a[2][3]
	a[1][4]
a[0][5]
			*/
			stat[0][s]++;
		}
	}
	
	s=0;
	
	//初步评分累加 
	for(i=1;i<17;i++)
	{
		s+=(stat[1][i]+stat[2][i]+stat[3][i]+stat[0][i])*weight[i]; //当前棋局的得分 
		
		STAT[i]=(stat[1][i]>0)+(stat[2][i]>0)+(stat[3][i]>0)+(stat[0][i]>0);  //存在这种棋型的方向的个数 
	}
	
	EVALUATION.result=0;  //记录当前棋局的结果，没有人 胜出则为 0. 
	
	//胜 
	if(STAT[1]>0)
	{
		s+=100000;   //在计算了各种棋型得分值情况下，这种棋局能使我方胜，则得分加 100000. 
		EVALUATION.result=1;
	}
	
	//负 
	else if(STAT[2]>0)
	{
		s-=100000;  //在计算了各种棋型得分值情况下，这种棋局能使对方胜，则得分减 100000.  
		EVALUATION.result=2;
	}
	
	//对手冲四、活四 
	else if(STAT[4]>0||STAT[6]>0)
	{
		s-=30000;
	}
	
	//对手无冲四、活四 
	else if(STAT[4]==0&&STAT[6]==0)
	{
		int k=0;
		
		//检验 冲四活三
		for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				if(i!=j)
				{
					k+=stat[i][5]*stat[j][7];
				}
			}
		}
		
		//活四
		if(STAT[3]>0)
		{
			s+=20000;
		} 
		
		//双冲四
		else if(STAT[5]>=2)
		{
			s+=20000;
		}
		
		//冲四活三
		else if(k>0)
		{
			s+=20000;
		} 
		
		//对手有活三 
		else if(STAT[8]>0&&STAT[5]==0)
		{
			s-=20000;
		}
		
		//双活三 
		else if(STAT[7]>=2&&STAT[8]==0)
		{
			s+=10000;
		}
	}
	EVALUATION.score=s;
		
	return EVALUATION;
}

struct points
{
	//N=7, 每一个子周围8个，0-7 
	int coo_x[N]; //横坐标 
	
	int coo_y[N]; //纵坐标 
	
	int eva[N]; //评估值 
	
	int exi[N]; //存在性 
};

//评估出最佳的N个待定落子点 
struct points seek_points(char A[][15])
{
	struct points best_points;
	
	struct evaluation EVA;
	
	int i,j,k;
	
	int worth[15][15];
	
	int B[15][15]={0};
	 
	for(k=0;k<N;k++)
	{
		best_points.exi[k]=0;
	}
	/*
	评估当前棋盘 
	*/ 
	EVA=evaluate(A);
	
	
	if(EVA.result>0)
	{
		best_points.exi[0]=1;
		
		best_points.eva[0]=EVA.score;
		
		for(i=0;i<15;i++)
		{
			for(j=0;j<15;j++)
			{
				if(A[i][j]==1)
				{
					best_points.coo_x[0]=i;
					
					best_points.coo_y[0]=j;
					
					goto the_end;
				}
			}
		}
	}
	
	for(i=0;i<15;i++)
	{
		for(j=0;j<15;j++)
		{
			if(A[i][j]!=0)
			{//a[i][j]处有子，在i,j的四周辐射3层 
				for(k=-3;k<=3;k++)
				{
					if(i+k>=0&&i+k<15)
					{
						B[i+k][j]=1;
						
						if(j+k>=0&&j+k<15)
						{
							B[i+k][j+k]=1;
						}
						if(j-k>=0&&j-k<15)
						{
							B[i+k][j-k]=1;
						}
					}
					if(j+k>=0&&j+k<15)
					{
						B[i][j+k]=1;
					}
				}
			}
		}
	}
	
	//对于棋盘A上的空点，评估在该处落子后的局势 
	for(i=0;i<15;i++)
	{
		for(j=0;j<15;j++)
		{
			worth[i][j]=-1000000;
			
			if(A[i][j]==0&&B[i][j]==1)
			{//从近到远，考察四周辐射的3层 
				A[i][j]=1;
				
				EVA=evaluate(A);
				
				worth[i][j]=EVA.score;
				
				A[i][j]=0;
			}
		}
	}

	int w;

	//筛选最佳的N个点 
	for(k=0;k<N;k++)
	{//最佳的N个点，[0]对应的点评估收益最高 
		w=-1000000;
		
		for(i=0;i<15;i++)
		{
			for(j=0;j<15;j++)
			{
				if(worth[i][j]>w)
				{
					w=worth[i][j];
					
					best_points.coo_x[k]=i;
					
					best_points.coo_y[k]=j;
				}
			}
		}
		//best_points.eva[0]是评估的N个里的最高收益，-w>3000，可以结束，因为收益已经很小 
		if( (k>0) && ((best_points.eva[0]-w)>3000) ) break;
		
		best_points.eva[k]=w;
		
		best_points.exi[k]=1;
		
		worth[best_points.coo_x[k]][best_points.coo_y[k]]=-1000000;
	}
	
	the_end:
	
	return best_points;
}

struct decision
{
	int coo_x;
	
	int coo_y;
	
	int eva;
};
//全局变量-记录决策的落子位置 
struct decision DECISION;

int level_limit=6;

//博弈树MinMax递归分析 AlphaBeta剪枝
int analyse(char A[][15],int level,int alpha,int beta)
{	
	if(level==level_limit)
	{
		struct points P;
		
		P=seek_points(A);
		
		alpha=P.eva[0];
		
		return alpha;
	}
	
	else if(level%2==0)
	{//max层，我方决策 
		struct points P;
		
		P=seek_points(A);
		
		for(int i=0;i<N;i++)
		{
			if(P.exi[i]==1)
			{
				char buff[15][15];
				
				copy_board(A,buff);
				//我方做出决策 
				buff[P.coo_x[i]][P.coo_y[i]]=1;
				
				int a;
				//递归分析我方决策后的棋局 
				a=analyse(buff,level+1,alpha,beta);
				//我方关注最大化收益，alpha初始-inf，只会增加或不变 
				if(a>alpha)
				{
					alpha=a;
					
					if(level==0)
					{
						DECISION.coo_x=P.coo_x[i];
						
						DECISION.coo_y=P.coo_y[i];
						
						DECISION.eva=a;
					}
				}
			}
			//剪枝策略
			if(beta<=alpha) break;
		}
		return alpha;
	}
	
	else if(level%2==1)
	{//min层，敌方决策 
		char BUFF[15][15];
		//在min层，对方采取使我方收益最小的策略，反转棋盘则过程同上了 
		inv_board(A,BUFF);
		
		struct points P; 
		//敌方在BUFF棋盘寻找最佳落子处（对我方最不利的） 
		P=seek_points(BUFF);
		
		for(int i=0;i<N;i++)
		{
			if(P.exi[i]==1)
			{
				char buff[15][15];
				
				copy_board(A,buff);
				//敌方落子 
				buff[P.coo_x[i]][P.coo_y[i]]=2;
				
				int a;
				//递归分析敌方决策后的棋局 
				a=analyse(buff,level+1,alpha,beta);
				
				if(a<beta)
				{
					beta=a;
				}
			}
			if(beta<=alpha) break;
		}
		return beta;
	}
}

bool start() {
	for(int i=0;i<15;i++)
		for(int j=0;j<15;j++)//开始棋盘为空 
			if(board[i][j]!=0) return 0;
	return 1;
}

Value pushout(int x,int y) {
	Json::Value action;
	action["x"]=x;
	action["y"]=y;
	return action;
}


int main()
{
	
	init_score();
	
	string str;
	getline(cin, str);
	Reader reader;
	Value input;
	reader.parse(str, input); 
	int turnID = input["responses"].size();
	

	int turnNum_rsp = input["responses"].size();
	int turnNum_req = input["requests"].size();
	//对方先手
	int zero = 0;
	if (input["requests"][zero]["x"] != -1)
	{
		//己方第四回合是否选择了换手,第六回合才能从resopnses判别
		if (turnNum_rsp >= 2 && input["responses"][1]["x"] == -1)
		{
			//换手前下的子
			for (int i = 0; i < 2; i++)
			{
				placeAt(input["requests"][i]["x"].asInt(),input["requests"][i]["y"].asInt(),1);
				placeAt(input["responses"][i]["x"].asInt(),input["responses"][i]["y"].asInt(),2);
			}
			//换手后下的子
			for (int i = 2; i < turnNum_rsp; i++)
			{
				placeAt(input["requests"][i]["x"].asInt(),input["requests"][i]["y"].asInt(),2);
				placeAt(input["responses"][i]["x"].asInt(),input["responses"][i]["y"].asInt(),1);
			}
			placeAt(input["requests"][turnNum_rsp]["x"].asInt(),input["requests"][turnNum_rsp]["y"].asInt(),2);
		}
		//未达第六回合或未换手
		else
		{
			for (int i = 0; i < turnNum_rsp; i++)
			{
				placeAt(input["requests"][i]["x"].asInt(),input["requests"][i]["y"].asInt(),2);
				placeAt(input["responses"][i]["x"].asInt(),input["responses"][i]["y"].asInt(),1);
			}
			placeAt(input["requests"][turnNum_rsp]["x"].asInt(),input["requests"][turnNum_rsp]["y"].asInt(),2);
		}
	}
	//己方先手
	else
	{
		//对方选择了换手
		if (turnNum_req >= 3 && input["requests"][2]["x"] == -1)
		{
			//换手前下的子
			for (int i = 0; i < 2; i++)
			{
				placeAt(input["requests"][i]["x"].asInt(),input["requests"][i]["y"].asInt(),1);
				placeAt(input["responses"][i]["x"].asInt(),input["responses"][i]["y"].asInt(),2);
			}
			//换手后下的子
			for (int i = 2; i < turnNum_rsp; i++)
			{
				placeAt(input["requests"][i]["x"].asInt(),input["requests"][i]["y"].asInt(),2);
				placeAt(input["responses"][i]["x"].asInt(),input["responses"][i]["y"].asInt(),1);
			}
			placeAt(input["requests"][turnNum_rsp]["x"].asInt(),input["requests"][turnNum_rsp]["y"].asInt(),2);
		}
		//未达到第五回合或未换手
		else
		{
			for (int i = 0; i < turnNum_rsp; i++)
			{
				placeAt(input["requests"][i]["x"].asInt(),input["requests"][i]["y"].asInt(),2);
				placeAt(input["responses"][i]["x"].asInt(),input["responses"][i]["y"].asInt(),1);
			}
			placeAt(input["requests"][turnNum_rsp]["x"].asInt(),input["requests"][turnNum_rsp]["y"].asInt(),2);
		}
	}
	
	Value ret;
	if(start()) 
	{
		ret["response"]=pushout(7,7);
	}
	else if(input["requests"][zero]["x"] != -1&&turnID == 1)
	{
		int x1=input["requests"][zero]["x"].asInt();
		int y1=input["requests"][zero]["y"].asInt();
		int x2=input["requests"][1]["x"].asInt();
		int y2=input["requests"][1]["y"].asInt();
		int a=(x1-x2)*(x1-x2);
		int b=(y1-y2)*(y1-y2);
		if(a<=1&&b<=1)
		{
			ret["response"]=pushout(-1,-1);
		}
		else
		{
			//核心函数-analyse 
		analyse(board,0,-1000000,1000000);
		board[DECISION.coo_x][DECISION.coo_y]=1;
		ret["response"]=pushout(DECISION.coo_x,DECISION.coo_y);
		}
		
	}
	 else {
		//核心函数-analyse 
		analyse(board,0,-1000000,1000000);
		board[DECISION.coo_x][DECISION.coo_y]=1;
		ret["response"]=pushout(DECISION.coo_x,DECISION.coo_y);
	}
	FastWriter writer;
	cout<<writer.write(ret)<<endl;
	
	return 0;
}

