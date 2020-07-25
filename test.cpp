#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
using namespace std;

void png2hex(string filename, vector<int> &input){
    FILE *f=fopen(filename.c_str(),"rb");
    int n;
    unsigned char buf[16];
    unsigned long count = 0;
    while((n = fread(buf, sizeof(char), 16, f))>0){
        /*vector<int> tmp;
        for(int i=0; i<n; i++){
            tmp.push_back((unsigned)buf[i]);
        }
        input.push_back(tmp);*/
        for(int i=0; i<n; i++){
            input.push_back((unsigned)buf[i]);
        }
    }
    fclose(f);

}

void use_uninitial_pointer(){
    int *a;
    *a=1;
}

void use_uninitial_mem(){
    int x;
    if (x == 0){
        x++;
    }
}

void out_of_bound(){
    vector<int> v(10, 0);
    int a = v[10];
    //cout << v[10] << std::endl;
}

void no_free(){
    int *a=(int*)malloc(sizeof(int));
    *a=2;
}

int main(int argc,char *argv[])
{
    vector<int> fpng;
    vector<int> origion;
    int length,index=8;
    char title;
    png2hex(*++argv,fpng);
    png2hex("sample.png",origion);
    for(int i=0;i<32;i++){
        if(fpng[i]!=origion[i]){
            use_uninitial_pointer();
            return 0;
        }
    }

    for(int i=64;i<96;i++){
        if(fpng[i]!=origion[i]){
            use_uninitial_mem();
            return 0;
        }
    }

    for(int i=128;i<144;i++){
        if(fpng[i]!=origion[i]){
            out_of_bound();
            return 0;
        }
    }

    for(int i=172;i<188;i++){
        if(fpng[i]!=origion[i]){
            no_free();
            return 0;
        }
    }
    /*FILE *sf;
           sf = fopen("sample.png", "rb");
           int n;
           unsigned char buf[16];
           unsigned long count = 0;
           while((n = fread(buf, sizeof(char), 16, sf)) > 0) {
                   int i;
                   printf("%08lx ", count);
                   for(i=0; i<n; i++) {
                           printf("%02x ", (unsigned)buf[i]);
                   }

                   if(n < 16) {
                           for(i=n; i<16; i++) printf("    ");
                   }

                   for(i=0; i<n; i++) {
                           putchar(isprint(buf[i]) ? buf[i] : '.');
                   }

                   putchar('\n');

                   count += 16;

           }
           fclose(sf);*/
}
