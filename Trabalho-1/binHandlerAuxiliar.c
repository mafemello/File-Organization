#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "constants.h"
#include "binHandlerAuxiliar.h"


/**
 * @brief Função para uso interno do módulo escreve o cabeçalho no arquivo
 * 
 * @param h o cabeçalho com os valores já setados
 * @param fp ponteiro do arquivo binario onde será escrito o cabeçalho, o ponteiro fica posicionado após o cabeçalho
 *           ao fim da execução
 */
void escrever_cabecalho(Header h, FILE * fp){
    fseek(fp,0,SEEK_SET);
    fwrite(&h.status,sizeof(char),1,fp);
    fwrite(&h.RRNproxRegistro,sizeof(int),1,fp);
    fwrite(&h.numeroRegistrosInseridos,sizeof(int),1,fp);
    fwrite(&h.numeroRegistrosRemovidos,sizeof(int),1,fp);
    fwrite(&h.numeroRegistrosAtualizados,sizeof(int),1,fp);
    fwrite(h.lixo,sizeof(char),111,fp);
}

/**
 * @brief le um registro do arquivo binario e o retorna pela struct registro
 * 
 * @param fp ponteiro para o arquivo binario - deve estar posicionado no primeiro byte de um registro
 * 
 * @return Registro o registro lido
 */
Registro ler_registro(FILE * fp){
    Registro r;
    fread(&r.cidadeMaeTamanho,sizeof(int),1,fp);
    fread(&r.cidadeBebeTamanho,sizeof(int),1,fp);
    memset(r.cidadeMae,'\0',105);
    for(int i = 0;i<r.cidadeMaeTamanho;i++){
        fread(&r.cidadeMae[i],sizeof(char),1,fp);
    }
    memset(r.cidadeBebe,'\0',105);
    for(int i = 0;i<r.cidadeBebeTamanho;i++){
        fread(&r.cidadeBebe[i],sizeof(char),1,fp);
    }
    char lixo;
    for(int i=0;i<105-8-r.cidadeMaeTamanho-r.cidadeBebeTamanho;i++){
        fread(&lixo,sizeof(char),1,fp);
    }
    fread(&r.idNascimento,sizeof(int),1,fp);
    fread(&r.idadeMae,sizeof(int),1,fp);
    memset(r.dataNascimento,'\0',11);
    fread(r.dataNascimento,sizeof(char),10,fp);
    fread(&r.sexoBebe,sizeof(char),1,fp);
    memset(r.estadoMae,'\0',3);
    fread(r.estadoMae,sizeof(char),2,fp);
    memset(r.estadoBebe,'\0',3);
    fread(r.estadoBebe,sizeof(char),2,fp);
    return r;
}

/**
 * @brief Essa função le o cabeçalho do arquivo binário e o retorna por meio da struct Header
 * 
 * @param fp ponteiro do arquivo a ser lido - deve estar aberto em modo leitura - 
 *           ao fim da execução o ponteiro estará posicionado no primeiro byte após o cabeçalho
 * 
 * @return Header cabeçalho que será montado
 */
Header ler_cabecalho(FILE * fp){
    Header h;
    fseek (fp, 0, SEEK_SET);
    fread (&h.status, sizeof(char), 1, fp);
    fread (&h.RRNproxRegistro, sizeof(int), 1 ,fp);
    fread (&h.numeroRegistrosInseridos, sizeof(int), 1 ,fp);
    fread (&h.numeroRegistrosRemovidos, sizeof(int), 1, fp);
    fread (&h.numeroRegistrosAtualizados, sizeof(int), 1 ,fp);
    fread(h.lixo,sizeof(char),111,fp);    

    return h;
}



/**
 * @brief recebe um registro montado e o escreve no arquivo
 * 
 * @param r rgistro a ser escrito
 * @param fp ponteiro para o arquivo a ser escrito - o registro será escrito na posição atual do ponteiro
 */
void escrever_registro(Registro r, FILE *fp){
    fwrite(&r.cidadeMaeTamanho,sizeof(int),1,fp);
    fwrite(&r.cidadeBebeTamanho,sizeof(int),1,fp);
    for(int i=0;i<r.cidadeMaeTamanho;i++){
        fwrite(&(r.cidadeMae[i]),sizeof(char),1,fp);
    }
    for(int i=0;i<r.cidadeBebeTamanho;i++){
        fwrite(&(r.cidadeBebe[i]),sizeof(char),1,fp);
    }
    //preenche os bytes vazios da seção reservada aos campos de tamanho variável com BLANK
    char lixo = BLANK;
    for(int i=0;i<105-8-r.cidadeMaeTamanho-r.cidadeBebeTamanho;i++){
        fwrite(&lixo,sizeof(char),1,fp);
    }
    fwrite(&r.idNascimento,sizeof(int),1,fp);
    fwrite(&r.idadeMae,sizeof(int),1,fp);
    fwrite(r.dataNascimento,sizeof(char),10,fp);
    fwrite(&r.sexoBebe,sizeof(char),1,fp);
    fwrite(r.estadoMae,sizeof(char),2,fp);
    fwrite(r.estadoBebe,sizeof(char),2,fp);
}