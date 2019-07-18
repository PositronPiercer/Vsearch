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
    mysql_query(con, "CREATE TABLE signatures(pos VARCHAR(500), sig VARCHAR(500))");
    // if (mysql_query(con, "SOURCE data/keyw.sql")){
    //     printf ("%s\n", mysql_error(con));
    // }
    printf ("Done\n");

    mysql_close(con);
    exit(0);

}
