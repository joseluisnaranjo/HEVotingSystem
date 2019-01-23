#ifndef BALLOT_H
#define BALLOT_H
#include "QList"
#include "candidate.h"
class Ballot
{
private :
    int ballotNumber;


public:

    Ballot();
    int candidatoA;
    int candidatoB;
    int candidatoC;
    int candidatoD;
    QList<Candidate>  lstCandidatos;
    void RegisterVote(Candidate);
    int getVoteValue(Candidate);
};

#endif // BALLOT_H
