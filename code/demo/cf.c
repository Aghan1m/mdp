#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>

#define MAX_LINE 300
#define SHEETNUM 5000
#define SONGNUM 5000
void initialize();
void split();
void add(long long song,long long sheet);
void recommend();
double cos(int i,int id);
void quickSort(double valueUseful[],long long songUseful[],int len);

long long cfsheet[SHEETNUM];
long long cfsong[SONGNUM];
bool cf[SHEETNUM][SONGNUM];

int main(void)
{
	int i,j;
	initialize();
	split();
	recommend();
}
void initialize(){
	int i,j;
	for(i=0;i<SHEETNUM;i++){
		cfsheet[i]=0;
		for(j=0;j<SONGNUM;j++){
			cf[i][j]=0;
			if(i==0)
				cfsong[j]=0;
		}
	}
}
void split(){
	char name[MAX_LINE];
	char singers[MAX_LINE];
	char songId[MAX_LINE];
	char sheetId[MAX_LINE];
	FILE *fp=fopen("Music_Data_Out.txt","r");
	FILE *fpname=fopen("Music_Data_name.txt","w");
	FILE *fpsingers=fopen("Music_Data_singers.txt","w");
	FILE *fpsongId=fopen("Music_Data_songId.txt","w");
	FILE *fpsheetId=fopen("Music_Data_sheetId.txt","w");
	char buf[MAX_LINE];
	char* bf;
	int i=0;
	int len;
	long long song;
	long long sheet;
	while(fgets(buf,MAX_LINE,fp)){
		bf=buf;
		len=strstr(bf,"--")-bf;
		strncpy(name,bf,len);
		name[len]='\n';
		name[len+1]=0;
		fputs(name,fpname);
		
		bf+=len+2;
		len=strstr(bf,"--")-bf;
		strncpy(singers,bf,len);
		singers[len]='\n';
		singers[len+1]=0;
		fputs(singers,fpsingers);
		
		bf+=len+2;
		len=strstr(bf,"--")-bf;
		strncpy(songId,bf,len);
		songId[len]='\n';
		songId[len+1]=0;
		fputs(songId,fpsongId);
		song=atoll(songId);
		
		bf+=len+2;
		strcpy(sheetId,bf);
		fputs(sheetId,fpsheetId);
		sheet=atoll(sheetId);
		
		add(song,sheet);
	}
	fclose(fp);
	fclose(fpname);
	fclose(fpsingers);
	fclose(fpsongId);
	fclose(fpsheetId);
}
void add(long long song,long long sheet){
	int i,j;
	for(i=0;cfsheet[i]!=0;i++)
		if(cfsheet[i]==sheet)
			break;
	for(j=0;cfsong[j]!=0;j++)
		if(cfsong[j]==song)
			break;
	cfsong[j]=song;
	cfsheet[i]=sheet;
	cf[i][j]=1;
}
void recommend(){
	long long sheet;
	char input[50];
	printf("Enter the sheetID:");
	scanf("%s",input);
	sheet=atoll(input);
	double value[SONGNUM];
	double valueUseful[SONGNUM];
	long long songUseful[SONGNUM];
	int i,j,count;
	double rel;
	for(i=0;i<SONGNUM;i++)
		value[i]=0;
	for(i=0;cfsheet[i];i++)
		if(sheet==cfsheet[i])
			break;
	int id=i;
	for(i=0;cfsheet[i];i++){
		if(i==id)
			rel=0;
		else
			rel=cos(i,id);
		for(j=0;cfsong[j];j++)
			value[j]+=rel*(double)cf[i][j];
	}
	for(i=0,j=0;cfsong[i];i++)
		if(value[i]>0.000000001){
			valueUseful[j]=value[i];
			songUseful[j]=cfsong[j];
			j++;
		}
	quickSort(valueUseful,songUseful,j);
	for(i=0;i<j;i++)
		printf("%lld %.12f\n",songUseful[i],valueUseful[i]);
}
double cos(int i,int id){
	int j;
	double result=0;
	double len1=0;
	double len2=0;
	for(j=0;cfsong[j];j++){
		len1+=cf[i][j];
		len2+=cf[id][j];
		result+=cf[i][j]&cf[id][j];
	}
	return result/len1/len2;
}
void quickSort(double valueUseful[],long long songUseful[],int numsSize){
    int i=-1,j;
    double temp1;
    long long temp2;
    if(numsSize<=0)
		return;
    double x=valueUseful[numsSize-1];
    for(j=0;j<numsSize-1;j++)
		if(valueUseful[j]>=x){
			i++;
			temp1=valueUseful[i];
			valueUseful[i]=valueUseful[j];
			valueUseful[j]=temp1;
			temp2=songUseful[i];
			songUseful[i]=songUseful[j];
			songUseful[j]=temp2;
		}
	temp1=valueUseful[i+1];
	valueUseful[i+1]=valueUseful[numsSize-1];
	valueUseful[numsSize-1]=temp1;
	temp2=songUseful[i+1];
	songUseful[i+1]=songUseful[numsSize-1];
	songUseful[numsSize-1]=temp2;
	quickSort(valueUseful,songUseful,i);
	quickSort(valueUseful+i+1,songUseful,numsSize-i-2);
}
