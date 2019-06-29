#define MAX_KEYWORD_LENGTH 50+8*40 //this has to accomodate Ks or Kt during concat
#define SHA_DIGEST_LENGTH 500
#define MAX_KEY_LENGTH 8*100
#define MAX_ID_LENGTH 120
#define MAX_NID 10
#define LAMBDA 256
#define PARAM_LENGTH 250
#define SIGNATURE_LENGTH 8*100

#include <stdbool.h>

typedef struct n{
  char * tag;
  char * tau_sigma_f;
}token;


void read_entire_file (FILE * fileptr, char * obuf);
void F (unsigned char * ibuf, unsigned char * obuf);
const char *byte_to_binary(int x);
unsigned long int R (unsigned char * ibuf);
FILE * get_id_file(char * keyword);
void binary2bytes(unsigned char * ibuf,unsigned char * obuf);
void set_q (unsigned char * key);
void byte2binary_total (unsigned char * ibuf, unsigned char * obuf);
int get_keywords_from_file (FILE * input_file, int count, char keyword_present[count][MAX_KEYWORD_LENGTH], char keyword_all[count][MAX_KEYWORD_LENGTH], int kcount[count]);
bool keyword_is_present (int count, char keyword_all[count][MAX_KEYWORD_LENGTH], char * buff);
FILE * create_new_id_file(char * keyword);
