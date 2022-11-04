#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <wchar.h>
#include <wctype.h>

short isdir(struct _wdirent *ent,wchar_t *pathName);
short isreg(struct _wdirent *ent,wchar_t *pathName);
void nameCreater(wchar_t *fileName);
void fileNameCopier(wchar_t *srcPath, wchar_t *dstPath);
long int fileCount = 0;

int main(){
    printf(">>>File Name Copier<<<");

    wchar_t srcPath[2000];
    wchar_t dstPath[2000];
    wchar_t line[2000];

    printf("\nEnter the source path name: ");
    fgetws(srcPath,sizeof(line),stdin);
    srcPath[wcslen(srcPath)-1] = '\0';
    if(srcPath[wcslen(srcPath)-1]!='\\'&&srcPath[wcslen(srcPath)-1]!='/')wcscat(srcPath,L"/");

    printf("\nEnter the destination path name: ");
    fgetws(dstPath,sizeof(line),stdin);
    dstPath[wcslen(dstPath)-1] = '\0';
    if(dstPath[wcslen(dstPath)-1]!='\\'&&dstPath[wcslen(dstPath)-1]!='/')wcscat(dstPath,L"/");

    fileNameCopier(srcPath,dstPath);

    return 0;
}
short isdir(struct _wdirent *ent,wchar_t *pathName){
    wchar_t flName[2000];
    wcscpy(flName,pathName);
    wcscat(flName,(ent->d_name));
    struct _stat Status;
    _wstat(flName,&Status);
    return S_ISDIR(Status.st_mode);
}
short isreg(struct _wdirent *ent,wchar_t *pathName){
    wchar_t flName[2000];
    wcscpy(flName,pathName);
    wcscat(flName,ent->d_name);
    struct _stat Status;
    _wstat(flName,&Status);
    return S_ISREG(Status.st_mode);
}
void fileNameCopier(wchar_t *srcPath, wchar_t *dstPath){
    _WDIR *srcDir;
    if(!(srcDir = _wopendir(srcPath))){
        printf("Can not open path \"%ls\"",srcPath);
        exit(1);
    }
    wchar_t dstFileName[2000];
    wchar_t srcFileName[2000];
    struct _wdirent *ent;
    while((ent = _wreaddir(srcDir))){
        wcscpy(dstFileName,dstPath);
        wcscpy(srcFileName,srcPath);
        if(wcscmp(ent->d_name,L".") == 0);
        else if(wcscmp(ent->d_name,L"..") == 0);
        else if(isdir(ent, srcPath)){
            _wmkdir(wcscat(dstFileName, ent->d_name));
            fileNameCopier(wcscat(wcscat(srcFileName, ent->d_name),L"/"),wcscat(dstFileName,L"/"));
        }
        else if(isreg(ent, srcPath)){
            nameCreater(wcscat(dstFileName, ent->d_name));
            printf("\r%ld",++fileCount);
        }
    }
    _wclosedir(srcDir);
}
void nameCreater(wchar_t *fileName){
    FILE *file;
    if((file = _wfopen(fileName,L"r")) == NULL){
        if((file = _wfopen(fileName,L"wb")) == NULL){
            printf("Cannot create \"%ls\"\n",fileName);
            return;
        }
    }
    fclose(file);
}

