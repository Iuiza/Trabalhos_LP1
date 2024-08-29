#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ProdutoStruct {
    int codigo;
    char nome[50];
    char descricao[100];
    int quantidade;
    float preco;
    struct ProdutoStruct *prox;
} ProdutoType;


// ProdutoType *listaProdutos = NULL;

ProdutoType *buscarProduto(int codigo, ProdutoType *listaProdutos);

void limparTela() {
//    system(CLEAR_SCREEN);
    system("cls||clear");
}

void incluirProduto(ProdutoType **listaProdutos) {
    int codigo;
    ProdutoType *atual;
    atual = *listaProdutos;
// Equivalente à: ProdutoType *atual = *listaProdutos;

    limparTela();
    printf("Digite o código do produto: ");
    scanf("%d", &codigo);

// Verifica se o código já está sendo utilizado
    while (atual != NULL) {
        if (atual->codigo == codigo) {
            printf("Já existe um produto com o código %d.\n", codigo);
            return;
        }
        atual = atual->prox;
    }

// Cria novo produto
    ProdutoType *novoProduto = (ProdutoType *)malloc(sizeof(ProdutoType));
    novoProduto->codigo = codigo;

    getchar(); // Captura o caractere ENTER

    printf("Digite o nome do produto: ");
    fgets(novoProduto->nome, sizeof(novoProduto->nome), stdin);
    novoProduto->nome[strcspn(novoProduto->nome, "\n")] = '\0';
// Remove o caractere ENTER

    printf("Digite a descrição do produto: ");
    fgets(novoProduto->descricao, sizeof(novoProduto->descricao), stdin);
    novoProduto->descricao[strcspn(novoProduto->descricao, "\n")] = '\0';
// Remove o caractere ENTER

    printf("Digite a quantidade do produto: ");
    scanf("%d", &(novoProduto->quantidade));

    printf("Digite o preço unitário do produto: ");
    scanf("%f", &(novoProduto->preco));

    novoProduto->prox = NULL;

// Inclui o novo produto na lista encadeada
    if (*listaProdutos == NULL) {
        *listaProdutos = novoProduto;
    } else {
        ProdutoType *ultimo = *listaProdutos;
        while (ultimo->prox != NULL) {
            ultimo = ultimo->prox;
        }
        ultimo->prox = novoProduto;
    }

    printf("Produto incluído com sucesso.\n");
}


void lerProdutosDoArquivo(ProdutoType **listaProdutos) {
    FILE *arquivo = fopen("estoque.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    ProdutoType *produtoExistente;
    ProdutoType *novoProduto;

    while (!feof(arquivo)) {
        novoProduto = (ProdutoType *)malloc(sizeof(ProdutoType));

        fscanf(arquivo, "%d\n", &novoProduto->codigo);
        fscanf(arquivo, "%[^\n]\n", novoProduto->nome);
        fscanf(arquivo, "%[^\n]\n", novoProduto->descricao);
        fscanf(arquivo, "%d\n", &novoProduto->quantidade);
        fscanf(arquivo, "%f\n", &novoProduto->preco);

        produtoExistente = buscarProduto(novoProduto->codigo, *listaProdutos);
        if (produtoExistente != NULL) {
            produtoExistente->quantidade += novoProduto->quantidade;
            free(novoProduto);
        } else {
            novoProduto->prox = *listaProdutos;
            *listaProdutos = novoProduto;
        }
    }

    fclose(arquivo);
    printf("Produtos do arquivo foram carregados com sucesso!\n");
}

ProdutoType *buscarProduto(int codigo, ProdutoType *listaProdutos) {

    ProdutoType *atual = listaProdutos;
    while (atual != NULL) {
        if (atual->codigo == codigo) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

void acrescentarItens(ProdutoType *listaProdutos) {
    int codigo;
    int quantidade;

    printf("Digite o código do produto: ");
    scanf("%d", &codigo);

    ProdutoType *produto = buscarProduto(codigo, listaProdutos);

    if (produto == NULL) {
        printf("Produto não encontrado.\n");
        return;
    }

    printf("Digite a quantidade de itens a serem acrescentados: ");
    scanf("%d", &quantidade);

    produto->quantidade += quantidade;

    printf("Itens acrescentados com sucesso!\n");
}

void retirarItens(ProdutoType *listaProdutos) {
    int codigo;
    int quantidade;

    printf("Digite o código do produto: ");
    scanf("%d", &codigo);

    ProdutoType *produto = buscarProduto(codigo, listaProdutos);

    if (produto == NULL) {
        printf("Produto não encontrado.\n");
        return;
    }

    printf("Digite a quantidade de itens a serem retirados: ");
    scanf("%d", &quantidade);

    if (quantidade > produto->quantidade) {
        printf("Quantidade insuficiente no estoque.\n");
    } else {
        produto->quantidade -= quantidade;
        printf("Itens retirados com sucesso!\n");
    }
}

void gravarProdutosEmArquivo(ProdutoType *listaProdutos) {
    FILE *arquivo = fopen("estoque.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    ProdutoType *atual = listaProdutos;

    while (atual != NULL) {
        fprintf(arquivo, "%d\n", atual->codigo);
        fprintf(arquivo, "%s\n", atual->nome);
        fprintf(arquivo, "%s\n", atual->descricao);
        fprintf(arquivo, "%d\n", atual->quantidade);
        fprintf(arquivo, "%.2f\n", atual->preco);

        atual = atual->prox;
    }

    fclose(arquivo);
    printf("Produtos gravados no arquivo com sucesso!\n");
}

void encerrarPrograma(ProdutoType *listaProdutos) {
    ProdutoType *atual = listaProdutos;
    ProdutoType *prox;

    while (atual != NULL) {
        prox = atual->prox;
        free(atual);
        atual = prox;
    }

    listaProdutos = NULL;

    printf("Programa encerrado.\n");
    exit(0);
}

void exibirProduto(ProdutoType *listaProdutos) {
    int codigo;

    limparTela();
    printf("Digite o código do produto: ");
    scanf("%d", &codigo);

    ProdutoType *produto = buscarProduto(codigo, listaProdutos);

    if (produto == NULL) {
        printf("Produto não encontrado.\n");
    } else {
        limparTela();
        printf("======= Produto =======\n");
        printf("Código: %d\n", produto->codigo);
        printf("Nome: %s\n", produto->nome);
        printf("Descrição: %s\n", produto->descricao);
        printf("Quantidade: %d\n", produto->quantidade);
        printf("Preço: R$ %.2f\n", produto->preco);
        printf("=======================\n");
    }
}

void exibirEstoque(ProdutoType *listaProdutos) {
    ProdutoType *atual = listaProdutos;

    limparTela();
    if (atual == NULL) {
        printf("Estoque vazio.\n");
    } else {
        printf("======= Estoque =======\n");
        while (atual != NULL) {
            printf("Código: %d\n", atual->codigo);
            printf("Nome: %s\n", atual->nome);
            printf("Descrição: %s\n", atual->descricao);
            printf("Quantidade: %d\n", atual->quantidade);
            printf("Preço: R$ %.2f\n", atual->preco);
            printf("-----------------------\n");
            atual = atual->prox;
        }
        printf("=======================\n");
    }
}

void excluirProduto(ProdutoType **listaProdutos) {
    int codigo;

    limparTela();
    printf("Digite o código do produto a ser excluído: ");
    scanf("%d", &codigo);

    ProdutoType *atual = *listaProdutos;
    ProdutoType *anterior = NULL;

    while (atual != NULL) {
        if (atual->codigo == codigo) {
            if (anterior == NULL) {
                // Produto a ser excluído é o primeiro da lista
                *listaProdutos = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }

            free(atual);
            printf("Produto excluído com sucesso.\n");
            return;
        }

        anterior = atual;
        atual = atual->prox;
    }

    printf("Produto não encontrado.\n");
}

void exibirMenu() {
    printf("======= Controle de Estoque =======\n");
    printf("1. Incluir um produto\n");
    printf("2. Excluir um produto\n");
    printf("3. Ler produtos do arquivo\n");
    printf("4. Acrescentar itens\n");
    printf("5. Retirar itens\n");
    printf("6. Gravar produtos no arquivo\n");
    printf("7. Exibir um produto do estoque\n");
    printf("8. Exibir todo o estoque\n");
    printf("9. Encerrar o programa\n");
    printf("===================================\n");
    printf("Digite a opção desejada: ");
}

int main() {
    ProdutoType *listaProdutos;
    listaProdutos = NULL;
//  Equivalente à: ProdutoType *listaProdutos = NULL;
    int opcao;

    while (1) {
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                incluirProduto(&listaProdutos);
                break;
            case 2:
                excluirProduto(&listaProdutos);
                break;
            case 3:
                lerProdutosDoArquivo(&listaProdutos);
                break;
            case 4:
                acrescentarItens(listaProdutos);
                break;
            case 5:
                retirarItens(listaProdutos);
                break;
            case 6:
                gravarProdutosEmArquivo(listaProdutos);
                break;
            case 7:
                exibirProduto(listaProdutos);
                break;
            case 8:
                exibirEstoque(listaProdutos);
                break;
            case 9:
                encerrarPrograma(listaProdutos);
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }

        getchar(); // Captura o caractere ENTER

        printf("Pressione Enter para continuar...");
        getchar(); // Espera por um ENTER
        limparTela();
    }

    return 0;
}
