# C implementation of a Forward Secure Publicly Verifiable DSSE Scheme
The paper is avalable at -

## Make sure these libraries are installed before proceeding
* [PBC](https://crypto.stanford.edu/pbc/)
* [GMP](https://gmplib.org/)
* [OpenSSL](https://www.openssl.org/)
* `mysql-server`  `mysql-client`  `libmysqlclient-dev`

For building the inverted index, all the test files must be in same directory

## Running Instructions
* Download or clone the repo
* Modify the `include/db_secrets` according to you database configuration
* Run `make` to create the binaries
* Run `./init_db` to setup the database
* Now you have to load the keyword table into your database. Login to your database and enter `SOURCE data/keyw.sql;` ( replace 'data/keyw.sql' with the address of the keyw.sql file.
* Run `python3 create_inv_index.py` to create the inverted index (Note that building inverted index is not a part of the algorithm and a hack has been used to build it, therefore it is required that you run `./vsearch 1`before building the inverted index)
* Run `./vsearch` with the required parameter to execute

