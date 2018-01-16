#ifndef GERAR_SUDOKU_H_INCLUDED
#define GERAR_SUDOKU_H_INCLUDED

int tamnhoMatriz(int tam){
	switch(tam){
	case 2:
		tam= tam*tam;
		break;
	case 3:
		tam= tam*tam;
		break;
	case 4:
		tam= tam*3;
		break;
	}
	return tam ;
}

int **Alocar_matriz_real (int m, int n)
{
  int **v; 
  int   i;   
  if (m < 1 || n < 1) { /* verifica parametros recebidos */
     printf ("** Erro: Parametro invalido **\n");
     return (NULL);
     }
  /* aloca as linhas da matriz */
  v = (int **) calloc (m, sizeof(int *));	/* Um vetor de m ponteiros para float */
  if (v == NULL) {
     printf ("** Erro: Memoria Insuficiente **");
     return (NULL);
     }
  /* aloca as colunas da matriz */
  for ( i = 0; i < m; i++ ) {
      v[i] = (int*) calloc (n, sizeof(int));/* m vetores de n floats */
      if (v[i] == NULL) {
         printf ("** Erro: Memoria Insuficiente **");
         return (NULL);
         }
      }
  return (v); 
}

int **liberarMat(int tam, int **matz) {
	int tamMatz = tamnhoMatriz(tam);
	register int i;
	for ( i = 0; i < tamMatz; i++ ) {
      free (matz[i]); 
	}
    free (matz); 
    return NULL;
}

int indicedogrupo(int i,int j,int tam){
	int m;
	switch(tam){
	case 2:
		if((i==0 || i==1 )&& (j==0 || j==1 ))m=0;
		if((i==0 || i==1 )&& (j==2 || j==3 ))m=1;
		if((i==2 || i==3 )&& (j==0 || j==1 ))m=2;
		if((i==2 || i==3 )&& (j==2 || j==3 ))m=3;
		break;
	case 3:
		if( (i >=0 && i<=2 ) && ( j>=0 && j<=2 ) )m=0;
		if( (i >=0 && i<=2 ) && ( j>=3 && j<=5 ) )m=1;
		if( (i >=0 && i<=2 ) && ( j>=6 && j<=8 ) )m=2;
		if( (i >=3 && i<=5 ) && ( j>=0 && j<=2 ) )m=3;
		if( (i >=3 && i<=5 ) && ( j>=3 && j<=5 ) )m=4;
		if( (i >=3 && i<=5 ) && ( j>=6 && j<=8 ) )m=5;
		if( (i >=6 && i<=8 ) && ( j>=0 && j<=2 ) )m=6;
		if( (i >=6 && i<=8 ) && ( j>=3 && j<=5 ) )m=7;
		if( (i >=6 && i<=8 ) && ( j>=6 && j<=8 ) )m=8;
		break;
	case 4:
		if( (i >=0 && i<=2 ) && ( j>=0 && j<=3 ) )m=0;
		if( (i >=0 && i<=2 ) && ( j>=4 && j<=7 ) )m=1;
		if( (i >=0 && i<=2 ) && ( j>=8 && j<=11 ) )m=2;
		if( (i >=3 && i<=5 ) && ( j>=0 && j<=3 ) )m=3;
		if( (i >=3 && i<=5 ) && ( j>=4 && j<=7 ) )m=4;
		if( (i >=3 && i<=5 ) && ( j>=8 && j<=11 ) )m=5;
		if( (i >=6 && i<=8 ) && ( j>=0 && j<=3 ) )m=6;
		if( (i >=6 && i<=8 ) && ( j>=4 && j<=7 ) )m=7;
		if( (i >=6 && i<=8 ) && ( j>=8 && j<=11 ) )m=8;
		if( (i >=9 && i<=11 ) && ( j>=0 && j<=3 ) )m=9;
		if( (i >=9 && i<=11 ) && ( j>=4 && j<=7 ) )m=10;
		if( (i >=9 && i<=11 ) && ( j>=8 && j<=11 ) )m=11;
		break;
	}
	return m;
}

int tamColmGroup(int tam){
    if(tam==4){
    	tam=3;}
	return tam;
}

int **gerarsudoku(int tam){
	register int i,l,k,m,cont;
	int j=0;;
    int tamComGrop=tamColmGroup(tam);
	int tamMatz = tamnhoMatriz(tam);
	int **matzPont = Alocar_matriz_real(tamMatz,tamMatz);
	int matz[tamMatz][tamMatz],vetLinha[tamMatz];
	int numAlet=0,numAletAnt=0 , testeErro=0,erro=0;
	int *matzGrupos[tamMatz][tam][tam];
	srand((unsigned)time(NULL));
	
	//--------------------------//
	//ponteiro para grupos
	switch(tam){
		case 2:
			m=0;
			for(i=0;i<tamMatz;i++){
				for(j=0;j<tamMatz;j++){
					if((i==0 || i==2) && (j==0 || j==2) ){
						for(k=0;k<tamComGrop;k++){
							for(l=0;l<tam;l++){
								matzGrupos[m][k][l]=&matz[i+k][j+l];
							}
						}
						m++;
		   			}		
				}
			}
			break;
		case 3:
			m=0;
			for(i=0;i<tamMatz;i++){
				for(j=0;j<tamMatz;j++){
					if((i==0 || i==3 || i==6) && (j==0 || j==3 || j==6) ){
						for(k=0;k<tamComGrop;k++){
							for(l=0;l<tam;l++){
								matzGrupos[m][k][l]=&matz[i+k][j+l];
							}
						}
						m++;
		   			}
				}		
			}
			break;
		case 4:
			m=0;
			for(i=0;i<tamMatz;i++){
				for(j=0;j<tamMatz;j++){
					if((i==0 || i==3 || i==6 || i==9 ) && (j==0 || j==4 || j==8) ){
						for(k=0;k<tamComGrop;k++){
							for(l=0;l<tam;l++){
								matzGrupos[m][k][l]=&matz[i+k][j+l];
							}
						}
						m++;
		   			}
				}		
			}
			break;
	}
	
	//--------------------------//
	//zerando a matriz
	for(i=0;i<tamMatz;i++){
		for(j=0;j<tamMatz;j++){
			matz[i][j]=0;
		}
	}	
	
	//--------------------------//
	// gerando a sudoku
	int coluna=0,temp;
	for(i=0;i<tamMatz;i++){
		cont=0;
		vetLinha[tamMatz]==0;
//			gerando o vetor que será inserido na matriz
			do{
				erro=1;
				numAlet = rand() % tamMatz+1;
				if(numAlet== 0 || numAletAnt == numAlet){
					erro=0;
				}
				//testando se numAlet é igual a anterior;
				if(cont>0){
					for(k=0;k<cont;k++){
						if(vetLinha[k]==numAlet){
							erro=0;
						}
					}	
				}
				if(erro){
					vetLinha[cont]=numAlet;
					cont++;
				}
				numAletAnt = numAlet;
			}while(cont<tamMatz);
			
			cont=0;

			do{
			// gerar linha
			erro=1;
			//testando o grupo ;
				m = indicedogrupo(i,coluna,tam);
				for(k=0;k<tamComGrop;k++){
					for(l=0;l<tam;l++){
						if ( *matzGrupos[m][k][l] == vetLinha[cont] ){
							erro=0;
							break;
							break;
						}
					}
				}
//			testando coluna 
			for(k=0;k<i;k++){
				if(matz[k][coluna]==vetLinha[cont]){
					erro=0;
					break;
				}
			}
			
			if(erro==1){
				if (matz[i][coluna]==0){
				matz[i][coluna]=vetLinha[cont];
				cont++;
				}else{
					temp=matz[i][coluna];
					matz[i][coluna]=vetLinha[cont];
					vetLinha[cont]=temp;
					testeErro++;
					if(testeErro == 999){
						return NULL;
					}
				}
			}
			
			coluna++;
			if(coluna==tamMatz){
			coluna=0;
			}
			
		}while(cont<tamMatz);
	}
			
	for(i=0;i<tamMatz;i++){
		for(j=0;j<tamMatz;j++){
			matzPont[i][j]=matz[i][j];
		}
	}		
			
	return matzPont;
}

int gerarDificuldade(int op,int tam){
    int dificuldade;
    int tamMatz = tamnhoMatriz(tam);
    tamMatz=tamMatz*tamMatz;
	switch(tam){
	case 2:
	    dificuldade = 8;
		break;
	case 3:
	    switch(op){
            case 1:
                dificuldade = 38 ;
                break;
            case 2:
                dificuldade = 46 ;
                break;
            case 3:
                dificuldade = 56 ;
                break;
            }
		break;
	case 4:
	    dificuldade = 100;
		break;
	}
	return dificuldade;
}
int sortear(){
    int numAlet=0;
    numAlet = rand() % 2;
    return numAlet;
    }
int indice_aleatorio(int tamMatz){
	int numAlet=0;
	numAlet = rand() % tamMatz;
	return numAlet;

}

#endif // GERAR_SUDOKU_H_INCLUDED
