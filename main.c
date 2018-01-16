#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/base.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "GERAR_SUDOKU.h"
//tipos de dados personalizados
typedef struct {
    int i,j;
}area_do_mouse;

typedef struct {
    int i,j,num;
}area_do_teclado;
typedef struct {
    float quadx1,quadx2,quady1,quady2;
    int i,j;
}quadOp;
typedef struct {
    float x1,x2,y1,y2,espessura;
}linhas;
typedef struct {
    int tam,op_dificuldade;
}opJogo;
//constantes
const int LARGURA_TELA = 800;
const int ALTURA_TELA = 600;

//BOOL
bool sair=false;
//inteiros
int matzJogo[9][9];
int matzControle[9][9];
int matzEspelho[9][9];
quadOp matzQuadrado[9][9];
//ponteiros para biblioteca allegro
ALLEGRO_DISPLAY *janela =NULL;
ALLEGRO_BITMAP *background =NULL;
ALLEGRO_BITMAP *titte_box =NULL;
ALLEGRO_FONT *title = NULL;
ALLEGRO_FONT *menu_option_numb = NULL;
ALLEGRO_FONT *menu_option_char = NULL;
ALLEGRO_FONT *number = NULL;
ALLEGRO_FONT *game_title = NULL;
ALLEGRO_FONT *game_menu = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos =NULL;
ALLEGRO_COLOR menu_color[4],menu_game_btn[3],menu_game_font[3];
ALLEGRO_EVENT evento;

bool iniciar();
bool carregararquivos();
void finalizar();
opJogo menu();
int menu_dificuldade_9x9();
int menu_tam();
void backgroundSudoku();
int gerarmatrizes(int tam,int op_dificuldade);
void startJogo(int tam,int op_dificuldade);
void jogo(int op_dificuldade,int dificuldade);
int main(void){
    //inicializando os add_on e ponteiros
    if (!iniciar())return 1;
    if (!carregararquivos())return 1;
    //variaveis
    opJogo selectJogo;
    ///repeticao
    while(!sair){
    //menu
    selectJogo = menu();
    //jogo
    if(!sair)startJogo(selectJogo.tam,selectJogo.op_dificuldade);
    selectJogo.tam=0;
    selectJogo.op_dificuldade=0;
    }
    finalizar();
    return 0;
}
void startJogo(int tam,int op_dificuldade){
    int dificuldade=0;
    //GERA AS MATRIZES -------------------------------------------
     dificuldade = gerarmatrizes(tam,op_dificuldade);
    //TELA DO JOGO -----------------------------------------------
    backgroundSudoku();
    //Inicia o jogo em si ----------------------------------------
    jogo(op_dificuldade,dificuldade);
}
void jogo(int op_dificuldade,int dificuldade ){
    //VARIAVEIS  -----------------------------------------------------------------------------------
    int i=0,j=0,k=0,l=0,num,contConcluir=0,testeConcluir=0;
    bool terminar=false;
    FILE *arquivo;
    quadOp vetmouse[81];
    area_do_mouse indiceQudradoColor,indiceClickMouse,indiceClickMouseTemp;
    ALLEGRO_COLOR quadColor,quadColorCorrigindo;
    area_do_teclado teclado;
    indiceClickMouse.i=-1;
    indiceClickMouse.j=-1;
    //VETOR MAPIAR MOUSE --------------------------------------------------------------------------
    arquivo = fopen("quadrados.bin","r+b");
    fread(vetmouse,sizeof(quadOp),81,arquivo);
    fclose(arquivo);
    //LOOP DO JOGO -------------------------------------------------------------------------------
    while(!terminar)
    {   
        //DEFAULT -------------------------------------------------------------------------------
        quadColor = al_map_rgb(253,236,166);
        //LOOP EVENTOS -------------------------------------------------------------------------------
        while (!al_is_event_queue_empty(fila_eventos))
        {
                    for (k = 0; k < 3; k++)
                    {
                        menu_game_btn[k] =al_map_rgb(219,239,109);
                        menu_game_font[k] =al_map_rgb(112,116,70);
                    }
            // manipulando os eventos
            al_wait_for_event(fila_eventos, &evento);
            if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) //Evento CLOSE TELA-----------------------------------------------------------
                {
                    terminar = true;sair=true;
                }
                else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) // Click do Mouse --------------------------------------------------
                    {
                        //LOOP EFEITO CLICK -------------------------------------------------------------------------------
                        k=0;
                        indiceClickMouseTemp.i=-1;
                        indiceClickMouseTemp.j=-1;
                        while(k!=81)
                        {
                            if (evento.mouse.x >= vetmouse[k].quadx1 && evento.mouse.x <= vetmouse[k].quadx2 &&
                                evento.mouse.y >= vetmouse[k].quady1 && evento.mouse.y <= vetmouse[k].quady2 )
                                {
                                    indiceClickMouse.i=vetmouse[k].i;
                                    indiceClickMouse.j=vetmouse[k].j;
                                }
                            k++; 
                         }
                         if(indiceClickMouseTemp.i !=-1 && indiceClickMouseTemp.j !=-1)
                         {
                            indiceClickMouse = indiceClickMouseTemp;
                        }
                        // BOTÂO CORRIGIR -------------------------------------------------------------------------------
                        if(evento.mouse.x >= 585 && evento.mouse.x <= 784 && evento.mouse.y >= 426 && evento.mouse.y <= 475.0 )
                        {
                        	teclado.num=0;
                            for (k = 0; k < 9; k++)
                            {
                                for ( l = 0; l < 9; l++)
                                {
                                    if(matzJogo[k][l]!=-1)
                                    {
                                        matzJogo[k][l]= matzEspelho[k][l];
                                    }
                                }
                            }
                        }
                        // BOTÂO CONCLUIR -------------------------------------------------------------------------------
                            else if (evento.mouse.x >= 585 && evento.mouse.x <= 784 && evento.mouse.y >= 481 && evento.mouse.y <= 530 )
                            {   
                                contConcluir=0;
                                for (k = 0; k < 9; k++)
                                {
                                    for ( l = 0; l < 9; l++)
                                    {
                                        if(matzJogo[k][l]!=-1)
                                        {
                                            if(matzJogo[k][l]== matzEspelho[k][l])contConcluir++;
                                        }
                                    }
                                }
                                if(contConcluir==dificuldade)
                                {
                                    al_draw_filled_rounded_rectangle(585,140,784.0,395.0,10,10,al_map_rgb(219,239,109));
                                    al_draw_text(game_menu,al_map_rgb(112,116,70),687,250,ALLEGRO_ALIGN_CENTRE,"VENCEU" );
                                    al_flip_display();
                                    al_rest(1.5);
                                    terminar=true;

                                }else
                                    {
                                        al_draw_filled_rounded_rectangle(585,140,784.0,395.0,10,10,al_map_rgb(219,239,109));
                                        al_draw_text(game_menu,al_map_rgb(112,116,70),687,250,ALLEGRO_ALIGN_CENTRE,"ERRADO" );
                                        al_flip_display();
                                        al_rest(1.5);
                                    }
                            }
                        // BOTÂO SAIR -----------------------------------------------------------------------------------
                                else if(evento.mouse.x >= 585 && evento.mouse.x <= 784 && evento.mouse.y >= 536 && evento.mouse.y <= 585 )terminar=true;

                    }
                    else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)// Evento movimento do mouse -----------------------------------------
                        {   
                            //LOOP EFEITO HOVER -------------------------------------------------------------------------------
                            k=0;
                            indiceQudradoColor.i=-1;
                            indiceQudradoColor.j=-1;
                            while(k!=81)
                            {
                                if (evento.mouse.x >= vetmouse[k].quadx1 && evento.mouse.x <= vetmouse[k].quadx2 &&
                                    evento.mouse.y >= vetmouse[k].quady1 && evento.mouse.y <= vetmouse[k].quady2 )
                                    {
                                        indiceQudradoColor.i=vetmouse[k].i;
                                        indiceQudradoColor.j=vetmouse[k].j;
                                    }
                                k++; 
                            }
                            // BOTÂO CORRIGIR -------------------------------------------------------------------------------
                            if(evento.mouse.x >= 585 && evento.mouse.x <= 784 && evento.mouse.y >= 426 && evento.mouse.y <= 475.0 )
                            {
                                menu_game_btn[0] =al_map_rgb(112,116,70);
                                menu_game_font[0] =al_map_rgb(219,239,109);
                            }
                            // BOTÂO CONCLUIR -------------------------------------------------------------------------------------------------
                                else if (evento.mouse.x >= 585 && evento.mouse.x <= 784 && evento.mouse.y >= 481 && evento.mouse.y <= 530 )
                                {
                                    menu_game_btn[1] =al_map_rgb(112,116,70);
                                    menu_game_font[1] =al_map_rgb(219,239,109);
                                }
                            // BOTÂO SAIR ------------------------------------------------------------------------------------------------------
                                    else if(evento.mouse.x >= 585 && evento.mouse.x <= 784 && evento.mouse.y >= 536 && evento.mouse.y <= 585 )
                                    {
                                        menu_game_btn[2] =al_map_rgb(112,116,70);
                                        menu_game_font[2] =al_map_rgb(219,239,109);
                                    }
                        }
                        else if (evento.type == ALLEGRO_EVENT_KEY_CHAR)// Entrada de numeros ------------------------------------------------
                        {
                            char temp[] = {evento.keyboard.unichar, '\0'};
                            if (evento.keyboard.unichar >= '1' &&
                                evento.keyboard.unichar <= '9'){
                                    num = atoi(temp); 
                            }
                             teclado.i = indiceClickMouse.i;
                             teclado.j = indiceClickMouse.j;
                             if ( num >= 1 && num <= 9 ) teclado.num=num;
                                else 
                                {
                                    teclado.i=-1;
                                    teclado.j=-1;
                                    teclado.num =0;
                                }
                        }
        }
        //VERIFICANDO EVENTOS ----------------------------------------------------------------------------------
            //EFEITO HOVER -------------------------------------------------------------------------------
            if (indiceQudradoColor.i == i && indiceQudradoColor.j == j && matzControle[i][j] == 0) quadColor =al_map_rgb(199,221,42);
            //CLICK NOS QUADRADOS-------------------------------------------------------------------------------
            if (indiceClickMouse.i == i && indiceClickMouse.j == j ) quadColor =al_map_rgb(199,221,42);
        //APLICANDO EVENTOS ----------------------------------------------------------------------------------------
            //MENU LATERAL -------------------------------------------------------------------------------------
                //resposta --------------------------------------------------------------------------
                al_draw_filled_rounded_rectangle(585,426,784.0,475.0,10,10,menu_game_btn[0]);
                al_draw_text(game_menu,menu_game_font[0],687,420,ALLEGRO_ALIGN_CENTRE,"Corrigir" );
                //finalizar --------------------------------------------------------------------------
                al_draw_filled_rounded_rectangle(585,481,784.0,530.0,10,10,menu_game_btn[1]);
                al_draw_text(game_menu,menu_game_font[1],687,479,ALLEGRO_ALIGN_CENTRE,"Concluir" );
                //voltar --------------------------------------------------------------------------
                al_draw_filled_rounded_rectangle(585,536,784.0,585.0,10,10,menu_game_btn[2]);
                al_draw_text(game_menu,menu_game_font[2],687,533,ALLEGRO_ALIGN_CENTRE,"Sair" );
            //TESTE DE CONCLUIR -------------------------------------------------------------------------------------
                    al_draw_filled_rounded_rectangle(585,140,784.0,395.0,10,10,al_map_rgb(253,236,166));

            //DESENHANDO QUADRADOS E ESCREVENDO NUMEROS -----------------------------------------------------------------------------------
            if(op_dificuldade==1)
            {
                if (teclado.i==i && teclado.j==j && teclado.num !=0)
                    {   
                        if (matzJogo[i][j]!=-1)matzJogo[i][j] = teclado.num;// inserindo o valor na matriz
                    }
                if ( matzEspelho[i][j]==matzJogo[i][j] ) quadColorCorrigindo = al_map_rgb(51,240,146);
                    else if ( matzEspelho[i][j] != matzJogo[i][j] && matzJogo[i][j]!=0 && matzJogo[i][j]!=-1 ) quadColorCorrigindo = al_map_rgb(255,83,83);
                        else quadColorCorrigindo = quadColor;

                    if(matzControle[i][j]==0) al_draw_filled_rounded_rectangle(matzQuadrado[i][j].quadx1,matzQuadrado[i][j].quady1,matzQuadrado[i][j].quadx2,matzQuadrado[i][j].quady2,10,10,quadColorCorrigindo);
                    if(matzJogo[i][j]!=0 && matzJogo[i][j]!=-1)al_draw_textf(number,al_map_rgb(62,62,62),matzQuadrado[i][j].quadx1+30,matzQuadrado[i][j].quady1,ALLEGRO_ALIGN_CENTRE,"%d",matzJogo[i][j]);
            }else
                {
                    // DESENHANDO O QUADRADO -------------------------------------------------------------- 
                    if(matzControle[i][j]==0) al_draw_filled_rounded_rectangle(matzQuadrado[i][j].quadx1,matzQuadrado[i][j].quady1,matzQuadrado[i][j].quadx2,matzQuadrado[i][j].quady2,10,10,quadColor);
                    if (teclado.i==i && teclado.j==j && teclado.num !=0)
                    {   
                        al_draw_filled_rounded_rectangle(matzQuadrado[i][j].quadx1,matzQuadrado[i][j].quady1,matzQuadrado[i][j].quadx2,matzQuadrado[i][j].quady2,10,10,quadColor);

                        if (matzJogo[i][j]!=-1 && teclado.num!=0)matzJogo[i][j] = teclado.num;// inserindo o valor na matriz
                        	
                        al_draw_textf(number,al_map_rgb(62,62,62),matzQuadrado[i][j].quadx1+30,matzQuadrado[i][j].quady1,ALLEGRO_ALIGN_CENTRE,"%d",matzJogo[i][j]);
                    }
                    if(matzJogo[i][j]!=0 && matzJogo[i][j]!=-1)al_draw_textf(number,al_map_rgb(62,62,62),matzQuadrado[i][j].quadx1+30,matzQuadrado[i][j].quady1,ALLEGRO_ALIGN_CENTRE,"%d",matzJogo[i][j]);
                }
            //OS NUMEROS CORRETOS ---------------------------------------   ----------------------------------------    

            if( matzControle[i][j]!=0)
            {
                al_draw_filled_rounded_rectangle(matzQuadrado[i][j].quadx1,matzQuadrado[i][j].quady1,matzQuadrado[i][j].quadx2,matzQuadrado[i][j].quady2,10,10,al_map_rgb(219,239,109));
                al_draw_textf(number,al_map_rgb(62,62,62),matzQuadrado[i][j].quadx1+30,matzQuadrado[i][j].quady1,ALLEGRO_ALIGN_CENTRE,"%d",matzControle[i][j]);
            }
        //MANIPULANDO INDICES SEM FOR ---------------------------------------   ----------------------------------------
        j++;
        if(j==9){ j=0; i++; }
        if(i==9){ i=0; j=0; }
        //ATUALIZANDO DISPLAY -------------------------------------------------------------------------------
        al_flip_display();
    }
}
void backgroundSudoku(){
    //variaveis --------------------------------------------------------------------------
    linhas linha[4];
    ALLEGRO_COLOR quadColor;
    int i,j;
    FILE *arquivo;
    //background --------------------------------------------------------------------------
    al_draw_bitmap(background,0,0,0);
    al_draw_rounded_rectangle(5.0,16.0,562.97,593.0,10,10,al_map_rgb(255,255,255),3.5 );
    //traco --------------------------------------------------------------------------
    al_draw_filled_rounded_rectangle(580.0,120,790.0,593.0,10,10,al_map_rgb(253,236,166));
    //preenchimento --------------------------------------------------------------------------
    al_draw_rounded_rectangle(578,120,790,593,10,10,al_map_rgb(215,182,135),3.5 );
    // titulo --------------------------------------------------------------------------
    al_draw_filled_rounded_rectangle(578,20,790.0,100.0,10,10,al_map_rgb(219,239,109));
    al_draw_text(game_title,al_map_rgb(112,116,70),687,21,ALLEGRO_ALIGN_CENTRE,"SuDOkU" );
    //resposta --------------------------------------------------------------------------
    al_draw_filled_rounded_rectangle(585,426,784.0,475.0,10,10,al_map_rgb(219,239,109));
    al_draw_text(game_menu,al_map_rgb(112,116,70),687,420,ALLEGRO_ALIGN_CENTRE,"Corrigir" );
    //finalizar --------------------------------------------------------------------------
    al_draw_filled_rounded_rectangle(585,481,784.0,530.0,10,10,al_map_rgb(219,239,109));
    al_draw_text(game_menu,al_map_rgb(112,116,70),687,479,ALLEGRO_ALIGN_CENTRE,"Concluir" );
    //voltar --------------------------------------------------------------------------
    al_draw_filled_rounded_rectangle(585,536,784.0,585.0,10,10,al_map_rgb(219,239,109));
    al_draw_text(game_menu,al_map_rgb(112,116,70),687,533,ALLEGRO_ALIGN_CENTRE,"Sair" );
    // linhas --------------------------------------------------------------------------
    arquivo = fopen("linhas.bin","r+b");
    fread(linha,sizeof(linhas),4,arquivo);
    fclose(arquivo);
    for (i = 0; i < 4; i++)
    {
        al_draw_line(linha[i].x1,linha[i].y1,linha[i].x2,linha[i].y2,al_map_rgb(255,255,255),linha[i].espessura);
    }
     //quadrados --------------------------------------------------------------------------
     for (i = 0; i < 9; i++){
        for ( j = 0; j < 9; j++){
            if(matzControle[i][j]!=0)quadColor= al_map_rgb(219,239,109);
                else quadColor = al_map_rgb(253,236,166);
            al_draw_filled_rounded_rectangle(matzQuadrado[i][j].quadx1,matzQuadrado[i][j].quady1,matzQuadrado[i][j].quadx2,matzQuadrado[i][j].quady2,10,10,quadColor);
            if( matzControle[i][j]!=0)
            {
                al_draw_textf(number,al_map_rgb(62,62,62),matzQuadrado[i][j].quadx1+30,matzQuadrado[i][j].quady1,ALLEGRO_ALIGN_CENTRE,"%d",matzControle[i][j]);
            }
        }
    }
}
int gerarmatrizes(int tam,int op_dificuldade){
    int i,j;
    int **matzPont=NULL;
    int dificuldade=0;
    int cont=0,linha=0,coluna=0,temp=0;
    quadOp quadrados[81];
    quadOp *ponteiro;
    FILE *arquivo;
    dificuldade = gerarDificuldade(op_dificuldade,tam);
    while(matzPont==NULL)matzPont = gerarsudoku(tam);
    //CARREGANDO A MATRIZ GERADA PARA VETORES --------------------------------------------------------------------------
    for (i = 0; i < 9; i++){
        for ( j = 0; j < 9; j++){
            matzEspelho[i][j]=matzPont[i][j];
            matzControle[i][j]=matzPont[i][j];
            matzJogo[i][j] = -1;
        }
    }
    //LIMPANDO MATRIZ JOGO --------------------------------------------------------------------------
    while( cont != dificuldade){    
            linha = indice_aleatorio(9);
            coluna = indice_aleatorio(9);
            temp = sortear();
             if(temp==0 && cont < dificuldade && matzControle[linha][coluna]!=0 ){
                matzControle[linha][coluna]=0;
                matzJogo[linha][coluna]=0;
                cont++;
             }
    }
    matzPont=liberarMat(tam,matzPont);
    // MATRIZ ASSOCIADA A QUADRADOS  --------------------------------------------------------------------------
    arquivo = fopen("quadrados.bin","r+b");
    fread(quadrados,sizeof(quadOp),81,arquivo);
    fclose(arquivo);
    ponteiro = &matzQuadrado[0][0];
    for ( i = 0; i < 81; i++)
    {
        *ponteiro = quadrados[i];
         ponteiro++;
    }
    return dificuldade;
}
bool iniciar(){
     // INICIANDO BIBLIOTECA ALLEGRO
     if(!al_init()){
     fprintf(stderr," FALHA AO INICIAR ALEGRO.\n");
     return false;
     }
     // INICIALIZANDO ADD_ON IMAGENS
     if(!al_init_image_addon()){
     fprintf(stderr," FALHA AO INICIAR ADD_ON IMAGENS");
     return false;
     }
     // INICIALIZANDO ADD_ON FONTES TTF
     al_init_font_addon();
     if(!al_init_ttf_addon()){
     fprintf(stderr," FALHA AO INICIAR ADD_ON FONTES TTF");
     return false;
     }
     // INICIALIZANDO ADD_ON PRIMITIVES
     if(!al_init_primitives_addon()){
     fprintf(stderr," FALHA AO INICIAR ADD_ON PRIMITIVES");
     return false;
     }
     //CRIANDO JANELA
     janela = al_create_display(LARGURA_TELA,ALTURA_TELA);
     if (!janela){
     fprintf(stderr," FALHA AO INICIAR JANELA.\n");
     return false;
     }
     //TITULO DA JANELA
     al_set_window_title(janela,"Sudoku");
     
     //INICIALIZANDO O USO DO MOUSE
     if (!al_install_mouse()){
     fprintf(stderr,"FALHA AO INICIAR MOUSE");
     return false;
     }
     //INICIALIZANDO O CUSOR PADRAO
     if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)){
     fprintf(stderr,"FALHA AO INICIAR PONTEIRO DO MOUSE");
     return false;
     }
     // INICIALIZANDO TECLADO
     if(!al_install_keyboard()){
      fprintf(stderr, "FALHA AO INICIAR TECLADO!\n");
      return false;  
     }
     // INICIALIZANDO FILA DE EVENTOS
     fila_eventos = al_create_event_queue();
     if (!fila_eventos){
     fprintf(stderr,"FALHA AO INICIAR FILA DE EVENTOS");
     return false;
     }

     al_register_event_source(fila_eventos,al_get_keyboard_event_source());
     al_register_event_source(fila_eventos, al_get_mouse_event_source());
     al_register_event_source(fila_eventos, al_get_display_event_source(janela));

     return true;}
bool carregararquivos(){
     // CARREGANDO BACKGROUND
     background = al_load_bitmap("background.jpg");  
     if(!background){
     fprintf(stderr," FALHA AO INICIAR | BACKGROND.jpg | \n");
     al_destroy_display(janela);
     return false;
     }
     // CARREGANDO FONTE TITULO
     title = al_load_font("title.ttf",120,0);
     if(!title){
     fprintf(stderr,"FALHA AO INICIAR | MENU_OPTION.TTF |");
     return false;
     }
     // CARREGANDO FONTE MENU PARA NUMEROS
     menu_option_numb= al_load_font("menu_option_numb.ttf",65,0);
     if(!menu_option_numb){
     fprintf(stderr,"FALHA AO INICIAR | TITLE.TTF |");
     return false;
     }
     // CARREGANDO FONTE MENU
     menu_option_char= al_load_font("menu_option_char.ttf",65,0);
     if(!menu_option_char){
     fprintf(stderr,"FALHA AO INICIAR | TITLE.TTF |");
     return false;
     }
     number = al_load_font("number.ttf",55,0);
     if(!number ){
     fprintf(stderr,"FALHA AO INICIAR | NUMBER.TTF |");
     return false;
     }
     game_title = al_load_font("title.ttf",47,0);
     if(!game_title ){
     fprintf(stderr,"FALHA AO INICIAR | NUMBER_TITLE.TTF |");
     return false;}
     game_menu = al_load_font("title.ttf",37,0);
     if(!game_title ){
     fprintf(stderr,"FALHA AO INICIAR | NUMBER_TITLE.TTF |");
     return false;}
     return true;}
void finalizar(){
     al_destroy_bitmap(background);
     if (title!= NULL);al_destroy_font(title);
     if (menu_option_char!= NULL)al_destroy_font(menu_option_char);
     if (menu_option_numb!= NULL)al_destroy_font(menu_option_numb);
     if (number!= NULL)al_destroy_font(number);
     if (game_title != NULL)al_destroy_font(game_title );
     if (game_menu!= NULL)al_destroy_font(game_menu);
     al_destroy_event_queue(fila_eventos);
     al_destroy_display(janela);
 }
opJogo menu(){
    opJogo temp;
    temp.tam=0;
    temp.op_dificuldade =0;
    while(temp.op_dificuldade==0 &&!sair ){
        switch(temp.tam){
            case 0:
            temp.tam = menu_tam();
                break;
            case 3:
            temp.op_dificuldade =  menu_dificuldade_9x9();
            if (temp.op_dificuldade==0)
            {
                temp.tam=0;
            }
                break;
        }
    }
    return temp;
 }
int menu_tam(){
    int i,tam=0;
    bool terminar= false;
    while(tam==0 && !terminar)
    {
            while (!al_is_event_queue_empty(fila_eventos))
            {

            for(i=0;i<4;i++)menu_color[i]= al_map_rgb(196,255,14);
            al_wait_for_event(fila_eventos, &evento);
            if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){sair=true;terminar=true;} 
            else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
            {
                if (evento.mouse.x >= 270 && evento.mouse.x <= 530 && evento.mouse.y >= 315 && evento.mouse.y <= 394) {menu_color[0] = al_map_rgb(253,236,166);}
                    else if (evento.mouse.x >= 270 && evento.mouse.x <= 530 && evento.mouse.y >= 495 && evento.mouse.y <= 574){menu_color[1] = al_map_rgb(253,236,166);}
            }
            else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){

                if (evento.mouse.x >= 270 && evento.mouse.x <= 530 && evento.mouse.y >= 315 && evento.mouse.y <= 394){tam=3;}
                    else if (evento.mouse.x >= 270 && evento.mouse.x <= 530 && evento.mouse.y >= 495 && evento.mouse.y <= 574){sair=true;terminar=true;}
            }
             al_draw_bitmap(background,0,0,0);
             al_draw_filled_rounded_rectangle(110.0,55.0,690.0,190.0,10,10,al_map_rgb(253,236,166));
             al_draw_text(title,al_map_rgb(62,62,62),LARGURA_TELA/2,35,ALLEGRO_ALIGN_CENTRE,"SuDOkU" );

             al_draw_filled_rounded_rectangle(270.0,315.0,530.0,394.0,10,10,menu_color[0]);
             al_draw_text(menu_option_numb,al_map_rgb(62,62,62),LARGURA_TELA/2,305,ALLEGRO_ALIGN_CENTRE,"09x09" );
             
             
             al_draw_filled_rounded_rectangle(270.0,495.0,530.0,574.0,10,10,menu_color[1]);
             al_draw_text(menu_option_numb,al_map_rgb(62,62,62),LARGURA_TELA/2,479,ALLEGRO_ALIGN_CENTRE,"sair" );

            }
        al_flip_display();
    }
    return tam;
 }     
int menu_dificuldade_9x9(){
     int i,op_dificuldade=0;
     while(op_dificuldade==0)
     {
        while ( !al_is_event_queue_empty(fila_eventos)){   
        for(i=0;i<4;i++)menu_color[i]= al_map_rgb(196,255,14);
             al_wait_for_event(fila_eventos, &evento);
             if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){exit(0);} 

             else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
             {
                 if (evento.mouse.x >= 270 && evento.mouse.x <= 530 &&
                    evento.mouse.y >= 220 && evento.mouse.y <= 299)menu_color[0] = al_map_rgb(253,236,166);
                    else if (evento.mouse.x >= 270 && evento.mouse.x <= 530 &&
                    evento.mouse.y >= 315 && evento.mouse.y <= 394)menu_color[1] = al_map_rgb(253,236,166);
                         else if (evento.mouse.x >= 270 && evento.mouse.x <= 530 &&
                    evento.mouse.y >= 405 && evento.mouse.y <= 484)menu_color[2] = al_map_rgb(253,236,166);
                            else if (evento.mouse.x >= 270 && evento.mouse.x <= 530 &&
                    evento.mouse.y >= 495 && evento.mouse.y <= 574)menu_color[3] = al_map_rgb(253,236,166);
             }

             else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
                if (evento.mouse.x >= 270 && evento.mouse.x <= 530 &&
                    evento.mouse.y >= 220 && evento.mouse.y <= 299)op_dificuldade=1;
                    else if (evento.mouse.x >= 270 && evento.mouse.x <= 530 &&
                    evento.mouse.y >= 315 && evento.mouse.y <= 394)op_dificuldade=2;
                         else if (evento.mouse.x >= 270 && evento.mouse.x <= 530 &&
                    evento.mouse.y >= 405 && evento.mouse.y <= 484)op_dificuldade=3;
                            else if (evento.mouse.x >= 270 && evento.mouse.x <= 530 &&
                    evento.mouse.y >= 495 && evento.mouse.y <= 574)return 0;
            }
            al_draw_bitmap(background,0,0,0);
            al_draw_filled_rounded_rectangle(110.0,55.0,690.0,190.0,10,10,al_map_rgb(253,236,166));
            al_draw_text(title,al_map_rgb(62,62,62),LARGURA_TELA/2,35,ALLEGRO_ALIGN_CENTRE,"SuDOkU" );

             al_draw_filled_rounded_rectangle(270.0,220.0,530.0,299.0,10,10,menu_color[0]);
             al_draw_text(menu_option_char,al_map_rgb(62,62,62),LARGURA_TELA/2,230,ALLEGRO_ALIGN_CENTRE,"Facil" );
             
             al_draw_filled_rounded_rectangle(270.0,315.0,530.0,394.0,10,10,menu_color[1]);
             al_draw_text(menu_option_char,al_map_rgb(62,62,62),LARGURA_TELA/2,325,ALLEGRO_ALIGN_CENTRE,"Medio" );
             
             al_draw_filled_rounded_rectangle(270.0,405.0,530.0,484.0,10,10,menu_color[2]);
             al_draw_text(menu_option_char,al_map_rgb(62,62,62),LARGURA_TELA/2,415,ALLEGRO_ALIGN_CENTRE,"Dificil" );
             
             al_draw_filled_rounded_rectangle(270.0,495.0,530.0,574.0,10,10,menu_color[3]);
             al_draw_text(menu_option_char,al_map_rgb(62,62,62),LARGURA_TELA/2,499,ALLEGRO_ALIGN_CENTRE,"voltar" );
        }
     al_flip_display();
     }
     return op_dificuldade;
 }
