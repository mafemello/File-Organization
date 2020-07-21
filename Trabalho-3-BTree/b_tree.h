/**
 *  esse módulo é responsável pela manipulação da estrutura arvore B
 * 
 * 
 */
#ifndef B_TREE_H


#define B_TREE_H
#include "constants.h"


/**
 * @brief cria um arquivo somente com um cabeçalho vazio
 * 
 * @param index_filename 
 * @return true 
 * @return false 
 */
bool criar_indice_vazio(char * index_filename);



/**
 * @brief preenche o indice a partir do arquivo de dados
 * 
 * @param index_filename 
 * @param fp_bin deve estar aberto em modo leitura
 */
void preencher_indice(char* index_filename,FILE** fp_bin);

/**
 * @brief recupera e imprime um registro a partir de seu idNascimento
 * 
 * @param fp_bin deve estar aberto em modo leitura
 * @param index_filename 
 * @param idNascimento 
 */
void pesquisaPeloIndice(FILE** fp_bin,char* index_filename, int idNascimento);


void inserir_arquivo_atualizando_indice(char * index_filename,FILE** fp_bin,int numero_insercoes,Registro* registros);

#endif