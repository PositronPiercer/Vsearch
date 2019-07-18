#include <stdio.h>
#include <stdlib.h>
#include <pbc/pbc.h>
#include <string.h>
#include <openssl/sha.h>
#include <my_global.h>
#include <mysql.h>
#include "db_secrets.h"
#include "vsearch.h"

int search_server (token s_token){
  MYSQL * con = mysql_init (NULL);
  if (mysql_real_connect (con, "localhost", "root", ROOT_PASS, DATABASE_NAME, 0, NULL, 0) == NULL){
      printf ("%s\n", mysql_error (con));
  }
  printf ("_______________Cloud-Search_______________\n");
  //get the id file
  FILE * id_file = get_id_file (s_token.tau_sigma_f);

  //build the pairing
  FILE * param_file = common_file_open ("data/param.txt", "r");
  FILE * owner_result  = common_file_open ("data/owner-result.txt", "w"); //search results for owner
  FILE * auditor_result = common_file_open ("data/auditor-result.txt", "w"); //search results for auditor
  char param_buf[PARAM_LENGTH];
  param_buf[0] = 0; //null char at beginning


  pbc_param_t param;
  pairing_t pairing;

  //get parameters
  read_entire_file (param_file, param_buf);
  common_file_close (param_file);

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
    if (id[strlen (id) - 1] == '\n')
    id[strlen (id) - 1] = 0;

    fprintf (owner_result, "%s\n", id);
    i++;
    //calculate pos
    strcpy (tag, s_token.tag);
    strcat (tag, id);
    sprintf (tag + strlen (tag), "%d", i);
    F (tag, pos);

    //get signature
    unsigned char pos_binary[8* sizeof (unsigned char) * SHA_DIGEST_LENGTH] = "";
    for (int i = 0; i < strlen (pos); i++){
      sprintf (pos_binary + i * 8 * sizeof (unsigned char), "%s", byte_to_binary (*(pos + i)));
    }
    char query[700] = "";
    strcpy (query, "SELECT sig FROM signatures WHERE pos='");
    strcat (query, pos_binary);
    strcat(query,"'");
    memset(pos_binary, 0, 8* sizeof (unsigned char) * SHA_DIGEST_LENGTH);
    mysql_query (con, query);
    MYSQL_RES * result = mysql_store_result (con);
    MYSQL_ROW row;
    row = mysql_fetch_row (result);
    strcpy(sig_bits, row[0]);
    binary2bytes (sig_bits, key_);
    element_from_bytes_compressed (sigma, key_);
    if (i == 1){
        element_set (sigma_prod, sigma);
    }
    else {
        element_mul (sigma_prod, sigma_prod, sigma);
    }
    mysql_free_result(result);
  }
  element_to_bytes_compressed (key_, sigma_prod);
  printf("Owner results written\n");
  printf ("Writing auditor-result...");
  for (int i = 0; i < element_length_in_bytes_compressed (sigma_prod); i++){
      fprintf (auditor_result, "%s", byte_to_binary (key_[i]));
  }
  fprintf (auditor_result, "\n");
  printf ("done\n");

  element_clear(sigma);
  element_clear(sigma_prod);
  element_clear(h);
  element_clear(g);
  element_clear(pbc_id_hash);
  common_file_close (owner_result);
  common_file_close (auditor_result);
}
