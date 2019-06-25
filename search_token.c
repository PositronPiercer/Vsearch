#include <stdio.h>
#include <stdlib.h>
#include <pbc/pbc.h>
#include <string.h>
#include <openssl/sha.h>
#include "utilities.h"




token search_token (char * keyword){
  printf ("search-token generation\n");
  token s_token;
  s_token.tau_sigma_f = (char *)malloc (strlen (keyword) * sizeof (char));
  strcpy (s_token.tau_sigma_f, keyword);

  FILE * param_file = fopen ("param.txt", "r");
  char param_buf[PARAM_LENGTH] = "";


  pbc_param_t param;
  pairing_t pairing;

  //pbc_param_init_a_gen (param, LAMBDA, 128);
  //get parameters
  read_entire_file (param_file, param_buf);
  fclose (param_file);

  pbc_param_init_set_str(param, param_buf);
  pairing_init_pbc_param (pairing, param);

  element_t tag_seed;
  element_init_Zr (tag_seed, pairing);

  //get the tag_seed
  printf ("Generaring tag...");
  char key_[MAX_KEY_LENGTH] = "";
  char tag[SHA_DIGEST_LENGTH] = "";
  FILE * secret_file = fopen ("secrets", "r");
  fscanf (secret_file, "%*s %s",key_);
  fscanf (secret_file, "%*s %s",key_);
  fscanf (secret_file, "%*s %s",key_);
  fscanf (secret_file, "%*s %s",key_);
  //first 4 keys skkipped
  fscanf (secret_file, "%*s %s",key_);
  fclose (secret_file);
  element_set_str (tag_seed, key_, 2);

  //generate tag
  char tg[MAX_KEYWORD_LENGTH + MAX_KEY_LENGTH] = "";
  strcpy (tg, keyword);
  element_to_bytes (tg + strlen (tg), tag_seed);
  F (tg, tag);
  printf("Done\n");
  s_token.tag = (char *)malloc (strlen (tag) * sizeof (char));
  strcpy (s_token.tag, tag);


return s_token;

}
