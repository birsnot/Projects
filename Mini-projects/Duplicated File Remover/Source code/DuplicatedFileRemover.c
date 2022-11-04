#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctype.h>

short isdir(struct dirent *ent,char *pathName);
short isreg(struct dirent *ent,char *pathName);
void strrep(char *flName,char *rmv,char *rpl);
void remover(DIR *dr,char *rmv,char *rpl,char *pathName);
void allRemover(DIR *dr,char *rmv,char *rpl,char *pathName);
int fileCheck(char *fileName,char *fileName2);

int main(){
    printf(">>>Repeated File Remover<<<");
    DIR *dr;
    char pathName[2000]="";
    char line[2000]="";
    char rm[100]="";
    char rp[100]="";
    printf("\nEnter the path name that contains the repeated files you want to remove:");
    fgets(line,sizeof(line),stdin);
    sscanf(line,"%[^\n]s",pathName);
    if(pathName[strlen(pathName)-1]!='\\'&&pathName[strlen(pathName)-1]!='/')strcat(pathName,"/");
    if(!(dr=opendir(pathName))||strlen(pathName)<4){
        printf("Invalid path!");
        exit(1);
    }
    printf("What word/letter that indicates the repetition? (for the default = \" (1)\", press Enter.)");
    fgets(line,100,stdin);
    sscanf(line,"%[^\n]s",rm);
    if(strlen(rm)==0)strcpy(rm," (1)");
    strcpy(rp,"");
    printf("\nDo you want to rename the files in subdirectories also?(Y\\N)");
    char ch = getchar();
    ch = toupper(ch);
    if(ch=='Y')allRemover(dr,rm,rp,pathName);
    else remover(dr,rm,rp,pathName);
    closedir(dr);
    return 0;
}
short isdir(struct dirent *ent,char *pathName){
    char flName[2000]="";
    strcpy(flName,pathName);
    strcat(flName,ent->d_name);
    struct stat Status;
    stat(flName,&Status);
    return S_ISDIR(Status.st_mode);
}
short isreg(struct dirent *ent,char *pathName){
    char flName[2000]="";
    strcpy(flName,pathName);
    strcat(flName,ent->d_name);
    struct stat Status;
    stat(flName,&Status);
    return S_ISREG(Status.st_mode);
}
void strrep(char *flName,char *rmv,char *rpl){
    char *temp;
    char temp2[100]="";
    char *temp3;
    int a=0;
    for(temp3=flName;temp=strstr(temp3,rmv);temp3=&flName[a]){
        *temp='\0';
        temp +=strlen(rmv);
        strcpy(temp2,temp);
        strcat(flName,rpl);
        a = strlen(flName);
        strcat(flName,temp2);
    }
}
void remover(DIR *dr,char *rmv,char *rpl,char *pathName){
    struct dirent *ent;
    char fileName[2000]="";
    char fileName2[2000]="";
    strcpy(fileName,pathName);
    strcpy(fileName2,pathName);
    while(ent=readdir(dr)){
        strcpy(fileName,pathName);
        strcpy(fileName2,pathName);
        if(isreg(ent,pathName)){
            strcat(fileName,ent->d_name);
            strrep(ent->d_name,rmv,rpl);
            strcat(fileName2,ent->d_name);
            if(strstr(fileName,rmv)&&fileCheck(fileName,fileName2))
            remove(fileName);
        }

    }
}
int fileCheck(char *fileName,char *fileName2){
        FILE *fl;
        if((fl = fopen(fileName,"rb")) == NULL){
        return 0;}
        fseek(fl,0,SEEK_END);
        int l = ftell(fl);
        fclose(fl);
        FILE *fl2;
        if((fl2 = fopen(fileName2,"rb")) == NULL){
        return 0;}
        fseek(fl2,0,SEEK_END);
        int l2 = ftell(fl);
        fclose(fl);
        if(l == l2)return 1;
        else return 0;
    }
void allRemover(DIR *dr,char *rmv,char *rpl,char *pathName){
    remover(dr,rmv,rpl,pathName);
    struct dirent *ent;
    seekdir(dr,SEEK_SET);
    while(ent=readdir(dr)){
        if(isdir(ent,pathName)&&strcmp(ent->d_name,".")&&strcmp(ent->d_name,"..")){
            DIR *dr2;
            char tempPath[2000];
            strcpy(tempPath,pathName);
            strcat(tempPath,ent->d_name);
            strcat(tempPath,"/");
            dr2=opendir(tempPath);
            allRemover(dr2,rmv,rpl,tempPath);
            closedir(dr2);
        }
    }
}
