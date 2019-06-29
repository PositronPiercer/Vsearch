#include <stdio.h>
#include <stdlib.h>
#include <pbc/pbc.h>
#include "utilities.h"
#include <string.h>
#include <stdbool.h>

bool auditor_verification (char * keyword){
    printf ("_______________Auditor-Search_______________\n");
    FILE * owner2auditor = fopen ("owner2auditor.txt", "r");
    FILE * auditor_result = fopen ("auditor-result.txt", "r");
    //generate the pairing
    FILE * secret_file = fopen ("secrets", "r");
    FILE * param_file = fopen ("param.txt", "r");
    char param_buf[PARAM_LENGTH];
    param_buf[0] = 0; //null char at beginning

    pbc_param_t param;
    pairing_t pairing;

    //get parameters
    read_entire_file (param_file, param_buf);
    fclose (param_file);

    pbc_param_init_set_str(param, param_buf);
    pairing_init_pbc_param (pairing, param);

    element_t public_key, g, h, temp1, temp2, m;
    element_t sigma, sig_owner, temp;

    element_init_G2(g, pairing);
    element_init_G2(h, pairing);
    element_init_G2(temp, pairing);
    element_init_G2(sig_owner, pairing);
    element_init_GT(temp1, pairing);
    element_init_GT(temp2, pairing);
    element_init_G2(public_key, pairing);
    element_init_Zr (m, pairing);



    unsigned char key_[MAX_KEY_LENGTH] = "";
    unsigned char temp_bytes[MAX_KEY_LENGTH / 8] = "";
    //read the keys from file
    printf ("Loading Keys...");
    fscanf (secret_file,"%*s %s",key_);
    unsigned char * g_bytes = (unsigned char *)malloc (strlen (key_) * sizeof (unsigned char));
    binary2bytes (key_, g_bytes);
    element_from_bytes_compressed (g, g_bytes);
    free (g_bytes);
    fscanf (secret_file,"%*s %s",key_);
    //skip secret_key
    fscanf (secret_file,"%*s %s",key_);
    unsigned char * p_bytes = (unsigned char *)malloc (strlen (key_) * sizeof (unsigned char));
    binary2bytes (key_, p_bytes);
    int plength = element_from_bytes_compressed (public_key, p_bytes);
    free (p_bytes);
    printf("Done.\n");
    fclose (secret_file);
    //get the verification data
    fscanf (auditor_result, "%s", key_);
    binary2bytes (key_, temp_bytes);
    element_from_bytes_compressed (sig_owner, temp_bytes);
    element_printf ("sig : %B\n",sig_owner);
    fscanf (owner2auditor, "%s", key_);
    element_set_str (m, key_, 2);


    fclose (auditor_result);
    fclose (owner2auditor);
    element_pow_zn (h, g, m);

    pairing_apply(temp1, sig_owner, g, pairing);
    pairing_apply(temp2, h, public_key, pairing);

    if (element_cmp (temp1, temp2))
        return false;

    return true;

}

int main(){
    char * keyword = "apple";
    if (auditor_verification (keyword))
        printf ("Accept\n");
    else printf ("Reject\n");
}
