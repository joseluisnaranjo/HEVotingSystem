#include <QCoreApplication>
#include "QFile"
#include "QTextStream"
#include "QDataStream"
#include "candidate.h"
#include "ballot.h"
#include <chrono>

#include "FHE.h"
#include "EncryptedArray.h"
#include <NTL/lzz_pXFactoring.h>
#include <fstream>
#include <sstream>
#include <sys/time.h>


Ctxt calculateHammingWeight (vector<Ctxt> values ){

    int dimension = values.size();
    Ctxt ctSum = values[0];
    for ( int i=1; i < dimension; i++){
        ctSum += values[i];
    }
    return ctSum;
}
Ctxt calculateHammingWeight (QList<Ctxt> values ){

    Ctxt ctSum = values.takeFirst();
    foreach( Ctxt value , values){
        ctSum += value;
    }
    return ctSum;
}
int DecryptCipheredValue (Ctxt value, EncryptedArray ea, FHESecKey secretKey ){

    vector<long> res;
    ea.decrypt(value, secretKey, res);
    return res[0];
}

void Guardar(QList<double> time){
    QFile file("/home/jolunavi/file.txt");
    if (!file.open(QIODevice::WriteOnly)){
        cout << "GCouldnt openb file" << endl;
        return;
    }
    QDataStream out(&file);
      //out.setVersion(QDataStream::Qt_5_9);
   QString sata = "hola a todos";
   int num = 22;
    out << time ;
    //out << a ;

   file.flush();
    file.close();
}
void GuardarC(QList<double> time, string type){


    if (type == "HW")    {
        ofstream file("/home/jolunavi/hwdata.txt");
        if (file.is_open()){
            foreach ( double dat, time){
                file << dat << endl;
            }

            file.close();
        }
        else cout << "unable to open file";
    }
    else if (type == "NV") {
        ofstream file("/home/jolunavi/nvdata.txt");
        if (file.is_open()){
            foreach ( double dat, time){
                file << dat << endl;
            }

            file.close();
        }
        else cout << "unable to open file";

    }
    //ofstream file("/home/jolunavi/file.txt");


}
void Load(){
    QFile file("/home/jolunavi/file.txt");
    if (!file.open(QIODevice::ReadOnly)){
        cout << "GCouldnt openb file" << endl;
        return;
    }

    QDataStream in(&file);

      in.setVersion(QDataStream::Qt_5_9);
   QString sata = "";
   int num ;
    in >> num >> sata ;

    file.close();

    cout << num ;
    cout << sata.toStdString();
}
QList<Candidate>  LoadCandidates(){
    QList<Candidate> lstcand;
    Candidate uno("Carlos","Sanchez", "ca","sa");
    uno.setLogo("A");
    lstcand.append(uno);
    Candidate dos("Maria","Bonilla", "ma","bo");
    dos.setLogo("B");
    lstcand.append(dos);
    Candidate tres("Elisa","Aguirre", "el","sa");
    tres.setLogo("C");
    lstcand.append(tres);
    Candidate cuatro("Jorge","Ramos", "jo","ra");
    cuatro.setLogo("D");
    lstcand.append(cuatro);
    return lstcand;

}
int VotingSystem()
{
    QList<double> datosck;
    int numVoters = 4;
    int numCandidates = 4;
    Candidate whiteCandidate("WHITE","white", "white","white");
    Candidate nuleCandidate("NULL","nule", "nule","nule");
    cout << "*******************************************************************************" << endl;
    cout << "*                                                                             *" << endl;
    cout << "* Welcome to the first Demo of a Voting Systema based in the BGV cryptosystem *" << endl;
    cout << "*                                                                             *" << endl;
    cout << "*******************************************************************************" << endl;
    cout << "" << endl;
    cout << "" << endl;
    cout << "Intup the number of Voters: " << endl;
    cin >> numVoters ;
    cout << "The number of candidates has been set to: 5" << endl;

    cout << "The parameters for encryption are being configured..." << endl;

    long m=0, p=1013, r=1; // Native plaintext space
    // Computations will be 'modulo p'
    long L=16;          // Levels
    long c=3;           // Columns in key switching matrix
    long w=64;          // Hamming weight of secret key
    long d=0;
    long security = 128;
    ZZX G;
    m = FindM(security,L,c,p, d, 0, 0);

    FHEcontext context(m, p, r);
    // initialize context
    buildModChain(context, L, c);
    // modify the context, adding primes to the modulus chain
    FHESecKey secretKey(context);
    // construct a secret key structure
    const FHEPubKey& publicKey = secretKey;
    // an "upcast": FHESecKey is a subclass of FHEPubKey

    //if(0 == d)
    G = context.alMod.getFactorsOverZZ()[0];

    secretKey.GenSecKey(w);
    // actually generate a secret key with Hamming weight w

    addSome1DMatrices(secretKey);
    cout << "Generated key" << endl;

    EncryptedArray ea(context, G);
    // constuct an Encrypted array object ea that is
    // associated with the given context and the polynomial G

    Ctxt c0(publicKey);
    Ctxt v0(publicKey);
    Ctxt v1(publicKey);
    ZZX plainvote = to_ZZX((0));
    publicKey.Encrypt(v0, plainvote);
    plainvote = to_ZZX((1));
    publicKey.Encrypt(v1, plainvote);

    vector<Ctxt> vc(numCandidates,c0);         // A vector of numcandidatos ciphertexts

    NewPlaintextArray p0(ea);
    int nc= numCandidates +2;
    QList<Candidate> lstCandidates; // A List of candidatos
    lstCandidates = LoadCandidates();
    //Inivializacion de vectores de dator individuales
    int numData =  numCandidates * numVoters;

    vector<Ctxt> vcData(numData,c0);         // A vector of numcandidatos ciphertexts

    //ubicacion del ciphertext en el vector de Datos completo
    int dataPosition = 0;// puntero para el vector de los datos totales

    for (long i=0; i<numVoters; i++) {
        Ballot ballot;
        ballot.lstCandidatos = lstCandidates;
        cout << "Please press the letre of the candidate that you want to vote for:" << endl;
        foreach( Candidate ca , lstCandidates){
            cout << "press " << ca.getLogo() << " for: " << ca.getName() << " " << ca.getLastName() << endl;
        }

        // local variable declaration:
        char option;
        cin >> option;
        //Candidate candidateE;

        switch(option) {
        case 'A' :
            cout << "Candidate A" << endl;
            ballot.lstCandidatos[0].vote = 1;
           // ballot.lstCandidatos->takeAt(1).vote << 1;
            ballot.candidatoA = 1;
            break;
        case 'B' :
            cout << "Candidate B" << endl;
            ballot.lstCandidatos[1].vote = 1;
            ballot.candidatoB = 1;
            break;
        case 'C' :
            cout << "Candidate C" << endl;
            ballot.lstCandidatos[2].vote = 1;
            ballot.candidatoC = 1;
            break;
        case 'D' :
            cout << "Candidate D" << endl;
            ballot.lstCandidatos[3].vote = 1;
            ballot.candidatoD = 1;
            break;
            //la dimension del vector aun no esta definda esta opcion solo candidatos
        case 'N' :
            cout << "None" << endl;
            break;
        default :
            cout << "Invalid candidate! " << endl;
        }
        cout << "Your selected the candidate: " << option << endl;

        QList<Ctxt> lstData;
        int k = 0 ;
        foreach( Candidate c, ballot.lstCandidatos){
            ZZX plainvote = to_ZZX((c.vote));
            publicKey.Encrypt(vc[k], plainvote);
            k++;
        }

        auto starthw = std::chrono::steady_clock::now();
        c0 = calculateHammingWeight(vc);
        auto stophw = std::chrono::steady_clock::now();
        double elapsed_timehw = double (std::chrono::duration_cast <std::chrono::nanoseconds> (stophw - starthw).count());
        //cout << "Elapsed time (s) " << elapsed_timehw /1e9 << endl;
        datosck.append(elapsed_timehw);


        int sum = DecryptCipheredValue (c0, ea, secretKey );
        if (sum == 1){
            for(int i=0; i< numCandidates ;i++){
                Candidate  a = lstCandidates.takeFirst();
                a.LstVotes.append(vc[i]);
                lstCandidates.append(a);
            }
                whiteCandidate.LstVotes.append(v0);
                nuleCandidate.LstVotes.append(v0);

        }else if (sum == 0){
            for(int i=0; i< numCandidates ;i++){
                Candidate  a = lstCandidates.takeFirst();
                a.LstVotes.append(vc[i]);
                lstCandidates.append(a);
            }
                whiteCandidate.LstVotes.append(v1);
                nuleCandidate.LstVotes.append(v0);

        }else if (sum >1) {
            for(int i=0; i< numCandidates ;i++){
                Candidate  a = lstCandidates.takeFirst();
                a.LstVotes.append(v0);
                lstCandidates.append(a);
            }
                whiteCandidate.LstVotes.append(v0);
                nuleCandidate.LstVotes.append(v1);

        }

    cout << "Los datos han sido encriptados Corectamente " << endl;

    //Computation of the number of votes per candidate

}
   GuardarC(datosck, "HW");
   datosck.clear();
    for (long i=0; i<numCandidates; i++) {
        Candidate  a = lstCandidates.takeFirst();
        vector<Ctxt> ctVotes (numVoters, c0);
        auto start = std::chrono::steady_clock::now();
        ctVotes[i] = calculateHammingWeight(a.LstVotes);
        auto stop = std::chrono::steady_clock::now();
        double elapsed_time = double (std::chrono::duration_cast <std::chrono::nanoseconds> (stop - start).count());
        datosck.append(elapsed_time);
        int sum = DecryptCipheredValue (ctVotes[i], ea, secretKey );
    cout << a.getName() << " has: " << sum << " votes" << endl;
    }
    int nuleVotes = DecryptCipheredValue((calculateHammingWeight(nuleCandidate.LstVotes)),ea, secretKey);
    int whitVotes = DecryptCipheredValue((calculateHammingWeight(whiteCandidate.LstVotes)),ea, secretKey);;

    cout << whiteCandidate.getName() << " has: " << whitVotes << " votes" << endl;
    cout << nuleCandidate.getName() << " has: " << nuleVotes << " votes" << endl;
GuardarC(datosck, "NV");
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    VotingSystem();
    return a.exec();


}
