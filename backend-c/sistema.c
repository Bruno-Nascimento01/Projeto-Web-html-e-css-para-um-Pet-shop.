// =============================================================
// Projeto: Pet Shop
// Autor: Bruno Nascimento Ribeiro Oliveira
// Membros do grupo: 
// 1- Bruno Nascimento Ribeiro Oliveira (Lider)
// 2-Keith Leandra Carvalho da Silva 
// 3- Lucas de Melo Eduardo 
// 4-Kaique de Oliveira Silva 
// 5-Otavio Santos Lidorio 
// 6-Davi Sousa Santana 
// 7-kayk Carvalho Andrade 

// Descrição:
// Software desktop desenvolvido em linguagem C para automatizar a gestão operacional
// de um petshop. O sistema realiza o cadastro, controle e listagem de clientes,
// animais (com vínculo aos seus respectivos donos), funcionários/veterinários,
// cargos, departamentos, produtos (estoque) e serviços oferecidos.
// =============================================================

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
 
// ============================================================
// CONSTANTES DE DEFINIÇÃO DE TAMANHO DE STRINGS 
// ============================================================
#define TAM_CHAR_GRANDE 100
#define TAM_CHAR_MEDIO 50
#define TAM_CHAR_PEQUENO 10
#define TAM_CEP 11
#define TAM_ESTADO 5
#define TAM_CPF 15
#define TAM_DATA 20
#define TAM_DDD 6
#define TAM_TELEFONE 20
#define TAM_TEXTO 500
#define TAM_Crmv 20
 
// ============================================================
// LIMITES MÁXIMOS PARA OS ARRAYS DO SISTEMA (VETORES GLOBAIS)
// ============================================================
#define MAX_CLIENTES 100
#define MAX_ANIMAIS 100
#define MAX_FUNCIONARIOS 100
#define MAX_CARGOS 100
#define MAX_DEPARTAMENTOS 100
#define MAX_PRODUTOS 100
#define MAX_SERVICOS 100
 
int escolha; // Variável global para armazenar as opções dos menus
 
/**
 * Função utilitária para limpar o buffer do teclado (stdin).
 * Evita que o caractere '\n' (Enter) 
 * estrague a leitura de strings feitas por funções como fgets.
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
 
// ============================================================
// ESTRUTURAS DE DADOS (STRUCTS)
// ============================================================
 
// Estrutura para armazenar as informações cadastrais dos Clientes
struct cliente {
    int id; // ID Único do Cliente
    char nomeCliente[TAM_CHAR_MEDIO];
    char sobrenomeCliente[TAM_CHAR_MEDIO];
    char cpf_Cliente[TAM_CPF];
    char email_Cliente[TAM_CHAR_GRANDE];
    char logradouro_Cliente[TAM_CHAR_GRANDE];
    char numeroResid_Cliente[TAM_CHAR_PEQUENO];
    char cep_Cliente[TAM_CEP];
    char bairro_Cliente[TAM_CHAR_MEDIO];
    char cidade_cliente[TAM_CHAR_MEDIO];
    char estadoUF_cliente[TAM_ESTADO];
    char ddd_Tele_Cliente[TAM_DDD];
    char numero_tel_Cliente[TAM_TELEFONE];
    char tipo_telefone[TAM_CHAR_MEDIO];
    char data_cadastro_Cliente[TAM_DATA];
};
typedef struct cliente cliente;
 
// Estrutura para os Animais, mapeando o ID do cliente (dono) para relacionamento
struct animal {
   int id; // ID Único do Animal
   char nomeAnimal[TAM_CHAR_GRANDE];
   char data_nascimento[TAM_DATA];
   char sexo[TAM_CHAR_MEDIO];
   double peso;
   char raca[TAM_CHAR_GRANDE];
   char cor[TAM_CHAR_GRANDE];
   char especie[TAM_CHAR_GRANDE];
   char observacao[TAM_TEXTO];
   int idCliente; // Chave estrangeira/relacionamento com o ID do Cliente
};
typedef struct animal animal;
 
// Estrutura para Funcionários e Veterinários (com campos específicos para CRMV)
struct funcionario {
    int id; // ID Único do Funcionário
    char nomeFuncionario[TAM_CHAR_MEDIO];
    char sobrenomeFuncionario[TAM_CHAR_MEDIO];
    char cpf_Funcionario[TAM_CPF];
    char CRMV_Veterinario[TAM_Crmv]; // Preenchido apenas se for veterinário
    char data_Admissao[TAM_DATA];
    char status_Funcionario[TAM_CHAR_PEQUENO]; // ATIVO ou INATIVO
    char formacao_Veterinario[TAM_CHAR_GRANDE];
    int cargo_id;        // Relacionamento com o ID do Cargo
    int departamento_id; // Relacionamento com o ID do Departamento
};
typedef struct funcionario funcionario;
 
// Estrutura para os Cargos da empresa e respectivas faixas salariais
struct cargo {
    int id; // ID Único do Cargo
    char nomeCargo[TAM_CHAR_GRANDE];
    double salarioCargo;
};
typedef struct cargo cargo;
 
// Estrutura para os Departamentos do petshop
struct departamento {
    int id; // ID Único do Departamento
    char nomeDepartamento[TAM_CHAR_GRANDE];
    char descricaoDepartamento[TAM_TEXTO];
};
typedef struct departamento departamento;
 
// Estrutura para o Controle de Estoque de Produtos
struct produto {
    int id; // ID Único do Produto
    char nomeProduto[TAM_CHAR_GRANDE];
    char descricaoProduto[TAM_TEXTO];
    char marcaProduto[TAM_CHAR_GRANDE];
    char categoriaProduto[TAM_CHAR_MEDIO];
    double precoUniProduto;
    int quantidade_Estq_Produto;
    int IdFornecedor;
};
typedef struct produto produto;
 
// Estrutura para os Serviços oferecidos pelo petshop (Ex: Banho, Tosa, Consulta)
struct servico {
    int id; // ID Único do Serviço
    char nomeServico[TAM_CHAR_GRANDE];
    char descricaoServico[TAM_TEXTO];
    double precoServico;
};
typedef struct servico servico;
 
// ============================================================
// ARRAYS GLOBAIS (BANCO DE DADOS EM MEMÓRIA RAM)
// ============================================================
cliente      clientes[MAX_CLIENTES];
animal       animais[MAX_ANIMAIS];
funcionario  funcionarios[MAX_FUNCIONARIOS];
cargo        cargos[MAX_CARGOS];
departamento departamentos[MAX_DEPARTAMENTOS];
produto      produtos[MAX_PRODUTOS];
servico      servicos[MAX_SERVICOS];
 
// Contadores globais para rastrear a quantidade de registros inseridos
int total_clientes      = 0;
int total_animais       = 0;
int total_funcionarios  = 0;
int total_cargos        = 0;
int total_departamentos = 0;
int total_produtos      = 0;
int total_servicos      = 0;
 
// ============================================================
// FUNÇÕES DE PERSISTÊNCIA (GRAVAÇÃO EM ARQUIVOS TEXTO)
// Data serializada utilizando o caractere pipe '|' como delimitador
// ============================================================
 
void salvarCliente() {
    FILE *arquivo = fopen("clientes.txt", "w");
    if (arquivo == NULL) { printf("Erro ao salvar clientes!\n"); return; }
    for (int i = 0; i < total_clientes; i++) {
        fprintf(arquivo, "%d|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s\n",
            clientes[i].id,
            clientes[i].nomeCliente, clientes[i].sobrenomeCliente,
            clientes[i].cpf_Cliente, clientes[i].email_Cliente,
            clientes[i].logradouro_Cliente, clientes[i].numeroResid_Cliente,
            clientes[i].cep_Cliente, clientes[i].bairro_Cliente,
            clientes[i].cidade_cliente, clientes[i].estadoUF_cliente,
            clientes[i].ddd_Tele_Cliente, clientes[i].numero_tel_Cliente,
            clientes[i].tipo_telefone, clientes[i].data_cadastro_Cliente);
    }
    fclose(arquivo);
    printf("Clientes salvos com sucesso!\n");
}
 
void salvarAnimais() {
    FILE *arquivo = fopen("animais.txt", "w");
    if (arquivo == NULL) { printf("Erro ao salvar animais!\n"); return; }
    for (int i = 0; i < total_animais; i++) {
        fprintf(arquivo, "%d|%s|%s|%s|%lf|%s|%s|%s|%s|%d\n",
            animais[i].id,
            animais[i].nomeAnimal, animais[i].data_nascimento,
            animais[i].sexo, animais[i].peso,
            animais[i].raca, animais[i].cor,
            animais[i].especie, animais[i].observacao,
            animais[i].idCliente);
    }
    fclose(arquivo);
    printf("Animais salvos com sucesso!\n");
}
 
void salvarFuncionarios() {
    FILE *arquivo = fopen("funcionarios.txt", "w");
    if (arquivo == NULL) { printf("Erro ao salvar funcionarios!\n"); return; }
    for (int i = 0; i < total_funcionarios; i++) {
        fprintf(arquivo, "%d|%s|%s|%s|%s|%s|%s|%s|%d|%d\n",
            funcionarios[i].id,
            funcionarios[i].nomeFuncionario, funcionarios[i].sobrenomeFuncionario,
            funcionarios[i].cpf_Funcionario, funcionarios[i].CRMV_Veterinario,
            funcionarios[i].data_Admissao, funcionarios[i].status_Funcionario,
            funcionarios[i].formacao_Veterinario,
            funcionarios[i].cargo_id, funcionarios[i].departamento_id);
    }
    fclose(arquivo);
    printf("Funcionarios salvos com sucesso!\n");
}
 
void salvarCargos() {
    FILE *arquivo = fopen("cargos.txt", "w");
    if (arquivo == NULL) { printf("Erro ao salvar cargos!\n"); return; }
    for (int i = 0; i < total_cargos; i++) {
        fprintf(arquivo, "%d|%s|%lf\n", cargos[i].id, cargos[i].nomeCargo, cargos[i].salarioCargo);
    }
    fclose(arquivo);
    printf("Cargos salvos com sucesso!\n");
}
 
void salvarDepartamentos() {
    FILE *arquivo = fopen("departamentos.txt", "w");
    if (arquivo == NULL) { printf("Erro ao salvar departamentos!\n"); return; }
    for (int i = 0; i < total_departamentos; i++) {
        fprintf(arquivo, "%d|%s|%s\n",
            departamentos[i].id,
            departamentos[i].nomeDepartamento,
            departamentos[i].descricaoDepartamento);
    }
    fclose(arquivo);
    printf("Departamentos salvos com sucesso!\n");
}
 
void salvarProdutos() {
    FILE *arquivo = fopen("produtos.txt", "w");
    if (arquivo == NULL) { printf("Erro ao salvar produtos!\n"); return; }
    for (int i = 0; i < total_produtos; i++) {
        fprintf(arquivo, "%d|%s|%s|%s|%s|%lf|%d|%d\n",
            produtos[i].id,
            produtos[i].nomeProduto, produtos[i].descricaoProduto,
            produtos[i].marcaProduto, produtos[i].categoriaProduto,
            produtos[i].precoUniProduto, produtos[i].quantidade_Estq_Produto,
            produtos[i].IdFornecedor);
    }
    fclose(arquivo);
    printf("Produtos salvos com sucesso!\n");
}
 
void salvarServicos() {
    FILE *arquivo = fopen("servicos.txt", "w");
    if (arquivo == NULL) { printf("Erro ao salvar servicos!\n"); return; }
    for (int i = 0; i < total_servicos; i++) {
        fprintf(arquivo, "%d|%s|%s|%lf\n",
            servicos[i].id,
            servicos[i].nomeServico,
            servicos[i].descricaoServico,
            servicos[i].precoServico);
    }
    fclose(arquivo);
    printf("Servicos salvos com sucesso!\n");
}

// ============================================================
// FUNÇÕES DE CAPTURA E CADASTRO
// Recebem ponteiros para preencher diretamente a struct correspondente
// ============================================================
 
void cadastroCliente(cliente *cliente, int novoId){
        system("clear");
        cliente->id = novoId; // Atribui ID auto-incremental

        printf("\n*********************************");
        printf("\n* ENTRE COM OS DADOS DO CLIENTE *");
        printf("\n* ID GERADO: %d                  *", cliente->id);
        printf("\n*********************************");

        printf("\nPrimeiro nome: ");
        fgets(cliente->nomeCliente, sizeof(cliente->nomeCliente), stdin);
        cliente->nomeCliente[strcspn(cliente->nomeCliente, "\n")] = '\0';

        printf("\nSobrenome: ");
        fgets(cliente->sobrenomeCliente, sizeof(cliente->sobrenomeCliente), stdin);
        cliente->sobrenomeCliente[strcspn(cliente->sobrenomeCliente, "\n")] = '\0';

        printf("\nCPF: ");
        fgets(cliente->cpf_Cliente, sizeof(cliente->cpf_Cliente), stdin);
        cliente->cpf_Cliente[strcspn(cliente->cpf_Cliente, "\n")] = '\0';

        printf("\nEmail: ");
        fgets(cliente->email_Cliente, sizeof(cliente->email_Cliente), stdin);
        cliente->email_Cliente[strcspn(cliente->email_Cliente, "\n")] = '\0';

        printf("\n************************");
        printf("\n* ENTRE COM O ENDERECO *");
        printf("\n************************");

        printf("\nLogradouro: ");
        fgets(cliente->logradouro_Cliente, sizeof(cliente->logradouro_Cliente), stdin);
        cliente->logradouro_Cliente[strcspn(cliente->logradouro_Cliente, "\n")] = '\0';

        printf("\nNumero Residential: ");
        fgets(cliente->numeroResid_Cliente, sizeof(cliente->numeroResid_Cliente), stdin);
        cliente->numeroResid_Cliente[strcspn(cliente->numeroResid_Cliente, "\n")] = '\0';

        printf("\nCEP: ");
        fgets(cliente->cep_Cliente, sizeof(cliente->cep_Cliente), stdin);
        cliente->cep_Cliente[strcspn(cliente->cep_Cliente, "\n")] = '\0';

        printf("\nBairro: ");
        fgets(cliente->bairro_Cliente, sizeof(cliente->bairro_Cliente), stdin);
        cliente->bairro_Cliente[strcspn(cliente->bairro_Cliente, "\n")] = '\0';

        printf("\nCidade: ");
        fgets(cliente->cidade_cliente, sizeof(cliente->cidade_cliente), stdin);
        cliente->cidade_cliente[strcspn(cliente->cidade_cliente, "\n")] = '\0';

        printf("\nEstado(UF): ");
        fgets(cliente->estadoUF_cliente, sizeof(cliente->estadoUF_cliente), stdin);
        cliente->estadoUF_cliente[strcspn(cliente->estadoUF_cliente, "\n")] = '\0';

        printf("\n************************");
        printf("\n* ENTRE COM O TELEFONE *");
        printf("\n************************");

        printf("\nDDD: ");
        fgets(cliente->ddd_Tele_Cliente, sizeof(cliente->ddd_Tele_Cliente), stdin);
        cliente->ddd_Tele_Cliente[strcspn(cliente->ddd_Tele_Cliente, "\n")] = '\0';

        printf("\nNumero: ");
        fgets(cliente->numero_tel_Cliente, sizeof(cliente->numero_tel_Cliente), stdin);
        cliente->numero_tel_Cliente[strcspn(cliente->numero_tel_Cliente, "\n")] = '\0';

        // Captura automática da data do sistema para o cadastro do cliente
        time_t data = time(NULL);
        struct tm *hoje = localtime(&data);
        strftime(cliente->data_cadastro_Cliente, sizeof(cliente->data_cadastro_Cliente), "%d-%m-%Y", hoje);

        // Tela de confirmação dos dados digitados
        system("clear");
        printf("\n=============================================");
        printf("\n   CONFERÊNCIA DOS DADOS CADASTRADOS");
        printf("\n=============================================");
        printf("\nID Cliente:    %d", cliente->id);
        printf("\nNome Completo: %s %s", cliente->nomeCliente, cliente->sobrenomeCliente);
        printf("\nCPF:           %s", cliente->cpf_Cliente);
        printf("\nEmail:         %s", cliente->email_Cliente);
        printf("\n---------------------------------------------");
        printf("\nEndereço:      %s, Nº %s", cliente->logradouro_Cliente, cliente->numeroResid_Cliente);
        printf("\nBairro:        %s", cliente->bairro_Cliente);
        printf("\nCidade/UF:     %s - %s", cliente->cidade_cliente, cliente->estadoUF_cliente);
        printf("\nCEP:           %s", cliente->cep_Cliente);
        printf("\n---------------------------------------------");
        printf("\nTelefone:      (%s) %s", cliente->ddd_Tele_Cliente, cliente->numero_tel_Cliente);
        printf("\nData Cadastro: %s", cliente->data_cadastro_Cliente);
        printf("\n=============================================\n");
}
 
void cadastroAnimal(animal *animal, int novoId){
        system("clear");
        animal->id = novoId; // Atribui ID auto-incremental

        printf("\n********************************");
        printf("\n* Entre com os dados do Animal *");
        printf("\n* ID GERADO: %d                  *", animal->id);
        printf("\n********************************");

        printf("\nNome: ");
        fgets(animal->nomeAnimal, sizeof(animal->nomeAnimal), stdin);
        animal->nomeAnimal[strcspn(animal->nomeAnimal, "\n")] = '\0';

        printf("\nData de nascimento (ex:10-06-2025): ");
        fgets(animal->data_nascimento, sizeof(animal->data_nascimento), stdin);
        animal->data_nascimento[strcspn(animal->data_nascimento, "\n")] = '\0';

        printf("\nPeso: ");
        scanf("%lf", &animal->peso);
        limparBuffer(); 

        printf("\nCor: ");
        fgets(animal->cor, sizeof(animal->cor), stdin);
        animal->cor[strcspn(animal->cor, "\n")] = '\0';

        printf("\nEspecie: ");
        fgets(animal->especie, sizeof(animal->especie), stdin);
        animal->especie[strcspn(animal->especie, "\n")] = '\0';

        printf("\nObservação: ");
        fgets(animal->observacao, sizeof(animal->observacao), stdin);
        animal->observacao[strcspn(animal->observacao, "\n")] = '\0';

        printf("ID do Dono (Cliente): ");
        scanf("%d", &animal->idCliente);
        limparBuffer();

        // Tela de confirmação do animal
        system("clear");
        printf("\n=============================================");
        printf("\n   CONFERÊNCIA DOS DADOS DO ANIMAL");
        printf("\n=============================================");
        printf("\nID Animal:      %d", animal->id);
        printf("\nNome do Animal: %s", animal->nomeAnimal);
        printf("\nEspécie:        %s", animal->especie);
        printf("\nCor:            %s", animal->cor);
        printf("\nNascimento:     %s", animal->data_nascimento);
        printf("\nPeso:           %.2lf kg", animal->peso);
        printf("\nID do Dono:     %d", animal->idCliente);
        printf("\n---------------------------------------------");
        printf("\nObservações:    %s", animal->observacao);
        printf("\n=============================================\n");
}
 
void cadastroFuncionario(funcionario *funcionario, int novoId){
        int tipoFuncionario = 0;
        system("clear");
        funcionario->id = novoId; // Atribui ID auto-incremental

        printf("Deseja cadastrar um veterinario?\n(1) SIM\n(2) NÃO\nOpção: ");
        scanf("%d", &tipoFuncionario);
        limparBuffer();

        if (tipoFuncionario == 1) {
            printf("\n*************************************");
            printf("\n* Entre com os dados do Veterinario *");
            printf("\n* ID GERADO: %d                      *", funcionario->id);
            printf("\n*************************************");
        } else {
            printf("\n*************************************");
            printf("\n* Entre com os dados do Funcionario *");
            printf("\n* ID GERADO: %d                      *", funcionario->id);
            printf("\n*************************************");
        }

        printf("\nPrimeiro nome: ");
        fgets(funcionario->nomeFuncionario, sizeof(funcionario->nomeFuncionario), stdin);
        funcionario->nomeFuncionario[strcspn(funcionario->nomeFuncionario, "\n")] = '\0';

        printf("\nSobrenome: ");
        fgets(funcionario->sobrenomeFuncionario, sizeof(funcionario->sobrenomeFuncionario), stdin);
        funcionario->sobrenomeFuncionario[strcspn(funcionario->sobrenomeFuncionario, "\n")] = '\0';

        printf("\nCPF: ");
        fgets(funcionario->cpf_Funcionario, sizeof(funcionario->cpf_Funcionario), stdin);
        funcionario->cpf_Funcionario[strcspn(funcionario->cpf_Funcionario, "\n")] = '\0';

        // Coleta de dados exclusivos para Veterinários
        if (tipoFuncionario == 1) {
            printf("\nCRMV: ");
            fgets(funcionario->CRMV_Veterinario, sizeof(funcionario->CRMV_Veterinario), stdin);
            funcionario->CRMV_Veterinario[strcspn(funcionario->CRMV_Veterinario, "\n")] = '\0';
        } else {
            strcpy(funcionario->CRMV_Veterinario, "N/A");
        }

        printf("\nData admissão: ");
        fgets(funcionario->data_Admissao, sizeof(funcionario->data_Admissao), stdin);
        funcionario->data_Admissao[strcspn(funcionario->data_Admissao, "\n")] = '\0';

        // Validação básica do Status do Funcionário
        int status = 0;
        do {
            printf("\nStatus (ATIVO/INATIVO) em maiusculo: ");
            fgets(funcionario->status_Funcionario, sizeof(funcionario->status_Funcionario), stdin);
            funcionario->status_Funcionario[strcspn(funcionario->status_Funcionario, "\n")] = '\0';
            if (strcmp(funcionario->status_Funcionario, "ATIVO") == 0 || strcmp(funcionario->status_Funcionario, "INATIVO") == 0)
                status = 1;

        } while (status == 0);

        if (tipoFuncionario == 1) {
            printf("\nFormação: ");
            fgets(funcionario->formacao_Veterinario, sizeof(funcionario->formacao_Veterinario), stdin);
            funcionario->formacao_Veterinario[strcspn(funcionario->formacao_Veterinario, "\n")] = '\0';
        } else {
            strcpy(funcionario->formacao_Veterinario, "N/A");
        }

        printf("ID do Cargo: ");
        scanf("%d", &funcionario->cargo_id);
        limparBuffer();

        printf("ID do Departamento: ");
        scanf("%d", &funcionario->departamento_id);
        limparBuffer();

        // Tela de confirmação do funcionário/veterinário
        system("clear");
        printf("\n=============================================");
        printf("\n   CONFERÊNCIA DOS DADOS DO FUNCIONÁRIO");
        printf("\n=============================================");
        printf("\nID Funcionário:  %d", funcionario->id);
        printf("\nNome Completo:   %s %s", funcionario->nomeFuncionario, funcionario->sobrenomeFuncionario);
        printf("\nCPF:             %s", funcionario->cpf_Funcionario);
        printf("\nData Admissão:   %s", funcionario->data_Admissao);
        printf("\nStatus:          %s", funcionario->status_Funcionario);
        printf("\nID Cargo:        %d", funcionario->cargo_id);
        printf("\nID Departamento: %d", funcionario->departamento_id);
        if (tipoFuncionario == 1) {
            printf("\n---------------------------------------------");
            printf("\n[DADOS EXTRAS DO VETERINÁRIO]");
            printf("\nCRMV:     %s", funcionario->CRMV_Veterinario);
            printf("\nFormação: %s", funcionario->formacao_Veterinario);
        }
        printf("\n=============================================\n");
}
 
void cadastroCargo(cargo *cargo, int novoId){
        system("clear");
        cargo->id = novoId; // Atribui ID auto-incremental

        printf("\n*********************************");
        printf("\n* ENTRE COM OS DADOS DO CARGO  *");
        printf("\n* ID GERADO: %d                  *", cargo->id);
        printf("\n*********************************");

        printf("\nNome: ");
        fgets(cargo->nomeCargo, sizeof(cargo->nomeCargo), stdin);
        cargo->nomeCargo[strcspn(cargo->nomeCargo, "\n")] = '\0';

        printf("\nSalario: ");
        scanf("%lf", &cargo->salarioCargo);
        limparBuffer();

        system("clear");
        printf("\n=============================================");
        printf("\n   CONFERÊNCIA DOS DADOS DO CARGO");
        printf("\n=============================================");
        printf("\nID Cargo:      %d", cargo->id);
        printf("\nNome do cargo: %s", cargo->nomeCargo);
        printf("\nSalario:       %.2lf", cargo->salarioCargo);
        printf("\n=============================================\n");
}
 
void cadastroDepartamento(departamento *departamento, int novoId){
        system("clear");
        departamento->id = novoId; // Atribui ID auto-incremental

        printf("\n****************************************");
        printf("\n* ENTRE COM OS DADOS DO DEPARTAMENTO  *");
        printf("\n* ID GERADO: %d                         *", departamento->id);
        printf("\n****************************************");

        printf("\nNome: ");
        fgets(departamento->nomeDepartamento, sizeof(departamento->nomeDepartamento), stdin);
        departamento->nomeDepartamento[strcspn(departamento->nomeDepartamento, "\n")] = '\0';

        printf("\nDescrição: ");
        fgets(departamento->descricaoDepartamento, sizeof(departamento->descricaoDepartamento), stdin);
        departamento->descricaoDepartamento[strcspn(departamento->descricaoDepartamento, "\n")] = '\0';

        system("clear");
        printf("\n=============================================");
        printf("\n   CONFERÊNCIA DOS DADOS DO DEPARTAMENTO");
        printf("\n=============================================");
        printf("\nID Depto:      %d", departamento->id);
        printf("\nNome do depto: %s", departamento->nomeDepartamento);
        printf("\nDescrição:     %s", departamento->descricaoDepartamento);
        printf("\n=============================================\n");
}
 
void cadastroProduto(produto *produto, int novoId){
        system("clear");
        produto->id = novoId; // Atribui ID auto-incremental

        printf("\n*********************************");
        printf("\n* ENTRE COM OS DADOS DO PRODUTO *");
        printf("\n* ID GERADO: %d                  *", produto->id);
        printf("\n*********************************");

        printf("\nNome: ");
        fgets(produto->nomeProduto, sizeof(produto->nomeProduto), stdin);
        produto->nomeProduto[strcspn(produto->nomeProduto, "\n")] = '\0';

        printf("\nDescrição: ");
        fgets(produto->descricaoProduto, sizeof(produto->descricaoProduto), stdin);
        produto->descricaoProduto[strcspn(produto->descricaoProduto, "\n")] = '\0';

        printf("\nMarca: ");
        fgets(produto->marcaProduto, sizeof(produto->marcaProduto), stdin);
        produto->marcaProduto[strcspn(produto->marcaProduto, "\n")] = '\0';

        printf("\nCategoria: ");
        fgets(produto->categoriaProduto, sizeof(produto->categoriaProduto), stdin);
        produto->categoriaProduto[strcspn(produto->categoriaProduto, "\n")] = '\0';

        printf("\nPreço: ");
        scanf("%lf", &produto->precoUniProduto);
        limparBuffer();

        printf("\nQuantidade (estoque): ");
        scanf("%d", &produto->quantidade_Estq_Produto);
        limparBuffer();

        printf("\nId fornecedor: ");
        scanf("%d", &produto->IdFornecedor);
        limparBuffer();

        system("clear");
        printf("\n=============================================");
        printf("\n   CONFERÊNCIA DOS DADOS DO PRODUTO");
        printf("\n=============================================");
        printf("\nID Produto:  %d", produto->id);
        printf("\nNome:        %s", produto->nomeProduto);
        printf("\nMarca:       %s", produto->marcaProduto);
        printf("\nCategoria:   %s", produto->categoriaProduto);
        printf("\nPreço Unit.: R$ %.2lf", produto->precoUniProduto);
        printf("\nEstoque Qtd: %d", produto->quantidade_Estq_Produto);
        printf("\nID Fornec.:  %d", produto->IdFornecedor);
        printf("\n---------------------------------------------");
        printf("\nDescrição:   %s", produto->descricaoProduto);
        printf("\n=============================================\n");
}
 
void cadastroServico(servico *servico, int novoId){
        system("clear");
        servico->id = novoId; // Atribui ID auto-incremental

        printf("\n*********************************");
        printf("\n* ENTRE COM OS DADOS DO SERVIÇO *");
        printf("\n* ID GERADO: %d                  *", servico->id);
        printf("\n*********************************");

        printf("\nNome: ");
        fgets(servico->nomeServico, sizeof(servico->nomeServico), stdin);
        servico->nomeServico[strcspn(servico->nomeServico, "\n")] = '\0';

        printf("\nDescrição: ");
        fgets(servico->descricaoServico, sizeof(servico->descricaoServico), stdin);
        servico->descricaoServico[strcspn(servico->descricaoServico, "\n")] = '\0';

        printf("\nPreço: ");
        scanf("%lf", &servico->precoServico);
        limparBuffer();

        system("clear");
        printf("\n=============================================");
        printf("\n   CONFERÊNCIA DOS DADOS DO SERVIÇO");
        printf("\n=============================================");
        printf("\nID Serviço:      %d", servico->id);
        printf("\nNome do Serviço: %s", servico->nomeServico);
        printf("\nPreço Cobrado:   R$ %.2lf", servico->precoServico);
        printf("\n---------------------------------------------");
        printf("\nDescrição:       %s", servico->descricaoServico);
        printf("\n=============================================\n");
}
 
// ============================================================
// FUNÇÕES DE LEITURA (CARREGAR ARQUIVOS TEXTO PARA A MEMÓRIA)
// Utiliza sscanf com expressões regulares básicas ([^|]) para parsing
// ============================================================
 
void carregarCliente() {
    FILE *arquivo = fopen("clientes.txt", "r");
    if (arquivo == NULL) return; 
    char linha[1000];
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        sscanf(linha,
            "%d|%49[^|]|%49[^|]|%14[^|]|%99[^|]|%99[^|]|%9[^|]|%10[^|]|%49[^|]|%49[^|]|%4[^|]|%5[^|]|%19[^|]|%49[^|]|%19[^\n]",
            &clientes[total_clientes].id,
            clientes[total_clientes].nomeCliente,
            clientes[total_clientes].sobrenomeCliente,
            clientes[total_clientes].cpf_Cliente,
            clientes[total_clientes].email_Cliente,
            clientes[total_clientes].logradouro_Cliente,
            clientes[total_clientes].numeroResid_Cliente,
            clientes[total_clientes].cep_Cliente,
            clientes[total_clientes].bairro_Cliente,
            clientes[total_clientes].cidade_cliente,
            clientes[total_clientes].estadoUF_cliente,
            clientes[total_clientes].ddd_Tele_Cliente,
            clientes[total_clientes].numero_tel_Cliente,
            clientes[total_clientes].tipo_telefone,
            clientes[total_clientes].data_cadastro_Cliente);
        total_clientes++; 
    }
    fclose(arquivo);
}
 
void carregarAnimais() {
    FILE *arquivo = fopen("animais.txt", "r");
    if (arquivo == NULL) return;
    char linha[2000];
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        sscanf(linha,
            "%d|%99[^|]|%19[^|]|%49[^|]|%lf|%99[^|]|%99[^|]|%99[^|]|%499[^|]|%d",
            &animais[total_animais].id,
            animais[total_animais].nomeAnimal,
            animais[total_animais].data_nascimento,
            animais[total_animais].sexo,
            &animais[total_animais].peso,
            animais[total_animais].raca,
            animais[total_animais].cor,
            animais[total_animais].especie,
            animais[total_animais].observacao,
            &animais[total_animais].idCliente);
        total_animais++;
    }
    fclose(arquivo);
}
 
void carregarFuncionarios() {
    FILE *arquivo = fopen("funcionarios.txt", "r");
    if (arquivo == NULL) return;
    char linha[1000];
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        sscanf(linha,
            "%d|%49[^|]|%49[^|]|%14[^|]|%19[^|]|%19[^|]|%9[^|]|%99[^|]|%d|%d",
            &funcionarios[total_funcionarios].id,
            funcionarios[total_funcionarios].nomeFuncionario,
            funcionarios[total_funcionarios].sobrenomeFuncionario,
            funcionarios[total_funcionarios].cpf_Funcionario,
            funcionarios[total_funcionarios].CRMV_Veterinario,
            funcionarios[total_funcionarios].data_Admissao,
            funcionarios[total_funcionarios].status_Funcionario,
            funcionarios[total_funcionarios].formacao_Veterinario,
            &funcionarios[total_funcionarios].cargo_id,
            &funcionarios[total_funcionarios].departamento_id);
        total_funcionarios++;
    }
    fclose(arquivo);
}
 
void carregarCargos() {
    FILE *arquivo = fopen("cargos.txt", "r");
    if (arquivo == NULL) return;
    char linha[200];
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        sscanf(linha, "%d|%99[^|]|%lf",
            &cargos[total_cargos].id,
            cargos[total_cargos].nomeCargo,
            &cargos[total_cargos].salarioCargo);
        total_cargos++;
    }
    fclose(arquivo);
}
 
void carregarDepartamentos() {
    FILE *arquivo = fopen("departamentos.txt", "r");
    if (arquivo == NULL) return;
    char linha[600];
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        sscanf(linha, "%d|%99[^|]|%499[^\n]",
            &departamentos[total_departamentos].id,
            departamentos[total_departamentos].nomeDepartamento,
            departamentos[total_departamentos].descricaoDepartamento);
        total_departamentos++;
    }
    fclose(arquivo);
}
 
void carregarProdutos() {
    FILE *arquivo = fopen("produtos.txt", "r");
    if (arquivo == NULL) return;
    char linha[1000];
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        sscanf(linha,
            "%d|%99[^|]|%499[^|]|%99[^|]|%49[^|]|%lf|%d|%d",
            &produtos[total_produtos].id,
            produtos[total_produtos].nomeProduto,
            produtos[total_produtos].descricaoProduto,
            produtos[total_produtos].marcaProduto,
            produtos[total_produtos].categoriaProduto,
            &produtos[total_produtos].precoUniProduto,
            &produtos[total_produtos].quantidade_Estq_Produto,
            &produtos[total_produtos].IdFornecedor);
        total_produtos++;
    }
    fclose(arquivo);
}
 
void carregarServicos() {
    FILE *arquivo = fopen("servicos.txt", "r");
    if (arquivo == NULL) return;
    char linha[700];
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        sscanf(linha, "%d|%99[^|]|%499[^|]|%lf",
            &servicos[total_servicos].id,
            servicos[total_servicos].nomeServico,
            servicos[total_servicos].descricaoServico,
            &servicos[total_servicos].precoServico);
        total_servicos++;
    }
    fclose(arquivo);
}
 
//=============================================================
// FUNÇÕES DE LISTAGEM / RELATÓRIOS
// Percorrem os arrays globais exibindo os dados formatados na tela
//=============================================================

void listarCliente(){
    system("clear");
    printf("\n=============================================");
    printf("\n             CLIENTES CADASTRADOS");
    printf("\n=============================================\n");

    for (int i = 0; i < total_clientes; i++) {
        printf("=======================================================\n");
        printf("\nID: %d | NOME COMPLETO: %s %s\n", clientes[i].id, clientes[i].nomeCliente, clientes[i].sobrenomeCliente);
        printf("=======================================================\n");
        printf("\nCPF:           %s", clientes[i].cpf_Cliente);
        printf("\nEmail:         %s", clientes[i].email_Cliente);
        printf("\n---------------------------------------------");
        printf("\nEndereço:      %s, Nº %s", clientes[i].logradouro_Cliente, clientes[i].numeroResid_Cliente);
        printf("\nBairro:        %s", clientes[i].bairro_Cliente);
        printf("\nCidade/UF:     %s - %s", clientes[i].cidade_cliente, clientes[i].estadoUF_cliente);
        printf("\nCEP:           %s", clientes[i].cep_Cliente);
        printf("\n---------------------------------------------");
        printf("\nTelefone:      (%s) %s", clientes[i].ddd_Tele_Cliente, clientes[i].numero_tel_Cliente);
        printf("\nData Cadastro: %s", clientes[i].data_cadastro_Cliente);
        printf("\n=============================================\n");
    }
    printf("\n");
    printf("Dê um enter para retornar ao menu anterior!\n");
    getchar(); 
}

void listarAnimal(){
    system("clear");
    printf("\n=============================================");
    printf("\n           ANIMAIS CADASTRADOS");
    printf("\n=============================================\n");

    for (int i = 0; i < total_animais; i++)
    {
        printf("\n=============================================");
        printf("\nID: %d | NOME DO ANIMAL: %s", animais[i].id, animais[i].nomeAnimal);
        printf("\n=============================================");
        printf("\nEspécie:        %s", animais[i].especie);
        printf("\nCor:            %s", animais[i].cor);
        printf("\nNascimento:     %s", animais[i].data_nascimento);
        printf("\nPeso:           %.2lf kg", animais[i].peso);
        printf("\nID do Dono:     %d", animais[i].idCliente);
        printf("\n---------------------------------------------");
        printf("\nObservações:    %s", animais[i].observacao);
        printf("\n=============================================\n");
    }

    printf("\n");
    printf("Dê um enter para retornar ao menu anterior!\n");
    getchar();
}

void listarFuncionario(){
    system("clear");
    printf("\n=============================================");
    printf("\n         FUNCIONÁRIOS CADASTRADOS");
    printf("\n=============================================\n");

    for (int i = 0; i < total_funcionarios; i++)
    {
        printf("\n=============================================");
        printf("\nID: %d | Nome Completo:   %s %s", funcionarios[i].id, funcionarios[i].nomeFuncionario, funcionarios[i].sobrenomeFuncionario);
        printf("\n=============================================");
        printf("\nCPF:             %s", funcionarios[i].cpf_Funcionario);
        printf("\nData Admissão:   %s", funcionarios[i].data_Admissao);
        printf("\nStatus:          %s", funcionarios[i].status_Funcionario);
        printf("\nID Cargo:        %d", funcionarios[i].cargo_id);
        printf("\nID Departamento: %d", funcionarios[i].departamento_id);
        printf("\n---------------------------------------------");
        printf("\n[DADOS EXTRAS DO VETERINÁRIO]");
        printf("\nCRMV:     %s", funcionarios[i].CRMV_Veterinario);
        printf("\nFormação: %s", funcionarios[i].formacao_Veterinario);
        printf("\n=============================================\n");
    }

    printf("\n");
    printf("Dê um enter para retornar ao menu anterior!\n");
    getchar();
}

void listarCargo(){
    system("clear");
    printf("\n=============================================");
    printf("\n            CARGOS CADASTRADOS");
    printf("\n=============================================\n");

    for (int i = 0; i < total_cargos; i++)
    {
        printf("\n=============================================");
        printf("\nID: %d | Nome do cargo: %s", cargos[i].id, cargos[i].nomeCargo);
        printf("\n=============================================");
        printf("\nSalario:       R$ %.2lf", cargos[i].salarioCargo);
        printf("\n=============================================\n");  
    }
    
    printf("\n");
    printf("Dê um enter para retornar ao menu anterior!\n");
    getchar();
}

void listarDepartamentos(){
    system("clear");
    printf("\n=============================================");
    printf("\n          DEPARTAMENTOS CADASTRADOS");
    printf("\n=============================================\n");

    for (int i = 0; i < total_departamentos; i++)
    {
        printf("\n=============================================");
        printf("\nID: %d | Nome do depto: %s", departamentos[i].id, departamentos[i].nomeDepartamento);
        printf("\n=============================================");
        printf("\nDescrição:     %s", departamentos[i].descricaoDepartamento);
        printf("\n=============================================\n");
    }

    printf("\n");
    printf("Dê um enter para retornar ao menu anterior!\n");
    getchar();
}
       
void listarProdutos(){
    system("clear");
    printf("\n=============================================");
    printf("\n            PRODUTOS CADASTRADOS");
    printf("\n=============================================\n");

    for (int i = 0; i < total_produtos; i++)
    {
        printf("\n=============================================");
        printf("\nID: %d | Nome:        %s", produtos[i].id, produtos[i].nomeProduto);
        printf("\n=============================================");
        printf("\nMarca:       %s", produtos[i].marcaProduto);
        printf("\nCategoria:   %s", produtos[i].categoriaProduto);
        printf("\nPreço Unit.: R$ %.2lf", produtos[i].precoUniProduto);
        printf("\nEstoque Qtd: %d", produtos[i].quantidade_Estq_Produto);
        printf("\nID Fornec.:  %d", produtos[i].IdFornecedor);
        printf("\n---------------------------------------------");
        printf("\nDescrição:   %s", produtos[i].descricaoProduto);
        printf("\n=============================================\n");
    }

    printf("\n");
    printf("Dê um enter para retornar ao menu anterior!\n");
    getchar();
}

void listarServicos(){
    system("clear");
    printf("\n=============================================");
    printf("\n             SERVIÇOS CADASTRADOS");
    printf("\n=============================================\n");

    for (int i = 0; i < total_servicos; i++)
    {
        printf("\n=============================================");
        printf("\nID: %d | Nome do Serviço: %s", servicos[i].id, servicos[i].nomeServico);
        printf("\n=============================================");
        printf("\nPreço Cobrado:   R$ %.2lf", servicos[i].precoServico);
        printf("\n---------------------------------------------");
        printf("\nDescrição:       %s", servicos[i].descricaoServico);
        printf("\n=============================================\n");
    }
    printf("\n");
    printf("Dê um enter para retornar ao menu anterior!\n");
    getchar(); 
}

// ============================================================
// INTERFACE DO USUÁRIO (TELAS, LOGO E MENUS)
// ============================================================
 
// Exibe o logo em ASCII art do Mundo Pet
void logo(){
    printf("============================\n");
    printf(" ######## Mundo pet ####### \n");
    printf("============================\n");
    printf("   /\\_/\\     __   __  \n");
    printf("  ( o.o )   /  \\_/  \\ \n");
    printf("   > ^ <   (   o.o  )  \n");
    printf("  /     \\   \\_    _/   \n");
    printf(" |       |   /     \\    \n");
    printf(" (__/\\___)  (__/\\___) \n\n");
}
 
// Submenu responsável por gerenciar novos Cadastros
void menuCadastro(){
    int escolha = -1;
    do
    {
        system("clear");
        printf("=============================\n");
        printf(" ######### CADASTRO ######## \n");
        printf("=============================\n");
        printf("****************************\n");
        printf("* O QUE VOCE DESEJA FAZER? *\n");
        printf("****************************\n");
        printf("(1) Cadastrar cliente\n");
        printf("(2) Cadastrar animal\n");
        printf("(3) Cadastrar funcionario\n");
        printf("(4) Cadastrar cargo\n");
        printf("(5) Cadastrar Departamento\n");
        printf("(6) Cadastrar produto\n");
        printf("(7) Cadastrar serviço oferecido\n");
        printf("(0) Voltar\n");
        printf("Entre com a sua escolha: ");
        scanf("%d", &escolha);
        limparBuffer();

        switch (escolha) {
            case 1:
                // O ID será a posição atual + 1 (Iniciando em 1)
                cadastroCliente(&clientes[total_clientes], total_clientes + 1);
                total_clientes++;
                break;
            case 2:
                cadastroAnimal(&animais[total_animais], total_animais + 1);
                total_animais++;
                break;
            case 3:
                cadastroFuncionario(&funcionarios[total_funcionarios], total_funcionarios + 1);
                total_funcionarios++;
                break;
            case 4:
                cadastroCargo(&cargos[total_cargos], total_cargos + 1);
                total_cargos++;
                break;
            case 5:
                cadastroDepartamento(&departamentos[total_departamentos], total_departamentos + 1);
                total_departamentos++;
                break;
            case 6:
                cadastroProduto(&produtos[total_produtos], total_produtos + 1);
                total_produtos++;
                break;
            case 7:
                cadastroServico(&servicos[total_servicos], total_servicos + 1);
                total_servicos++;
                break;
            case 0:
                break; 
            default:
                printf("\nOpção inválida! Aperte ENTER para tentar novamente...");
                getchar();
                break;
        } 
    } while (escolha != 0);

    // Salva todas as atualizações nos arquivos locais ao sair do menu de cadastro
    salvarCliente();
    salvarAnimais();
    salvarFuncionarios();
    salvarCargos();
    salvarDepartamentos();
    salvarProdutos();
    salvarServicos();
}

// Submenu responsável por gerenciar a exibição/listagem dos registros
void menuListar(){
    int escolha = -1;
    do {
        system("clear");
        printf("**************************\n");
        printf("* O QUE VOCE QUER FAZER? *\n");
        printf("**************************\n");
        printf("(1) Listar clientes\n");
        printf("(2) Listar animais\n");
        printf("(3) Listar funcionarios\n");
        printf("(4) Listar cargos\n");
        printf("(5) Listar Departamentos\n");
        printf("(6) Listar produtos\n");
        printf("(7) Listar serviços oferecidos\n");
        printf("(0) Voltar\n");
        printf("Entre com a sua escolha: ");
        scanf("%d", &escolha);
        limparBuffer();
    
        if (escolha == 0){
             return; 
        }
 
        switch (escolha) {
            case 1:
                listarCliente();
                break;
            case 2:
                listarAnimal();
                break;
            case 3:
                listarFuncionario();
                break;
            case 4:
                listarCargo();
                break;
            case 5:
                listarDepartamentos();
                break;
            case 6:
                listarProdutos();
                break;
            case 7:
                listarServicos();
                break;
            default:
                printf("\nOpção inválida! Aperte ENTER para tentar novamente...");
                getchar();
                break;
        }
    } while (escolha != 0);
}

// ============================================================
// FUNÇÃO PRINCIPAL (PONTO DE ENTRADA DO SISTEMA)
// ============================================================
 
int main() {
    system("clear");
    
    // Carrega os arquivos salvos em disco para a memória RAM assim que o software inicia
    carregarCliente();
    carregarAnimais();
    carregarFuncionarios();
    carregarCargos();
    carregarDepartamentos();
    carregarProdutos();
    carregarServicos();
    
    int escolha = -1;
 
    // Loop de controle do menu principal
    do {
        if (escolha == 0) break;
        
        logo();
        printf("=================================\n");
        printf(" ######## MENU PRINCIPAL ####### \n");
        printf("=================================\n");
        printf("****************************\n");
        printf("* O QUE VOCE DESEJA FAZER? *\n");
        printf("****************************\n");
        printf("(0) Sair\n");
        printf("(1) Cadastro\n");
        printf("(2) Listar cadastro\n");
        printf("Opção: ");
        scanf("%d", &escolha);
        limparBuffer();

        switch (escolha)
        {
            case 1:
                menuCadastro(); 
                break;
            case 2:
                menuListar();    
                break;
            case 0:
                break;
            default:
                printf("\nOpção inválida! Aperte ENTER para tentar novamente...");
                getchar();
                break;
        }
        
    } while (escolha != 0);
 
    printf("\nObrigado por usar o sistema Mundo Pet! Até logo.\n");
    return 0;
}