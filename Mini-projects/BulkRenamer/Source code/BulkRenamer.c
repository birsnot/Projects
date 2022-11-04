#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctype.h>

short isdir(struct dirent *ent,char *pathName);
short isreg(struct dirent *ent,char *pathName);
void strrep(char *flName,char *rmv,char *rpl);
void renamer(DIR *dr,char *rmv,char *rpl,char *pathName);
void allRenamer(DIR *dr,char *rmv,char *rpl,char *pathName);

int main(){
    printf(">>>File Renamer<<<");
    DIR *dr;
    char pathName[2000]="";
    char line[2000]="";
    char rm[100]="";
    char rp[100]="";
    printf("\nEnter the path name that contains the files you want to rename:");
    fgets(line,sizeof(line),stdin);
    sscanf(line,"%[^\n]s",pathName);
    if(pathName[strlen(pathName)-1]!='\\'&&pathName[strlen(pathName)-1]!='/')strcat(pathName,"/");
    if(!(dr=opendir(pathName))||strlen(pathName)<4){
        printf("Invalid path!");
        exit(1);
    }
    printf("What word/letter do you want remove?");
    fgets(line,100,stdin);
    sscanf(line,"%[^\n]s",rm);
    if(strlen(rm)==0){
        printf("You entered nothing!");
        exit(2);
    }
    printf("What word/letter do you want to replace with it?(hit only Enter if you only want to delete it.)");
    fgets(line,100,stdin);
    sscanf(line,"%[^\n]s",rp);
    printf("\nDo you want to rename the files in subdirectories also?(Y\\N)");
    char ch = getchar();
    ch = toupper(ch);
    if(ch=='Y')allRenamer(dr,rm,rp,pathName);
    else renamer(dr,rm,rp,pathName);
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
void renamer(DIR *dr,char *rmv,char *rpl,char *pathName){
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
            rename(fileName,fileName2);
        }

    }
}
void allRenamer(DIR *dr,char *rmv,char *rpl,char *pathName){
    renamer(dr,rmv,rpl,pathName);
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
            allRenamer(dr2,rmv,rpl,tempPath);
            closedir(dr2);
        }
    }
}
