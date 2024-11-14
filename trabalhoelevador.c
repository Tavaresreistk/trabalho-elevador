#include <stdio.h>
#include <stdlib.h>

typedef struct Pessoa {
    int id;
    int andar_destino; // Andar para onde a pessoa quer ir
    struct Pessoa* prox;
} Pessoa;

typedef struct Andar {
    int numero;
    int qtd_pessoas;
    Pessoa* topo_pilha;
    struct Andar* prox;
} Andar;

typedef struct {
    Andar* inicio;
} ListaAndares;

typedef struct {
    Pessoa* topo;
} Elevador;

void inicializarListaAndares(ListaAndares* lista) {
    lista->inicio = NULL;
}

void inicializarElevador(Elevador* elevador) {
    elevador->topo = NULL;
}

Andar* buscarAndar(ListaAndares* lista, int numero) {
    Andar* atual = lista->inicio;
    while (atual != NULL) {
        if (atual->numero == numero) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

void adicionarAndar(ListaAndares* lista, int numero) {
    if (buscarAndar(lista, numero) != NULL) {
        printf("Andar %d já existe.\n", numero);
        return;
    }
    Andar* novo_andar = (Andar*)malloc(sizeof(Andar));
    novo_andar->numero = numero;
    novo_andar->qtd_pessoas = 0;
    novo_andar->topo_pilha = NULL;
    novo_andar->prox = lista->inicio;
    lista->inicio = novo_andar;
    printf("Andar %d adicionado.\n", numero);
}

void removerAndar(ListaAndares* lista, int numero) {
    Andar* atual = lista->inicio;
    Andar* anterior = NULL;
    while (atual != NULL && atual->numero != numero) {
        anterior = atual;
        atual = atual->prox;
    }
    if (atual == NULL) {
        printf("Andar %d não encontrado.\n", numero);
        return;
    }
    if (anterior == NULL) {
        lista->inicio = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }
    free(atual);
    printf("Andar %d removido.\n", numero);
}

void adicionarPessoa(Andar* andar, int* contador_id) {
    int andar_destino;
    printf("Digite o andar de destino da pessoa: ");
    scanf("%d", &andar_destino);

    Pessoa* nova_pessoa = (Pessoa*)malloc(sizeof(Pessoa));
    nova_pessoa->id = (*contador_id)++;
    nova_pessoa->andar_destino = andar_destino;
    nova_pessoa->prox = andar->topo_pilha;
    andar->topo_pilha = nova_pessoa;
    andar->qtd_pessoas++;
    printf("Pessoa %d adicionada no andar %d com destino ao andar %d.\n", nova_pessoa->id, andar->numero, andar_destino);
}

void removerPessoaDoAndar(Andar* andar) {
    if (andar->topo_pilha == NULL) {
        return;
    }
    Pessoa* pessoa_removida = andar->topo_pilha;
    andar->topo_pilha = andar->topo_pilha->prox;
    andar->qtd_pessoas--;
    free(pessoa_removida);
}

void listarAndares(ListaAndares* lista) {
    Andar* atual = lista->inicio;
    if (atual == NULL) {
        printf("Nenhum andar cadastrado.\n");
        return;
    }
    printf("Andares cadastrados:\n");
    while (atual != NULL) {
        printf("Andar %d - %d pessoas\n", atual->numero, atual->qtd_pessoas);
        atual = atual->prox;
    }
}

void coletarPessoasNoAndar(Elevador* elevador, Andar* andar) {
    while (andar->topo_pilha != NULL) {
        Pessoa* pessoa = andar->topo_pilha;
        printf("Elevador coletou Pessoa %d no andar %d para o andar %d\n", pessoa->id, andar->numero, pessoa->andar_destino);

        // Move a pessoa do andar para o elevador
        andar->topo_pilha = pessoa->prox;
        pessoa->prox = elevador->topo;
        elevador->topo = pessoa;
        andar->qtd_pessoas--;
    }
}

void entregarPessoas(Elevador* elevador) {
    Pessoa* atual = elevador->topo;
    while (atual != NULL) {
        printf("Elevador entregou Pessoa %d no andar %d\n", atual->id, atual->andar_destino);
        
        // Remove pessoa do elevador
        Pessoa* pessoa_removida = atual;
        elevador->topo = elevador->topo->prox;
        free(pessoa_removida);
        atual = elevador->topo;
    }
}

void simularElevador(ListaAndares* lista) {
    Elevador elevador;
    inicializarElevador(&elevador);

    printf("\nIniciando simulação do elevador...\n");
    
    // Etapa 1: Coleta todas as pessoas dos andares
    Andar* andar_atual = lista->inicio;
    while (andar_atual != NULL) {
        coletarPessoasNoAndar(&elevador, andar_atual);
        andar_atual = andar_atual->prox;
    }

    // Etapa 2: Entrega todas as pessoas aos seus respectivos andares
    entregarPessoas(&elevador);

    printf("Elevador retornou ao térreo após finalizar todos os transportes.\n");
}

int main() {
    ListaAndares lista;
    inicializarListaAndares(&lista);

    Elevador elevador;  // Declaração da variável elevador
    inicializarElevador(&elevador); // Inicializa o elevador

    int opcao, andar_numero;
    int contador_id = 1; // Contador para IDs únicos de pessoas

    do {
        printf("\nOpções:\n");
        printf("1. Adicionar Andar\n");
        printf("2. Remover Andar\n");
        printf("3. Adicionar Pessoa\n");
        printf("4. Listar Andares Cadastrados\n");
        printf("5. Simular Elevador\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o número do andar para adicionar: ");
                scanf("%d", &andar_numero);
                adicionarAndar(&lista, andar_numero);
                break;
            case 2:
                printf("Digite o número do andar para remover: ");
                scanf("%d", &andar_numero);
                removerAndar(&lista, andar_numero);
                break;
            case 3:
                printf("Digite o número do andar: ");
                scanf("%d", &andar_numero);
                Andar* andar = buscarAndar(&lista, andar_numero);
                if (andar != NULL) {
                    adicionarPessoa(andar, &contador_id);
                } else {
                    printf("Andar %d não encontrado.\n", andar_numero);
                }
                break;
            case 4:
                listarAndares(&lista);
                break;
            case 5:
                simularElevador(&lista);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}
