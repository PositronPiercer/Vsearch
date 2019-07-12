#include <pbc/pbc.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
//#include "utilities.h"
#include "vsearch.h"

int keygen_compute(unsigned long int lambda,keys * K_psi_l){
    printf ("\n_______________Keygen_______________\n");
    pbc_param_t param;
    pairing_t pairing;

    pbc_param_init_a_gen (param,lambda,128);
    pairing_init_pbc_param (pairing, param);

    //store the parameters
    FILE * param_file = common_file_open ("data/param.txt", "w");
    pbc_param_out_str(param_file, param);
    common_file_close (param_file);

    // declarations
    element_t g;
    element_t public_key;
    element_t private_key,tag_seed,sw_seed;
    // initializations
    element_init_G2(g, pairing);
    element_init_G2(public_key, pairing);
    element_init_Zr(private_key, pairing);
    element_init_Zr(sw_seed, pairing);
    element_init_Zr(tag_seed, pairing);

    element_random(g); //generate system parameters,
    element_random(private_key); //generate a private key,
    element_random(sw_seed);
    element_random(tag_seed);

    element_pow_zn(public_key, g, private_key); //corresponding public key.

    K_psi_l->Sk_length = element_length_in_bytes(private_key);
    K_psi_l->Ks_length = element_length_in_bytes(sw_seed);
    K_psi_l->Kt_length = element_length_in_bytes(tag_seed);
    K_psi_l->Pk_length = element_length_in_bytes_compressed(public_key);
    K_psi_l->g_length = element_length_in_bytes_compressed(g);

    K_psi_l->Sk = (unsigned char *)malloc(K_psi_l->Sk_length*sizeof(unsigned char));
    K_psi_l->Pk = (unsigned char *)malloc(K_psi_l->Pk_length*sizeof(unsigned char));
    K_psi_l->Ks = (unsigned char *)malloc(K_psi_l->Ks_length*sizeof(unsigned char));
    K_psi_l->Kt = (unsigned char *)malloc(K_psi_l->Kt_length*sizeof(unsigned char));
    K_psi_l->g = (unsigned char *)malloc(K_psi_l->g_length*sizeof(unsigned char));


    element_to_bytes(K_psi_l->Sk,private_key);
    element_to_bytes_compressed(K_psi_l->Pk,public_key);
    element_to_bytes(K_psi_l->Kt,tag_seed);
    element_to_bytes(K_psi_l->Ks,sw_seed);
    element_to_bytes_compressed(K_psi_l->g, g);

    //freeing Memory
    element_clear(private_key);
    element_clear(public_key);
    element_clear(sw_seed);
    element_clear(tag_seed);
    element_clear(g);

    return 0;
}

int keygen() {
    keys * K_psi_l;
    K_psi_l = (keys*)malloc(sizeof(keys));
    printf("Generating Keys...");
    while(keygen_compute(256,K_psi_l)){
        printf("Retrying...\n");
    }


    printf("Done\n");

    //writing to file
    printf("Writing to file\n");
    FILE * secret_file;
    secret_file = common_file_open("data/secrets","w");


    print_to_binary(K_psi_l->g_length, secret_file,"g",K_psi_l->g);
    print_to_binary(K_psi_l->Sk_length,secret_file,"Sk",K_psi_l->Sk);
    print_to_binary(K_psi_l->Pk_length,secret_file,"Pk",K_psi_l->Pk);
    print_to_binary(K_psi_l->Ks_length,secret_file,"Ks",K_psi_l->Ks);
    print_to_binary(K_psi_l->Kt_length,secret_file,"Kt",K_psi_l->Kt);
    common_file_close(secret_file);

    printf("Freeing Memory\n");
    free(K_psi_l->Sk);
    free(K_psi_l->Pk);
    free(K_psi_l->Kt);
    free(K_psi_l->Ks);
    free(K_psi_l);
    printf("Keygen complete\n");
}
