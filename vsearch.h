#include "utilities.h"
//keygen
typedef struct{
    unsigned char * Kt;
    int Kt_length;

    unsigned char * Ks;
    int Ks_length;

    unsigned char * Sk;
    int Sk_length;

    unsigned char * Pk;
    int Pk_length;

    unsigned char * g;
    int g_length;

}keys;
int keygen_compute(unsigned long int lambda,keys * K_psi_l);
int keygen();
//__________________________________________________________________

//build
void build ();
//__________________________________________________________________

//search-cloud
void set_signature (unsigned char * pos, unsigned char * sig_bits);
int search_server (token s_token);
token search_token (char * keyword);
//__________________________________________________________________

//search-owner
int get_c (char * keyword);
void owner_search (char * keyword);
//__________________________________________________________________

//search-auditor
bool auditor_verification (char * keyword);
//__________________________________________________________________

//update
int getIndex (int count, char * ibuf, char set[count][MAX_KEYWORD_LENGTH]);
void update (char * id);
