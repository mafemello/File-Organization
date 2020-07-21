/**
 * Esse módulo tem o próposito de manipular o arquivo binário gerado a partir do csv - os métodos são de alto nível de abstração
 * sendo inspirados diretamente nas funcionalidades do trabalho
 * 
 **/


#ifndef HANDLER_BIN_H
#define HANDLER_BIN_H

#include "constants.h"





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
void imprime_arquivo_binario(FILE **fp);

/**
 * @brief imprime o registro de RRN especificado
 * 
 * @param fp deve ter sido aberto em modo leitura - IMPORTANTE fecha o arquivo
 * @param RRN especificado
 */
void imprime_RRN (FILE **fp, int RRN);


/**
 * @brief a partir de uma lista de filtros, faz a busca composto exibindo os registros que se enquadrarem
 * 
 * @param fp ponteiro para o arquivo binário, deve ter sido aberto em modo leitura - IMPORTANTE fecha o arquivo
 * @param filtros filtros da busca baseados na entrada do
 */
void recupera_arquivo_binario(FILE **fp,QueryList * filtros);

/**
 * @brief Adciona os dados salvos no array registros ao arquivo usando a abordagem estática para registros logicamente removidos
 * 
 * @param fp 
 * @param numero_de_registros 
 * @param registros array com os registros
 */
void adciona_registros_binario(FILE **fp,int numero_de_registros,Registro * registros);

/**
 * @brief Atualiza vários registros
 * 
 * @param fp ponteiro para o arquivo binário, deve ter sido aberto em modo alterar - IMPORTANTE fecha o arquivo
 * @param numero_de_atualizacoes 
 * @param pacote_de_atualizacoes estrutura contendo os rrns e alterações a serem feitas em cada registro
 */
void atualiza_registros_binario(FILE ** fp,int numero_de_atualizacoes,RRNupdater *  pacote_de_atualizacoes);

/**
 * @brief remove vários registros especificados em múltiplas queries
 * 
 * @param fp ponteiro para o arquivo binário, deve ter sido aberto em modo alterar - IMPORTANTE fecha o arquivo
 * @param remocoes um array de queries
 * @param numero_de_remocoes 
 */
void remover (FILE **fp, QueryList **remocoes, int numero_de_remocoes);

#endif