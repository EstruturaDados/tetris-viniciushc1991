#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3

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

// Estrutura para a pilha de reserva
typedef struct {
    Peca pecas[TAMANHO_PILHA];
    int topo;
    int quantidade;
} PilhaReserva;

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

// Função para inicializar a pilha de reserva
void inicializarPilha(PilhaReserva *pilha) {
    pilha->topo = -1;
    pilha->quantidade = 0;
}

// Função para verificar se a fila está vazia
int filaVazia(FilaCircular *fila) {
    return fila->quantidade == 0;
}

// Função para verificar se a fila está cheia
int filaCheia(FilaCircular *fila) {
    return fila->quantidade == TAMANHO_FILA;
}

// Função para verificar se a pilha está vazia
int pilhaVazia(PilhaReserva *pilha) {
    return pilha->quantidade == 0;
}

// Função para verificar se a pilha está cheia
int pilhaCheia(PilhaReserva *pilha) {
    return pilha->quantidade == TAMANHO_PILHA;
}

// Função para enfileirar (inserir) uma peça na fila
void enfileirar(FilaCircular *fila, Peca peca) {
    if (filaCheia(fila)) {
        printf("Erro: Fila cheia!\n");
        return;
    }
    
    fila->tras = (fila->tras + 1) % TAMANHO_FILA;
    fila->pecas[fila->tras] = peca;
    fila->quantidade++;
}

// Função para desenfileirar (remover) uma peça da fila
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

// Função para empilhar (reservar) uma peça
void empilhar(PilhaReserva *pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        printf("Erro: Pilha de reserva cheia!\n");
        return;
    }
    
    pilha->topo++;
    pilha->pecas[pilha->topo] = peca;
    pilha->quantidade++;
}

// Função para desempilhar (usar) uma peça reservada
Peca desempilhar(PilhaReserva *pilha) {
    Peca pecaVazia = {'?', -1}; // Peça vazia para erro
    
    if (pilhaVazia(pilha)) {
        printf("Erro: Pilha de reserva vazia!\n");
        return pecaVazia;
    }
    
    Peca pecaRemovida = pilha->pecas[pilha->topo];
    pilha->topo--;
    pilha->quantidade--;
    
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

// Função para visualizar a pilha de reserva
void visualizarPilha(PilhaReserva *pilha) {
    printf("\n=== PILHA DE RESERVA ===\n");
    
    if (pilhaVazia(pilha)) {
        printf("Pilha de reserva vazia!\n");
    } else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("Reserva %d: Peça '%c' (ID: %d)\n", 
                   pilha->topo - i + 1, 
                   pilha->pecas[i].tipo, 
                   pilha->pecas[i].id);
        }
    }
    
    printf("Total de peças reservadas: %d/%d\n", pilha->quantidade, TAMANHO_PILHA);
    printf("=========================\n");
}

// Função para mostrar o menu
void mostrarMenu() {
    printf("\n=== TETRIS - NÍVEL AVENTUREIRO ===\n");
    printf("1 - Jogar peça (da frente da fila)\n");
    printf("2 - Reservar peça (da frente da fila)\n");
    printf("3 - Usar peça reservada (do topo da pilha)\n");
    printf("0 - Sair do jogo\n");
    printf("Escolha uma opção: ");
}

int main() {
    FilaCircular fila;
    PilhaReserva pilha;
    int opcao;
    
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
    
    // Inicializa a fila com 5 peças e a pilha vazia
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    
    printf("Bem-vindo ao Tetris - Nível Aventureiro!\n");
    printf("Fila circular inicializada com 5 peças.\n");
    printf("Pilha de reserva com capacidade para 3 peças.\n");
    
    do {
        // Mostra o estado atual da fila e pilha
        visualizarFila(&fila);
        visualizarPilha(&pilha);
        
        mostrarMenu();
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1: {
                // Jogar peça (da frente da fila)
                if (!filaVazia(&fila)) {
                    Peca pecaJogada = desenfileirar(&fila);
                    printf("\n🎮 Peça jogada da FILA: '%c' (ID: %d)\n", 
                           pecaJogada.tipo, pecaJogada.id);
                    
                    // Insere automaticamente uma nova peça no final da fila
                    Peca novaPeca = gerarPeca();
                    enfileirar(&fila, novaPeca);
                    printf("➕ Nova peça adicionada na FILA: '%c' (ID: %d)\n",
                           novaPeca.tipo, novaPeca.id);
                } else {
                    printf("❌ Não há peças na fila para jogar!\n");
                }
                break;
            }
                
            case 2: {
                // Reservar peça (da frente da fila)
                if (!filaVazia(&fila)) {
                    if (!pilhaCheia(&pilha)) {
                        Peca pecaReservada = desenfileirar(&fila);
                        empilhar(&pilha, pecaReservada);
                        printf("\n💾 Peça reservada: '%c' (ID: %d)\n", 
                               pecaReservada.tipo, pecaReservada.id);
                        
                        // Repõe a peça na fila
                        Peca novaPeca = gerarPeca();
                        enfileirar(&fila, novaPeca);
                        printf("➕ Nova peça adicionada na FILA: '%c' (ID: %d)\n",
                               novaPeca.tipo, novaPeca.id);
                    } else {
                        printf("❌ Pilha de reserva cheia! Não é possível reservar mais peças.\n");
                    }
                } else {
                    printf("❌ Não há peças na fila para reservar!\n");
                }
                break;
            }
                
            case 3: {
                // Usar peça reservada (do topo da pilha)
                if (!pilhaVazia(&pilha)) {
                    Peca pecaUsada = desempilhar(&pilha);
                    printf("\n🎮 Peça usada da RESERVA: '%c' (ID: %d)\n", 
                           pecaUsada.tipo, pecaUsada.id);
                    
                    // A fila mantém suas 5 peças (não há reposição automática aqui)
                    printf("ℹ️  A fila permanece com 5 peças.\n");
                } else {
                    printf("❌ Não há peças na reserva para usar!\n");
                }
                break;
            }
                
            case 0:
                printf("\n👋 Obrigado por jogar Tetris!\n");
                break;
                
            default:
                printf("\n❌ Opção inválida! Tente novamente.\n");
                break;
        }
        
    } while (opcao != 0);
    
    return 0;
}