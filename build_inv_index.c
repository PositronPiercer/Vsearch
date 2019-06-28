#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"
#include <string.h>
#include <stdbool.h>

bool keyword_is_present (int count, char keyword_all[count][MAX_KEYWORD_LENGTH], char * buff){
    for (int i = 0; i < count; i++){
        if (!strcmp (keyword_all[i], buff)) {
            strcpy (keyword_all[i],"");
            return true;
        }
    }
    return false;
}

int get_keywords_from_file (FILE * input_file, int count, char keyword_present[count][MAX_KEYWORD_LENGTH], char keyword_all[count][MAX_KEYWORD_LENGTH], int kcount[count]){
    //returns the number of keywords present
    //fills keyword_all , kcount and keyword_present
    char keyword_all_duplicate[count][MAX_KEYWORD_LENGTH];
    FILE * keywords = fopen ("keywords", "r");
    char buff[MAX_KEY_LENGTH];
    int tmpInt;


    for (int i = 0; i < count; i++){
        fscanf (keywords, "%s %d", buff, &tmpInt);
        strcpy (keyword_all [i], buff);
        strcpy (keyword_all_duplicate [i], buff);
        //printf ("%s\n", keyword_all [i]);
        kcount[i] = tmpInt;
    }
    fclose (keywords);
    int j = 0;
    while (fscanf (input_file, "%s", buff) == 1){
        if (keyword_is_present (count, keyword_all_duplicate, buff)){
            strcpy (keyword_present[j], buff);
            j++;
        }
    }
    return j;

}
// int main (){
//     int count = 3;
//     FILE * input_file = fopen ("input_files/input1.txt", "r");
//     char kp[count][MAX_KEYWORD_LENGTH];
//     int h = get_keywords_from_file(input_file, count, kp);
//     for (int i = 0; i < h; i++){
//         printf ("present %s\n", kp[i]);
//     }
// }
