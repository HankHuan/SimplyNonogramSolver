#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include <ctime>
//-----------------------------------------------
int goableCount1=1;
int goableCount2=1;
int goableCase=1;
int status=1; //0:CONFLICT 1:INCOMPLETE 2:SOLVED 3:PAINTED
int p0Status;
int p1Status;
struct S{
	int s[26];//s[0]==0 start at index 1
};
S sInit(S ss){
	int i;
	ss.s[0]=0;
	for(i=1;i<26;i++){
		ss.s[i]=2;
	}
	return ss;
}

//-----------------------------------------------

struct answerS{
	int inAnswer[26][26];
	int doWhat[3];
};

answerS initAnswerS(answerS as){
	int i=0,j=0;
	for(i=0;i<26;i++){
		for(j=0;j<26;j++){
			if(i==0||j==0){
				as.inAnswer[i][j]=0;
			}
			else{
				as.inAnswer[i][j]=2;	
			}
		}
	}
	return as;
}
//-----------------------------------------------

// white:0 black:1 borw:2
void sInit(void);
void oneLineInit(int index);
void oneLineCovertStringToD(int index);
void propagation(void);

bool fix(int i,int j,int index,S ss);
bool fix0(int i,int j,int index,S ss);
bool fix1(int i,int j,int index,S ss);
bool match1(int i,int j,int index,S ss);
bool match(int i,int j,int index,S ss);

S paint(int i,int j,int index,S ss);
S paintI(int i,int j,int index,S ss);
S paint0(int i,int j,int index,S ss);
S paint1(int i,int j,int index,S ss);
S merge(int i,int j,S ss,S tt,int index);


void propagation(int mode);
void FP1(answerS);
void probe(int pR,int pC);
void probeG(int pR,int pC,int c);

answerS backtracking(answerS aS);
void chosePixel(answerS aS);


bool checkAnswer();
//-----------------------------------------------


//-----------------------------------------------


int answer[26][26];
int answerP0[26][26];
int answerP1[26][26];
int finalAnswer[26][26];

void initAnswer(void){
	int i=0,j=0;
	for(i=0;i<26;i++){
		for(j=0;j<26;j++){
			if(i==0||j==0){
				answer[i][j]=0;
			}
			else{
				answer[i][j]=2;	
			}
		}
	}
}

void printAnswer(){
	int i=0,j=0;
	for(i=1;i<26;i++){
		for(j=1;j<26;j++){
			printf("%d ",answer[i][j]);
		}
		printf("\n");
	}	
}

void writeAnswer(int cas){
	FILE *fp;
	int i,j;
	fp = fopen("output.txt","a");
	fprintf(fp,"$%d",cas);
	fprintf(fp,"\n");
	for(i=1;i<=25;i++){
		for(j=1;j<=25;j++){
			fprintf(fp,"%d	",answer[i][j]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}


//-----------------------------------------------


//-----------------------------------------------

struct LINE{
	char numStream[60];
	int d[13];//star at index 1
	int dNum;
};
LINE line[60];//index from 0

void oneLineInit(int index){
	int i;
	for(i=0;i<60;i++){
		line[index].numStream[i]=NULL;
	}
	for(i=0;i<13;i++){
		line[index].d[i]=-1;
	}
	line[index].dNum=0;
}

void oneLineCovertStringToD(int index){
	int i=1;
	line[index].dNum=0;
	char *deli = "	";
	char *one;
	one = strtok(line[index].numStream,deli);
	while(one!=NULL){
		line[index].d[i++]=atoi(one);
		one = strtok(NULL,deli);
		line[index].dNum++;
	}
	
}

//-----------------------------------------------

//-----------------------------------------------

struct G{
	int g[50];
};

G gInit(G gg){
	int i=0;
	for(i=0;i<50;i++){
		gg.g[i]=1;
	}
	return gg;
}

bool gHave(G gg){
	int i;
	for(i=0;i<50;i++){
		if(gg.g[i]==1){
			return true;
		}
	}
	return false;
}

int gGetCount(G gg){
	int i;
	for(i=0;i<50;i++){
		if(gg.g[i]==1){
			return i;
		}
	}
	return -1;
}

G gColSetCount(G gg,S ss,S tt){
	int i=0;
	for(i=1;i<=25;i++){
		if(ss.s[i]!=tt.s[i]){
			gg.g[i]=1;
		}
	}
	return gg;
}

G gRowSetCount(G gg,S ss,S tt){
	int i=0;
	for(i=1;i<=25;i++){
		if(ss.s[i]!=tt.s[i]){
			gg.g[i+24]=1;
		}
	}
	return gg;
}

//-----------------------------------------------

//-----------------------------------------------



//count0~24:row  count25~49:col   
//mode==2 fp1 call //mode==0 p0 call  //mode==1 p1 call
void propagation(int mode){
	S ss;
	S tt;
	G gg;
	gg=gInit(gg);
	
	int count=0,i,j=0,err=0;
	

	//printf("%dss%d\n",mode,answer[1][3]);

	
	//initAnswer();
	//printAnswer();
	while(gHave(gg)){
		count=gGetCount(gg);
		if(count<=24){
			for(i=0;i<26;i++){
				if(mode==2){
					ss.s[i]=answer[count+1][i];
					tt.s[i]=answer[count+1][i];					
				}
				else if(mode==0){
					ss.s[i]=answerP0[count+1][i];
					tt.s[i]=answerP0[count+1][i];						
				}
				else if(mode==1){
					ss.s[i]=answerP1[count+1][i];
					tt.s[i]=answerP1[count+1][i];						
				}
				//printf("%d ",ss.s[i]);
			}
			//printf("%d\n",goableCount++);
		}
		else{
			for(i=0;i<26;i++){
				if(mode==2){
					ss.s[i]=answer[i][count+1-25];
					tt.s[i]=answer[i][count+1-25];										
				}
				else if(mode==0){
					ss.s[i]=answerP0[i][count+1-25];
					tt.s[i]=answerP0[i][count+1-25];						
				}
				else if(mode==1){
					ss.s[i]=answerP1[i][count+1-25];
					tt.s[i]=answerP1[i][count+1-25];						
				}

				//printf("%d ",ss.s[i]);
			}
			//printf("%d\n",goableCount++);				
		}
		
		
		if(!fix(25,line[count].dNum,count,ss)){
			if(mode==2){
				status=0;
				return;
			}
			else if(mode==0){
				p0Status=0;
				return;
			}
			else if(mode==1){
				p1Status=0;
				return;
			}
		}
		else{
			//for(i=0;i<26;i++){
				//printf("%d",ss.s[i]);
			//}
			//printf(" %d\n",goableCount1++);
			
			
			//if(count==2){
				//printf("ss u %d %d %d\n",ss.s[1],goableCount2,mode);
			//}
			
			ss=paint(25,line[count].dNum,count,ss);
			
			//if(count==2){
				//printf("ss d %d %d %d\n",ss.s[1],goableCount2,mode);
			//}
					
			//for(i=0;i<26;i++){
				//printf("%d",ss.s[i]);
			//}
			//printf(" %d\n",goableCount2++);				
			//printf("\n");
			if(count<=24){
				for(i=0;i<26;i++){
					if(mode==2){
						answer[count+1][i]=ss.s[i];
					}
					else if(mode==0){
						answerP0[count+1][i]=ss.s[i];
					}
					else if(mode==1){
						answerP1[count+1][i]=ss.s[i];
					}
					//printf("%d ",ss.s[i]);
				}
				//printf("%d\n",goableCount++);
				gg.g[count]=0;
				gg=gRowSetCount(gg,ss,tt);
				
			}
			else{
				for(i=0;i<26;i++){
					if(mode==2){
						answer[i][count+1-25]=ss.s[i];	
					}
					else if(mode==0){
						answerP0[i][count+1-25]=ss.s[i];
					}
					else if(mode==1){
						answerP1[i][count+1-25]=ss.s[i];
					}
					//printf("%d ",ss.s[i]);
				}
				//printf("%d\n",goableCount++);
				gg.g[count]=0;	
				gg=gColSetCount(gg,ss,tt);
					
			}	
			
			//if(count==27){
				//printf("an d %d %d %d\n",answer[1][3],goableCount2,mode);
			//}
			//goableCount2++;										
		}
	}
	
	//printf("ano d %d %d %d\n",answer[1][3],goableCount2,mode);
	
	if(mode==2){
		for(i=0;i<26;i++){
			for(j=0;j<26;j++){
				if(answer[i][j]==2){
					err++;
				}
			}
		}
		if(err==0){
			status=2;
		}
		else{
			status=1;
		}		
	}
	else if(mode==0){
		for(i=0;i<26;i++){
			for(j=0;j<26;j++){
				if(answerP0[i][j]==2){
					err++;
				}
			}
		}
		if(err==0){
			p0Status=2;
		}
		else{
			p0Status=1;
		}		
	}
	else if(mode==1){
		for(i=0;i<26;i++){
			for(j=0;j<26;j++){
				if(answerP1[i][j]==2){
					err++;
				}
			}
		}
		if(err==0){
			p1Status=2;
		}
		else{
			p1Status=1;
		}		
	}
	
	
		
	//ss=sInit(ss);
	//ss=paint(25,line[0].dNum,0,ss);
	//for(i=1;i<26;i++){
		//printf("%d",ss.s[i]);
	//}
	//printf(" test\n");		
	return;
}
//-----------------------------------------------

//-----------------------------------------------


//-----------------------------------------------

//-----------------------------------------------

struct whereeII{
	int wii[2];
};

//status  0:CONFLICT 1:INCOMPLETE 2:SOLVED 3:PAINTED
int II[26][26];

void initII(void){
	int i=0,j=0;
	for(i=0;i<26;i++){
		for(j=0;j<26;j++){
			if(i==0||j==0){
				II[i][j]=-1;
			}
			else{
				II[i][j]=0;	
			}
		}
	}
}

int haveII(){
	int i=0,j=0,k=0;
	for(i=1;i<26;i++){
		for(j=1;j<26;j++){
			if(II[i][j]==1){
				k++;
			}
		}
	}
	return k;	
}

whereeII whereII(){
	whereeII w;
	
	int i=0,j=0,k=0;
	for(i=1;i<26;i++){
		for(j=1;j<26;j++){
			if(II[i][j]==1){
				w.wii[0]=i;
				w.wii[1]=j;
				return w;
			}
		}
	}
	return w;	
}

struct Pix{
	int pixScore;
	int pix0;
	int pix1;
};
Pix pix[26][26];
void initPix(){
	int i,j;
	for(i=1;i<26;i++){
		for(j=1;j<26;j++){
			pix[i][j].pix0=0;
			pix[i][j].pix1=0;
			pix[i][j].pixScore=-1;
		}
	}
}


int goableFcount=0;

//status  0:CONFLICT 1:INCOMPLETE 2:SOLVED 3:PAINTED
void FP1(void){
	initPix();
	whereeII w;
	G gg;
	int i,j,k,l,m=0,change=0;
	int tem[26][26];	
	
	initII();
	goableFcount=0;
	//printf("tt%d\n",answer[1][3]);
	//printf("%d ",haveII());
	//initAnswer();
	
	while(m==0 || haveII()>0){
		m++;
		//writeAnswer(1);
		w=whereII();
		//printf("%d haveII(): %d i=%d j=%d\n",m,haveII(),w.wii[0],w.wii[1]);
		//printf("=%d=\n\n", haveII());
		for(i=1;i<26;i++){
			for(j=1;j<26;j++){
				tem[i][j]=answer[i][j];
			}
		}
		//printf("p u %d %d\n",answer[1][3],goableCount1);	
		//printf(" %d ",answer[1][11]);
		propagation(2);
		//printf(" %d \n",answer[1][11]);
		//printf("p d %d %d\n",answer[1][3],goableCount1);
		//goableCount1++;
		
		if(status==0 || status==2){
			return;
		}
		
		for(i=1;i<26;i++){
			for(j=1;j<26;j++){
				if(tem[i][j]!=answer[i][j]){
					II[i][j]=1;
				}
				else{
					II[i][j]=0;
				}
			}
		}		
		
		initPix();
		for(i=1;i<26;i++){
			for(j=1;j<26;j++){
				if(answer[i][j]==2){
					probe(i,j);
					//printf("%d ",status);
					if(status==0 || status==2){
						return;
					}
					if(status==3){
						j=30;
						i=30;
					}
					//printf("=%d %d=",i,j);						
				}
				//printf("=%d %d=",i,j);	
			}
			//change=0;
			//for(k=0;k<26;k++){
				//for(l=0;l<26;l++){
					//if(tem[k][l]!=answer[k][l]){
						//change++;
					//}
				//}
			//}	
			//printf("%d %d \n",i,change);
			//writeAnswer(i);
		}
		
		//printf("-%d- ",haveII());
		//writeAnswer(1);
		//printf("\n");
	}
		
	
	return;
}

//status  0:CONFLICT 1:INCOMPLETE 2:SOLVED 3:PAINTED

void probe(int pR,int pC){
	int i,j,err=0;
	//int tempII[26][26];
	
	//for(i=0;i<26;i++){
		//for(j=0;j<26;j++){
			//tempII[i][j]=II[i][j];
		//}
	//}
	
	
	probeG(pR,pC,0);
	probeG(pR,pC,1);
	if(p0Status==0 && p1Status==0){
		status=0;
		return;
	}
	
	if(p0Status==0){
		for(i=1;i<26;i++){
			for(j=1;j<26;j++){
				if(answer[i][j]!=answerP1[i][j]){
					answer[i][j]=answerP1[i][j];
					II[i][j]=1;
				}
				else{
					II[i][j]=0;
					//printf("%d ",II[i][j]);
				}
			}
		}
	}
	else if(p1Status==0){
		for(i=1;i<26;i++){
			for(j=1;j<26;j++){
				if(answer[i][j]!=answerP0[i][j]){
					answer[i][j]=answerP0[i][j];
					II[i][j]=1;
				}
				else{
					II[i][j]=0;
					//printf("%d ",II[i][j]);
				}
			}
		}		
	}
	else{
		for(i=1;i<26;i++){
			for(j=1;j<26;j++){
				if(answerP1[i][j]==answerP0[i][j] && answerP1[i][j]!=answer[i][j]){
					answer[i][j]=answerP0[i][j];
					II[i][j]=1;
				}
				else{
					II[i][j]=0;
					if(answerP0[i][j]!=answer[i][j]){
						pix[i][j].pix0++;
					}
					else if(answerP1[i][j]!=answer[i][j]){
						pix[i][j].pix1++;
					}
					//printf("%d ",II[i][j]);
				}
			}
		}	
		//printf("@@%d ",haveII());		
	}
	
	
	if(haveII()>0){
		//for(i=0;i<26;i++){
			//for(j=0;j<26;j++){
				//II[i][j]=tempII[i][j];
			//}
		//}		
		status=3;
	}
	else{
		status=1;
	}
	
	//for(i=1;i<26;i++){
		//for(j=0;j<26;j++){
			//if(answer[i][j]==2){
				//err++;
			//}
		//}
	//}
	//if(err==0){
		//status=2;
	//}
	//else{
		//status=1;
	//}
	return;
}

void probeG(int pR,int pC,int c){
	int i,j;
	
	
	if(c==0){
		for(i=0;i<26;i++){
			for(j=0;j<26;j++){
				answerP0[i][j]=answer[i][j];
			}
		}
		answerP0[pR][pC]=c;		
	}
	else{
		for(i=0;i<26;i++){
			for(j=0;j<26;j++){
				answerP1[i][j]=answer[i][j];
			}
		}	
		answerP1[pR][pC]=c;	
	}
	
	
	propagation(c);
	//if(c==0){
		//if(p0Status==0){
			//return;
		//}				
	//}
	//else{
		//if(p1Status==0){
			//return;
		//}		
	//}
	return;
}



//-----------------------------------------------

//-----------------------------------------------

int goableBacktractingTimes=0;

//status  0:CONFLICT 1:INCOMPLETE 2:SOLVED 3:PAINTED
answerS backtracking(answerS aS){
	int i,j,k,l;
	
	
	for(i=0;i<26;i++){
		for(j=0;j<26;j++){
			answer[i][j]=aS.inAnswer[i][j];
		}
	}
	
	
	//printf("fu%d\n",aS.inAnswer[1][3]);
	//printAnswer();
	//printf("ans u %d\n",answer[1][3]);
	FP1();
	//printf(" \n first fp1 end\n");
	//writeAnswer(1);
	//printf("ans d %d\n",answer[1][3]);
	//printAnswer();
	//status=1=2;
	//printf("fd%d\n",aS.inAnswer[1][3]);
	
	for(i=0;i<26;i++){
		for(j=0;j<26;j++){
			aS.inAnswer[i][j]=answer[i][j];
		}
	}
	//printf("fp1");
	//writeAnswer(1);
	if(status==0){
		//printf("\n\ns=0 CONFLICT\n\n");
		return aS;
	}
	if(status==2){
		if(goableBacktractingTimes>0){
			//writeAnswer(goableCase);
			//printf("status : solve\n");
			if(checkAnswer()){
				//printf("check answer: true");
			}
			else{
				//printf("check answer: false");
			}						
		}		
		//printf("\ns=2 SOLVE\n");
		return aS;
	}
	
	
	//printf("backtracking start \n");
	goableBacktractingTimes++;
	chosePixel(aS);
	
	//writeAnswer(1);
	
	//aS=chosePixel(aS,0);
	//backtracking(aS);
	//aS=chosePixel(aS,1);
	//backtracking(aS);	
	
	
	
	//chosePixel(0);
	//backtracking();
	//chosePixel(1);
	//backtracking();
	
	//return;
}

int goableI=1;
int goableJ=1;

void chosePixel(answerS aS){
	int i,j,pi,pj,pf,ps;
	answerS aS0,aS1;
	aS0=initAnswerS(aS0);
	aS1=initAnswerS(aS1);
	
	
	int max=0;
	for(i=1;i<26;i++){
		for(j=1;j<26;j++){
			pix[i][j].pixScore=(pix[i][j].pix0+1)*(pix[i][j].pix1+1);
			if(pix[i][j].pixScore>max){
				max=pix[i][j].pixScore;
				pi=i;
				pj=j;				
			}
		}
	}
	
	if(pix[pi][pj].pix0>=pix[pi][pj].pix1){
		pf=0;
		ps=1;
	}
	else{
		pf=1;
		ps=0;
	}
	
/*	
	for(i=25;i>0;i--){
		for(j=25;j>0;j--){
			if(aS.inAnswer[i][j]==2){
				//aS.inAnswer[i][j]=p;
				//aS.doWhat[0]=p;
				//aS.doWhat[1]=i;
				//aS.doWhat[2]=j;
				
				
				pi=i;
				pj=j;
				i=-1;
				j=-1;
				//printf("chose %d %d \n",pi,pj);
			}
		}
	}
*/	
	for(i=0;i<26;i++){
		for(j=0;j<26;j++){
			aS0.inAnswer[i][j]=aS.inAnswer[i][j];
		}
	}
	
	
	
	//printf("chose %d %d to %d\n",pi,pj,pf);	
	aS0.inAnswer[pi][pj]=pf;
	backtracking(aS0);
	
	if(status==2){
		return;
	}
	
	for(i=0;i<26;i++){
		for(j=0;j<26;j++){
			aS1.inAnswer[i][j]=aS.inAnswer[i][j];
		}
	}
	//printf("chose %d %d to %d\n",pi,pj,ps);		
	aS1.inAnswer[pi][pj]=ps;
	backtracking(aS1);
}







//-----------------------------------------------
//-----------------------------------------------


#define HashSize 10000000
struct HashTable{
	int key;
	S sAns;
};

HashTable *hashTable[HashSize+1];


void hashTableInit(){
	int i;
	for(i=0;i<HashSize+1;i++){
		hashTable[i] = (HashTable *) malloc (sizeof(HashTable));
	}
}

void hashTableInsert(S ss){
	int i;
	for(i=0;i<26;i++){
		
	}
}

S hashTableFind(S ss){
	return ss;
}

//-----------------------------------------------
//-----------------------------------------------

bool fix(int i,int j,int index,S ss){
	if(i==0 && j==0){
		return true;
	}
	else if(i==0&&j>=1){
		return false;
	}
	else{
		return (fix0(i,j,index,ss) || fix1(i,j,index,ss));
	}
}

bool fix0(int i,int j,int index,S ss){
	if(ss.s[i]==0||ss.s[i]==2){
		return fix(i-1,j,index,ss);
	}
	else{
		return false;
	}
}

bool fix1(int i,int j,int index,S ss){
	//printf("%d ",j);
	if(j==1 && i==line[index].d[j]&&match1(i,j,index,ss)){
		return fix(i-line[index].d[j],j-1,index,ss);
	}
	if(j>=1 && i>=line[index].d[j]+1 && match(i,j,index,ss)){
		return fix(i-line[index].d[j]-1,j-1,index,ss);
	}
	else{
		return false;	
	}
}

bool match(int i,int j,int index,S ss){
	int m,err=0;
	if(ss.s[i-line[index].d[j]]==1){
		err++;
	}
	for(m=i-line[index].d[j]+1;m<=i;m++){
		if(ss.s[m]==0){
			err++;
		}
	}
	if(err==0){
		return true;
	}
	else{
		return false;
	}
}

bool match1(int i,int j,int index,S ss){
	int m,err=0;


	for(m=i-line[index].d[j]+1;m<=i;m++){
		if(ss.s[m]==0){
			err++;
		}
	}
	if(err==0){
		return true;
	}
	else{
		return false;
	}
}
//-----------------------------------------------


//-----------------------------------------------
S paint(int i,int j,int index,S ss){
	//printf("%d ",line[index].d[j]);
	if(i==0){
		return ss;
	}
	else{
		return paintI(i,j,index,ss);
	}
}

S paintI(int i,int j,int index,S ss){
	if(fix0(i,j,index,ss) && !fix1(i,j,index,ss)){
		return paint0(i,j,index,ss);
	}
	else if(!fix0(i,j,index,ss) && fix1(i,j,index,ss)){
		return paint1(i,j,index,ss);
	}
	else{
		return merge(i,j,paint0(i,j,index,ss),paint1(i,j,index,ss),index);
	}
}

S paint0(int i,int j,int index,S ss){
	ss.s[i]=0;
	//printf("--%d--",i);
	ss=paint(i-1,j,index,ss);
	return ss;
}

S paint1(int i,int j,int index,S ss){
	int m;
	if(j>1 && i>line[index].d[j]){
	ss.s[i-line[index].d[j]]=0;	
	}
	
	//printf("%d ",line[index].d[j]);
	for(m=i-line[index].d[j]+1;m<=i;m++){
		ss.s[m]=1;
	}
	if(j>1 && i>line[index].d[j]){
		ss=paint(i-line[index].d[j]-1,j-1,index,ss);
	}	
	else{
		ss=paint(i-line[index].d[j],j-1,index,ss);
	}
	return ss;
}

S merge(int i,int j,S ss,S tt,int index){
	int k=0;
	for(k=1;k<=i;k++){
		if(ss.s[k]==1 && tt.s[k]==1){
			ss.s[k]=1;
		}
		else if(ss.s[k]==0 && tt.s[k]==0){
			ss.s[k]=0;
		}
		else{
			ss.s[k]=2;
		}
	}
	return ss;
}
//-----------------------------------------------

//-----------------------------------------------

bool checkAnswer(){
	LINE answerLine[60];
	int i,j,k=0,lineC=0,lineDC=1,err=0;
	
	
	for(i=0;i<60;i++){
		for(j=0;j<13;j++){
			answerLine[i].d[j]=-1;	
		}
		answerLine[i].dNum=0;
	}
	
	for(i=1;i<26;i++){
		for(j=1;j<26;j++){
			if(answer[i][j]==1){
				k++;
			}
			else{
				if(k!=0){
					answerLine[lineC].d[lineDC]=k;
					answerLine[lineC].dNum++;
					lineDC++;
					k=0;					
				}
			}
		}
		lineC++;
		lineDC=1;
		k=0;
	}
	
	k=0,lineDC=1;
	for(j=1;j<26;j++){
		for(i=1;i<26;i++){
			if(answer[i][j]==1){
				k++;
			}
			else{
				if(k!=0){
					answerLine[lineC].d[lineDC]=k;
					answerLine[lineC].dNum++;
					lineDC++;
					k=0;					
				}
			}
		}
		lineC++;
		lineDC=1;
		k=0;
	}
	/*
	printAnswer();
	
	for(i=0;i<50;i++){
		printf("%d: ",i);
		for(j=1;j<answerLine[i].dNum;j++){
			printf("%d ",answerLine[i].d[j]);
		}
		printf("\n");
	}
	*/
	for(i=0;i<50;i++){
		for(j=1;j<line[i].dNum;j++){
			if(answerLine[i].d[j]!=line[i].d[j]){
				printf(" %d %d \n",answerLine[i].d[j],line[i].d[j]);
				err++;
			}
		}
	}
	
	if(err==0){
		return true;
	}
	else{
		printf("err:%d\n",err);
		return false;
	}
		
}


//-----------------------------------------------
int main(void){
	//hashTableInit();
	
	
	int count=0,i,j,cas=1;
	answerS aS;
	char numStream[60];
	char filename[20];

	//char *inFileName;
	//printf("file name: ");
	//scanf("%s",inFileName);
	
	FILE *file,*fi;
	fi = fopen("output.txt","w");
	fclose(fi);
	system("del output.txt");
	//file = fopen("questions.txt","r");
	
	printf("Enter input file name:");
	scanf("%s",filename);
	
	if(!(file = fopen(filename,"r"))){
		printf("file %s can not found\n",filename);
		system("pause");
		return 0;
	}
	
	time_t startTime = time(NULL);
	clock_t startClk = clock();
	clock_t thisClk;
	
	while(fgets(line[(count<25?count+25:count-25)].numStream,60,file)){
		if(line[(count<25?count+25:count-25)].numStream[0]=='$'){
			for(i=1;i<60;i++){
				numStream[i-1]=line[(count<25?count+25:count-25)].numStream[i];
			}
			cas=atoi(numStream);
			
			printf("$%d Start\n",cas);
			goableCase=cas;
			count=0;
			for(i=0;i<60;i++){
				oneLineInit(i);
			}
		}
		else{
			
			oneLineCovertStringToD((count<25?count+25:count-25));
			
			//for(i=1;i<=line[(count<25?count+25:count-25)].dNum;i++){
				//printf("%d ",line[(count<25?count+25:count-25)].d[i]);
			//}
			//printf("\n");
			
			count++;
			if(count==50){
				thisClk = clock();
				initAnswer();
				aS = initAnswerS(aS);
				backtracking(aS);
				//initAnswer();
				//propagation(2);
				if(status==2){
					if(goableBacktractingTimes>=0){
						writeAnswer(cas);
						//printf("status : solve\n");
						if(checkAnswer()){
							//printf("check answer: true\n");
							printf ( "$%d Solved\tOneTime:%4lfsec\tTotalTime:%ldsec\n" , cas , (double)(clock()-thisClk)/CLOCKS_PER_SEC, time(NULL)-startTime);
						}
						else{
							printf("check answer: false");
						}						
					}
				}
				else if(status==0){
					writeAnswer(cas);
					//printAnswer();
					printf(" CONFLICT");
				}
				else{
					writeAnswer(cas);
					printf(" INCOMPLETE");
				}
				//printAnswer();
				//printf("\n");
				//checkAnswer();
			}			
		}
	}
	fclose(file);
	
	return 0;
}

