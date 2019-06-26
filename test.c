#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pbc/pbc.h>
#include "utilities.h"

int main(){
    char key[MAX_KEY_LENGTH];
    FILE * param_file = fopen ("param.txt", "r");
    FILE * secret_file = fopen ("secrets", "r");
    FILE * id_file = fopen ("owner-result.txt", "r");
    char param_buf[PARAM_LENGTH];
    param_buf[0] = 0; //null char at beginning
    pbc_param_t param;
    pairing_t pairing;
    //get parameters
    read_entire_file (param_file, param_buf);
    fclose (param_file);

    pbc_param_init_set_str (param, param_buf);
    pairing_init_pbc_param (pairing, param);

    element_t  a,b,c;


    element_init_Zr(a, pairing);
    element_init_Zr(b, pairing);
    element_init_Zr(c, pairing);

    mpz_t d,e,f;
    mpz_init (d);
    mpz_init (e);
    mpz_init (f);

    mpz_set_ui (d,46846846454654654);
    mpz_set_ui (e, 84648848654168464);
    mpz_add (f, d, e);
    //mpz_get_str (key_, 10, f);
    //printf ("gmp %s\n",key_);

    element_set_mpz (c, f);
    element_printf ("%B\n", c);

    element_set_mpz (a, d);
    element_set_mpz (b, e);
    element_add (c, a, b);

    element_printf ("%B\n", c);


}
