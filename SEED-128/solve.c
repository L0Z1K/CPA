#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <stdint.h>

typedef uint32_t ULONG;
typedef uint8_t UCHAR;

ULONG T;
#define TwoWordLRot(A,B) 				\
		T = A;							\
		A = (A<<8) ^ (B>>24);			\
		B = (B<<8) ^ (T>>24);           \

#define TwoWordRRot(A,B) 				\
		T = A;							\
		A = (A>>8) ^ (B<<24);			\
		B = (B>>8) ^ (T<<24);			\

const UCHAR SEED_S1box[256] = {0xa9, 0x85, 0xd6, 0xd3, 0x54, 0x1d, 0xac, 0x25, 0x5d, 0x43, 0x18, 0x1e, 0x51, 0xfc, 0xca, 0x63, 0x28, 0x44, 0x20, 0x9d, 0xe0, 0xe2, 0xc8, 0x17, 0xa5, 0x8f, 0x03, 0x7b, 0xbb, 0x13, 0xd2, 0xee, 0x70, 0x8c, 0x3f, 0xa8, 0x32, 0xdd, 0xf6, 0x74, 0xec, 0x95, 0x0b, 0x57, 0x5c, 0x5b, 0xbd, 0x01, 0x24, 0x1c, 0x73, 0x98, 0x10, 0xcc, 0xf2, 0xd9, 0x2c, 0xe7, 0x72, 0x83, 0x9b, 0xd1, 0x86, 0xc9, 0x60, 0x50, 0xa3, 0xeb, 0x0d, 0xb6, 0x9e, 0x4f, 0xb7, 0x5a, 0xc6, 0x78, 0xa6, 0x12, 0xaf, 0xd5, 0x61, 0xc3, 0xb4, 0x41, 0x52, 0x7d, 0x8d, 0x08, 0x1f, 0x99, 0x00, 0x19, 0x04, 0x53, 0xf7, 0xe1, 0xfd, 0x76, 0x2f, 0x27, 0xb0, 0x8b, 0x0e, 0xab, 0xa2, 0x6e, 0x93, 0x4d, 0x69, 0x7c, 0x09, 0x0a, 0xbf, 0xef, 0xf3, 0xc5, 0x87, 0x14, 0xfe, 0x64, 0xde, 0x2e, 0x4b, 0x1a, 0x06, 0x21, 0x6b, 0x66, 0x02, 0xf5, 0x92, 0x8a, 0x0c, 0xb3, 0x7e, 0xd0, 0x7a, 0x47, 0x96, 0xe5, 0x26, 0x80, 0xad, 0xdf, 0xa1, 0x30, 0x37, 0xae, 0x36, 0x15, 0x22, 0x38, 0xf4, 0xa7, 0x45, 0x4c, 0x81, 0xe9, 0x84, 0x97, 0x35, 0xcb, 0xce, 0x3c, 0x71, 0x11, 0xc7, 0x89, 0x75, 0xfb, 0xda, 0xf8, 0x94, 0x59, 0x82, 0xc4, 0xff, 0x49, 0x39, 0x67, 0xc0, 0xcf, 0xd7, 0xb8, 0x0f, 0x8e, 0x42, 0x23, 0x91, 0x6c, 0xdb, 0xa4, 0x34, 0xf1, 0x48, 0xc2, 0x6f, 0x3d, 0x2d, 0x40, 0xbe, 0x3e, 0xbc, 0xc1, 0xaa, 0xba, 0x4e, 0x55, 0x3b, 0xdc, 0x68, 0x7f, 0x9c, 0xd8, 0x4a, 0x56, 0x77, 0xa0, 0xed, 0x46, 0xb5, 0x2b, 0x65, 0xfa, 0xe3, 0xb9, 0xb1, 0x9f, 0x5e, 0xf9, 0xe6, 0xb2, 0x31, 0xea, 0x6d, 0x5f, 0xe4, 0xf0, 0xcd, 0x88, 0x16, 0x3a, 0x58, 0xd4, 0x62, 0x29, 0x07, 0x33, 0xe8, 0x1b, 0x05, 0x79, 0x90, 0x6a, 0x2a, 0x9a};
const UCHAR SEED_S1box_inv[256] = {0x5a, 0x2f, 0x80, 0x1a, 0x5c, 0xfa, 0x7c, 0xf6, 0x57, 0x6e, 0x6f, 0x2a, 0x84, 0x44, 0x66, 0xb8, 0x34, 0xa5, 0x4d, 0x1d, 0x75, 0x95, 0xf0, 0x17, 0xa, 0x5b, 0x7b, 0xf9, 0x31, 0x5, 0xb, 0x58, 0x12, 0x7d, 0x96, 0xbb, 0x30, 0x7, 0x8c, 0x63, 0x10, 0xf5, 0xfe, 0xdd, 0x38, 0xc6, 0x79, 0x62, 0x91, 0xe8, 0x24, 0xf7, 0xc0, 0xa0, 0x94, 0x92, 0x97, 0xb2, 0xf1, 0xd0, 0xa3, 0xc5, 0xc9, 0x22, 0xc7, 0x53, 0xba, 0x9, 0x11, 0x9a, 0xdb, 0x89, 0xc2, 0xb1, 0xd6, 0x7a, 0x9b, 0x6b, 0xce, 0x47, 0x41, 0xc, 0x54, 0x5d, 0x4, 0xcf, 0xd7, 0x2b, 0xf2, 0xad, 0x49, 0x2d, 0x2c, 0x8, 0xe4, 0xeb, 0x40, 0x50, 0xf4, 0xf, 0x77, 0xde, 0x7f, 0xb3, 0xd2, 0x6c, 0xfd, 0x7e, 0xbd, 0xea, 0x69, 0xc4, 0x20, 0xa4, 0x3a, 0x32, 0x27, 0xa8, 0x61, 0xd8, 0x4b, 0xfb, 0x88, 0x1b, 0x6d, 0x55, 0x86, 0xd3, 0x8d, 0x9c, 0xae, 0x3b, 0x9e, 0x1, 0x3e, 0x74, 0xef, 0xa7, 0x83, 0x65, 0x21, 0x56, 0xb9, 0x19, 0xfc, 0xbc, 0x82, 0x6a, 0xac, 0x29, 0x8a, 0x9f, 0x33, 0x59, 0xff, 0x3c, 0xd4, 0x13, 0x46, 0xe3, 0xd9, 0x90, 0x68, 0x42, 0xbf, 0x18, 0x4c, 0x99, 0x23, 0x0, 0xcc, 0x67, 0x6, 0x8e, 0x93, 0x4e, 0x64, 0xe2, 0xe7, 0x85, 0x52, 0xdc, 0x45, 0x48, 0xb7, 0xe1, 0xcd, 0x1c, 0xca, 0x2e, 0xc8, 0x70, 0xb4, 0xcb, 0xc3, 0x51, 0xaf, 0x73, 0x4a, 0xa6, 0x16, 0x3f, 0xe, 0xa1, 0x35, 0xee, 0xa2, 0xb5, 0x87, 0x3d, 0x1e, 0x3, 0xf3, 0x4f, 0x2, 0xb6, 0xd5, 0x37, 0xaa, 0xbe, 0xd1, 0x25, 0x78, 0x8f, 0x14, 0x5f, 0x15, 0xe0, 0xec, 0x8b, 0xe6, 0x39, 0xf8, 0x9d, 0xe9, 0x43, 0x28, 0xda, 0x1f, 0x71, 0xed, 0xc1, 0x36, 0x72, 0x98, 0x81, 0x26, 0x5e, 0xab, 0xe5, 0xdf, 0xa9, 0xd, 0x60, 0x76, 0xb0};
const UCHAR SEED_S2box_inv[256] = {0xd6, 0xb9, 0x6b, 0xa7, 0x6d, 0xb7, 0x1b, 0x70, 0x5a, 0xaf, 0xf6, 0x51, 0x94, 0x36, 0xe8, 0x81, 0xa6, 0x1a, 0xb4, 0x5c, 0xc1, 0xa0, 0x26, 0x24, 0x56, 0xb1, 0x83, 0xce, 0x1c, 0xd3, 0xca, 0x4b, 0x69, 0xcc, 0x6c, 0x8e, 0xf2, 0xd4, 0xeb, 0x3c, 0x30, 0x14, 0xbb, 0x39, 0x4d, 0x2, 0x95, 0x3d, 0xe0, 0x63, 0xec, 0x12, 0x4f, 0xa5, 0x1e, 0xf0, 0x0, 0xea, 0x93, 0xfd, 0xe3, 0x2d, 0x33, 0x2c, 0xb3, 0x42, 0x40, 0xbf, 0xc, 0x29, 0xd7, 0x82, 0xc6, 0x9d, 0xfe, 0x1f, 0xf9, 0x8f, 0x31, 0xd5, 0xe9, 0x79, 0xda, 0xf5, 0xd1, 0xa, 0xbe, 0x58, 0xd9, 0x77, 0x99, 0xf, 0x8a, 0x54, 0xbc, 0xcf, 0x9, 0x74, 0x11, 0x8d, 0xa8, 0xe2, 0x3a, 0x45, 0x6e, 0xee, 0x66, 0xe, 0x22, 0xa9, 0x97, 0xd, 0xa2, 0x6f, 0x3f, 0x44, 0xab, 0xa3, 0xb0, 0xd0, 0x9e, 0xc7, 0x3b, 0x89, 0xe7, 0x61, 0xdc, 0xa4, 0x4a, 0x80, 0xcb, 0xfa, 0xed, 0xc0, 0x5e, 0xf7, 0x21, 0xc2, 0x65, 0x47, 0xcd, 0x86, 0x2e, 0xfb, 0x7a, 0x19, 0x9b, 0xef, 0x55, 0xe1, 0x88, 0xc8, 0x2f, 0x73, 0x7c, 0xc3, 0x92, 0x72, 0x91, 0x98, 0x15, 0x8c, 0x8b, 0x7d, 0xf3, 0x34, 0x3, 0x17, 0x23, 0xbd, 0x4e, 0x7e, 0x46, 0x49, 0x59, 0x8, 0xc4, 0x67, 0xd2, 0x6, 0xad, 0x13, 0xe4, 0xff, 0x7, 0x5f, 0x96, 0xe6, 0x1d, 0xb6, 0x75, 0x87, 0x43, 0x62, 0x28, 0x10, 0x25, 0x5b, 0xaa, 0xb, 0x90, 0xde, 0x4c, 0xf4, 0x9f, 0x5d, 0xfc, 0x4, 0x7f, 0x68, 0x50, 0x18, 0x41, 0xac, 0x2b, 0x6a, 0x38, 0xf8, 0xdd, 0x71, 0x7b, 0x78, 0x5, 0x37, 0xb5, 0x2a, 0x16, 0x84, 0xe5, 0xc5, 0x76, 0xf1, 0x1, 0x53, 0xae, 0xdb, 0x85, 0xd8, 0x52, 0x20, 0xba, 0x3e, 0x9a, 0x9c, 0x27, 0x64, 0x32, 0x48, 0x57, 0x35, 0xb8, 0xa1, 0xc9, 0xdf, 0xb2, 0x60};
const UCHAR SEED_S2box[256] = {0x38, 0xe8, 0x2d, 0xa6, 0xcf, 0xde, 0xb3, 0xb8, 0xaf, 0x60, 0x55, 0xc7, 0x44, 0x6f, 0x6b, 0x5b, 0xc3, 0x62, 0x33, 0xb5, 0x29, 0xa0, 0xe2, 0xa7, 0xd3, 0x91, 0x11, 0x06, 0x1c, 0xbc, 0x36, 0x4b, 0xef, 0x88, 0x6c, 0xa8, 0x17, 0xc4, 0x16, 0xf4, 0xc2, 0x45, 0xe1, 0xd6, 0x3f, 0x3d, 0x8e, 0x98, 0x28, 0x4e, 0xf6, 0x3e, 0xa5, 0xf9, 0x0d, 0xdf, 0xd8, 0x2b, 0x66, 0x7a, 0x27, 0x2f, 0xf1, 0x72, 0x42, 0xd4, 0x41, 0xc0, 0x73, 0x67, 0xac, 0x8b, 0xf7, 0xad, 0x80, 0x1f, 0xca, 0x2c, 0xaa, 0x34, 0xd2, 0x0b, 0xee, 0xe9, 0x5d, 0x94, 0x18, 0xf8, 0x57, 0xae, 0x08, 0xc5, 0x13, 0xcd, 0x86, 0xb9, 0xff, 0x7d, 0xc1, 0x31, 0xf5, 0x8a, 0x6a, 0xb1, 0xd1, 0x20, 0xd7, 0x02, 0x22, 0x04, 0x68, 0x71, 0x07, 0xdb, 0x9d, 0x99, 0x61, 0xbe, 0xe6, 0x59, 0xdd, 0x51, 0x90, 0xdc, 0x9a, 0xa3, 0xab, 0xd0, 0x81, 0x0f, 0x47, 0x1a, 0xe3, 0xec, 0x8d, 0xbf, 0x96, 0x7b, 0x5c, 0xa2, 0xa1, 0x63, 0x23, 0x4d, 0xc8, 0x9e, 0x9c, 0x3a, 0x0c, 0x2e, 0xba, 0x6e, 0x9f, 0x5a, 0xf2, 0x92, 0xf3, 0x49, 0x78, 0xcc, 0x15, 0xfb, 0x70, 0x75, 0x7f, 0x35, 0x10, 0x03, 0x64, 0x6d, 0xc6, 0x74, 0xd5, 0xb4, 0xea, 0x09, 0x76, 0x19, 0xfe, 0x40, 0x12, 0xe0, 0xbd, 0x05, 0xfa, 0x01, 0xf0, 0x2a, 0x5e, 0xa9, 0x56, 0x43, 0x85, 0x14, 0x89, 0x9b, 0xb0, 0xe5, 0x48, 0x79, 0x97, 0xfc, 0x1e, 0x82, 0x21, 0x8c, 0x1b, 0x5f, 0x77, 0x54, 0xb2, 0x1d, 0x25, 0x4f, 0x00, 0x46, 0xed, 0x58, 0x52, 0xeb, 0x7e, 0xda, 0xc9, 0xfd, 0x30, 0x95, 0x65, 0x3c, 0xb6, 0xe4, 0xbb, 0x7c, 0x0e, 0x50, 0x39, 0x26, 0x32, 0x84, 0x69, 0x93, 0x37, 0xe7, 0x24, 0xa4, 0xcb, 0x53, 0x0a, 0x87, 0xd9, 0x4c, 0x83, 0x8f, 0xce, 0x3b, 0x4a, 0xb7};
const ULONG SEED_KC[24] = {0x9e3779b9UL, 0x3c6ef373UL, 0x78dde6e6UL, 0xf1bbcdccUL, 0xe3779b99UL, 0xc6ef3733UL, 0x8dde6e67UL, 0x1bbcdccfUL, 0x3779b99eUL, 0x6ef3733cUL, 0xdde6e678UL, 0xbbcdccf1UL, 0x779b99e3UL, 0xef3733c6UL, 0xde6e678dUL, 0xbcdccf1bUL, 0x79b99e37UL, 0xf3733c6eUL, 0xe6e678ddUL, 0xcdccf1bbUL, 0x9b99e377UL, 0x3733c6efUL, 0x6e678ddeUL, 0xdccf1bbcUL};

int startpoint = 0;
int endpoint = 0;
int **data;
UCHAR **plaintext;
double *Sy, *Syy;
int s, t;
ULONG xor_key;
ULONG RK[2][2] = { 0 };

void SEED_G(ULONG* S){
	UCHAR  Y[4];
	
	Y[0] = SEED_S1box[((*S) >>  0) & 0xFF];
	Y[1] = SEED_S2box[((*S) >>  8) & 0xFF];		
	Y[2] = SEED_S1box[((*S) >> 16) & 0xFF];
	Y[3] = SEED_S2box[((*S) >> 24) & 0xFF];
	
	(*S) = ((ULONG)((Y[0] & 0xFC) ^ (Y[1] & 0xF3) ^ (Y[2] & 0xCF) ^ (Y[3] & 0x3F)) <<  0) |
		   ((ULONG)((Y[0] & 0xF3) ^ (Y[1] & 0xCF) ^ (Y[2] & 0x3F) ^ (Y[3] & 0xFC)) <<  8) |
	       ((ULONG)((Y[0] & 0xCF) ^ (Y[1] & 0x3F) ^ (Y[2] & 0xFC) ^ (Y[3] & 0xF3)) << 16) |
	       ((ULONG)((Y[0] & 0x3F) ^ (Y[1] & 0xFC) ^ (Y[2] & 0xF3) ^ (Y[3] & 0xCF)) << 24) ;
}

void SEED_G_INV(ULONG *S){
    UCHAR Z[4];
    Z[0] = ((*S)>>0) & 0xFF;
    Z[1] = ((*S)>>8) & 0xFF;
    Z[2] = ((*S)>>16) & 0xFF;
    Z[3] = ((*S)>>24) & 0xFF;

    UCHAR U[4];
    U[0] = Z[0]^Z[1]^Z[2];
    U[1] = Z[0]^Z[1]^Z[3];
    U[2] = Z[0]^Z[2]^Z[3];
    U[3] = Z[1]^Z[2]^Z[3];

    UCHAR Y[4];
    Y[0] = SEED_S1box_inv[(U[0]&0xC0)^(U[1]&0x30)^(U[2]&0x0C)^(U[3]&0x03)];
    Y[1] = SEED_S2box_inv[(U[0]&0x03)^(U[1]&0xC0)^(U[2]&0x30)^(U[3]&0x0C)];
    Y[2] = SEED_S1box_inv[(U[0]&0x0C)^(U[1]&0x03)^(U[2]&0xC0)^(U[3]&0x30)];
    Y[3] = SEED_S2box_inv[(U[0]&0x30)^(U[1]&0x0C)^(U[2]&0x03)^(U[3]&0xC0)];

    ULONG X = Y[3];
    X = X << 8;
    X += Y[2];
    X = X << 8;
    X += Y[1];
    X = X << 8;
    X += Y[0];

    (*S) = X;
}

ULONG uc2ul_l(UCHAR *x){
    ULONG a = x[0];
    a = a << 8;
    a += x[1];
    a = a<< 8;
    a += x[2];
    a= a<<8;
    a+=x[3];
    return a;
}

ULONG uc2ul_b(UCHAR *x){
    ULONG a = x[3];
    a = a << 8;
    a += x[2];
    a = a<< 8;
    a += x[1];
    a= a<<8;
    a += x[0];
    return a;
}

void read_trace(void){
    printf("[*] read trace : %d - %d points\n", startpoint, endpoint);
    FILE *rfp = fopen("SEED.traces", "rb");
    if(rfp == NULL){
        printf("file open error\n");
    }
    fread(&s, 4, 1, rfp);
    //printf("%d samples\n", s);
    fread(&t, 4, 1, rfp);
    //printf("%d traces\n", t);
    
    float *trace;
    trace = (float *)calloc(s, sizeof(float));
    data = (int **)calloc(t, sizeof(int*));
    for(int i=0;i<t;i++){
        data[i] = (int*)calloc(endpoint-startpoint, sizeof(int));
        fread(trace, 4, s, rfp);
        for(int j=0;j<endpoint-startpoint;j++){
            data[i][j] = int(trace[j+startpoint]);
        }
    }
    fclose(rfp);
    free(trace);
}

void read_plaintext(void){
    puts("[*] read plaintext");
    FILE *rfp = fopen("plaintext.txt", "rb");
    UCHAR buf[256] = { 0 };
    plaintext = (UCHAR**)calloc(t, sizeof(UCHAR*));
    for(int i=0;i<t;i++){
        fread(buf, 1, 34, rfp);
        plaintext[i] = (UCHAR*)calloc(16, sizeof(UCHAR));
        for(int j=0;j<16;j++){
            UCHAR x = buf[2*j];
            UCHAR y = buf[2*j+1];
            if(x>='A' && x<='Z') x = x-'A'+10;
            else x = x-'0';
            if(y>='A' && y<='Z') y = y-'A'+10;
            else y = y-'0';
            plaintext[i][j] = 16*x+y;
        }
    }
}

ULONG find_key(ULONG xor_key){
    // Calculate G(C^D^K1,0^K1,1) and store in temp
    ULONG *temp = (ULONG *)calloc(t, sizeof(ULONG));
    for(int i=0;i<t;i++){
        ULONG c = uc2ul_l(&plaintext[i][8]);
        ULONG d = uc2ul_l(&plaintext[i][12]);
        temp[i] = xor_key^c^d;
        SEED_G(&temp[i]);
    }

    ULONG realkey = 0;
    for(int idx = 0;idx<4;idx++){
        //printf("%x\n", realkey);
        double max = 0;
        int max_key = 0;
        for(int key = 0;key<256;key++){
            //printf("%d KEY\n", key);
            double Sx = 0, Sxx = 0;
            int *hw = (int*)calloc(t, sizeof(int));
            for(int i=0;i<t;i++){
                ULONG c = uc2ul_l(&plaintext[i][8]);
                UCHAR x;
                if(idx % 2 == 1)
                    x = SEED_S2box[(((c^(realkey+(key<<(8*idx))))+temp[i])>>(8*idx))&0xff];
                else
                    x = SEED_S1box[(((c^(realkey+(key<<(8*idx))))+temp[i])>>(8*idx))&0xff];
                    
                for(int k=0;k<8;k++){
                    hw[i] += (x>>k)&1;
                }
                Sx += (double)hw[i]/t;
                Sxx += (double)hw[i]*hw[i]/t;
            }
            for(int j=0;j<endpoint-startpoint;j++){
                double Sxy = 0;
                for(int i=0;i<t;i++){
                    Sxy += (double)hw[i]*data[i][j]/t;
                }
                double cov = (Sxy-Sx*Sy[j])/sqrt((Syy[j]-Sy[j]*Sy[j])*(Sxx-Sx*Sx));
                //fprintf(wfp, "%f\n", cov);
                if(max < fabs(cov)){
                    max = fabs(cov);
                    max_key = key;
                    //printf("%d KEY : %f\n", key, max);
                }
            }
            if(max > 0.3)
                break;
            free(hw);
        }
        printf("%d KEY : %f\n", max_key, max);
        realkey += max_key<<(8*idx);
        //printf("%x\n", realkey);
    }
    free(temp);

    return realkey;
}

ULONG find_xor_key(void){
    ULONG xor_key = 0;
    for(int idx = 8; idx<12;idx++){
        double max = 0;
        int max_point = 0;
        int correct_key = 0;
        for(int key = 0;key<256;key++){
            //printf("%d KEY\n", key);
            double Sx = 0, Sxx = 0;
            int *hw = (int*)calloc(t, sizeof(int));
            for(int i=0;i<t;i++){
                UCHAR x;
                if(idx%2==1){
                    x = SEED_S1box[plaintext[i][idx]^plaintext[i][idx+4]^key];
                }
                else{
                    x = SEED_S2box[plaintext[i][idx]^plaintext[i][idx+4]^key]; 
                }
                for(int k=0;k<8;k++){
                    hw[i] += (x>>k)&1;
                }
                Sx += (double)hw[i]/t;
                Sxx += (double)hw[i]*hw[i]/t;
            }
            for(int j=0;j<endpoint-startpoint;j++){
                double Sxy = 0;
                for(int i=0;i<t;i++){
                    Sxy += (double)hw[i]*data[i][j]/t;
                }
                double cov = (Sxy-Sx*Sy[j])/sqrt((Syy[j]-Sy[j]*Sy[j])*(Sxx-Sx*Sx));
                if(max < fabs(cov)){
                    max = fabs(cov);
                    max_point = j;
                    correct_key = key;
                    //printf("%d KEY : %f\n", key, max);
                }
            }
            if(max > 0.3)
                break;
            free(hw);
        }
        printf("%d KEY : %f\n", correct_key, max);
        xor_key += correct_key << (8*(11-idx));
    }
    return xor_key;
}

void enc_1stround(ULONG key_0, ULONG key_1){
    for(int i=0;i<t;i++){
        ULONG L[2] = { 0 }, R[2] = { 0 }, temp[2];
        ULONG K[2] = {key_0, key_1};
        L[0] = uc2ul_l(&plaintext[i][0]);
        L[1] = uc2ul_l(&plaintext[i][4]);
        R[0] = uc2ul_l(&plaintext[i][8]);
        R[1] = uc2ul_l(&plaintext[i][12]);
        temp[0] = R[0] ^ K[0];
        temp[1] = R[1] ^ K[1];
        
        temp[1] ^= temp[0];

        SEED_G(temp + 1);  
        temp[0] += temp[1];

        SEED_G(temp);
        temp[1] += temp[0];

        SEED_G(temp + 1);
        temp[0] += temp[1];

        L[0] ^= temp[0];
        L[1] ^= temp[1];

        plaintext[i][11] = L[0] & 0xff;
        L[0] = L[0] >> 8;
        plaintext[i][10] = L[0] & 0xff;
        L[0] = L[0] >> 8;
        plaintext[i][9] = L[0] & 0xff;
        L[0] = L[0] >> 8;
        plaintext[i][8] = L[0] & 0xff;
        plaintext[i][15] = L[1] & 0xff;
        L[1] = L[1] >> 8;
        plaintext[i][14] = L[1] & 0xff;
        L[1] = L[1] >> 8;
        plaintext[i][13] = L[1] & 0xff;
        L[1] = L[1] >> 8;
        plaintext[i][12] = L[1] & 0xff;
    }
}

void find_masterkey(void){
    ULONG T0[2] = { 0 }, T1[2] = { 0 };
    T0[0] = RK[0][0];
    T1[0] = RK[0][1];
    T0[1] = RK[1][0];
    T1[1] = RK[1][1];
    SEED_G_INV(&T0[0]);
    SEED_G_INV(&T0[1]);
    SEED_G_INV(&T1[0]);
    SEED_G_INV(&T1[1]);

    T0[0] += SEED_KC[0];
    T0[1] += SEED_KC[1];
    T1[0] -= SEED_KC[0];
    T1[1] -= SEED_KC[1];
    
    ULONG X = 0x89111111; // T0[0] - T0[1]
    ULONG Y = 0x11111111; // T1[0] - T1[1]

    FILE *f_key = fopen("key.txt", "wb");
    if(f_key == NULL){
        printf("file open error");
    }

    for(int i=0x0;i<=0xff;i++){
        int A[4] = { 0 }, B[4] = { 0 };
        A[0] += i & 0xff;
        A[1] += A[0] - (X & 0xff);
        if(A[1] < 0){
            A[1] += 0x100;
            A[2]--;
        }
        A[2] += A[1] - ((X>>8) & 0xff);
        if(A[2] < 0){
            A[2] += 0x100;
            A[3]--;
        }
        A[3] += A[2] - ((X>>16) & 0xff);
        if(A[3] < 0){
            A[3] += 0x100;
            B[0]--;
        }
        B[0] += A[3] - ((X>>24) & 0xff);
        if(B[0] < 0){
            B[0] += 0x100;
        }
        B[1] += B[0] - (Y & 0xff);
        if(B[1] < 0){
            B[1] += 0x100;
            B[2]--;
        }
        B[2] += B[1] - ((Y>>8) & 0xff);
        if(B[2] < 0){
            B[2] += 0x100;
            B[3]--;
        }
        B[3] += B[2] - ((Y>>16) & 0xff);
        if(B[3] < 0){
            B[3] += 0x100;
        }

        UCHAR AA[4], BB[4];
        for(int j=0;j<4;j++){
            AA[3-j] = (UCHAR)A[j];
            BB[3-j] = (UCHAR)B[j];
        }

        ULONG AAA = uc2ul_l(&AA[0]);
        ULONG BBB = uc2ul_l(&BB[0]); 
        ULONG C = T0[0] - AAA;
        ULONG D = BBB - T1[0];

        if(AAA+C != T0[0] || BBB-D != T1[0]){
            continue;
        }

        TwoWordRRot(AAA, BBB);
        
        if(AAA+C != T0[1] || BBB-D != T1[1]){
            continue;
        }

        TwoWordLRot(AAA, BBB);

        fprintf(f_key, "%08x%08x%08x%08x\n", AAA, BBB, C, D);
    }
}

void CPA(void){
    // CPA in 1st Round. 
    // Analyze 480000-500000 point in 1st round.
    startpoint = 485000;
    endpoint = 490000;
    // read trace and store in data(int).
    read_trace();

    // read plaintext and store in plaintext(UCHAR).
    read_plaintext();

    // Calculate some value for corr in advance.    
    Sy = (double*)calloc(endpoint-startpoint, sizeof(double));
    Syy = (double*)calloc(endpoint-startpoint, sizeof(double));
    for(int j=0;j<endpoint-startpoint;j++){
        for(int i=0;i<t;i++){
            Sy[j] += (double)data[i][j]/t;
            Syy[j] += (double)data[i][j]*data[i][j]/t;
        }
    }

    puts("[*] Find K1,0 ^ K1,1");
    // Find K1,0^K1,1 by CPA.
    xor_key = find_xor_key();

    puts("[*] Find K1,0");
    // Find K1,0 by CPA.
    RK[0][0] = find_key(xor_key);
    RK[0][1] = xor_key ^ RK[0][0];

    free(data);
    free(Sy);
    free(Syy);

    // encrypt plaintext only 1st round.
    enc_1stround(RK[0][0], RK[0][1]);
    
    // CPA in 2nd Round. 
    // Analyze 500000-530000 point in 2nd round.
    startpoint = 505000;
    endpoint = 515000; 

    read_trace();

    // Calculate some value for corr in advance.    
    Sy = (double*)calloc(endpoint-startpoint, sizeof(double));
    Syy = (double*)calloc(endpoint-startpoint, sizeof(double));
    for(int j=0;j<endpoint-startpoint;j++){
        for(int i=0;i<t;i++){
            Sy[j] += (double)data[i][j]/t;
            Syy[j] += (double)data[i][j]*data[i][j]/t;
            //printf("%f %f\n", Sy[j], Syy[j]);
        }
    }

    puts("[*] Find K2,0 ^ K2,1");
    // Find K2,0^K2,1 by CPA.
    xor_key = find_xor_key();

    puts("[*] Find K2,0");
    // Find K2,0 by CPA.
    RK[1][0] = find_key(xor_key);
    RK[1][1] = xor_key ^ RK[1][0];

    free(data);
    free(Sy);
    free(Syy);

    // Calculate Master Key.
    puts("[*] Calculate Master Key");
    find_masterkey();

    puts("[*] Check 'key.txt' in your directory.");
    puts("[*] $ python2 seed.py | xxd");
}

int main(void){
    CPA();
    return 0;
}