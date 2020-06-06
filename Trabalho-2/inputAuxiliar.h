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


RRNupdater * ler_atualizacoes(int numero_de_atualizacoes);

void apagar_filtros(QueryList ** q);

void apagar_pacote_atualizacoes(RRNupdater ** r, int numero_de_atualizacoes);
QueryList **ler_remocoes (int numero_de_remocoes);


#endif