#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TAMANHO_FILA 5

// Estrutura para representar uma peça do Tetris
typedef struct {
    char tipo;      // 'I', 'O', 'T', 'L'
    int id;         // identificador único
} Peca;

// Estrutura para a fila circular
typedef struct {
    Peca pecas[TAMANHO_FILA];
    int frente;
    int tras;
    int quantidade;
} FilaCircular;

// Função para gerar uma peça aleatória
Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    
    // Gera um ID único baseado no tempo
    novaPeca.id = rand() % 1000 + 1;
    
    // Escolhe um tipo aleatório
    novaPeca.tipo = tipos[rand() % 4];
    
    return novaPeca;
}

// Função para inicializar a fila circular
void inicializarFila(FilaCircular *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->quantidade = 0;
    
    // Preenche a fila com peças iniciais
    for (int i = 0; i < TAMANHO_FILA; i++) {
        fila->pecas[i] = gerarPeca();
        fila->tras = (fila->tras + 1) % TAMANHO_FILA;
        fila->quantidade++;
    }
}

// Função para verificar se a fila está vazia
int filaVazia(FilaCircular *fila) {
    return fila->quantidade == 0;
}

// Função para verificar se a fila está cheia
int filaCheia(FilaCircular *fila) {
    return fila->quantidade == TAMANHO_FILA;
}

// Função para enfileirar (inserir) uma peça
void enfileirar(FilaCircular *fila, Peca peca) {
    if (filaCheia(fila)) {
        printf("Erro: Fila cheia!\n");
        return;
    }
    
    fila->tras = (fila->tras + 1) % TAMANHO_FILA;
    fila->pecas[fila->tras] = peca;
    fila->quantidade++;
}

// Função para desenfileirar (remover) uma peça
Peca desenfileirar(FilaCircular *fila) {
    Peca pecaVazia = {'?', -1}; // Peça vazia para erro
    
    if (filaVazia(fila)) {
        printf("Erro: Fila vazia!\n");
        return pecaVazia;
    }
    
    Peca pecaRemovida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAMANHO_FILA;
    fila->quantidade--;
    
    return pecaRemovida;
}

// Função para visualizar a fila atual
void visualizarFila(FilaCircular *fila) {
    printf("\n=== FILA DE PEÇAS FUTURAS ===\n");
    
    if (filaVazia(fila)) {
        printf("Fila vazia!\n");
        return;
    }
    
    int indice = fila->frente;
    
    for (int i = 0; i < fila->quantidade; i++) {
        printf("Posição %d: Peça '%c' (ID: %d)\n", 
               i + 1, 
               fila->pecas[indice].tipo, 
               fila->pecas[indice].id);
        indice = (indice + 1) % TAMANHO_FILA;
    }
    
    printf("Total de peças na fila: %d\n", fila->quantidade);
    printf("==============================\n");
}

// Função para mostrar o menu
void mostrarMenu() {
    printf("\n=== TETRIS - NÍVEL NOVATO ===\n");
    printf("1 - Visualizar fila de peças\n");
    printf("2 - Jogar peça (remover da frente)\n");
    printf("3 - Inserir nova peça automaticamente\n");
    printf("4 - Sair do jogo\n");
    printf("Escolha uma opção: ");
}

int main() {
    FilaCircular fila;
    int opcao;
    
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
    
    // Inicializa a fila com 5 peças
    inicializarFila(&fila);
    
    printf("Bem-vindo ao Tetris - Nível Novato!\n");
    printf("Fila circular inicializada com 5 peças.\n");
    
    do {
        mostrarMenu();
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                // Visualizar fila
                visualizarFila(&fila);
                break;
                
            case 2:
                // Jogar peça (remover da frente)
                if (!filaVazia(&fila)) {
                    Peca pecaJogada = desenfileirar(&fila);
                    printf("\n🎮 Peça jogada: '%c' (ID: %d)\n", 
                           pecaJogada.tipo, pecaJogada.id);
                    
                    // Insere automaticamente uma nova peça no final
                    Peca novaPeca = gerarPeca();
                    enfileirar(&fila, novaPeca);
                    printf("➕ Nova peça adicionada: '%c' (ID: %d)\n",
                           novaPeca.tipo, novaPeca.id);
                    
                    visualizarFila(&fila);
                } else {
                    printf("❌ Não há peças para jogar!\n");
                }
                break;
                
            case 3:
                // Inserir nova peça automaticamente
                if (!filaCheia(&fila)) {
                    Peca novaPeca = gerarPeca();
                    enfileirar(&fila, novaPeca);
                    printf("\n➕ Nova peça inserida: '%c' (ID: %d)\n",
                           novaPeca.tipo, novaPeca.id);
                    visualizarFila(&fila);
                } else {
                    printf("❌ Fila cheia! Não é possível adicionar mais peças.\n");
                }
                break;
                
            case 4:
                printf("\n👋 Obrigado por jogar Tetris!\n");
                break;
                
            default:
                printf("\n❌ Opção inválida! Tente novamente.\n");
                break;
        }
        
    } while (opcao != 4);
    
    return 0;
}