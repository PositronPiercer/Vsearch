#include <my_global.h>
#include <mysql.h>
#include "db_secrets.h"
#include <string.h>

int main(int argc, char **argv){
    printf ("Creating database %s ... ", DATABASE_NAME);
    MYSQL * con = mysql_init(NULL);

    if (con == NULL){
        printf ("Connection to db failed!\n");
        exit (1);
    }

    if (mysql_real_connect (con, "localhost", "root", ROOT_PASS, NULL, 0, NULL, 0) == NULL){
        printf ("%s\n", mysql_error (con));
        mysql_close (con);
        exit (1);
    }

    if (mysql_query(con, "CREATE DATABASE vsearch"))
    {
      fprintf(stderr, "%s\n", mysql_error(con));
      mysql_close(con);
      exit(1);
    }
    printf ("Done \n");

    printf ("Creating tables ... ");
    char command[100] = "";
    strcpy (command,"USE ");
    strcat (command,DATABASE_NAME);
    if (mysql_query(con, command))
    {
      fprintf(stderr, "%s\n", mysql_error(con));
      mysql_close(con);
      exit(1);
    }

    mysql_query(con, "source data/keyw.sql");
    mysql_query(con, "CREATE TABLE signatures(pos VARCHAR(500), sig VARCHAR(500))");
    printf ("Done\n");
    printf ("Loading Keywords ... ");
    FILE * keywords = fopen ("data/korg", "r");
    memset(command, 0, 100);
    char keyw[100] = "";
    int i = 0;
    while (fscanf (keywords, "%s %*d", keyw) == 1){
        i++;
        if (i%100 == 0)
        printf ("%d\n", i);
        strcpy(command, "INSERT INTO keywords VALUES('");
        strcat(command, keyw);
        strcat(command, "',0)");
        mysql_query(con, command);
        memset(command, 0, 100);
        memset(keyw, 0, 100);
    }


    mysql_close(con);
    exit(0);

}
