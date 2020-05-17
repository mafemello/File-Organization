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
typedef struct  registro_{
    int cidadeMaeTamanho;
    int cidadeBebeTamanho;
    int idNascimento;
    int idadeMae;
    char sexoBebe;
    char  cidadeMae[105];
    char  cidadeBebe[105];
    char dataNascimento[11];
    char estadoMae[3];
    char estadoBebe[3];
    
}Registro;



#endif