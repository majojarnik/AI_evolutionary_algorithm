#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <math.h>

#define N 100



int fitnes(char *bity, char **mapa, int vyska, int sirka, int start_x, int start_y, int poklady){		//funkcia na vypocet fitnes hodnot
	unsigned char stroj[64];
	int i, j, x = start_x, y = start_y, sucet = 0, posledny = 0, kroky = 0;
	unsigned char instr, adr, pohyb;
	 
	/*for (i = 0; i < vyska; i++)
		for (j = 0; j < sirka; j++)
			mapka[i][j] = mapa[i][j];
	*/			
	for (i = 0; i < 64; i++){
		stroj[i] = bity[i];
	}
	
	i = 0;
	
	for (j = 0; j < 500; j++){
		instr = stroj[i] >> 6;						//ziskanie instrukcie a adresy z bunky
		adr = stroj[i] << 2;
		adr = adr >> 2;
		
		i++;
		switch (instr){
		    case 0:									//pripad kedy je instrukcia 00 = inkrementacia
		      	stroj[adr]++;
		      	break;
		
		    case 1:									//pripad kedy je instrukcia 01 = dekrementacia
				stroj[adr]--;
				break;
		    
			case 2:									//pripad kedy je instrukcia 10 = skok
		   		i = adr;
		      	break;
		      
		    case 3:									//pripad kedy je instrukcia 11 = vypis/posunutie na mape
		    	kroky++;
		      	pohyb = stroj[adr] << 6;				//ziskanie smeru pohybu podla poslednych 2 bitov bunky
				pohyb = pohyb >> 6;
				if (pohyb == 0)							//pripad kedy je pohyb 00 = hore
					x--;
				else if (pohyb == 1)					//pripad kedy je pohyb 01 = dole
					x++;
				else if (pohyb == 2)					//pripad kedy je pohyb 10 = doprava
					y++;
				else									//pripad kedy je pohyb 11 = dolava
					y--;							
				
				if (x < 0 || y < 0 || x == vyska || y == sirka)
					break;
				
				if (mapa[x][y] == 1){					//ak najdem poklad zvysim premennu sucet(pocet najdenych pokladov) a miesto na mape oznacim 2 aby som v buducnosti poklad nezarataval
					sucet++; 
					mapa[x][y] = 2;
					posledny = kroky;
				}
			    break;
		      	
		    default:
		    	break;
		}
		
		if (x < 0 || y < 0 || x == vyska || y == sirka || sucet == poklady)			//ak som mimo mapy, koncim fitnes funkciu
			break;
		
		if (i == 64)							//pokial som presiahol pocet buniek vraciam sa na prvu bunku
			i = 0;
	}
	
	int pomoc = sucet;
	
	if (pomoc > 0)								//reset mapy
		for (i = 0; i < vyska; i++)
			for (j = 0; j < sirka; j++){
				if (mapa[i][j] == 2){
					pomoc--;
					mapa[i][j] = 1;
				}
				if (pomoc == 0)
					break; 
			}
		
	if (sucet != poklady)						//pokial funkcia nenasla vsetky poklady vracia tuto hodnotu
		return sucet * 1000 + j;
		
	return sucet * 1000 + 500 - posledny;		//ak nasla vsetky poklady vracia tuto
}

void vypisPostupnost(char *bity, char **mapa, int vyska, int sirka, int start_x, int start_y, int poklady){		//funkcia identicka s fitnes funkciou akurat bez vratenia hodnoty
	unsigned char stroj[64];																					//a s vypisovanim danych krokov na konzolu
	int i, j, x = start_x, y = start_y, sucet = 0, posledny = 0, kroky = 0;
	unsigned char instr, adr, pohyb;
	
	printf("\n");	
	for (i = 0; i < vyska; i++){
		for (j = 0; j < sirka; j++)
			if (i == start_x && j == start_y)
				printf("S ");
			else
				printf("%d ",mapa[i][j]);
		printf("\n");
	}
	 
	/*for (i = 0; i < vyska; i++)
		for (j = 0; j < sirka; j++)
			mapka[i][j] = mapa[i][j];*/
				
	for (i = 0; i < 64; i++){
		stroj[i] = bity[i];
	}
	
	i = 0;
	
	for (j = 0; j < 500; j++){
		instr = stroj[i] >> 6;
		adr = stroj[i] << 2;
		adr = adr >> 2;
		
		i++;
		switch (instr){
		    case 0:
		      	stroj[adr]++;
		      	break;
		
		    case 1:
			    stroj[adr]--;
			    break;
		    
			case 2:
		   		i = adr;
		      	break;
		      
		    case 3:									//pri pohybe sa vypisuju na konzolu aj kroky riesenia
		    	kroky++;
		      	pohyb = stroj[adr] << 6;
				pohyb = pohyb >> 6;
				if (pohyb == 0){
					x--;
					printf("H");
				}
				else if (pohyb == 1){
					x++;
					printf("D");
				}
				else if (pohyb == 2){
					y++;
					printf("P");
				}
				else{
					y--;
					printf("L");
				}
					
				if (x < 0 || y < 0 || x == vyska || y == sirka)
					break;
				
				if (mapa[x][y] == 1){
					sucet++; 
					mapa[x][y] = 2;
					posledny = kroky;
				}
			    break;
		      	
		    default:
		    	break;
		}
		
		if (x < 0 || y < 0 || x == vyska || y == sirka || sucet == poklady)
			break;
		
		if (i == 64)
			i = 0;
	}
	
	int pomoc = sucet;
	
	if (pomoc > 0)		
		for (i = 0; i < vyska; i++)
			for (j = 0; j < sirka; j++){
				if (mapa[i][j] == 2){
					pomoc--;
					mapa[i][j] = 1;
				}
				if (pomoc == 0)
					break; 
			}
				
	printf("\nPocet krokov: %d\n", posledny);
	
	if (poklady != sucet)
		printf("Pocet najdenych pokladov: %d\n", sucet);
	
	printf("\n");
}

int main() {
	FILE *f;										//CITANIE ZO SUBORU A INICIALIZACIA
	f = fopen("ui_zadanie3.txt", "r");
	char buff[5];
	
	int vyska, sirka, start_x, start_y, pocet_pokladov, x, y, i, j, k;
	
	char **mapa;
	
	fscanf(f, "%s", buff);
	vyska = atoi(buff);
	fscanf(f, "%s", buff);
	sirka = atoi(buff);
	
	mapa = (char **) malloc (vyska * sizeof(char*));	
	
	for (i = 0; i < vyska; i++)
		mapa[i] = (char *) malloc (sirka * sizeof(char));
			
	for (i = 0; i < vyska; i++)
		for (j = 0; j < sirka; j++)
			mapa[i][j] = 0;
	
	fscanf(f, "%s", buff);
	start_x = atoi(buff);
	fscanf(f, "%s", buff);
	start_y = atoi(buff);
	fscanf(f, "%s", buff);
	pocet_pokladov = atoi(buff);
	
		
	for (i = 0; i < pocet_pokladov; i++){
		fscanf(f, "%s", buff);
		x = atoi(buff);
		fscanf(f, "%s", buff);
		y = atoi(buff);
		mapa[x][y] = 1;
	}
	
	/*for (i = 0; i < vyska; i++){
		for (j = 0; j < sirka; j++)
			if (i == start_x && j == start_y)
				printf("S ");
			else
				printf("%d ",mapa[i][j]);
		printf("\n");
	}*/
	
	int generacie, selekcia, premutovanie;
	
	printf("Kolko si prajete generacii do prerusenia programu?\n");
	scanf("%d", &generacie);
	printf("Ako sa maju vyberat rodicia?\n1 -> ruleta   2 -> turnaj\n");
	scanf("%d", &selekcia);
	printf("Po kolkych nepodarenych generaciach si prajete premutovat vacsinu prvkov?\nPokial nechcete premutovavat zadajte 0\n");
	scanf("%d", &premutovanie);
		
	int hodnoty_fitnes[N];
	unsigned char jedince[N][64];
	
	srand(time(0)); 
	
	for (i = 0; i < N; i++)						//nahodna generacia jedincov
		for (j = 0; j < 64; j++)
			jedince[i][j] = rand();
	

	
	int l, najlepsie_riesenie = 0;
	
	clock_t start, koniec;						//meranie casu pre testovanie
	double cas;
	
	start = clock();
	for (l = 0; l < 50000000; l++){													//ZACIATOK FORCYKLU KDE BEZI CELY ALGORITMUS
		int celkom = 0, max = 0, maxi = 0;
		
		for (i = 0; i < N; i++){								//fitnes hodnoty a priprava na ruletu
			hodnoty_fitnes[i] = fitnes(jedince[i], mapa, vyska, sirka, start_x, start_y, pocet_pokladov);
			
			if (selekcia == 1)
				celkom += hodnoty_fitnes[i];
			
			if (max < hodnoty_fitnes[i]){
				max = hodnoty_fitnes[i];
				maxi = i;	
			}

			
			if (hodnoty_fitnes[i] > pocet_pokladov * 1000 && hodnoty_fitnes[i] > najlepsie_riesenie)
				break;
		}
		
		if (i < N && hodnoty_fitnes[i] > pocet_pokladov * 1000 && hodnoty_fitnes[i] > najlepsie_riesenie){		//pri najdeni riesenia vypis riesenia a moznost pokracovat
			koniec = clock();
    		cas = ((double) (koniec - start)) / CLOCKS_PER_SEC;
			 
			printf("\nNasiel som riesenie\n");
			printf("Cas hladania: %lf\n",cas);
			
			najlepsie_riesenie = hodnoty_fitnes[i];
			vypisPostupnost(jedince[i], mapa, vyska, sirka, start_x, start_y, pocet_pokladov);
			printf("Generacia: %d\n", l);
			
			int volba;
			printf("Prajete si pokracovat a najst lepsiu cestu? (Nemusi to byt mozne)\n1 -> ano, 2 -> nie\n");
			scanf("%d", &volba);
			
			if (volba == 1){	
				l = 0;
				start = clock();
			}
			else		
				break;
		}
		
		
		if ((l % generacie == 0) && l > 0){														//pri pozadovanom pocte generacii vypis zatial najlepsieho a moznost pokracovat
			printf("\n\nPresiel pozadovany pocet generacii.\nRiesene sa nepodarilo najst.\nNajlepsie najdene riesenie\n");
			vypisPostupnost(jedince[maxi], mapa, vyska, sirka, start_x, start_y, pocet_pokladov);
			int volba;
			printf("Prajete si pokracovat?\n1 -> pokracovat s aktualnou generaciou,  2 -> pokracovat a silne zmutovat generaciu,  3 -> ukoncit program\n");
			scanf("%d", &volba);
			
			if (volba == 2){
				for (i = 0; i < N; i++)
					for (j = 0; j < 64; j++)
						if (rand() % 3 == 0)
							jedince[i][j] = rand();
			}
			else if (volba == 3){
				break;
			}
		}
		else if (premutovanie > 0 && (l % premutovanie == 0) && l > 0){				//pri pozadovanom pocte generacii na premutovanie silna mutacia v generacii
			printf("Mutujem tretinu buniek vsetkych prvkov\n");
			for (i = 0; i < N; i++)
				for (j = 0; j < 64; j++)
					if (rand() % 3 == 0)
						jedince[i][j] = rand();
		}
		else{
			
			char rodicia[10][64];
			int cislo, pom = 0;
			
			if (selekcia == 1){												//RULETA
				for (i = 0; i < 64; i++){									//jedinec s najvyssou fitnes ostane vzdy v generacii
					rodicia[0][i] = jedince[maxi][i];
				}
				
				for (i = 1; i < 10; i++){			
					pom = 0;
					cislo = (rand() * rand()) % celkom;						//nahodne vygenerujem ktore jedince budu rodicmi (cim vacsia fitnes hodnota, tym vacsia sanca)
					for (j = 0; j < N; j++){
						pom += hodnoty_fitnes[j];
						if (pom >= cislo){
							for (k = 0; k < 64; k++)
								rodicia[i][k] = jedince[j][k];
							break;
						}		
					}
				}
			}
			else{															//TURNAJ
				max = 0;
				maxi = 0;
				
				for (i = 0; i < N; i++){									//najlepsi z kazdej skupiny po 10 jedincov sa stane rodicom
					if (i > 0 && i % 10 == 0){
						for (k = 0; k < 64; k++)
							rodicia[(i / 10) - 1][k] = jedince[maxi][k];
						max = 0;
						maxi = 0;
					}
					if (max < hodnoty_fitnes[i]){
						max = hodnoty_fitnes[i];
						maxi = i;
					}
				}
				
				for (k = 0; k < 64; k++)
					rodicia[9][k] = jedince[maxi][k];
			}
			
			
			for (i = 0; i < 10; i++)				//PRVYCH 10 JEDINCOV NOVEJ POPULACIE BUDU RODICIA
				for (k = 0; k < 64; k++)
					jedince[i][k] = rodicia[i][k];
			
			int index = 9, zlom;
			
			pom = 10 + rand() % 90;	
			
			for (i = 0; i < 10; i++){											//KRIZENIE A NIEKEDY MUTACIA 
				zlom = rand() % 64;
				for (j = 0; j < 10; j++)
					if (i != j){
						index += 1;
						if (index == pom){										//1 jedinec v generacii bude cisto nahodny
							for (k = 0; k < 64; k++)
								jedince[index][k] = rand();						
						}
						else{	
							for (k = 0; k < 64; k++)							//jednobodove krizenie (1 cast z jedneho rodica, druha z druheho rodica)
								if (k < zlom)
									jedince[index][k] = rodicia[i][k];
								else
									jedince[index][k] = rodicia[j][k];
											
							if (rand() % 7 == 1){								//+-15% pravdepodobnost ze krizenemu jedincovi vygenerujem 5 nahodnych buniek
								for (k = 0; k < 5; k++)
									jedince[index][rand() % 64] = rand();
							}
							else if (rand() % 20 == 2){							//+-4% pravdepodobnost ze krizenemu jedincovi vymenim 2 susedne bunky medzi sebou
								unsigned char a, b;
								b = rand() % 63;
								
								a = jedince[index][b];
								jedince[index][b] = jedince[index][b + 1];
								jedince[index][b + 1] = a;
							}
							else if (rand() % 50 == 3){							//+-2% pravdepodobnost ze krizenemu jedincovi invertujem kazdy bit
								for (k = 0; k < 64; k++)
									jedince[index][k] = -jedince[index][k] - 1;
							}
						}
					}
			}
		}
		
	}
	return 0;
}
