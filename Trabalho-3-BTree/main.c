/**
 *     Primeiro Trabalho Prático da Disciplina SCC0215 - Organização de Arquivos
 *     Autores:
 *              Maria Fernanda Lucio de Mello - nUsp 11320860
 *              Victor Rodrigues Russo        - nUsp 11218855
 *     
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "manipulaBinGerado.h"
#include "binarioNaTela.h"
#include "inputAuxiliar.h"
#include "b_tree.h"


/**
 * @brief Essa é a main do programa, a lógica dela é intencionalmente bem simples, lendo o método e seus respectivos
 *         parâmetros e então chamando os métodos responsáveis pelo processamento
 * 
 */
int main(void){
    int funcionalidade;
    int numero_de_filtros;  
    int numero_de_insercoes;
    int RRN;
    char bin_filename[30], csv_filename[30],index_filename[30];
    FILE * fp_bin;
    scanf("%d", &funcionalidade);
    switch (funcionalidade){

        //Primeira funcionalidade
        /**  Permite a leitura de vários registros obtidos a partir de um arquivo de entrada csv
         *   e a gravação desses registros em um arquivo binário de dados de saída. 
         **/
        case 1:
            scanf(" %s %s",csv_filename,bin_filename);
            if(abrir_binario(&fp_bin,"gerar",bin_filename) != STATUS_OK || preencher_binario(&fp_bin,csv_filename) != STATUS_OK){
                printf("Falha no carregamento do arquivo.\n");
            }
            else{
                binarioNaTela(bin_filename);
            }
        break;

        // Segunda funcionalidade
        /**  Permite a recuperação dos dados, de todos os registros, armazenados no arquivo binário de
         *   dados, mostrando os dados de forma organizada na saída padrão para permitir a
         *   distinção dos campos e registros. 
         **/
        case 2:
            scanf (" %s", bin_filename);
            if(abrir_binario(&fp_bin,"ler",bin_filename) != STATUS_OK){
                printf("Falha no processamento do arquivo.\n");
            }
            else{
                imprime_arquivo_binario(&fp_bin);
            }
        break;

        // Terceira Funcionalidade
        /**
         * permite a busca utilizando um filtro composto
         * 
         */
        case 3:
            
            scanf (" %s %d", bin_filename, &numero_de_filtros);
            QueryList * filtros = ler_filtro_busca(numero_de_filtros);
            if(abrir_binario(&fp_bin,"ler",bin_filename) != STATUS_OK){
                printf("Falha no processamento do arquivo.\n");
            }
            else{
                recupera_arquivo_binario(&fp_bin,filtros);
            }

            apagar_filtros(&filtros);
        break;
        
        // Permite recuperação dos dados a partir do RRN 
        case 4:
            scanf (" %s %d", bin_filename, &RRN);
            if (abrir_binario (&fp_bin, "ler", bin_filename) != STATUS_OK){
                printf("Falha no processamento do arquivo.\n");   
            } else {
                imprime_RRN (&fp_bin, RRN);
            }

        break;
        
        // Quinta Funcionalidade
        /**
         * Exclui registros logicamente baseado em múltiplos filtros compostos
         * 
         */
        case 5:;
            int numero_de_remocoes; 
            scanf (" %s %d", bin_filename, &numero_de_remocoes);
            QueryList** remocoes = ler_remocoes (numero_de_remocoes);
            if (abrir_binario (&fp_bin, "alterar", bin_filename) != STATUS_OK) {
                printf("Falha no processamento do arquivo.\n"); 
            } else {
                remover (&fp_bin, remocoes, numero_de_remocoes);
                binarioNaTela(bin_filename);
            }
            for (int i = 0; i < numero_de_remocoes; i++) {
                apagar_filtros (&remocoes[i]);
            }
            free (remocoes);
            break;

        // Sexta Funcionalidade
        /**
         * Permite a inserção no fim de um arquivo sem reaproveitamento de espaço
         * 
         */
        case 6:;
            scanf(" %s %d",bin_filename,&numero_de_insercoes);
            Registro * registros = ler_registros_terminal(numero_de_insercoes);
            if(abrir_binario(&fp_bin,"alterar",bin_filename)!=STATUS_OK){
                printf("Falha no processamento do arquivo.\n");
            }
            else{
                adciona_registros_binario(&fp_bin,numero_de_insercoes,registros);
                binarioNaTela(bin_filename);
            }
            free(registros);
        break;


        // Alteração
        /**
         * A partir do RRn promove alterações nos registros
         * 
         */
        case 7:;
            int numero_de_atualizacoes;
            scanf(" %s %d",bin_filename,&numero_de_atualizacoes);
            RRNupdater * pacote_de_atualizacoes = ler_atualizacoes(numero_de_atualizacoes);
            if(abrir_binario(&fp_bin,"alterar",bin_filename)!=STATUS_OK){
                printf("Falha no processamento do arquivo.\n");
            }
            else{
                atualiza_registros_binario(&fp_bin,numero_de_atualizacoes,pacote_de_atualizacoes);
                binarioNaTela(bin_filename);
            }


            apagar_pacote_atualizacoes(&pacote_de_atualizacoes,numero_de_atualizacoes);
        break;


        // Criação do indice arvore B
        /**
         * a partir do arquivo binário de dados gera um arquivo binário de indice utilizando a estrutur arvore B
         * 
         */
        case 8:
            scanf(" %s %s",bin_filename,index_filename);
            if(abrir_binario(&fp_bin,"ler",bin_filename) != STATUS_OK){
                printf("Falha no processamento do arquivo.\n");
            }
            else{
                if(!criar_indice_vazio(index_filename)){
                    printf("Falha no processamento do arquivo.\n");
                }
                else{
                    preencher_indice(index_filename,&fp_bin);
                    binarioNaTela(index_filename);
                }
            }
        break;

        //Pesquisa um registro
        /**
         * a partir da chave idNascimento, recupera o registro, se utilizando do arquivo de indice
         * 
         */
        case 9: ;
            char lixo_idNascimento[20];
            int idNascimento;
            scanf(" %s %s %s %d",bin_filename,index_filename,lixo_idNascimento,&idNascimento);
            if(abrir_binario(&fp_bin,"ler",bin_filename) != STATUS_OK){
                printf("Falha no processamento do arquivo.\n");
            }
            else{
                pesquisaPeloIndice(&fp_bin,index_filename,idNascimento);
            }

        break;

        case 10:;
            int numero_de_insercoes;
            scanf(" %s %s %d",bin_filename,index_filename,&numero_de_insercoes);
            Registro * registros_aux = ler_registros_terminal(numero_de_insercoes);
            if(abrir_binario(&fp_bin,"alterar",bin_filename)!=STATUS_OK){
                printf("Falha no processamento do arquivo.\n");
                free(registros_aux);
            }
            else{
                inserir_arquivo_atualizando_indice(index_filename,&fp_bin,numero_de_insercoes,registros_aux);
                binarioNaTela(index_filename);
            }
        break;


    }
    return 0;
}