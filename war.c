// =// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings, tempo e suporte a locale
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

/* =====================================================
   ESTRUTURA DE DADOS
   ===================================================== */
// Estrutura de território: nome, cor do exército e quantidade de tropas
typedef struct {
    char nome[30];
    char cor[30];
    int tropas;
} Territorio;

/* =====================================================
   PROTÓTIPOS DAS FUNÇÕES
   ===================================================== */
void limparBufferEntrada(void);
void cadastrarTerritorios(Territorio* mapa, int n);
void exibirMapa(const Territorio* mapa, int n);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa, char** missoesJogadores, int qntJogadores);
void exibirMissao(const char* missao);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho);

/* =====================================================
   ETAPA 1 - FUNÇÕES BÁSICAS DE ENTRADA E EXIBIÇÃO
   ===================================================== */

// Limpa o buffer de entrada para evitar conflitos entre scanf e fgets
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Cadastro de territórios (nome, cor e tropas)
void cadastrarTerritorios(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\nCadastro do território %d:\n", i + 1);

        printf("Digite o nome do território: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Digite a cor do exército: ");
        fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBufferEntrada();
    }
}

// Exibição dos territórios cadastrados
void exibirMapa(const Territorio* mapa, int n) {
    printf("\n=== Mapa de Territórios ===\n");
    for (int i = 0; i < n; i++) {
        printf("\nTerritório %d:\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor do exército: %s\n", mapa[i].cor);
        printf("Quantidade de tropas: %d\n", mapa[i].tropas);
    }
}

/* =====================================================
   ETAPA 2 - FUNÇÃO DE ATAQUE ENTRE TERRITÓRIOS
   ===================================================== */

// Simula um ataque usando dados (valores aleatórios de 1 a 6)
void atacar(Territorio* atacante, Territorio* defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\n[AVISO] Não é possível atacar um território da mesma cor!\n");
        return;
    }
    if (atacante->tropas <= 0) {
        printf("\n[AVISO] O território atacante não possui tropas suficientes!\n");
        return;
    }

    printf("\n%s (%s) está atacando %s (%s)!\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("Dado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor: %d\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf(">>> O atacante venceu!\n");
        strcpy(defensor->cor, atacante->cor);     // defensor muda de dono
        defensor->tropas = atacante->tropas / 2; // defensor recebe metade
        atacante->tropas = atacante->tropas / 2; // atacante perde metade
    } else {
        printf(">>> O defensor resistiu ao ataque!\n");
        atacante->tropas -= 1; // atacante perde uma tropa
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

/* =====================================================
   ETAPA 3 - MISSÕES ESTRATÉGICAS
   ===================================================== */

// Exibe a missão do jogador (somente no início)
void exibirMissao(const char* missao) {
    printf("\nSua missão: %s\n", missao);
}

// Sorteia uma missão aleatória
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int idx = rand() % totalMissoes;
    strcpy(destino, missoes[idx]);
}

// Verifica se a missão foi cumprida
int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    if (strstr(missao, "3 territorios seguidos") != NULL) {
        for (int i = 0; i + 2 < tamanho; i++) {
            if (strcmp(mapa[i].cor, mapa[i+1].cor) == 0 &&
                strcmp(mapa[i].cor, mapa[i+2].cor) == 0) {
                return 1;
            }
        }
        return 0;
    }
    if (strstr(missao, "cor vermelha") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (strcasecmp(mapa[i].cor, "vermelha") == 0 && mapa[i].tropas > 0) return 0;
        }
        return 1;
    }
    if (strstr(missao, "metade ou mais do mapa") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            int cont = 0;
            for (int j = 0; j < tamanho; j++) {
                if (strcasecmp(mapa[i].cor, mapa[j].cor) == 0) cont++;
            }
            if (cont * 2 >= tamanho) return 1;
        }
        return 0;
    }
    if (strstr(missao, "10 ou mais tropas") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].tropas >= 10) return 1;
        }
        return 0;
    }
    if (strstr(missao, "0 tropas") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].tropas == 0) return 0;
        }
        return 1;
    }
    return 0;
}

/* =====================================================
   LIBERAÇÃO DE MEMÓRIA
   ===================================================== */
void liberarMemoria(Territorio* mapa, char** missoesJogadores, int qntJogadores) {
    if (mapa) free(mapa);
    if (missoesJogadores) {
        for (int i = 0; i < qntJogadores; i++) {
            free(missoesJogadores[i]);
        }
        free(missoesJogadores);
    }
}

/* =====================================================
   FUNÇÃO PRINCIPAL (MAIN)
   ===================================================== */
int main(void) {
    setlocale(LC_ALL, "Portuguese"); // garante exibição correta de acentos

    srand((unsigned)time(NULL)); // garante aleatoriedade

    // --- Etapa 1: Cadastro inicial ---
    int n;
    printf("Digite o número de territórios: ");
    scanf("%d", &n);
    limparBufferEntrada();

    Territorio* mapa = (Territorio*)calloc(n, sizeof(Territorio));
    if (!mapa) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }
    cadastrarTerritorios(mapa, n);
    exibirMapa(mapa, n);

    // --- Etapa 3: Missões ---
    char* missoesDisponiveis[] = {
        "Conquistar 3 territorios seguidos (mesma cor).",
        "Eliminar todas as tropas da cor vermelha.",
        "Controlar metade ou mais do mapa por uma unica cor.",
        "Existir um territorio com 10 ou mais tropas.",
        "Nao existir nenhum territorio com 0 tropas."
    };
    int totalMissoes = sizeof(missoesDisponiveis) / sizeof(missoesDisponiveis[0]);

    int jogadores = 2; // exemplo fixo
    char** missoesJogadores = (char**)malloc(jogadores * sizeof(char*));
    const size_t TAM_MISSAO = 200;
    for (int i = 0; i < jogadores; i++) {
        missoesJogadores[i] = (char*)malloc(TAM_MISSAO);
        atribuirMissao(missoesJogadores[i], missoesDisponiveis, totalMissoes);
        printf("\n=== Jogador %d ===", i + 1);
        exibirMissao(missoesJogadores[i]); // exibida apenas no início
    }

    // --- Etapa 2: Loop de ataque + checagem de missão ---
    while (1) {
        int idxAtacante, idxDefensor;

        printf("\nEscolha o território atacante (1 a %d): ", n);
        scanf("%d", &idxAtacante);
        printf("Escolha o território defensor (1 a %d): ", n);
        scanf("%d", &idxDefensor);
        limparBufferEntrada();

        if (idxAtacante < 1 || idxAtacante > n || idxDefensor < 1 || idxDefensor > n) {
            printf("[AVISO] Índices inválidos!\n");
            continue;
        }

        atacar(&mapa[idxAtacante - 1], &mapa[idxDefensor - 1]);
        exibirMapa(mapa, n);

        int vencedor = 0;
        for (int j = 0; j < jogadores; j++) {
            if (verificarMissao(missoesJogadores[j], mapa, n)) {
                vencedor = j + 1;
                break;
            }
        }
        if (vencedor) {
            printf("\n=================================\n");
            printf(">>> Jogador %d CUMPRIU a missão!\n", vencedor);
            printf("Missão: %s\n", missoesJogadores[vencedor - 1]);
            printf("=========== FIM DE JOGO =========\n");
            break;
        }

        char opc;
        printf("\nDeseja realizar outro ataque? (s/n): ");
        scanf(" %c", &opc);
        limparBufferEntrada();
        if (opc != 's' && opc != 'S') {
            printf("\nEncerrando a partida.\n");
            break;
        }
    }

    liberarMemoria(mapa, missoesJogadores, jogadores);
    return 0;
}
