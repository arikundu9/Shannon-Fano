#include<iostream>
#include<fstream>
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

void decode(const char *iFile,
            const char *oFile,
            char (*symbols)[MX_SMBL_L],
            char (*codewords)[MX_CODW_L],
            int nOr){
    ifstream source(iFile);
    ofstream outf(oFile,ios::binary);
    char chr;
    int i,j=0;
    bool indicator[nOr],F=false;
    for(i=0;i<nOr;i++){
        indicator[i]=true;
    }
    
    if(source.fail()) {
        cout<<"\tERROR :: Unable To Read Source File!! ["<<iFile<<"]\n";
        return;
    }
    while(source and source>>noskipws>>chr){
        for(i=0;i<nOr;i++){
            if(chr==codewords[i][j] and indicator[i]==true){
                indicator[i]=true;
            }
            else{
                indicator[i]=false;
            }
            F = F or indicator[i];
            if(codewords[i][j+1]=='\0' and indicator[i]==true){
                outf<<symbols[i];
                F=false;
            }
        }
        if(F){
            j++;
            F=false;
        }
        else{
            j=0;
            for(i=0;i<nOr;i++){
                indicator[i]=true;
            }
        }
        //j%=MX_SMBL_L;
    }
}

int main(int argc,char *argv[]){
    if(argc==1){
        cout<<"\t:::: Prefix Decoder ::::\nUsage: ./decoder [INPUT-FILE-NAME] [OUTPUT-FILE-NAME]\n";
        exit(0);
    }
    container codeTable;
    if(readCodeFile(&codeTable)){
        /* for(int i=0;i<codeTable.nOr;i++){
            cout<<codeTable.symbols[i]<<" = "<<codeTable.codewords[i]<<endl;
        } */
        decode(argv[1], argv[2], codeTable.symbols, codeTable.codewords, codeTable.nOr);
    } 
    return 0;
}