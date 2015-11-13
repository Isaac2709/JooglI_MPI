#include <iostream>
#include <fstream>
// strings and c-strings
#include <cstring>
#include <string>
#include <algorithm>
#include <cctype>
#include <mpi.h>
#include <unistd.h>
//#include <windows.h>
#include <curl/curl.h>


using namespace std;

string tokens[100];
int flag = 1;
//string data; //will hold the url's contents

class Token {
    private:
        std::string strToken;
        int intMatches;
    public:
        Token *sig;
        Token *ant;
        Token(std::string strToken){
            this->strToken = strToken;
            this->intMatches = 0;
            sig = NULL;
            ant = NULL;
        }
        Token(std::string strToken, int intMatches){
            this->strToken = strToken;
            this->intMatches = intMatches;
            sig = NULL;
            ant = NULL;
        }
        std::string getStrToken(){
            return strToken;
        }
        void setStrToken(std::string strToken){
            this->strToken = strToken;
        }
        int getIntMatches(){
            return intMatches;
        }
        void setIntMatches(int intMatches){
            this->intMatches = intMatches;
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
        std::string summary;
        Token *listTokensMatches;
    public:
        Site *sig;
        Site *ant;
        Site(std::string name, std::string address){
            this->name = name;
            this->address = address;
            this->body = "";
            this->summary = "";
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
        std::string getSummary()
        {
            return summary;
        }
        void setSummary(std::string summary){
            this->summary = summary;
        }
        int getSizeListTokens(){
            int intSize = 0;
            for(Token *tempTokens  = listTokensMatches; tempTokens != NULL; tempTokens = tempTokens->sig){
                intSize++;
            }
            return intSize;
        }
        void addToken(Token *token){
            if(this->listTokensMatches != NULL){
                Token *tempTokens  = listTokensMatches;
                for(tempTokens; tempTokens->sig != NULL; tempTokens = tempTokens->sig){}
                token->ant = tempTokens;
                tempTokens->sig = token;
            }
            else{
                listTokensMatches = token;
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
        void setListTokensMatches(Token *listTokensMatches){
            this->listTokensMatches = listTokensMatches;
        }
        void open(){
            string cmd="start " + this->address;
            system(cmd.c_str());
            //ShellExecute(NULL, "open", "http://google.com", NULL, NULL, SW_SHOWNORMAL);
        }
        void print(){
            cout<<"Nombre: "<<this->name<<" | Direccion: "<<this->address<<endl;
        }
        void printAll(){
            cout<<"-------------------------------------------------------"<<endl;
            cout<<"Nombre: "<<this->name<<" | Direccion: "<<this->address<<endl;
            for(Token *tempToken  = listTokensMatches; tempToken != NULL; tempToken = tempToken->sig){
                cout<<"Token: "<<tempToken->getStrToken()<< " | Apariciones: "<<tempToken->getIntMatches()<<endl;
            }
            cout<<"-------------------------------------------------------"<<endl;
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
void printListSites(Site *listSites, int optionToPrint);
Token *getListTokensToSearch(string strSearch);
Site *addSiteToList(Site *listSites, Site *site);
Site *loadWEBSites(Site *listSites);
Site *searchEngine(Site *listSites,  Token *listTokens);
Site *searchByWebSite(Site *site, Token *listTokens);
Site *addSiteToList(Site *listSites, Site *site);
size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up);
std::string connectSite(std::string address);
std::string getBody(std::string htmlContent);
void menu(int pid);
void orderListSitesAscending(Site *listSize);
int getSizeOfListSites(Site *listSites);
void printArraySites(Site *arraySites[], int sizeArray);
// END INTERFACE

// ----------------- GET SOURCE CODE FROM THE WEB SITES --------------------//
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
    char *a = (char*)contents;
    char b = ' ';
    /*if(strcmp(a, b)){
        cout<<"Encontro";
    }*/
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string connectSite(std::string address){
    std::string readBuffer;
    CURL *curl = NULL;
    CURLcode res;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, address.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        readBuffer = getBody(readBuffer);

        std::string::iterator end_pos = std::remove(readBuffer.begin(), readBuffer.end(), ' ');
        readBuffer.erase(end_pos, readBuffer.end());

    }
    return readBuffer;
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
            break;
        }
        if(i == 0)
            break;
        found = htmlContent.find(str2);
        i = i-1;
    }
    return htmlContent;
}
// ----------------- END GET SOURCE CODE FROM THE WEB SITES --------------------//

// ----------------- SEARCH ENGINE CONSOLE VIEW --------------------//
Token *getListTokensToSearch(string strSearch){
    Token *listTokensToSearch = NULL;
    Site *listMatchSites = NULL;
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
    //Site *listSites = loadWEBSites(NULL);
    return listTokensToSearch;
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

void printListSites(Site *listSites, int option){
    int c = 0;
    for(Site *tempSites = listSites; tempSites!=NULL;tempSites = tempSites->sig){
        if(option == 0){
            cout<<c<<"- ";
            tempSites->print();
            c++;
        }
        else if(option == 1){
            tempSites->printAll();
        }
    }
}

void printArraySites(Site *arraySites[], int sizeArray){
    for(int pos = 0; pos < sizeArray; pos ++){
        arraySites[pos]->printAll();
    }
}
// ----------------- END METHODS TO PRINT LIST --------------------//

// ----------------- METHOD TO GET THE SIZE OF THE LIST SITES --------------------//
int getSizeOfListSites(Site *listSites){
    int intSize = 0;
    for(Site *tempSites = listSites; tempSites!=NULL;tempSites = tempSites->sig){
        intSize++;
    }
    return intSize;
}
// ----------------- END METHOD TO GET THE SIZE OF THE LIST SITES --------------------//

// ----------------- METHOD TO GET THE SIZE OF THE LIST SITES --------------------//
int getSizeOfListTokens(Token *listTokes){
    int intSize = 0;
    for(Token *tempTokens = listTokes; tempTokens!=NULL;tempTokens = tempTokens->sig){
        intSize++;
    }
    return intSize;
}
// ----------------- END METHOD TO GET THE SIZE OF THE LIST SITES --------------------//

// ----------------- METHOD TO ORDER THE LIST IN ASCENDING --------------------//
void orderListSitesAscending(Site *listSites){
    // Convert from a list to Array of web sites
    Site *temp;
    int Nelementos = getSizeOfListSites(listSites);
    Site *arraySites[Nelementos]; int i = 0;
    for(Site *tempSite = listSites; tempSite!=NULL; tempSite = tempSite->sig){
        temp = new Site(tempSite->getName(), tempSite->getAddress());
        temp->setListTokensMatches(tempSite->getListTokensMatches());
        arraySites[i] = temp;
        i++;
    }
    // END listToArray

    int j;                //Variables contadoras del ciclo.
    temp=NULL;             //Variable temporal.

    for (i=1;i<Nelementos;i++)
    {
       for (j=0; j < Nelementos-1 ;j++)
       {
          if (arraySites[j]->getSizeListTokens() < arraySites[j+1]->getSizeListTokens())//Condicion mayor-menor
          {
            temp=arraySites[j];
            arraySites[j]=arraySites[j+1];
            arraySites[j+1]=temp;
          }
       }
    }
    printArraySites(arraySites, Nelementos);
}
// ----------------- END METHOD TO ORDER THE LIST IN ASCENDING --------------------//

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
    bool match;Site *listMatchSites = NULL;
    Site *tempSiteToAdd;
    for(Site *tempSite = listSites; tempSite != NULL; tempSite = tempSite->sig){
        tempSiteToAdd = searchByWebSite(tempSite, listTokens);
        if(tempSiteToAdd->getListTokensMatches() != NULL)
        {
            //cout<<"SE ENCONTRARON VARIOS TOKENS en:"<<endl;
            //tempSiteToAdd->print();
            listMatchSites = addSiteToList(listMatchSites, tempSiteToAdd);
        }
    }
    return listMatchSites;
}

Site *searchByWebSite(Site *site, Token *listTokens){
    bool match;
    Site *newSite = new Site(site->getName(), site->getAddress());
    newSite->setAddress(site->getAddress());
    int numberMatches;
    for(Token *tempToken = listTokens; tempToken != NULL; tempToken = tempToken->sig){
        //
        numberMatches = 0;
        string strToken = tempToken->getStrToken();
        string strTitle = site->getName();
        string strBody = site->getBody();
        //cout<<"TITLE"<<endl;
        for(int n = 0; n < strTitle.length(); n++){
            if(strTitle[n] == strToken[0]){
                int tempN = n; match = true;
                for(int m = 0; m < strToken.length(); m++){
                    //cout<< strTitle[tempN] << " == "<< strToken[m]<<endl;
                    if(tempN >= strTitle.length() || strTitle[tempN] != strToken[m]){
                        match = false;
                        break;
                    }
                    tempN = tempN + 1;
                }
                if(match){
                    numberMatches ++;
                }
            }
        }
        //cout<<"BODY:"<<strBody<<endl;
        for(int n = 0; n < strBody.length(); n++){
            if(strBody[n] == strToken[0]){
                int tempN = n; match = true;
                for(int m = 0; m < strToken.length(); m++){
                    //cout<< strBody[tempN] << " == "<< strToken[m]<<endl;
                    if(tempN >= strBody.length() || strBody[tempN] != strToken[m]){
                        match = false;
                        break;
                    }
                    tempN = tempN + 1;
                }
                if(match){
                    numberMatches ++;
                    //cout<<">> Match in: "<<strToken<<endl;
                }
            }
        }
        //
        if(numberMatches > 0){
            //cout<<">> Match in: "<<strToken<<endl;
            Token *newToken = new Token(tempToken->getStrToken(), numberMatches);
            newSite->addToken(newToken);
        }
    }
    return newSite;
}

void master(int nprocs){
    //printf("Soy el procesador %d\n",pid);
    char option[10];
    option[0] = '0';
    MPI_Status status;
    //while(option!="9"){
        cout<<"**** Menu ****"<<endl;
        cout<<"1. Busqueda"<<endl;
        cout<<"2. Ayuda"<<endl;
        cout<<"9. Salir"<<endl;
        cout<<"Escoja una opcion: ";
        //cin.get(option)
        //std::getline(cin, option);
        cin.getline(option, 10);
        cout<<"************"<<endl;
        if(option[0] == '1'){
            cout<<"Busqueda: ";
            //string strSearch;
            char strSearch[100];
            //std::getline(cin, strSearch);
            std::cin.get(strSearch, 100);
            Site *listSites = loadWEBSites(NULL);
        ///
        Token *listTokensToSearch = getListTokensToSearch(strSearch);
        // --- searchEngine ---
        bool match;Site *listMatchSites = NULL; MPI_Status status;
        printListTokens(listTokensToSearch);
        //Site *tempSiteToAdd;
        for(Site *tempSite = listSites; tempSite != NULL; tempSite = tempSite->sig){
            ///tempSiteToAdd = searchByWebSite(tempSite, listTokens);
            ///
            bool match;
            Site *newSite = new Site(tempSite->getName(), tempSite->getAddress());
            newSite->setAddress(tempSite->getAddress());
            int numberMatches;int intRank = 1; int faltantes = getSizeOfListTokens(listTokensToSearch);
            for(Token *tempToken = listTokensToSearch; tempToken != NULL; tempToken = tempToken->sig){
                //cout<<"LLEGO AQUI"<<endl;
                char strToken[100]; char strTitle[100]; char strBody[100000];
                strcpy(strToken, tempToken->getStrToken().c_str());
                //cout<<"strToken"<<endl;
                strcpy(strTitle, tempSite->getName().c_str());
                //cout<<"strTitle"<<endl;
                strcpy(strBody, tempSite->getBody().c_str());
                //cout<<"strBody"<<endl;
                /*string strTitle = tempSite->getName();
                string strBody = tempSite->getBody();*/
                flag = 1;
                MPI_Send(&flag, 1, MPI_INT, intRank, 0, MPI_COMM_WORLD);
                MPI_Send(strToken, 100, MPI_CHAR, intRank, 1, MPI_COMM_WORLD);
                MPI_Send(strTitle, 100, MPI_CHAR, intRank, 2, MPI_COMM_WORLD);
                MPI_Send(strBody, 100000, MPI_CHAR, intRank, 3, MPI_COMM_WORLD);
                //cout<<">> intRank= "<<intRank<<" != "<<"Nproces= "<<nprocs-1<< endl;
                if(intRank == nprocs-1 || intRank == faltantes){
                    //cout<<">> Entro: "<<strToken<<endl;
                    int numberMatches;
                    for(;intRank > 0; intRank--){
                        //cout<<">> Termino: "<<strToken<<endl;
                        numberMatches = 0;
                        char tempStrToken[100];
                        MPI_Recv(&numberMatches, 1, MPI_INT, intRank, 4, MPI_COMM_WORLD, &status);
                        MPI_Recv(tempStrToken, 100, MPI_CHAR, intRank, 5, MPI_COMM_WORLD, &status);
                        if(numberMatches > 0){
                            //cout<<">> Match in: "<<strToken<<endl;
                            Token *newToken = new Token(tempStrToken, numberMatches);
                            newSite->addToken(newToken);

                        }
                        faltantes = faltantes - 1;
                    }
                    intRank = 0;
                }
                intRank++;
            }

            ///
            //cout<<"Salio bien: "<<endl;
            /*for (int intRank = 1; intRank < nprocs; intRank++) {
            }*/

            if(newSite->getListTokensMatches() != NULL)
            {
                //cout<<"SE ENCONTRARON VARIOS TOKENS en:"<<endl;
                //tempSiteToAdd->print();
                //cout<<"Aqui aun1: "<<endl;
                listMatchSites = addSiteToList(listMatchSites, newSite);
                //cout<<"Aqui aun2: "<<endl;
            }
            //cout<<"NO SE ENCONTRARON VARIOS TOKENS en:"<<endl;
        }
        flag = 0;
        for (int intRank = 1; intRank < nprocs; intRank++) {
            MPI_Send(&flag, 1, MPI_INT, intRank, 0, MPI_COMM_WORLD);
        }


        //MPI_Send(0, 1, MPI_INT, intRank, 0, MPI_COMM_WORLD);
        // --- End searchEngine ---
        //searchEngine(listSites, listTokensToSearch);
        cout<<">> Lista de coincidencias <<"<<endl;
        //printListSites(listMatchSites, 1);
        orderListSitesAscending(listMatchSites);
        //printListTokens(listTokensToSearch);
        ///
        }

    /*int result;
    MPI_Recv(&result,	// message buffer
			1,		// one data item
			MPI_INT,	// data item is a double real
			MPI_ANY_SOURCE,	// receive from any sender
			1,	// receive any type of message
			MPI_COMM_WORLD,	// always use this
			&status);	// info about received message*/
}

void slave(int pid, int nprocs){
    /*MPI::COMM_WORLD.Probe(0, 0, status);
        int l = status.Get_count(MPI::CHAR);
        char *buf = new char[l];*/
        MPI_Status status;
        MPI_Recv(&flag, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if(flag == 1){
            char strToken[100]; char strTitle[100]; char strBody[100000];
            MPI_Recv(strToken, 100, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(strTitle, 100, MPI_CHAR, 0, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(strBody, 100000, MPI_CHAR, 0, 3, MPI_COMM_WORLD, &status);
            int numberMatches = 0; bool match;
            //cout<<"TITLE"<<endl;

            for(int n = 0; n < strlen(strTitle); n++){
                if(strTitle[n] == strToken[0]){
                    int tempN = n; match = true;
                    for(int m = 0; m < strlen(strToken); m++){
                        //cout<< strTitle[tempN] << " == "<< strToken[m]<<endl;
                        if(tempN >= strlen(strTitle) || strTitle[tempN] != strToken[m]){
                            match = false;
                            break;
                        }
                        tempN = tempN + 1;
                    }
                    if(match){
                        numberMatches ++;
                    }
                }
            }
            //cout<<"BODY:"<<strBody<<endl;
            for(int n = 0; n < strlen(strBody); n++){
                if(strBody[n] == strToken[0]){
                    int tempN = n; match = true;
                    for(int m = 0; m < strlen(strToken); m++){
                        //cout<< strBody[tempN] << " == "<< strToken[m]<<endl;
                        if(tempN >= strlen(strBody) || strBody[tempN] != strToken[m]){
                            match = false;
                            break;
                        }
                        tempN = tempN + 1;
                    }
                    if(match){
                        numberMatches ++;
                        //cout<<">> Match in: "<<strToken<<endl;
                    }
                }
            }
            MPI_Send(&numberMatches, 1, MPI_INT, 0, 4, MPI_COMM_WORLD);
            MPI_Send(strToken, 100, MPI_CHAR, 0, 5, MPI_COMM_WORLD);
            slave(pid, nprocs);
        }
        //searchEngineConsoleView(listSites, strSearch, pid);
        /*if(option == "2"){

        }*/
}

// ----------------- MAIN --------------------//
int main(int argc, char **argv){
    int pid, nprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    //
    //  Have Process 0 say hello.
    //
    //printf("Soy el procesador %d de un total de %d\n",pid,nprocs);

    /*Site *google = new Site("Google", "google.com");
    listSites = google;
    Site *facebook = new Site("Facebook", "http://facebook.com");
    listSites->sig = facebook;*/
    //facebook->open();
    //connectSite(google->getAddress());

    //printListSites(listSites);
    //if(pid == 0){
    if (pid == 0) {
		master(nprocs);
    } else {
		slave(pid, nprocs);
	}
	MPI_Barrier(MPI_COMM_WORLD);
    //}
    /*tipoToken *listTokensMatches = NULL;
    tipoToken *tempTokens  = listTokensMatches;*/
    cout << "End nodo: " <<pid<< endl;
    MPI_Finalize();
    return 0;
}
// ----------------- END MAIN --------------------//
