#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Aniversario{ //struct para as informações sobre a data de nascimento dos contatos
    int dia;
    int mes;
    int ano;
} Aniversario;

typedef struct Endereco{ //struct para as informações de endereço dos contatos
    int numero;
    char rua[40];
    char bairro[40];
    char cidade[40];
    char estado[40];
} Endereco;

typedef struct Contato{ //struct para cada contato
    char nome[40];
    char telefone[40];
    Aniversario aniversario;
    Endereco endereco;
    struct Contato* prox;
} Contato;

typedef struct PilhaContatos{ //struct para o topo da pilha de contatos
    Contato* topo;
} PilhaContatos;

typedef struct Indice{ //struct de índices para os contatos
    Contato* infos;
    struct Indice* prox;
} Indice;

typedef struct PilhaIndices{ //struct para o topo da pilha de índices
    Indice* topo;
} PilhaIndices;

void limparTela(){ //função para limpar a tela
    system("cls||clear");
}

Contato* buscarPeloNome(char* nome, PilhaContatos* pilhaContatos){ //função para buscar um contato pelo nome
    Contato* atual = pilhaContatos->topo;

    while (atual != NULL){
        if (strcmp(atual->nome, nome) == 0) {
            return atual;
        }
        atual = atual->prox;
    }

    return NULL;
}

Contato* buscarPeloTelefone(char* telefone, PilhaContatos* pilhacontatos){ //função para buscar um contato pelo número de celular
    Contato* atual = pilhacontatos->topo;

    while (atual != NULL){
        if (strcmp(atual->telefone, telefone) == 0){
            return atual;
        }
        atual = atual->prox;
    }

    return NULL;
}

void adicionarIndice(Contato* novoContato, PilhaIndices* pilhaIndices){ //função para adicionar um novo índice
    Indice* novoIndice = (Indice*)malloc(sizeof(Indice)); //cria um novo índice

    //inclui o novo índice na pilha de índices
    novoIndice->infos = novoContato;
    novoIndice->prox = pilhaIndices->topo;
    pilhaIndices->topo = novoIndice;
}

void adicionarContato(PilhaContatos* pilhaContatos, PilhaIndices* pilhaIndices){ //função para adicionar um novo contato
    Contato* novoContato = (Contato*)malloc(sizeof(Contato)); //cria um novo contato

    limparTela();

    //o usuário insere os dados do novo contato
    printf("Novo contato:");

    printf("\nNome: ");
    fgets(novoContato->nome, 40, stdin);
    novoContato->nome[strcspn(novoContato->nome, "\n")] = '\0';

    Contato* contatoExistente1 = buscarPeloNome(novoContato->nome, pilhaContatos);

    if (contatoExistente1 != NULL){ //verifica se já existe um contato com o mesmo nome
        printf("Esse contato ja foi cadastrado.\n");
        free(novoContato);
        return;
    }    

    printf("\nTelefone celular: ");
    fgets(novoContato->telefone, 40, stdin);
    novoContato->telefone[strcspn(novoContato->telefone, "\n")] = '\0';

    Contato* contatoExistente2 = buscarPeloTelefone(novoContato->telefone, pilhaContatos);

    if (contatoExistente2 != NULL){ //verifica se já existe um contato com o mesmo número de celular
        printf("Esse número ja foi cadastrado.\n");
        free(novoContato);
        return;
    }

    printf("\nData de nascimento (dia, mes e ano, separados por espaco): ");
    scanf("%d %d %d", &novoContato->aniversario.dia, &novoContato->aniversario.mes, &novoContato->aniversario.ano);
    getchar();

    printf("\nEndereco do contato:");
    printf("\nRua: ");
    fgets(novoContato->endereco.rua, 40, stdin);
    novoContato->endereco.rua[strcspn(novoContato->endereco.rua, "\n")] = '\0';
    printf("\nNumero: ");
    scanf("%d", &novoContato->endereco.numero);
    getchar();
    printf("\nBairro: ");
    fgets(novoContato->endereco.bairro, 40, stdin);
    novoContato->endereco.bairro[strcspn(novoContato->endereco.bairro, "\n")] = '\0';
    printf("\nCidade: ");
    fgets(novoContato->endereco.cidade, 40, stdin);
    novoContato->endereco.cidade[strcspn(novoContato->endereco.cidade, "\n")] = '\0';
    printf("\nEstado: ");
    fgets(novoContato->endereco.estado, 40, stdin);
    novoContato->endereco.estado[strcspn(novoContato->endereco.estado, "\n")] = '\0';

    limparTela();

    //inclui o novo contato na pilha de contatos
    novoContato->prox = pilhaContatos->topo;
    pilhaContatos->topo = novoContato;
    adicionarIndice(novoContato, pilhaIndices);
}

void lerContatosDoArquivo(PilhaContatos* pilhaContatos, PilhaIndices* pilhaIndices){ //função para ler os dados do arquivo
    FILE* arquivo = fopen("ListaDeContatos.txt", "r");

    if (arquivo == NULL) { //verifica se foi possível abrir o arquivo
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Contato* novoContato;
    Contato* contatoExistente1;
    Contato* contatoExistente2;

    while (!feof(arquivo)){ //lê até o final do arquivo
        novoContato = (Contato*)malloc(sizeof(Contato));

        //lê os dados do arquivo
        fscanf(arquivo, "%[^\n]\n", novoContato->nome);
        fscanf(arquivo, "%[^\n]\n", novoContato->telefone);
        fscanf(arquivo, "%d\n", &novoContato->aniversario.dia);
        fscanf(arquivo, "%d\n", &novoContato->aniversario.mes);
        fscanf(arquivo, "%d\n", &novoContato->aniversario.ano);
        fscanf(arquivo, "%[^\n]\n", novoContato->endereco.rua);
        fscanf(arquivo, "%d\n", &novoContato->endereco.numero);
        fscanf(arquivo, "%[^\n]\n", novoContato->endereco.bairro);
        fscanf(arquivo, "%[^\n]\n", novoContato->endereco.cidade);
        fscanf(arquivo, "%[^\n]\n", novoContato->endereco.estado);

        contatoExistente1 = buscarPeloNome(novoContato->nome, pilhaContatos);
        contatoExistente2 = buscarPeloTelefone(novoContato->telefone, pilhaContatos);

        if (contatoExistente1 != NULL || contatoExistente2 != NULL){ //verifica se já existe um contato com o mesmo nome ou número de celular
            free(novoContato);
        }
        else {
            //inclui o novo contato na pilha
            novoContato->prox = pilhaContatos->topo;
            pilhaContatos->topo = novoContato;

            //inclui o novo índice na pilha
            adicionarIndice(novoContato, pilhaIndices);
        }
    }

    fclose(arquivo); //fecha o arquivo
    printf("Contatos do arquivo foram carregados com sucesso!\n");
}

void removerContato(PilhaContatos* pilhaContatos, PilhaIndices* pilhaIndices){ //função para remover um contato
    char nome[40];

    limparTela();

    printf("Digite o nome do contato que deseja excluir: ");
    fgets(nome, 40, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    if(pilhaContatos->topo == NULL){ //verifica se a lista está vazia
        printf("Erro: a lista de contatos esta vazia!\n");
        return;
    }

    //cria dois contatos e dois índices temporários
    Contato* contatoAtual = pilhaContatos->topo;
    Indice* indiceAtual = pilhaIndices->topo;
    Contato* contatoAnterior = NULL;
    Indice* indiceAnterior = NULL;

    //busca o contato para ser excluído
    while (contatoAtual != NULL){
        if (strcmp(contatoAtual->nome, nome) == 0){ //exclui o contato atual e seu respectivo índice
            if (contatoAnterior == NULL){
                pilhaContatos->topo = contatoAtual->prox;
                pilhaIndices->topo = indiceAtual->prox;
            } 
            else {
                contatoAnterior->prox = contatoAtual->prox;
                indiceAnterior->prox = indiceAtual->prox;
            }

            free(contatoAtual);
            free(indiceAtual);
            printf("Contato removido com sucesso.\n");
            return;
        }
        
        contatoAnterior = contatoAtual;
        contatoAtual = contatoAtual->prox;
        indiceAnterior = indiceAtual;
        indiceAtual = indiceAtual->prox;
    }

    printf("Contato nao encontrado.\n");
}

void atualizarInformacoes(PilhaContatos* pilhaContatos){ //função para modificar/atualizar informações dos contatos
    int opcao;
    char nome[40];
    Contato* atual;

    limparTela();

    printf("Digite o nome do contato a ser atualizado: ");
    fgets(nome, 40, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    //busca o contato desejado
    atual = buscarPeloNome(nome, pilhaContatos);

    printf("\n1. Mudar numero de telefone");
    printf("\n2. Mudar data de nascimento");
    printf("\n3. Mudar endereco");
    printf("\nDigite a opcao desejada: ");
    scanf("%d", &opcao);
    getchar();
    limparTela();

    //seleciona qual informação vai ser modificada
    switch (opcao)
    {
    case 1:
        printf("Digite o novo numero de telefone: ");
        fgets(atual->telefone, 40, stdin);
        atual->telefone[strcspn(atual->telefone, "\n")] = '\0';
        printf("Numero atualizado com sucesso.");
        break;
    case 2:
        printf("Digite a nova data de nascimento (dia, mes e ano, separados por espaco): ");
        scanf("%d %d %d", &atual->aniversario.dia, &atual->aniversario.mes, &atual->aniversario.ano);
        break;

    case 3:
        printf("Digite o novo endereco: ");
        printf("\nRua: ");
        fgets(atual->endereco.rua, 40, stdin);
        atual->endereco.rua[strcspn(atual->endereco.rua, "\n")] = '\0';
        printf("\nNumero: ");
        scanf("%d", &atual->endereco.numero);
        getchar();
        printf("\nBairro: ");
        fgets(atual->endereco.bairro, 40, stdin);
        atual->endereco.bairro[strcspn(atual->endereco.bairro, "\n")] = '\0';
        printf("\nCidade: ");
        fgets(atual->endereco.cidade, 40, stdin);
        atual->endereco.cidade[strcspn(atual->endereco.cidade, "\n")] = '\0';
        printf("\nEstado: ");
        fgets(atual->endereco.estado, 40, stdin);
        atual->endereco.estado[strcspn(atual->endereco.estado, "\n")] = '\0';
    default:
        break;
    }

    limparTela();

    //mostra o contato atualizado
    printf("Contato atualizado:");
    printf("\nTelefone celular: %s\n", atual->telefone);
    printf("\nData de nascimento: %d/%d/%d\n", atual->aniversario.dia, atual->aniversario.mes, atual->aniversario.ano);
    printf("\nEndereco: %s, %d, %s, %s, %s\n", atual->endereco.rua, atual->endereco.numero, atual->endereco.bairro, atual->endereco.cidade, atual->endereco.estado);
}

void ordenarContatos(PilhaIndices* pilhaIndices){ //função para organizar os contatos em ordem alfabética
    Indice* aux1 = pilhaIndices->topo;

    while(aux1 != NULL){
        Indice* aux2 = aux1->prox;

        while(aux2 != NULL){
            if(strcmp(aux1->infos->nome, aux2->infos->nome) > 0){
                Contato* temp = aux1->infos;
                aux1->infos = aux2->infos;
                aux2->infos = temp;
            }
            aux2 = aux2->prox;
        }
        aux1 = aux1->prox;
    }
}

void exibirContatos(PilhaContatos* pilhaContatos){ //função para exibir todos os contatos
    Contato* atual = pilhaContatos->topo;

    limparTela();

    if(atual == NULL) {
        printf("Lista de contatos vazia.\n");
    } 
    else {
        printf("======= Contatos =======\n");

        while (atual != NULL) {
            printf("Nome: %s\n", atual->nome);
            atual = atual->prox;
        }
        printf("========================\n");
    }
}

void exibirContatosOrdenados(PilhaIndices* pilhaIndices){ //função para exibir todos os contatos em ordem alfabética
    ordenarContatos(pilhaIndices);
    Indice* atual = pilhaIndices->topo;

    limparTela();

    if(atual == NULL){
        printf("Lista de contatos vazia.\n");
    }
    else {
        printf("======= Contatos =======\n");
        while (atual != NULL){
            printf("Nome: %s\n", atual->infos->nome);
            atual = atual->prox;
        }
        printf("========================\n");
    }
}

void exibirContatoBuscado(PilhaContatos* pilhaContatos){ //função para exibir as informações do contato buscado
    char nome[40];
    char telefone[40];
    int opcao;
    Contato* contatoBuscado;

    limparTela();
    printf("1. Buscar pelo nome\n");
    printf("2. Buscar pelo telefone celular\n");
    printf("\nDigite a opcao desejada: ");
    scanf("%d", &opcao);
    getchar();
    limparTela();

    //verifica qual foi a opção escolhida
    if(opcao == 1){ 
        printf("Digite o nome do contato: ");
        fgets(nome, 40, stdin);
        nome[strcspn(nome, "\n")] = '\0';

        contatoBuscado = buscarPeloNome(nome, pilhaContatos);
        if (contatoBuscado == NULL){ //verifica se o contato existe
            printf("Contato não encontrado.\n");
            return;
        } 
    }

    else if(opcao == 2){
        printf("Digite o numero de celular do contato: ");
        fgets(telefone, 40, stdin);
        telefone[strcspn(telefone, "\n")] = '\0';

        contatoBuscado = buscarPeloTelefone(telefone, pilhaContatos);
        if (contatoBuscado == NULL){ //verifica se o contato existe
            printf("Contato não encontrado.\n");
            return;
        } 
    }

    else{
        printf("Opcao invalida.\n");
        return;
    }

    //exibe as informações do contato
    limparTela();
    printf("======= Contato =======\n");
    printf("Nome: %s\n", contatoBuscado->nome);
    printf("Telefone celular: %s\n", contatoBuscado->telefone);
    printf("Data de nascimento: %d/%d/%d\n", contatoBuscado->aniversario.dia, contatoBuscado->aniversario.mes, contatoBuscado->aniversario.ano);
    printf("Endereco: %s, %d, %s, %s, %s\n", contatoBuscado->endereco.rua, contatoBuscado->endereco.numero, contatoBuscado->endereco.bairro, contatoBuscado->endereco.cidade, contatoBuscado->endereco.estado);
    printf("========================\n");
}

void exibirInformacoes(PilhaContatos* pilhaContatos){ //função para exibir as informações de todos os contatos
    Contato* atual = pilhaContatos->topo;

    limparTela();

    if(atual == NULL) {
        printf("Lista de contatos vazia.\n");
    } 
    else {
        printf("======= Contatos =======\n");

        while (atual != NULL){ //percorre toda a lista
            printf("Nome: %s\n", atual->nome);
            printf("Telefone celular: %s\n", atual->telefone);
            printf("Data de nascimento: %d/%d/%d\n", atual->aniversario.dia, atual->aniversario.mes, atual->aniversario.ano);
            printf("Endereco: %s, %d, %s, %s, %s\n", atual->endereco.rua, atual->endereco.numero, atual->endereco.bairro, atual->endereco.cidade, atual->endereco.estado);
            printf("------------------------\n");

            atual = atual->prox;
        }
        printf("========================\n");
    }
}

void salvarNoArquivo(PilhaContatos* pilhaContatos){ //função para salvar os dados do programa em um arquivo
    FILE *arquivo = fopen("ListaDeContatos.txt", "w");

    if (arquivo == NULL){ //verifica se foi possível abrir o arquivo
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Contato* atual = pilhaContatos->topo;

    //escreve os dados no arquivo
    while (atual != NULL){
        fprintf(arquivo, "%s\n", atual->nome);
        fprintf(arquivo, "%s\n", atual->telefone);
        fprintf(arquivo, "%d\n", atual->aniversario.dia);
        fprintf(arquivo, "%d\n", atual->aniversario.mes);
        fprintf(arquivo, "%d\n", atual->aniversario.ano);
        fprintf(arquivo, "%s\n", atual->endereco.rua);
        fprintf(arquivo, "%d\n", atual->endereco.numero);
        fprintf(arquivo, "%s\n", atual->endereco.bairro);
        fprintf(arquivo, "%s\n", atual->endereco.cidade);
        fprintf(arquivo, "%s\n", atual->endereco.estado);

        atual = atual->prox;
    }

    fclose(arquivo); //fecha o arquivo
    printf("Contatos salvos com sucesso!\n");
}

void encerrarPrograma(PilhaContatos* pilhaContatos, PilhaIndices* pilhaIndices){ //função para encerrar o programa e liberar a memória alocada
    //cria dois contatos e dois índices temporários
    Contato* atual1 = pilhaContatos->topo;
    pilhaContatos->topo = NULL;
    Contato* proximo1;

    Indice* atual2 = pilhaIndices->topo;
    pilhaIndices->topo = NULL;
    Indice* proximo2;    
    
    //percorre toda a pilha para liberar a memória anteriormente alocada
    while(atual1 != NULL){
        proximo1 = atual1->prox;
        proximo2 = atual2->prox;
        free(atual1);
        free(atual2);
        atual1 = proximo1;
        atual2 = proximo2;
    }

    printf("Programa encerrado.\n");
    exit(0);
}

void exibirMenu(){ //função para exibir o menu para o usuário
    printf("======= Lista de Contatos =======\n");
    printf("1. Adicionar contato\n");
    printf("2. Remover contato\n");
    printf("3. Exibir contatos\n");
    printf("4. Exibir contatos em ordem alfabetica\n");
    printf("5. Buscar contato\n");
    printf("6. Ler contatos do arquivo\n");
    printf("7. Atualizar informacoes sobre um contato\n");
    printf("8. Exibir as informacoes de todos os contatos\n");
    printf("9. Salvar contatos no arquivo\n");
    printf("10. Encerrar o programa\n");
    printf("=================================\n");
    printf("Digite a opcao desejada: ");
}

int main(){
    PilhaContatos* pilhaContatos;
    PilhaIndices* pilhaIndices;
    int opcao;

    pilhaContatos = (PilhaContatos*)malloc(sizeof(PilhaContatos));
    pilhaIndices = (PilhaIndices*)malloc(sizeof(PilhaIndices));
    pilhaContatos->topo = NULL;
    pilhaIndices->topo = NULL;

    while(1){
        exibirMenu();
        scanf("%d", &opcao);
        getchar();

        switch (opcao){
            case 1:
                adicionarContato(pilhaContatos, pilhaIndices);
                break;
            case 2:
                removerContato(pilhaContatos, pilhaIndices);
                break;
            case 3:
                exibirContatos(pilhaContatos);
                break;
            case 4:
                exibirContatosOrdenados(pilhaIndices);
                break;
            case 5:
                exibirContatoBuscado(pilhaContatos);
                break;
            case 6:
                lerContatosDoArquivo(pilhaContatos, pilhaIndices);
                break;
            case 7:
                atualizarInformacoes(pilhaContatos);
                break;
            case 8:
                exibirInformacoes(pilhaContatos);
                break;
            case 9:
                salvarNoArquivo(pilhaContatos);
                break;
            case 10:
                encerrarPrograma(pilhaContatos, pilhaIndices);
                break;
            default:
                printf("Opcao invalida.");
                break;
        }
        printf("Pressione Enter para continuar...");
        getchar();
        limparTela();
    }
    
    free(pilhaContatos);
    free(pilhaIndices);
    return 0; 
}