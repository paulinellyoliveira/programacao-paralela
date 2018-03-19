/*
IMPLEMENTATION OF APRIORI ALGORITHM IN C++

fonte: http://ankurm.com/implementation-of-apriori-algorithm-in-c/

    
    10000 compras
    até 30 itens por compra
    500 produtos diferentes
*/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(){
    int t1,f1,f2,f3;    
    int f, i,j, k, l, n;
    int a[10000][30];

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

    int min=0;
    printf("\n Enter minimum acceptance level: ");
    scanf("%d", &min);
    printf("\n Initial Input:\n");
    printf("\nTrasaction\tItems\n");

    /* Impressão das transações x itens */
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
    for(i=0; i<500; i++){  /*Imprimindo C_1*/
        printf("%i: %i \n",i+1, l1[i]);
    }
    
    printf("\n\nGenerating L_1 from data\n");     
    int p2pcount=0;
    int p2items[500];
    int p2pos=0;
    for(i=0; i<500; i++){
        if(l1[i]>=min){
            p2pcount++;
            p2items[p2pos]=i;
            p2pos++;
        }
    }     
    for(i=0; i<p2pos; i++){  /*Imprimindo L_1*/
        printf("%i \t %i \n",p2items[i]+1, l1[p2items[i]]);
    }    
 
	printf("\n\nGenerating L_2 from data\n");
    int l2[500000][3];
    int l2t1;
    int l2t2;
    int l2pos1=0;
    int l2ocount=0;
    int l2jcount=0;
    int inteiro;       
    for(i=0; i<p2pcount; i++){	
        for(j=i+1; j<p2pcount; j++){
  	
            l2t1=p2items[i]+1;
            l2t2=p2items[j]+1;
            if(l2t1==l2t2){
                continue;
            }

			l2pos1 = l2jcount;     
			l2jcount++;   		            
			l2[l2pos1][0]=l2t1;
            l2[l2pos1][1]=l2t2;            
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
            l2[l2pos1][2]=l2ocount;
        }
    } 
    for(i=0; i<l2jcount; i++){   /*Imprimindo L_2*/	
        for(j=0; j<3; j++){
            printf("%i \t", l2[i][j]);
        }
        printf("\n");
    }
    
    int p3pcount=0;
    int p3items[10000];
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

	printf("\n\nGenerating L_3 from data\n");
    int l3[50000][4];
    int l3ocount=0;
    int l3jcount=0;
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
    for(i=0; i<l3jcount; i++){   /*Imprimindo L_3*/
        for(j=0; j<4; j++){
            printf("%i \t",l3[i][j]);
        }
        printf("\n");
    }
    return 0;
}
