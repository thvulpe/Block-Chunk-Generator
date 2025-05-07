// Realizat de Theodor Vulpe - 315CB

#include "chunk.h"
#include <stdlib.h>

// Elibereaza memoria asignata unui chunk
void elibereaza_chunk(char*** chunk, int width, int height) {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                free(chunk[i][j]);
            }
            free(chunk[i]);
        }
        free(chunk);
}

char*** chunk_rotate_y(
    char*** chunk, int width, int height, int depth) {

        // Chunk auxiliar, rotit la 90 de grade
        char*** aux = malloc(depth * sizeof(char**));
        for (int i = 0; i < depth; i++) {
            aux[i] = malloc(height * sizeof(char*));
            for (int j = 0; j < height; j++) {
                aux[i][j] = malloc(width * sizeof(char));
            }
        }

        // (x,y,z) devine (z,y,width-x-1)
        for (int x = 0; x < depth; x++) {
            for (int y = 0; y < height; y++) {
                for (int z = 0; z < width; z++) {
                    aux[x][y][z] = chunk[z][y][depth - x - 1];
                }
            }
        }

        // Eliberez memoria pentru chunk-ul initial
        elibereaza_chunk(chunk, width, height);

        return aux;
}

// Definesc un tip ce retine o coordonata in spatiu
typedef struct {
    int x, y, z;
} coordXYZ;

// Intoarce un corp retinut sub forma unui chunk
char*** get_body_at(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z) {

        // Chunk auxiliar care retine 1 unde se afla corp si 0 in caz contrar
        char*** aux = malloc(width * sizeof(char**));
        for (int i = 0; i < width; i++) {
            aux[i] = malloc(height * sizeof(char*));
            for (int j = 0; j < height; j++) {
                aux[i][j] = malloc(depth * sizeof(char));
            }
        }

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                for (int k = 0; k < depth; k++) {
                    aux[i][j][k] = 0;
                }
            }
        }

        // Vectori de coordonate pentru a trece mai usor prin toti vecinii
        int vx[] = {0, 1, 0, -1, 0, 0};
        int vy[] = {-1, 0, 0, 0, 0, 1};
        int vz[] = {0, 0, 1, 0, -1, 0};

        char initial_block = chunk[x][y][z];

        // Intoarce NULL daca nu exista niciun corp
        if (initial_block == BLOCK_AIR) {
            elibereaza_chunk(aux, width, height);
            return NULL;
        }

        // Implementez un algoritm de tip Fill, care necesita o coada
        const int kQueueSize = width * height * depth;
        coordXYZ q[kQueueSize];
        int st = 0, dr = 0;

        // Retin coordonatele deja vizitate
        int viz[width][height][depth];
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                for (int k = 0; k < depth; k++) {
                    viz[i][j][k] = 0;
                }
            }
        }

        // Adaug block-ul initial
        q[dr].x = x;
        q[dr].y = y;
        q[dr].z = z;
        dr++;
        viz[x][y][z] = 1;
        aux[x][y][z] = chunk[x][y][z];

        // Cat timp coada nu este goala
        while (st < dr) {
            int xcrt = q[st].x, ycrt = q[st].y, zcrt = q[st].z;

            // Parcurg vecinii
            int kMax = sizeof(vx) / sizeof(int);
            for (int k = 0; k < kMax; k++) {
                int xvec = xcrt + vx[k];
                int yvec = ycrt + vy[k];
                int zvec = zcrt + vz[k];

                int in_chunk = is_in_chunk(width, height, depth, xvec, yvec, zvec);

                if (in_chunk) {
                    if (viz[xvec][yvec][zvec] == 0
                        && chunk[xvec][yvec][zvec] == initial_block) {
                            // Adaug in coada
                            q[dr].x = xvec;
                            q[dr].y = yvec;
                            q[dr].z = zvec;
                            dr++;
                            viz[xvec][yvec][zvec] = 1;
                            aux[xvec][yvec][zvec] = chunk[x][y][z];
                    }
                }
            }

            st++;
        }

        return aux;
}

// Intoarce un corp retinut sub forma unui chunk (cu block-uri mixte)
char*** get_mixed_body_at(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z) {

        // Chunk auxiliar
        char*** aux = malloc(width * sizeof(char**));
        for (int i = 0; i < width; i++) {
            aux[i] = malloc(height * sizeof(char*));
            for (int j = 0; j < height; j++) {
                aux[i][j] = malloc(depth * sizeof(char));
            }
        }

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                for (int k = 0; k < depth; k++) {
                    aux[i][j][k] = 0;
                }
            }
        }

        // Vectori de coordonate pentru a trece mai usor prin toti vecinii
        int vx[] = {0, 1, 0, -1, 0, 0};
        int vy[] = {-1, 0, 0, 0, 0, 1};
        int vz[] = {0, 0, 1, 0, -1, 0};

        // Intoarce NULL daca nu exista niciun corp
        if (!chunk[x][y][z]) {
            elibereaza_chunk(aux, width, height);
            return NULL;
        }

        // Implementez un algoritm de tip Fill, care necesita o coada
        const int kQueueSize = width * height * depth;
        coordXYZ q[kQueueSize];
        int st = 0, dr = 0;

        // Retin coordonatele deja vizitate
        int viz[width][height][depth];
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                for (int k = 0; k < depth; k++) {
                    viz[i][j][k] = 0;
                }
            }
        }

        // Adaug block-ul initial
        q[dr].x = x;
        q[dr].y = y;
        q[dr].z = z;
        dr++;
        viz[x][y][z] = 1;
        aux[x][y][z] = chunk[x][y][z];

        // Cat timp coada nu este goala
        while (st < dr) {
            int xcrt = q[st].x, ycrt = q[st].y, zcrt = q[st].z;

            // Parcurg vecinii
            int kMax = sizeof(vx) / sizeof(int);
            for (int k = 0; k < kMax; k++) {
                int xvec = xcrt + vx[k];
                int yvec = ycrt + vy[k];
                int zvec = zcrt + vz[k];

                if (is_in_chunk(width, height, depth, xvec, yvec, zvec)) {
                    if (viz[xvec][yvec][zvec] == 0
                        && chunk[xvec][yvec][zvec]) {
                            // Adaug in coada
                            q[dr].x = xvec;
                            q[dr].y = yvec;
                            q[dr].z = zvec;
                            dr++;
                            viz[xvec][yvec][zvec] = 1;
                            aux[xvec][yvec][zvec] = chunk[xvec][yvec][zvec];
                    }
                }
            }

            st++;
        }

        return aux;
}

// Verifica daca un corp atinge "pamantul"
int is_touching_ground(char ***chunk, int width, int height, int depth,
    char*** body) {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                for (int z = 0; z < depth; z++) {
                    if (y == 0 && body[x][y][z]) {
                        return 1;
                    } else if (y > 0) {
                        if (body[x][y][z] && !body[x][y - 1][z]
                        && chunk[x][y - 1][z]) {
                            return 1;
                        }
                    }
                }
            }
        }
        return 0;
}

/*
 * Deplaseaza un corp cu o pozitie mai jos.
 * Intoarce noul corp.
*/
char*** lower_body(char*** chunk, int width, int height, int depth,
char*** body) {
    // Daca atinge pamantul, nu efectuez operatia
    if (is_touching_ground(chunk, width, height, depth, body)) {
        return body;
    }

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            for (int z = 0; z < depth; z++) {
                if (body[x][y][z]) {
                    // Sterg corpul de la pozitia anterioara
                    chunk[x][y][z] = 0;
                    // Il plasez cu o pozitie mai jos
                    chunk[x][y - 1][z] = body[x][y][z];
                    body[x][y - 1][z] = body[x][y][z];
                    // Marchez mutarea corpului
                    body[x][y][z] = 0;
                }
            }
        }
    }

    return body;
}

char*** chunk_apply_gravity(
    char*** chunk, int width, int height, int depth, int* new_height) {
        // Verific fiecare corp mixt din chunk
        for (int x = 0; x < width; x++) {
            for (int y = 1; y < height; y++) {
                for (int z = 0; z < depth; z++) {
                    char*** body = get_mixed_body_at(chunk, width, height,
                    depth, x, y, z);
                    if (body != NULL) {
                        // Verific daca corpul atinge pamantul
                        int touches_ground = is_touching_ground(chunk, width,
                        height, depth, body);

                        // Cat timp nu atinge, il deplasez in jos
                        while (!touches_ground) {
                            body = lower_body(chunk, width, height,
                            depth, body);
                            touches_ground = is_touching_ground(chunk, width,
                            height, depth, body);
                        }

                        elibereaza_chunk(body, width, height);
                    }
                }
            }
        }

        // Verific fiecare corp uniform din chunk
        for (int x = 0; x < width; x++) {
            for (int y = 1; y < height; y++) {
                for (int z = 0; z < depth; z++) {
                    char*** body = get_body_at(chunk, width, height, depth,
                    x, y, z);
                    if (body != NULL) {
                        // Verific daca corpul atinge pamantul
                        int touches_ground = is_touching_ground(chunk, width,
                        height, depth, body);

                        // Cat timp nu atinge, il deplasez in jos
                        while (!touches_ground) {
                            body = lower_body(chunk, width, height,
                            depth, body);
                            touches_ground = is_touching_ground(chunk, width,
                            height, depth, body);
                        }

                        elibereaza_chunk(body, width, height);
                    }
                }
            }
        }
        // Caut noua inaltime
        int yMax = height - 1, f = 0;
        while (!f) {
            for (int x = 0; x < width; x++) {
                for (int z = 0; z < depth; z++) {
                    if (chunk[x][yMax][z]) {
                        f = 1;
                    }
                }
            }
            if (!f) {
                yMax--;
            }
        }
        *new_height = yMax + 1;

        // Elibereaza memoria inaltimii sterse si redimensioneaza chunk-ul
        for (int x = 0; x < width; x++) {
            for (int y = *new_height; y < height; y++) {
                free(chunk[x][y]);
            }
            chunk[x] = realloc(chunk[x], *new_height * sizeof(char *));
        }

        return chunk;
}

