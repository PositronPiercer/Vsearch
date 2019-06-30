#include <stdio.h>
#include <stdlib.h>
#include <pbc/pbc.h>
#include <string.h>
#include <openssl/sha.h>
//#include "utilities.h"
#include "vsearch.h"

void set_signature (unsigned char * pos, unsigned char * sig_bits){
  FILE * sig_file = fopen ("signatures", "r");
  //convert pos to binary
  unsigned char pos_binary[8* sizeof (unsigned char) * SHA_DIGEST_LENGTH] = "";
  unsigned char pos_temp[8* sizeof (unsigned char) * SHA_DIGEST_LENGTH] = "";
  for (int i = 0; i < strlen (pos); i++){
    sprintf (pos_binary + i * 8 * sizeof (unsigned char), "%s", byte_to_binary (*(pos + i)));
  }
  while ((fscanf (sig_file,"%s %s", pos_temp, sig_bits) == 2) && (strcmp (pos_temp, pos_binary) != 0));
  fclose (sig_file);
}


int search_server (token s_token){
  printf ("_______________Cloud-Search_______________\n");
  //get the id file
  FILE * id_file = get_id_file (s_token.tau_sigma_f);

  //build the pairing
  FILE * param_file = fopen ("param.txt", "r");
  FILE * owner_result  = fopen ("owner-result.txt", "w"); //search results for owner
  FILE * auditor_result = fopen ("auditor-result.txt", "w"); //search results for auditor
  char param_buf[PARAM_LENGTH];
  param_buf[0] = 0; //null char at beginning


  pbc_param_t param;
  pairing_t pairing;

  //get parameters
  read_entire_file (param_file, param_buf);
  fclose (param_file);

  pbc_param_init_set_str (param, param_buf);
  pairing_init_pbc_param (pairing, param);

  element_t g, pbc_id_hash, h, sigma_prod, sigma;

  element_init_G2 (g, pairing);
  element_init_G2 (h, pairing);
  element_init_G2 (sigma, pairing);
  element_init_G2 (sigma_prod, pairing);
  element_init_Zr(pbc_id_hash, pairing);

  char key_[MAX_KEY_LENGTH] = "";
  char id[MAX_ID_LENGTH] = "";
  unsigned char tag[SHA_DIGEST_LENGTH + MAX_ID_LENGTH + MAX_NID] = ""; //has to store id and i during concat
  unsigned char pos[SHA_DIGEST_LENGTH] = "";
  unsigned char sig_bits[SIGNATURE_LENGTH * 8 * sizeof (unsigned char)] = "";

  int i = 0;

  while (fgets (id, MAX_ID_LENGTH, id_file) != NULL){
    //remove \n from end
    id[strlen (id) - 1] = 0;

    fprintf (owner_result, "%s\n", id);
    i++;
    //calculate pos
    strcpy (tag, s_token.tag);
    strcat (tag, id);
    sprintf (tag + strlen (tag), "%d", i);
    F (tag, pos);

    //get signature
    set_signature(pos, sig_bits);
    binary2bytes (sig_bits, key_);
    element_from_bytes_compressed (sigma, key_);
    //element_printf ("sig %B\n", sigma);
    if (i == 1){
        element_set (sigma_prod, sigma);
    }
    else {
        element_mul (sigma_prod, sigma_prod, sigma);
    }
  }
  element_to_bytes_compressed (key_, sigma_prod);
  printf("Owner results written\n");
  printf ("Writing auditor-result...");
  for (int i = 0; i < element_length_in_bytes_compressed (sigma_prod); i++){
      fprintf (auditor_result, "%s", byte_to_binary (key_[i]));
  }
  fprintf (auditor_result, "\n");
  printf ("done\n");

  fclose (owner_result);
  fclose (auditor_result);


}
