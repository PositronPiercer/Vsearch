#include <stdio.h>
#include <stdlib.h>
#include <pbc/pbc.h>
#include <string.h>
#include "utilities.h"
#include <my_global.h>
#include <mysql.h>
#include "db_secrets.h"

void update (char * id){
    //--------------
    MYSQL * con = mysql_init (NULL);
    if (mysql_real_connect (con, "localhost", "root", ROOT_PASS, DATABASE_NAME, 0, NULL, 0) == NULL){
        printf ("%s\n", mysql_error (con));
    }
    FILE * secret_file = common_file_open ("data/secrets", "r");
    FILE * param_file = common_file_open ("data/param.txt", "r");
    char param_buf[PARAM_LENGTH];
    param_buf[0] = 0; //null char at beginning


    pbc_param_t param;
    pairing_t pairing;

    //get parameters
    read_entire_file (param_file, param_buf);
    common_file_close (param_file);

    pbc_param_init_set_str(param, param_buf);
    pairing_init_pbc_param (pairing, param);

    element_t secret_key, g, sw_seed, tag_seed, m, h;
    element_t sigma, temp, sig_temp;

    element_init_G2(g, pairing);
    element_init_G2(h, pairing);
    element_init_G2(temp, pairing);
    element_init_G2(sig_temp, pairing);
    element_init_G2(sigma, pairing);
    element_init_Zr(secret_key, pairing);
    element_init_Zr(sw_seed, pairing);
    element_init_Zr(tag_seed, pairing);
    element_init_Zr(m, pairing);


    char buff[MAX_KEY_LENGTH] = "";

    //read the keys from file
    //printf ("Loading Keys...");
    fscanf (secret_file,"%*s %s",buff);
    unsigned char * g_bytes = (unsigned char *)malloc (strlen (buff) * sizeof (unsigned char));
    binary2bytes (buff, g_bytes);
    element_from_bytes_compressed (g, g_bytes);
    free (g_bytes);
    fscanf (secret_file,"%*s %s",buff);
    element_set_str(secret_key, buff, 2);
    fscanf (secret_file,"%*s %s",buff);
    //skip public_key
    fscanf (secret_file,"%*s %s",buff);
    element_set_str(sw_seed, buff, 2);
    fscanf (secret_file,"%*s %s",buff);
    element_set_str(tag_seed, buff, 2);
    //printf("Done.\n");

    unsigned char w[MAX_KEYWORD_LENGTH] = "";
    unsigned char w2[MAX_KEYWORD_LENGTH] = "";
    unsigned char pos[SHA_DIGEST_LENGTH] = "";
    unsigned char sw[SHA_DIGEST_LENGTH + MAX_NID] = ""; //has to store i during concat
    unsigned char tag[SHA_DIGEST_LENGTH + MAX_ID_LENGTH + MAX_NID] = ""; //has to store id and i during concat

    mpz_t q, gmp_id, m_temp, r, gmp_temp;
    mpz_init (q);
    mpz_init (gmp_id);
    mpz_init (m_temp);
    mpz_init (r);
    mpz_init (gmp_temp);
    set_q (buff); //get the group order
    mpz_set_str (q, buff, 10);
    //--------------
    FILE * newFile = common_file_open (id, "r");
    int i = 0;
    char query_[200] = "";
    while (fscanf (newFile, "%s", buff) == 1){
        strcpy(query_, "SELECT IFNULL( (SELECT c FROM keywords WHERE keyword='");
        strcat(query_, buff);
        strcat(query_, "'),-1)");
        mysql_query (con, query_);
        
        MYSQL_RES * result = mysql_store_result (con);
        MYSQL_ROW row;
        row = mysql_fetch_row (result);
        int kcount = atoi(row[0]);
        if (kcount == -1)
            continue;

        //update the count
        memset(query_,0,200);
        strcpy(query_, "UPDATE keywords SET c=");
        sprintf (query_ + strlen (query_), "%d", kcount +1);
        strcat (query_," WHERE keyword='");
        strcat (query_,buff);
        strcat (query_,"'");

        mysql_query (con, query_);
        kcount++;
        //add the id to the id file
        if (kcount == 1){
            //new file needs to be created
            FILE * id_file = create_new_id_file(buff);
            fprintf (id_file, "%s\n", id);
            common_file_close (id_file);
        }
        else{
            // should check whether the id is already present or not
            FILE * id_file = get_id_file (buff);
            fseek (id_file, 0 , SEEK_END);
            fprintf (id_file, "%s\n", id);
            common_file_close (id_file);
        }

        //create the pos and signature pair
        strcpy (w, buff);
        strcpy (w2,w);
        //appending sw_seed to the end of w
        element_to_bytes (w + strlen (w), sw_seed);
        element_to_bytes (w2 + strlen (w2), tag_seed);
        F (w, sw);
        memset (tag, 0, SHA_DIGEST_LENGTH + MAX_ID_LENGTH + MAX_NID);
        F (w2, tag);

        //calculate r
        //append i to the end of sw
        sprintf (sw + strlen (sw), "%d", kcount);
        unsigned long int rtemp = R (sw);
        //remove end digit from sw
        int c_copy = kcount;
        while (c_copy){
        sw[strlen (sw) - 1] = 0;
        c_copy /= 10;
        }
        mpz_set_ui (r, rtemp);

        byte2binary_total (id, buff);
        mpz_set_str (gmp_id, buff, 2);

        //calculate m
        mpz_mul (gmp_temp, r, gmp_id);
        mpz_mod (m_temp, gmp_temp, q);
        element_set_mpz (m, m_temp);



        //  sign
        element_pow_zn (temp, g, m);
        element_pow_zn (sigma, temp, secret_key);

        //calculate pos
        strcat (tag, id);
        sprintf (tag + strlen (tag), "%d", kcount);

        F (tag, pos);
        tag[strlen (tag) - 1] = 0;
        int sigma_length = element_length_in_bytes_compressed (sigma);

        unsigned char * sigma_bytes = (unsigned char *)malloc (sigma_length * sizeof (unsigned char));
        element_to_bytes_compressed (sigma_bytes, sigma);

        //enter into database
        unsigned char * query = (unsigned char *)malloc (2000 * sizeof (unsigned char));
        strcpy (query, "INSERT INTO signatures VALUES('");
        for (int t = 0; t < strlen (pos); t++){
          strcat (query, byte_to_binary (pos[t]));
        }
        strcat (query, "','");
      	if (sigma_length == 0){
      		printf ("Failed to sign\n");
      	}
        for (int t = 0; t < sigma_length; t++){
          strcat (query, byte_to_binary (sigma_bytes[t]));
        }
        free (sigma_bytes);
        strcat (query, "')");
        if (mysql_query (con, query)){
            printf ("%s\n", mysql_error(con));
        }
        free (query);

        i++;
	    mysql_free_result(result);
        memset(buff, 0, MAX_KEY_LENGTH);
        memset(query_, 0, 200);
    }

    element_clear (secret_key);
    element_clear (g);
    element_clear (sw_seed);
    element_clear (tag_seed);
    element_clear (m);
    element_clear (h);
    element_clear (sigma);
    element_clear (temp);
    element_clear (sig_temp);
}
