#include<iostream>
#include<fstream>
#include<string>
#include<cmath>
#define MX_SMBL_L 10
#define MX_CODW_L 30
using namespace std;

typedef struct{
    char (*symbols)[MX_SMBL_L];
    char (*codewords)[MX_CODW_L];
    int nOr;
} container;

bool readCodeFile(container *TAB) {
    ifstream codetab("prefixCodeTable.txt");
    string line;
    int i=0;
    if(codetab.fail()) {
        cout<<"\tERROR :: Unable To Read Codeword File!! [prefixCodeTable.txt]\n";
        return false;
    } else {
        i=0;
        codetab>>TAB->nOr;
        TAB->symbols=new char[TAB->nOr][MX_SMBL_L];
        TAB->codewords=new char[TAB->nOr][MX_CODW_L];
        while(codetab) {
            getline(codetab,line);
            if(sscanf(line.c_str(),"%s %s",(TAB->symbols)[i],(TAB->codewords)[i]) > 0) {
                i++;
            }
        }
        codetab.close();
    }
    return true;
}

void encode(const char *iFile,
            const char *oFile,
            char (*symbols)[MX_SMBL_L],
            char (*codewords)[MX_CODW_L],
            int nOr){
    ifstream source(iFile);
    ofstream outf(oFile,ios::binary);
    char chr;
    int i,j=0;
    bool indicator[nOr],F=false;
    string packable;
    for(i=0;i<nOr;i++){
        indicator[i]=true;
    }

    if(source.fail()) {
        cout<<"\tERROR :: Unable To Read Source File!! ["<<iFile<<"]\n";
        return;
    }
    while(source and source>>noskipws>>chr){
        for(i=0;i<nOr;i++){
            if(chr==symbols[i][j] and indicator[i]==true){
                indicator[i]=true;
            }
            else{
                indicator[i]=false;
            }
            F = F or indicator[i];
            if(symbols[i][j+1]=='\0' and indicator[i]==true){ //one symbol successfully matched
                //outf<<codewords[i];
                packable+=codewords[i];
                F=false;
            }
        }
        /*column indexing mechanism*/
        if(F){ // atleast one symbol has possibility to match
            j++;
            F=false;
        }
        else{ //no possibility of matching further, resetting column indicator-j and indicator array.
            j=0;
            for(i=0;i<nOr;i++){
                indicator[i]=true;
            }
        }
        //j%=MX_SMBL_L;

        
    }
    /*bit packing mechanism*/
    while(packable.length()>=8){
        int x;
        char c=0;
        for(x=0;x<8;x++){
            c=c+packable[x]*pow(2,x);
            cout<<packable[x];
        }
        outf<<c;
        cout<<"["<<c<<"]"<<endl;
        packable=packable.substr(8);
    }
    while(packable.length()<8)
        packable+= packable[packable.length()-1]==0 ? '1' : '0';
    int x;
    char c=0;
    for(x=0;x<8;x++){
        c=c+packable[x]*pow(2,x);
        cout<<packable[x];
    }
    outf<<c;
    cout<<"["<<c<<"]"<<endl;
}

int main(int argc,char *argv[]){
    if(argc==1){
        cout<<"\t:::: Encoder ::::\nUsage: ./encoder [INPUT-FILE-NAME] [OUTPUT-FILE-NAME]\n";
        exit(0);
    }
    container codeTable;
    if(readCodeFile(&codeTable)){
        /* for(int i=0;i<codeTable.nOr;i++){
            cout<<codeTable.symbols[i]<<" = "<<codeTable.codewords[i]<<endl;
        } */
        encode(argv[1], argv[2], codeTable.symbols, codeTable.codewords, codeTable.nOr);
    }
    return 0;
}