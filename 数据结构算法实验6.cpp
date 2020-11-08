#include<iostream>
#include<string.h>
#include<stdio.h>
#include<fstream>
#include<math.h>
using namespace std;

struct textMark
{
    char data;
    int weight;
};
typedef textMark TM;
typedef textMark TMptr;

struct WeightList
{
    TM *List;
    int length;
    string filename;
};


typedef struct
{
    char data,*code;
    int lchild,rchild,parent,weight;
}HTNode,*HuffmanTree;

WeightList creatWeightList()
{
    string filename="";
    cout<<"请输入需要加密的文件"<<endl;
    cin>>filename;
    ifstream rf(filename.c_str());
    char temp[1024]={0};
    WeightList wl;
    if(!rf.is_open()){
        wl.length=-1;
        cout<<"没有此文件"<<endl;
        return wl;
    }
    TM tma[1024];
    int tma_length=0;
    rf.getline(temp,1024);
    for(int i=0;i<strlen(temp);i++){
        if(tma_length==0){
            tma[0].data=temp[0];
            tma[0].weight++;
            tma_length++;
            continue;
        }
        for(int j=0;j<tma_length;j++){
            if(tma[j].data==temp[i]){
                tma[j].weight++;
                break;
            }
            if(j==tma_length-1){
                tma[tma_length].data=temp[i];
                tma[tma_length].weight++;
                tma_length++;
                break;
            }
        }
    }
    wl.List=tma;
    wl.length=tma_length;
    wl.filename=filename;
    rf.close();
    return wl;
}

void Select(HuffmanTree HT,int i,int &s1,int &s2)
{
    s1=0;
    s2=0;
    int sm1=20000;
    int sm2=20000;
    for(int j=0;j<i;j++){
        if(HT[j].parent==0){
            if(HT[j].weight<sm1){
                sm2=sm1;
                sm1=HT[j].weight;
                s2=s1;
                s1=j;
            }
            else if(HT[j].weight<sm2){
                sm2=HT[j].weight;
                s2=j;
            }
        }
    }
}

void CreatHuffmanTree(HuffmanTree &HT,int n,WeightList wl)
{
    int s1,s2;
    if(n<=1) return;
    int m=2*n-1;
    HT=new HTNode[m];
    for(int i=0;i<m;i++){
        HT[i].parent=0;
        HT[i].lchild=0;
        HT[i].rchild=0;
        HT[i].weight=0;
    }
    for(int i=0;i<n;i++){
        HT[i].data=wl.List[i].data;
        HT[i].weight=wl.List[i].weight;
    }
    for(int i=n;i<m;i++){
        Select(HT,i,s1,s2);
        HT[s1].parent=i;
        HT[s2].parent=i;
        HT[i].lchild=s1;
        HT[i].rchild=s2;
        HT[i].weight=HT[s1].weight+HT[s2].weight;
    }
}

string CreatHuffmanCode(HuffmanTree &HT,int n,string filename)
{
    int pointer,p,m;
    char *code=new char[n];
    code[n-1]='\0';
    for(int i=0;i<n;i++){
        pointer=n-1;
        m=i;
        p=HT[i].parent;
        while(p!=0){
            pointer--;
            if(HT[p].lchild==m) code[pointer]='0';
            else code[pointer]='1';
            m=p;
            p=HT[p].parent;
        }
        HT[i].code=new char[n-pointer];
        strcpy(HT[i].code,&code[pointer]);
        cout<<HT[i].data<<":"<<HT[i].code<<endl;
    }
    ifstream is(filename.c_str());
    char temp[1024]={0};
    is.getline(temp,1024);
    char encryption[1024]={0};
    for(int i=0;i<strlen(temp);i++){
        int j=0;
        while(HT[j].data!=temp[i]){
            j++;
        }
        strcat(encryption,HT[j].code);
    }
    string encryptionFile;
    cout<<"请输入保存编码文件名"<<endl;
    cin>>encryptionFile;
    ofstream os(encryptionFile.c_str(),ios::out);
    for(int i=0;i<strlen(encryption);i++){
        os<<encryption[i];
    }
    os.close();
}

void decoding(HuffmanTree HT,int n)
{
    string encryptionFile,encode;
    cout<<"请输入需要解码的文件"<<endl;
    cin>>encryptionFile;
    cout<<"请输入解码后的文件"<<endl;
    cin>>encode;
    ifstream is(encryptionFile.c_str());
    ofstream os(encode.c_str(),ios::out);
    char c;
    char decode[1024];
    char temp[1024]={0};
    int tempLength=0;
    int pointer=2*n-2;
    while(is.get(c)){
        if(c=='0'){
            pointer=HT[pointer].lchild;
        }
        else{
            pointer=HT[pointer].rchild;
        }
        if(HT[pointer].lchild == 0 && HT[pointer].rchild == 0){
            os<<HT[pointer].data;
            pointer=2*n-2;
        }
    }
    os.close();
    is.close();
}

void zip()
{
    int sum=0;
    char temp[10];
    int zip[1000];
    int length=0;
    int lengtht;
    int flag=0;
    ifstream is("security.cod");
    while(!is.eof()){
        is.get(temp,9);
        for(int i=0;i<8;i++){
            if(temp[i]==0){
                flag=i;
                break;
            }
        }
        cout<<flag<<endl;
        cout<<(int)temp[1]<<"!!!!!"<<endl;
        if(flag!=0) break;
        for(int i=7;i>=0;i--){
            cout<<temp[7-i]<<endl;
            if(temp[7-i]=='1'){
                sum=sum+pow(2,i);
                cout<<sum<<endl;
            }
        }
        zip[length]=sum;
        length++;
        sum=0;
        cout<<"!!"<<endl;
    }
    if(flag!=0){
        for(int i=0;i<flag;i++){
            if(temp[flag-i-1]=='1')
            sum=sum+pow(2,i);
        }
        zip[length]=sum;
        length++;
    }
    for(int j=0;j<length;j++){
        cout<<zip[j];
    }
    ofstream os("zip.txt");
    for(int i=0;i<length;i++){
        os<<zip[i];
        os<<'/';
    }
    os.close();
}

int main()
{
    WeightList wl;
    HuffmanTree HT;
    int choice=0;
    while(choice!=-1){
        cout<<"********************************************"<<endl;
        cout<<"*             哈夫曼编码译码器             *"<<endl;
        cout<<"*                                          *"<<endl;
        cout<<"*  1.选择需要进行编码的文件                *"<<endl;
        cout<<"*  2.建立哈夫曼树                          *"<<endl;
        cout<<"*  3.建立密码本并对文件编码                *"<<endl;
        cout<<"*  4.选择需要进行解码的文件并解码          *"<<endl;
        cout<<"*  5.按位压缩方式对文件进行压缩            *"<<endl;
        cout<<"*  0.退出                                  *"<<endl;
        cout<<"*                                          *"<<endl;
        cout<<"********************************************"<<endl;
        cout<<"请输入要进行的操作"<<endl;
        cin>>choice;
        if(choice==1) wl=creatWeightList();
        else if(choice==2) CreatHuffmanTree(HT,wl.length,wl);
        else if(choice==3) CreatHuffmanCode(HT,wl.length,wl.filename);
        else if(choice==4) decoding(HT,wl.length);
        else if(choice==5) zip();
        else if(choice==0) break;
        else continue;
    }
}
