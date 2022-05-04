
Resolviendo el sudoku con grafos
=====

---
**ANTES DE COMENZAR**

* Abra su cuenta en repl.it
* En el home de repl.it busque el lab y clónelo para comenzar a trabajar
* Seleccione el lenguaje (C) y escriba el comando para el botón run: `bash test.sh`
* Sólo puede modificar los archivos *sudoku.c*
* Para testear sus códigos basta con que apriete el botón **Run**
* Actualice seguido sus cambios, debe ingresar su contraseña de github y [token](https://drive.google.com/file/d/1s3PyI_HCn0HAx5dm7h0jRiC-G9Cq2p-g/view?usp=sharing)
* No está permitido usar comandos de git (a menos que el profesor lo indique)

---


En este laboratorio intentarán resolver un tablero de [sudoku](https://www.sudoku-online.org/) usando un grafo implícito y un algoritmo de búsqueda.

![image](https://upload.wikimedia.org/wikipedia/commons/thumb/f/ff/Sudoku-by-L2G-20050714.svg/364px-Sudoku-by-L2G-20050714.svg.png)

Antes de comenzar el laboratorio debe haber visto la presentación sobre [grafos](https://docs.google.com/presentation/d/1Mqo51pc6knDje153O2eqYiS_tN8Nv6qDz4czjHp493s/edit#slide=id.g15c7a1a2a9_0_0).


Construcción del grafo implícito
----

Representaremos el *proceso de resolución del problema* usando un *grafo implícito*:
- Los *nodos del grafo* corresponderán a **estados** del problema, es decir a un tablero parcialmente resuelto.
- Los *arcos* representarán las **acciones** para movernos de un estado a otro.
  
Una **acción** corresponderá a **colocar un número en una casilla vacía del tablero**.

El tipo de dato nodo contiene una matriz que representa el tablero del juego:

    typedef struct{
       int sudo[9][9];
    }Node;

Para el ejemplo de la figura la matriz, la variable sudo del nodo se vería así (los ceros indican las casillas vacías):

    5 3 0 0 7 0 0 0 0 
    6 0 0 1 9 5 0 0 0
    0 9 8 0 0 0 0 6 0
    8 0 0 0 6 0 0 0 3 
    4 0 0 8 0 3 0 0 1
    7 0 0 0 2 0 0 0 6
    0 6 0 0 0 0 2 8 0
    0 0 0 4 1 9 0 0 5
    0 0 0 0 8 0 0 7 9

Ejercicios
--

1.Cree una función que a partir de un nodo genere una **lista con los nodos adyacentes**:

    List* get_adj_nodes(Node* n){
       List* list=createList();
       /* 
       obtenga los nodos adyacentes a n
       y agréguelos a la lista
       */
       return list;
    }

Recuerde que los nodos adyacentes son generados aplicando las acciones al estado actual.

> - Para el caso del ejemplo, la función debería retornar una lista con **9 nodos**. Cada uno de ellos idéntico al nodo original pero cambiando el valor de la primera casilla vacía, es decir: sudo[0][2], por 1, 2, ..., 9.
> - Utilice la función Node* copy(Node* n) para copiar nodos.

2.Cree la función **int is_valid(Node * n)**, para validar si un estado/nodo es válido (cumple con las restricciones del problema). Debe validar que:

- No se repitan números en las filas
- No se repitan números en las columnas
- No se repitan números en las submatrices de 3x3

Si el estado es válido la función retorna 1, si no lo es retorna 0.

> Para marcar los números que vayan apareciendo en una fila/columna/submatriz puede usar un arreglo de enteros de largo 10 inicializado con 0s. Cada vez que aparezca un número i, verifique que la casilla i del arreglo sea igual a 0, luego márquela con un '1'. Si la casilla es '1' quiere decir que el número ya estaba marcado por lo que la fla/columna/submatriz no es válida.

----
**¿Cómo recorrer las submatrices de 3x3?**
El siguiente código se puede usar para recorrer los elementos de una sumbatriz k (por ejemplo k=4):

    int k=4,p; 
    for(p=0;p<9;p++){
        int i=3*(k/3) + (p/3) ;
        int j=3*(k%3) + (p%3) ;
        printf("%d ",nodo->sudo[i][j]);
        if(p%3 == 2) printf("\n");
    }

La salida del código con el estado de la figura es (submatriz del centro):
    
    0 6 0
    8 0 3
    0 2 0

-----

3.Modifique la función *get_adj_nodes* para que sólo los nodos válidos sean retornados (use la función *is_valid*).


4.Implemente la función **int is_final(Node * n)**. Esta función retorna 1 si el nodo corresponde a un nodo final (es decir, todos los valores de la matriz son distintos a 0) y 0 en caso contrario.

Ya está listo el grafo implícito, ahora necesitamos construir un método de búsqueda que a partir del estado inicial sea capaz de explorar el grafo de manera sistemática hasta alcanzar un nodo final.

Implementando una búsqueda en profundidad
--

Antes de continuar se sugiere que haya visto y **entendido** la segunda parte de la presentación sobre [grafos](https://docs.google.com/presentation/d/1Mqo51pc6knDje153O2eqYiS_tN8Nv6qDz4czjHp493s/edit#slide=id.g1df2841e01_0_77) (recuerde investigar o preguntar si no entiende bien algo).

5.Implemente la función Node* DFS(Node* n, int* cont). Esta función realiza una búsqueda en profundidad a partir del nodo n. El algoritmo es el siguiente:

1. Cree un stack S (pila) e inserte el nodo.
2. Mientras el stack S no se encuentre vacío:

   a) Saque y elimine el primer nodo de S.
   
   b) Verifique si corresponde a un estado final, si es así retorne el nodo.
   
   c) Obtenga la lista de nodos adyacentes al nodo.
   
   d) Agregue los nodos de la lista (uno por uno) al stack S.
   
   e) Libere la memoria usada por el nodo.
   
3. Si terminó de recorre el grafo sin encontrar una solución, retorne NULL.

Almacene en la variable cont, la cantidad de iteraciones que realiza el algoritmo.

> - Puede ver un código de ejemplo en las [diapos](https://docs.google.com/presentation/d/1Mqo51pc6knDje153O2eqYiS_tN8Nv6qDz4czjHp493s/edit#slide=id.g27f9355a1a_0_13).
> - Recuerde revisar las operaciones del TDA Stack en el archivo *list.h*.

Acaba de implementar un método para resolver sudokus!
Puede probarlo usando el siguiente código (descomente el main en *sudoku.c*):

    int main( int argc, char *argv[] ){

       Node* initial= read_file("s12a.txt");

       int cont=0;
       Node* final = DFS(initial, &cont);
       printf("iterations:%d\n",cont);
       print_node(final);

       return 0;
    }

Compile: `gcc sudoku -o sudoku`
y ejecute: `./sudoku`

**Recuerde volver a comentar el main para subir su nota al servidor.**
