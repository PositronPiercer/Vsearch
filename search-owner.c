#include <stdio.h>
#include <stdlib.h>
#include <pbc/pbc.h>
#include <string.h>
#include "utilities.h"
#include <time.h>

int get_c (char * keyword){
    FILE * keyword_file = fopen ("keywords", "r");
    int c = 0;
    char kw[MAX_KEYWORD_LENGTH] = "";
    while (fscanf (keyword_file, "%s %d", kw, &c) && (strcmp (keyword, kw) != 0));
    fclose (keyword_file);
    return c;
}

void owner_search (char * keyword){
    //build pairing
    FILE * owner2auditor = fopen ("owner2auditor.txt", "w");
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

    element_t g, sw_seed, m;
    element_t temp;

    element_init_G2(g, pairing);
    element_init_G2(temp, pairing);
    element_init_Zr(sw_seed, pairing);
    element_init_Zr(m, pairing);


    char key_[MAX_KEY_LENGTH] = "";
    //read the keys from file
    printf ("Loading Keys...");
    fscanf (secret_file,"%*s %s",key_);
    fscanf (secret_file,"%*s %s",key_);
    fscanf (secret_file,"%*s %s",key_);
    fscanf (secret_file,"%*s %s",key_);
    element_set_str(sw_seed, key_, 2);
    printf("Done.\n");
    fclose (secret_file);

    int c = get_c (keyword);

    //calculate sw
    char sw_seed_[MAX_KEYWORD_LENGTH] = "";
    unsigned char sw[SHA_DIGEST_LENGTH] = "";
    strcpy (sw_seed_, keyword);
    element_to_bytes (sw_seed_ + strlen (sw_seed_), sw_seed); //no need to create pbc element
    F (sw_seed_, sw);

    char id[MAX_ID_LENGTH] = "";
    unsigned char id_hash[SHA_DIGEST_LENGTH] = "";
    int i = 0;

    mpz_t q, gmp_id, m_temp, r, gmp_temp, m_prod;
    mpz_init (q);
    mpz_init (gmp_id);
    mpz_init (m_temp);
    mpz_init (r);
    mpz_init (gmp_temp);
    set_q (key_); //get the group order
    mpz_set_str (q, key_, 10);
    while (fgets (id, MAX_ID_LENGTH, id_file) != NULL){
        id[strlen (id) -1] = 0;
        printf("%s\n", id);
        i++;
        //calculate r
        //append i to the end of sw
        sprintf (sw + strlen (sw), "%d", i);
        unsigned long int rtemp = R (sw);
  	  //remove end digit from sw
  	    sw[strlen (sw) - 1] = 0;
        mpz_set_ui (r, rtemp);

        byte2binary_total (id, key_);
        mpz_set_str (gmp_id, key_, 2);

        //calculate m
        mpz_mul (gmp_temp, r, gmp_id);
        mpz_mod (m_temp, gmp_temp, q);

        if (i == 1){
            mpz_set (m_prod, m_temp);
        }
        else {
            mpz_mul (gmp_temp, m_prod, m_temp);
            mpz_mod (m_prod, gmp_temp, q);
        }


    }
    if (i != c){
        printf ("count mismatch\n");
    }
    else {
        //write data to file
        printf ("count matched\n");
        element_set_mpz (m, m_prod);
        element_printf ("pbc %B\n", m);
        //mpz_get_str (key_, 10, m_prod);
        //printf ("gmp %s\n",key_);
        printf ("%d\n", element_length_in_bytes (m));
        element_to_bytes (key_, m);
        printf ("%d\n", element_length_in_bytes (m));
        for (int i = 0; i< element_length_in_bytes (m); i++){
            fprintf (owner2auditor, "%s", byte_to_binary (key_[i]));
        }
        fprintf (owner2auditor, "\n");
    }
    fclose (id_file);
    fclose (owner2auditor);
}

int main(){
    char * keyword = "apple";
    owner_search (keyword);
}
