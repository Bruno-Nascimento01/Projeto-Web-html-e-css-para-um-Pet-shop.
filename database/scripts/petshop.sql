-- =============================================================
-- Projeto: Banco de Dados - Pet Shop
-- Autor: Bruno Nascimento Ribeiro Oliveira
-- Membros do grupo: 
-- 1- Bruno Nascimento Ribeiro Oliveira (Lider)
-- 2-Keith Leandra Carvalho da Silva 
-- 3- Lucas de Melo Eduardo 
-- 4-Kaique de Oliveira Silva 
-- 5-Otavio Santos Lidorio 
-- 6-Davi Sousa Santana 
-- 7-kayk Carvalho Andrade 
-- Descrição: Script de criação do banco de dados do Pet Shop,
--            incluindo todas as tabelas, chaves primárias,
--            chaves estrangeiras e constraints.
-- =============================================================

SET FOREIGN_KEY_CHECKS = 0;

-- -------------------------------------------------------------
-- Criação do banco de dados
-- -------------------------------------------------------------
DROP DATABASE IF EXISTS Pet_shop;
CREATE DATABASE IF NOT EXISTS Pet_shop DEFAULT CHARACTER SET utf8mb4;
USE Pet_shop;

-- =============================================================
-- NÍVEL 1 - Tabelas sem dependências (sem FK)
-- =============================================================

-- -------------------------------------------------------------
-- Tabela: Departamento
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Departamento;
CREATE TABLE Departamento (
    idDepartamento      INT          NOT NULL AUTO_INCREMENT,
    nome_departamento   VARCHAR(60)  NOT NULL,
    descricao           VARCHAR(500) NULL,
    PRIMARY KEY (idDepartamento)
);

-- -------------------------------------------------------------
-- Tabela: Cargo
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Cargo;
CREATE TABLE Cargo (
    idCargo     INT          NOT NULL AUTO_INCREMENT,
    nome_cargo  VARCHAR(90)  NOT NULL,
    salario     DECIMAL(10,2) NOT NULL,
    PRIMARY KEY (idCargo)
);

-- -------------------------------------------------------------
-- Tabela: Cliente
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Cliente;
CREATE TABLE Cliente (
    idCliente           INT          NOT NULL AUTO_INCREMENT,
    nome_cliente        VARCHAR(50)  NOT NULL,
    sobrenome_cliente   VARCHAR(50)  NOT NULL,
    email               VARCHAR(100) NOT NULL,
    data_cadastro       DATE         NOT NULL,
    PRIMARY KEY (idCliente)
);

-- -------------------------------------------------------------
-- Tabela: Servico
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Servico;
CREATE TABLE Servico (
    idServico       INT           NOT NULL AUTO_INCREMENT,
    nome_servico    VARCHAR(100)  NOT NULL,
    descricao       VARCHAR(500)  NOT NULL,
    preco           DECIMAL(10,2) NOT NULL,
    PRIMARY KEY (idServico)
);

-- -------------------------------------------------------------
-- Tabela: Produto
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Produto;
CREATE TABLE Produto (
    idProduto           INT           NOT NULL AUTO_INCREMENT,
    nome_produto        VARCHAR(200)  NOT NULL,
    descricao           TEXT          NOT NULL,
    marca               VARCHAR(100)  NULL,
    categoria           VARCHAR(50)   NULL,
    preco_unitario      DECIMAL(10,2) NOT NULL,
    quantidade_estoque  INT           NOT NULL,
    PRIMARY KEY (idProduto)
);

-- -------------------------------------------------------------
-- Tabela: Fornecedor
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Fornecedor;
CREATE TABLE Fornecedor (
    idFornecedor        INT          NOT NULL AUTO_INCREMENT,
    nome_fornecedor     VARCHAR(100) NOT NULL,
    cnpj                CHAR(18)     NOT NULL,
    email               VARCHAR(100) NOT NULL,
    PRIMARY KEY (idFornecedor)
);

-- -------------------------------------------------------------
-- Tabela: Historico_pet
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Historico_pet;
CREATE TABLE Historico_pet (
    idHistorico_pet INT  NOT NULL AUTO_INCREMENT,
    descricao       TEXT NOT NULL,
    data_registro   DATE NOT NULL,
    tipo_registro   ENUM('Consulta', 'Vacina', 'Cirurgia', 'Exame') NOT NULL,
    PRIMARY KEY (idHistorico_pet)
);

-- =============================================================
-- NÍVEL 2 - Tabelas que dependem do Nível 1
-- =============================================================

-- -------------------------------------------------------------
-- Tabela: Funcionario
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Funcionario;
CREATE TABLE Funcionario (
    idFuncionario           INT         NOT NULL AUTO_INCREMENT,
    cpf                     CHAR(11)    NOT NULL,
    nome_funcionario        VARCHAR(45) NOT NULL,
    sobrenome_funcionario   VARCHAR(50) NOT NULL,
    data_admissao           DATE        NOT NULL,
    status_funcionario      ENUM('Ativo', 'Inativo') NOT NULL,
    Cargo_idCargo           INT         NOT NULL,
    Departamento_idDepartamento INT     NOT NULL,
    PRIMARY KEY (idFuncionario),
    CONSTRAINT fk_Funcionario_Cargo
        FOREIGN KEY (Cargo_idCargo)
        REFERENCES Cargo (idCargo),
    CONSTRAINT fk_Funcionario_Departamento
        FOREIGN KEY (Departamento_idDepartamento)
        REFERENCES Departamento (idDepartamento)
);

-- -------------------------------------------------------------
-- Tabela: Animal
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Animal;
CREATE TABLE Animal (
    idAnimal            INT           NOT NULL AUTO_INCREMENT,
    data_nascimento     DATE          NOT NULL,
    nome_animal         VARCHAR(100)  NOT NULL,
    sexo                VARCHAR(45)   NOT NULL,
    peso                DECIMAL(10,2) NOT NULL,
    raca                VARCHAR(100)  NOT NULL,
    cor                 VARCHAR(100)  NOT NULL,
    especie             VARCHAR(100)  NOT NULL,
    observacao          TEXT          NOT NULL,
    Cliente_idCliente   INT           NOT NULL,
    PRIMARY KEY (idAnimal),
    CONSTRAINT fk_Animal_Cliente
        FOREIGN KEY (Cliente_idCliente)
        REFERENCES Cliente (idCliente)
);

-- -------------------------------------------------------------
-- Tabela: Creche
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Creche;
CREATE TABLE Creche (
    idCreche            INT         NOT NULL AUTO_INCREMENT,
    periodo             VARCHAR(45) NOT NULL,
    capacidade_maxima   INT         NOT NULL,
    Servico_idServico   INT         NOT NULL,
    PRIMARY KEY (idCreche),
    CONSTRAINT fk_Creche_Servico
        FOREIGN KEY (Servico_idServico)
        REFERENCES Servico (idServico)
);

-- -------------------------------------------------------------
-- Tabela: Estetica
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Estetica;
CREATE TABLE Estetica (
    idEstetica          INT          NOT NULL AUTO_INCREMENT,
    tipo                VARCHAR(200) NOT NULL,
    Servico_idServico   INT          NOT NULL,
    PRIMARY KEY (idEstetica),
    CONSTRAINT fk_Estetica_Servico
        FOREIGN KEY (Servico_idServico)
        REFERENCES Servico (idServico)
);

-- -------------------------------------------------------------
-- Tabela: Venda_faturamento
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Venda_faturamento;
CREATE TABLE Venda_faturamento (
    idVenda_faturamento     INT           NOT NULL AUTO_INCREMENT,
    data_venda              DATE          NOT NULL,
    valor_total             DECIMAL(10,2) NOT NULL,
    desconto                DECIMAL(10,2) NOT NULL,
    valor_final             DECIMAL(10,2) NOT NULL,
    status_venda            ENUM('Concluido', 'Aguardando') NOT NULL,
    Cliente_idCliente       INT           NOT NULL,
    PRIMARY KEY (idVenda_faturamento),
    CONSTRAINT fk_Venda_faturamento_Cliente
        FOREIGN KEY (Cliente_idCliente)
        REFERENCES Cliente (idCliente)
);

-- -------------------------------------------------------------
-- Tabela: Produto_Fornecido
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Produto_Fornecido;
CREATE TABLE Produto_Fornecido (
    Fornecedor_idFornecedor INT NOT NULL,
    Produto_idProduto       INT NOT NULL,
    PRIMARY KEY (Fornecedor_idFornecedor, Produto_idProduto),
    CONSTRAINT fk_Produto_Fornecido_Fornecedor
        FOREIGN KEY (Fornecedor_idFornecedor)
        REFERENCES Fornecedor (idFornecedor),
    CONSTRAINT fk_Produto_Fornecido_Produto
        FOREIGN KEY (Produto_idProduto)
        REFERENCES Produto (idProduto)
);

-- -------------------------------------------------------------
-- Tabela: Endereco_Cliente
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Endereco_Cliente;
CREATE TABLE Endereco_Cliente (
    idEndereco_Cliente  INT          NOT NULL AUTO_INCREMENT,
    logradouro          VARCHAR(100) NOT NULL,
    numero_rua          VARCHAR(10)  NOT NULL,
    bairro              VARCHAR(50)  NOT NULL,
    complemento         VARCHAR(50)  NOT NULL,
    cidade              VARCHAR(50)  NOT NULL,
    estado_UF           CHAR(2)      NOT NULL,
    cep                 CHAR(8)      NOT NULL,
    Cliente_idCliente   INT          NOT NULL,
    PRIMARY KEY (idEndereco_Cliente),
    CONSTRAINT fk_Endereco_Cliente_Cliente
        FOREIGN KEY (Cliente_idCliente)
        REFERENCES Cliente (idCliente)
);

-- -------------------------------------------------------------
-- Tabela: Telefone_cliente
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Telefone_cliente;
CREATE TABLE Telefone_cliente (
    idTelefone_cliente  INT         NOT NULL AUTO_INCREMENT,
    ddd_cliente         CHAR(4)     NOT NULL,
    numero_cliente      VARCHAR(20) NOT NULL,
    tipo                ENUM('Telefone', 'Celular') NOT NULL,
    Cliente_idCliente   INT         NOT NULL,
    PRIMARY KEY (idTelefone_cliente),
    CONSTRAINT fk_Telefone_cliente_Cliente
        FOREIGN KEY (Cliente_idCliente)
        REFERENCES Cliente (idCliente)
);

-- -------------------------------------------------------------
-- Tabela: Endereco_fornecedor
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Endereco_fornecedor;
CREATE TABLE Endereco_fornecedor (
    idEndereco_fornecedor   INT          NOT NULL AUTO_INCREMENT,
    logradouro              VARCHAR(100) NOT NULL,
    numero                  VARCHAR(10)  NOT NULL,
    complemento             VARCHAR(50)  NOT NULL,
    bairro                  VARCHAR(50)  NOT NULL,
    cidade                  VARCHAR(50)  NOT NULL,
    estado                  CHAR(2)      NOT NULL,
    cep                     CHAR(8)      NOT NULL,
    Fornecedor_idFornecedor INT          NOT NULL,
    PRIMARY KEY (idEndereco_fornecedor),
    CONSTRAINT fk_Endereco_fornecedor_Fornecedor
        FOREIGN KEY (Fornecedor_idFornecedor)
        REFERENCES Fornecedor (idFornecedor)
);

-- -------------------------------------------------------------
-- Tabela: Telefone_Fornecedor
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Telefone_Fornecedor;
CREATE TABLE Telefone_Fornecedor (
    idTelefone_Fornecedor   INT         NOT NULL AUTO_INCREMENT,
    ddd                     CHAR(4)     NOT NULL,
    numero                  VARCHAR(20) NOT NULL,
    tipo                    ENUM('Telefone', 'Celular') NOT NULL,
    Fornecedor_idFornecedor INT         NOT NULL,
    PRIMARY KEY (idTelefone_Fornecedor),
    CONSTRAINT fk_Telefone_Fornecedor_Fornecedor
        FOREIGN KEY (Fornecedor_idFornecedor)
        REFERENCES Fornecedor (idFornecedor)
);

-- =============================================================
-- NÍVEL 3 - Tabelas que dependem do Nível 2
-- =============================================================

-- -------------------------------------------------------------
-- Tabela: Veterinario
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Veterinario;
CREATE TABLE Veterinario (
    idVeterinario               INT         NOT NULL AUTO_INCREMENT,
    Crmv                        VARCHAR(20) NOT NULL,
    formacao                    VARCHAR(100) NOT NULL,
    Funcionario_idFuncionario   INT         NOT NULL,
    PRIMARY KEY (idVeterinario),
    CONSTRAINT fk_Veterinario_Funcionario
        FOREIGN KEY (Funcionario_idFuncionario)
        REFERENCES Funcionario (idFuncionario)
);

-- -------------------------------------------------------------
-- Tabela: Funcionario_Geral
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Funcionario_Geral;
CREATE TABLE Funcionario_Geral (
    idFuncionario_Geral         INT         NOT NULL AUTO_INCREMENT,
    setor_responsavel           VARCHAR(45) NOT NULL,
    data_treinamento            DATE        NOT NULL,
    Funcionario_idFuncionario   INT         NOT NULL,
    PRIMARY KEY (idFuncionario_Geral),
    CONSTRAINT fk_Funcionario_Geral_Funcionario
        FOREIGN KEY (Funcionario_idFuncionario)
        REFERENCES Funcionario (idFuncionario)
);

-- -------------------------------------------------------------
-- Tabela: Endereco_Funcionario
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Endereco_Funcionario;
CREATE TABLE Endereco_Funcionario (
    idEndereco_Funcionario      INT          NOT NULL AUTO_INCREMENT,
    logradouro                  VARCHAR(100) NOT NULL,
    numero                      VARCHAR(10)  NOT NULL,
    complemento                 VARCHAR(50)  NOT NULL,
    bairro                      VARCHAR(50)  NOT NULL,
    cidade                      VARCHAR(50)  NOT NULL,
    estado                      CHAR(2)      NOT NULL,
    cep                         CHAR(8)      NOT NULL,
    Funcionario_idFuncionario   INT          NOT NULL,
    PRIMARY KEY (idEndereco_Funcionario),
    CONSTRAINT fk_Endereco_Funcionario_Funcionario
        FOREIGN KEY (Funcionario_idFuncionario)
        REFERENCES Funcionario (idFuncionario)
);

-- -------------------------------------------------------------
-- Tabela: Telefone_funcionario
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Telefone_funcionario;
CREATE TABLE Telefone_funcionario (
    idTelefone_funcionario      INT         NOT NULL AUTO_INCREMENT,
    ddd                         CHAR(4)     NOT NULL,
    numero                      VARCHAR(20) NOT NULL,
    tipo                        ENUM('Telefone', 'Celular') NOT NULL,
    Funcionario_idFuncionario   INT         NOT NULL,
    PRIMARY KEY (idTelefone_funcionario),
    CONSTRAINT fk_Telefone_funcionario_Funcionario
        FOREIGN KEY (Funcionario_idFuncionario)
        REFERENCES Funcionario (idFuncionario)
);

-- -------------------------------------------------------------
-- Tabela: Agendamento
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Agendamento;
CREATE TABLE Agendamento (
    idAgendamento               INT      NOT NULL AUTO_INCREMENT,
    data_hora                   DATETIME NOT NULL,
    status_                     ENUM('Agendado', 'Em andamento', 'Cancelado') NOT NULL,
    observacao                  TEXT     NOT NULL,
    Veterinario_idVeterinario   INT      NOT NULL,
    Cliente_idCliente           INT      NOT NULL,
    Animal_idAnimal             INT      NOT NULL,
    PRIMARY KEY (idAgendamento),
    CONSTRAINT fk_Agendamento_Veterinario
        FOREIGN KEY (Veterinario_idVeterinario)
        REFERENCES Veterinario (idVeterinario),
    CONSTRAINT fk_Agendamento_Cliente
        FOREIGN KEY (Cliente_idCliente)
        REFERENCES Cliente (idCliente),
    CONSTRAINT fk_Agendamento_Animal
        FOREIGN KEY (Animal_idAnimal)
        REFERENCES Animal (idAnimal)
);

-- -------------------------------------------------------------
-- Tabela: Metodo_pagamento
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Metodo_pagamento;
CREATE TABLE Metodo_pagamento (
    idMetodo_pagamento                  INT           NOT NULL AUTO_INCREMENT,
    tipo                                VARCHAR(50)   NOT NULL,
    descricao                           VARCHAR(500)  NOT NULL,
    data_pagamento                      DATE          NOT NULL,
    valor_pago                          DECIMAL(10,2) NOT NULL,
    parcela                             DECIMAL(10,2) NOT NULL,
    Venda_faturamento_idVenda_faturamento INT         NOT NULL,
    PRIMARY KEY (idMetodo_pagamento),
    CONSTRAINT fk_Metodo_pagamento_Venda_faturamento
        FOREIGN KEY (Venda_faturamento_idVenda_faturamento)
        REFERENCES Venda_faturamento (idVenda_faturamento)
);

-- -------------------------------------------------------------
-- Tabela: Produto_vendido
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Produto_vendido;
CREATE TABLE Produto_vendido (
    Produto_idProduto                       INT NOT NULL,
    Venda_faturamento_idVenda_faturamento   INT NOT NULL,
    PRIMARY KEY (Produto_idProduto, Venda_faturamento_idVenda_faturamento),
    CONSTRAINT fk_Produto_vendido_Produto
        FOREIGN KEY (Produto_idProduto)
        REFERENCES Produto (idProduto),
    CONSTRAINT fk_Produto_vendido_Venda_faturamento
        FOREIGN KEY (Venda_faturamento_idVenda_faturamento)
        REFERENCES Venda_faturamento (idVenda_faturamento)
);

-- -------------------------------------------------------------
-- Tabela: Venda_Servico
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Venda_Servico;
CREATE TABLE Venda_Servico (
    Servico_idServico                       INT NOT NULL,
    Venda_faturamento_idVenda_faturamento   INT NOT NULL,
    PRIMARY KEY (Servico_idServico, Venda_faturamento_idVenda_faturamento),
    CONSTRAINT fk_Venda_Servico_Servico
        FOREIGN KEY (Servico_idServico)
        REFERENCES Servico (idServico),
    CONSTRAINT fk_Venda_Servico_Venda_faturamento
        FOREIGN KEY (Venda_faturamento_idVenda_faturamento)
        REFERENCES Venda_faturamento (idVenda_faturamento)
);

-- -------------------------------------------------------------
-- Tabela: Especialidade
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Especialidade;
CREATE TABLE Especialidade (
    idEspecialidade             INT          NOT NULL AUTO_INCREMENT,
    nome_especialidade          VARCHAR(200) NOT NULL,
    descricao                   TEXT         NOT NULL,
    Veterinario_idVeterinario   INT          NOT NULL,
    PRIMARY KEY (idEspecialidade),
    CONSTRAINT fk_Especialidade_Veterinario
        FOREIGN KEY (Veterinario_idVeterinario)
        REFERENCES Veterinario (idVeterinario)
);

-- =============================================================
-- NÍVEL 4 - Tabelas que dependem do Nível 3
-- =============================================================

-- -------------------------------------------------------------
-- Tabela: Consulta
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Consulta;
CREATE TABLE Consulta (
    idConsulta                  INT      NOT NULL AUTO_INCREMENT,
    data                        DATE     NULL,
    hora                        TIME     NOT NULL,
    motivo                      TEXT     NOT NULL,
    diagnostico                 TEXT     NOT NULL,
    tratamento                  TEXT     NOT NULL,
    observacao                  TEXT     NOT NULL,
    Animal_idAnimal             INT      NOT NULL,
    Veterinario_idVeterinario   INT      NOT NULL,
    Agendamento_idAgendamento   INT      NOT NULL,
    PRIMARY KEY (idConsulta),
    CONSTRAINT fk_Consulta_Animal
        FOREIGN KEY (Animal_idAnimal)
        REFERENCES Animal (idAnimal),
    CONSTRAINT fk_Consulta_Veterinario
        FOREIGN KEY (Veterinario_idVeterinario)
        REFERENCES Veterinario (idVeterinario),
    CONSTRAINT fk_Consulta_Agendamento
        FOREIGN KEY (Agendamento_idAgendamento)
        REFERENCES Agendamento (idAgendamento)
);

-- -------------------------------------------------------------
-- Tabela: Funcionario_servico
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Funcionario_servico;
CREATE TABLE Funcionario_servico (
    Funcionario_Geral_idFuncionario_Geral   INT  NOT NULL,
    Servico_idServico                       INT  NOT NULL,
    tempo_duracao                           INT  NOT NULL,
    data_atendimento                        DATE NOT NULL,
    PRIMARY KEY (Funcionario_Geral_idFuncionario_Geral, Servico_idServico),
    CONSTRAINT fk_Funcionario_servico_Funcionario_Geral
        FOREIGN KEY (Funcionario_Geral_idFuncionario_Geral)
        REFERENCES Funcionario_Geral (idFuncionario_Geral),
    CONSTRAINT fk_Funcionario_servico_Servico
        FOREIGN KEY (Servico_idServico)
        REFERENCES Servico (idServico)
);

-- -------------------------------------------------------------
-- Tabela: Cliente_Pagamento
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Cliente_Pagamento;
CREATE TABLE Cliente_Pagamento (
    Metodo_pagamento_idMetodo_pagamento INT NOT NULL,
    Cliente_idCliente                   INT NOT NULL,
    PRIMARY KEY (Metodo_pagamento_idMetodo_pagamento, Cliente_idCliente),
    CONSTRAINT fk_Cliente_Pagamento_Metodo_pagamento
        FOREIGN KEY (Metodo_pagamento_idMetodo_pagamento)
        REFERENCES Metodo_pagamento (idMetodo_pagamento),
    CONSTRAINT fk_Cliente_Pagamento_Cliente
        FOREIGN KEY (Cliente_idCliente)
        REFERENCES Cliente (idCliente)
);

-- =============================================================
-- NÍVEL 5 - Tabelas que dependem do Nível 4
-- =============================================================

-- -------------------------------------------------------------
-- Tabela: Log_Auditoria
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Log_Auditoria;
CREATE TABLE Log_Auditoria (
    idLog_Auditoria                     INT         NOT NULL AUTO_INCREMENT,
    acao                                VARCHAR(50) NOT NULL,
    tabela_afetada                      VARCHAR(50) NOT NULL,
    descricao                           TEXT        NOT NULL,
    Funcionario_Geral_idFuncionario_Geral INT       NOT NULL,
    PRIMARY KEY (idLog_Auditoria),
    CONSTRAINT fk_Log_Auditoria_Funcionario_Geral
        FOREIGN KEY (Funcionario_Geral_idFuncionario_Geral)
        REFERENCES Funcionario_Geral (idFuncionario_Geral)
);

-- -------------------------------------------------------------
-- Tabela: Prontuario_clinico
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Prontuario_clinico;
CREATE TABLE Prontuario_clinico (
    idProntuario_clinico            INT  NOT NULL AUTO_INCREMENT,
    data_abertura                   DATE NOT NULL,
    observacao                      TEXT NOT NULL,
    Historico_pet_idHistorico_pet   INT  NOT NULL,
    Consulta_idConsulta             INT  NOT NULL,
    PRIMARY KEY (idProntuario_clinico),
    CONSTRAINT fk_Prontuario_clinico_Historico_pet
        FOREIGN KEY (Historico_pet_idHistorico_pet)
        REFERENCES Historico_pet (idHistorico_pet),
    CONSTRAINT fk_Prontuario_clinico_Consulta
        FOREIGN KEY (Consulta_idConsulta)
        REFERENCES Consulta (idConsulta)
);

-- -------------------------------------------------------------
-- Tabela: Agendamento_servico
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Agendamento_servico;
CREATE TABLE Agendamento_servico (
    Agendamento_idAgendamento                               INT NOT NULL,
    Funcionario_servico_Funcionario_Geral_idFuncionario_Geral INT NOT NULL,
    Funcionario_servico_Servico_idServico                   INT NOT NULL,
    PRIMARY KEY (
        Agendamento_idAgendamento,
        Funcionario_servico_Funcionario_Geral_idFuncionario_Geral,
        Funcionario_servico_Servico_idServico
    ),
    CONSTRAINT fk_Agendamento_servico_Agendamento
        FOREIGN KEY (Agendamento_idAgendamento)
        REFERENCES Agendamento (idAgendamento),
    CONSTRAINT fk_Agendamento_servico_Funcionario_servico
        FOREIGN KEY (
            Funcionario_servico_Funcionario_Geral_idFuncionario_Geral,
            Funcionario_servico_Servico_idServico
        )
        REFERENCES Funcionario_servico (
            Funcionario_Geral_idFuncionario_Geral,
            Servico_idServico
        )
);

-- -------------------------------------------------------------
-- Tabela: Auditado
-- -------------------------------------------------------------
DROP TABLE IF EXISTS Auditado;
CREATE TABLE Auditado (
    Venda_faturamento_idVenda_faturamento   INT      NOT NULL,
    Log_Auditoria_idLog_Auditoria           INT      NOT NULL,
    data_hora                               DATETIME NOT NULL,
    PRIMARY KEY (Venda_faturamento_idVenda_faturamento, Log_Auditoria_idLog_Auditoria),
    CONSTRAINT fk_Auditado_Venda_faturamento
        FOREIGN KEY (Venda_faturamento_idVenda_faturamento)
        REFERENCES Venda_faturamento (idVenda_faturamento),
    CONSTRAINT fk_Auditado_Log_Auditoria
        FOREIGN KEY (Log_Auditoria_idLog_Auditoria)
        REFERENCES Log_Auditoria (idLog_Auditoria)
);

SET FOREIGN_KEY_CHECKS = 1;
