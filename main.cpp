#include <iostream>
//#include <mpi.h>

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
    Site *facebook = new Site("Facebook", "facebook.com");
    listSites->sig = facebook;

    /*tipoToken *listTokensMatches = NULL;
    tipoToken *tempTokens  = listTokensMatches;*/
    cout << "Hello world!" << endl;
    //MPI_Finalize();
    return 0;
}
