// Realizat de Theodor Vulpe - 315CB

#include <stdio.h>
#include <stdlib.h>
#include "chunk.h"

const int N1 = 4095;

const int SHIFT_TIP = 6;
const int SHIFT_DIM = 5;
const int SHIFT_DIM_B = 4;

// 2^5 = 32
const unsigned char N_MIC = 32;
const unsigned char N_MIC_M = 31;
// 2^4 = 16
const unsigned char N_MIC_B = 16;
const unsigned char N_MIC_B_M = 15;

const char SHIFT_BYTE = 8;

// Encode unui run
void run_encode(int tip, int n, unsigned char *runs, int *runsInd) {
    while (n > N1) {
        runs[*runsInd] = ((tip << SHIFT_TIP) | (1 << SHIFT_DIM));
        int aux = N1;
        unsigned char* byte = (unsigned char*)&aux;
        runs[*runsInd] |= *(byte + 1);
        (*runsInd)++;
        runs[*runsInd] = *byte;
        (*runsInd)++;
        n -= N1;
    }
    if (n < N_MIC) {
        runs[*runsInd] = (tip << SHIFT_TIP) | n;
        (*runsInd)++;
    } else {
        runs[*runsInd] = ((tip << SHIFT_TIP) | (1 << SHIFT_DIM));
        unsigned char* byte = (unsigned char*)&n;
        runs[*runsInd] |= *(byte + 1);
        (*runsInd)++;
        runs[*runsInd] = *byte;
        (*runsInd)++;
    }
}

unsigned char* chunk_encode(
    char*** chunk, int width, int height, int depth,
    int* length) {
        char* flat = malloc(width * height * depth * sizeof(char));
        for (int i = 0; i < width * height * depth; i++) {
            flat[i] = 0;
        }

        // Indice curent
        int ind = 0;

        // Dau "flatten" la chunk
        for (int y = 0; y < height; y++) {
            for (int z = 0; z < depth; z++) {
                for (int x = 0; x < width; x++) {
                    flat[ind++] = chunk[x][y][z];
                }
            }
        }

        int runsInd = 0;
        unsigned char *runs = malloc(ind * sizeof(unsigned char));
        for (int i = 0; i < ind; i++) {
            runs[i] = 0;
        }

        // Caut subsecventele
        int n = 1, tip = flat[0];

        for (int i = 1; i < ind; i++) {
            if (flat[i] == tip) {
                n++;
            } else {
                run_encode(tip, n, runs, &runsInd);
                n = 1;
                tip = flat[i];
            }
        }
        // Verific secventa ramasa
        run_encode(tip, n, runs, &runsInd);
        free(flat);

    *length = runsInd;
    return runs;
}

// Intoarce tipul de block dintr-un run encodat
unsigned char decode_type(unsigned char run) {
    return (run >> SHIFT_TIP);
}

// Intoarce daca exista un run nn...n dupa
int decode_longer(unsigned char run) {
    unsigned char c1 = ((run >> SHIFT_DIM) & 1);
    unsigned char c2 = ((run >> SHIFT_DIM_B) & 1);
    return (c1 && !c2);
}

// Intoarce numarul de block-uri dintr-un run encodat
int decode_number(unsigned char* runs, int ind) {
    unsigned char mask = 0;
    int n = 0;
    if (decode_longer(runs[ind])) {
        mask = N_MIC_B_M;
        n = (runs[ind] & mask);
        n = (n << SHIFT_BYTE);
        unsigned char* p = (unsigned char*)&n;
        *p = runs[ind + 1];
    } else {
        mask = N_MIC_M;
        n = (runs[ind] & mask);
    }
    return n;
}

// Intoarce numarul de runs din code
int get_runs(unsigned char* code) {
    return (sizeof(code) / sizeof(unsigned char *));
}

char*** chunk_decode(
    unsigned char* code, int width, int height, int depth) {
        // Aloc memorie pentru chunk-ul decodificat
        char ***chunk = malloc(width * sizeof(char **));
        for (int i = 0; i < width; i++) {
            chunk[i] = malloc(height * sizeof(char *));
            for (int j = 0; j < height; j++) {
                chunk[i][j] = malloc(depth * sizeof(char));
            }
        }

        unsigned char tip = decode_type(code[0]);
        int n = decode_number(code, 0);
        // Indicele curent in vector
        int ind = 0;

        for (int y = 0; y < height; y++) {
            for (int z = 0; z < depth; z++) {
                for (int x = 0; x < width; x++) {
                    // Asez block-urile in chunk
                    chunk[x][y][z] = (char)tip;
                    n--;
                    if (n == 0 && !(x == width - 1 && y == height - 1
                        && z == depth - 1)) {
                            if (decode_longer(code[ind])) {
                                ind += 2;
                            } else {
                                ind++;
                            }
                            tip = decode_type(code[ind]);
                            n = decode_number(code, ind);
                    }
                }
            }
        }
        return chunk;
}

