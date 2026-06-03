#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO_DADOS "pets.dat"

typedef struct {
    int id;
    char tipo[100];
    char nome[100];
    char especie[100];
    int idade;
    float peso;
} Animal;

Animal *animais = NULL;
int quantidadeAnimais = 0;
int proximoID = 1;

/* ---------- Funções utilitárias ---------- */

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void lerTexto(const char *mensagem, char *destino, int tamanho) {
    do {
        printf("%s", mensagem);
        if (fgets(destino, tamanho, stdin) != NULL) {
            destino[strcspn(destino, "\n")] = '\0';
        }

        if (strlen(destino) == 0) {
            printf("Entrada inválida. O campo não pode ficar vazio.\n");
        }
    } while (strlen(destino) == 0);
}

int lerInteiro(const char *mensagem) {
    int valor;
    while (1) {
        printf("%s", mensagem);
        if (scanf("%d", &valor) == 1) {
            limparBuffer();
            return valor;
        } else {
            printf("Entrada inválida. Digite um número inteiro.\n");
            limparBuffer();
        }
    }
}

float lerFloat(const char *mensagem) {
    float valor;
    while (1) {
        printf("%s", mensagem);
        if (scanf("%f", &valor) == 1) {
            limparBuffer();
            return valor;
        } else {
            printf("Entrada inválida. Digite um número válido.\n");
            limparBuffer();
        }
    }
}

int lerInteiroPositivo(const char *mensagem) {
    int valor;
    do {
        valor = lerInteiro(mensagem);
        if (valor < 0) {
            printf("Valor inválido. Digite um número maior ou igual a 0.\n");
        }
    } while (valor < 0);

    return valor;
}

float lerFloatPositivo(const char *mensagem) {
    float valor;
    do {
        valor = lerFloat(mensagem);
        if (valor < 0) {
            printf("Valor inválido. Digite um número maior ou igual a 0.\n");
        }
    } while (valor < 0);

    return valor;
}

/* ---------- Arquivo ---------- */

void salvarEmArquivo() {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "wb");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar os dados.\n");
        return;
    }

    fwrite(&proximoID, sizeof(int), 1, arquivo);
    fwrite(&quantidadeAnimais, sizeof(int), 1, arquivo);

    if (quantidadeAnimais > 0) {
        fwrite(animais, sizeof(Animal), quantidadeAnimais, arquivo);
    }

    fclose(arquivo);
}

void carregarDoArquivo() {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "rb");

    if (arquivo == NULL) {
        return;
    }

    fread(&proximoID, sizeof(int), 1, arquivo);
    fread(&quantidadeAnimais, sizeof(int), 1, arquivo);

    if (quantidadeAnimais > 0) {
        animais = (Animal *) malloc(sizeof(Animal) * quantidadeAnimais);

        if (animais == NULL) {
            printf("Erro de memória ao carregar os dados.\n");
            fclose(arquivo);
            quantidadeAnimais = 0;
            return;
        }

        fread(animais, sizeof(Animal), quantidadeAnimais, arquivo);
    }

    fclose(arquivo);
}

/* ---------- Busca ---------- */

int buscarIndicePorID(int id) {
    for (int i = 0; i < quantidadeAnimais; i++) {
        if (animais[i].id == id) {
            return i;
        }
    }
    return -1;
}

/* ---------- Funcionalidades ---------- */

void listarAnimais() {
    if (quantidadeAnimais == 0) {
        printf("\nNenhum animal cadastrado.\n");
        return;
    }

    printf("\n========== LISTA DE ANIMAIS ==========\n");

    for (int i = 0; i < quantidadeAnimais; i++) {
        printf("ID: %d\n", animais[i].id);
        printf("Tipo: %s\n", animais[i].tipo);
        printf("Nome: %s\n", animais[i].nome);
        printf("Espécie: %s\n", animais[i].especie);
        printf("Idade: %d ano(s)\n", animais[i].idade);
        printf("Peso: %.2f kg\n", animais[i].peso);
        printf("--------------------------------------\n");
    }
}

void adicionarAnimal() {
    Animal novo;

    novo.id = proximoID++;

    printf("\n========== CADASTRAR ANIMAL ==========\n");

    lerTexto("Digite o tipo de animal: ", novo.tipo, sizeof(novo.tipo));
    lerTexto("Digite o nome do animal: ", novo.nome, sizeof(novo.nome));
    lerTexto("Digite a espécie: ", novo.especie, sizeof(novo.especie));
    novo.idade = lerInteiroPositivo("Digite a idade: ");
    novo.peso = lerFloatPositivo("Digite o peso: ");

    Animal *temp = (Animal *) realloc(animais, sizeof(Animal) * (quantidadeAnimais + 1));

    if (temp == NULL) {
        printf("Erro ao alocar memória para adicionar o animal.\n");
        proximoID--;
        return;
    }

    animais = temp;
    animais[quantidadeAnimais] = novo;
    quantidadeAnimais++;

    salvarEmArquivo();
    printf("Animal cadastrado com sucesso!\n");
}

void buscarAnimalPorID() {
    if (quantidadeAnimais == 0) {
        printf("\nNenhum animal cadastrado.\n");
        return;
    }

    int id = lerInteiro("Digite o ID do animal que deseja buscar: ");
    int indice = buscarIndicePorID(id);

    if (indice == -1) {
        printf("Animal com esse ID não foi encontrado.\n");
        return;
    }

    printf("\n========== ANIMAL ENCONTRADO ==========\n");
    printf("ID: %d\n", animais[indice].id);
    printf("Tipo: %s\n", animais[indice].tipo);
    printf("Nome: %s\n", animais[indice].nome);
    printf("Espécie: %s\n", animais[indice].especie);
    printf("Idade: %d ano(s)\n", animais[indice].idade);
    printf("Peso: %.2f kg\n", animais[indice].peso);
    printf("---------------------------------------\n");
}

void editarAnimal() {
    if (quantidadeAnimais == 0) {
        printf("\nNenhum animal cadastrado.\n");
        return;
    }

    int id = lerInteiro("Digite o ID do animal que deseja editar: ");
    int indice = buscarIndicePorID(id);

    if (indice == -1) {
        printf("Animal com esse ID não foi encontrado.\n");
        return;
    }

    int opcao;

    do {
        printf("\n========== EDITAR ANIMAL ==========\n");
        printf("1. Editar tipo\n");
        printf("2. Editar nome\n");
        printf("3. Editar espécie\n");
        printf("4. Editar idade\n");
        printf("5. Editar peso\n");
        printf("6. Voltar\n");

        opcao = lerInteiro("Escolha uma opção: ");

        switch (opcao) {
            case 1:
                lerTexto("Novo tipo: ", animais[indice].tipo, sizeof(animais[indice].tipo));
                printf("Tipo atualizado com sucesso.\n");
                break;

            case 2:
                lerTexto("Novo nome: ", animais[indice].nome, sizeof(animais[indice].nome));
                printf("Nome atualizado com sucesso.\n");
                break;

            case 3:
                lerTexto("Nova espécie: ", animais[indice].especie, sizeof(animais[indice].especie));
                printf("Espécie atualizada com sucesso.\n");
                break;

            case 4:
                animais[indice].idade = lerInteiroPositivo("Nova idade: ");
                printf("Idade atualizada com sucesso.\n");
                break;

            case 5:
                animais[indice].peso = lerFloatPositivo("Novo peso: ");
                printf("Peso atualizado com sucesso.\n");
                break;

            case 6:
                printf("Voltando ao menu principal.\n");
                break;

            default:
                printf("Opção inválida.\n");
        }

        if (opcao >= 1 && opcao <= 5) {
            salvarEmArquivo();
        }

    } while (opcao != 6);
}

void removerAnimal() {
    if (quantidadeAnimais == 0) {
        printf("\nNenhum animal cadastrado.\n");
        return;
    }

    int id = lerInteiro("Digite o ID do animal que deseja remover: ");
    int indice = buscarIndicePorID(id);

    if (indice == -1) {
        printf("Animal com esse ID não foi encontrado.\n");
        return;
    }

    char confirmacao;

    printf("Tem certeza que deseja remover o animal \"%s\"? (s/n): ", animais[indice].nome);
    scanf(" %c", &confirmacao);
    limparBuffer();

    if (confirmacao != 's' && confirmacao != 'S') {
        printf("Remoção cancelada.\n");
        return;
    }

    for (int i = indice; i < quantidadeAnimais - 1; i++) {
        animais[i] = animais[i + 1];
    }

    quantidadeAnimais--;

    if (quantidadeAnimais == 0) {
        free(animais);
        animais = NULL;
    } else {
        Animal *temp = (Animal *) realloc(animais, sizeof(Animal) * quantidadeAnimais);

        if (temp != NULL) {
            animais = temp;
        }
    }

    salvarEmArquivo();
    printf("Animal removido com sucesso.\n");
}

void mostrarTotalAnimais() {
    printf("\nTotal de animais cadastrados: %d\n", quantidadeAnimais);
}

/* ---------- Menu ---------- */

void exibirMenu() {
    printf("\n=========== MENU PET SHOP ===========\n");
    printf("1. Listar animais\n");
    printf("2. Cadastrar animal\n");
    printf("3. Buscar animal por ID\n");
    printf("4. Editar animal\n");
    printf("5. Remover animal\n");
    printf("6. Mostrar total de animais\n");
    printf("7. Sair\n");
}

int main() {
    int opcao;

    carregarDoArquivo();

    do {
        exibirMenu();
        opcao = lerInteiro("Escolha uma opção: ");

        switch (opcao) {
            case 1:
                listarAnimais();
                break;

            case 2:
                adicionarAnimal();
                break;

            case 3:
                buscarAnimalPorID();
                break;

            case 4:
                editarAnimal();
                break;

            case 5:
                removerAnimal();
                break;

            case 6:
                mostrarTotalAnimais();
                break;

            case 7:
                salvarEmArquivo();
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 7);

    free(animais);
    return 0;
}