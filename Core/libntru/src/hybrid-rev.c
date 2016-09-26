#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ntru.h"
#include <openssl/evp.h>

/*****************************************************************************
 * Sample code that shows how to do hybrid encryption using NTRU and AES.    *
 *****************************************************************************/

int openssl_encr_decr(uint8_t *inbuf, int inlen, uint8_t *outbuf, int *outlen, uint8_t *key, uint8_t *iv, int do_encrypt) {
    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);
    EVP_CipherInit_ex(&ctx, EVP_aes_128_cbc(), NULL, NULL, NULL, do_encrypt);
    EVP_CipherInit_ex(&ctx, NULL, NULL, key, iv, do_encrypt);
    int retval = EVP_CipherUpdate(&ctx, outbuf, outlen, inbuf, inlen);
    int pad_bytes = 0;
    if (retval && !EVP_CipherFinal_ex(&ctx, outbuf+*outlen, &pad_bytes))
        retval = 0;
    *outlen += pad_bytes;
    EVP_CIPHER_CTX_cleanup(&ctx);
    return retval;
}

/**
 * @brief Hybrid Encryption
 *
 * Encrypts a message of arbitrary length.
 *
 * @param msg The message to encrypt
 * @param msg_len length of msg
 * @param pub the public key to encrypt the message with
 * @param params the NtruEncrypt parameters to use
 * @param rand_ctx an initialized random number generator. See ntru_rand_init() in rand.h.
 * @param enc output parameter; a pointer to store the encrypted message. Must accommodate
 *            ntru_enc_len(params)+msg_len+16 bytes.
 * @param enc_len output parameter; number of bytes written
 * @return NTRU_SUCCESS on success, or one of the NTRU_ERR_ codes on failure; 99 for OpenSSL error
 */
uint8_t ntru_encrypt_hybrid(uint8_t *msg, uint16_t msg_len, NtruEncPubKey *pub, const NtruEncParams *params, NtruRandContext *rand_ctx, uint8_t *enc, int *enc_len) {
    uint8_t key_iv[32];   /* key + iv */
    if (ntru_rand_generate(key_iv, 32, rand_ctx) != NTRU_SUCCESS)
        return NTRU_ERR_PRNG;
    int retval = ntru_encrypt(key_iv, 32, pub, params, rand_ctx, enc);   /* put encrypted sym key + iv at the beginning */
    int outlen;
    if (!openssl_encr_decr(msg, msg_len, enc+ntru_enc_len(params), &outlen, key_iv, key_iv+16, 1))   /* followed by the encrypted msg */
        retval = 99;
    *enc_len = outlen;
    *enc_len += ntru_enc_len(params);    /* add length of encrypted sym key + iv */

    memset(key_iv, 0, 32);
    return retval;
}

/**
 * @brief Hybrid Decryption
 *
 * Decrypts a message encrypted with ntru_encrypt_hybrid().
 *
 * @param enc The message to decrypt
 * @param enc_len length of enc
 * @param kp a key pair that contains the public key the message was encrypted
 *           with, and the corresponding private key
 * @param params the NtruEncrypt parameters the message was encrypted with
 * @param dec output parameter; a pointer to store the decrypted message. Must accommodate
 *            enc_len-ntru_enc_len(params) bytes.
 * @param dec_len output parameter; pointer to store the length of dec
 * @return NTRU_SUCCESS on success, or one of the NTRU_ERR_ codes on failure; 99 for OpenSSL error
 */
uint8_t ntru_decrypt_hybrid(uint8_t *enc, int enc_len, NtruEncKeyPair *kp, const NtruEncParams *params, uint8_t *dec, int *dec_len) {
    uint8_t key_iv[32];
    uint16_t key_len;
    uint8_t retval = ntru_decrypt(enc, kp, params, key_iv, &key_len);
    if (retval != NTRU_SUCCESS)
        return retval;

    if (!openssl_encr_decr(enc+ntru_enc_len(params), enc_len-ntru_enc_len(params), dec, dec_len, key_iv, key_iv+16, 0))
        retval = 99;

    return retval;
}

int main(int arc, char **argv) {
    char plain_char[160];
    strcpy(plain_char, "This text is a good fit in an NTRU message, so we'll not use " \
                       "symmetric encryption and instead use the NTRU PKI to encrypt it.");
    //    uint8_t plain[strlen(plain_char)];
    uint8_t plain[170];    
    char password_char[160];

    printf("enter a password: ");
    fgets(password_char, 160, stdin);
    password_char[(strlen(password_char) - 1)]='\0';
    uint8_t password[strlen(password_char)];
    
    unsigned i;
    //    for (i=0; i<strlen(plain_char); i++)
    //        plain[i] = plain_char[i];
    for (i=0; i<strlen(password_char); i++)
        password[i] = password_char[i];

    /* generate an NTRU key */
    NtruEncKeyPair kp;
    NtruRandGen rng = NTRU_RNG_DEFAULT;
    NtruRandContext rand_ctx;
    NtruRandGen rng_sk = NTRU_RNG_DEFAULT;
    NtruRandContext rand_sk_ctx;

    if (ntru_rand_init_det(&rand_ctx, &rng, password, strlen(password)) != NTRU_SUCCESS)
        printf("rng fail\n");
    if (ntru_gen_key_pair(&EES1087EP2, &kp, &rand_ctx) != NTRU_SUCCESS)
        printf("keygen fail\n");
    if (ntru_rand_init(&rand_sk_ctx, &rng_sk) != NTRU_SUCCESS)
        printf("rng_sk fail\n");
    if (ntru_rand_generate(plain, 64, &rand_sk_ctx) != NTRU_SUCCESS) {
        return NTRU_ERR_PRNG;
    } else {
      printf("generated 64 byte random symmetric key\n");
    }
    char buffer_int[4];
    int randomID = arc4random() % 2000 + 1000;
    printf("hash multiplier: [%i]\n",randomID);
    sprintf(buffer_int,"%i",randomID);
    for (i=64; i<68; i++)
      plain[i] = buffer_int[(i-64)];
    randomID=0;
    
    printf("256 bit mode comparison:\n");
    printf("size of ciphertext EES1499EP1 params: %i\n",ntru_enc_len(&EES1499EP1));
    printf("max length of plaintext EES1499EP1 params: %i\n",ntru_max_msg_len(&EES1499EP1));
    printf("size of ciphertext EES1087EP2 params: %i\n",ntru_enc_len(&EES1087EP2));
    printf("max length of plaintext EES1087EP2 params: %i\n",ntru_max_msg_len(&EES1087EP2));
    printf("size of ciphertext EES1171EP1 params: %i\n",ntru_enc_len(&EES1171EP1));
    printf("max length of plaintext EES1171EP1 params: %i\n",ntru_max_msg_len(&EES1171EP1));
    printf("size of ciphertext NTRU_DEFAULT_PARAMS_128_BITS params: %i\n",ntru_enc_len(&NTRU_DEFAULT_PARAMS_128_BITS));
    printf("max length of plaintext NTRU_DEFAULT_PARAMS_128_BITS params: %i\n",ntru_max_msg_len(&NTRU_DEFAULT_PARAMS_128_BITS));
    printf("size of ciphertext NTRU_DEFAULT_PARAMS_256_BITS params: %i\n",ntru_enc_len(&NTRU_DEFAULT_PARAMS_256_BITS));
    printf("max length of plaintext NTRU_DEFAULT_PARAMS_256_BITS params: %i\n",ntru_max_msg_len(&NTRU_DEFAULT_PARAMS_256_BITS));
    printf("192 bits:\n");
    printf("size of ciphertext EES887EP1 params: %i\n",ntru_enc_len(&EES887EP1));
    printf("max length of plaintext EES887EP1 params: %i\n",ntru_max_msg_len(&EES887EP1));
    printf("size of ciphertext EES1087EP1 params: %i\n",ntru_enc_len(&EES1087EP1));
    printf("max length of plaintext EES1087EP1 params: %i\n",ntru_max_msg_len(&EES1087EP1));

    /* encrypt */
    uint8_t enc[ntru_enc_len(&EES1087EP2)+strlen(plain_char)+16];
    // int enc_len;
    //    if (ntru_encrypt_hybrid(plain, strlen(plain_char), &kp.pub, &EES1087EP2, &rand_ctx, enc, &enc_len) != NTRU_SUCCESS)
    //        printf("encrypt fail\n");
    //    if (ntru_encrypt(plain, strlen(plain_char), &kp.pub, &EES1087EP2, &rand_ctx, enc) != NTRU_SUCCESS)
    //  printf("encrypt fail\n");
    if (ntru_encrypt(plain, 170, &kp.pub, &EES1087EP2, &rand_ctx, enc) != NTRU_SUCCESS)
      printf("encrypt fail\n");

    /* decrypt */
    uint8_t dec[ntru_enc_len(&EES1087EP2)];
    int dec_len;
    //    if (ntru_decrypt_hybrid((uint8_t*)&enc, enc_len, &kp, &EES1087EP2, (uint8_t*)&dec, &dec_len) != NTRU_SUCCESS)
    //    printf("decrypt fail\n");
    if (ntru_decrypt((uint8_t*)&enc, &kp, &EES1087EP2, (uint8_t*)&dec, &dec_len) != NTRU_SUCCESS)
      printf("decrypt fail\n");

    dec[dec_len] = 0;   /* string terminator */
    ntru_rand_release(&rand_ctx);

    printf("encryption+decryption\n ");
    //    printf(strcmp((char*)plain, (char*)dec)==0 ? "successful\n" : "failed\n");
    //    printf("message: [%s]\npassword: [%s]\n",(char *)plain, (char *)dec);
    int dec_success=1;
    for (i=0; i<68; i++)
      if(plain[i] != dec[i])
	dec_success = 0;
    char buffer_intd[4];
    for (i=64; i<68; i++)
      buffer_intd[(i-64)]=dec[i];
    randomID=strtol(buffer_intd, (char **)NULL, 10);
    if (dec_success==1) {
      printf("decrypted generated symmetric key successfully using password [%s]\nhash multiplier: [%i]\n", password_char,randomID);
    } else {
      printf("decryption of generated symmetric key failed\n");
    }
}
