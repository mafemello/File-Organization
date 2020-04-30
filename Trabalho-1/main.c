/**
 *     Primeiro Trabalho Prático da Disciplina SCC0215 - Organização de Arquivos
 *     Autores:
 *              Maria Fernanda Lucio de Mello - nUsp 11320860
 *              Victor Rodrigues Russo        - nUps 11218855
 *     
 */
#include<stdio.h>
#include "manipulaBinGerado.h"
#include "binarioNaTela.h"

int main(void){
    int funcionalidade;
    //unsigned char status;
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
        scanf ("%s", bin_filename);
        if(abrir_binario(&fp_bin,"ler",bin_filename) != STATUS_OK){
            printf("Falha no processamento do arquivo.\n");
        }
        else{
            imprime_registros(&fp_bin);
        }
        break;
    }


    return 0;
}