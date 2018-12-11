
//============================================================================
// Name        : Test_VotinfSystem.cpp
// Author      : Jose Luis Naranjo Villota
// Version     : 1.0.0
// Copyright   : Computer Sciences and  Technology at SWJTU, Sichuan, China.
// Description : Demo HE Voting Sytem in C++, Ansi-style
//============================================================================

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

int DecryptCipheredValue (Ctxt value, EncryptedArray ea, FHESecKey secretKey ){

	vector<long> res;
	ea.decrypt(value, secretKey, res);
	return res[0];
}
int main(int argc, char **argv)
{
	int numVoters = 4;
	int numCandidates = 4;
	cout << "Welcome to the first Demo of a Voting Systema based in the BGV cryptosystem " << endl;
	cout << "Ingrese le numero de Votantes: " ;
	cin >> numVoters ;
	cout << "Ingrese le numero de candidatos: 4";
	//cin >> numCandidates << endl;

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
	vector<Ctxt> vc(numCandidates,c0);         // A vector of numcandidatos ciphertexts

	NewPlaintextArray p0(ea);
	vector<int> vp(numCandidates); // A vector of numcandidatos plaintexts

	//Inivializacion de vectores de dator individuales
	int numData =  numCandidates * numVoters;

	vector<Ctxt> vcData(numData,c0);         // A vector of numcandidatos ciphertexts

	//ubicacion del ciphertext en el vector de Datos completo
	int dataPosition = 0;// puntero para el vector de los datos totales

	for (long i=0; i<numVoters; i++) {
		int ballot[numCandidates] = {0,0,0,0};
		cout << "Please press the letre of the candidate that you want to vote for:" << endl;
		cout << "press A for:   Alex Carrion" << endl;
		cout << "press B for:   Jose Bravo" << endl;
		cout << "press C for:   Juan Flores" << endl;
		cout << "press D for:   Maria Sanchez" << endl;
		// local variable declaration:
		char option;
		cin >> option;

		switch(option) {
		case 'A' :
			cout << "Candidate A" << endl;
			ballot[0] = 1;
			break;
		case 'B' :
			cout << "Candidate B" << endl;
			ballot[1] = 1;
			break;
		case 'C' :
			cout << "Candidate C" << endl;
			ballot[2] = 1;
			break;
		case 'D' :
			cout << "Candidate D" << endl;
			ballot[3] = 1;
			break;
			//la dimension del vector aun no esta definda esta opcion solo candidatos
		case 'N' :
			cout << "None" << endl;
			break;
		default :
			cout << "Invalid candidate! " << endl;
		}
		cout << "Your selected the candidate: " << option << endl;
		for (long i=0; i<numCandidates; i++) {

			ZZX plainv = to_ZZX(ballot[i]);
			publicKey.Encrypt(vc[i], plainv);
			//cout << "pocision:" << dataPosition<< ", Valor:" << DecryptCipheredValue (vc[i], ea, secretKey )<< endl;
			vcData[dataPosition] =  vc[i];
			dataPosition ++;
		}
	};
	cout << "Los datos han sido encriptados Corectamente " << endl;

	//Computation of the number of votes per candidate

	for (int i=0; i< numData; i++){
		cout << "pocision:" << i << ", Valor:" << DecryptCipheredValue (vcData[i], ea, secretKey )<< endl;
	}
	for (long i=0; i<numCandidates; i++) {
		vector<Ctxt> ctVotes (numVoters, c0);
		int puntero = i;
		for (long j=0; j<numVoters; j++) {
			ctVotes[j] = vcData[puntero];
			//cout << "Vector de votos del candidato: " << i << endl;
			cout << "pocision:" << puntero << ", Valor:" << DecryptCipheredValue (ctVotes[j], ea, secretKey )<< endl;
			puntero +=numCandidates;
		};
		cout << "Calculando numero de votos del candidato: " << i << endl;
		vc[i] = calculateHammingWeight(ctVotes);
	}
	cout << "Se han calculado el numero de votos correctamente "  << endl;
	//Decryption of the final data:
	for (long i=0; i<numCandidates; i++) {
		vp[i]= DecryptCipheredValue (vc[i], ea, secretKey );
	}
	cout << "The final results are the following: " << endl;
	cout << "  The canditate A: " << vp[0] << endl;
	cout << "  The canditate B: " << vp[1] << endl;
	cout << "  The canditate C: " << vp[2] << endl;
	cout << "  The canditate D: " << vp[3] << endl;
	return 0;
}














