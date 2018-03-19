/*
IMPLEMENTATION OF APRIORI ALGORITHM IN C++

fonte: http://ankurm.com/implementation-of-apriori-algorithm-in-c/

    
    10000 compras
    até 30 itens por compra
    500 produtos diferentes
*/
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <omp.h>
#include <string.h>

	int a[10000][30];
	int t1,f1,f2,f3;    
	int f, i, j, k, l, n;
	int p2pcount=0, numProcs, rank;
    int p2items[501];
    int p2pos=0;
    int l2[500000][3], kl, l2jcount=0;        
    int l2t1;
    int l2t2;   
    int l2ocount=0;  
    int p3pcount=0;
    int p3items[10000];
    int l3[10000][4];
    int l3ocount=0;
    int l3jcount=0;
    int resposta[5000000], vetor[501];     
    int controle_posicao = 0;  
    int l2pos1 =0; 
    int aux;
    
    int mestre();
    int escravo();
    
    
int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);      

    char url[]="compras.txt";
    int ch1;
	FILE *arq;
	arq = fopen(url, "r");
	if(arq == NULL)
		printf("Erro, nao foi possivel abrir o arquivo\n");
    else
        for(i=0; i<10000; i++){
            fscanf(arq,"%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d\n", &ch1,&a[i][0],&a[i][1],&a[i][2],&a[i][3],&a[i][4],&a[i][5],&a[i][6],&a[i][7],&a[i][8],&a[i][9],&a[i][10],&a[i][11],&a[i][12],&a[i][13],&a[i][14],&a[i][15],&a[i][16],&a[i][17],&a[i][18],&a[i][19],&a[i][20],&a[i][21],&a[i][22],&a[i][23],&a[i][24],&a[i][25],&a[i][26],&a[i][27],&a[i][28],&a[i][29]);
        }
    fclose(arq);    

    if(rank==0) mestre();
    else
        escravo();

    MPI_Finalize();
    return 0;
}

int mestre(){
    MPI_Status status;
    
    int min=0;
    printf("\n Enter minimum acceptance level: ");
    scanf("%d", &min);
    printf("\n Initial Input:\n");
    printf("\nTrasaction\tItems\n");
    for(i=0; i<10000; i++){
		printf("%i:\t", i+1);        
        for(j=0; j<30; j++){
            printf("%i\t", a[i][j]);
        }
        printf("\n");
    }
    printf("\nAssume minimum support: %i",min);
    
    printf("\n\nGenerating C_1 from data\n");
    int l1[500];    
    for(i=0; i<500; i++){
        t1=0;
        for(j=0; j<10000; j++){
            for(k=0; k<30; k++){
                if(a[j][k]==i+1){
                    t1++;
                }
            }
        }
        l1[i]=t1;
    }    
    for(i=0; i<500; i++){   /*Imprimindo C_1*/
        printf("%i: %i \n",i+1, l1[i]);
    }
    
    printf("\n\nGenerating L_1 from data\n");
    for(i=0; i<500; i++){
        if(l1[i]>=min){
            p2pcount++;
            p2items[p2pos]=i;
            p2pos++;
        }
    }     
    for(i=0; i<p2pos; i++){  /*Imprimindo L_1*/
        printf("%i \t %i \n",p2items[i]+1, l1[p2items[i]]); /*p2items[] deve ser enviado ao escravo*/
    }
    p2items[500] = p2pcount;    
	
	printf("\n\nGenerating L_2 \n");
	/*L_2 -> enviadno mensagens aos escravos*/
    for (i=1; i< numProcs; i++) {
		MPI_Send (p2items, 501, MPI_INT, i, 1, MPI_COMM_WORLD);
	}   
    /*L_2 -> recebendo mensagens dos escravos*/        
    for (i=1; i< numProcs ; i++) {     
		kl=0;
		MPI_Recv (resposta, 5000000, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);		
		while (resposta[kl]!= -1 && kl<=4999997){
			l2[l2jcount][0]=resposta[kl];
			l2[l2jcount][1]=resposta[kl+1];
			l2[l2jcount][2]=resposta[kl+2];			
			kl=kl+3;
			l2jcount++;			
		}
	}	
    for(i=0; i<l2jcount; i++){  /*Imprimindo L_2*/		
        for(j=0; j<3; j++){
            printf("%i \t ", l2[i][j]);
        }
        printf("\n");
    }
    
    for(i = 0; i< 10000;i++){
        p3items[i]=-1;
    }
    int p3pos=0;
    for(i=0; i<l2jcount; i++){
        if(l2[i][2]>=min){
            f=0;
            for(j=0; j<500; j++){
                if(p3items[j]==l2[i][0]){
                    f=1;
                }
            }
            if(f!=1){
                p3items[p3pos]=l2[i][0];
                p3pos++;
                p3pcount++;
            }
            f=0;
            for(j=0; j<500; j++){
                if(p3items[j]==l2[i][1]){
                    f=1;
                }
            }
            if(f!=1){
                p3items[p3pos]=l2[i][1];
                p3pos++;
                p3pcount++;
            }
        }
    }
    for(i=0; i<p3pos; i++){
        printf("%i  \n",p3items[i]);
    }
    
	printf("\n\nGenerating L_3\n");
    for(i=0; i<p3pcount; i++){
        for(j=i+1; j<p3pcount; j++){
            for(k=j+1; k<p3pcount; k++){
                l3[l3jcount][0]=p3items[i];
                l3[l3jcount][1]=p3items[j];
                l3[l3jcount][2]=p3items[k];

                l3ocount=0;
                for(n=0; n<10000; n++){
                    f1=f2=f3=0;
                    for(l=0; l<30; l++){
                        if(l3[l3jcount][0]==a[n][l]){
                            f1=1;
                        }
                        if(l3[l3jcount][1]==a[n][l]){
                            f2=1;
                        }
                        if(l3[l3jcount][2]==a[n][l]){
                            f3=1;
                        }
                    }
                    if(f1==1 && f2==1 && f3==1){
                        l3ocount++;
                    }
                }
                l3[l3jcount][3]=l3ocount;
                l3jcount++;
            }
        }
    }    
    for(i=0; i<l3jcount; i++){
        for(j=0; j<4; j++){
            printf("%i \t",l3[i][j]);
        }
        printf("\n");
    }    
}

int escravo(){
    MPI_Status status;    
    MPI_Recv (vetor, 501, MPI_INT, MPI_ANY_SOURCE,MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    
    memset(resposta,-1,sizeof(resposta));   
	/*Generating L_2*/
	#pragma omp parallel for shared (controle_posicao) private (j, k, l, l2t1, l2t2, f1, f2, l2ocount,l2pos1)	
	for(i=(rank-1); i<vetor[500]; i=i+(numProcs-1)){		
		for(j=i+1; j<vetor[500]; j++){			
            l2t1=vetor[i]+1;
            l2t2=vetor[j]+1;
            if(l2t1==l2t2){
                continue;
            }

			#pragma omp critical
            {    
				l2pos1 = controle_posicao;     
				controle_posicao= controle_posicao +3;   
	        }  	

            resposta[l2pos1]=l2t1;
			resposta[l2pos1+1]=l2t2;	  
            l2ocount=0;            
            for(k=0; k<10000; k++){					
                f1=f2=0;

                for(l=0; l<30; l++){					
                    if(l2t1==a[k][l]){
                        f1=1;
                    }
                    if(l2t2==a[k][l]){
                        f2=1;
                    }
                }
                if(f1==1&&f2==1){
                    l2ocount++;
                }
            }                    
			resposta[l2pos1+2] = l2ocount;
        }
    }
    /*L_2 -> enviadno mensagem ao mestre*/
    MPI_Send (&resposta, 5000000, MPI_INT, 0,1,MPI_COMM_WORLD);
}
