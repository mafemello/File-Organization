/**
 * @file inputAuxiliar.h
 *        
 *      esse módulo contêm funções de input
 *  
 */

#ifndef INPUT_AUX_H
#define INPUT_AUX_H
#include "constants.h"

/**
 * @brief Essa função le e monta uma estrutura com o filtro de busca 
 * 
 * @param numero_de_filtros numero de pares de nomes de campo e valores
 * 
 * @return QueryList* a lista da estrutura  
 */
QueryList * ler_filtro_busca(int numero_de_filtros);

/**
 * @brief pega os registro da entrada do usuario e monta seguindo as especificações da estrutura Registro
 * 
 * @param numero_de_insercoes numero de entradas
 * @return Registro* array contendo os dados
 */
Registro  * ler_registros_terminal(int numero_de_insercoes);

/**
 * @brief le da entrada do terminal as atualizações a serem executadas
 * 
 * @param numero_de_atualizacoes 
 * @return RRNupdater* array com todas as atualizaçoes
 */
RRNupdater * ler_atualizacoes(int numero_de_atualizacoes);

/**
 * @brief libera espaço na heap
 * 
 * @param q 
 */
void apagar_filtros(QueryList ** q);

/**
 * @brief libera espaço na heap
 * 
 * @param r 
 * @param numero_de_atualizacoes 
 */
void apagar_pacote_atualizacoes(RRNupdater ** r, int numero_de_atualizacoes);

/**
 * @brief le do terminal os filtros de remoçao
 * 
 * @param numero_de_remocoes 
 * @return QueryList** um array de queries para remoção
 */
QueryList **ler_remocoes (int numero_de_remocoes);


#endif