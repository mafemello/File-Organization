/**
 * @brief esse módulo manipula o binário em nível mais baixo(executando fwrites e freads), contendo métodos que são usados em diversas
 *        funcionalidades como escrever_cabeçalho ou ler_registro
 * 
 */

#ifndef BIN_AUXILIAR
#define BIN_AUXILIAR


/**
 * @brief Função para uso interno do módulo escreve o cabeçalho no arquivo
 * 
 * @param h o cabeçalho com os valores já setados
 * @param fp ponteiro do arquivo binario onde será escrito o cabeçalho, o ponteiro fica posicionado após o cabeçalho
 *           ao fim da execução
 */
void escrever_cabecalho(Header h, FILE * fp);

/**
 * @brief le um registro do arquivo binario e o retorna pela struct registro
 * 
 * @param fp ponteiro para o arquivo binario - deve estar posicionado no primeiro byte de um registro
 * 
 * @return Registro o registro lido
 */
Registro ler_registro(FILE * fp);


/**
 * @brief Essa função le o cabeçalho do arquivo binário e o retorna por meio da struct Header
 * 
 * @param fp ponteiro do arquivo a ser lido - deve estar aberto em modo leitura - 
 *           ao fim da execução o ponteiro estará posicionado no primeiro byte após o cabeçalho
 * 
 * @return Header cabeçalho que será montado
 */
Header ler_cabecalho(FILE * fp);



/**
 * @brief recebe um registro montado e o escreve no arquivo
 * 
 * @param r rgistro a ser escrito
 * @param fp ponteiro para o arquivo a ser escrito - o registro será escrito na posição atual do ponteiro
 */
void escrever_registro(Registro r, FILE *fp);



#endif