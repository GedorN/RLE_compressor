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
#define LEVEL 8

#define MIN(a,b) ((a<b)?a:b)
#define MAX(a,b) ((a>b)?a:b)

typedef struct node node;
#pragma pack(1)
struct node{
    unsigned char color: 1;
    node** children;
};
void fillComponent(int component_width, int component_height, Coordenada tl, int cor, Imagem *img) {
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
    sx->color = (unsigned char )0;
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
        return sizeof(*n) ;
    } else {
        int a = 0;
       for (int i = 0; i < 4; i++) {
           a+= size_node((n->children[i])) ;
       }
        return a;
    }
}

int get_bg_color(Imagem *img) {
    int sum_colors = 0;
    
    for (int i = 0; i < img->largura; i++) {
        for (int j = 0; j < img->altura; j++) {
            sum_colors += img->dados[0][j][i] == 0 ? -1 : 1;
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

    Coordenada a_aux = criaCoordenada(a_aux_x, a_aux_y);
    Coordenada b_aux = criaCoordenada(b_aux_x, b_aux_y);
    if (a_aux_y < b_aux_y) {
        for (int i = a_aux.x; i <= b_aux.x; i++) {
            for (int j = a_aux.y; j <= b_aux.y; j++) {
                if (j < img->altura && i < img->largura && i > 0 && j > 0) {
                    component_pixels++;
                    sum_colors += img->dados[0][j][i];
                }
            }
        }

    } else {
        for (int i = a_aux.x; i <= b_aux.x; i++) {
            for (int j = a_aux.y; j >= b_aux.y; j--) {
                if (j < img->altura && i < img->largura && i > 0 && j > 0) {
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
    if (size_x  <= 0 || size_y <= 0) {

        node* n = createNode();
        n->color = img->dados[0][center.y][center.x] == 0 ? BLACK : WHITE;
        return n;
    }
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
        child_1->color = (unsigned char)color_a & 0x01;
        root->children[0] = child_1;
    } else {
        node* child_1 = createNode();
        Coordenada c1 = criaCoordenada(ceil(center.x - size_x / 2), ceil(center.y - size_y / 2));
        child_1 = create_children(child_1, img, depth + 1, c1, (size_x / 2), (size_y / 2));
        root->children[0] = child_1;
    }

    if (color_b == BLACK || color_b == WHITE) {
        node* child_2 = createNode();
        child_2->color = (unsigned char)color_b & 0x01;
        root->children[1] = child_2;
    } else {
        node* child_2 = createNode();
        Coordenada c2 = criaCoordenada(ceil(center.x + size_x / 2), ceil(center.y - size_y / 2));
        child_2 = create_children(child_2, img, depth + 1, c2, (size_x / 2), (size_y / 2));
        root->children[1] = child_2;
    }

    if (color_c == BLACK || color_c == WHITE) {
        node* child_3 = createNode();
        child_3->color = (unsigned char)color_c & 0x01;
        root->children[2] = child_3;
    } else {
        node* child_3 = createNode();
        Coordenada c3 = criaCoordenada(ceil(center.x - size_x / 2), ceil(center.y + size_y / 2));
        child_3 = create_children(child_3, img, depth + 1, c3, (size_x / 2), (size_y / 2));
        root->children[2] = child_3;
    }

    if (color_d == BLACK || color_d == WHITE) {
        node* child_4 = createNode();
        child_4->color = (unsigned char)color_d & 0x01;
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
    Imagem *img = abreImagem("img/a.bmp", 3);


    Imagem ***bpps = (Imagem***)malloc(sizeof(Imagem**) * img->n_canais);
    node*** root = (node ***)malloc(sizeof(node**) * img->n_canais);
    for (int i = 0; i < img->n_canais; i++) {
        bpps[i] = (Imagem **) malloc(sizeof(Imagem*) * 8);
        root[i] = (node **)malloc(sizeof(node **) * LEVEL);
    }

    for (int canal = 0; canal < img->n_canais; canal++) {
        for (int i = 0; i < LEVEL; i++) {
            root[canal][i] = createNode();
        }
    }
    Coordenada lt = { x: 0, y: 0};
    Coordenada rt = { x: img->largura, y: 0  };
    Coordenada lb = { x: 0, y: img->altura };
    Coordenada rb = { x: img->largura, y: img->altura };

    for (int canal = 0; canal < img->n_canais; canal++) {
        for (int i = 0; i < LEVEL; i++) {
            bpps[canal][i] = criaImagem(img->largura, img->altura, 1);
        }
        
        for (int i = 0; i < img->largura; i++) {
            for (int j = 0; j < img->altura; j++) {
                int pixel = (img->dados[canal][j][i]);
                for (int bi = 0; bi < LEVEL; bi++) {
                    unsigned int aux_pixel = ((pixel << bi) >> 7) & 0x01;
                    bpps[canal][bi]->dados[0][j][i] = aux_pixel == 0 ? 0 : 255;
                }
            }
        }

    }
    Coordenada center = criaCoordenada(img->largura / 2, img->altura / 2);
    for (int canal = 0; canal < img->n_canais; canal++) {
        for (int i = 0; i < LEVEL; i++) {
            int color = get_bg_color(bpps[canal][i]);
            root[canal][i]->color = color == WHITE ? (unsigned char) 1 : (unsigned char) 0;
            root[canal][i] = create_children(root[canal][i], bpps[canal][i], 1, center, img->largura/2, img->altura/2);
            destroiImagem(bpps[canal][i]);
        }
        free(bpps[canal]);

    }

    free(bpps);


    //  Criando imagem final
    Imagem* final = criaImagem(img->largura, img->altura, img->n_canais);
    for (int canal = 0; canal < final->n_canais; canal++) {
        for (int i = 0; i < img->largura; i++) {
            for (int j = 0; j < img->altura; j++) {
                final->dados[canal][j][i] = 0;
            }
        }
    }

    // revertendo os bpps
    for (int canal = 0; canal < final->n_canais; canal++) {
        for (int i = 0; i < LEVEL; i++) {
            Imagem* reversed = revert_to_image(root[canal][i], img->largura, img->altura);
            for (int a = 0; a < final->largura; a++) {
                for (int b = 0; b < final->altura; b++) {
                    if (reversed->dados[0][b][a] == 0) {
                        final->dados[canal][b][a] |= 0 << (7 - i);
                    } else {
                        final->dados[canal][b][a] |= 1 << (7 - i);
                    }
                }
            }
            destroiImagem(reversed);
        }

    }


    int total = 0;
    int total_estutura = 0;
    for (int canal = 0; canal < final->n_canais; canal++) {
        for(int i = 0; i < img->largura; i++) {
            for (int j = 0; j < img->altura; j++) {
                total += sizeof(img->dados[canal][j][i]);
            }
        }
        for (int i = 0; i < LEVEL; i++) {
            total_estutura+= size_node(root[canal][i]);
            destroyNode(root[canal][i]);
        }

        free(root[canal]);
    }




    stop = clock();
    printf("Tempo total: %.3f\n", ((double)(stop - start) / CLOCKS_PER_SEC));
    printf("Total de tamanho original: %d\n", total/1000);
    printf("Total da estrutura: %d\n", total_estutura /1000);
    salvaImagem(final, "img/final.bmp");
    free(root);
    destroiImagem(final);
    destroiImagem(img);

    return 0;
}