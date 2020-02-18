/*

Input file: 
	line 1. function name
	line 2. list of minterm names separted by spaces
	line 3-EOF. list of decimal numbers representing min-terms

*/
#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

#define AS 4096
#define NB 128

char funct[AS];
char var[AS][AS];
int nv;

int N_BITS = 4;		/* number of bits for minterms */
int pow_2_N_BITS_1;     /* 2^(N_BITS+1) */
int N_MINTERMS = 0;	/* number of minterms */

struct P_GRP{			/* a four bit num = 1 and dashes = 4 => "0-01" */
	int num;		/* integer number representing an N_BITS term */
	int numb[NB];		/* binary representation of num */
	int dashes;		/* integer representing position of dashes in term */
	int dashb[NB];		/* binary representation of dashes */
	bool use;		/* true if not combined at a higher level */
	int n_ones;		/* number of 1's in num */
};

P_GRP **PI;
int *input_value;

int ***G1;              /* 3-D array for groups of 1's and subgroups of dashes => N_BITS+1 x 2^N_BITS x 2^NBITS */
int *NG1;               /* number of elements in each group of 1's */
int **NGd;              /* number of elements in each group of dashes */

int N_GRP[NB];

/*----------------------------------------------------------*/
void QM();				/* QM alg */
void getinput(char*);			/* get input from MINTERM_FILE  */
int count_1s(int number);	 	/* count the number of 1s in a number */
int count_0s(int number);	 	/* count the number of 0s in a number */
int count_bits(int n);			/* min bits to represent a number */
void print_groups();			/* print the PIs */
void num2numb(int,int*);		/* convert num to numb */
int numb2num(int*);			/* convert numb to num */
void xor2(int*,int*,int*,int*,int*);	/* special bitwise xor function */
void group(int,int,int);		/* create group */
/*----------------------------------------------------------*/

int main(int argc, char *argv[]) {
	char MINTERM_FILE[128];
        strcpy(MINTERM_FILE,"unknown");

	printf("\nQM - Quine McCluskey Tabular Reduction\n");
	if(argc == 2) { strcpy(MINTERM_FILE,argv[1]); }
	else { printf("minterm filename?\n\n"); exit(1); }

	getinput(MINTERM_FILE);

	QM();

	int i=-1;while(++i <= N_BITS){ free(PI[i]); } free(PI);
	free(input_value);	
	return 0;
}

void print_groups() {
	int i,j,k,m,n,count=0,count2;
	bool covered;
	printf("%s ( %s",funct,var[0]); i=0;while(++i < nv){ printf(", %s",var[i]); } printf(" ) = ");
	i=-1;while(++i < N_GRP[0]-1){printf("m%d+",PI[0][i].num);}
	printf("m%d\nFINAL PIs: \n",PI[0][N_GRP[0]-1].num);
	i=N_BITS+1;while(--i > -1){
		j=-1;while(++j < N_GRP[i]){
			if(PI[i][j].num != -1){
				if(PI[i][j].use == true){
					/* printf("lev %d   grp %d\t",i,j);
					printf("%d %d ",PI[i][j].num,PI[i][j].dashes);
					k=N_BITS;while(--k > -1){printf("%d",PI[i][j].numb[k]);}
					printf(" = %d    ",numb2num(PI[i][j].numb));
					k=N_BITS;while(--k > -1){printf("%d",PI[i][j].dashb[k]);}
					printf(" = %d",numb2num(PI[i][j].dashb));
					printf("\t%d\n",PI[i][j].use); */
					printf("PI_%d:\t",count++);
					k=N_BITS;while(--k > -1){
						if(PI[i][j].dashb[k] == 1){printf("-");}
						else{printf("%d",PI[i][j].numb[k]);}
					}
					printf("  =>  cover for minterms:\n"); 
					m=-1;while(++m < N_GRP[0]){
						covered = true;
						n=-1;while(++n < N_BITS){
							if(PI[i][j].numb[n]!=PI[0][m].numb[n] && PI[i][j].dashb[n]!=1){
								covered=false;}
							}
						if(covered == true){printf("\t%d",PI[0][m].num);}
					} 
					printf("\n");
				}
			}
		}
	}
	if(N_GRP[0] < 32){
		printf("%s ( %s",funct,var[0]); i=0;while(++i < nv){ printf(", %s",var[i]); }
		printf(" ) = \n   Sm(");
		i=-1;while(++i < N_GRP[0]-1){printf("%3d,",PI[0][i].num);}
		printf("%3d)\n",PI[0][N_GRP[0]-1].num);
		count=0;i=N_BITS+1;while(--i > -1){
			j=-1;while(++j < N_GRP[i]){
				if(PI[i][j].num != -1){
					if(PI[i][j].use == true){
						printf("PI_%d:\t",count++);
						m=-1;while(++m < N_GRP[0]){
							covered = true;
							n=-1;while(++n < N_BITS){
								if(PI[i][j].numb[n]!=PI[0][m].numb[n] && PI[i][j].dashb[n]!=1){
									covered=false;}
								}
							if(covered == true){printf("*   ");}
							else{printf("    ");}
						} 
						printf("\n");
					}
				}
			}
		}
	}
	printf("  -- SOP form of output expresson -- NOTE: not all PIs may be essential\n");
	count=0;printf("  %s <= ",funct); 
	i=N_BITS+1;while(--i > -1){
		j=-1;while(++j < N_GRP[i]){
			if(PI[i][j].num != -1){
				if(PI[i][j].use == true){
					count++; if(count > 1){printf("or (");} else{printf("(");}
					count2=0;k=N_BITS;while(--k > -1){
						if(PI[i][j].dashb[k] != 1){
							count2++; if(count2 > 1){printf(" and ");} 
							if(PI[i][j].numb[k] == 1){
								printf("%s",var[N_BITS-k-1]);
							}
							else{
								printf("not(%s)",var[N_BITS-k-1]);
							}
						}
					}
					//printf("  =>  cover for minterms:\n"); 
					//m=-1;while(++m < N_GRP[0]){
					//	covered = true;
					//	n=-1;while(++n < N_BITS){
					//		if(PI[i][j].numb[n]!=PI[0][m].numb[n] && PI[i][j].dashb[n]!=1){
					//			covered=false;}
					//		}
					//	if(covered == true){printf("\t%d",PI[0][m].num);}
					//} 
					if(count2 == 0){ printf(" '1' ) "); } else{ printf(") "); }
				}
			}
		}
	}
	printf(";\n");
}
void getinput(char* MINTERM_FILE) {
	FILE* mfile;
	if((mfile = fopen(MINTERM_FILE,"r"))==(FILE*)NULL)  {
		printf("File %s could not be opened!\n",MINTERM_FILE);
		exit(1);
	} 
	int j,i=-1;while(++i < AS){funct[i] = '0';}
	j=0;fscanf(mfile,"%c",&funct[j]);
	while( funct[j] != '\n' ){
		if( funct[j] == ' '){ funct[j] = '\0';j=-1; }
		fscanf(mfile, "%c",&funct[++j]);
	} funct[j] = '\0';
	i=-1;while(++i < AS){j=-1;while(++j < AS){var[i][j] = '0';}}
	nv=0;j=0;fscanf(mfile,"%c",&var[nv][j]);
	while( var[nv][j] != '\n' ){
		if( var[nv][j] == ' '){ var[nv][j] = '\0';nv++;j=-1; }
		fscanf(mfile, "%c",&var[nv][++j]);
	} var[nv++][j] = '\0';
	/* printf("%s\n",funct); j=-1;while(++j < nv){ printf("%s\n",var[j]); } */
	int in;
	N_MINTERMS = -1;
	int num_bits=0;
	input_value = (int *)malloc(2 * sizeof(int));
	while(feof(mfile)==0){
		fscanf(mfile,"%d",&in);
		if(feof(mfile)==0){
			input_value[++N_MINTERMS] = in;
			/* printf("%d\n",in); */
			input_value = (int*) realloc((char*)input_value,(N_MINTERMS+2)*sizeof(int));
		}
		num_bits = count_bits(in); if(num_bits>N_BITS) N_BITS = num_bits;
	}
	N_MINTERMS++;
	/* i=-1;while(++i < N_MINTERMS){ printf("%d\n",input_value[i]); } */

        N_BITS = nv; 
	if(N_BITS < num_bits){printf("ERROR: not enough inputs for the # of minterms\n");exit(1);}
	pow_2_N_BITS_1 = (int)pow(2,N_BITS+1);
        /* printf("%d %d\n",pow_2_N_BITS_1, N_BITS);exit(1); */

	/* N_BITS x 2^(N_BITS+1) x variable */
	/* we can have at most N_BITS 1's */
	G1 = (int***)malloc((N_BITS+1) * sizeof(int**));
        i=-1;while(++i <= N_BITS){
	  /* we can have at most 2^(N_BITS+1) for the number representing dashes */
	  G1[i] = (int**)malloc(pow_2_N_BITS_1 * sizeof(int*)); 
          j=-1;while(++j < pow_2_N_BITS_1){
	    G1[i][j] = (int*)malloc(1 * sizeof(int)); G1[i][j][0]=-1;
	  }
        }

	NG1 = (int*)malloc((N_BITS+1) * sizeof(int)); 
        i=-1;while(++i <= N_BITS){
          NG1[i]=0;
        }

	NGd = (int**)malloc((N_BITS+1) * sizeof(int*)); 
        i=-1;while(++i <= N_BITS){
	  NGd[i] = (int*)malloc(pow_2_N_BITS_1 * sizeof(int)); 
	  j=-1;while(++j < pow_2_N_BITS_1){
            NGd[i][j]=0;
          }
        }

	fclose(mfile);
}
int count_1s(int number){int bit=0,count=0;while(number>0){bit=number%2;number>>=1;if(bit)count++;}return count;}
int count_0s(int number){int bit=0,count=0;while(number>0){bit=number%2;number>>=1;if(!bit)count++;}return count;}
int count_bits(int n){int bit =0;int count = 0;while(n>0){bit=n%2;n>>=1;count++;}return count;}
int numb2num(int* A){int p=1,B=A[0],i=0; while(++i < N_BITS){B += (A[i]*(p<<=1));} return B; }
void num2numb(int A, int* B){int i=-1; while(++i < N_BITS){ B[i] = A%2; A>>=1; } }
void xor2(int* A, int* AD, int* B, int* BD, int* Z){
	int i=-1;while(++i < N_BITS){
		if(AD[i]==1 && BD[i]==1){Z[i]=1;  /* if both have dashes then set to 1 */ }
		else{ if(A[i]==B[i]){Z[i]=0;} else{ Z[i]=1; } }
	} 
}
void group(int l, int A, int B) {
/*    
	      eg: 	A=00110		B=00100     
	     num:	6 => 00110	4 => 00100
	  dashes:	0 => 00000	0 => 00000
	  result:	num = 4		dashes = 2	group = 001-0

	      eg:	A=00100		B=01100
	     num:	4 => 00100	12 => 01100
	  dashes:	2 => 00010	2 => 00010
	  result:	num = 4		dashes = 10	group = 0-1-0

	      eg:	A=00000		B=00100
	     num:	0 => 00000	4 => 00100
	  dashes:	10 => 01010	10 => 01010
	  result:	num = 0		dashes = 14	group = 0---0
*/
        /* printf("    l=%d    A=%d=%d    B=%d=%d \n",l,A,PI[l][A].num,B,PI[l][B].num);
        if((A != PI[l][A].num) || (B != PI[l][B].num)){printf("ERROR A B\n");exit(1);} */
	bool exists;
	int i, Z[1024], tmp_num, tmp_dashes, tmp_dashb[1024];
	xor2(PI[l][A].numb,PI[l][A].dashb,PI[l][B].numb,PI[l][B].dashb,Z);		/* Z = A xor B */

	/* only combine if dashes are identical and only one other bit is different between the A and B */
	if(PI[l][A].dashes==PI[l][B].dashes && count_1s(numb2num(Z))==l+1){
		PI[l][A].use = false;
		PI[l][B].use = false;
	
		if(PI[l][A].num < PI[l][B].num){tmp_num=PI[l][A].num;}else{tmp_num=PI[l][B].num;}
		tmp_dashes=numb2num(Z);

		exists = false;i=-1;while(++i < N_GRP[l+1]){
			if(tmp_num == PI[l+1][i].num && tmp_dashes==PI[l+1][i].dashes){exists=true;}
		}
		if(exists == false){
			PI[l+1]=(P_GRP*)realloc((char*)PI[l+1],(N_GRP[l+1]+1)*sizeof(P_GRP)); 
	
			PI[l+1][N_GRP[l+1]].use = true;
			if(PI[l][A].num < PI[l][B].num){PI[l+1][N_GRP[l+1]].num=PI[l][A].num;}
			else{PI[l+1][N_GRP[l+1]].num=PI[l][B].num;}
			num2numb(PI[l+1][N_GRP[l+1]].num,PI[l+1][N_GRP[l+1]].numb);
			xor2(PI[l][A].numb,PI[l][A].dashb,PI[l][B].numb,PI[l][B].dashb,PI[l+1][N_GRP[l+1]].dashb);
			PI[l+1][N_GRP[l+1]].dashes=numb2num(Z);
                        PI[l+1][N_GRP[l+1]].n_ones = count_1s(PI[l+1][N_GRP[l+1]].num);

			/* printf("\t%d\t%d = ",N_GRP[l+1],PI[l+1][N_GRP[l+1]].num); 
			i=N_BITS;while(--i > -1){printf("%d",PI[l+1][N_GRP[l+1]].numb[i]);}
			printf("\t%d = ",PI[l+1][N_GRP[l+1]].dashes);
			i=N_BITS;while(--i > -1){printf("%d",PI[l+1][N_GRP[l+1]].dashb[i]);}
			printf("\n"); */

                        //printf("                   N=%d \n",PI[l+1][N_GRP[l+1]].num);
			N_GRP[l+1]++; 
		}
		else{	
			/* printf("\tnot grouped:\t%d %d %d\n",l+1,PI[l][A].num,PI[l][B].num); */
		}
	}
}
void QM() {                        /* determine PI via Quine McCluskey */
	int N_ones,N_dashes,l,n;
	int i,j,k;
	PI = (P_GRP**)malloc((N_MINTERMS)*sizeof(P_GRP*));
	i=0;PI[i] = (P_GRP*)malloc((N_MINTERMS)*sizeof(P_GRP)); 
	i=0;while(++i <= N_BITS){ PI[i] = (P_GRP*)malloc((1)*sizeof(P_GRP)); }
	N_GRP[0]=N_MINTERMS;i=0;while(++i < NB){N_GRP[i]=0;}
	j=0;i=-1;while(++i < N_MINTERMS){ 
          PI[j][i].num = input_value[i]; PI[j][i].dashes = 0; PI[j][i].use = true; 
          PI[j][i].n_ones = count_1s(PI[j][i].num);
          //printf("%d   %d\n",PI[j][i].num,PI[j][i].n_ones);
        }
	j=0;while(++j < N_BITS){ PI[j][0].num = -1; PI[j][0].dashes = 0; PI[j][0].use = false; }
	j=0;i=-1;while(++i < N_MINTERMS){ num2numb(PI[j][i].num, PI[j][i].numb);}
	j=0;while(++j < N_BITS){ num2numb(PI[j][0].dashes, PI[j][0].dashb);} 
        printf("N_BITS = %d\n",N_BITS);
	i=-1;while(++i < N_BITS){

		j=-1;while(++j <= N_BITS){
			NG1[j]=0;                  /* empty each group of 1's */
			k=-1;while(++k < pow_2_N_BITS_1){
				NGd[j][k]=0;       /* empty each group of dashes */
			}
		}

		j=-1;while(++j < N_GRP[i]){
			N_ones = PI[i][j].n_ones;       /* for group j level i */
			N_dashes = PI[i][j].dashes;
			/* printf("  l %d   1's %d %d %d   d's %d\n",i,N_ones,NG1[N_ones],N_GRP[i],N_dashes); */
			G1[N_ones][N_dashes] = (int*)realloc((char*)G1[N_ones][N_dashes],
                                               (NGd[N_ones][N_dashes]+1)*sizeof(int));
			G1[N_ones][N_dashes][NGd[N_ones][N_dashes]] = j;
			/* printf("    %d    %d    %d\n",N_ones,NG1[N_ones]+1,G1[N_ones][NG1[N_ones]]); */
			NG1[N_ones]++; 
			NGd[N_ones][N_dashes]++;
		}
                printf("%d cube: ",i);j=-1;while(++j <= N_BITS-i){printf("%d ",NG1[j]);}printf("\n");
                /* j=-1;while(++j <= N_BITS-i){
			k=-1;while(++k < pow_2_N_BITS_1){printf("  %d",NGd[j][k]);}printf("\n");
		}
		printf("\n"); */

		/* j=-1;while(++j < N_GRP[i]){
			k=j;while(++k < N_GRP[i]){
                                //printf("l = %d %d %d %d %d\n",N_BITS,i,j,k,N_GRP[i]); 
                                //printf("   %d %d %d %d\n",i,j,k,N_GRP[i]); 
			        group(i,j,k);
			}
		} */

		l=-1;while(++l < N_BITS-i){     /* one less group each pass */
			//printf(" %d",l);
			printf(" G%d[%d]-G%d[%d]",l,NG1[l],l+1,NG1[l+1]);
			n=-1;while(++n < pow_2_N_BITS_1){    /* # possible dashes */
				if((NGd[l][n] > 0) && (NGd[l+1][n] > 0)){
					j=-1;while(++j < NGd[l][n]){
						k=-1;while(++k < NGd[l+1][n]){
							group(i,G1[l][n][j],G1[l+1][n][k]);
						}
					}
				}
			}
		}
		printf("\n");

	}
	print_groups();
}
