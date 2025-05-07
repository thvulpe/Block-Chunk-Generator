#include <math.h>
#include "chunk.h"

char*** chunk_place_block(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
        if (x < 0 || x >= width) {
            return chunk;
        }
        if (y < 0 || y >= height) {
            return chunk;
        }
        if (z < 0 || z >= depth) {
            return chunk;
        }
        chunk[x][y][z] = block;
        return chunk;
}

char*** chunk_fill_cuboid(
    char*** chunk, int width, int height, int depth,
    int x0, int y0, int z0, int x1, int y1, int z1, char block) {
        if (x0 > x1) {
            int aux = x0;
            x0 = x1;
            x1 = aux;
        }
        if (y0 > y1) {
            int aux = y0;
            y0 = y1;
            y1 = aux;
        }
        if (z0 > z1) {
            int aux = z0;
            z0 = z1;
            z1 = aux;
        }
        for (int x = x0; x <= x1; x++) {
            for (int y = y0; y <= y1; y++) {
                for (int z = z0; z <= z1; z++) {
                    chunk_place_block(chunk, width, height, depth,
                    x, y, z, block);
                }
            }
        }
        return chunk;
}

char*** chunk_fill_sphere(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, double radius, char block) {
        int x0 = x - (int)radius;
        int y0 = y - (int)radius;
        int z0 = z - (int)radius;
        int x1 = x + (int)radius;
        int y1 = y + (int)radius;
        int z1 = z + (int)radius;

        for (int xs = x0; xs <= x1; xs++) {
            for (int ys = y0; ys <= y1; ys++) {
                for (int zs = z0; zs <= z1; zs++) {
                    // Calculez distanta dintre blockuri
                    int a = (x - xs);
                    a *= a;
                    int b = (y - ys);
                    b *= b;
                    int c = (z - zs);
                    c *= c;
                    /*
                     * Pentru a fi in sfera, distanta trebuie sa fie mai mica
                     * decat raza
                    */
                    if (a + b + c < radius * radius) {
                        chunk_place_block(chunk, width, height, depth,
                        xs, ys, zs, block);
                    }
                }
            }
        }

        return chunk;
}
