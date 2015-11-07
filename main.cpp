#include <iostream>

using namespace std;


typedef struct token {
   std::string strToken;
   struct token *sig;
   struct token *ant;
   token(std::string strToken){
       this->strToken = strToken;
       sig = NULL;
       ant = NULL;
   }
} tipoToken;

class Site{
    private:
        std::string name;
        std::string address;
        tipoToken *listTokensMatches;
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
        void addToken(tipoToken *token){
            if(this->listTokensMatches != NULL){
                tipoToken *tempTokens  = listTokensMatches;
                for(tempTokens; tempTokens->sig != NULL; tempTokens = tempTokens->sig){}
                token->ant = tempTokens;
                tempTokens->sig = token;
            }
        }
        tipoToken *getToken(int pos){
            if(this->listTokensMatches != NULL){
                tipoToken *tempTokens  = listTokensMatches;
                int tempPos = 0;
                for(tempTokens; tempTokens->sig != NULL; tempTokens = tempTokens->sig){
                    if(pos == tempPos){
                        return tempTokens;
                    }
                }
                return NULL;
            }
        }
        void print(){
            cout<<"Nombre: "<<this->name<<"Dirección: "<<this->address<<endl;
        }
};

void printListSites(Site *listSites){
    for(Site *tempSites = listSites; tempSites!=NULL;tempSites = tempSites->sig){
        tempSites->print();
    }
}

int main()
{
    Site *listSites;
    Site *google = new Site("Google", "google.com");
    listSites = google;
    Site *facebook = new Site("Facebook", "facebook.com");
    listSites->sig = facebook;

    /*tipoToken *listTokensMatches = NULL;
    tipoToken *tempTokens  = listTokensMatches;*/
    cout << "Hello world!" << endl;
    return 0;
}
