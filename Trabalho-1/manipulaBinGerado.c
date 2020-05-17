#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "manipulaBinGerado.h"
#include "binHandlerAuxiliar.h"
#include "outputAuxiliar.h"



/**
 * @brief Abre o arquivo binario em modo leitura, escrita (pode crescer a depender dos proximos trabalhos) 
 * levando em conta o cabeçalho definido na struct Header
 * 
 * @param fp o ponteiro para o arquivo que será aberto - estará posicionado no primeiro byte após o cabeçalho de registro
 * @param mode Indica o modo que o arquivo será aberto sendo as opções :
 *              gerar: cria um arquivo e inicializa seu cabeçalho
 *              ler: Tent abrir um arquivo em modo leitura e verifica seu status
 *       
 * @param bin_filename O nome/caminho do arquivo a ser aberto
 * @return unsigned char um código indicando se a operação foi bem sucedida ou não
 */

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



/**
 * @brief Preenche o arquivo binário a partir do arquivo texto seguindo as especificações definidas na struct Registro
 * 
 * @param fp Ponteiro para o arquivo binário - o arquivo deve ter sido aberto anteriormente em modo w e o cabeçalho tem de estar preenchido estando
 * o ponteiro posicionado na primeira posição após o header
 * 
 * @param csv_filename  Nome do arquivo texto que será a base para o preenchimento
 * 
 * @return unsigned char um código indicando se a operação foi bem sucedida ou não
 */

unsigned char preencher_binario(FILE ** fp, char * csv_filename){
    FILE * fp_csv = fopen(csv_filename , "r");
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
    
    //esse trecho atualiza o header de acordo com os novos registros
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




/**
 * @brief Essa função imprime os registro do arquivo binário seguindo o exemplho:
 *          Nasceu em SAO CARLOS/SP, em 2020-04-18, um bebe de sexo FEMININO.
 *          
 * 
 * @param fp ponteiro apontando para o arquivo binário - deve estar aberto em modo leitura - IMPORTANTE fecha o arquivo
 */
void imprime_arquivo_binario(FILE **fp){
    Header h = ler_cabecalho((*fp));
    if (h.status == '0'){
        printf("Falha no processamento do arquivo.\n");
        fclose((*fp));
        return;
    }
    if(h.numeroRegistrosInseridos == 0){
        printf("Registro inexistente.\n");
        fclose((*fp));
        return;
    }
    Registro r;
    
    
    while(h.numeroRegistrosInseridos--){
        r = ler_registro((*fp));
        //printf("%d %d %s %s %d %d %s %c %s %s\n",r.cidadeMaeTamanho,r.cidadeBebeTamanho,r.cidadeMae,r.cidadeBebe,r.idNascimento,r.idadeMae,r.dataNascimento,r.sexoBebe,r.estadoMae,r.estadoBebe) ;
        imprimir_registro(r);
    }

    fclose((*fp));
}