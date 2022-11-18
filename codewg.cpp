#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstring>
#define MX_KWD_L 9
using namespace std;

class SFnode {
    public:
        SFnode *left,*right;
        int weight;
        int sIndex;
        SFnode(){
            left=right=nullptr;
            weight=-1;
            sIndex=-1;
        }
        SFnode(int f,int si,SFnode *l=nullptr,SFnode *r=nullptr){
            weight=f;
            left=l;
            right=r;
            sIndex=si;
        }
        bool isLeaf(){
            return left==nullptr and right==nullptr ;
        }
};

typedef struct{
    char (*keywords)[MX_KWD_L];
    int *freq;
    int nOr;
} container;

int gSum(int *freq,int ll,int ul){
    int sum=0,i;
    for(i=ll;i<=ul;i++){
        sum+= *(freq+i);
    }
    return sum;
}

int gDiff(int *freq,int part,int ll,int ul){
    int i,sum=0;
    for(i=ll;i<=ul;i++){
        if(i<=part){
            sum+= *(freq+i);
        }
        else{
            sum-= *(freq+i);
        }
    }
    return (sum<0) ? sum*(-1) : sum;
}

int gPart(int *freq,int ll,int ul){
    int o_part=ll,
        part=ll,
        n_diff=gDiff(freq,part,ll,ul);
    int diff=n_diff;
    for(o_part=ll+1; o_part<ul; o_part++){
        n_diff=gDiff(freq,o_part,ll,ul);
        if(n_diff<diff){
            diff=n_diff;
            part=o_part;
        }
    }
    return part;
}

SFnode* gTree(int *freq,int ll,int ul){
    int part,sum1,sum2;
    if(ll==ul){
        return new SFnode(freq[ll],ll);
    }
    else{
        SFnode *tmp=new SFnode();
        part=gPart(freq,ll,ul);
        sum1=gSum(freq,ll,part);
        sum2=gSum(freq,part+1,ul);
        if(sum1<sum2){
            tmp->left=gTree(freq,ll,part);
            tmp->right=gTree(freq,part+1,ul);
        }
        else{
            tmp->right=gTree(freq,ll,part);
            tmp->left=gTree(freq,part+1,ul);
        }
        return tmp;
    }
}

void writeCWords(SFnode *T,char (*kws)[MX_KWD_L],ostream &out,string pcode=""){
    if(T!=nullptr){
        if(T->isLeaf()){
            out<<*(kws+T->sIndex)<<" "<<pcode<<endl;
            cout<<*(kws+T->sIndex)<<" - "<<pcode<<endl;
        }
        writeCWords(T->left,kws,out,pcode+"0");
        writeCWords(T->right,kws,out,pcode+"1");
    }
}

void writeCodeFile(SFnode *T,char (*kws)[MX_KWD_L],int N){
    ofstream codeFile("prefixCodeTable.txt");
    codeFile<<N<<endl;
    writeCWords(T,kws,codeFile);
    codeFile.close();
}

bool readFreqFile(container *fTAB) {
    ifstream codetab("freq_tab.txt");
    string line;
    int i=0;
    if(codetab.fail()) {
        cout<<"\tERROR :: Unable To Read Frequency File!! [freq_tab.txt]\n";
        return false;
    } else {
        codetab>>fTAB->nOr;
        fTAB->freq=new int[fTAB->nOr];
        fTAB->keywords=new char[fTAB->nOr][MX_KWD_L];
        while(codetab) {
            getline(codetab,line);
            if(sscanf(line.c_str(),"%s %d",(fTAB->keywords)[i],(fTAB->freq)+i) > 0) {
                i++;
            }
        }
        codetab.close();
    }
    return true;
}

/* int QSgPart(int *a,char (*kws)[MX_KWD_L],int lb,int ub){
    int pivot=a[lb],
        start=lb,
        end=ub;
    while(start<end){
        while(a[start]>=pivot)
            start++;
        while(a[end]<pivot)
            end--;
        if(start<end){
            swap(a[start],a[end]);
            swap(kws[start],kws[end]);
        }
    }
    swap(a[lb],a[end]);
    swap(kws[lb],kws[end]);
    return end;
} */

int QSgPart(int *arr, char (*kws)[MX_KWD_L], int low, int high){
    int pivot=arr[high];
    int i=(low-1);
    for(int j=low; j<=high-1; j++){
        if(arr[j] > pivot){
            i++;
            swap(arr[i],arr[j]);
            swap(kws[i],kws[j]);
        }
    }
    swap(arr[i+1],arr[high]);
    swap(kws[i+1],kws[high]);
    return(i+1);
}

void quicksort(int *a,char (*kws)[MX_KWD_L],int lb,int ub){
    if(lb<ub){
        int loc=QSgPart(a,kws,lb,ub);
        quicksort(a,kws,lb,loc-1);
        quicksort(a,kws,loc+1,ub);
    }
}

int main(){
    container freqTable;
    SFnode *SFtree;
    if(readFreqFile(&freqTable)){
        quicksort(freqTable.freq, freqTable.keywords, 0, freqTable.nOr-1);
        /* for(int i=0;i<freqTable.nOr;i++){
            cout<<freqTable.keywords[i]<<" - "<<freqTable.freq[i]<<endl;
        } */
        SFtree=gTree(freqTable.freq,0,freqTable.nOr-1);
        //writeCWords(SFtree, freqTable.keywords, cout);
        writeCodeFile(SFtree, freqTable.keywords, freqTable.nOr);
    }
    return 0;
}
