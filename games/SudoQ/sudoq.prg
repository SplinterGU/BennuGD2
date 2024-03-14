/*
Autor : Bomberlink
Grupo : BoMbErGaMeS
Fecha : 09 - 04 - 06

BennuGD2 Port by SplinterGU (2021/07/11)
*/

PROGRAM SudoQ;

import "libmod_gfx";
import "libmod_input";
import "libmod_misc";
import "libmod_sound";

declare PROCESS cuadrado(double x, y, int graph,numero);
declare PROCESS boton(double x,y, int graph);

GLOBAL

	STRING nombre_fichero="";

	int otro_mas;
	int espera;
	int mini_cuadro;
	int posicion_cuadro;
	int cambio_cuadro;

	int tablero[82];
	int tablero_color[82];
	int tablero_copia[82];

	int fichero;
	
	STRUCT savegame;
		int tablero[82];
		int tablero_color[82];
		int tablero_copia[82];
		int dificultad;
		int seg; int min; int hora;
		int sudoku_finalizado;
		int skin;
	end

	int guarda;
	int sudoku_aleatoriox=1;
	int graficos;
	int procesando=1;
	int tecla_pulsada;
	int dificultad=1;
	int seg; int min; int hora;
	int contador1; int completo; int completo1;
	int tablero_completado;
	int sudoku_finalizado;
	int cambia_skin;
	int skin=22;
	int idtxt1;
	int cancion;
	int espera_cancion;
	int esconder_cuadro;
	int hay_repetidos;
	int tablero_dificultad;
	int escalado;

PRIVATE
	int dot_pos;

BEGIN
	set_fps (60, 1) ;
	set_mode (320,240);

	window_set_title("SudoQ v0.8 - Bombergames Soft 2016");

	graficos=fpg_load("sudoq.fpg");
	window_set_icon(graficos,18);
	x=160; y=120; graph=1;
	
	// cambia semilla
	rand_seed(time());

	boton(255,340,8);
	boton(255,340,9);
	boton(255,340,10);
	boton(255,340,11);
	boton(255,340,12);

	raton(1);

	// carga el tablero tal y como lo dejamos
	if (fexists(get_pref_path("bennugd.org", "sudoq") + "save.dat"))
		fichero=fopen(get_pref_path("bennugd.org", "sudoq") + "save.dat",O_READ);
		fread(fichero,savegame);
		fclose(fichero);

		// asigna estructura en tableros
		for(guarda=1;guarda<82;guarda++)
			tablero[guarda]=savegame.tablero[guarda];
			tablero_color[guarda]=savegame.tablero_color[guarda];
			tablero_copia[guarda]=savegame.tablero_copia[guarda];
		end

		dificultad=savegame.dificultad;
		seg=savegame.seg;
		min=savegame.min;
		hora=savegame.hora;
		sudoku_finalizado=savegame.sudoku_finalizado;
		skin=savegame.skin;

		sudoku_aleatoriox=2;
	else
		write(0,100,100,4,"PROCESSING");
		write(0,100,110,4,"WAIT ...");
		frame;
	end

	signal(type pon_pantalla,s_kill);
	pon_pantalla(graficos,skin);

	sudoku_aleatorio(sudoku_aleatoriox);

	LOOP

		// MUESTRA O QUITA EL MENU DE BOTONES
		if (get_joy_button(0,JOY_BUTTON_A)==0) otro_mas=0; end

		if (otro_mas==0)
			if (get_joy_button(0,JOY_BUTTON_A)==1) 
				if (esconder_cuadro==0) 
					esconder_cuadro=1; otro_mas=1; 
				else 
					esconder_cuadro=0; otro_mas=1; 
				end 
			end
		end

		// BUSCA EN EL DIRECTORIO DE MUSICA ALGO QUE REPRODUCIR
		if (espera_cancion>0) espera_cancion--; end
	
		if (espera_cancion==0)
			if (get_joy_button(0,JOY_BUTTON_RIGHTSTICK) || NOT music_is_playing())
				nombre_fichero = glob("music/*");
				while( nombre_fichero != "" )
					if ( nombre_fichero != "." && nombre_fichero != ".." )
						dot_pos = find(strrev(nombre_fichero), ".");
						if ( dot_pos > -1 )
							switch ( lcase( substr(nombre_fichero, -dot_pos) ) )
								case	"opus", "mod", "s3m", "wav", "mp3", "xm":
									// Valid
									break;
								end
								case 	"flac", "ogg", "mid":
									if ( os_id != OS_SWITCH )
										break;
									end
								end
							end
						end
					end
					nombre_fichero = glob("music/*");
				end
				if ( nombre_fichero != "" )
					cancion=music_load("music/"+nombre_fichero);
					music_play(cancion,0); espera_cancion=30;
				else
					glob(""); // Reset glob
				end
			end
		end

		if (procesando==0)
			// cambia skins
			if ((get_joy_button(0,JOY_BUTTON_LEFTSTICK) || key(_a)) && cambia_skin==0) skin--; if (skin<20) skin=23; end skins(); cambia_skin=1; end
			if ( key(_d) && cambia_skin==0) skin++; if (skin>23) skin=20; end skins(); cambia_skin=1; end
		
			// cambia modo ventana o pantalla completa
			if (key(_enter) && key(_alt)) if (screen.fullscreen==true) screen.fullscreen=false; else screen.fullscreen=true; end set_mode (320,240); end

			if ( get_joy_button(0,JOY_BUTTON_RIGHTSHOULDER) )
				switch ( dificultad )
					case 1:
						dificultad = 2;
					end

					case 2:
						dificultad = 3;
					end

					case 3:
						dificultad = 1;
					end
				end
				while ( get_joy_button(0,JOY_BUTTON_RIGHTSHOULDER) ) frame; end
			end
		end

		// comprobamos si el tablero ya esta completo
		if (tablero_completado==0 && procesando==0)

			contador1++;

			// comparacion con tablero original
			if (tablero_copia[contador1]==tablero[contador1]) completo++; end

			// comprueba si se han rellenado todas las casillas, pero no su coherencia
			if (tablero[contador1]!=0 && completo1<82) completo1++; end

			// borra comentario provisional
			if (idtxt1!=0 && completo1!=82) write_delete(idtxt1); idtxt1=0; hay_repetidos=0; end

			if (contador1>80)
				if (completo>80) 
					write(0,230,211,4,"COMPLETED!"); sudoku_finalizado++; tablero_completado=1;
				else
					// comparacion "fuerza bruta" secundaria de tablero con multiples soluciones , sudoku invalido
					if (tablero[1]+tablero[2]+tablero[3]+tablero[4]+tablero[5]+tablero[6]+tablero[7]+tablero[8]+tablero[9]==45 &&
					tablero[10]+tablero[11]+tablero[12]+tablero[13]+tablero[14]+tablero[15]+tablero[16]+tablero[17]+tablero[18]==45 &&
					tablero[19]+tablero[20]+tablero[21]+tablero[22]+tablero[23]+tablero[24]+tablero[25]+tablero[26]+tablero[27]==45 &&
					tablero[28]+tablero[29]+tablero[30]+tablero[31]+tablero[32]+tablero[33]+tablero[34]+tablero[35]+tablero[36]==45 &&
					tablero[37]+tablero[38]+tablero[39]+tablero[40]+tablero[41]+tablero[42]+tablero[43]+tablero[44]+tablero[45]==45 &&
					tablero[46]+tablero[47]+tablero[48]+tablero[49]+tablero[50]+tablero[51]+tablero[52]+tablero[53]+tablero[54]==45 &&
					tablero[55]+tablero[56]+tablero[57]+tablero[58]+tablero[59]+tablero[60]+tablero[61]+tablero[62]+tablero[63]==45 &&
					tablero[64]+tablero[65]+tablero[66]+tablero[67]+tablero[68]+tablero[69]+tablero[70]+tablero[71]+tablero[72]==45 &&
					tablero[73]+tablero[74]+tablero[75]+tablero[76]+tablero[77]+tablero[78]+tablero[79]+tablero[80]+tablero[81]==45 &&

					tablero[1]+tablero[10]+tablero[19]+tablero[28]+tablero[37]+tablero[46]+tablero[55]+tablero[64]+tablero[73]==45 &&
					tablero[2]+tablero[11]+tablero[20]+tablero[29]+tablero[38]+tablero[47]+tablero[56]+tablero[65]+tablero[74]==45 &&
					tablero[3]+tablero[12]+tablero[21]+tablero[30]+tablero[39]+tablero[48]+tablero[57]+tablero[66]+tablero[75]==45 &&
					tablero[4]+tablero[13]+tablero[22]+tablero[31]+tablero[40]+tablero[49]+tablero[58]+tablero[67]+tablero[76]==45 &&
					tablero[5]+tablero[14]+tablero[23]+tablero[32]+tablero[41]+tablero[50]+tablero[59]+tablero[68]+tablero[77]==45 &&
					tablero[6]+tablero[15]+tablero[24]+tablero[33]+tablero[42]+tablero[51]+tablero[60]+tablero[69]+tablero[78]==45 &&
					tablero[7]+tablero[16]+tablero[25]+tablero[34]+tablero[43]+tablero[52]+tablero[61]+tablero[70]+tablero[79]==45 &&
					tablero[8]+tablero[17]+tablero[26]+tablero[35]+tablero[44]+tablero[53]+tablero[62]+tablero[71]+tablero[80]==45 &&
					tablero[9]+tablero[18]+tablero[27]+tablero[36]+tablero[45]+tablero[54]+tablero[63]+tablero[72]+tablero[81]==45)

						write(0,230,211,4,"DONE: Multiple solution"); sudoku_finalizado++; tablero_completado=1; 
					end
				end
			
				// escribe mensaje guia al tener el tablero completado con errores
				if (tablero_completado==0 && completo1==81) 
					idtxt1=write(0,230,211,4,"ERROR: Repeated numbers"); completo1=82; hay_repetidos=1;
				end
			
				completo=0; 
				contador1=0;
				if (completo1<82) hay_repetidos=0; completo1=0; end
			end		

			// Contador de tiempo
			if (procesando==0)
				if (timer[0]>99) seg++; timer[0]=0; end
				if (seg>59) min++; seg=0; end
				if (min>59) hora++; min=0; end
			end
		end
		
		// agranda ventana
		if (escalado==1) if(key(_f1)) screen.scale_resolution = 03200240; set_mode (320,240); escalado=0; end end
		if (escalado==0) if(key(_f2)) screen.scale_resolution = 06400480; set_mode (320,240); escalado=1; end end
		

		// Salir del programa
		if (key(_esc) || wm_info.exit_status==1)

			if (tablero_completado==0) // si el tablero estaba a medias lo guardamos

				// asigna tableros en estructura
				for(guarda=1;guarda<82;guarda++)
					savegame.tablero[guarda]=tablero[guarda];
					savegame.tablero_color[guarda]=tablero_color[guarda];
					savegame.tablero_copia[guarda]=tablero_copia[guarda];
				end

				savegame.seg=seg;
				savegame.min=min;
				savegame.hora=hora;
			end

			// el resto de opciones siempre se guardaran
			savegame.dificultad=dificultad;
			savegame.sudoku_finalizado=sudoku_finalizado;
			savegame.skin=skin;

			fichero=fopen(get_pref_path("bennugd.org", "sudoq") + "save.dat",O_WRITE);
			fwrite(fichero ,savegame);
			fclose(fichero);

			exit(0,0);
		end
	
		frame;
	end
end

// fondos del programa
PROCESS skins()
PRIVATE 
	int fundido;
	
BEGIN 
	x=160; y=120; graph=7; alpha=0; z=500;
	LOOP
	
	if (fundido==0)
		if (alpha<255) 
			alpha+=5; 
		else 
			signal(type pon_pantalla,s_kill); pon_pantalla(graficos,skin); fundido=1; 
		end
	else
		if (alpha>0) alpha-=5; else cambia_skin=0; signal(id,s_kill); end
	end

	frame; 
	end 
end

// ALGORITMO SUDOKU
PROCESS sudoku_aleatorio(tipo)
PRIVATE
	int rondas;
	int contador;
	int copia;
	int random;
	int random1;	
	int i;
	int b;
	int c;
	int d;
	int e;
	int borrados;
	int tablero_borrar[82];
	
	// todas las variables de rutina
	int calculo[10];
	int comparacion[10];
	int real_x;
	int real_y;
	int compara_x;
	int compara_y;
	int compara_tablerox;
	int compara_tablerox1;
	int compara_tableroy;
	int compara_tableroy1;
	int cuadricula_x1;
	int cuadricula_x2;
	int cuadricula_y1;
	int cuadricula_y2;
	int cuantos1;
	int cuantos2;
	int cuantos3;
	int cuantos4;
	int busca_numero1;
	int busca_numero2;
	int compara_1;
	int compara_2;
	int compara_3;
	int compara_4;
	
	int compara_[5];
	int optimiza_1;
	int optimiza_2;
	int optimiza_3;
	int optimiza_4;
	int optimiza_5;
	int optimiza_6;

BEGIN

	tablero_completado=1;
	hay_repetidos=0;
	tablero_dificultad=dificultad;

	// borra y reinicializa contenido anterior
	signal(type cuadrado,s_kill);
	write_delete(all_text);

	if (tipo==2) copia=1; end // si el tablero se carga desde archivo la copia no sera necesaria

	if (tipo==0)

		// devuelve valores originales al tablero para resolver puzzle
		for(contador=1;contador<82;contador++)
			tablero[contador]=tablero_copia[contador];
		end
	end

	procesando=1;

	if (tipo==1)
		min=0; hora=0; timer[0]=0; seg=0;

		// rellena segun dificultad
		if (dificultad==3) rondas=rand(300,350); end
		if (dificultad==2) rondas=rand(130,170); end
		if (dificultad==1) rondas=rand(80,120); end

		mini_cuadro=-1; 
		signal(type pixel,s_kill); signal(type raton,s_kill); signal(type pixel1,s_kill); 
		signal(type raton1,s_kill); raton(1); signal(type semi_cuadro,s_kill); signal(type baldosa,s_kill);
		write(0,100,100,4,"PROCESSING");
		write(0,100,110,4,"WAIT ...");
		frame;

		// pone casillas a cero
		for(contador=1;contador<81;contador++)
			tablero[contador]=0;			
		end

		coloca();
	end

	// crea una copia del tablero
	if (copia==0)
		
		for(contador=1;contador<82;contador++)
			tablero_copia[contador]=tablero[contador];
		end

		copia=1;
	end
		
	// BUSCANDO CASILLAS CONFLICTIVAS
	if (sudoku_aleatoriox==1)
	
		for (e=1;e<6;e++)
			
			for(i=1;i<82;i++)
				random1=i;
					
				// posicion en el tablero
				compara_tableroy=1+(random1-1)/9;
				compara_tablerox=(9+random1)-(compara_tableroy*9);
					
				// cuadricula
				compara_x=1; real_x=compara_tablerox;
				if (compara_tablerox>3) compara_x=2; real_x=compara_tablerox-3; end
				if (compara_tablerox>6) compara_x=3; real_x=compara_tablerox-6; end
					
				compara_y=1; real_y=compara_tableroy;
				if (compara_tableroy>3) compara_y=2; real_y=compara_tableroy-3; end
				if (compara_tableroy>6) compara_y=3; real_y=compara_tableroy-6; end
					
				// cuadricula donde se encuentra el numero a eliminar
				for (d=1;d<4;d++)
					calculo[d]=d+((compara_x-1)*3)+((compara_y-1)*27);
					calculo[d+3]=(d+9)+((compara_x-1)*3)+((compara_y-1)*27);
					calculo[d+6]=(d+18)+((compara_x-1)*3)+((compara_y-1)*27);
				end

				// reorganiza condiciones segun posicion
				comparacion[1]=real_x;
				comparacion[2]=3+real_x;
				if (comparacion[2]>6) comparacion[2]-=3; end
				comparacion[3]=6+real_x;
				if (comparacion[3]>9) comparacion[3]-=3; end
				comparacion[4]=1+real_x;
				if (comparacion[4]>3) comparacion[4]-=3; end
				comparacion[5]=2+real_x;
				if (comparacion[5]>3) comparacion[5]-=3; end
				comparacion[6]=4+real_x;
				if (comparacion[6]>6) comparacion[6]-=3; end
				comparacion[7]=7+real_x;
				if (comparacion[7]>9) comparacion[7]-=3; end
				comparacion[8]=5+real_x;
				if (comparacion[8]>6) comparacion[8]-=3; end
				comparacion[9]=8+real_x;						
				if (comparacion[9]>9) comparacion[9]-=3; end					
					
				for (c=1;c<10;c++)
					comparacion[c]+=(3*(real_y-1));
					if (comparacion[c]>9) comparacion[c]-=9; end
				end
				
				// NUMEROS MAS CONFLICTIVOS
				if (e==1)	
				
					// COMPARACIONES
					for (b=2;b<10;b++)
						busca_numero1=tablero_copia[calculo[comparacion[1]]];
						busca_numero2=tablero_copia[calculo[comparacion[b]]];
						compara_tableroy1=1+(calculo[comparacion[b]]-1)/9;
						compara_tablerox1=(9+calculo[comparacion[b]])-(compara_tableroy1*9);
							
						cuantos1=0;
						cuantos2=0;
						cuantos3=0;
						cuantos4=0;
						
						if (tablero_borrar[calculo[comparacion[b]]]<5)
						
							for (c=1;c<10;c++)
									
								// no permite comparaciones horizontales en combinaciones verticales
								if (b!=4 && b!=5)
										
									// mira el numero condicional inverso
									if (tablero_copia[c+((compara_tableroy-1)*9)]==busca_numero2 && tablero_borrar[c+((compara_tableroy-1)*9)]<5)  
										cuantos1=1; 
										cuadricula_x1=1;
										if (c>3) cuadricula_x1=2; end
										if (c>6) cuadricula_x1=3; end
									end
										
									// mira el numero target inverso
									if (tablero_copia[c+((compara_tableroy1-1)*9)]==busca_numero1 && tablero_borrar[c+((compara_tableroy1-1)*9)]<5)
										cuantos2=1;
										cuadricula_x2=1;
										if (c>3) cuadricula_x2=2; end
										if (c>6) cuadricula_x2=3; end									
									end 
											
									// esta presente?
									if (cuantos1>0 && cuantos2>0)
										
										// en la misma casilla
										if (cuadricula_x1==cuadricula_x2)
											tablero_borrar[random1]++;
											if (b==2) tablero_borrar[random1]++; end
											cuantos1=0; cuantos2=0;
										end	
										
									end
								end
							
								// no permite comparaciones verticales en combinaciones horizontales
								if (b!=2 && b!=3)
											
									// condicional inverso
									if (tablero_copia[compara_tablerox+(c-1)*9]==busca_numero2 && tablero_borrar[compara_tablerox+(c-1)*9]<5)
										cuantos3=1;
										cuadricula_y1=1;
										if (c>3) cuadricula_y1=2; end
										if (c>6) cuadricula_y1=3; end									
									end	
											
									// target
									if (tablero_copia[compara_tablerox1+(c-1)*9]==busca_numero1 && tablero_borrar[compara_tablerox1+(c-1)*9]<5)
										cuantos4=1;
										cuadricula_y2=1;
										if (c>3) cuadricula_y2=2; end
										if (c>6) cuadricula_y2=3; end									
									end
											
									// esta presente?
									if (cuantos3>0 && cuantos4>0)
										
										// en la misma casilla
										if (cuadricula_y1==cuadricula_y2)
											tablero_borrar[random1]++;
											if (b==4) tablero_borrar[random1]++; end
											cuantos3=0; cuantos4=0;
										end	
									end				
								end
							end	
						end	
					end		
				end
				
				// NUMEROS MENOS CONFLICTIVOS PERO POTENCIALMENTE PELIGROSOS
				if (e==2)
					
					// el objetivo no esta presente
					if (tablero_borrar[random1]<5)
					
						// COMPARACIONES DIRECTAS, SOLO POR LINEA X/Y
						for (b=2;b<6;b++)
							busca_numero1=tablero_copia[calculo[comparacion[1]]];
							busca_numero2=tablero_copia[calculo[comparacion[b]]];
							compara_tableroy1=1+(calculo[comparacion[b]]-1)/9;
							compara_tablerox1=(9+calculo[comparacion[b]])-(compara_tableroy1*9);
							
							// solo si los 4 numeros a comparar no estan entre los conflictivos
							if (tablero_borrar[calculo[comparacion[b]]]<5)
								
								for (c=1;c<4;c++)
										
									// no permite comparaciones horizontales en combinaciones verticales
									if (b<4)
										
										if (compara_x==1) compara_1=3+c; compara_2=6; end
										if (compara_x==2) compara_1=0+c; compara_2=6+c; end
										if (compara_x==3) compara_1=0+c; compara_2=3+c; end
										
										// mira el numero condicional inverso
										if (tablero_copia[compara_1+((compara_tableroy-1)*9)]==busca_numero2)  
											// mira el numero target inverso
											if (tablero_copia[compara_1+((compara_tableroy1-1)*9)]==busca_numero1)
												// el numero vecino no estaba tachado
												if (tablero_borrar[compara_1+((compara_tableroy-1)*9)]<5)
													// el otro tampoco
													if (tablero_borrar[compara_1+((compara_tableroy1-1)*9)]<5)
														tablero_borrar[random1]+=10; // asegura ser tachado
														break;
													end													 
												end	
											end
										end
										
										// mira el numero condicional inverso
										if (tablero_copia[compara_2+((compara_tableroy-1)*9)]==busca_numero2)  
											// mira el numero target inverso
											if (tablero_copia[compara_2+((compara_tableroy1-1)*9)]==busca_numero1)
												// el numero vecino no estaba tachado
												if (tablero_borrar[compara_2+((compara_tableroy-1)*9)]<5)
													// el otro tampoco
													if (tablero_borrar[compara_2+((compara_tableroy1-1)*9)]<5)
														tablero_borrar[random1]+=10; // asegura ser tachado
														break;
													end													 
												end	
											end
										end										
										
									// no permite comparaciones verticales en combinaciones horizontales	
									else
										
										if (compara_y==1) compara_3=3+c; compara_4=6; end
										if (compara_y==2) compara_3=0+c; compara_4=6+c; end
										if (compara_y==3) compara_3=0+c; compara_4=3+c; end
										
										// condicional inverso
										if (tablero_copia[compara_tablerox+(compara_3-1)*9]==busca_numero2)
											// target
											if (tablero_copia[compara_tablerox1+(compara_3-1)*9]==busca_numero1)											
												// vecino no tachado
												if (tablero_borrar[compara_tablerox+(compara_3-1)*9]<5)
													// tampoco
													if (tablero_borrar[compara_tablerox1+(compara_3-1)*9]<5)
														tablero_borrar[random1]+=10; // asegura ser tachado
														break;
													end
												end
											end	
										end			
														
										// condicional inverso
										if (tablero_copia[compara_tablerox+(compara_4-1)*9]==busca_numero2)
											// target
											if (tablero_copia[compara_tablerox1+(compara_4-1)*9]==busca_numero1)											
												// vecino no tachado
												if (tablero_borrar[compara_tablerox+(compara_4-1)*9]<5)
													// tampoco
													if (tablero_borrar[compara_tablerox1+(compara_4-1)*9]<5)
														tablero_borrar[random1]+=10; // asegura ser tachado														
														break;
													end
												end
											end	
										end									
									end
								end								
							end	
						end
					end
				end	
				
				// FILAS O COLUMNAS DE 3 ROTADAS EN 2 CUADRICULAS
				if (e==3)				
					// el objetivo no esta presente
					if (tablero_borrar[random1]<7)
						
						for (c=1;c<5;c++)
							for (b=0;b<3;b++)
						
								// cuadricula X
								if (c<3)
									optimiza_1=calculo[real_x];
									optimiza_2=calculo[real_x+3];
									optimiza_3=calculo[real_x+6];
								
									// sin tachar
									if (tablero_borrar[optimiza_1]<7 && tablero_borrar[optimiza_2]<7 && tablero_borrar[optimiza_3]<7)					
										if (compara_x==1) compara_[1]=3+b; compara_[2]=6+b; end
										if (compara_x==2) compara_[1]=-3+b; compara_[2]=3+b; end
										if (compara_x==3) compara_[1]=-6+b; compara_[2]=-3+b; end
									
										if (tablero_borrar[calculo[1]+compara_[c]]<7 && tablero_borrar[calculo[4]+compara_[c]]<7 && tablero_borrar[calculo[7]+compara_[c]]<7)
										
											// combinacion rotada
											if (tablero_copia[optimiza_1]==tablero_copia[calculo[7]+compara_[c]])
												if (tablero_copia[optimiza_2]==tablero_copia[calculo[1]+compara_[c]])
													if (tablero_copia[optimiza_3]==tablero_copia[calculo[4]+compara_[c]])
														tablero_borrar[random1]+=10; // conflictivo
														break;
													end
												end	
											end	
											
											if (tablero_copia[optimiza_1]==tablero_copia[calculo[4]+compara_[c]])
												if (tablero_copia[optimiza_2]==tablero_copia[calculo[7]+compara_[c]])
													if (tablero_copia[optimiza_3]==tablero_copia[calculo[1]+compara_[c]])
														tablero_borrar[random1]+=10; // conflictivo
														break;
													end
												end	
											end																
										end	
									end
									
								// cuadricula Y
								else							
									optimiza_4=calculo[1+((real_y-1)*3)];
									optimiza_5=calculo[2+((real_y-1)*3)];
									optimiza_6=calculo[3+((real_y-1)*3)];
									
									// sin tachar
									if (tablero_borrar[optimiza_4]<7 && tablero_borrar[optimiza_5]<7 && tablero_borrar[optimiza_6]<7)
										if (compara_y==1) compara_[3]=27-((real_y-1)*9)+(b*9); compara_[4]=54-((real_y-1)*9)+(b*9); end
										if (compara_y==2) compara_[3]=-27-((real_y-1)*9)+(b*9); compara_[4]=27-((real_y-1)*9)+(b*9); end
										if (compara_y==3) compara_[3]=-54-((real_y-1)*9)+(b*9); compara_[4]=-27-((real_y-1)*9)+(b*9); end						
										
										if (tablero_borrar[optimiza_4+compara_[c]]<7 && tablero_borrar[optimiza_5+compara_[c]]<7 && tablero_borrar[optimiza_6+compara_[c]]<7)
										
											if (tablero_copia[optimiza_4]==tablero_copia[optimiza_6+compara_[c]])
												if (tablero_copia[optimiza_5]==tablero_copia[optimiza_4+compara_[c]])
													if (tablero_copia[optimiza_6]==tablero_copia[optimiza_5+compara_[c]])											
														tablero_borrar[random1]+=10; // conflictivo
														break;
													end
												end	
											end	
											
											if (tablero_copia[optimiza_4]==tablero_copia[optimiza_5+compara_[c]])
												if (tablero_copia[optimiza_5]==tablero_copia[optimiza_6+compara_[c]])
													if (tablero_copia[optimiza_6]==tablero_copia[optimiza_4+compara_[c]])
														tablero_borrar[random1]+=10; // conflictivo
														break;
													end
												end	
											end					
										end
									end
								end		
							end		
						end	
					end	
				end	

				// COMBINACION TRIPLE CRUZADA (3 CUADRICULAS)
				if (e==4)
					// el objetivo no esta presente
					if (tablero_borrar[random1]<5)
						
						for (c=1;c<3;c++)
							for (b=0;b<3;b++)
								for (d=0;d<3;d++)
						
									// cuadricula X
									if (c<2)
										optimiza_1=calculo[real_x];
										optimiza_2=calculo[real_x+3];
										optimiza_3=calculo[real_x+6];
									
										// sin tachar linea target
										if (tablero_borrar[optimiza_1]<5 && tablero_borrar[optimiza_2]<5 && tablero_borrar[optimiza_3]<5)
											if (compara_x==1) compara_[1]=3+b; compara_[2]=6+d; end
											if (compara_x==2) compara_[1]=-3+b; compara_[2]=3+d; end
											if (compara_x==3) compara_[1]=-6+b; compara_[2]=-3+d; end

											// sin tachar las otras 2 simultaneas
											if (tablero_borrar[calculo[1]+compara_[1]]<5 && tablero_borrar[calculo[4]+compara_[1]]<5 && tablero_borrar[calculo[7]+compara_[1]]<5)
												if (tablero_borrar[calculo[1]+compara_[2]]<5 && tablero_borrar[calculo[4]+compara_[2]]<5 && tablero_borrar[calculo[7]+compara_[2]]<5)
												
													// comparacion triple A
													if (tablero_copia[optimiza_1]==tablero_copia[calculo[7]+compara_[1]])
														if (tablero_copia[optimiza_3]==tablero_copia[calculo[1]+compara_[2]])
															
															// 1
															if (tablero_copia[calculo[1]+compara_[1]]==tablero_copia[calculo[7]+compara_[2]])
																tablero_borrar[random1]+=6; // conflictivo
																break;																
															end
															
															// 2
															if (tablero_copia[calculo[4]+compara_[1]]==tablero_copia[calculo[7]+compara_[2]])
																tablero_borrar[random1]+=6; // conflictivo
																break;																
															end															
															
															// 3
															if (tablero_copia[calculo[7]+compara_[1]]==tablero_copia[calculo[4]+compara_[2]])
																tablero_borrar[random1]+=6; // conflictivo
																break;																
															end
															
														end	
													end	
												end
											end	
										end	
										
									// cuadricula Y	
									else	
										optimiza_4=calculo[1+((real_y-1)*3)];
										optimiza_5=calculo[2+((real_y-1)*3)];
										optimiza_6=calculo[3+((real_y-1)*3)];
										
										// sin tachar
										if (tablero_borrar[optimiza_4]<5 && tablero_borrar[optimiza_5]<5 && tablero_borrar[optimiza_6]<5)
											if (compara_y==1) compara_[3]=27-((real_y-1)*9)+(b*9); compara_[4]=54-((real_y-1)*9)+(d*9); end
											if (compara_y==2) compara_[3]=-27-((real_y-1)*9)+(b*9); compara_[4]=27-((real_y-1)*9)+(d*9); end
											if (compara_y==3) compara_[3]=-54-((real_y-1)*9)+(b*9); compara_[4]=-27-((real_y-1)*9)+(d*9); end
											
											if (tablero_borrar[optimiza_4+compara_[3]]<5 && tablero_borrar[optimiza_5+compara_[3]]<5 && tablero_borrar[optimiza_6+compara_[3]]<5)
												if (tablero_borrar[optimiza_4+compara_[4]]<5 && tablero_borrar[optimiza_5+compara_[4]]<5 && tablero_borrar[optimiza_6+compara_[4]]<5)

													// comparacion triple A
													if (tablero_copia[optimiza_4]==tablero_copia[optimiza_6+compara_[3]])
														if (tablero_copia[optimiza_6]==tablero_copia[optimiza_4+compara_[4]])
															
															// 1
															if (tablero_copia[optimiza_4+compara_[3]]==tablero_copia[optimiza_6+compara_[4]])
																tablero_borrar[random1]+=6; // conflictivo
																break;																
															end
															
															// 2
															if (tablero_copia[optimiza_5+compara_[3]]==tablero_copia[optimiza_6+compara_[4]])
																tablero_borrar[random1]+=6; // conflictivo
																break;																
															end															
															
															// 3
															if (tablero_copia[optimiza_6+compara_[3]]==tablero_copia[optimiza_5+compara_[4]])
																tablero_borrar[random1]+=6; // conflictivo
																break;																
															end
															
															
														end	
													end	
												end
											end
										end		
									end
								end	
							end
						end
					end
				end
				
				// COMBINACION DOBLE CRUZADA (3 CUADRICULAS)
				if (e==5)
					// el objetivo no esta presente
					if (tablero_borrar[random1]<5)
						
						for (c=1;c<3;c++)
							for (b=0;b<3;b++)
								for (d=0;d<3;d++)
						
									// cuadricula X
									if (c<2)
										optimiza_1=calculo[real_x];
										optimiza_2=calculo[real_x+3];
										optimiza_3=calculo[real_x+6];
										if (compara_x==1) compara_[1]=3+b; compara_[2]=6+d; end
										if (compara_x==2) compara_[1]=-3+b; compara_[2]=3+d; end
										if (compara_x==3) compara_[1]=-6+b; compara_[2]=-3+d; end										
									
										// sin tachar linea target
										if (tablero_borrar[optimiza_1]<5 && tablero_borrar[optimiza_2]<5)
											if (tablero_borrar[calculo[1]+compara_[1]]<5 && tablero_borrar[calculo[4]+compara_[1]]<5)
												if (tablero_borrar[calculo[1]+compara_[2]]<5 && tablero_borrar[calculo[4]+compara_[2]]<5)
												
													// comparacion doble A
													if (tablero_copia[optimiza_1]==tablero_copia[calculo[4]+compara_[2]])
														if (tablero_copia[optimiza_2]==tablero_copia[calculo[1]+compara_[1]])
															if (tablero_copia[calculo[4]+compara_[1]]==tablero_copia[calculo[1]+compara_[2]])
																tablero_borrar[random1]+=6; // conflictivo
																break;																
															end	
														end	
													end	
													
												end
											end	
										end	

										// sin tachar linea target
										if (tablero_borrar[optimiza_1]<5 && tablero_borrar[optimiza_3]<5)
											if (tablero_borrar[calculo[1]+compara_[1]]<5 && tablero_borrar[calculo[7]+compara_[1]]<5)
												if (tablero_borrar[calculo[1]+compara_[2]]<5 && tablero_borrar[calculo[7]+compara_[2]]<5)
												
													// comparacion doble B
													if (tablero_copia[optimiza_1]==tablero_copia[calculo[7]+compara_[2]])
														if (tablero_copia[optimiza_3]==tablero_copia[calculo[1]+compara_[1]])
															if (tablero_copia[calculo[7]+compara_[1]]==tablero_copia[calculo[1]+compara_[2]])
																tablero_borrar[random1]+=6; // conflictivo
																break;																
															end	
														end	
													end	
													
												end
											end	
										end
										
										// sin tachar linea target
										if (tablero_borrar[optimiza_2]<5 && tablero_borrar[optimiza_3]<5)
											if (tablero_borrar[calculo[4]+compara_[1]]<5 && tablero_borrar[calculo[7]+compara_[1]]<5)
												if (tablero_borrar[calculo[4]+compara_[2]]<5 && tablero_borrar[calculo[7]+compara_[2]]<5)
												
													// comparacion doble C
													if (tablero_copia[optimiza_2]==tablero_copia[calculo[7]+compara_[2]])
														if (tablero_copia[optimiza_3]==tablero_copia[calculo[4]+compara_[1]])
															if (tablero_copia[calculo[7]+compara_[1]]==tablero_copia[calculo[4]+compara_[2]])
																tablero_borrar[random1]+=6; // conflictivo
																break;																
															end	
														end	
													end	
													
												end
											end	
										end		
										
										// sin tachar linea target
										if (tablero_borrar[optimiza_1]<5 && tablero_borrar[optimiza_2]<5)
											if (tablero_borrar[calculo[4]+compara_[1]]<5 && tablero_borrar[calculo[7]+compara_[1]]<5)
												if (tablero_borrar[calculo[7]+compara_[2]]<5)
												
													// comparacion doble D
													if (tablero_copia[optimiza_1]==tablero_copia[calculo[4]+compara_[1]])
														if (tablero_copia[optimiza_2]==tablero_copia[calculo[7]+compara_[1]])
															if (tablero_copia[optimiza_1]==tablero_copia[calculo[7]+compara_[2]])
																tablero_borrar[random1]+=6; // conflictivo
																break;																
															end	
														end	
													end	
													
												end
											end	
										end
										

										// sin tachar linea target
										if (tablero_borrar[optimiza_2]<5 && tablero_borrar[optimiza_3]<5)
											if (tablero_borrar[calculo[1]+compara_[1]]<5 && tablero_borrar[calculo[4]+compara_[1]]<5)
												if (tablero_borrar[calculo[1]+compara_[2]]<5)
												
													// comparacion doble E
													if (tablero_copia[optimiza_2]==tablero_copia[calculo[1]+compara_[1]])
														if (tablero_copia[optimiza_3]==tablero_copia[calculo[4]+compara_[1]])
															if (tablero_copia[optimiza_3]==tablero_copia[calculo[1]+compara_[2]])
																tablero_borrar[random1]+=6; // conflictivo
																break;																
															end	
														end	
													end	
													
												end
											end	
										end											
																				
									// cuadricula Y	
									else	
										optimiza_4=calculo[1+((real_y-1)*3)];
										optimiza_5=calculo[2+((real_y-1)*3)];
										optimiza_6=calculo[3+((real_y-1)*3)];									
										if (compara_y==1) compara_[3]=27-((real_y-1)*9)+(b*9); compara_[4]=54-((real_y-1)*9)+(d*9); end
										if (compara_y==2) compara_[3]=-27-((real_y-1)*9)+(b*9); compara_[4]=27-((real_y-1)*9)+(d*9); end
										if (compara_y==3) compara_[3]=-54-((real_y-1)*9)+(b*9); compara_[4]=-27-((real_y-1)*9)+(d*9); end
						
										// sin tachar linea target
										if (tablero_borrar[optimiza_4]<5 && tablero_borrar[optimiza_5]<5)
											if (tablero_borrar[optimiza_4+compara_[3]]<5 && tablero_borrar[optimiza_5+compara_[3]]<5)
												if (tablero_borrar[optimiza_4+compara_[4]]<5 && tablero_borrar[optimiza_5+compara_[4]]<5)
												
													// comparacion doble A
													if (tablero_copia[optimiza_4]==tablero_copia[optimiza_5+compara_[4]])
														if (tablero_copia[optimiza_5]==tablero_copia[optimiza_4+compara_[3]])
															if (tablero_copia[optimiza_5+compara_[3]]==tablero_copia[optimiza_4+compara_[4]])
																tablero_borrar[random1]+=6; // conflictivo
																break;																
															end	
														end	
													end	
													
												end
											end	
										end	

										// sin tachar linea target
										if (tablero_borrar[optimiza_4]<5 && tablero_borrar[optimiza_6]<5)
											if (tablero_borrar[optimiza_4+compara_[3]]<5 && tablero_borrar[optimiza_6+compara_[3]]<5)
												if (tablero_borrar[optimiza_4+compara_[4]]<5 && tablero_borrar[optimiza_6+compara_[4]]<5)
												
													// comparacion doble B1
													if (tablero_copia[optimiza_4]==tablero_copia[optimiza_6+compara_[4]])
														if (tablero_copia[optimiza_6]==tablero_copia[optimiza_4+compara_[3]])
															if (tablero_copia[optimiza_6+compara_[3]]==tablero_copia[optimiza_4+compara_[4]])
																tablero_borrar[random1]+=6; // conflictivo
																break;																
															end	
														end	
													end	
													
												end
											end	
											
											
											if (tablero_borrar[optimiza_5+compara_[3]]<5 && tablero_borrar[optimiza_6+compara_[3]]<5)
												if (tablero_borrar[optimiza_4+compara_[4]]<5 && tablero_borrar[optimiza_5+compara_[4]]<5)
												
													// comparacion doble B2
													if (tablero_copia[optimiza_4]==tablero_copia[optimiza_6+compara_[3]])
														if (tablero_copia[optimiza_6]==tablero_copia[optimiza_5+compara_[3]])
															if (tablero_copia[optimiza_6]==tablero_copia[optimiza_4+compara_[4]] && tablero_copia[optimiza_4]==tablero_copia[optimiza_5+compara_[4]])
																tablero_borrar[random1]+=6; // conflictivo
																break;																
															end	
														end	
													end	
													
												end
											end		

											if (tablero_borrar[optimiza_4+compara_[3]]<5 && tablero_borrar[optimiza_5+compara_[3]]<5)
												if (tablero_borrar[optimiza_5+compara_[4]]<5 && tablero_borrar[optimiza_6+compara_[4]]<5)
												
													// comparacion doble B3
													if (tablero_copia[optimiza_4]==tablero_copia[optimiza_5+compara_[3]])
														if (tablero_copia[optimiza_6]==tablero_copia[optimiza_4+compara_[3]])
															if (tablero_copia[optimiza_6]==tablero_copia[optimiza_5+compara_[4]] && tablero_copia[optimiza_4]==tablero_copia[optimiza_6+compara_[4]])
																tablero_borrar[random1]+=6; // conflictivo
																break;																
															end	
														end	
													end	
													
												end
											end												
											
											
										end
										
										// sin tachar linea target
										if (tablero_borrar[optimiza_5]<5 && tablero_borrar[optimiza_6]<5)
											if (tablero_borrar[optimiza_5+compara_[3]]<5 && tablero_borrar[optimiza_6+compara_[3]]<5)
												if (tablero_borrar[optimiza_5+compara_[4]]<5 && tablero_borrar[optimiza_6+compara_[4]]<5)
												
													// comparacion doble C
													if (tablero_copia[optimiza_5]==tablero_copia[optimiza_6+compara_[4]])
														if (tablero_copia[optimiza_6]==tablero_copia[optimiza_5+compara_[3]])
															if (tablero_copia[optimiza_6+compara_[3]]==tablero_copia[optimiza_5+compara_[4]])
																tablero_borrar[random1]+=6; // conflictivo
																break;																
															end	
														end	
													end	
													
												end
											end	
										end	

										// sin tachar linea target
										if (tablero_borrar[optimiza_4]<5 && tablero_borrar[optimiza_5]<5)
											if (tablero_borrar[optimiza_5+compara_[3]]<5 && tablero_borrar[optimiza_6+compara_[3]]<5)
												if (tablero_borrar[optimiza_6+compara_[4]]<5)
												
													// comparacion doble D
													if (tablero_copia[optimiza_4]==tablero_copia[optimiza_5+compara_[3]])
														if (tablero_copia[optimiza_5]==tablero_copia[optimiza_6+compara_[3]])
															if (tablero_copia[optimiza_4]==tablero_copia[optimiza_6+compara_[4]])
																tablero_borrar[random1]+=6; // conflictivo
																break;																
															end	
														end	
													end	
													
												end
											end	
										end
										

										// sin tachar linea target
										if (tablero_borrar[optimiza_5]<5 && tablero_borrar[optimiza_6]<5)
											if (tablero_borrar[optimiza_4+compara_[3]]<5 && tablero_borrar[optimiza_5+compara_[3]]<5)
												if (tablero_borrar[optimiza_4+compara_[4]]<5)
												
													// comparacion doble E
													if (tablero_copia[optimiza_5]==tablero_copia[optimiza_4+compara_[3]])
														if (tablero_copia[optimiza_6]==tablero_copia[optimiza_5+compara_[3]])
															if (tablero_copia[optimiza_6]==tablero_copia[optimiza_4+compara_[4]])
																tablero_borrar[random1]+=6; // conflictivo
																break;																
															end	
														end	
													end	
													
												end
											end	
										end
										
										// sin tachar linea target
										if (tablero_borrar[optimiza_4]<5 && tablero_borrar[optimiza_6]<5 && compara_x==1)
											if (tablero_borrar[optimiza_4+compara_[3]]<5 && tablero_borrar[optimiza_6+compara_[4]]<5)
												
												// comparacion doble F
												if (tablero_copia[optimiza_4]==tablero_copia[optimiza_6+compara_[4]])
													if (tablero_copia[optimiza_6]==tablero_copia[optimiza_4+compara_[3]])
														tablero_borrar[random1]+=6; // conflictivo
														break;																	
													end	
												end	
													
											end	
										end										
										
										
									end
								end	
							end
						end
					end
				end				
				
				
			end
		end
		
		random1=0;
		
		// BORRAR CASILLAS
		for(i=rondas;i>0;i--)
			random=rand(-1,0);
			
			if (random==0)
				
				if (dificultad<3)
					random1=rand(1,81);
					if (tablero_borrar[random1]<5) i++; end // si elige un numero bloqueado, le concede otro intento
				else
					random1++; if (random1>81) random1=81; i=0; end
				end
				
				// si el numero puede borrarse y no ha sido borrado
				if (tablero[random1]>0 && tablero_borrar[random1]<5)
					tablero[random1]=0;
					tablero_color[random1]=1;
					borrados++;			
				end	
			end
		end	
		
		/*
		for (i=1;i<82;i++)
			say(i+" : "+tablero_borrar[i]);
		end
		*/
		
		// say("borrados: "+borrados);
	end		

	pinta_tablero();

	write_var(0,70,211,4, hora);
	write(0,82,211,4,":");
	write_var(0,94,211,4, min);
	write(0,106,211,4,":");
	write_var(0,118,211,4, seg);
	write_var(0,118,228,4, sudoku_finalizado);

	if (tipo==2) tablero_completado=0; sudoku_aleatoriox=1; end

	signal(id,s_kill);
	frame; 
end

// ESTO GENERA *SOLO EL TABLERO ALEATORIO con todos los numeros y asigna a la tabla que guardara el resultado
PROCESS coloca()
PRIVATE
	int contador;
	int contadorx=9; // es el numero que se va a poner en la casilla , siempre los pone de 9 hasta 1
	int cuadrantez[10][10];
	int casilla;
	int recuadro=1;
	int lineaY1[10][10];
	int lineaY;
	int lineaX1[10][10];
	int lineaX;

	int i;
	int b;
	int c;
	int calculo;
	int cuadricula_x;
	int suma_total;
	
	int registra_X[10];
	int registra_Y[10];
	int fallo_casilla;
	
BEGIN
	signal(type sudoku_aleatorio,s_freeze);

	write_delete(all_text);
	write(0,100,100,4,"PROCESSING");
	write(0,100,110,4,"WAIT ...");
	
	while(recuadro!=10)
		contadorx=9;
		recuadro=1;
		
		// limpia numeros
		for(b=1;b<10;b++)
			registra_X[b]=0;
			registra_Y[b]=0;
				
			for(c=1;c<10;c++)	
				lineaX1[b][c]=0;
				lineaY1[b][c]=0;
				cuadrantez[b][c]=0;
			end	
		end
		
		// si no pudo generar uno intenta con otro
		for(i=0;i<1001;i++)
			suma_total++;
			casilla=rand(1,9); // casilla equivale a una posicion aleatoria dentro de un recuadro de 9		
			fallo_casilla++;
			
			// verificamos en el recuadro actual que ningun numero se repita
			if (cuadrantez[recuadro][casilla]==0) 
				
				// calculo para sacar lineas X de desplazamiento
				lineaX=1;
				if (casilla==2 || casilla==5 || casilla==8) lineaX=2; end
				if (casilla==3 || casilla==6 || casilla==9) lineaX=3; end

				if (recuadro==2 || recuadro==5 || recuadro==8) lineaX+=3; end
				if (recuadro==3 || recuadro==6 || recuadro==9) lineaX+=6; end
				
				// calculo para sacar lineas Y de desplazamiento
				lineaY=(1+((casilla-1)/3))+(3*((recuadro-1)/3)); 				
				
				if (lineaY1[lineaY][contadorx]==0) // verifica que en lineaY no hay repetido contadorx
					if (lineaX1[lineaX][contadorx]==0) // verifica que en lineaX no hay repetido contadorx 
					
						cuadrantez[recuadro][casilla]=contadorx; // rellena casilla con numero
						lineaY1[lineaY][contadorx]=contadorx; // rellena lineaY
						lineaX1[lineaX][contadorx]=contadorx; // rellena lineaX
						registra_X[contadorx]=lineaX; // guarda linea X del recuadro actual
						registra_Y[contadorx]=lineaY; // guarda linea Y del recuadro actual
						contadorx--; // resta numero a poner dentro del recuadro
						fallo_casilla=0; // si pone numero sigue buscando
						
						if (contadorx<1) // cuando haya puesto los 9 numeros de un recuadro pasa al siguiente
							contadorx=9; recuadro++; 
						
							// limpia copia temporal
							for(b=1;b<10;b++)	
								registra_X[b]=0;
								registra_Y[b]=0;
							end		
						end
					end	
				end
			end
			
			// si falla demasiado al rellenar una cuadricula, la borra y vuelve a intentar de nuevo (back tracking)
			if (fallo_casilla>32)
				
				for(b=1;b<10;b++)
					cuadrantez[recuadro][b]=0;
					lineaX1[registra_X[b]][b]=0;
					lineaY1[registra_Y[b]][b]=0;
					registra_X[b]=0;
					registra_Y[b]=0;
				end		
				
				contadorx=9;
				fallo_casilla=0;
			end	
						
			if (recuadro==10) break; end
		end		
	
		if (recuadro==10)		
			
			// todos los cuadros que se representan en pantalla y a que posicion apuntan
			for (c=1;c<10;c++)
				cuadricula_x=0;
				if (c==2 || c==5 || c==8) cuadricula_x=3; end
				if (c==3 || c==6 || c==9) cuadricula_x=6; end				
				calculo=cuadricula_x+(((c-1)/3)*27);
					
				for (b=1;b<4;b++)		
					tablero[b+calculo]=cuadrantez[c][b];
					tablero[b+9+calculo]=cuadrantez[c][b+3];
					tablero[b+18+calculo]=cuadrantez[c][b+6]; 				
				end
			end
				
			// pone copia a 0 porque se va a generar un nuevo tablero
			for(contador=1;contador<82;contador++)
				tablero_copia[contador]=0;
				tablero_color[contador]=0;
			end	
		
			write_delete(all_text);
			tablero_completado=0;
			signal(type sudoku_aleatorio,s_wakeup);
			// say(suma_total);
			frame;
			signal(id,s_kill); 
		end 
	end
end

// PINTA RESULTADO
PROCESS pinta_tablero()
PRIVATE 
	int printer; 
	int x1; 
	int y1=1;

BEGIN
	
	LOOP
		contador1=0;
		completo=0;

		if (sudoku_aleatoriox!=0)
			printer++;	
			x1++;
			if (x1>9) x1=1; y1++; end

			if (tablero[printer]!=0 && tablero_color[printer]==0)
				write_var(0,20*x1,20*y1,4, tablero[printer]);
				if (sudoku_aleatoriox!=0)
					cuadrado((20*x1)-1,20*y1,2,printer);
				else
					if (tablero_color[printer]==0) cuadrado((20*x1)-1,20*y1,13,printer); end
				end
			end

			if (tablero_color[printer]==1)
				if (tablero[printer]==0 || sudoku_aleatoriox!=0) cuadrado((20*x1)-1,20*y1,4,printer); end
				if (tablero[printer]==tablero_copia[printer] && sudoku_aleatoriox==0) cuadrado((20*x1)-1,20*y1,14,printer); end
			end

			if (printer>80) procesando=0; timer[0]=0; signal(id,s_kill); end

		else

			for (printer=1;printer<82;printer++)
				x1++;
				if (x1>9) x1=1; y1++; end

				if (tablero[printer]!=0 && tablero_color[printer]==0)
					write_var(0,20*x1,20*y1,4, tablero[printer]);
					if (tablero_color[printer]==0) cuadrado((20*x1)-1,20*y1,13,printer); end
				end

				if (tablero_color[printer]==1)
					if (tablero[printer]==tablero_copia[printer]) cuadrado((20*x1)-1,20*y1,14,printer); end
				end

				if (printer>80) procesando=0; timer[0]=0; signal(id,s_kill); end

			end
		end

		frame; 
	end 
end

// RECURSOS GRAFICOS , ENTORNO
PROCESS cuadrado(double x, double y, int graph,numero)
PRIVATE 
	int alterna=1; 
	int estado; 
	int pinta; 
	int idtxt; 
	int yo; 
	int toca;
	
BEGIN 
	file=graficos; z=1; priority=300;
	if (tablero[numero]!=0 && pinta==0) idtxt=write_var(0,x+1,y,4, tablero[numero]); pinta=1; end
	if (tablero[numero]!=0 && graph==4) graph=14; end
	
	LOOP
	
		if (collision(type pixel)) 
			posicion_cuadro=numero; toca=1; 
		else 
			toca=0; 
		end
		
		if (tablero_completado!=0)
			if (graph==3) graph=14; alpha=255; end
		end

		if (graph!=2 && graph!=13 && tablero_completado==0 && procesando==0)
			
			if (toca==1) 
				graph=3; flags=0;
			
				if (alterna==0) alpha+=5; end
				if (alterna==1) alpha-=5; end
				if (alpha<100) alterna=0; end
				if (alpha>250) alterna=1; end

				// CONTROLES DE TECLAS
				if (tecla_pulsada==0 && mini_cuadro==0)
					
					if (mouse.left) 
						espera++; 
						if (espera>1) semi_cuadro(x+30,y+30,numero); mini_cuadro=1; yo=numero; end 	
					else 
						if (espera>0) espera--; end 
					end

					if (key(_1)) tablero[numero]=1; completo=0; completo1=0; contador1=0; end
					if (key(_2)) tablero[numero]=2; completo=0; completo1=0; contador1=0; end
					if (key(_3)) tablero[numero]=3; completo=0; completo1=0; contador1=0; end
					if (key(_4)) tablero[numero]=4; completo=0; completo1=0; contador1=0; end
					if (key(_5)) tablero[numero]=5; completo=0; completo1=0; contador1=0; end
					if (key(_6)) tablero[numero]=6; completo=0; completo1=0; contador1=0; end
					if (key(_7)) tablero[numero]=7; completo=0; completo1=0; contador1=0; end
					if (key(_8)) tablero[numero]=8; completo=0; completo1=0; contador1=0; end
					if (key(_9)) tablero[numero]=9; completo=0; completo1=0; contador1=0; end

					if (get_joy_button(0,JOY_BUTTON_X)) 
						if (tablero[numero]>1) 
							tablero[numero]--; completo=0; completo1=0; contador1=0; 
						else 
							tablero[numero]=9; completo=0; completo1=0; contador1=0; 
						end 
					end
						
					if (get_joy_button(0,JOY_BUTTON_B)) 
						if (tablero[numero]<9) 
							tablero[numero]++; completo=0; completo1=0; contador1=0; 
						else 
							tablero[numero]=1; completo=0; completo1=0; contador1=0; 
						end 
					end
						
					if ((get_joy_button(0,JOY_BUTTON_Y) || key(_del)) && pinta==1) 
						write_delete(idtxt); tablero[numero]=0; completo=0; completo1=0; contador1=0; pinta=0; 
					end
				end
					
				if (key(_1) || key(_2) || key(_3) || key(_4) || 
				    key(_5) || key(_6) || key(_7) || key(_8) || key(_9) 
				    OR key(_del) || get_joy_button(0,JOY_BUTTON_X) || get_joy_button(0,JOY_BUTTON_B) || get_joy_button(0,JOY_BUTTON_Y)) 
					tecla_pulsada=1; 
				else 
					tecla_pulsada=0; 
				end

				if (tablero[numero]!=0 && pinta==0) idtxt=write_var(0,x+1,y,4, tablero[numero]); pinta=1; end

			else
				alpha=255; alterna=1; graph=4; flags=128; 
				if (tablero[numero]!=0) graph=14; end  
				if (mini_cuadro!=0 && yo==numero) 
					graph=3; 
				else 
					yo=-1; 
				end
				
				if (hay_repetidos==1 && tablero_dificultad<3)
					if (tablero[numero]!=tablero_copia[numero]) graph=15; end
				end
			end
		end
			
		frame; 
	end 
end

PROCESS boton(double x,y, int graph)
BEGIN 
	file=graficos;  
	z=graph;

	LOOP

		if (mini_cuadro==0) 
			if (esconder_cuadro==0) 
				if (x>255) x-=10; end 
				if (x<255) x=255; end 
			else 
				if (x<350) x+=10; end
				if (x>350) x=350; end 
			end
		end

		if (graph==8 || graph==108)
			if (y>30) 
				y-=5;
			else
				y=30;
				if (procesando==0) alpha=255; end
				if ((collision(type pixel) || get_joy_button(0,JOY_BUTTON_START)) && procesando==0) 
					graph=108;
					if (mouse.left || get_joy_button(0,JOY_BUTTON_START)) 
						procesando=1; tablero_completado=1; sudoku_aleatoriox=1; sudoku_aleatorio(1); alpha=128; 
						frame; 
					end
				else
					graph=8;
				end
			end
		end

		if (graph==9 || graph==109)
			if (y>90) 
				y-=5;
			else
				y=90;
				if (procesando==0) alpha=255; end
				if ((collision(type pixel) || get_joy_button(0,JOY_BUTTON_LEFTSHOULDER)) && procesando==0 && sudoku_aleatoriox==1 && tablero_completado==0) 
					graph=109;
					if (mouse.left || get_joy_button(0,JOY_BUTTON_LEFTSHOULDER)) 
						procesando=1; tablero_completado=1; sudoku_aleatoriox=0; sudoku_aleatorio(0); alpha=128; 
						frame; 
					end
				else
					graph=9;
				end
			end
		end

		if (graph==10 || graph==110)
			if (y>140) 
				y-=5;
			else
				y=140;
				if (dificultad==1) 
					alpha=128; 
				else 
					alpha=255; 
				end

				if (collision(type pixel) && procesando==0 )
					graph=110;
					if (mouse.left) dificultad=1; end
				else
					graph=10;
				end
			end
		end
		if (graph==11 || graph==111)
			if (y>162) 
				y-=5;
			else
				y=162;
				if (dificultad==2) 
					alpha=128; 
				else 
					alpha=255; 
				end
				
				if (collision(type pixel) && procesando==0 )
					graph=111;
					if (mouse.left) dificultad=2; end
				else
					graph=11;
				end
			end
		end

		if (graph==12 || graph==112)
			if (y>184) 
				y-=5;
			else
				y=184;
				if (dificultad==3) 
					alpha=128; 
				else 
					alpha=255; 
				end
				
				if (collision(type pixel) && procesando==0 )
					graph=112;
					if (mouse.left) dificultad=3; end
				else
					graph=12;
				end
			end
		end

		frame; 
	end 
end

PROCESS raton(dato)
PRIVATE 
	int tecla;
	
BEGIN 
	file=graficos; z=-500; graph=5;
	pixel();
	posicion_cuadro=dato; cambio_cuadro=1;
	
	LOOP

		if (mini_cuadro==-1 && NOT mouse.left) 
			mini_cuadro=0; espera=0; posicion_cuadro=dato; cambio_cuadro=1; 
		end

		if (collision(type cuadrado)) 
			alpha=128; 
		else 
			alpha=255; 
		end
	
		x=mouse.x;
		y=mouse.y;

		if (tecla==0)
			if ((get_joy_button(0,JOY_BUTTON_DPAD_UP)==1 || key(_up)) && posicion_cuadro>9) posicion_cuadro-=9; cambio_cuadro=1; end // arriba
			if ((get_joy_button(0,JOY_BUTTON_DPAD_DOWN)==1 || key(_down)) && posicion_cuadro<73) posicion_cuadro+=9; cambio_cuadro=1; end // abajo
			if ((get_joy_button(0,JOY_BUTTON_DPAD_LEFT)==1 || key(_left)) && posicion_cuadro>1) posicion_cuadro--; cambio_cuadro=1; end // izq
			if ((get_joy_button(0,JOY_BUTTON_DPAD_RIGHT)==1 || key(_right)) && posicion_cuadro<81) posicion_cuadro++; cambio_cuadro=1; end // der

			if (cambio_cuadro==1)
				if (posicion_cuadro>0 && posicion_cuadro<10) mouse.x=20*posicion_cuadro; mouse.y=20; end
				if (posicion_cuadro>9 && posicion_cuadro<19) mouse.x=20*(posicion_cuadro-9); mouse.y=40; end
				if (posicion_cuadro>18 && posicion_cuadro<28) mouse.x=20*(posicion_cuadro-18); mouse.y=60; end
				if (posicion_cuadro>27 && posicion_cuadro<37) mouse.x=20*(posicion_cuadro-27); mouse.y=80; end
				if (posicion_cuadro>36 && posicion_cuadro<46) mouse.x=20*(posicion_cuadro-36); mouse.y=100; end
				if (posicion_cuadro>45 && posicion_cuadro<55) mouse.x=20*(posicion_cuadro-45); mouse.y=120; end
				if (posicion_cuadro>54 && posicion_cuadro<64) mouse.x=20*(posicion_cuadro-54); mouse.y=140; end
				if (posicion_cuadro>63 && posicion_cuadro<73) mouse.x=20*(posicion_cuadro-63); mouse.y=160; end
				if (posicion_cuadro>72 && posicion_cuadro<82) mouse.x=20*(posicion_cuadro-72); mouse.y=180; end
				cambio_cuadro=0;
			end
		end

		if (get_joy_button(0,JOY_BUTTON_DPAD_UP) || get_joy_button(0,JOY_BUTTON_DPAD_DOWN) || get_joy_button(0,JOY_BUTTON_DPAD_LEFT) || get_joy_button(0,JOY_BUTTON_DPAD_RIGHT) 
		OR key(_up) || key(_down) || key(_left) || key(_right)) 
			tecla=1; 
		else 
			tecla=0; 
		end

		frame; 
	end 
end

PROCESS pixel()
BEGIN 
	file=graficos; z=-500; graph=6;
	priority=-100;

	LOOP
		x=father.x;	
		y=father.y;
		alpha=father.alpha;
		if (procesando==0) 
			graph=6; 
		else 
			graph=0; 
		end
		
		frame; 
	end 
end

PROCESS pon_pantalla(file,graph)
BEGIN
	x=160; y=120; z=700; flags=128;
	LOOP 
		frame; 
	end 
end

PROCESS semi_cuadro(double x,y, int numero)
PRIVATE
	int una_vez;
	
BEGIN 
	file=graficos; z=-400; graph=16;
	signal(type pixel,s_kill);
	signal(type raton,s_kill);
	pixel1(); raton1();
	size_y=0;
	
	LOOP
		if (NOT mouse.left) mini_cuadro=2; end
		if (size_y<100) 
			size_y+=10; 
		else 
			size_y=100;
		
			if (una_vez==0)
				baldosa(x-20,y-20,numero,1); baldosa(x,y-20,numero,2); baldosa(x+20,y-20,numero,3);
				baldosa(x-20,y,numero,4); baldosa(x,y,numero,5); baldosa(x+20,y,numero,6);
				baldosa(x-20,y+20,numero,7); baldosa(x,y+20,numero,8); baldosa(x+20,y+20,numero,9);
				baldosa(300,220,0,10);
				una_vez=1;
			end
		end
		
		frame; 
	end 
end

PROCESS baldosa(x,y,numero,posicion)
PRIVATE
	int alterna;
	
BEGIN
	file=graficos; z=-399; graph=4; priority=-200; 
	if (posicion==10) graph=17; end
	
	LOOP

		if (collision(type pixel1) && mini_cuadro==2) 
			if (posicion!=10) graph=3; end
			if (alterna==0) alpha+=5; end
			if (alterna==1) alpha-=5; end
			if (alpha<100) alterna=0; end
			if (alpha>250) alterna=1; end
			
			if (mouse.left) 
				completo=0; completo1=0; contador1=0;
				if (posicion!=10) tablero[numero]=posicion; end 
				mini_cuadro=-1; signal(type pixel1,s_kill); signal(type raton1,s_kill); raton(numero); 
				signal(type semi_cuadro,s_kill); signal(type baldosa,s_kill); 
			end
			
		else
			alpha=255; alterna=1; 
			if (posicion!=10) graph=4; end
		end

		frame; 
	end 
end

PROCESS pixel1()
BEGIN 
	file=graficos; z=-500; graph=6;
	priority=-100;
	
	LOOP
		x=mouse.x;
		y=mouse.y;
		if (collision(type baldosa)) 
			alpha=128; 
		else 
			alpha=255; 
		end
		
		frame; 
	end 
end

PROCESS raton1()
BEGIN 
	file=graficos; z=-500; graph=5;
	priority=-100;
	
	LOOP
		x=mouse.x;
		y=mouse.y;
		if (collision(type baldosa)) 
			alpha=128; 
		else 
			alpha=255; 
		end
		frame; 
	end 
end
