#include <iostream>
#include <cstdio>
#include <fstream>
#include <stdlib.h>
#include <list>
#include <vector>
using namespace std;

void png2hex(string filename, vector<char> &input){
    FILE *f=fopen(filename.c_str(),"rb");
    int n;
    unsigned char buf[16];
    unsigned long count = 0;
    while((n = fread(buf, sizeof(char), 16, f))>0){
        for(int i=0; i<n; i++){
            input.push_back(buf[i]);
        }
    }
    fclose(f);

}

void detect_bugs(){
    string s,tmp;
    int no=0;
    int flag=0;
    char ch;
    ifstream f("result.txt");
    while(f.get(ch)) s+=ch;
    //check out of bound error
    int a=s.find("Invalid read of size");
    if(a!=-1) {
        flag = 1;
        printf("(%d):  find bug!!! array out of bound bug  ",++no);
    }

    //check unfree mem
    a=s.find("definitely lost");
    if(a!=-1) {
        flag = 1;
        printf("(%d):  find bug!!! some memory are not freed  ",++no);
    }

    //check uninitial pointer error
    a=s.find("Use of uninitialised");
    if(a!=-1) {
        flag = 1;
        printf("(%d):  find bug!!! use uninitial pointer  ",++no);
    }

    //check uninitial memory error
    a=s.find("Conditional jump");
    if(a!=-1) {
        flag = 1;
        printf("(%d):  find bug!!! use uninitial memory  ",++no);
    }

    if(!flag) cout<<"no bug detected!";
    printf("\n");
}

//insert a random num at a random index
void variation_insert(string filename){
    vector<char> v;
    //png2hex("sample.png",v);
    FILE *inf=fopen("sample.png","rb");
    FILE *outf=fopen(filename.c_str(),"wb");
    unsigned char buf[1];
    srand((unsigned)time(NULL));
    int i=0;
    int index = rand()%560;
    while(fread(buf,sizeof(char),1,inf)>0){
        i++;
        fwrite(buf,sizeof(char),1,outf);
        if(i==index){
            buf[0]=rand()%200;
            fwrite(buf,sizeof(char),1,outf);
            printf("insert a random num [%d] at %d index, ",buf[0],index);
        }
    }
    fclose(inf);
    fclose(outf);

}

//delete data at random index
void variation_delete(string filename){
    FILE *inf=fopen("sample.png","rb");
    FILE *outf=fopen(filename.c_str(),"wb");
    unsigned char buf[1];
    srand((unsigned)time(NULL));
    int i=0;
    int index = rand()%560+1;
    while(fread(buf,sizeof(char),1,inf)>0){
        i++;
        if(i==index) {
            printf("delete data at %d index, ",index);
            continue;
        }
        fwrite(buf,sizeof(char),1,outf);

    }
    fclose(inf);
    fclose(outf);

}

//replace data
void variation_replace(string filename){
    FILE *inf=fopen("sample.png","rb");
    FILE *outf=fopen(filename.c_str(),"wb");
    unsigned char buf[1];
    srand((unsigned)time(NULL));
    int i=0;
    int index = rand()%560+1;
    while(fread(buf,sizeof(char),1,inf)>0){
        i++;
        if(i==index) {
            char tmp=rand()%100;
            printf("replace data at %d index, from %xd to %xd",index,buf[0],tmp);
            buf[0]=tmp;
        }
        fwrite(buf,sizeof(char),1,outf);

    }
    fclose(inf);
    fclose(outf);

}

int main(int argc, char *argv[]){
    int no = 0;
    //file variation
    for(int i=0; i<3; i++){
        for(int j=0;j<10;j++){
            string cmd = "cp sample.png ";
            string filename = "inputs/sample";
            filename +=(char)(i+'0');
            filename +=(char)(j+'0');
            cmd += filename;
            system(cmd.c_str());
            //variation
            printf("case no. %d : action : ",++no);
            if(i==0) variation_delete(filename);
            else if(i==1) variation_insert(filename);
            else variation_replace(filename);

            //fuzzing
            string fuzzing_cmd="valgrind --tool=memcheck --leak-check=full --log-file=\"result.txt\" ./test "+filename;
            system(fuzzing_cmd.c_str());
            cout<<"  result:  ";
            detect_bugs();

        }
    }


}
