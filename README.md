# C implementation of a Forward Secure Publicly Verifiable DSSE Scheme
The paper is avalable at -

## Make sure these libraries are installed before proceeding
* [PBC](https://crypto.stanford.edu/pbc/)
* [GMP](https://gmplib.org/)
* [OpenSSL](https://www.openssl.org/)

For building the inverted index, all the test files must be in same directory

## Running Instructions
* Download or clone the repo
* Run `make` to create the binaries
* Run `python create_inv_index.py` to create the inverted index (Note that building inverted index is not a part of the algorithm and a hack has been used to build it, therefore it is required that you run `./vsearch` and then select keygen before building the inverted index)
* Run `./vsearch` to execute
