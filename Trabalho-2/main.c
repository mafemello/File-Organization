/**
 *     Segundo Trabalho Prático da Disciplina SCC0215 - Organização de Arquivos
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


/**
 * @brief Essa é a main do programa, a lógica dela é intencionalmente bem simples, lendo o método e seus respectivos
 *         parâmetros e então chamando os métodos responsáveis pelo processamento
 * 
 */
int main(void){
    int funcionalidade;
    int numero_de_filtros;  
    int RRN;
    char bin_filename[30], csv_filename[30];
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


        case 6:;
            int numero_de_insercoes;
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



    }
    return 0;
}
