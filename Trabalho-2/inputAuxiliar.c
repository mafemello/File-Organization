
#include "inputAuxiliar.h"
#include "binarioNaTela.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



QueryList * criar_query_list(int numero_de_filtros){
    QueryList * filtros = (QueryList*)malloc(sizeof(QueryList));
    if(filtros == NULL){
        exit(0);
    }
    filtros->numeroDeFiltros = numero_de_filtros;
    filtros->lista = (QueryPair *)malloc(sizeof(QueryPair)*numero_de_filtros);
    return filtros;
}



void apagar_filtros(QueryList ** q){
    free((*q)->lista);
    free(*q);
    q = NULL;
}

void apagar_pacote_atualizacoes(RRNupdater ** r,int numero_de_atualizacoes){
    for(int i=0;i<numero_de_atualizacoes;i++){
        apagar_filtros(&(*r)[i].atualizacoes);
    }
    free(*r);
    r = NULL;
}

/**
 * @brief le um único QueryPair, que é constituído de nomeDoCampo e valor
 * 
 * @param q queryPair a ser lido, não deve ser nulo
 */
void ler_query_pair(QueryPair * q){
    if(q != NULL){
        scanf(" %s", q->nomeDoCampo);
        if(strcmp(q->nomeDoCampo, "idNascimento")==0||strcmp(q->nomeDoCampo, "idadeMae")==0){
            char aux[MAX_LEN];
            scanf(" %s",aux);
            if(aux[0]=='N'){
                q->valor.valor_numerico = -1;
            }
            else{
                q->valor.valor_numerico = atoi(aux);
            }
        }
        else
            scan_quote_string(q->valor.valor_string);
    }

}


/**
 * @brief Essa função le e monta uma estrutura com o filtro de busca 
 * 
 * @param numero_de_filtros numero de pares de nomes de campo e valores
 * 
 * @return QueryList* a lista da estrutura  
 */
QueryList * ler_filtro_busca(int numero_de_filtros){
    QueryList * filtros = criar_query_list(numero_de_filtros);
    for(int i=0;i<numero_de_filtros;i++){
        ler_query_pair(&filtros->lista[i]);
    }
    return filtros;
}


QueryList **ler_remocoes (int numero_de_remocoes) {
    QueryList ** filtros = (QueryList**)malloc(sizeof(QueryList*)*numero_de_remocoes);
    int quantidade_de_campos;
    for (int i = 0; i < numero_de_remocoes; i ++) {
        scanf ("%d" , &quantidade_de_campos);
        filtros[i] = ler_filtro_busca(quantidade_de_campos);
    }

    return filtros;
}

RRNupdater * ler_atualizacoes(int numero_de_atualizacoes){
    RRNupdater * pacote_de_atualizacoes = (RRNupdater *)malloc(sizeof(RRNupdater)*numero_de_atualizacoes);
    for(int i=0;i<numero_de_atualizacoes;i++){
        int n;
        scanf(" %d %d",&pacote_de_atualizacoes[i].RRN,&n);
        pacote_de_atualizacoes[i].atualizacoes = ler_filtro_busca(n);
    }

    return pacote_de_atualizacoes;
}


Registro  * ler_registros_terminal(int numero_de_insercoes){
    Registro * registros = (Registro *)malloc(sizeof(Registro)*numero_de_insercoes);
    char buffer[MAX_LEN];
    for(int i=0;i<numero_de_insercoes;i++){

        //lendo cidadeMae
        scan_quote_string(buffer);
        registros[i].cidadeMaeTamanho = strlen(buffer);
        strcpy(registros[i].cidadeMae,buffer);

        //lendo cidadeBebe
        scan_quote_string(buffer);
        registros[i].cidadeBebeTamanho = strlen(buffer);
        strcpy(registros[i].cidadeBebe,buffer);

        //lendo idNascimento
        scanf(" %s",buffer);
        if(buffer[0] == 'N')
            registros[i].idNascimento = -1;
        else
            registros[i].idNascimento = atoi(buffer);
        
        //lendo idadeMae
        scanf(" %s",buffer);
        if(buffer[0] == 'N')
            registros[i].idadeMae = -1;
        else
            registros[i].idadeMae = atoi(buffer);

        //lendo dataNascimento
        memset(registros[i].dataNascimento,BLANK,11);
        scan_quote_string(buffer);
        strcpy(registros[i].dataNascimento,buffer);

        //lendo sexoBebe  
        //TODO confirmar se NULO é 0
        scan_quote_string(buffer);
        if(strcmp(buffer,"")==0)
            registros[i].sexoBebe='0';
        else
            registros[i].sexoBebe = buffer[0];

        
        //lendo estadoMae
        memset(registros[i].estadoMae,BLANK,3);
        scan_quote_string(buffer);
        strcpy(registros[i].estadoMae,buffer);
        
        //lendo estadoBebe
        memset(registros[i].estadoBebe,BLANK,3);
        scan_quote_string(buffer);
        strcpy(registros[i].estadoBebe,buffer);
    }
    return registros;   
}