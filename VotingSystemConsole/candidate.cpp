#include "candidate.h"


vector<Ctxt> vecVotes;
Candidate::Candidate()
{
     vote = 0;

}

Candidate::Candidate(string n, string ln, string un, string p)
{
    Name = n;
    LastName = ln;
    UserName = un;
    Password = p;
    vote = 0;
}

void setVotesVector(vector<Ctxt> vc)
{
   vecVotes = vc;

}

string Candidate::getName(){
    return Name;
}

string Candidate::getLastName(){
    return LastName;
}
string Candidate::getLogo(){
    return Logo;
}
void Candidate::setLogo(string logoo){
    Logo = logoo;
}
