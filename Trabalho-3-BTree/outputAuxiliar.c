#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "outputAuxiliar.h"


/**
 * @brief essa função imprime um registro r
 * 
 * @param r o registro com os campos setados
 */

void imprimir_registro(Registro r){
    if(r.cidadeMaeTamanho == EXCLUIDO)
        return;
    printf("Nasceu em ");
    if(r.cidadeBebe[0]=='\0')
        printf("-/");
    else
        printf("%s/",r.cidadeBebe);
    if(r.estadoBebe[0]=='\0')
        printf("-,");
    else
        printf("%s,",r.estadoBebe);
    printf(" em ");
    if(r.dataNascimento[0]=='\0')
        printf("-,");
    else
        printf("%s,",r.dataNascimento);
    printf(" um bebê de sexo ");
    if(r.sexoBebe == '0')
        printf("IGNORADO.\n");
    else if(r.sexoBebe == '1')
        printf("MASCULINO.\n");
    else
        printf("FEMININO.\n");
    
}