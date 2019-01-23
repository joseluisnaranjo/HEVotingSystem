#ifndef CANDIDATE_H
#define CANDIDATE_H
#include <string>
using std ::string;


#include "FHE.h"
#include "EncryptedArray.h"
#include <NTL/lzz_pXFactoring.h>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include "qlist.h"


class Candidate
{
private :
    int Id;
    string Name;
    string LastName;
    string UserName;
    string Password;
    string PKey;
    string Skey;
    string Logo;



public:
    int  vote;
    //Ctxt cvote;
    QList<Ctxt> LstVotes;
    //vector<Ctxt> vecVotes;
    Candidate();
    Candidate( string n, string ln, string un, string p);
    void setVotesVector(vector<Ctxt> vc);
    string getName();
    string getLastName();
    string getLogo();
    void setLogo(string l);
};
#endif // CANDIDATE_H
