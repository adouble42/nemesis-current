#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "ntru.h"
// #include <openssl/evp.h>
#define NTRU_PRIVLEN 339
#define NTRU_ENCLEN 1495
#define NTRU_PUBLEN 1499
#define PRIVATE_TLEN 39
#define PUBLIC_TLEN 38
#define PRIVATE_KEYTAG "-----BEGIN NTRU PRIVATE KEY BLOCK-----"
#define PUBLIC_KEYTAG "-----BEGIN NTRU PUBLIC KEY BLOCK-----"
/*****************************************************************************
 * lightweight NTRU public key in block mode, with SHAKE-256 stream cipher   *
 *****************************************************************************/

/*
Implementation by the Keccak, Keyak and Ketje Teams, namely, Guido Bertoni,
Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer, hereby
denoted as "the implementer".

For more information, feedback or questions, please refer to our websites:
http://keccak.noekeon.org/
http://keyak.noekeon.org/
http://ketje.noekeon.org/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

typedef unsigned char kc_u8;
typedef unsigned long long kc_u64;
#define FOR(i,n) for (i = 0;i < n;++i)
#define H(i,r,p,d) int crypto_hash_##i(kc_u8 *h,const kc_u8 *m,kc_u64 n) { Keccak(r*8,m,n,6+25*p,h,d); return 0; }



static kc_u64 ROL(kc_u64 a, kc_u8 n)
{
  return (a << n) | (a >> (64-n));
}

static kc_u64 L64(const kc_u8 *x)
{
  kc_u64 r=0,i;
  FOR(i,8) r |= (kc_u64)x[i] << 8*i;
  return r;
}

static void F(kc_u64 *s)
{
  kc_u8 x, y, j, R=1, r, n;
  kc_u64 t, B[5], Y;

  FOR(n,24)
  {
    FOR(x,5)
    {
      B[x] = 0;
      FOR(y,5) B[x] ^= s[x+5*y];
    }

    FOR(x,5)
    {
      t = B[(x+4)%5] ^ ROL(B[(x+1)%5], 1);
      FOR(y,5) s[x+5*y] ^= t;
    }

    t = s[1];
    y = r = 0;
    x = 1;
    FOR(j,24)
    {
      r += j+1;
      Y=2*x+3*y;
      x=y;
      y=Y%5;
      Y = s[x+5*y];
      s[x+5*y] = ROL(t, r%64);
      t = Y;
    }

    FOR(y,5)
    {
      FOR(x,5) B[x] = s[x+5*y];
      FOR(x,5) s[x+5*y] = B[x] ^(~B[(x+1)%5] & B[(x+2)%5]);
    }

    FOR(y,7)
        if ((R=(R<<1)^(113*(R>>7))) & 2) *s^= 1ULL<<((1<<y)-1);
  }
}

static void Keccak(kc_u8 r, const kc_u8 *m, kc_u64 n, kc_u8 p, kc_u8 *h, kc_u64 d)
{
  kc_u64 s[25],i;
  kc_u8 t[200];

  FOR(i,25)
    s[i] = 0;

  while(n>=r)
  {
    FOR(i,r/8)
      s[i] ^= L64(m+8*i);
    F(s);
    n -= r;
    m += r;
  }

  FOR(i,r)
    t[i] = 0;
  FOR(i,n)
    t[i] = m[i];
  t[i] = p;
  t[r-1] |= 128;

  FOR(i,r/8)
    s[i] ^= L64(t+8*i);
  F(s);

  FOR(i,d)
    h[i] = s[i/8]>>8*(i%8);
}


//void Keccak(unsigned int rate, unsigned int capacity, const unsigned char *input, unsigned long long int inputByteLen, unsigned char delimitedSuffix, unsigned char *output, unsigned long long int outputByteLen);
int crypto_hash_shake256(kc_u8 *h, const kc_u8 *m, kc_u64 n);

/**
  *  Function to compute SHAKE256 on the input message with any output length.
  */
void FIPS202_SHAKE256(const unsigned char *input, unsigned int inputByteLen, unsigned char *output, int outputByteLen)
{
  //    Keccak(1088, 512, input, inputByteLen, 0x1F, output, outputByteLen);
          unsigned char buffer[136];
          crypto_hash_shake256(buffer, input, inputByteLen);
          memcpy(output, buffer, outputByteLen);
}

int zChrSearch(char *token,char s, int len){
    if (!token)
        return 0;

    for (;*token; token++)
        if (*token == s)
            return 1;

    return 0;
}

char *zStrrmv(char *str,char *newstr, char *bad, int len) {
//    *newstr = (char *)malloc(sizeof(str) * sizeof(char));
    if (!newstr) newstr = (char *)malloc(sizeof(str) * sizeof(char));

    char *src = str;
    newstr = str;
    while(*src)
        if(zChrSearch(bad,*src, len))
            *src++;
        else
            *newstr++ = *src++;  /* assign first, then increment */

    newstr[len]='\0';
}

/* utility function to convert hex character representation to their 
 * nibble (4 bit) values
 */
static uint8_t
nibbleFromChar(char c)
{
	if(c >= '0' && c <= '9') return c - '0';
	if(c >= 'a' && c <= 'f') return c - 'a' + 10;
	if(c >= 'A' && c <= 'F') return c - 'A' + 10;
	return 255;
}

/* Convert a string of characters representing a hex buffer into a 
 * series of bytes of that real value
 */
void
hexStringToBytes(char *inhex, uint8_t *arr, int keysize)
{
//	*arr = (uint8_t *)malloc(keysize * sizeof(uint8_t));
	if (!arr) arr = (uint8_t *)malloc(keysize*sizeof(uint8_t));
	uint8_t *retval;
	uint8_t *p;
	int len, i;

    len = strlen(inhex) / 2;
//	retval = malloc(len+1);
	for(i=0, p = (uint8_t *) inhex; i<len; i++) {
		arr[i] = (nibbleFromChar(*p) << 4) | nibbleFromChar(*(p+1));
		p += 2;
	}
}
H(shake256,17,1,136)



static char byteMap[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
static int byteMapLen = sizeof(byteMap);

/* Utility function to convert nibbles (4 bit values) into a hex 
character representation */
static char
nibbleToChar(uint8_t nibble)
{
	if(nibble < byteMapLen) return byteMap[nibble];
	return '*';
}

char
*bytesToHexString(uint8_t *bytes, size_t buflen)
{
	char *retval;
	int i,count,sz;

	retval = malloc(buflen*2 + 1);
	for(i=0; i<buflen; i++) {
                        retval[(i)*2] = nibbleToChar(bytes[i] >> 4);
                        retval[(i)*2+1] = nibbleToChar(bytes[i] & 0x0f);
	}
    retval[i*2+1] = '\0';
	return retval;
}
int main(int argc, char *argv[]) {
    char plain_char[170];
    strcpy(plain_char, "This text is a good fit in an NTRU message, so we'll not use " \
                       "symmetric encryption and instead use the NTRU PKI to encrypt it.");
    char shake_dec[170];
    strcpy(shake_dec, "this is garbage");
    char header_publine[PUBLIC_TLEN];
    char header_privline[PRIVATE_TLEN];
    uint8_t plain[170];    
    char password_char[170];
    FILE *Handle=NULL;
    FILE *input, *output;
    signed char fbuf[170];
    size_t nt;
    uint8_t shake_outp[NTRU_PRIVLEN];
    uint8_t shake_finalp[NTRU_PRIVLEN];
    char pkname[64];
    char skname[64];
    char sfname[64];
    char ofname[64];
    int encMode = 0;
    int decMode = 0;
    int keyGen = 0;
    int show_params = 0;
    
    sprintf (header_publine,"%s\n",PUBLIC_KEYTAG);
    sprintf (header_privline,"%s\n",PRIVATE_KEYTAG);
    strcpy (pkname, "NTRUPublic.key");
    strcpy (skname, "NTRUPrivate.key");
    int opc;
    for (opc = 1; opc < argc; opc++)
    {
	if (strcmp(argv[opc], "--pubkey") == 0) {
		strncpy(pkname, argv[opc+1], 64);
		opc++;
	}
        if (strcmp(argv[opc], "--privkey") == 0) {
                strncpy(skname, argv[opc+1], 64);
                opc++;
        }
        if (strcmp(argv[opc], "--infile") == 0) {
                strncpy(sfname, argv[opc+1], 64);
                opc++;
        }
        if (strcmp(argv[opc], "--enc") == 0) {
                encMode = 1;
		strncpy(ofname, sfname, 64);
		strncat(ofname, ".enc", 64);
        }
        if (strcmp(argv[opc], "--dec") == 0) {
		decMode = 1;
		strncpy(ofname, sfname, 64);
		ofname[(strlen(sfname)-4)] = '\0';
        }
        if (strcmp(argv[opc], "--keygen") == 0) {
		keyGen = 1;
        }
	if (strcmp(argv[opc], "--show-params") == 0) {
	        show_params = 1;
	}
    }
            NtruRandGen rng = NTRU_RNG_DEFAULT;
            NtruRandContext rand_ctx;

    if (keyGen == 1) {
	    static struct termios oldt, newt;
	    tcgetattr( STDIN_FILENO, &oldt);
    	    newt = oldt;

	    /*setting the approriate bit in the termios struct*/
    	    newt.c_lflag &= ~(ECHO);          

	    /*setting the new bits*/
            tcsetattr( STDIN_FILENO, TCSANOW, &newt);

	    printf("enter a strong passphrase to protect the private key on disk: ");
	    fgets(password_char, 100, stdin);
	    printf("\n");
	    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
	    FIPS202_SHAKE256(password_char, strlen(password_char), &shake_outp, NTRU_PRIVLEN);
	    int i;
	    for (i=0; i<10000; i++) { // put the lime in the coconut
	      FIPS202_SHAKE256(shake_outp, NTRU_PRIVLEN, &shake_finalp, NTRU_PRIVLEN);
	      FIPS202_SHAKE256(shake_finalp, NTRU_PRIVLEN, &shake_outp, NTRU_PRIVLEN);
	    }

 	    /* generate an NTRU key */
	    NtruEncKeyPair kp; // master gen set
    if (ntru_rand_init(&rand_ctx, &rng) != NTRU_SUCCESS)
        printf("rng fail\n");
    if (ntru_gen_key_pair(&EES1087EP2, &kp, &rand_ctx) != NTRU_SUCCESS)
        printf("keygen fail\n");
    ntru_rand_release(&rand_ctx);


    uint8_t pub_arr[NTRU_PUBLEN];
    ntru_export_pub(&kp.pub, pub_arr);
    uint8_t priv_arr[NTRU_PRIVLEN];
    ntru_export_priv(&kp.priv, priv_arr);

    printf("exporting hex armored NTRU private key to file %s\n",skname);
    Handle=fopen(skname,"wb");
    if(Handle!=NULL)
    {
      fwrite(header_privline,sizeof(char),sizeof(header_privline),Handle);
      int xl;
      FIPS202_SHAKE256(shake_outp, NTRU_PRIVLEN, &shake_finalp, NTRU_PRIVLEN);
      for (xl=0;xl<NTRU_PRIVLEN;xl++) {
	shake_outp[xl] = priv_arr[xl] ^ shake_finalp[xl];
    //      for (xl=0;xl<170;xl++)
    //	shake_dec[xl] = shake_out[xl] ^ shake_final[xl];
    }

      fwrite(bytesToHexString(shake_outp,sizeof(shake_outp)),sizeof(char) * 2,sizeof(shake_outp),Handle);
      fclose(Handle);
    }

    printf("exporting hex armored NTRU public key to file %s\n",pkname);
    Handle=fopen(pkname,"wb");
    if(Handle!=NULL)
    {
      fwrite(header_publine,sizeof(char),sizeof(header_publine),Handle);
      fwrite(bytesToHexString(pub_arr,sizeof(pub_arr)),sizeof(char) * 2,sizeof(pub_arr),Handle);
//      fwrite(pub_arr,sizeof(pub_arr),1, Handle);
      fclose(Handle);

    }
    exit(EXIT_SUCCESS);
}
//    char buffer_int[4];
//    int randomID = arc4random() % 2000 + 1000;
//    printf("hash multiplier: [%i]\n",randomID);
//    sprintf(buffer_int,"%i",randomID);
//    for (i=64; i<68; i++)
//      plain[i] = buffer_int[(i-64)];
//    randomID=0;
    if (show_params == 1) {
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
    }

    NtruEncKeyPair kr; // recover from file
    char *stripchars = "\r\n";
    
    uint8_t pub_arr_imp[NTRU_PUBLEN];
    uint8_t priv_arr_imp[NTRU_PRIVLEN]; 

    if (decMode == 1) { // don't load it unless we need it
	    static struct termios oldt, newt;
            tcgetattr( STDIN_FILENO, &oldt);
            newt = oldt;
 
            /*setting the approriate bit in the termios struct*/
            newt.c_lflag &= ~(ECHO);

            /*setting the new bits*/
            tcsetattr( STDIN_FILENO, TCSANOW, &newt);

            printf("enter coconut password to retrieve lime: ");
	    fgets(password_char, 100, stdin);
	    printf("\n");
            tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
	    printf("decrypting SHAKE-256, may take a moment.\n");

	    FIPS202_SHAKE256(password_char, strlen(password_char), &shake_outp, NTRU_PRIVLEN);
	    int i;
	    for (i=0; i<10000; i++) { // put the lime in the coconut
	      FIPS202_SHAKE256(shake_outp, NTRU_PRIVLEN, &shake_finalp, NTRU_PRIVLEN);
	      FIPS202_SHAKE256(shake_finalp, NTRU_PRIVLEN, &shake_outp, NTRU_PRIVLEN);
	    }
	    FIPS202_SHAKE256(shake_outp, NTRU_PRIVLEN, &shake_finalp, NTRU_PRIVLEN);
    printf("decrypting NTRU private key from file %s\n",skname);
    Handle=fopen(skname,"rb");
    if(Handle!=NULL)
    {
      char priv_buf[(sizeof(priv_arr_imp)*2)+30];
      char priv_buf2[(sizeof(priv_arr_imp)*2)+30];
      fseek(Handle,sizeof(header_privline),SEEK_SET);
      fread(priv_buf,(sizeof(char)),(NTRU_PRIVLEN*2)+30,Handle);
      zStrrmv(priv_buf, priv_buf2,stripchars, (NTRU_PRIVLEN*2));

      hexStringToBytes(priv_buf, &priv_arr_imp, NTRU_PRIVLEN);

      int xl;
      for (xl=0;xl<NTRU_PRIVLEN;xl++) {
	shake_outp[xl] = priv_arr_imp[xl] ^ shake_finalp[xl];
      }

      fclose(Handle);
    }
    NtruEncPrivKey krpriv;
    printf("importing\n");
    ntru_import_priv(shake_outp, &krpriv);
    kr.priv = krpriv;
    printf("private key loaded\n");
    }
    printf("importing NTRU public key from file %s\n",pkname);
    Handle=fopen(pkname,"rb");
    if(Handle!=NULL)
    {
      char pub_buf[(1499*2)+60];
      char pub_buf2[(1499*2)+60];
      fseek(Handle,sizeof(header_publine), SEEK_SET);
      fread(pub_buf,(sizeof(char)), (ntru_pub_len(&EES1087EP2)*2)+60,Handle);
      zStrrmv(pub_buf, pub_buf2,stripchars, (ntru_pub_len(&EES1087EP2)*2));
      hexStringToBytes(pub_buf, &pub_arr_imp, ntru_pub_len(&EES1087EP2));
      fclose(Handle);
    }
    NtruEncPubKey krpub;
    ntru_import_pub(pub_arr_imp, &krpub);
    kr.pub=krpub;
    printf("keys imported.\n");

    struct fileHeader {
      int fileSize;
    };
    
if (encMode == 1) {
    NtruRandGen rng_sk = NTRU_RNG_DEFAULT;
    NtruRandContext rand_sk_ctx;
    uint8_t shake_key[170];
    uint8_t stream_block[170];
    uint8_t stream_in[170];
    uint8_t stream_final[170];
    uint8_t enc[NTRU_ENCLEN];
    const void *fptr = (void *) fbuf;
    struct fileHeader myInfo;
    int remainder, xx;
    float blocks;

    struct stat in_info;
    stat(sfname, &in_info);
    input = fopen(sfname, "rb");
    output = fopen(ofname, "wb");
    myInfo.fileSize=in_info.st_size;
    lseek(input,0, SEEK_SET);
    blocks = floor((myInfo.fileSize / 170));
    remainder = (myInfo.fileSize - (170 * blocks));
    myInfo.fileSize=blocks;
    fwrite(&myInfo, sizeof(struct fileHeader), 1, output);
    if (ntru_rand_init(&rand_sk_ctx, &rng_sk) != NTRU_SUCCESS)
        printf("rng_sk fail\n");
    if (ntru_rand_generate(shake_key, 170, &rand_sk_ctx) != NTRU_SUCCESS) {
        return NTRU_ERR_PRNG;
    } else {
      printf("generated 170 byte random key for SHAKE-256 stream\n");
    }
    shake_key[1] = (uint8_t) remainder; // encode actual size of final block
    printf("encrypting: source -  %s | target - %s\n",sfname, ofname);
    if (ntru_encrypt(shake_key, 170, &kr.pub, &EES1087EP2, &rand_sk_ctx, enc) == NTRU_SUCCESS)
	fwrite(enc, sizeof(enc),1, output);
    FIPS202_SHAKE256(shake_key, 170, (unsigned char *) &stream_block, 170);
    while ((nt=fread(fptr,sizeof(char), 170, input)) == 170) {
      fbuf[nt] = '\0';
      for (xx=0;xx<nt;xx++) {
	stream_final[xx] = fbuf[xx] ^ stream_block[xx];
      }
      FIPS202_SHAKE256(fptr, nt, (unsigned char *) &stream_block, 170);
	    if (ntru_encrypt(stream_final, nt, &kr.pub, &EES1087EP2, &rand_sk_ctx, enc) == NTRU_SUCCESS)
		fwrite(enc, sizeof(enc),1,output);
    }
    rewind(input);
    fseek(input, (0-nt), SEEK_END);
    memset(fptr,0,sizeof(fptr));
    fread(fptr,sizeof(char), nt, input);
    fbuf[nt] = '\0';
    for (xx=0;xx<nt;xx++) {
      stream_final[xx] = fbuf[xx] ^ stream_block[xx];
    }
    ntru_encrypt(stream_final, nt, &kr.pub, &EES1087EP2, &rand_sk_ctx, enc);
    fwrite(enc, sizeof(enc),1,output);   
    
    ntru_rand_release(&rand_sk_ctx);
    fclose(input);
    fclose(output);
}
if (decMode == 1) {
    NtruRandGen rng_sk = NTRU_RNG_DEFAULT;
    NtruRandContext rand_sk_ctx;
    uint8_t decp[NTRU_ENCLEN];
    uint8_t dec[NTRU_ENCLEN];
    uint8_t shake_key[170];
    uint8_t stream_block[170];
    uint8_t stream_in[170];
    uint8_t stream_final[170];
    struct fileHeader myInfo;
    const void *decptr = (void *) decp;
    int offset, xx, dec_len, blocks, remainder;

    if (ntru_rand_init(&rand_sk_ctx, &rng_sk) != NTRU_SUCCESS)
        printf("rng_sk fail\n");
    input = fopen(sfname, "rb");
    output = fopen(ofname, "wb");
    fread(&myInfo,sizeof(struct fileHeader),1,input);
    fread(decptr,sizeof(char),1495,input);
    ntru_decrypt(decptr, &kr, &EES1087EP2,&shake_key, (uint16_t *) &dec_len);
    printf("decrypting: source -  %s | target - %s\n",sfname, ofname);
    FIPS202_SHAKE256(shake_key, 170, (unsigned char *) &stream_block, 170);
    blocks = myInfo.fileSize;
    remainder = (int) shake_key[1];
    int blockCount = 0;
    while ((nt=fread(decptr,sizeof(char),1495, input)) == 1495) {
      blockCount++;
      ntru_decrypt(decptr, &kr, &EES1087EP2, (uint8_t *) &dec, &dec_len);
      if ((myInfo.fileSize + 1) == blockCount)
	dec_len = remainder;
      for (xx=0;xx<dec_len;xx++)
	stream_final[xx] = dec[xx] ^ stream_block[xx];	
      FIPS202_SHAKE256(stream_final, dec_len, (unsigned char *) &stream_block, dec_len);
      fwrite((uint8_t *)stream_final, sizeof(char) * dec_len,1,output);
    }
    ntru_rand_release(&rand_sk_ctx);
    fclose(input);
    fclose(output);
}
}
