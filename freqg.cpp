#include<iostream>
#include<fstream>
using namespace std;

/* Takes a string & a pattern, returns number of occurrence of the pattern on that string.
 * Arguments:
 * const char *s : A string
 * const char *p : A pattern
 * Ex: strMatchCount("abcabcababcdef","abc") returns 3.
 *                    ------  ---
 */
int strMatchCount(const char *s,const char *p){
    const char *q=p;
    int c=0;
    while(*s!='\0'){
        if(*s==*q){
            q++;
            if(*q=='\0'){
                c++;
                q=p;
            }
        }
        else{
            q=p;
        }
        s++;
    }
    return c;
}

int main(int argc,char *argv[]){
    if(argc==1){
        cout<<"\t:::: Frequency Generator ::::\nUsage: ./freqg [FILE-1] [FILE-2] . . . [FILE-N]\n";
        exit(0);
    }

    char keywords[32][9]={"auto","break","case","char",
                        "const","continue","default","do",
                        "double","else","enum","extern",
                        "float","for","goto","if",
                        "int","long","register","return",
                        "short","signed","sizeof","static",
                        "struct","switch","typedef","union",
                        "unsigned","void","volatile","while"};
    int freq[32];
    ifstream source;
    ofstream freqtab;
    string line;
    int i,a;
    for(i=0;i<32;i++){
        freq[i]=0;
    }
    for(a=1;a<argc;a++){
        source.open(argv[a]);
        if(source.fail()){
            cout<<"\tERROR :: Unable To Read File!! ["<<argv[a]<<"]\n";
        }
        else{
            while(source){
                getline(source,line);
                for(i=0;i<32;i++){
                    freq[i]+=strMatchCount(line.c_str(),keywords[i]);
                }
            }
            source.close();
        }
    }
    freqtab.open("freq_tab.txt");
    freqtab<<32<<endl;
    for(i=0;i<32;i++){
        cout<<keywords[i]<<" - "<<freq[i]<<endl;
        freqtab<<keywords[i]<<" "<<freq[i]<<endl;
    }
    freqtab.close();
    return 0;
}