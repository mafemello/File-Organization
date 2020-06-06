/**
 * @brief Esse módulo contém as constantes usadas em todo o projeto 
 * 
 */


#ifndef CONSTANTS_H

#define CONSTANTS_H

#define ERROR '0'
#define STATUS_OK '1' 
#define BLANK '$'
#define MAX_LEN 150
#define TOKEN ","
#define EOF_TOKEN -2
#define EXCLUIDO -1
#define DISK_PAGE 128

typedef unsigned char bool;

#define TRUE 1
#define FALSE 0

/**
 * @brief essa estrutura é uma representação do cabeçalho do arquivo
 * 
 */
typedef struct header_{
    char status;
    int RRNproxRegistro;
    int numeroRegistrosInseridos;
    int numeroRegistrosRemovidos;
    int numeroRegistrosAtualizados;
    char lixo[111];
}Header;

/**
 * @brief essa estrutura é uma representação do registro de tamanho fixo
 * 
 */
typedef struct registro_{
    int cidadeMaeTamanho;
    int cidadeBebeTamanho;
    char cidadeMae[105];
    char cidadeBebe[105];
    int idNascimento;
    int idadeMae;
    char sexoBebe;
    char dataNascimento[11];
    char estadoMae[3];
    char estadoBebe[3];
    
}Registro;

/**
 * @brief essa struct representa um filtro de busca com o nome do campo e seu valor podendo assumir um valor inteiro ou string
 * 
 */
typedef struct query_pair_{
    char nomeDoCampo[MAX_LEN];
    union {
        int valor_numerico;
        char valor_string[MAX_LEN];
    }valor;
}QueryPair;

/**
 * @brief essa strut representa um filtro composto de mais de um campo, com uma lista de cada filtro e o numero de filtros
 * 
 */
typedef struct query_list_{
    QueryPair * lista;
    int numeroDeFiltros;
}QueryList;


typedef struct rrn_updater_{
    int RRN;
    QueryList * atualizacoes;
}RRNupdater;

#endif