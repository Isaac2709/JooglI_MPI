#include <iostream>
//#include <mpi.h>
// strings and c-strings
#include <cstring>
#include <string>
#include <windows.h>
#include <curl/curl.h>

using namespace std;

string tokens[100];


class Token {
    private:
        std::string strToken;

    public:
        Token *sig;
        Token *ant;
        Token(std::string strToken){
            this->strToken = strToken;
            sig = NULL;
            ant = NULL;
        }
        std::string getStrToken(){
            return strToken;
        }
        void setStrToken(std::string strToken){
            this->strToken = strToken;
        }
        void print(){
            cout<<"Token: "<<this->strToken<<endl;
        }
};

class Site{
    private:
        std::string name;
        std::string address;
        Token *listTokensMatches;
    public:
        Site *sig;
        Site *ant;
        Site(std::string name, std::string address){
            this->name = name;
            this->address = address;
            this->listTokensMatches = NULL;
            this->sig = NULL;
            this->ant = NULL;
        }
        std::string getName()
        {
            return name;
        }
        void setName(std::string name){
            this->name = name;
        }
        std::string getAddress()
        {
            return address;
        }
        void setAddress(std::string address){
            this->address = address;
        }
        void addToken(Token *token){
            if(this->listTokensMatches != NULL){
                Token *tempTokens  = listTokensMatches;
                for(tempTokens; tempTokens->sig != NULL; tempTokens = tempTokens->sig){}
                token->ant = tempTokens;
                tempTokens->sig = token;
            }
        }
        Token *getToken(int pos){
            if(this->listTokensMatches != NULL){
                Token *tempTokens  = listTokensMatches;
                int tempPos = 0;
                for(tempTokens; tempTokens->sig != NULL; tempTokens = tempTokens->sig){
                    if(pos == tempPos){
                        return tempTokens;
                    }
                }
                return NULL;
            }
        }
        void open(){
            string cmd="start " + this->address;
            system(cmd.c_str());
            //ShellExecute(NULL, "open", "http://google.com", NULL, NULL, SW_SHOWNORMAL);
        }
        void print(){
            cout<<"Nombre: "<<this->name<<"Dirección: "<<this->address<<endl;
        }
};

class arrayString{
    public:
        std::string *str[100];
        arrayString(){}
        arrayString(std::string strArray[100]){
            *this->str = strArray;
        }
        int arraySize(){
            int n = sizeof(str)/sizeof(str[0]);
            return n;
        }
};
// ----------------- END CLASS DECLARATIONS --------------------//
// INTERFACE
void printListTokens(Token *listTokens);
void printListSites(Site *listSites);

void searchEngineConsoleView(){
    Token *listTokensToSearch = NULL;
    string strSearch;
    std::getline(cin, strSearch);
    char * cstr = new char [strSearch.length()+1];
    std::strcpy(cstr, strSearch.c_str());
    // cstr now contains a c-string copy of str

    char * p = std::strtok (cstr," ");
    for(int i = 0; p!=0; i++)
    {
        Token *newToken = new Token(p);
        if(listTokensToSearch != NULL){
            listTokensToSearch->ant = newToken;
            newToken->sig = listTokensToSearch;
        }
        listTokensToSearch = newToken;
        //std::cout << p << '\n';
        p = std::strtok(NULL," ");
    }
    delete[] cstr;
    printListTokens(listTokensToSearch);
}

void printListTokens(Token *listTokens){
    int i = 0;
    for(Token *tempListTokens = listTokens; tempListTokens != NULL; tempListTokens = tempListTokens->sig){
        cout<<i<<" | ";
        tempListTokens->print();
        i++;
    }
}

void printListSites(Site *listSites){
    for(Site *tempSites = listSites; tempSites!=NULL;tempSites = tempSites->sig){
        tempSites->print();
    }
}


int main(int argc, char **argv)
{

    int pid, nprocs;
    /*MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    //
    //  Have Process 0 say hello.
    //
    printf("Soy el procesador %d de un total de %d\n",pid,nprocs);*/


    Site *listSites;
    Site *google = new Site("Google", "google.com");
    listSites = google;
    Site *facebook = new Site("Facebook", "http://facebook.com");
    listSites->sig = facebook;

    searchEngineConsoleView();
    /*tipoToken *listTokensMatches = NULL;
    tipoToken *tempTokens  = listTokensMatches;*/
    cout << "Hello world!" << endl;
    //MPI_Finalize();
    return 0;
}
