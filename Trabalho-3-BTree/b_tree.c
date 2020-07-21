#include<stdio.h>
#include<stdlib.h>

#include "b_tree.h"
#include "binHandlerAuxiliar.h"
#include "outputAuxiliar.h"
#define UNDEFINED -1
#define ORDEM_B_TREE 6
#define NODE_SIZE 72
#define PROMOTION 0
#define NO_PROMOTION 1


/**
 * @brief estrutura que representa o cabeçalho do arquivo de indice
 * 
 */
typedef struct b_tree_header_ {
    char status;
    int noRaiz;
    int nroNiveis;
    int proxRRN;
    int nroChaves;
}BTreeHeader;

/**
 * @brief estrutura que representa um nó do arvore B
 * 
 */
typedef struct disk_page_{
    int nivel;
    int keycount;
    int key[ORDEM_B_TREE-1];
    int rrns[ORDEM_B_TREE-1];
    int child[ORDEM_B_TREE];
}NodeBTree;

/**
 * @brief le um no da arvore
 * 
 * @param fp 
 * @return NodeBTree 
 */
NodeBTree ler_node_arvore_b(FILE * fp){
    NodeBTree node;
    fread(&node.nivel,sizeof(int),1,fp);
    fread(&node.keycount,sizeof(int),1,fp);
    for(int i=0;i<ORDEM_B_TREE-1;i++){
        fread(&node.key[i],sizeof(int),1,fp);
        fread(&node.rrns[i],sizeof(int),1,fp);
    }
    for(int i=0;i<ORDEM_B_TREE;i++)
        fread(&node.child[i],sizeof(int),1,fp);
    return node;
}


void escrever_node(NodeBTree node,FILE* fp){
    fwrite(&node.nivel,sizeof(int),1,fp);
    fwrite(&node.keycount,sizeof(int),1,fp);
    for(int i=0;i<ORDEM_B_TREE-1;i++){
        fwrite(&node.key[i],sizeof(int),1,fp);
        fwrite(&node.rrns[i],sizeof(int),1,fp);
    }
    for(int i=0;i<ORDEM_B_TREE;i++){
        fwrite(&node.child[i],sizeof(int),1,fp);
    }
}

/**
 * @brief escreve o cabeçalho
 * 
 * @param fp 
 * @param cabecalho 
 */
void escrever_cabecalho_indice(FILE * fp, BTreeHeader cabecalho){
    if(fp != NULL){
        fseek(fp,0,SEEK_SET);
        fwrite(&cabecalho.status,sizeof(char),1,fp);
        fwrite(&cabecalho.noRaiz,sizeof(int),1,fp);
        fwrite(&cabecalho.nroNiveis,sizeof(int),1,fp);
        fwrite(&cabecalho.proxRRN,sizeof(int),1,fp);
        fwrite(&cabecalho.nroChaves,sizeof(int),1,fp);
        char lixo = BLANK;
        for(int i=0;i<55;i++)
            fwrite(&lixo,sizeof(char),1,fp);   
    }
} 
/**
 * @brief le o cabeçalho
 * 
 * @param fp 
 * @return BTreeHeader 
 */
BTreeHeader ler_cabecalho_indice(FILE * fp){
    BTreeHeader cabecalho;
    cabecalho.status = UNDEFINED;    
    if(fp != NULL){
        
        fseek(fp,0,SEEK_SET);
        fread(&cabecalho.status,sizeof(char),1,fp);
        fread(&cabecalho.noRaiz,sizeof(int),1,fp);
        fread(&cabecalho.nroNiveis,sizeof(int),1,fp);
        fread(&cabecalho.proxRRN,sizeof(int),1,fp);
        fread(&cabecalho.nroChaves,sizeof(int),1,fp);
        char lixo = BLANK;
        for(int i=0;i<55;i++)
            fread(&lixo,sizeof(char),1,fp);
    }
    return cabecalho;
}

bool criar_indice_vazio(char * index_filename){
    FILE * fp = fopen(index_filename,"wb");
    if(fp == NULL){
        return FALSE;
    }

    BTreeHeader cabecalho;
    cabecalho.status = ERROR;
    cabecalho.noRaiz = UNDEFINED;
    cabecalho.nroNiveis = 0;
    cabecalho.proxRRN = 0;
    cabecalho.nroChaves = 0;
    escrever_cabecalho_indice(fp,cabecalho);
    fclose(fp);
    return TRUE;
}

void inicializar_node_vazio(NodeBTree * node){
    node->keycount = 0;
    node->nivel = 0;
    for(int i=0;i<ORDEM_B_TREE;i++)
        node->child[i] = -1;
     for(int i=0;i<ORDEM_B_TREE-1;i++){
         node->key[i]=-1;
         node->rrns[i]=-1;
     }
}

void debug_node(NodeBTree atual){
    printf("Nivel:%d Qtd:%d\n",atual.nivel,atual.keycount);
    for(int i=0;i<ORDEM_B_TREE-1;i++){
        printf("%d(%d,%d) ",atual.child[i],atual.key[i],atual.rrns[i]);
    }
    printf("%d",atual.child[5]);
    printf("\n");
}


void insert_key(FILE* fp, int key,int key_offset, int curr_rrn, int* promote_key,int* promote_key_offset,int* promote_r_child,int* promote_nivel,BTreeHeader* cabecalho_indice){
    //pega a posição atual e le o nó
    fseek(fp, NODE_SIZE*(curr_rrn+1),SEEK_SET);
    NodeBTree atual = ler_node_arvore_b(fp);
    (*promote_key) = -1;
    (*promote_key_offset) = -1;
    (*promote_r_child) = -1;
    (*promote_nivel)=-1;
    /*printf("\n");
    for(int i = 0;i<ORDEM_B_TREE-1;i++){
        printf("(%d,%d) ",atual.key[i],atual.rrns[i]);
    }
    printf("\n");
    for(int i = 0;i<ORDEM_B_TREE;i++){
        printf("%d ",atual.child[i]);
    }*/
    //inserção na folha sem split 
    if(atual.nivel == 1 && atual.keycount < ORDEM_B_TREE-1){
        //insere a key e o rrn no lugar certo
        int j = atual.keycount;
        while(j>=0 && atual.key[j-1]>key){
            atual.key[j] = atual.key[j-1];
            atual.rrns[j] = atual.rrns[j-1];
            j--;
        }
        atual.key[j] = key; 
        atual.rrns[j] = key_offset;
        atual.keycount++;
        fseek(fp, NODE_SIZE*(curr_rrn+1),SEEK_SET);
        escrever_node(atual,fp);

        //promoção nula
        (*promote_key) = -1;
        (*promote_key_offset) = -1;
        (*promote_r_child) = -1;
        (*promote_nivel)=-1;
        return;
    }
    //split no nó folha
    else if(atual.nivel==1){
        

        //arrays auxiliares que podem armazenar uma key extra
        int key_aux[ORDEM_B_TREE];
        int rrns_aux[ORDEM_B_TREE];
        int child_aux[ORDEM_B_TREE+1];
        for(int i=0;i<ORDEM_B_TREE-1;i++){
            key_aux[i] = atual.key[i];
            rrns_aux[i] = atual.rrns[i];
        }
        key_aux[ORDEM_B_TREE-1]=-1;
        rrns_aux[ORDEM_B_TREE-1]=-1;
        for(int i=0;i<ORDEM_B_TREE;i++){
            child_aux[i] = atual.child[i];
        }
        child_aux[ORDEM_B_TREE]=-1;

        //acha a posição correta da chave a ser inserida
        int j = ORDEM_B_TREE-1;
        while(j>=0 && key_aux[j-1]>key){
            key_aux[j] = key_aux[j-1];
            rrns_aux[j] = rrns_aux[j-1];
            child_aux[j+1] = child_aux[j];
            j--;
        }
        key_aux[j] = key; 
        rrns_aux[j] = key_offset;
        child_aux[j+1]=-1;

       //a chave promovida é a primeira da segunda metade
        (*promote_key) = key_aux[ORDEM_B_TREE/2];
        (*promote_key_offset)=rrns_aux[ORDEM_B_TREE/2];

        NodeBTree new;
        inicializar_node_vazio(&new);
        new.nivel = 1;
        new.keycount = (ORDEM_B_TREE/2)-1;
        new.child[0] = -1;

        atual.child[0]=-1;
        atual.keycount=ORDEM_B_TREE/2;
        for(int i=0;i<ORDEM_B_TREE-1;i++){
            if(i<atual.keycount){
                atual.key[i] = key_aux[i];
                atual.rrns[i] = rrns_aux[i];
            }
            else{
                 atual.key[i] = -1;
                atual.rrns[i] = -1;
            }
        }

        for(int i=0;i<ORDEM_B_TREE-1;i++){
            if(i<new.keycount){
                new.key[i] = key_aux[atual.keycount+1+i];
                new.rrns[i] = rrns_aux[atual.keycount+1+i];
            }
            else{
                new.key[i] = -1;
                new.rrns[i] = -1;
            }
        }

        (*promote_r_child) = cabecalho_indice->proxRRN;
        (*promote_nivel) = 1;
        fseek(fp,NODE_SIZE*(curr_rrn+1),SEEK_SET);
        escrever_node(atual,fp);

        fseek(fp,NODE_SIZE*(cabecalho_indice->proxRRN+1),SEEK_SET);
        escrever_node(new,fp);


        cabecalho_indice->proxRRN++;
        return;//retorna com promoção
    }
    //buscar uma posição pra inserir
    else{
        int filho_pesquisado;
        for(filho_pesquisado=0;filho_pesquisado<ORDEM_B_TREE-1;filho_pesquisado++){
            if(atual.key[filho_pesquisado] == -1|| atual.key[filho_pesquisado]>key)
                break;    
        }
        insert_key(fp,key,key_offset,atual.child[filho_pesquisado],promote_key,promote_key_offset,promote_r_child,promote_nivel,cabecalho_indice);       
    }

    if((*promote_key) != -1){
        
        //tem espaço no node atual para a promoção
        if(atual.keycount<ORDEM_B_TREE-1){
            //insere a key e o rrn no lugar certo
            int j = atual.keycount;
            while(j>=0 && atual.key[j-1]>(*promote_key)){
                atual.key[j] = atual.key[j-1];
                atual.rrns[j] = atual.rrns[j-1];
                atual.child[j+1] = atual.child[j];
                j--;
            }
            atual.key[j] = (*promote_key); 
            atual.rrns[j] = (*promote_key_offset);
            atual.child[j+1] = (*promote_r_child);
            atual.keycount++;
            atual.nivel = (*promote_nivel)+1;
            fseek(fp, NODE_SIZE*(curr_rrn+1),SEEK_SET);
            escrever_node(atual,fp);
            
            //promoção nula
            (*promote_key) = -1;
            (*promote_key_offset) = -1;
            (*promote_r_child) = -1;
            (*promote_nivel) = -1;

            return;
        }
        //TODO terminar o split
        //debug_node(atual);
        //printf("%d %d %d %d\n",(*promote_key),(*promote_key_offset),(*promote_nivel),(*promote_r_child));


        //arrays auxiliares que podem armazenar uma key extra
        int key_aux[ORDEM_B_TREE];
        int rrns_aux[ORDEM_B_TREE];
        int child_aux[ORDEM_B_TREE+1];
        for(int i=0;i<ORDEM_B_TREE-1;i++){
            key_aux[i] = atual.key[i];
            rrns_aux[i] = atual.rrns[i];
        }
        key_aux[ORDEM_B_TREE-1]=-1;
        rrns_aux[ORDEM_B_TREE-1]=-1;
        for(int i=0;i<ORDEM_B_TREE;i++){
            child_aux[i] = atual.child[i];
        }
        child_aux[ORDEM_B_TREE]=-1;

        //acha a posição correta da chave a ser inserida
        int j = ORDEM_B_TREE-1;
        while(j>=0 && key_aux[j-1]>(*promote_key)){
            key_aux[j] = key_aux[j-1];
            rrns_aux[j] = rrns_aux[j-1];
            child_aux[j+1] = child_aux[j];
            j--;
        }
        key_aux[j] = (*promote_key); 
        rrns_aux[j] = (*promote_key_offset);
        child_aux[j+1]=(*promote_r_child);
/*
        for(int i=0;i<ORDEM_B_TREE;i++){
            printf("%d(%d,%d) ",child_aux[i],key_aux[i],rrns_aux[i]);
        }
        printf("%d",child_aux[6]);
        printf("\n");
*/
        (*promote_key) = key_aux[ORDEM_B_TREE/2];
        (*promote_key_offset) = rrns_aux[ORDEM_B_TREE/2];
        (*promote_r_child) = cabecalho_indice->proxRRN;

        //printf("%d %d %d\n",(*promote_key),(*promote_key_offset),(*promote_r_child));
        
        NodeBTree new;
        inicializar_node_vazio(&new);
        new.nivel = (*promote_nivel)+1;
        new.keycount = (ORDEM_B_TREE/2)-1;

        atual.keycount=ORDEM_B_TREE/2;
        atual.nivel = (*promote_nivel)+1;

        for(int i=0;i<ORDEM_B_TREE-1;i++){
            if(i<atual.keycount){
                atual.key[i] = key_aux[i];
                atual.rrns[i] = rrns_aux[i];
                atual.child[i+1] = child_aux[i+1];
            }
            else{
                 atual.key[i] = -1;
                atual.rrns[i] = -1;
                atual.child[i+1] = -1;
            }
        }
        atual.child[0] = child_aux[0];
        //debug_node(atual);
        for(int i=0;i<ORDEM_B_TREE-1;i++){
            if(i<new.keycount){
                new.key[i] = key_aux[atual.keycount+1+i];
                new.rrns[i] = rrns_aux[atual.keycount+1+i];
                new.child[i+1] = child_aux[atual.keycount+i+2];
            }
            else{
                new.key[i] = -1;
                new.rrns[i] = -1;
                new.child[i+1] = -1;
            }
        }
        new.child[0] = child_aux[atual.keycount+1];
        //debug_node(new);

        (*promote_nivel) = (*promote_nivel)+1;
        fseek(fp,NODE_SIZE*(curr_rrn+1),SEEK_SET);
        escrever_node(atual,fp);

        fseek(fp,NODE_SIZE*(cabecalho_indice->proxRRN+1),SEEK_SET);
        escrever_node(new,fp);


        cabecalho_indice->proxRRN++;
        return;//retorna com promoção        
    }


}

void preencher_indice(char* index_filename,FILE** fp_bin){
    FILE * fp_index = fopen(index_filename,"rb+");
    if(fp_index!= NULL && (*fp_bin)!=NULL){
        BTreeHeader cabecalho_indice = ler_cabecalho_indice(fp_index);
        Header cabecalho_arquivo = ler_cabecalho(*fp_bin);
        if(cabecalho_arquivo.status != STATUS_OK){
            printf("Falha no processamento do arquivo.\n");
            exit(0);
        }
        if(cabecalho_arquivo.numeroRegistrosInseridos >0){
            Registro registro_aux = ler_registro(*fp_bin);
            NodeBTree node_auxiliar;inicializar_node_vazio(&node_auxiliar);
            //inserindo o primeiro no
            node_auxiliar.keycount++; node_auxiliar.nivel=1; node_auxiliar.key[0]=registro_aux.idNascimento; node_auxiliar.rrns[0] = 0;
            cabecalho_indice.noRaiz = 0;cabecalho_indice.nroChaves++;cabecalho_indice.proxRRN=1;cabecalho_indice.nroNiveis=1;
            escrever_node(node_auxiliar,fp_index);
            registro_aux = ler_registro(*fp_bin);
            int rrn_registro = 1;
            while(registro_aux.cidadeMaeTamanho != EOF_TOKEN){
                if(registro_aux.cidadeMaeTamanho != EXCLUIDO){
                    int promote_key = -1;
                    int promote_r_child = -1;
                    int promote_key_offset = -1;
                    int promote_nivel = -1;
                    insert_key(fp_index, registro_aux.idNascimento,rrn_registro,cabecalho_indice.noRaiz,&promote_key,&promote_key_offset,&promote_r_child,&promote_nivel,&cabecalho_indice); 
                    if(promote_key != -1){
                        NodeBTree newRoot;
                        inicializar_node_vazio(&newRoot);
                        newRoot.keycount=1;
                        newRoot.nivel = promote_nivel+1;
                        newRoot.key[0]=promote_key;
                        newRoot.rrns[0]= promote_key_offset;
                        newRoot.child[0]=cabecalho_indice.noRaiz;
                        newRoot.child[1]=promote_r_child;
                        fseek(fp_index,NODE_SIZE*(cabecalho_indice.proxRRN+1),SEEK_SET);
                        escrever_node(newRoot,fp_index);
                        cabecalho_indice.noRaiz = cabecalho_indice.proxRRN;
                        cabecalho_indice.proxRRN++;
                        cabecalho_indice.nroNiveis++;
                    }
                    //printf("%d %d %d %d\n",promote_key,promote_child,promote_key_offset,promote_nivel);
                    cabecalho_indice.nroChaves++;
                }
                registro_aux = ler_registro(*fp_bin);
                rrn_registro ++;
            }
        }
        cabecalho_indice.status = STATUS_OK;
        escrever_cabecalho_indice(fp_index,cabecalho_indice);
        fclose(fp_index);
        fclose((*fp_bin));
    }

}

/**
 * @brief funçao recursiva auxiliar da busca
 * 
 * @param fp 
 * @param rrn_atual 
 * @param idNascimento 
 * @param level 
 * @return int 
 */
int buscaAuxiliar(FILE * fp, int rrn_atual,int idNascimento, int* level){
    if(rrn_atual == UNDEFINED)
        return UNDEFINED;
    fseek(fp, NODE_SIZE*(rrn_atual+1),SEEK_SET);
    NodeBTree node = ler_node_arvore_b(fp);
    (*level)++;
    for(int i=0;i<node.keycount;i++){
        if(idNascimento<=node.key[i]){
           return idNascimento == node.key[i]? node.rrns[i] :buscaAuxiliar(fp,node.child[i],idNascimento,level);
        }
    }
    return buscaAuxiliar(fp,node.child[node.keycount],idNascimento,level);
}

void pesquisaPeloIndice(FILE** fp_bin,char* index_filename, int idNascimento){
    Header h = ler_cabecalho((*fp_bin));
    if(h.status != STATUS_OK){
        printf("Falha no processamento do arquivo.\n");
        fclose(*fp_bin);
        return;
    }
    FILE* fp_index = fopen(index_filename,"rb");
    if(fp_index == NULL){
        printf("Falha no processamento do arquivo.\n");
        fclose(*fp_bin);
        return;
    }
    BTreeHeader bt_header  = ler_cabecalho_indice(fp_index);
    if(bt_header.status != STATUS_OK){
        printf("Falha no processamento do arquivo.\n");
        fclose(*fp_bin);
        fclose(fp_index);
        return;
    }
    if(bt_header.nroChaves == 0){
        printf("Registro inexistente.\n");
        fclose(*fp_bin);
        fclose(fp_index);
        return;
    }
    int level = 0;
    int rrnAchado = buscaAuxiliar(fp_index, bt_header.noRaiz,idNascimento, &level);
    if(rrnAchado != UNDEFINED){
        fseek(*fp_bin,128*(rrnAchado+1),SEEK_SET);
        Registro  aux = ler_registro(*fp_bin);
        imprimir_registro(aux);
        printf("Quantidade de paginas da arvore-B acessadas: %d\n",level);
    }
    else{
        printf("Registro inexistente.\n");
    }

    fclose(fp_index);
    fclose(*fp_bin);
}

void inserir_arquivo_atualizando_indice(char* index_filename,FILE** fp_bin,int numero_insercoes,Registro* registros){
    FILE* fp_index = fopen(index_filename, "rb+");
    if(fp_index==NULL){
        printf("Falha no processamento do arquivo.\n");
        fclose(*fp_bin);
        free(registros);
        exit(0);
    }
    char status = '0';
    fread(&status,sizeof(char),1,fp_index);
    if(status != STATUS_OK){
        printf("Falha no processamento do arquivo.\n");
        fclose(*fp_bin);
        fclose(fp_index);
        free(registros);
        exit(0);
    }
    status = ERROR;
    fseek(fp_index,0,SEEK_SET);
    fwrite(&status,sizeof(char),1,fp_index);
    fseek(fp_index,0,SEEK_SET);
    BTreeHeader cabecalho_indice = ler_cabecalho_indice(fp_index);
    Header cabecalho_arquivo = ler_cabecalho(*fp_bin);
    
    for(int i=0;i<numero_insercoes;i++){
        int key = registros[i].idNascimento;
        int key_rrn = cabecalho_arquivo.RRNproxRegistro;
        cabecalho_arquivo.RRNproxRegistro++;
        fseek(*fp_bin,DISK_PAGE*(key_rrn+1),SEEK_SET);
        escrever_registro(registros[i],*fp_bin);
        if(cabecalho_indice.noRaiz == -1){
            NodeBTree node_auxiliar;inicializar_node_vazio(&node_auxiliar);
            //inserindo o primeiro no
            node_auxiliar.keycount++; node_auxiliar.nivel=1; node_auxiliar.key[0]=key; node_auxiliar.rrns[0] = key_rrn;
            cabecalho_indice.noRaiz = 0;cabecalho_indice.nroChaves++;cabecalho_indice.proxRRN=1;cabecalho_indice.nroNiveis=1;
            escrever_node(node_auxiliar,fp_index);
        }
        else{
            int promote_key = -1;
            int promote_r_child = -1;
            int promote_key_offset = -1;
            int promote_nivel = -1;
            insert_key(fp_index, key,key_rrn,cabecalho_indice.noRaiz,&promote_key,&promote_key_offset,&promote_r_child,&promote_nivel,&cabecalho_indice); 
            if(promote_key != -1){
                NodeBTree newRoot;
                inicializar_node_vazio(&newRoot);
                newRoot.keycount=1;
                newRoot.nivel = promote_nivel+1;
                newRoot.key[0]=promote_key;
                newRoot.rrns[0]= promote_key_offset;
                newRoot.child[0]=cabecalho_indice.noRaiz;
                newRoot.child[1]=promote_r_child;
                fseek(fp_index,NODE_SIZE*(cabecalho_indice.proxRRN+1),SEEK_SET);
                escrever_node(newRoot,fp_index);
                cabecalho_indice.noRaiz = cabecalho_indice.proxRRN;
                cabecalho_indice.proxRRN++;
                cabecalho_indice.nroNiveis++;
            }
            cabecalho_indice.nroChaves++;
        }
    }
    cabecalho_indice.status = STATUS_OK;
    escrever_cabecalho_indice(fp_index,cabecalho_indice);

    fclose(*fp_bin);
    fclose(fp_index);
    free(registros);
}





/*
void split(int i_key,int i_rrn,NodeBTree* node_atual,int* promo_key,int* promo_r_child,NodeBTree* node_novo,int i_key_rrn){
    int key_aux[ORDEM_B_TREE];
    int rrns_aux[ORDEM_B_TREE];
    int child_aux[ORDEM_B_TREE+1];
    for(int i=0;i<ORDEM_B_TREE-1;i++){
        key_aux[i] = node_atual->key[i];
        rrns_aux[i] = node_atual->rrns[i];
    }
    for(int i=0;i<ORDEM_B_TREE;i++){
        child_aux[i] = node_atual->child[i];
    }

    int j = node_atual->keycount-1;
    while(j>=0 && key_aux[j]>i_key){
        key_aux[j+1] = key_aux[j];
        rrns_aux[j+1] = rrns_aux[j];
        child_aux[j+2] = child_aux[j+1];
        j--;
    }
    key_aux[j] = i_key; 
    rrns_aux[j] = i_key_rrn;
    child_aux[j+1] = i_rrn;
    
    inicializar_node_vazio(node_novo);

}

int insert_key(FILE* fp,int current_rrn, int key,int* promo_r_child,int* promo_key,int* promo_key_rrn,int rrn_registro){
    if(current_rrn == UNDEFINED){
        (*promo_key) = key;
        (*promo_key_rrn) = rrn_registro;
        (*promo_r_child) = UNDEFINED;
        return PROMOTION;
    }
    fseek(fp,NODE_SIZE*(current_rrn+1),SEEK_SET);
    NodeBTree node_aux = ler_node_arvore_b(fp);
    
    int pos = 0;
    //conferir depois casos de chave duplicada
    while(pos < node_aux.keycount){
        if(key<=node_aux.key[pos])
            break;
        pos++;
    }
    int  p_b_rrn = -1;
    int p_b_key = -1;
    int p_b_key_rrn = -1;
    int return_value = insert_key(fp,node_aux.child[pos],key,&p_b_rrn,&p_b_key,&p_b_key_rrn,rrn_registro);
    if(return_value == NO_PROMOTION)
        return return_value;
    else if(node_aux.keycount < ORDEM_B_TREE-1){
        //insere p_b_key - inserir no lugar certo
        node_aux.key[node_aux.keycount] = p_b_key; 
        node_aux.rrns[node_aux.keycount] = p_b_key_rrn;
        node_aux.child[node_aux.keycount+1] = p_b_rrn;
        int j = node_aux.keycount-1;
        while(j>=0 && node_aux.key[j]>p_b_key){
            node_aux.key[j+1] = node_aux.key[j];
            node_aux.rrns[j+1] = node_aux.rrns[j];
            node_aux.child[j+2] = node_aux.child[j+1];
            j--;
        }
        node_aux.key[j] = p_b_key; 
        node_aux.rrns[j] = p_b_key_rrn;
        node_aux.child[j+1] = p_b_rrn;
        node_aux.keycount++;

        fseek(fp, NODE_SIZE*(current_rrn+1),SEEK_SET);
        escrever_node(node_aux,fp);

        return NO_PROMOTION;
    }
    else{
        //split
        //escrever a pagina atual
        //escrever a pagina nova
        return PROMOTION;
    }

}
*/