#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "sudoku.c"

char * _strdup(const char * str) {
    char * aux = (char *)malloc(strlen(str) + 1);
    strcpy(aux, str);
    return aux;
}

char msg[300];
int test_id = -1;


int success(){
  printf("SUCCESS\n");
  exit(0);
}

void err_msg(char* msg){
    //if(test_id!=-1) return;
    printf("   [FAILED] ");
    printf("%s\n",msg);
    //print_trace();
}

void ok_msg(char* msg){
    //if(test_id!=-1) return;
    printf ("   [OK] ");
    printf("%s\n",msg);
}

void info_msg(char* msg){
    //if(test_id!=-1) return;
    printf ("   [ INFO ] %s\n",msg);
}

int test_suite(int(*test)(), char* msg, int max_score, int id, int req_id){
    if(test_id==-1 || test_id==id){
        printf("\n%s\n", msg);
        int score=test();

        if(id==req_id && score == max_score) success();

        printf("   partial_score: %d/%d\n", score, max_score); 
        
        return score;
    }
    return 0;
}


/*************  TESTS  ******************/

int adj_test1(){
    info_msg("cargando sudoku de la figura de ejemplo");
    Node* initial = read_file("s12a.txt");
    
    info_msg("obteniendo nodos adyacentes");
    List* adj = get_adj_nodes(initial);
    
    if(adj == NULL || is_empty(adj)==1){
        err_msg("retorna lista vacia o nula");
        return 0;
    }

    Node* aux = first(adj);
    Node* aux2 = next(adj);
    if (aux==aux2){
        err_msg("primer y segundo puntero en la lista apuntan al mismo dato!");
        return 0;
    }
    if(aux->sudo[0][0]!=5 || aux->sudo[0][1]!=3 || aux->sudo[0][2]!=1){
        sprintf(msg, "primer nodo %d %d %d... (deberia ser 5 3 1...)",aux->sudo[0][0],aux->sudo[0][1],aux->sudo[0][2]);
        err_msg(msg);
        return 0;
    }

    if(aux2->sudo[0][0]!=5 || aux2->sudo[0][1]!=3 || aux2->sudo[0][2]!=2){
        sprintf(msg, "segundo nodo %d %d %d... (deberia ser 5 3 2...)",aux2->sudo[0][0],aux2->sudo[0][1],aux2->sudo[0][2]);
        err_msg(msg);
        return 0;
    }
    ok_msg("los dos primeros nodos adyacentes parecen correctos");

    info_msg("llenando el tablero con unos");
    int i,j;
    for(i=0; i<9; i++)
        for(j=0; j<9; j++)
            initial->sudo[i][j]=1;

    info_msg("obteniendo nodos adyacentes");
    adj = get_adj_nodes(initial);

    if(is_empty(adj)==0){
        err_msg("deberia retornar lista vacia (pero no lo hace)");
        return 5;
    }

    ok_msg("retorna lista vacia");
    return 15;
}

int test_isvalid(){
    info_msg("cargando sudoku de la figura de ejemplo");
    Node* initial = read_file("s12a.txt");



    if(!is_valid(initial)){
        err_msg("is_valid retorna 0");
        return 0;
    }

    info_msg("duplicando 5 en primera fila ([0][8]=5)");
    initial->sudo[0][8]=5;

    if(is_valid(initial)){
        err_msg("is_valid retorna 1");
        return 0;
    }
    ok_msg("validacion correcta");
    info_msg("volviendo a estado anterior ([0][8]=0)");
    initial->sudo[0][8]=0;

    info_msg("duplicando 7 en quinta fila ([8][4]=7)");
    initial->sudo[8][4]=7;

    if(is_valid(initial)){
        err_msg("is_valid retorna 1");
        return 5;
    }
    ok_msg("validacion correcta");
    info_msg("volviendo a estado anterior ([8][4]=0)");
    initial->sudo[8][4]=0;

    info_msg("duplicando en submatriz central ([2][3]=6)");
    initial->sudo[2][3]=6;  
    if(is_valid(initial)){
        err_msg("is_valid retorna 1");
        return 10;
    }

    ok_msg("al parecer la validacion es correcta");  
    return 20;
}

int test_mod_adj(){
    info_msg("cargando sudoku de la figura de ejemplo");
    Node* initial = read_file("s12a.txt");

    info_msg("obteniendo lista de nodos adyacentes");
    List* adj = get_adj_nodes(initial);
    if(get_size(adj) != 3){
        sprintf(msg,"la lista deberia tener tamanno 3 pero tiene tamanno %d",get_size(adj));
        err_msg(msg);
        return 0;
    }


    Node* aux1 = first(adj);
    Node* aux2 = next(adj); 
    Node* aux3 = next(adj);
    if(aux1->sudo[0][2]!=1 || aux2->sudo[0][2]!=2 || aux3->sudo[0][2]!=4){
        sprintf(msg,"los valores validos para la tercera casilla son 1,2,4 y no %d,%d y %d",aux1->sudo[0][2],aux2->sudo[0][2],aux3->sudo[0][2]);
        err_msg(msg);
        return 0;
    }

    ok_msg("is_valid se integra correctamente en get_adjacent_nodes");
    return 10;
}

int test_isfinal(){
    info_msg("cargando sudoku de la figura de ejemplo");
    Node* initial = read_file("s12a.txt");
    
    if(is_final(initial)){
        err_msg("is_final retorna 1");
        return 0;
    }

    info_msg("llenando el tablero con unos");
    int i,j;
    for(i=0; i<9; i++)
        for(j=0; j<9; j++)
            initial->sudo[i][j]=1;

    if(!is_final(initial)){
        err_msg("is_final retorna 0");
        return 0;
    }

    ok_msg("funcion is_final parece correcta");

    return 10;
}

int test_dfs(){
    info_msg("cargando sudoku de la figura de ejemplo");
    Node* initial = read_file("s12a.txt");

    int cont=0;
    Node* final = DFS(initial, &cont);

    if(final == NULL){
        err_msg("DFS retorna NULL");
        return 0;
    }

    if(!is_valid(final)){
        err_msg("estado final no es valido");
        return 0;
    }

    if(!is_final(final)){
        err_msg("estado final no es final");
        return 0;
    }
   
    ok_msg("al parecer sudoku fue resuelto de manera correcta");

    info_msg("cargando sudoku de la figura de ejemplo");
    initial = read_file("s12a.txt");

    info_msg("alterando sudoku para que no tenga solucion");
    initial->sudo[0][2]=1;
    final = DFS(initial, &cont);

    if(final != NULL){
        err_msg("DFS deberia retornar NULL");
        return 5;
    }

    ok_msg("DFS o encuentra solucion");
    ok_msg("al parecer DFS se implementa de manera correcta");

    return 15;


}

int main( int argc, char *argv[] ) {
    
    if(argc>1) test_id=atoi(argv[1]);

    int total_score=0;
    total_score+=test_suite(adj_test1, "Test get_adj_nodes...", 15, 0, test_id);
    total_score+=test_suite(test_isvalid, "Test is_valid...", 20, 1, test_id);
    total_score+=test_suite(test_mod_adj, "Test get_adj + is_valid...", 10, 2, test_id);
    total_score+=test_suite(test_isfinal, "Test is_final...", 10, 3, test_id);
    total_score+=test_suite(test_dfs, "Test DFS...", 15, 4, test_id);

    if(argc==1)
      printf("\ntotal_score: %d/70\n", total_score);

    

    return 0;
}
