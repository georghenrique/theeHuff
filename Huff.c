#include "Huff.h"
#include "log.h"


/*Função que recebe uma string, calcula seu tamanho e add 1 espaço de memoria, retornando a string com o tamanho aumentado*/
char *strdup(const char *s){
    log_trace("strdup <-");
    char *p = malloc(strlen(s) + 1);// calcula o comprimento da string, mas não inclui o caractere nulo de terminação.
    if(p){
        log_info("strcpy copia a string original para a nova string");
        strcpy(p, s);//A strcpy()função copia a string apontada s(incluindo o caractere nulo) para o destino p.   
    } 
    log_trace("strdup ->\n");
    return p;
}

/*função que cria um nó do tipo NodeLista de uma árvore, esses nós são usados para saber a qtd de frequencia que uma letra aparece*/
nodeLista *novoNodeLista(nodeArvore *nArv){
    log_info("cria um nó do tipo NodeLista");
    log_trace("novoNodeLista <-");
    nodeLista *novo;
    novo = malloc(sizeof(*novo));

    if(novo == NULL){
        log_error("ERRO** NÓ Ñ ALOCADO !!");
        log_debug("novo: %p", novo);
        log_trace("novoNodeLista ->\n");
        return NULL;
    }

    novo->n = nArv;
    novo->proximo = NULL;
    log_debug("novo->proximo recebe NULL: %p", novo->proximo);
    log_debug("Adiciona a árvore ao nó novo->n: %p", novo->proximo);
    log_trace("novoNodeLista ->\n");
    return novo;
}
//função que cria os nós folhas de cada letra
/** Função que faz alocação de memória e trata os ponteiros soltos acerca de nós da árvore
*o byte a ser gravado no nó, a frequencia do byte, ponteiros para os nós filhos
*/

nodeArvore *novoNodeArvore(byte c, int frequencia, nodeArvore *esquerda, nodeArvore *direita){
    log_trace("novoNodeArvore <-");
    log_info("cria um nó do tipo nodeArvore");
    nodeArvore *novo;
    novo =  malloc(sizeof(*novo));
    
    if(novo == NULL){
        log_error("ERRO** NÓ Ñ ALOCADO !!");
        log_debug("novo: %p", novo);
        log_trace("novoNodeArvore ->\n");
        return NULL;
    }

    // Atribui na árvore os valores passados como parâmetro
    novo->c = c;
    novo->frequencia = frequencia;
    novo->esquerda = esquerda;
    novo->direita = direita;
    log_debug("novo->c recebe c: %c", novo->c);
    log_debug("novo->frequencia recebe frequencia: %d", novo->frequencia);
    log_debug("novo->esquerda recebe esquerda: %p", novo->esquerda);
    log_debug("novo->direita recebe direita: %p", novo->direita);

    log_trace("novoNodeArvore ->\n");
    return novo;
}

/** Função que um novo nó na lista encadeada que representa a fila de prioridade.
* um nó previamente criado, a lista que receberá o nó
*/
//função que add nós folha em uma lista simplismente ligada 
void insereLista(nodeLista *n, lista *l){
    log_info("função que add nós folha em uma lista simplismente ligada");
    log_trace("insereLista <-");
    
    // Se a lista passada como parâmetro (lista *l) não tem um nó no início (vazia), insira o nó (nodeLista *n) no início
    if (!l->head){
        log_trace("if (!l->head) <-");
        log_error("ERRO** A LISTA ESTÁ VAZIA");
        log_info("ADD o nó N a primara posição da lista");
        l->head = n;
        log_trace("if (!l->head) ->");

    }

    /* Se o campo 'frequência' do 'nó' parâmetro (nodeLista *n) for menor que o campo 'frequência' do primeiro
    item (head) da lista (Lista *l), incluir o novo nó como head, e colocar o head antigo como next desse novo*/
    else
        if(n->n->frequencia < l->head->n->frequencia){
        log_trace("if (n->n->frequencia < l->head->n->frequencia) <-");
        log_info("add os nos um apos o outro como fila");
        n->proximo = l->head;
        l->head = n;
        log_info("n->proximo = l->head");
        log_info("l->head = n");
        log_trace("if (n->n->frequencia < l->head->n->frequencia) ->");

    }
    else{
        // nó auxiliar que inicia apontando para o segundo nó da lista (head->proximo)
        nodeLista *aux = l->head->proximo;
        // nó auxiliar que inicia apontando para o primeiro nó da lista
        nodeLista *aux2 = l->head;

        // Laço que percorre a lista e insere o nó na posição certa de acordo com sua frequência.
        //
        // Se sabe que aux começa apontando para o segundo item da lista e aux2 apontando para o primeiro.
        // Sendo assim, os ponteiros seguirão mudando de posição enquanto aux não for o fim da lista,
        // e enquanto a frequência do nó apontado por aux for menor ou igual a frequência do 'nó' parâmetro.

        while (aux && aux->n->frequencia <= n->n->frequencia)
        {
            aux2 = aux;
            aux = aux2->proximo;
        }

        // Se insere o nó na posição certa.
        aux2->proximo = n;
        n->proximo = aux;
    }

    // Incrementa quantidade de elementos
    l->elementos++;
    log_trace("insereLista ->\n");

}

/** Função que 'solta' o nó apontado por 'head' da lista (o de menor frequência)
* (faz backup do nó e o desconecta da lista)
* uma lista encadeada.
*/

nodeArvore *popMinLista(lista *l){

    // Ponteiro auxilar que aponta para o primeiro nó da lista
    nodeLista *aux = l->head;

    // Ponteiro auxiliar que aponta para a árvore contida em aux (árvore do primeiro nó da lista)
    nodeArvore *aux2 = aux->n;

    // Aponta o 'head' da lista para o segundo elemento dela
    l->head = aux->proximo;

    // Libera o ponteiro aux
    free(aux);
    aux = NULL;

    // Decrementa a quantidade de elementos
    l->elementos--;

    return aux2;
}

/** Função que conta a frequência de ocorrências dos bytes de um dado arquivo
* um arquivo, uma lista de bytes
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void getByteFrequency(FILE *entrada, unsigned int *listaBytes){
    log_info("função que adiciona todos os caracteres no vetor listaBytes com seus valores da tabela ASCII");
    log_trace("getByteFrequency <-");

    byte c;
    log_debug("criação de byte c: %d", c);
    
    /*
    * o laço while percorrerá todo o arquivo (entrada) que foi passado como parametro
    * fread( variavel q armazenará oq foi lido , tamanho de cada elemento, quantos elementos, arquivo de entrada )
    * a função fread faz a leitura de todos os caracteres do arquivo a partir da inicio lendo de 1 em 1 byte e salva oq foi lido na variavel c que é atribuida ao vetor listabytes na posição correspondente ao valor ASCII do que foi lido
    * fread retorna a quantidade de blocos lidos com sucesso
    */

    while (fread(&c, 1, 1, entrada) >= 1)
    {
        log_debug("caracter: %c - valor ASCII: %d", c, c);
        listaBytes[c]++;//guardo dentro do vetor o numero de vezes que o caracter lido aparece 
        log_debug("caracter: %c, quantidade dentro do vetor: %d", c, listaBytes[c]);
        //listaBytes[(byte)c]++; //testar sem o modificador de tipo
    }
    rewind(entrada); //"rebobina o arquivo, pois foi usado um ponteiro para navergarmos até o final do arquivo entrada"
    log_trace("getByteFrequency ->\n");

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//  Obtem o código começando no nó n, utilizando o byte salvo em 'c', preenchendo 'buffer', desde o bucket 'tamanho'

/**
/ Função recursiva que percorre uma árvore de huffman e para ao encontrar o byte procurado (c)
/ @param: nó para iniciar a busca, byte a ser buscado, buffer para salvar os nós percorridos, posição para escrever
**/

bool pegaCodigo(nodeArvore *n, byte c, char *buffer, int tamanho){

    // Caso base da recursão:
    // Se o nó for folha e o seu valor for o buscado, colocar o caractere terminal no buffer e encerrar

    if (!(n->esquerda || n->direita) && n->c == c)
    {
        buffer[tamanho] = '\0';
        return true;
    }
    else
    {
        bool encontrado = false;

        // Se existir um nó à esquerda
        if (n->esquerda)
        {
            // Adicione '0' no bucket do buffer correspondente ao 'tamanho' nodeAtual
            buffer[tamanho] = '0';

            // fazer recursão no nó esquerdo
            encontrado = pegaCodigo(n->esquerda, c, buffer, tamanho + 1);
        }

        if (!encontrado && n->direita)
        {
            buffer[tamanho] = '1';
            encontrado = pegaCodigo(n->direita, c, buffer, tamanho + 1);
        }
        if (!encontrado)
        {
            buffer[tamanho] = '\0';
        }
        return encontrado;
    }

}

// Algoritmo para construir a árvore de huffman, inspirado no seguinte pseudocódigo:
// http://www.cs.gettysburg.edu/~ilinkin/courses/Spring-2014/cs216/assignments/a8.html
//
// procedure BUILD-TREE(frequencies):
//     pq ← make empty priority queue
//     for each (symbol, key) in frequencies:
//         h ← make a leaf node for the (symbol, key) pair
//         INSERT(pq, h)
//
//     n ← size[pq]
//     for i = 1 to n-1:
//         h1 ← POP-MIN(pq)
//         h2 ← POP-MIN(pq)
//         h3 ← MAKE-NODE(h1, h2)
//         INSERT(pq, h3)
//
//     return POP-MIN(pq)

/** Função que constrói a árvore de huffman
* @param: a fila de prioridade.
*/

nodeArvore *BuildHuffmanTree(unsigned *listaBytes){
    log_info("função que controi a arvore de Huffman");
    log_trace("BuildHuffmanTree <-");

    lista l = {NULL, 0};
    log_debug("var lista l é criada: head=NULL e elementos=0");

    // Popula usando a array 'listaBytes' (que contém as frequências) uma lista encadeada de nós.
    // Cada nó contém uma árvore.
    for (int i = 0; i < 256; i++)
    {
        log_debug("teste do que está acontecendo: posição %d conteudo: %d", i, listaBytes[i]);
        if (listaBytes[i]) // Se existe ocorrência do byte
        {

            // Insere na lista 'l' um nó referente ao byte i e sua respectiva frequência (guardada em listaBytes[i]).
            // Faz os nós esquerdo e direito das árvores apontarem para NULL;
            insereLista(novoNodeLista(novoNodeArvore(i, listaBytes[i], NULL, NULL)), &l);
        }
    }

    while (l.elementos > 1) // Enquanto o número de elementos da lista for maior que 1
    {
        // Nó esquerdo chama a função popMinLista() que vai na lista e pega a árvore fixada no primeiro nó
        // (que é a que contém a menor frequência)
        nodeArvore *nodeEsquerdo = popMinLista(&l);

        // Pega o outro nó que tem menor frequência
        nodeArvore *nodeDireito = popMinLista(&l);

        // Preenche com '#' o campo de caractere do nó da árvore.
        // Preenche o campo 'frequência' com a soma das frequências de 'nodeEsquerdo' e 'nodeDireito'.
        // Aponta o nó esquerdo para nodeEsquerdo e o nó direito para nodeDireito
        nodeArvore *soma = novoNodeArvore(
                               '#',
                               nodeEsquerdo->frequencia + nodeDireito->frequencia, nodeEsquerdo, nodeDireito
                           );

        // Reinsere o nó 'soma' na lista l
        insereLista(novoNodeLista(soma), &l);
    }
    
    log_trace("BuildHuffmanTree ->\n");
    return popMinLista(&l);
}

/** Função que libera memória da árvore de huffman
* @param: nó de uma (sub)árvore.
*/

void FreeHuffmanTree(nodeArvore *n){
    // Caso base da recursão, enquanto o nó não for NULL
    if (!n) return;
    else
    {
        nodeArvore *esquerda = n->esquerda;
        nodeArvore *direita = n->direita;
        free(n);
        FreeHuffmanTree(esquerda);
        FreeHuffmanTree(direita);
    }
}

/** Função que faz bitmasking no byte lido e retorna um valor booleano confirmando sua existência
* Ideia do bitmasking surgiu da leitura de http://ellard.org/dan/www/CS50-95/s10.html
* @param: arquivo para ler o byte, posição que se deseja mascarar o byte, byte a ser feita a checagem
*/

int geraBit(FILE *entrada, int posicao, byte *aux ){
    // É hora de ler um bit?
    (posicao % 8 == 0) ? fread(aux, 1, 1, entrada) : NULL == NULL ;

    // Exclamação dupla converte para '1' (inteiro) se não for 0. Caso contrário, deixa como está (0)
    // Joga '1' na casa binária 'posicao' e vê se "bate" com o byte salvo em *aux do momento
    // Isso é usado para percorrer a árvore (esquerda e direita)
    return !!((*aux) & (1 << (posicao % 8)));
}

/** Função para notificar ausência do arquivo. Encerra o programa em seguida.
*/
void erroArquivo(){
    printf("Arquivo nao encontrado\n");
    exit(0);
}

/** Função que comprime um arquivo utilizando a compressão de huffman
* @param: arquivo a comprimir, arquivo resultado da compressão
*/

void CompressFile(const char *arquivoEntrada, const char *arquivoSaida){

    clock_t inicio, final;
    double tempoGasto;
    inicio = clock();

    unsigned listaBytes[256] = {0};

    // Abre arquivo do parâmetro arquivoEntrada no modo leitura de binário
    FILE *entrada = fopen(arquivoEntrada, "rb");
    (!entrada) ? erroArquivo() : NULL == NULL ;

    // Abre arquivo do parâmetro arquivoSaida no modo escrita de binário
    FILE *saida = fopen(arquivoSaida, "wb");
    (!saida) ? erroArquivo() : NULL == NULL ;

    log_info("chamada da função getByteFrequency que armzena os caracyeres no vetor listaBytes");
    getByteFrequency(entrada, listaBytes);

    // Populando a árvore com a lista de frequência de bytes
    nodeArvore *raiz = BuildHuffmanTree(listaBytes);

    // Grava a lista de frequência nos primeiros 256 bytes do arquivo
    fwrite(listaBytes, 256, sizeof(listaBytes[0]), saida);

    // Move o ponteiro do stream 'saida' para a posição posterior ao tamanho de um unsigned int
    // É aqui que posteriormente será salvo o valor da variável 'tamanho'
    fseek(saida, sizeof(unsigned int), SEEK_CUR);

    byte c;
    unsigned tamanho = 0;
    byte aux = 0;

    /***
    * fread( array/bloco de memoria , tamanho de cada elemento, quantos elementos, arquivo de entrada )
    * fread retorna a quantidade de bytes lidos com sucesso
    *
    * Faz a leitura de 1 bloco de tamanho 1 byte a partir do arquivo 'entrada'
    * e salva no espaco de memoria de 'c'.
    ***/

    while (fread(&c, 1, 1, entrada) >= 1)
    {

        // Cria um buffer vazio
        char buffer[1024] = {0};

        // Busca o código começando no nó 'raiz', utilizando o byte salvo em 'c', preenchendo 'buffer', desde o bucket deste último
        pegaCodigo(raiz, c, buffer, 0);

        // Laço que percorre o buffer
        for (char *i = buffer; *i; i++)
        {
            // Se o caractere na posição nodeAtual for '1'
            if (*i == '1')
            {
                // 2 elevado ao resto da divisão de 'tamanho' por 8
                // que é o mesmo que jogar um '1' na posição denotada por 'tamanho % 8'
                //aux = aux + pow(2, tamanho % 8);
                aux = aux | (1 << (tamanho % 8));
            }

            tamanho++;

            // Já formou um byte, é hora de escrevê-lo no arquivo
            if (tamanho % 8 == 0)
            {
                fwrite(&aux, 1, 1, saida);
                // Zera a variável auxiliar
                aux = 0;
            }
        }
    }

    // Escreve no arquivo o que sobrou
    fwrite(&aux, 1, 1, saida);

    // Move o ponteiro do stream para 256 vezes o tamanho de um unsigned int, a partir do início dele (SEEK_SET)
    fseek(saida, 256 * sizeof(unsigned int), SEEK_SET);

    // Salva o valor da variável 'tamanho' no arquivo saida
    fwrite(&tamanho, 1, sizeof(unsigned), saida);

    final = clock();
    tempoGasto = (double)(final - inicio) / CLOCKS_PER_SEC;

    // Calcula tamanho dos arquivos
    fseek(entrada, 0L, SEEK_END);
    double tamanhoEntrada = ftell(entrada);

    fseek(saida, 0L, SEEK_END);
    double tamanhoSaida = ftell(saida);

    FreeHuffmanTree(raiz);

    printf("Arquivo de entrada: %s (%g bytes)\nArquivo de saida: %s (%g bytes)\nTempo gasto: %gs\n", arquivoEntrada, tamanhoEntrada / 1000, arquivoSaida, tamanhoSaida / 1000, tempoGasto);
    printf("Taxa de compressao: %d%%\n", (int)((100 * tamanhoSaida) / tamanhoEntrada));

    fclose(entrada);
    fclose(saida);

}

/** Função que descomprime um arquivo utilizando a compressão de huffman
* @param: arquivo a descomprimir, arquivo resultado da descompressão
*/

void DecompressFile(const char *arquivoEntrada, const char *arquivoSaida){

    clock_t inicio, final;
    double tempoGasto;
    inicio = clock();

    unsigned listaBytes[256] = {0};

    // Abre arquivo do parâmetro arquivoEntrada no modo leitura de binário
    FILE *entrada = fopen(arquivoEntrada, "rb");
    (!entrada) ? erroArquivo() : NULL == NULL ;

    // Abre arquivo do parâmetro arquivoSaida no modo escrita de binário
    FILE *saida = fopen(arquivoSaida, "wb");
    (!saida) ? erroArquivo() : NULL == NULL ;

    // Lê a lista de frequência que encontra-se nos primeiros 256 bytes do arquivo
    fread(listaBytes, 256, sizeof(listaBytes[0]), entrada);

    // Constrói árvore
    nodeArvore *raiz = BuildHuffmanTree(listaBytes);

    // Lê o valor dessa posição do stream para dentro da variável tamanho
    unsigned tamanho;
    fread(&tamanho, 1, sizeof(tamanho), entrada);

    unsigned posicao = 0;
    byte aux = 0;

    // Enquanto a posicao for menor que tamanho
    while (posicao < tamanho)
    {
        // Salvando o nodeArvore que encontramos
        nodeArvore *nodeAtual = raiz;

        // Enquanto nodeAtual não for folha
        while ( nodeAtual->esquerda || nodeAtual->direita )
        {
            nodeAtual = geraBit(entrada, posicao++, &aux) ? nodeAtual->direita : nodeAtual->esquerda;
        }

        fwrite(&(nodeAtual->c), 1, 1, saida);
    }

    FreeHuffmanTree(raiz);

    final = clock();
    tempoGasto = (double)(final - inicio) / CLOCKS_PER_SEC;

    fseek(entrada, 0L, SEEK_END);
    double tamanhoEntrada = ftell(entrada);

    fseek(saida, 0L, SEEK_END);
    double tamanhoSaida = ftell(saida);

    printf("Arquivo de entrada: %s (%g bytes)\nArquivo de saida: %s (%g bytes)\nTempo gasto: %gs\n", arquivoEntrada, tamanhoEntrada / 1000, arquivoSaida, tamanhoSaida / 1000, tempoGasto);
    printf("Taxa de descompressao: %d%%\n", (int)((100 * tamanhoSaida) / tamanhoEntrada));

    fclose(saida);
    fclose(entrada);
}


int main(int argc, char *argv[]){
    // Caso os parâmetros informados sejam insuficientes
    if (argc < 4)
    {
        printf("Uso: huffman [OPCAO] [ARQUIVO] [ARQUIVO]\n\n");
        printf("Opcoes:\n");
        printf("\t-c\tComprime\n");
        printf("\t-x\tDescomprime\n");
        printf("\nExemplo: ./huffman -c comprima.isso nisso.hx\n");
        return 0;
    }

    if (strcmp("-c", argv[1]) == 0)
    {
        if (strstr(argv[3], ".hx"))
        {
            CompressFile(argv[2], argv[3]);
        }
        else
        {
            printf("O arquivo resultante da compressao deve ter a extensao '.hx'.\n");
            printf("Exemplo: \n\t./huffman -c comprima.isso nisso.hx\n");
            return 0;
        }
    }
    else if (strcmp("-x", argv[1]) == 0)
    {
        if (strstr(argv[2], ".hx"))
        {
            DecompressFile(argv[2], argv[3]);
        }
        else
        {
            printf("O arquivo a ser descomprimido deve ter a extensao '.hx'.\n");
            printf("Exemplo: \n\t./huffman -x descomprime.hx nisso.extensao\n");
            return 0;
        }
    }
    else
    {
        printf("Uso: huffman [OPCAO] [ARQUIVO] [ARQUIVO]\n\n");
        printf("Opcoes:\n");
        printf("\t-c\tComprime\n");
        printf("\t-x\tDescomprime\n");
        printf("\nExemplo: ./huffman -c comprima.isso nisso.hx\n");
        return 0;
    }

    //CompressFile("meslo.ttf", "meslo.hx");
    //DecompressFile("meslo.hx", "meslo_copy.ttf");

    return 0;
}