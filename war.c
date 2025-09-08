// ============================================================================

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

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct{
    char nome[30];
    char cor[10];
    int tropas;
}Territorio;
// --- Protótipos das Funções ---
void limparBufferEntrada();
void cadastrarTerritorios(Territorio* mapa, int n);
void exibirMapa(const Territorio* mapa, int n);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa, char** missoesJogadores, int qntJogadores);
void exibirMissao(const char* missao);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho);

// --- ETAPA 1 - FUNÇÕES BÁSICAS DE ENTRADA E EXIBIÇÃO ---
// Limpa o buffer de entrada para evitar conflitos entre scanf e fgets
void limparBufferEntrada(){;
    int c;
    while ((c = getchar()) != '\n' && c != EOF);{}
}

// Cadastro de territórios (nome, cor e tropas)
void cadastrarTerritorios(Territorio* mapa, int n){
    for (int i = 0; i < n; i++){
        printf("\nCadastro do território %d:\n", i+1);

        printf("Digite o nome do território: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Digite a cor do exército: ");
        fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Digite a quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);

        //Limpar BufferEntrada
        limparBufferEntrada();
    }
}

// Exibição dos territórios cadastrados
void exibirMapa(const Territorio* mapa, int n){
    printf("\n=== Mapa de Territórios ===\n");
    for( int i = 0; i < n; i++){
        printf("\nTerritório %d:\n", i + 1);
        printf("Nome: %s \n", mapa[i].nome);
        Printf("Cor do exército: %s \n", mapa[i].cor);
        printf("Quantidade de tropas: %d \n", mapa[i].tropas);
    }
}

// ETAPA 2 - FUNÇÃO DE ATAQUE ENTRE TERRITÓRIOS
// Simula um ataque usando dados(valores aleatórios de 1 a 6)
void atacar(Territorio* atacante, Territorio* defensor){
    if(strcmp(atacante->cor, defensor->cor) == 0){
        printf("\n[AVISO] Não é possível atacar um território da mesma cor!\n");
        return;
    }
    if (atacante->tropas <= 0){
        printf("[AVISO] O território atacante não possui tropas suficientes!\n");
        return;
    }

    printf("\n%s(%s) está atacando %s(%s)!\n",
    atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("Dado do atacante: %d \n", dadoAtacante);
    printf("Dado do defensor: %d \n", dadoDefensor);

    if (dadoAtacante > dadoDefensor){
        printf(">>> O atacante venceu!\n");
        strcpy(defensor->cor, atacante->cor); // Defensor muda de dono
        defensor->tropas = atacante->tropas / 2; // Defensor recebe metade
        atacante->tropas = atacante->tropas / 2; // Atacante perde metade
    } else{
        printf(">>> O defensor resistiu ao ataque!\n");
        atacante->tropas -= 1; // atacante perde uma tropa
        if (atacante->tropas < 0) atacante ->tropas = 0;
    }
}

// ETAPA 3 - MISSÕES ESTRATÉGICAS
// Exibe a missão do jogador (somente no início)
void exibirMissao(const char* missao){
    printf("\nSua missão: %s\n", missao);
}

//Sorteia uma missão aleatória
void atribuirMissao(char* destino, char* missoes[], int totalMissoes){
    int idx = rand() % totalMissoes;
    strcpy(destino, missoes[idx]);
}

// Verifica se a missão foi cumprida
int verificarMissao(char* missao, Territorio* mapa, int tamanho){
    if(strst(missao, "3 território seguidos") != NULL){
        for(int i = 0; i + 2 < tamanho; i++){
            if(strcmp(mapa[i].cor, mapa[i+1].cor) == 0 &&
            strcmp(mapa[i].cor,mapa[i+2].cor) == 0){
                return 1;
        }
    }
    return 0;
}

