#include <openssl/rc4.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#define SALT_SIZE       8
#define KEY_SIZE        16
#define SALT_BUF_SIZE   16
#define BUFF_SIZE       4096

/*
    Usage: ./main [-e | -d] [-salt | -no-salt] [key] [input file] [out_file]
        -e - encrypt or -d - decrypt
*/

int main(int argc, char* argv[]) {
    int inFile;
    int outFile;
    unsigned char transformedKey[KEY_SIZE];
    RC4_KEY key;
    char in_Buf[BUFF_SIZE];
    char out_Buf[BUFF_SIZE];

    if (argc < 6){
        printf("Error: Missing arguments.\n");
        printf("Format: ./main [-e | -d] [-salt | -no-salt] [key] [input file] [out_file]\n");
        exit(-1);
    }
    else if (argc > 6){
        printf("Error: Excessive arguments.\n");
        printf("Format: ./main [-e | -d] [-salt | -no-salt] [key] [input file] [out_file]\n");
        exit(-1);
    }
    if (! (strcmp(argv[1], "-e") == 0 || strcmp(argv[1], "-d") == 0) ){
        printf("Error: Incorrect [-e | -d] input.\n");
        printf("Format: ./main [-e | -d] [-salt | -no-salt] [key] [input file] [out_file]\n");
        exit(-1);
    }

    if (! (strcmp(argv[2], "-salt") == 0 || strcmp(argv[2], "-no-salt") == 0) ){
        printf("Error: Incorrect [-salt | -no-salt] input.\n");
        printf("Format: ./main [-e | -d] [-salt | -no-salt] [key] [input file] [out_file]\n");
        exit(-1);
    }

    if (access(argv[4], F_OK) != 0){
        printf("Error: Input File does not exist.\n");
        printf("Format: ./main [-e | -d] [-salt | -no-salt] [key] [input file] [out_file]\n");
        exit(-1);
    }

    // opens file for reading 
    inFile = open(argv[4], O_RDONLY);

    // opens file for create and write, gives most permissions over file to current user 
    outFile = open(argv[5], O_CREAT | O_WRONLY, 0644);

    if (strcmp(argv[2], "-salt") == 0){
        // SALT
        unsigned char salt[SALT_SIZE];

        if (strcmp(argv[1], "-e") == 0){
            // Encryption
            char saltBuf[SALT_BUF_SIZE];
            memset(saltBuf, 0, SALT_BUF_SIZE);
            RAND_bytes(salt, SALT_SIZE); 
            sprintf(saltBuf, "Salted__%c%c%c%c%c%c%c",
            salt[0], salt[1], salt[2], salt[3],
            salt[4], salt[5], salt[6]);
            saltBuf[15] = salt[7];
            write(outFile, &saltBuf, SALT_BUF_SIZE); 
        }

        else if (strcmp(argv[1], "-d") == 0){
        // Decryption
            lseek(inFile, 8, SEEK_SET); 
            read(inFile, salt, SALT_SIZE);
        }

        // SALT KEY
        if (! EVP_BytesToKey(EVP_rc4(), EVP_sha256(),
        salt, (unsigned char *)argv[3], strlen(argv[3]), 1, transformedKey, NULL) )
        {
            printf("Error: Could not create encryption key.\n");
            exit(-1);
        }
        RC4_set_key(&key, KEY_SIZE, transformedKey);
    }
    else {
        // NO SALT KEY
        if (! EVP_BytesToKey(EVP_rc4(), EVP_sha256(),
        NULL, (unsigned char *)argv[3], (int)strlen(argv[3]), 1, transformedKey, NULL) )
        {
            printf("Error: Could not create encryption key.\n");
            exit(-1);
        }
        RC4_set_key(&key, KEY_SIZE, transformedKey);
    }

    // WRITE TO FILE
    ssize_t bytesRead = 0;
    while (bytesRead = read(inFile, &in_Buf, BUFF_SIZE)) {
        RC4(&key, bytesRead, (const unsigned char*) in_Buf, (unsigned char*) out_Buf);
        write(outFile, &out_Buf, bytesRead);
    }

    // handle closing files
    close (inFile);
    close (outFile);

    return 0;
}