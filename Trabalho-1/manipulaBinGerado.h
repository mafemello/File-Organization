/**
 * Esse módulo tem o próposito de manipular o arquivo binário gerado a partir do csv
 * 
 **/

#ifndef HANDLER_BIN_H
#define HANDLER_BIN_H
#define ERROR '0'
#define STATUS_OK '1' 
#define BLANK '$'

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




/**
 * @brief Abre o arquivo binario em modo leitura, escrita (pode crescer a depender dos proximos trabalhos) 
 * levando em conta o cabeçalho definido na struct Header
 * 
 * @param fp o ponteiro para o arquivo que será aberto - estará posicionado no primeiro byte após o cabeçalho de registro
 * @param mode Indica o modo que o arquivo será aberto sendo as opções :
 *              gerar: cria um arquivo e inicializa seu cabeçalho
 *       
 * @param bin_filename O nome/caminho do arquivo a ser aberto
 * @return unsigned char um código indicando se a operação foi bem sucedida ou não
 */
unsigned char abrir_binario(FILE **fp,char * mode,char * bin_filename);


/**
 * @brief Preenche o arquivo binário a partir do arquivo texto seguindo as especificações definidas na struct Registro
 * 
 * @param fp Ponteiro para o arquivo binário - o arquivo deve ter sido aberto anteriormente em modo w e o cabeçalho tem de estar preenchido sendo
 * o ponteiro na primeira posição após o header
 * 
 * @param csv_filename  Nome do arquivo texto que será a base para o preenchimento
 * 
 * @return unsigned char um código indicando se a operação foi bem sucedida ou não
 */
unsigned char preencher_binario(FILE **fp, char * csv_filename);


/**
 * @brief Essa função imprime os registro do arquivo binário seguindo o exemplho:
 *          Nasceu em SAO CARLOS/SP, em 2020-04-18, um bebe de sexo FEMININO.
 *          
 * 
 * @param fp ponteiro apontando para o arquivo binário - deve estar aberto em modo leitura - IMPORTANTE fecha o arquivo
 */
void imprime_registros(FILE **fp);

#endif