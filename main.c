#include <stdio.h>
#include <stdlib.h>
#include "imagem.h"
#include "geometria.h"
#include "desenho.h"
#include "cores.h"

#define BLACK 0
#define WHITE 1
#define UNDEFINED -1

#define MIN(a,b) ((a<b)?a:b)
#define MAX(a,b) ((a>b)?a:b)

typedef struct node node;
struct node{
    unsigned char color: 1;
    node** children;
};

node* createNode() {
    node* sx = malloc(sizeof(node));
    sx->children = (node **) malloc(sizeof(node*) * 4);
    for (int i = 0; i < 4; i++) {
        sx->children[i] = NULL;
    }
    return sx;
}


void destroyNode(node* n) {
    int children = sizeof(n->children) / sizeof(node);
    for (int i = 0; i < children; i ++) {
        if (n->children[i] != NULL) {
            destroyNode(n->children[i]);
        }
    }
    free(n);
}

void fillComponent() {

}

int getComponentColor(Imagem *img, Coordenada *a, Coordenada *b) {
    int component_pixels = 0;
    unsigned int sum_colors = 0;
    int a_aux_x = (a->x) < (b->x) ? a->x : b->x;
    int a_aux_y = (a->y) < (b->y) ? a->y : b->y;
    
    
    int b_aux_x = (a->x) > (b->x) ? a->x : b->x;
    int b_aux_y = (a->y) > (b->y) ? a->y : b->y;

    Coordenada a_aux = criaCoordenada(a_aux_x, a_aux_y);
    Coordenada b_aux = criaCoordenada(b_aux_x, b_aux_y);
    for (int i = a_aux.y; i < b_aux.y; i++) {
        for (int j = a_aux.x; j < b_aux.x; j++) {
            component_pixels++;
            // printf("somando esse: %u\n", img->dados[0][i][j]);
            sum_colors += img->dados[0][i][j];
        }
    }
    if (sum_colors == 0) {
        return BLACK;
    }
    if ( (sum_colors/component_pixels) == 255) {
        return WHITE;
    }

    return UNDEFINED;

}

node* create_children(node* root, Imagem *img, int depth, Coordenada center, int size_x, int size_y, Imagem* drawing) {
    putchar('\n');
    putchar('\n');
    putchar('\n');
    // getchar();
    printf("Centro: [%d][%d]\n", center.x, center.y);
    printf("Tamanhpo %d - %d\n", size_x, size_y);
    Cor cor = criaCor(255, 0, 0);
    if (size_x  == 1 || size_y == 1) {
    // getchar();

        // printf("Mas eu entrei aqui\n");
        // typeof(img->dados[0][center.x][center.y]) A = img->dados[0][center.x][center.y];
        node* n = createNode();
        node* c1 = createNode();
        node* c2 = createNode();
        node* c3 = createNode();
        node* c4 = createNode();
        printf("Tudo problema aqui %d %d\n", center.x - size_x, center.y - size_y);
        // printf("Da pra ver? %u", img->dados[0][center.x - size_x][center.y - size_y]);
        c1->color = img->dados[0][center.y - size_y][center.x - size_x] == 255 ? 1 : 0;
        c2->color = img->dados[0][center.y - size_y][center.x + size_x] == 255 ? 1 : 0;
        c3->color = img->dados[0][center.y + size_y][center.x - size_x] == 255 ? 1 : 0;
        c4->color = img->dados[0][center.y + size_y][center.x + size_x] == 255 ? 1 : 0;
        printf("Errou\n");
        n->children[0] = c1;
        n->children[1] = c2;
        n->children[2] = c3;
        n->children[3] = c4;
        // printf("Véi, outra tretaz\n");
        // printf("Vou voltar %d %d \n", center.x, img->largura);   
        printf("Vou voltar> \n");
        return n;
    }
    // Coordenada center = {x: x, y: y};
    Coordenada lt = { x: center.x - size_x, y: center.y - size_y };
    Coordenada rt = { x: center.x + size_x, y: center.y - size_y };
    Coordenada lb = { x: center.x - size_x, y: center.y + size_y };
    Coordenada rb = { x: center.x + size_x, y: center.y + size_y };

    int color_a = getComponentColor(img, &center, &lt);
    int color_b = getComponentColor(img, &center, &rt);
    int color_c = getComponentColor(img, &center, &lb);
    int color_d = getComponentColor(img, &center, &rb);

    printf("a: %d\n", color_a);
    printf("b: %d\n", color_b);
    printf("c: %d\n", color_c);
    printf("d: %d\n", color_d);


    Coordenada top = {x: center.x, y: 0 };
    Coordenada bot = {x: center.x, y: img->altura };
    Coordenada left = {x: 0, y: center.y };
    Coordenada rigth = {x: img->largura, y: center.y };
    desenhaLinha(center, top, cor, drawing);
    desenhaLinha(center, bot, cor, drawing);
    desenhaLinha(center, left, cor, drawing);
    desenhaLinha(center, rigth, cor, drawing);
    salvaImagem(drawing, "img/teste.bmp");


   if (color_a == BLACK || color_a == WHITE) {
       printf("no 1 a\n");
        node* child_1 = createNode();
        child_1->color = color_a == BLACK ? 0 : 1;
        root->children[0] = child_1;
    } else {
        printf("no 1 b\n");
        node* child_1 = createNode();

        Coordenada c1 = criaCoordenada(abs(center.x - size_x / 2), abs(center.y - size_y / 2));
        printf("Novo Centro: [%d][%d]\n", c1.x, c1.y);
        child_1 = create_children(child_1, img, depth + 1, c1, size_x / 2, size_y / 2, drawing);
        root->children[0] = child_1;
    }

    if (color_b == BLACK || color_b == WHITE) {
        printf("no 2 a\n");
        node* child_2 = createNode();
        child_2->color = color_a == BLACK ? 0 : 1;
        root->children[0] = child_2;
    } else {
        printf("no 2 b\n");
        node* child_2 = createNode();
        Coordenada c2 = criaCoordenada(abs(center.x + size_x / 2), abs(center.y - size_y / 2));
        printf("Novo Centro: [%d][%d]\n", c2.x, c2.y);
        child_2 = create_children(child_2, img, depth + 1, c2, size_x / 2, size_y / 2, drawing);
        root->children[1] = child_2;
    }

    if (color_c == BLACK || color_c == WHITE) {
        printf("no 3 a\n");
        node* child_3 = createNode();
        child_3->color = color_c == BLACK ? 0 : 1;
        root->children[2] = child_3;
    } else {
        printf("no 3 b\n");
        node* child_3 = createNode();
        Coordenada c3 = criaCoordenada(abs(center.x - size_x / 2), abs(center.y + size_y / 2));
        printf("Novo Centro: [%d][%d]\n", c3.x, c3.y);
        child_3 = create_children(child_3, img, depth + 1, c3, size_x / 2, size_y / 2, drawing);
        root->children[2] = child_3;
    }

    if (color_d == BLACK || color_d == WHITE) {
        printf("no 4 a\n");
        node* child_4 = createNode();
        child_4->color = color_d == BLACK ? 0 : 1;
        root->children[3] = child_4;
    } else {
        printf("no 4 b\n");
        node* child_4 = createNode();
        Coordenada c4 = criaCoordenada(abs(center.x + size_x / 2), abs(center.y + size_y / 2));
        printf("Novo Centro: [%d][%d]\n", c4.x, c4.y);
        child_4 = create_children(child_4, img, depth + 1, c4, size_x / 2, size_y / 2, drawing);
        root->children[3] = child_4;
    }

    

    return root;

}


int main () {
    Imagem *img = abreImagem("img/1.bmp", 3);
    Cor cor = criaCor(255, 0, 0);
    printf("Total: %d %d\n", img->largura, img->altura);


    Imagem *t = clonaImagem(img);
    Imagem *bpps[8];
    node* root = createNode();
    // Coordenada center = {x: img->largura / 2, y: img->altura / 2};
    Coordenada lt = { x: 0, y: 0};
    Coordenada rt = { x: img->largura, y: 0  };
    Coordenada lb = { x: 0, y: img->altura };
    Coordenada rb = { x: img->largura, y: img->altura };

    // Coordenada top = {x: img->largura / 2, y: 0 };
    // Coordenada bot = {x: img->largura / 2, y: img->altura };
    // Coordenada left = {x: 0, y: img->altura /2 };
    // Coordenada rigth = {x: img->largura, y: img->altura /2 };
    // desenhaLinha(center, top, cor, img);
    // desenhaLinha(center, bot, cor, img);
    // desenhaLinha(center, left, cor, img);
    // desenhaLinha(center, rigth, cor, img);
    // salvaImagem(img, "img/teste.bmp");

    for (int i = 0; i < 8; i++) {
        bpps[i] = criaImagem(img->largura, img->altura, 1);
    }
    for (int i = 0; i < img->largura; i++) {
        for (int j = 0; j < img->altura; j++) {
            int pixel = (img->dados[0][j][i]);
            for (int bi = 0; bi < 8; bi++) {
                unsigned int aux_pixel = ((pixel << bi) >> 7) & 0x01;
                bpps[bi]->dados[0][j][i] = aux_pixel == 1 ? 255 : 0;
            }
        }
    }
    Coordenada center = criaCoordenada(img->largura / 2, img->altura / 2);
    Imagem *drawing = clonaImagem(bpps[0]);
    create_children(root, bpps[0], 1, center, img->largura/2, img->altura/2, drawing);
    for (int i = 0; i < 8; i++) {
        // int color_a = getComponentColor(bpps[i], &center, &lt);
        // int color_b = getComponentColor(bpps[i], &center, &rt);
        // int color_c = getComponentColor(bpps[i], &center, &lb);
        // int color_d = getComponentColor(bpps[i], &center, &rb);
        // printf("No [%d] valor: %d \n", i, color_a);
        // printf("No [%d] valor: %d \n", i, color_b);
        // printf("No [%d] valor: %d \n", i, color_c);
        // printf("No [%d] valor: %d \n", i, color_d);

        // salvando imagem

        char arr[30];
        snprintf(arr, 12, "img/bpps_%d", i);
        salvaImagem(bpps[i], arr);
        destroiImagem(bpps[i]);
    }
    destroyNode(root);
    salvaImagem(t, "img/cacete.bmp");
    destroiImagem(t);
    destroiImagem(drawing);
    destroiImagem(img);

    return 0;
}