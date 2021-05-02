#include <stdio.h>
#include <stdlib.h>
#include "imagem.h"

int* bin(int n)
{
    int *bit = malloc(32 * sizeof(int));
    for (int i = 31; i >= 0; i--) {
        int k = n >> i;
        if (k & 1)
            bit[i]= 1;
        else
            bit[i]= 0;
    }
    putchar('\n');
    for (int j = 31; j >= 0; j--) {
            printf("%d", bit[j]);
    }

    return bit;
}
int main () {
    for (int i = 0; i < 10; i++) {
        int *n = bin(i);
        putchar('\n');
        for (int j = 31; j >= 0; j--) {
            printf("%d", n[j]);
        }
        free(n);
        putchar('\n');
    }
    // Imagem *img = abreImagem("img/1.bmp", 1);
    // for (int i = 0; i < img->altura; i++) {
    //     for (int j = 0; j < img->largura; j++) {
    //         printf("%lf\n", img->dados[0][i][j]);
    //     }
    // }
    // destroiImagem(img);
    return 0;
}