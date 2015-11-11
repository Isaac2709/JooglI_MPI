#include <iostream>
#include <fstream>
// strings and c-strings
#include <cstring>
#include <string>
#include <algorithm>
#include <cctype>
//#include <mpi.h>
//#include <windows.h>
//#include <curl/curl.h>


using namespace std;

string tokens[100];
string data; //will hold the url's contents

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
        std::string body;
        std::string address;
        Token *listTokensMatches;
    public:
        Site *sig;
        Site *ant;
        Site(std::string name, std::string address){
            this->name = name;
            this->address = address;
            this->body = "";
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
        std::string getAddress(){
            return address;
        }
        void setAddress(std::string address){
            this->address = address;
        }
        std::string getBody(){
            return body;
        }
        void setBody(std::string body){
            this->body = body;
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
        Token *getListTokensMatches(){
            return this->listTokensMatches;
        }
        void open(){
            string cmd="start " + this->address;
            system(cmd.c_str());
            //ShellExecute(NULL, "open", "http://google.com", NULL, NULL, SW_SHOWNORMAL);
        }
        void print(){
            cout<<"Nombre: "<<this->name<<" | Direccion: "<<this->address<<endl;
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
void searchEngineConsoleView();
Site *addSiteToList(Site *listSites, Site *site);
Site *loadWEBSites(Site *listSites);
Site *searchEngine(Site *listSites);
Site *searchByWebSite(Site *site, Token *listTokens);
Site *addSiteToList(Site *listSites, Site *site);
size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up);
std::string connectSite(std::string address);
std::string getBody(std::string htmlContent);

////

/*static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}*/

// ----------------- GET SOURCE CODE FROM THE WEB SITES --------------------//
/*size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{ //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer

    for (int c = 0; c<size*nmemb; c++)
    {
        data.push_back(buf[c]);
    }
    return size*nmemb; //tell curl how many bytes we handled
}

std::string getBody(std::string htmlContent){
    std::string startBody ("<body");
    std::string endBody ("</body>");
    std::size_t foundStartBody = htmlContent.find(startBody);
    std::size_t foundEndBody = htmlContent.find(endBody);
    if (foundStartBody!=std::string::npos && foundEndBody!=std::string::npos){
        htmlContent = htmlContent.substr(foundStartBody + 5);
        std::string str(">");
        std::size_t found = htmlContent.find(str);
        if(found!=std::string::npos){
            htmlContent = htmlContent.substr(found + 1);
        }
        foundEndBody = htmlContent.find(endBody);
        htmlContent = htmlContent.substr(0, foundEndBody);
    }

    std::string str2 ("<");
    std::string str3 (">");
    int i = 1000;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       ;
    // different member versions of find in the same order as above:
    std::size_t found = htmlContent.find(str2);
    std::size_t found2;

    while(found!=std::string::npos){
        found2 = htmlContent.find(str3);
        if (found2!=std::string::npos){
            //std::cout << "Pos <: "<<found << " | Pos >: "<<found2<<htmlContent.substr(found, (found2 - found) +1) << '\n';
            htmlContent = htmlContent.substr(0, found) + htmlContent.substr(found2+1);
            //std::cout <<"Resultado: "<< htmlContent << '\n';
        }
        else{
            std::cout << htmlContent << '\n';
        }
        if(i == 0)
            break;
        found = htmlContent.find(str2);
        i = i-1;
    }
    return htmlContent;
}

std::string connectSite(std::string address)
{
    CURL* curl; //our curl object

    curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, address.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress

    curl_easy_perform(curl);

    //cout << endl << data << endl;
    std::string readBuffer = getBody(data);
    // Delete empty spaces
    std::string::iterator end_pos = std::remove(readBuffer.begin(), readBuffer.end(), ' ');
    readBuffer.erase(end_pos, readBuffer.end());

    std::cout << readBuffer << std::endl;

    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return readBuffer;
}*/
// ----------------- END GET SOURCE CODE FROM THE WEB SITES --------------------//

// ----------------- SEARCH ENGINE CONSOLE VIEW --------------------//
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
    Site *listSites = loadWEBSites(NULL);
    //printListTokens(listTokensToSearch);
}
// ----------------- END SEARCH ENGINE CONSOLE VIEW --------------------//

// ----------------- METHODS TO PRINT LIST --------------------//
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
// ----------------- END METHODS TO PRINT LIST --------------------//

// ----------------- METHOD TO ADD WEB SITE TO LIST --------------------//
Site *addSiteToList(Site *listSites, Site *site){
    if(listSites == NULL){
        listSites = site;
    }
    else{
        Site *tempListSites = listSites;
        for(tempListSites; tempListSites->sig != NULL; tempListSites = tempListSites->sig){}
        if(tempListSites!=NULL && tempListSites->sig == NULL){
            site->ant = tempListSites;
            tempListSites->sig = site;
        }
    }
    return listSites;
}
// ----------------- END METHOD TO ADD WEB SITE TO LIST --------------------//

// ----------------- METHODS TO LOAD WEB SITES ADDRESS FROM A TEXT FILE --------------------//
Site *loadWEBSites(Site *listSites){
    //char cadena[128];
    // Crea un fichero de salida
    //ofstream fs("WEBSites.txt");

    // Enviamos una cadena al fichero de salida:
    //fs << "Hola, mundo" << endl;
    // Cerrar el fichero,
    // para luego poder abrirlo para lectura:
    //fs.close();

    // Abre un fichero de entrada
    //ifstream fe("WEBSites.txt");
    //char cadena[128];
    std::string cadena;
    ifstream fe("WEBSites.txt");
    std::string name, address;
    std::string separator ("|");
    while(!fe.eof()) {
        fe >> cadena;
        std::size_t foundSeparator= cadena.find(separator);
        if (foundSeparator!=std::string::npos){
            name = cadena.substr(0, foundSeparator);
            address = cadena.substr(foundSeparator+1);
            Site *tempSite = new Site(name, address);
            tempSite->setBody(connectSite(address));
            listSites = addSiteToList(listSites, tempSite);
        }

      //cout << "Nombre: "<<name <<" | Dirección: "<<address<< endl;
    }
    fe.close();
    return listSites;
    // Leeremos mediante getline, si lo hiciéramos
    // mediante el operador << sólo leeríamos
    // parte de la cadena:
    //fe.getline(cadena, 128);

    //cout << cadena << endl;
}
// ----------------- METHODS TO LOAD WEB SITES ADDRESS FROM A TEXT FILE --------------------//

Site *searchEngine(Site *listSites, Token *listTokens){
    bool match;Site *listMatchSites;
    for(Site *tempSite = listSites; tempSite != NULL; tempSite = tempSite->sig){
        tempSite = searchByWebSite(tempSite, listTokens);
        if(tempSite->getListTokensMatches() != NULL)
        {
            listMatchSites = addSiteToList(listMatchSites, tempSite);
        }
    }
    printListSites(listMatchSites);
}

Site *searchByWebSite(Site *site, Token *listTokens){
    bool match;

    for(Token *tempToken = listTokens; tempToken != NULL; tempToken = tempToken->sig){
        string strToken = tempToken->getStrToken();
        string strTitle = site->getName();
        string strBody = site->getName();
        for(int n = 0; n < strTitle.length(); n++){
            if(strTitle[n] == strToken[0]){
                int tempN = n; match = true;
                for(int m = 0; m < strToken.length(); m++){
                    if(tempN >= strTitle.length() || strTitle[tempN] != strToken[m]){
                        match = false;
                        break;
                    }
                    tempN = tempN + 1;
                }
                if(match){
                    site->addToken(tempToken);
                }
            }
        }
    }
    return site;
}
// ----------------- MAIN --------------------//
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


    Site *listSites = loadWEBSites(NULL);
    /*Site *google = new Site("Google", "google.com");
    listSites = google;
    Site *facebook = new Site("Facebook", "http://facebook.com");
    listSites->sig = facebook;*/
    //facebook->open();
    //connectSite(google->getAddress());

    printListSites(listSites);
    searchEngineConsoleView();
    /*tipoToken *listTokensMatches = NULL;
    tipoToken *tempTokens  = listTokensMatches;*/
    cout << "End program!" << endl;
    //MPI_Finalize();
    return 0;
}
// ----------------- END MAIN --------------------//
