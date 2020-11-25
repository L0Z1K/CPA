#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#define TRACES 1025
typedef uint8_t UCHAR;

const UCHAR inv_sbox[256] = {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

int data[TRACES][20000];
UCHAR cipher[TRACES][16];
double Sy[20000], Syy[20000];
int startpoint = 10000;
int endpoint = 20000;

void read_trace(void){
    FILE *fp = fopen("PowerConsumption.csv", "rb");
    if(fp==NULL){
        printf("file error");
    }
    char trace[10000] = { 0 };
    for(int i=0;i<20000;i++){
        memset(trace, 0, sizeof(trace));
        fgets(trace, sizeof(trace), fp);
        int len = strlen(trace);
        int j = 0;
        int k = 0;
        int idx = 0;
        char num[5] = { 0 };
        for(;;){
            if(trace[j] == ','){
                data[k][i] = atoi(num);
                k++;
                memset(num, 0, sizeof(num));
                idx = 0;
            }
            else if (j > len){
                data[k][i] = atoi(num);
                break;
            }
            else{
                num[idx] = trace[j];
                idx++;
            }
            j++;
            
        }
    }
    fclose(fp);
}

void read_cipher(void){
    FILE *fp = fopen("exam.hwp.encrypted", "rb");
    if(fp==NULL){
        printf("file open error");
    }

    fread(cipher, 1, 16400, fp);
    fclose(fp);
}

UCHAR xtime(UCHAR x)
{
  return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

UCHAR Multiply(UCHAR x, UCHAR y){
  return (((y & 1) * x) ^
       ((y>>1 & 1) * xtime(x)) ^
       ((y>>2 & 1) * xtime(xtime(x))) ^
       ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^
       ((y>>4 & 1) * xtime(xtime(xtime(xtime(x)))))); /* this last call to xtime() can be omitted */
}

void find_9R_second_key(void){
    read_trace();
    read_cipher();
    
    for(int j=startpoint;j<endpoint;j++){
        for(int i=0;i<TRACES;i++){
            Sy[j] += (double)data[i][j]/TRACES;
            Syy[j] += (double)data[i][j]*data[i][j]/TRACES;
            //printf("%f %f\n", Sy[j], Syy[j]);
        }
    }

    UCHAR x[TRACES][4] = { 0 };
    for(int i=0;i<TRACES;i++){
        x[i][0] = Multiply(inv_sbox[cipher[i][4]^0xE9], 0x0e) ^ Multiply(inv_sbox[cipher[i][1]^0xE5], 0x0b) ^ Multiply(inv_sbox[cipher[i][14]^0x3e], 0x0d) ^ Multiply(inv_sbox[cipher[i][11]^0x2C], 0x09);
        x[i][1] = Multiply(inv_sbox[cipher[i][4]^0xE9], 0x09) ^ Multiply(inv_sbox[cipher[i][1]^0xE5], 0x0e) ^ Multiply(inv_sbox[cipher[i][14]^0x3e], 0x0b) ^ Multiply(inv_sbox[cipher[i][11]^0x2C], 0x0d);
        x[i][2] = Multiply(inv_sbox[cipher[i][4]^0xE9], 0x0d) ^ Multiply(inv_sbox[cipher[i][1]^0xE5], 0x09) ^ Multiply(inv_sbox[cipher[i][14]^0x3e], 0x0e) ^ Multiply(inv_sbox[cipher[i][11]^0x2C], 0x0b);
        x[i][3] = Multiply(inv_sbox[cipher[i][4]^0xE9], 0x0b) ^ Multiply(inv_sbox[cipher[i][1]^0xE5], 0x0d) ^ Multiply(inv_sbox[cipher[i][14]^0x3e], 0x09) ^ Multiply(inv_sbox[cipher[i][11]^0x2C], 0x0e);
    }

    UCHAR T[4] = { 0 };
    for(int idx=0;idx<4;idx++){
        double max = 0;
        for(int key = 0;key<=0xff;key++){
            printf("%d KEY\n", key);
            double Sx = 0, Sxx = 0;
            int *hw = (int*)calloc(TRACES, sizeof(int));
            for(int i=0;i<TRACES;i++){ 
                UCHAR X = inv_sbox[x[i][idx]^key];
                for(int k=0;k<8;k++){
                    hw[i] += (X>>k)&1;
                }
                Sx += (double)hw[i]/TRACES;
                Sxx += (double)hw[i]*hw[i]/TRACES;
            }
            for(int j=startpoint;j<endpoint;j++){
                double Sxy = 0;
                for(int i=0;i<TRACES;i++){
                    Sxy += (double)hw[i]*data[i][j]/TRACES;
                }
                //printf("%f %f %f %f %f\n", Sxy, Sx, Sxx, Sy[j], Syy[j]);
                double cov = (Sxy-Sx*Sy[j])/sqrt((Syy[j]-Sy[j]*Sy[j])*(Sxx-Sx*Sx));
                //printf("%f\n", cov);
                if(max < fabs(cov)){
                    max = fabs(cov);
                    T[idx] = key;
                    printf("%d %f\n", j, max);
                }
            }
            free(hw);
            if(max > 0.3)
                break;
        }
    }

    UCHAR RK[4] = { 0 };
    RK[0] = Multiply(T[0], 0x02) ^ Multiply(T[1], 0x03) ^ Multiply(T[2], 0x01) ^ Multiply(T[3], 0x01);
    RK[1] = Multiply(T[0], 0x01) ^ Multiply(T[1], 0x02) ^ Multiply(T[2], 0x03) ^ Multiply(T[3], 0x01);
    RK[2] = Multiply(T[0], 0x01) ^ Multiply(T[1], 0x01) ^ Multiply(T[2], 0x02) ^ Multiply(T[3], 0x03);
    RK[3] = Multiply(T[0], 0x03) ^ Multiply(T[1], 0x01) ^ Multiply(T[2], 0x01) ^ Multiply(T[3], 0x02);
    printf("%x %x %x %x\n", RK[0], RK[1], RK[2], RK[3]);
    
}

void find_9R_first_key(void){
    read_trace();
    read_cipher();
    UCHAR c[4] = { 0 };
    UCHAR x[TRACES][4] = { 0 };
    for(int i=0;i<TRACES;i++){
        c[0] = inv_sbox[cipher[i][0]^0x22];
        c[1] = inv_sbox[cipher[i][13]^0x9B];
        c[2] = inv_sbox[cipher[i][10]^0x5B];
        c[3] = inv_sbox[cipher[i][7]^0x4C];
        x[i][0] = Multiply(c[0], 0x0e) ^ Multiply(c[1], 0x0b) ^ Multiply(c[2], 0x0d) ^ Multiply(c[3], 0x09);
        x[i][1] = Multiply(c[0], 0x09) ^ Multiply(c[1], 0x0e) ^ Multiply(c[2], 0x0b) ^ Multiply(c[3], 0x0d);
        x[i][2] = Multiply(c[0], 0x0d) ^ Multiply(c[1], 0x09) ^ Multiply(c[2], 0x0e) ^ Multiply(c[3], 0x0b);
        x[i][3] = Multiply(c[0], 0x0b) ^ Multiply(c[1], 0x0d) ^ Multiply(c[2], 0x09) ^ Multiply(c[3], 0x0e);
    }

    for(int j=startpoint;j<endpoint;j++){
        for(int i=0;i<TRACES;i++){
            Sy[j] += (double)data[i][j]/TRACES;
            Syy[j] += (double)data[i][j]*data[i][j]/TRACES;
            //printf("%f %f\n", Sy[j], Syy[j]);
        }
    }

    UCHAR T[4] = { 0 };
    for(int idx = 0; idx<4;idx++){
        double max = 0;
        for(int key = 0;key<=0xff;key++){
            printf("%d KEY\n", key);
            double Sx = 0, Sxx = 0;
            int *hw = (int*)calloc(TRACES, sizeof(int));
            for(int i=0;i<TRACES;i++){ 
                UCHAR X = inv_sbox[x[i][idx]^key];
                for(int k=0;k<8;k++){
                    hw[i] += (X>>k)&1;
                }
                Sx += (double)hw[i]/TRACES;
                Sxx += (double)hw[i]*hw[i]/TRACES;
            }
            for(int j=startpoint;j<endpoint;j++){
                double Sxy = 0;
                for(int i=0;i<TRACES;i++){
                    Sxy += (double)hw[i]*data[i][j]/TRACES;
                }
                //printf("%f %f %f %f %f\n", Sxy, Sx, Sxx, Sy[j], Syy[j]);
                double cov = (Sxy-Sx*Sy[j])/sqrt((Syy[j]-Sy[j]*Sy[j])*(Sxx-Sx*Sx));
                //printf("%f\n", cov);
                if(max < fabs(cov)){
                    max = fabs(cov);
                    T[idx] = key;
                    printf("%d %f\n", j, max);
                }
            }
            free(hw);
            if(max > 0.3)
                break;
        }
    }

    UCHAR RK[4] = { 0 };
    RK[0] = Multiply(T[0], 0x02) ^ Multiply(T[1], 0x03) ^ Multiply(T[2], 0x01) ^ Multiply(T[3], 0x01);
    RK[1] = Multiply(T[0], 0x01) ^ Multiply(T[1], 0x02) ^ Multiply(T[2], 0x03) ^ Multiply(T[3], 0x01);
    RK[2] = Multiply(T[0], 0x01) ^ Multiply(T[1], 0x01) ^ Multiply(T[2], 0x02) ^ Multiply(T[3], 0x03);
    RK[3] = Multiply(T[0], 0x03) ^ Multiply(T[1], 0x01) ^ Multiply(T[2], 0x01) ^ Multiply(T[3], 0x02);
    printf("%x %x %x %x\n", RK[0], RK[1], RK[2], RK[3]);
}

int main(void){
    //find_9R_first_key();
    //9R first key : a6 a8 6b 63
    //find_9R_second_key();
}