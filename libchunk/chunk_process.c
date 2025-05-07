// Realizat de Theodor Vulpe - 315CB

#include "chunk.h"

int is_in_chunk(int width, int height, int depth,
    int x, int y, int z) {
        if (x < 0 || x >= width) {
            return 0;
        }
        if (y < 0 || y >= height) {
            return 0;
        }
        if (z < 0 || z >= depth) {
            return 0;
        }
        return 1;
}

char*** chunk_shell(
    char*** chunk, int width, int height, int depth,
    char target_block, char shell_block) {
        // Vectori de coordonate pentru a trece mai usor prin toti vecinii
        int vx[] = {0, 1, 0, -1, 0, 0, 1, 1, -1, -1, 1, 0, -1, 0, 1, 0, -1, 0};
        int vy[] = {-1, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1, -1, -1, -1, 1, 1, 1, 1};
        int vz[] = {0, 0, 1, 0, -1, 0, 1, -1, 1, -1, 0, 1, 0, -1, 0, 1, 0, -1};

        /*
         * Retin ce blockuri au fost modificate pentru a nu le lua in
         * considerare in situatia in care target_block coincide cu
         * shell_block
        */
        int new[width][height][depth];

        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                for (int z = 0; z < depth; z++) {
                    new[x][y][z] = 0;
                }
            }
        }

        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                for (int z = 0; z < depth; z++) {
                    if (chunk[x][y][z] == target_block && new[x][y][z] == 0) {
                        // Verific toti vecinii
                        int kMax = sizeof(vx) / sizeof(int);
                        for (int k = 0; k < kMax; k++) {
                            // x, y, z vecini
                            int xv = x + vx[k];
                            int yv = y + vy[k];
                            int zv = z + vz[k];
                            if (is_in_chunk(width, height,
                                depth, xv, yv, zv)) {
                                    if (chunk[xv][yv][zv] != target_block) {
                                            chunk_place_block(chunk,
                                            width, height, depth,
                                            xv, yv, zv, shell_block);
                                            new[xv][yv][zv] = 1;
                                    }
                            }
                        }
                    }
                }
            }
        }
        return chunk;
}

// Definesc un tip ce retine o coordonata in planul xOz
typedef struct {
    int x, z;
} coordXZ;

char*** chunk_fill_xz(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
        // Vectori de coordonate pentru a trece mai usor prin toti vecinii
        int vx[] = {1, 0, -1, 0}, vz[] = {0, 1, 0, -1};

        char initial_block = chunk[x][y][z];

        // Implementez un algoritm de tip Fill, care necesita o coada
        const int kQueueSize = width * depth;
        coordXZ q[kQueueSize];
        int st = 0, dr = 0;

        // Retin coordonatele deja vizitate
        int viz[width][depth];
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < depth; j++) {
                viz[i][j] = 0;
            }
        }

        // Adaug block-ul initial
        q[dr].x = x;
        q[dr].z = z;
        dr++;
        viz[x][z] = 1;
        chunk_place_block(chunk, width, height, depth, x, y, z, block);

        // Cat timp coada nu este goala
        while (st < dr) {
            int xcrt = q[st].x, zcrt = q[st].z;

            // Parcurg vecinii
            for (int k = 0; k < 4; k++) {
                int xvec = xcrt + vx[k];
                int zvec = zcrt + vz[k];

                if (is_in_chunk(width, height, depth, xvec, y, zvec)) {
                    if (viz[xvec][zvec] == 0
                        && chunk[xvec][y][zvec] == initial_block) {
                            q[dr].x = xvec;
                            q[dr].z = zvec;
                            dr++;
                            viz[xvec][zvec] = 1;
                            chunk_place_block(chunk, width, height, depth, xvec,
                            y, zvec, block);
                    }
                }
            }

            st++;
        }

    return chunk;
}

// Definesc un tip ce retine o coordonata in spatiu
typedef struct {
    int x, y, z;
} coordXYZ;

char*** chunk_fill(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
        // Vectori de coordonate pentru a trece mai usor prin toti vecinii
        int vx[] = {0, 1, 0, -1, 0, 0};
        int vy[] = {-1, 0, 0, 0, 0, 1};
        int vz[] = {0, 0, 1, 0, -1, 0};

        char initial_block = chunk[x][y][z];

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
        chunk_place_block(chunk, width, height, depth, x, y, z, block);

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
                        && chunk[xvec][yvec][zvec] == initial_block) {
                            // Adaug in coada
                            q[dr].x = xvec;
                            q[dr].y = yvec;
                            q[dr].z = zvec;
                            dr++;
                            viz[xvec][yvec][zvec] = 1;
                            chunk_place_block(chunk, width, height, depth, xvec,
                            yvec, zvec, block);
                    }
                }
            }

            st++;
        }
        return chunk;
}
