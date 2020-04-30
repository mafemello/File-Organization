#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "manipulaBinGerado.h"
#define MAX_LEN 150
#define TOKEN ","

/**
 * @brief Função para uso interno do módulo escreve o cabeçalho no arquivo
 * 
 * @param h o cabeçalho já com os valores setados
 * @param fp ponteiro do arquivo binario onde será escrito o cabeçalho
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

/**
 * @brief le um registro do arquivo binario e o retorna pela struct registro
 * 
 * @param fp ponteiro para o arquivo binario 
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



unsigned char abrir_binario(FILE ** fp,char * mode, char * bin_filename){

    if(strcmp(mode,"gerar") == 0 ){
        (*fp) = fopen(bin_filename,"wb");
        if((*fp)==NULL)
            return ERROR;
        Header h;
        memset(h.lixo,BLANK,111);
        h.status = ERROR;
        h.RRNproxRegistro = 0;
        h.numeroRegistrosInseridos = 0;
        h.numeroRegistrosRemovidos = 0;
        h.numeroRegistrosAtualizados = 0;
        escrever_cabecalho(h,(*fp));
        return STATUS_OK;
    }

    if(strcmp(mode,"ler") == 0 ){
        // abre arquivo para leitura
        (*fp) = fopen (bin_filename, "rb");

        // se arquivo não existir, retorna erro
        if ((*fp) == NULL) 
            return ERROR;

        char status;
        int numeroRegistroInseridos;
        fread (&status, sizeof(char), 1, (*fp));
        // se status == 0 -----> arquivo inconsistente
        if (status == '0')
            return ERROR;
        fread(&numeroRegistroInseridos,sizeof(int),1,(*fp));
        fread(&numeroRegistroInseridos,sizeof(int),1,(*fp));
        if(numeroRegistroInseridos == 0)
            printf("Registro inexistente.\n");
        return STATUS_OK;
    }

    return ERROR;
}



/**
 * @brief outra função interna do módulo - a partir de uma linha do arquivo texto monta um registro
 * 
 * @param buffer uma linha inteira do arquivo csv com a virgula separando os campos
 * @return Registro estrutura do registro pronta para ser salva no disco
 */
Registro filtrar_buffer(char * buffer){
    Registro r;
    char aux[100];

    //le até a virgula e então copia para a estrutura
    //depois avança buffer ate o primeiro char depois da ,
    //le cidadeMae
    memset(aux,'\0',100);
    sscanf(buffer," %[^,]",aux);
    strcpy(r.cidadeMae,aux);
    r.cidadeMaeTamanho = strlen(r.cidadeMae);
    buffer = strstr(buffer,TOKEN)+1;

    //le cidadeBebe
    memset(aux,'\0',100);
    sscanf(buffer," %[^,]",aux);
    strcpy(r.cidadeBebe,aux);
    r.cidadeBebeTamanho = strlen(r.cidadeBebe);
    buffer = strstr(buffer,TOKEN)+1;

    //le o idNascimento
    //caso o capo esteja vazio ja seta para o valor nulo(-1)
    memset(aux,'\0',100);
    sscanf(buffer," %[^,]",aux);
    if(aux[0] != '\0')
        r.idNascimento = atoi(aux);
    else
        r.idNascimento = -1;
    buffer = strstr(buffer,TOKEN)+1;

    //le idadeMae
    memset(aux,'\0',100);
    sscanf(buffer," %[^,]",aux);
    if(aux[0] != '\0')
        r.idadeMae = atoi(aux);
    else
        r.idadeMae = -1;
    buffer = strstr(buffer,TOKEN)+1;

    //le a dataNascimento
    memset(aux,'\0',100);
    memset(r.dataNascimento, BLANK , 11);
    sscanf(buffer," %[^,]",aux);
    strcpy(r.dataNascimento,aux);
    buffer = strstr(buffer,TOKEN)+1;

    //le o sexoBebe
    memset(aux,'\0',100);
    sscanf(buffer," %[^,]",aux);
    if(aux[0] != '\0')
        r.sexoBebe = aux[0];
    else
        r.sexoBebe = '0';
    buffer = strstr(buffer,TOKEN)+1;


    //le estadoMae
    memset(aux,'\0',100);
    memset(r.estadoMae,BLANK , 3);
    sscanf(buffer," %[^,]",aux);
    strcpy(r.estadoMae,aux);
    buffer = strstr(buffer,TOKEN)+1;

    //le estadoBebe
    memset(aux,'\0',100);
    memset(r.estadoBebe, BLANK , 3);
    sscanf(buffer," %s",aux);
    strcpy(r.estadoBebe,aux);

    return r;

}




unsigned char preencher_binario(FILE ** fp, char * csv_filename){
    FILE * fp_csv = fopen(csv_filename , "r");
    //Registro r;
    if((*fp) == NULL || fp_csv == NULL)
        return ERROR;
    char buffer[MAX_LEN];
    fgets(buffer,MAX_LEN,fp_csv);
    int numeroRegistrosInseridos = 0;
    while(fgets(buffer,MAX_LEN,fp_csv) != NULL){
        Registro r = filtrar_buffer(buffer);
        escrever_registro(r,(*fp));
        numeroRegistrosInseridos++;
    }
    
    Header h;
    memset(h.lixo,BLANK,111);
    h.status = STATUS_OK;
    h.RRNproxRegistro = numeroRegistrosInseridos;
    h.numeroRegistrosInseridos = numeroRegistrosInseridos;
    h.numeroRegistrosRemovidos = 0;
    h.numeroRegistrosAtualizados = 0;
    escrever_cabecalho(h,(*fp));
    fclose(fp_csv);
    fclose((*fp));
    return STATUS_OK;

}



void imprimir_registro(Registro r){
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

void imprime_registros(FILE **fp){
    Header h = ler_cabecalho((*fp));
    Registro r;
    
    
    while(h.numeroRegistrosInseridos--){
        r = ler_registro((*fp));
        //printf("%d %d %s %s %d %d %s %c %s %s\n",r.cidadeMaeTamanho,r.cidadeBebeTamanho,r.cidadeMae,r.cidadeBebe,r.idNascimento,r.idadeMae,r.dataNascimento,r.sexoBebe,r.estadoMae,r.estadoBebe) ;
        imprimir_registro(r);
    }

    fclose((*fp));
}