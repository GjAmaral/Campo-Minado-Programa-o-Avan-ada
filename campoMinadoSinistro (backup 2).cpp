#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FACIL_TAMANHO 8
#define FACIL_MINAS 10
#define MEDIO_TAMANHO 16
#define MEDIO_MINAS 40
#define DIFICIL_TAMANHO 24
#define DIFICIL_MINAS 99

typedef struct {
    int temMina;
    int vizinhas;
    int revelada;
    int marcada;
} Celula;

Celula **tabuleiro;
int tamanho_tabuleiro;
int num_minas;
int pontuacao = 0;
int multiplicador = 1;
int marcacoes = 0;

void inicializarTabuleiro() {
    tabuleiro = (Celula **)malloc(tamanho_tabuleiro * sizeof(Celula *));
    for (int i = 0; i < tamanho_tabuleiro; i++) {
        tabuleiro[i] = (Celula *)malloc(tamanho_tabuleiro * sizeof(Celula));
        for (int j = 0; j < tamanho_tabuleiro; j++) {
            tabuleiro[i][j].temMina = 0;
            tabuleiro[i][j].vizinhas = 0;
            tabuleiro[i][j].revelada = 0;
            tabuleiro[i][j].marcada = 0;
        }
    }
}

void liberarTabuleiro() {
    for (int i = 0; i < tamanho_tabuleiro; i++) {
        free(tabuleiro[i]);
    }
    free(tabuleiro);
}

void colocarMinas() {
    int minas_colocadas = 0;
    while (minas_colocadas < num_minas) {
        int linha = rand() % tamanho_tabuleiro;
        int coluna = rand() % tamanho_tabuleiro;
        if (!tabuleiro[linha][coluna].temMina) {
            tabuleiro[linha][coluna].temMina = 1;
            minas_colocadas++;
        }
    }
}

void contarMinasVizinhas() {
    for (int i = 0; i < tamanho_tabuleiro; i++) {
        for (int j = 0; j < tamanho_tabuleiro; j++) {
            if (!tabuleiro[i][j].temMina) {
                int vizinhas = 0;
                for (int x = -1; x <= 1; x++) {
                    for (int y = -1; y <= 1; y++) {
                        int ni = i + x;
                        int nj = j + y;
                        if (ni >= 0 && ni < tamanho_tabuleiro && nj >= 0 && nj < tamanho_tabuleiro) {
                            if (tabuleiro[ni][nj].temMina) {
                                vizinhas++;
                            }
                        }
                    }
                }
                tabuleiro[i][j].vizinhas = vizinhas;
            }
        }
    }
}

void mostrarTabuleiro() {
    printf("   ");
    for (int j = 0; j < tamanho_tabuleiro; j++) {
        printf("%2d ", j + 1);
    }
    printf("\n");

    for (int i = 0; i < tamanho_tabuleiro; i++) {
        printf("%2d ", i + 1);
        for (int j = 0; j < tamanho_tabuleiro; j++) {
            if (tabuleiro[i][j].revelada) {
                if (tabuleiro[i][j].temMina) {
                    printf(" * ");
                } else {
                    printf(" %d ", tabuleiro[i][j].vizinhas);
                }
            } else if (tabuleiro[i][j].marcada) {
                printf(" M ");
            } else {
                printf(" . ");
            }
        }
        printf("\n");
    }
}

void mostrarTabuleiroCompleto() {
    printf("   ");
    for (int j = 0; j < tamanho_tabuleiro; j++) {
        printf("%2d ", j + 1);
    }
    printf("\n");

    for (int i = 0; i < tamanho_tabuleiro; i++) {
        printf("%2d ", i + 1);
        for (int j = 0; j < tamanho_tabuleiro; j++) {
            if (tabuleiro[i][j].temMina) {
                printf(" * ");
            } else {
                printf(" %d ", tabuleiro[i][j].vizinhas);
            }
        }
        printf("\n");
    }
}

void revelarCelula(int linha, int coluna) {
    if (linha < 0 || linha >= tamanho_tabuleiro || coluna < 0 || coluna >= tamanho_tabuleiro || tabuleiro[linha][coluna].revelada) {
        return;
    }

    tabuleiro[linha][coluna].revelada = 1;

    if (tabuleiro[linha][coluna].temMina) {
        printf("BOOOOOOOOMM !! Voce encontrou uma mina ! Fim de jogo, voce perdeu !! \n");
        printf("\nTabuleiro completo:\n");
        mostrarTabuleiroCompleto();
        printf("Sua pontuacao: %d\n", pontuacao);
        printf("Marcacoes: %d\n", marcacoes);
        char resposta;
        printf("\nDeseja jogar novamente? (S/N): ");
        scanf(" %c", &resposta);
        if (resposta == 'S' || resposta == 's') {
            inicializarTabuleiro();
            colocarMinas();
            contarMinasVizinhas();
            pontuacao = 0;
            marcacoes = 0;
            return;
        } else {
            printf("\nSaindo do jogo...\n");
            liberarTabuleiro();
            exit(0);
        }
    } else {
        pontuacao += 10 * multiplicador;
    }

    if (tabuleiro[linha][coluna].vizinhas == 0) {
        for (int i = linha - 1; i <= linha + 1; i++) {
            for (int j = coluna - 1; j <= coluna + 1; j++) {
                revelarCelula(i, j);
            }
        }
    }
}

void marcarCelula(int linha, int coluna) {
    if (linha < 0 || linha >= tamanho_tabuleiro || coluna < 0 || coluna >= tamanho_tabuleiro) {
        return;
    }
    if (!tabuleiro[linha][coluna].marcada) {
        tabuleiro[linha][coluna].marcada = 1;
        marcacoes++;
    } else {
        tabuleiro[linha][coluna].marcada = 0;
        marcacoes--;
        if (!tabuleiro[linha][coluna].temMina) {
            pontuacao -= 5 * multiplicador;
        }
    }
}

void verificarVitoria() {
    int todasReveladas = 1;
    for (int i = 0; i < tamanho_tabuleiro; i++) {
        for (int j = 0; j < tamanho_tabuleiro; j++) {
            if (!tabuleiro[i][j].temMina && !tabuleiro[i][j].revelada) {
                todasReveladas = 0;
                break;
            }
        }
        if (!todasReveladas) {
            break;
        }
    }
    if (todasReveladas) {
        pontuacao += 100 * multiplicador;
        printf("Parabens !!! Você venceu!\n");
        printf("Sua pontuacao: %d\n", pontuacao);
        printf("Marcacoes: %d\n", marcacoes);
        char resposta;
        printf("Deseja jogar novamente? (S/N): ");
        scanf(" %c", &resposta);
        if (resposta == 'S' || resposta == 's') {
            inicializarTabuleiro();
            colocarMinas();
            contarMinasVizinhas();
            pontuacao = 0;
            marcacoes = 0;
            return;
        } else {
            printf("Saindo do jogo...\n");
            liberarTabuleiro();
            exit(0);
        }
    }
}

void escolherDificuldade() {
    int escolhaDificuldade;
    printf("\nEscolha a dificuldade:\n");
    printf("1. Facil\n");
    printf("2. Medio\n");
    printf("3. Dificil\n");
    printf("4. Personalizado\n");
    scanf("%d", &escolhaDificuldade);

    switch (escolhaDificuldade) {
        case 1:
            tamanho_tabuleiro = FACIL_TAMANHO;
            num_minas = FACIL_MINAS;
            multiplicador = 1;
            break;
        case 2:
            tamanho_tabuleiro = MEDIO_TAMANHO;
            num_minas = MEDIO_MINAS;
            multiplicador = 2;
            break;
        case 3:
            tamanho_tabuleiro = DIFICIL_TAMANHO;
            num_minas = DIFICIL_MINAS;
            multiplicador = 3;
            break;
        case 4:
            printf("Digite o tamanho do tabuleiro: ");
            scanf("%d", &tamanho_tabuleiro);
            printf("Digite o numero de minas: ");
            scanf("%d", &num_minas);
            printf("Digite o multiplicador de dificuldade: ");
            scanf("%d", &multiplicador);
            break;
        default:
            printf("Opcao invalida! Escolhendo dificuldade facil por padrao.\n");
            tamanho_tabuleiro = FACIL_TAMANHO;
            num_minas = FACIL_MINAS;
            multiplicador = 1;
    }
}

void msgBemVindo() {
    system("cls");
    printf("-------------------------------------------------------------------\n");
    printf("\n          *** BEM VINDO AO CAMPO MINADO ***\n\n");
    printf("-------------------------------------------------------------------\n");
}

void mostrarMenu() {
    printf("\nEscolha uma opcao:\n");
    printf("1. Jogar\n");
    printf("2. Como jogar\n");
    printf("3. Creditos\n");
    printf("4. Sair\n");
}

void mostarRegras() {
	system("cls");
    printf("-------------------------------------------------------------------\n");
    printf("\n*** COMO JOGAR ***\n\n");
    printf("Bem-vindo ao Campo Minado, um jogo de estratEgia e habilidade!\n");
    printf("O objetivo do jogo eh limpar o campo sem detonar nenhuma mina.\n");
	printf("Cada célula do campo pode conter uma mina ou um número.\n");
    printf("O número indica quantas minas estão adjacentes a essa celula.\n");
    printf("Use as informações dos números para evitar as minas.\n");
    printf("Se você clicar em uma celula com uma mina, o jogo acaba!\n");
    printf("Se conseguir limpar todas as celulas sem minas, você vence!\n");
    printf("Boa sorte e divirta-se!\n");
    printf("-------------------------------------------------------------------\n");
}

void mostrarAlunos() {
	 system("cls");
    printf("-------------------------------------------------------------------\n");
    printf("\n          *** CREDITOS ***\n\n");
    printf("Alunos: \n");
    printf("- JULLIAN SCHMIDT            |- RM: 2022100901\n");
    printf("- JOAO GUILHERME AMARAL      |- RM: \n");
    printf("- JOAO PEDRO LUBANSKI        |- RM: \n");
    printf("-------------------------------------------------------------------\n");
}

int main() {
    srand(time(NULL));
    int escolha;

    do {
        msgBemVindo();
        mostrarMenu();
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                escolherDificuldade();
                inicializarTabuleiro();
                colocarMinas();
                contarMinasVizinhas();
                pontuacao = 0;
                marcacoes = 0;

                while (1) {
                    mostrarTabuleiro();
                    printf("Pontuacao: %d\n", pontuacao);
                    printf("Marcacoes: %d\n", marcacoes);
                    printf("Escolha sua acao (R para Revelar, M para Marcar): ");
                    char acao;
                    int linha, coluna;
                    scanf(" %c", &acao);
                    printf("Digite a linha e a coluna: ");
                    scanf("%d %d", &linha, &coluna);
                    linha--;
                    coluna--;

                    if (acao == 'R' || acao == 'r') {
                        revelarCelula(linha, coluna);
                    } else if (acao == 'M' || acao == 'm') { 
                        marcarCelula(linha, coluna);
                    } else {
                        printf("Acao invalida! Por favor, escolha uma acao valida (R para Revelar, M para Marcar).\n");
                    }

                    verificarVitoria(); // Verifica se o jogador venceu a cada jogada
                }
                liberarTabuleiro();
                break;


            case 2:
                mostarRegras();
                break;

            case 3:
                mostrarAlunos();
                break;

            case 4:
                system("cls");
                printf("\nSaindo do jogo...\n");
                break;

            default:
                system("cls");
                printf("\nOpcao invalida! Por favor, escolha uma opcao valida.\n");
        }
    } while (escolha != 4);

    return 0;
}

