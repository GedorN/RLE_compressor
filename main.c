#include <stdio.h>
#include <stdlib.h>
#include "imagem.h"
#include "geometria.h"
#include "desenho.h"
#include "cores.h"

#include <time.h>

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
void fillComponent(int component_width, int component_height, Coordenada tl, int cor, Imagem *img) {
    // printf("Vou começar do [%d][%d]\n", tl.x, tl.y);
    // printf("Até o %d x %d\n", component_width, component_height);
    for(int i = tl.x - 1; i <= tl.x + component_width + 1; i++) {
        for (int j = tl.y - 1; j <= tl.y + component_height + 1; j++) {
            if (j + component_height < img->altura && i + component_width < img->largura && i -1  > 0 && j - 1 > 0) {
                img->dados[0][j][i] = cor;
            }
        }
    }

}

void set_component(node* root, Imagem *img, Coordenada tl, int component_width, int component_height) {
    Coordenada tl1 = criaCoordenada(tl.x, tl.y);
    Coordenada tl2 = criaCoordenada(tl.x + (component_width / 2), tl.y  );
    Coordenada tl3 = criaCoordenada(tl.x, tl.y + (component_height / 2));
    Coordenada tl4 = criaCoordenada(tl.x + (component_width / 2), tl.y + (component_height / 2));
    // printf("Ué véi 1\n");
    // fillComponent(component_width / 2, component_height / 2, tl, 255, img);
    // printf("Ué véi 2\n");
    // fillComponent(component_width / 2, component_height / 2, tl2, 255, img);
    // printf("Ué véi 3\n");
    // fillComponent(component_width / 2, component_height / 2, tl3, 255, img);
    // printf("Ué véi 4\n");
    // fillComponent(component_width / 2, component_height / 2, tl4, 255, img);

    // fillComponent(component_width, component_height, tl, 255, img);
    if (root->children[0] == NULL && root->children[1] == NULL && root->children[2] == NULL && root->children[3] == NULL) {
        int cor = root->color == 0 ? 0 : 255;
        fillComponent(component_width, component_height, tl, cor, img);
    } else {
        if (root->children[0] != NULL) {
            set_component(root->children[0], img, tl1, (component_width / 2), (component_height / 2));
        }
        if (root->children[1] != NULL) {
            set_component(root->children[1], img, tl2, (component_width / 2), (component_height / 2));
        }
        if (root->children[2] != NULL) {
            set_component(root->children[2], img, tl3, (component_width / 2), (component_height / 2));
        }
        if (root->children[3] != NULL) {
            set_component(root->children[3], img, tl4, (component_width / 2), (component_height / 2));
        }
    }

}

Imagem* revert_to_image(node* root, int img_width, int img_height) {
    int depth = 1;
    int bg = root->color == 0 ? 0 : 255;
    Imagem *img = criaImagem(img_width, img_height, 1);
    for (int i = 0; i < img->largura; i++) {
        for (int j = 0; j < img->altura; j++) {
            img->dados[0][j][i] = bg;
        }
    }

    Coordenada center = criaCoordenada(img_width / 2, img_height / 2);
    Coordenada tl1 = criaCoordenada(0, 0);
    Coordenada tl2 = criaCoordenada(center.x, 0);
    Coordenada tl3 = criaCoordenada(0, center.y);
    Coordenada tl4 = criaCoordenada(center.x, center.y);

    set_component(root->children[0], img, tl1, (img_width / 2), (img_height / 2));
    set_component(root->children[1], img, tl2, (img_width / 2), (img_height / 2));
    set_component(root->children[2], img, tl3, (img_width / 2), (img_height / 2));
    set_component(root->children[3], img, tl4, (img_width / 2), (img_height / 2));





    return img;
}

node* createNode() {
    node* sx = malloc(sizeof(node));
    sx->color = 0;
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



void print_node(node* n) {
    if (n->children[0] == NULL && n->children[1] == NULL && n->children[2] == NULL && n->children[3] == NULL) {
        printf("%u\n", n->color);
    } else {
        for (int i = 0; i < 4; i++) {
            print_node(n->children[i]);
        }
    }
}
int size_node(node* n) {

    if (n->children[0] == NULL && n->children[1] == NULL && n->children[2] == NULL && n->children[3] == NULL) {
        return sizeof(*n);
    } else {
        int a = 0;
       for (int i = 0; i < 4; i++) {
           a+= size_node((n->children[i]));
       }
        return a;
    }
}

int get_bg_color(Imagem *img) {
    int sum_colors = 0;
    
    for (int i = 0; i < img->largura; i++) {
        for (int j = 0; j < img->altura; j++) {
            // printf("somando esse: %u\n", img->dados[0][i][j]);
            sum_colors += img->dados[0][j][i] == 255 ? 1 : -1;
        }
    }
    if (sum_colors > 0) {
        return WHITE;
    } else {
        return BLACK;
    }

}
int getComponentColor(Imagem *img, Coordenada *a, Coordenada *b) {
    int component_pixels = 0;
    unsigned int sum_colors = 0;
    int a_aux_x, a_aux_y, b_aux_x, b_aux_y;
    if ((a->x) < (b->x)) {
        a_aux_x = a->x;
        a_aux_y = a->y ;

        b_aux_x = b->x;
        b_aux_y = b->y;
    } else {
        a_aux_x = b->x;
        a_aux_y = b->y ;

        b_aux_x = a->x;
        b_aux_y = a->y;
    }
    // int a_aux_x = (a->x) < (b->x) ? a->x : b->x;
    // int a_aux_y = (a->y) < (b->y) ? a->y : b->y;
    
    
    // int b_aux_x = (a->x) > (b->x) ? a->x : b->x;
    // int b_aux_y = (a->y) > (b->y) ? a->y : b->y;

    Coordenada a_aux = criaCoordenada(a_aux_x, a_aux_y);
    Coordenada b_aux = criaCoordenada(b_aux_x, b_aux_y);
    if (a_aux_y < b_aux_y) {
        for (int i = a_aux.x; i < b_aux.x; i++) {
            for (int j = a_aux.y; j < b_aux.y; j++) {
                component_pixels++;
                sum_colors += img->dados[0][j][i];
            }
        }

    } else {
        for (int i = a_aux.x; i < b_aux.x; i++) {
            for (int j = a_aux.y; j > b_aux.y; j--) {
                if (j < img->altura && i < img->largura) {
                    component_pixels++;
                    sum_colors += img->dados[0][j][i];

                }
            }
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

node* create_children(node* root, Imagem *img, int depth, Coordenada center, int size_x, int size_y) {
    Cor cor = criaCor(255, 0, 0);
    if (size_x  == 0 || size_y == 0) {

        node* n = createNode();
        n->color = img->dados[0][center.y][center.x] == 0 ? BLACK : WHITE;
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



   if (color_a == BLACK || color_a == WHITE) {
        node* child_1 = createNode();
        child_1->color = color_a;
        root->children[0] = child_1;
    } else {
        node* child_1 = createNode();
        Coordenada c1 = criaCoordenada(ceil(center.x - size_x / 2), ceil(center.y - size_y / 2));
        child_1 = create_children(child_1, img, depth + 1, c1, (size_x / 2), (size_y / 2));
        root->children[0] = child_1;
    }

    if (color_b == BLACK || color_b == WHITE) {
        node* child_2 = createNode();
        child_2->color = color_b;
        root->children[1] = child_2;
    } else {
        node* child_2 = createNode();
        Coordenada c2 = criaCoordenada(ceil(center.x + size_x / 2), ceil(center.y - size_y / 2));
        child_2 = create_children(child_2, img, depth + 1, c2, (size_x / 2), (size_y / 2));
        root->children[1] = child_2;
    }

    if (color_c == BLACK || color_c == WHITE) {
        node* child_3 = createNode();
        child_3->color = color_c;
        root->children[2] = child_3;
    } else {
        node* child_3 = createNode();
        Coordenada c3 = criaCoordenada(ceil(center.x - size_x / 2), ceil(center.y + size_y / 2));
        child_3 = create_children(child_3, img, depth + 1, c3, (size_x / 2), (size_y / 2));
        root->children[2] = child_3;
    }

    if (color_d == BLACK || color_d == WHITE) {
        node* child_4 = createNode();
        child_4->color = color_d;
        root->children[3] = child_4;
    } else {
        node* child_4 = createNode();
        Coordenada c4 = criaCoordenada(ceil(center.x + size_x / 2), ceil(center.y + size_y / 2));
        child_4 = create_children(child_4, img, depth + 1, c4, (size_x / 2), (size_y / 2));
        root->children[3] = child_4;
    }
    
    return root;

}


int main () {
    clock_t start, stop;
    start = clock();
    Imagem *img = abreImagem("img/1.bmp", 3);
    Cor cor = criaCor(255, 0, 0);
    // printf("Total: %d %d\n", img->largura, img->altura);


    Imagem *bpps[8];
    node** root = (node **)malloc(sizeof(node*) * 8);
    for (int i = 0; i < 8; i++) {
        root[i] = createNode();
    }
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
    for (int i = 0; i < 8; i++) {
        int color = get_bg_color(bpps[i]);
        root[i]->color = color == WHITE ? 1 : 0;
        create_children(root[i], bpps[i], 1, center, img->largura/2, img->altura/2);
        // create_children(root[i], bpps[i], 1, center, img->largura/2, img->altura/2);
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

    for (int i = 0; i < 8; i++) {
        Imagem* reversed = revert_to_image(root[i], img->largura, img->altura);

        char arra[300];
        snprintf(arra, 50, "img/reversed_bpps_%d", i);
        printf("salvando... %s\n", arra);
        salvaImagem(reversed, arra);
        destroiImagem(reversed);
    }


    int total = 0;
    for(int i = 0; i < img->largura; i++) {
        for (int j = 0; j < img->altura; j++) {
            total += sizeof(img->dados[0][j][i]);
        }
    }
    int total_estutura = 0;
    for (int i = 0; i < 8; i++) {
        // print_node(root[i]);
        total_estutura+= size_node(root[i]);
        destroyNode(root[i]);
    }


    printf("Total de tamanho original: %d\n", total);
    printf("Total da estrutura: %d\n", total_estutura + (0 * img->altura * img->largura));
    destroyNode(root);
    destroiImagem(drawing);
    destroiImagem(img);
    stop = clock();
    printf("Tempo total: %.3f\n", ((double)(stop - start) / CLOCKS_PER_SEC));

    return 0;
}