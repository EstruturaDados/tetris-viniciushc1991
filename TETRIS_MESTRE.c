#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3
#define HISTORICO_MAX 10

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

// Estrutura para o histórico de ações
typedef struct {
    char acao[50];
    Peca pecaFilaFrente;
    Peca pecaPilhaTopo;
    int timestamp;
} AcaoHistorico;

// Estrutura para o histórico do jogo
typedef struct {
    AcaoHistorico historico[HISTORICO_MAX];
    int topo;
    int quantidade;
} HistoricoJogo;

// Função para gerar uma peça aleatória
Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    
    novaPeca.id = rand() % 1000 + 1;
    novaPeca.tipo = tipos[rand() % 4];
    
    return novaPeca;
}

// Função para inicializar a fila circular
void inicializarFila(FilaCircular *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->quantidade = 0;
    
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

// Função para inicializar o histórico
void inicializarHistorico(HistoricoJogo *historico) {
    historico->topo = -1;
    historico->quantidade = 0;
}

// Funções de verificação de estado
int filaVazia(FilaCircular *fila) {
    return fila->quantidade == 0;
}

int filaCheia(FilaCircular *fila) {
    return fila->quantidade == TAMANHO_FILA;
}

int pilhaVazia(PilhaReserva *pilha) {
    return pilha->quantidade == 0;
}

int pilhaCheia(PilhaReserva *pilha) {
    return pilha->quantidade == TAMANHO_PILHA;
}

int historicoVazio(HistoricoJogo *historico) {
    return historico->quantidade == 0;
}

int historicoCheio(HistoricoJogo *historico) {
    return historico->quantidade == HISTORICO_MAX;
}

// Funções da fila circular
void enfileirar(FilaCircular *fila, Peca peca) {
    if (filaCheia(fila)) return;
    
    fila->tras = (fila->tras + 1) % TAMANHO_FILA;
    fila->pecas[fila->tras] = peca;
    fila->quantidade++;
}

Peca desenfileirar(FilaCircular *fila) {
    Peca pecaVazia = {'?', -1};
    if (filaVazia(fila)) return pecaVazia;
    
    Peca pecaRemovida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAMANHO_FILA;
    fila->quantidade--;
    
    return pecaRemovida;
}

Peca verFrenteFila(FilaCircular *fila) {
    Peca pecaVazia = {'?', -1};
    if (filaVazia(fila)) return pecaVazia;
    return fila->pecas[fila->frente];
}

// Funções da pilha de reserva
void empilhar(PilhaReserva *pilha, Peca peca) {
    if (pilhaCheia(pilha)) return;
    
    pilha->topo++;
    pilha->pecas[pilha->topo] = peca;
    pilha->quantidade++;
}

Peca desempilhar(PilhaReserva *pilha) {
    Peca pecaVazia = {'?', -1};
    if (pilhaVazia(pilha)) return pecaVazia;
    
    Peca pecaRemovida = pilha->pecas[pilha->topo];
    pilha->topo--;
    pilha->quantidade--;
    
    return pecaRemovida;
}

Peca verTopoPilha(PilhaReserva *pilha) {
    Peca pecaVazia = {'?', -1};
    if (pilhaVazia(pilha)) return pecaVazia;
    return pilha->pecas[pilha->topo];
}

// Funções do histórico
void adicionarHistorico(HistoricoJogo *historico, const char *acao, Peca filaFrente, Peca pilhaTopo) {
    if (historicoCheio(historico)) {
        // Remove a ação mais antiga (implementação simplificada)
        for (int i = 0; i < HISTORICO_MAX - 1; i++) {
            historico->historico[i] = historico->historico[i + 1];
        }
        historico->quantidade--;
    }
    
    historico->topo = (historico->topo + 1) % HISTORICO_MAX;
    strcpy(historico->historico[historico->topo].acao, acao);
    historico->historico[historico->topo].pecaFilaFrente = filaFrente;
    historico->historico[historico->topo].pecaPilhaTopo = pilhaTopo;
    historico->historico[historico->topo].timestamp = rand() % 10000;
    historico->quantidade++;
}

AcaoHistorico removerHistorico(HistoricoJogo *historico) {
    AcaoHistorico vazia = {"", {'?', -1}, {'?', -1}, 0};
    if (historicoVazio(historico)) return vazia;
    
    AcaoHistorico acao = historico->historico[historico->topo];
    historico->topo--;
    historico->quantidade--;
    
    return acao;
}

// Funções das operações avançadas
void trocarPecaFilaPilha(FilaCircular *fila, PilhaReserva *pilha) {
    if (filaVazia(fila) || pilhaVazia(pilha)) {
        printf("❌ Não é possível trocar: fila ou pilha vazia!\n");
        return;
    }
    
    Peca pecaFila = desenfileirar(fila);
    Peca pecaPilha = desempilhar(pilha);
    
    enfileirar(fila, pecaPilha);
    empilhar(pilha, pecaFila);
    
    printf("🔄 Troca realizada: Fila('%c'↔'%c')Pilha\n", pecaFila.tipo, pecaPilha.tipo);
}

void inverterFilaComPilha(FilaCircular *fila, PilhaReserva *pilha) {
    if (filaVazia(fila) && pilhaVazia(pilha)) {
        printf("❌ Não é possível inverter: ambas estruturas vazias!\n");
        return;
    }
    
    // Salva estado atual para possível desfazer
    PilhaReserva tempPilha;
    FilaCircular tempFila;
    inicializarPilha(&tempPilha);
    inicializarFila(&tempFila);
    
    // Transfere fila para pilha temporária
    while (!filaVazia(fila)) {
        empilhar(&tempPilha, desenfileirar(fila));
    }
    
    // Transfere pilha principal para fila temporária
    while (!pilhaVazia(pilha)) {
        enfileirar(&tempFila, desempilhar(pilha));
    }
    
    // Transfere pilha temporária para pilha principal (invertida da fila original)
    while (!pilhaVazia(&tempPilha)) {
        empilhar(pilha, desempilhar(&tempPilha));
    }
    
    // Transfere fila temporária para fila principal (invertida da pilha original)
    while (!filaVazia(&tempFila)) {
        enfileirar(fila, desenfileirar(&tempFila));
    }
    
    printf("🔄 Inversão completa: Fila↔Pilha\n");
}

// Funções de visualização
void visualizarFila(FilaCircular *fila) {
    printf("\n=== FILA DE PEÇAS FUTURAS ===\n");
    
    if (filaVazia(fila)) {
        printf("Fila vazia!\n");
        return;
    }
    
    int indice = fila->frente;
    for (int i = 0; i < fila->quantidade; i++) {
        printf("Posição %d: Peça '%c' (ID: %d)\n", 
               i + 1, fila->pecas[indice].tipo, fila->pecas[indice].id);
        indice = (indice + 1) % TAMANHO_FILA;
    }
    printf("Total: %d/%d peças\n", fila->quantidade, TAMANHO_FILA);
}

void visualizarPilha(PilhaReserva *pilha) {
    printf("\n=== PILHA DE RESERVA ===\n");
    
    if (pilhaVazia(pilha)) {
        printf("Pilha vazia!\n");
    } else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("Reserva %d: Peça '%c' (ID: %d)\n", 
                   pilha->topo - i + 1, pilha->pecas[i].tipo, pilha->pecas[i].id);
        }
    }
    printf("Total: %d/%d peças\n", pilha->quantidade, TAMANHO_PILHA);
}

void visualizarHistorico(HistoricoJogo *historico) {
    printf("\n=== HISTÓRICO (últimas %d ações) ===\n", historico->quantidade);
    
    if (historicoVazio(historico)) {
        printf("Nenhuma ação no histórico.\n");
        return;
    }
    
    for (int i = historico->topo; i >= 0; i--) {
        printf("#%d: %s\n", historico->topo - i + 1, historico->historico[i].acao);
    }
}

// Função para mostrar o menu
void mostrarMenu() {
    printf("\n=== TETRIS - NÍVEL MESTRE ===\n");
    printf("1 - Jogar peça (da frente da fila)\n");
    printf("2 - Reservar peça (da frente da fila)\n");
    printf("3 - Usar peça reservada (do topo da pilha)\n");
    printf("4 - Trocar peça (topo da pilha ↔ frente da fila)\n");
    printf("5 - Desfazer última jogada\n");
    printf("6 - Inverter fila com pilha\n");
    printf("7 - Visualizar histórico\n");
    printf("0 - Sair do jogo\n");
    printf("Escolha uma opção: ");
}

int main() {
    FilaCircular fila;
    PilhaReserva pilha;
    HistoricoJogo historico;
    int opcao;
    
    srand(time(NULL));
    
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    inicializarHistorico(&historico);
    
    printf("Bem-vindo ao Tetris - Nível Mestre!\n");
    printf("Sistema avançado com trocas, desfazer e inversão.\n");
    
    do {
        visualizarFila(&fila);
        visualizarPilha(&pilha);
        
        mostrarMenu();
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1: {
                // Jogar peça da fila
                if (!filaVazia(&fila)) {
                    Peca pecaJogada = desenfileirar(&fila);
                    printf("\n🎮 Peça jogada: '%c' (ID: %d)\n", pecaJogada.tipo, pecaJogada.id);
                    
                    // Registra no histórico
                    adicionarHistorico(&historico, "Jogou peça da fila", verFrenteFila(&fila), verTopoPilha(&pilha));
                    
                    // Repõe na fila
                    Peca novaPeca = gerarPeca();
                    enfileirar(&fila, novaPeca);
                    printf("➕ Nova peça: '%c' (ID: %d)\n", novaPeca.tipo, novaPeca.id);
                } else {
                    printf("❌ Fila vazia!\n");
                }
                break;
            }
                
            case 2: {
                // Reservar peça da fila
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca pecaReservada = desenfileirar(&fila);
                    empilhar(&pilha, pecaReservada);
                    printf("\n💾 Peça reservada: '%c' (ID: %d)\n", pecaReservada.tipo, pecaReservada.id);
                    
                    adicionarHistorico(&historico, "Reservou peça", verFrenteFila(&fila), verTopoPilha(&pilha));
                    
                    // Repõe na fila
                    Peca novaPeca = gerarPeca();
                    enfileirar(&fila, novaPeca);
                    printf("➕ Nova peça: '%c' (ID: %d)\n", novaPeca.tipo, novaPeca.id);
                } else {
                    printf("❌ Fila vazia ou pilha cheia!\n");
                }
                break;
            }
                
            case 3: {
                // Usar peça da reserva
                if (!pilhaVazia(&pilha)) {
                    Peca pecaUsada = desempilhar(&pilha);
                    printf("\n🎮 Peça usada da reserva: '%c' (ID: %d)\n", pecaUsada.tipo, pecaUsada.id);
                    adicionarHistorico(&historico, "Usou peça da reserva", verFrenteFila(&fila), verTopoPilha(&pilha));
                } else {
                    printf("❌ Pilha vazia!\n");
                }
                break;
            }
                
            case 4: {
                // Trocar peça entre fila e pilha
                Peca frenteAntes = verFrenteFila(&fila);
                Peca topoAntes = verTopoPilha(&pilha);
                trocarPecaFilaPilha(&fila, &pilha);
                adicionarHistorico(&historico, "Trocou peça fila↔pilha", frenteAntes, topoAntes);
                break;
            }
                
            case 5: {
                // Desfazer última ação
                if (!historicoVazio(&historico)) {
                    AcaoHistorico ultimaAcao = removerHistorico(&historico);
                    printf("\n↩️  Desfeito: %s\n", ultimaAcao.acao);
                    printf("   Estado anterior - Fila: '%c', Pilha: '%c'\n", 
                           ultimaAcao.pecaFilaFrente.tipo, ultimaAcao.pecaPilhaTopo.tipo);
                } else {
                    printf("❌ Nada para desfazer!\n");
                }
                break;
            }
                
            case 6: {
                // Inverter fila com pilha
                printf("\n🔄 Invertendo fila com pilha...\n");
                inverterFilaComPilha(&fila, &pilha);
                adicionarHistorico(&historico, "Inverteu fila com pilha", verFrenteFila(&fila), verTopoPilha(&pilha));
                break;
            }
                
            case 7: {
                // Visualizar histórico
                visualizarHistorico(&historico);
                break;
            }
                
            case 0:
                printf("\n👋 Obrigado por jogar Tetris Mestre!\n");
                break;
                
            default:
                printf("\n❌ Opção inválida!\n");
                break;
        }
        
    } while (opcao != 0);
    
    return 0;
}