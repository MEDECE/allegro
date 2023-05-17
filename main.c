#include <allegro.h>
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#define LARGO 1000
#define ANCHO 720
typedef struct personnage
{
    BITMAP* bas[3];
    BITMAP* gauche[3];
    BITMAP* droite[3];
    BITMAP* haut[3];
    int x, y;
    int direction;
    int frame;
    int en_deplacement;
}t_personnage;

void separer_bitmap_personnage(t_personnage* personnage, BITMAP* bitmap)
{
    int taille_w = 48;
    int taille_h = 48;

    for (int j = 0; j < 3; j++) {
        personnage->bas[j] = create_sub_bitmap(bitmap, j * taille_w, 0 * taille_h, taille_w, taille_h);
        personnage->gauche[j] = create_sub_bitmap(bitmap, j * taille_w, 1 * taille_h, taille_w, taille_h);
        personnage->droite[j] = create_sub_bitmap(bitmap, j * taille_w, 2 * taille_h, taille_w, taille_h);
        personnage->haut[j] = create_sub_bitmap(bitmap, j * taille_w, 3 * taille_h, taille_w, taille_h);
    }
}

void dessiner_personnage(t_personnage* personnage, BITMAP* buffer)
{
    BITMAP* frame;
    switch (personnage->direction) {
        case 0:
            frame = personnage->bas[personnage->frame];
            break;
        case 1:
            frame = personnage->gauche[personnage->frame];
            break;
        case 2:
            frame = personnage->droite[personnage->frame];
            break;
        case 3:
            frame = personnage->haut[personnage->frame];
            break;
    }
    masked_blit(frame, buffer, 0, 0, personnage->x, personnage->y, frame->w, frame->h);
}


void maj_personnage(t_personnage* personnage)
{
    int vitesse = 1;
    personnage->en_deplacement = 0;
    if (key[KEY_UP]) {
        personnage->y -= vitesse;
        personnage->direction = 3;
        personnage->en_deplacement = 1;
    }
    if (key[KEY_DOWN]) {
        personnage->y += vitesse;
        personnage->direction = 0;
        personnage->en_deplacement = 1;
    }
    if (key[KEY_LEFT]) {
        personnage->x -= vitesse;
        personnage->direction = 1;
        personnage->en_deplacement = 1;
    }
    if (key[KEY_RIGHT]) {
        personnage->x += vitesse;
        personnage->direction = 2;
        personnage->en_deplacement = 1;
    }
}

/*void personnage() {
    allegro_init();
    install_keyboard();

    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1000, 720, 0, 0);

    BITMAP *buffer = create_bitmap(1000, 720);

    BITMAP *background = load_bitmap("../Map.bmp", NULL);
    if (!background) {
        allegro_message("Erreur lors du chargement de 'background.bmp'\n");

    }

    BITMAP *sprite = load_bitmap("../sprite.bmp", NULL);
    if (!sprite) {
        allegro_message("Erreur lors du chargement de 'sprite.bmp'\n");
        destroy_bitmap(background);
        destroy_bitmap(buffer);

    }

    t_personnage personnage;
    personnage.direction = 0;
    personnage.frame = 0;
    personnage.x = SCREEN_W/2;
    personnage.y = SCREEN_H/2;

    separer_bitmap_personnage(&personnage, sprite);

    int frame_counter = 0;

    while (!key[KEY_ESC]) {

        maj_personnage(&personnage);

        stretch_blit(background, buffer, 0, 0, background->w, background->h, 0, 0, buffer->w, buffer->h);

        dessiner_personnage(&personnage, buffer);

        blit(buffer, screen, 0, 0, 0, 0, buffer->w, buffer->h);
        textprintf_ex(screen,font,(SCREEN_W/2)+100,SCREEN_H-(SCREEN_H-100),0xFFFFFF,-1,"x: %d  y: %d ",personnage.x,personnage.y);
        frame_counter++;
        if (frame_counter >= 10) {
            if (personnage.en_deplacement) {
                personnage.frame = (personnage.frame + 1) % 3;
            } else {
                personnage.frame = 0;
            }
            frame_counter = 0;
        }
        if(personnage.x>400 && personnage.x<500 && personnage.y>400 && personnage.y<485){

        }

        rest(10);
    }

    for (int i = 0; i < 3; i++) {
        destroy_bitmap(personnage.bas[i]);
        destroy_bitmap(personnage.gauche[i]);
        destroy_bitmap(personnage.droite[i]);
        destroy_bitmap(personnage.haut[i]);
    }
    destroy_bitmap(sprite);
    destroy_bitmap(background);
    destroy_bitmap(buffer);


}*/


#include <allegro.h>
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#define LARGO 1000
#define ANCHO 720
#define LIMITE_NODOS 200
#define ARRiBA 0
#define ABAJO 1
#define IZQUIERDA 2
#define DERECHA 3
BITMAP *buffer;
BITMAP *cabeza;
BITMAP *cabeza_up;
BITMAP *cabeza_down;
BITMAP *cabeza_right;
BITMAP *cabeza_left;
BITMAP *nodo_cuerpo;
BITMAP *corazon;
BITMAP *comida;
BITMAP *game;
SAMPLE *snake_sound;
SAMPLE *death;
SAMPLE *success;
int LIMITE_X=LARGO/20;
int LIMITE_Y=ANCHO/20;
int DIR = DERECHA;
int TAM=3;
int COMIDA_X,COMIDA_Y;
int PUNTOS2=0,VIDAS=3,PUNTOS1=0;
int VEL=100;
struct Nodo{
    int x,y;
};
struct Nodo cuerpo[LIMITE_NODOS];
void setup(){
    allegro_init();
    install_keyboard();
    set_color_depth(32);
    buffer= create_bitmap(LARGO,ANCHO);
    srand(time(NULL));
    set_gfx_mode(GFX_AUTODETECT_WINDOWED,LARGO,ANCHO,0,0);
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, "pong");
    //buffer= create_bitmap(LARGO,ANCHO);
    snake_sound= load_sample("../apple-eating-36127.wav");
    death= load_sample("../death.wav");
    success= load_sample("../success-fanfare-trumpets-6185.wav");
    cabeza= create_bitmap(20,20);
    cabeza_up= create_bitmap(20,20);
    cabeza_down= create_bitmap(20,20);
    cabeza_right= create_bitmap(20,20);
    cabeza_left= create_bitmap(20,20);
    nodo_cuerpo= create_bitmap(20,20);
    game= create_bitmap(638,638);
    corazon= create_bitmap(20,20);
    comida= create_bitmap(20,20);
    cabeza_up= load_bitmap("../head_up.bmp",NULL);
    game= load_bitmap("../game.bmp",NULL);
    cabeza_down= load_bitmap("../head_down.bmp",NULL);
    cabeza_right= load_bitmap("../head_right.bmp",NULL);
    cabeza_left= load_bitmap("../head_left.bmp",NULL);
    nodo_cuerpo= load_bitmap("../body.bmp",NULL);
    comida= load_bitmap("../apple.bmp",NULL);
    corazon= load_bitmap("../borders.bmp",NULL);
};
void dibujar_bordes(){
    int color= makecol(0,0,255);
    rectfill(buffer,0,ANCHO-20,LARGO,ANCHO,color);
    rectfill(buffer,0,20,20,ANCHO,color);
    rectfill(buffer,0,20,LARGO,20,color);
    rectfill(buffer,LARGO-20,20,LARGO,ANCHO,color);
}
void actualizar_pantalla(){
    blit(buffer,screen,0,0,0,0,LARGO,ANCHO);

}

void generar_snake(){
    cuerpo[0].x=10;
    cuerpo[0].y=9;
    for(int i=1;i<=TAM;i++){
        cuerpo[i].x =cuerpo[0].x - i;
        cuerpo[i].y =cuerpo[0].y;
    }
}
void generar_comida(){
    COMIDA_X=(rand()%(LIMITE_X-2))+1;
    COMIDA_Y=(rand()%(LIMITE_Y-3))+2;
    int c=0;
    while(c<=TAM){
        if(COMIDA_X==cuerpo[c].x&&COMIDA_Y==cuerpo[c].y){
            COMIDA_X=(rand()%(LIMITE_X-2))+1;
            COMIDA_Y=(rand()%(LIMITE_Y-3))+2;
            c=0;
        }
        else{
            c++;
            play_sample(snake_sound, 255, 128, 1000, 0);
        }

    }

}
void dibujar_game(){
    while(!key[KEY_ENTER]){
        stretch_blit(game, screen, 0, 0, game->w, game->h,
                     0, 0, LARGO, ANCHO);
    }
}
void dibujar_comida(){
    draw_sprite(buffer,comida,COMIDA_X*20,COMIDA_Y*20);

}
void dibujar_snake(){

    switch(DIR)
    {
        case ARRiBA:
            blit(cabeza_up,cabeza,0,0,0,0,20,20);
            break;
        case ABAJO:
            blit(cabeza_down,cabeza,0,0,0,0,20,20);
            break;
        case DERECHA:
            blit(cabeza_right,cabeza,0,0,0,0,20,20);
            break;
        case IZQUIERDA:
            blit(cabeza_left,cabeza,0,0,0,0,20,20);
            break;
    }
    draw_sprite(buffer,cabeza,cuerpo[0].x*20,cuerpo[0].y*20);
    for(int i=1;i<=TAM;i++){
        draw_sprite(buffer,nodo_cuerpo,cuerpo[i].x*20,cuerpo[i].y*20);
    }

}
void actualizar_direction(){
    if(key[KEY_UP]){
        if(DIR !=ABAJO){
            DIR=ARRiBA;
        }

    }
    else if(key[KEY_DOWN]){
        if(DIR !=ARRiBA){
            DIR=ABAJO;
        }

    }
    else if(key[KEY_RIGHT]){
        if(DIR !=IZQUIERDA){
            DIR=DERECHA;
        }

    }
    else if(key[KEY_LEFT]){
        if(DIR !=DERECHA){
            DIR=IZQUIERDA;
        }

    }
}
void perder_vida1(){
    VIDAS--;
    if(VIDAS>0){
        clear(screen);
        textout_ex(screen,font,"Vous avez perdue une vie",40*10,40*9,0xFFFFFF,0x00000);
        play_sample(death, 255, 128, 1000, 0);
        rest(2000);
    }
    else{
        clear(screen);
        textout_ex(screen,font,"GAME OVER",40*10,40*9,0xFF0000,0x00000);
        rest(4000);
        clear(screen);

    }

}
void mover_snake(){


    for(int i=TAM;i>0;i--){
        cuerpo[i]=cuerpo[i-1];
    }
    switch(DIR){
        case ARRiBA:
            cuerpo[0].y--;
            break;
        case ABAJO:
            cuerpo[0].y++;
            break;
        case DERECHA:
            cuerpo[0].x++;
            break;
        case IZQUIERDA:
            cuerpo[0].x--;
            break;

    }
    if(cuerpo[0].x == COMIDA_X && cuerpo[0].y ==COMIDA_Y){
        generar_comida();
        TAM++;
        cuerpo[TAM]=cuerpo[TAM-1];
        PUNTOS2= PUNTOS2 + 10;
        if(PUNTOS2 % 100 == 0 && PUNTOS2 > 0 && VEL > 30){
            VEL-=15;
        }
    }
    if(cuerpo[0].x<1||cuerpo[0].x>(LIMITE_X-2)||cuerpo[0].y<2||cuerpo[0].y>(LIMITE_Y-2)){
        perder_vida1();
        generar_snake();
        DIR=DERECHA;
    }
    for(int i=1;i<=TAM;i++){
        if(cuerpo[0].x==cuerpo[i].x && cuerpo[0].y == cuerpo[i].y){
            perder_vida1();
            generar_snake();
            DIR=DERECHA;
            break;
        }
    }

}
void mover_snake2(){
    for(int i=TAM;i>0;i--){
        cuerpo[i]=cuerpo[i-1];
    }
    switch(DIR){
        case ARRiBA:
            cuerpo[0].y--;
            break;
        case ABAJO:
            cuerpo[0].y++;
            break;
        case DERECHA:
            cuerpo[0].x++;
            break;
        case IZQUIERDA:
            cuerpo[0].x--;
            break;

    }
    if(cuerpo[0].x == COMIDA_X && cuerpo[0].y ==COMIDA_Y){
        generar_comida();
        TAM++;
        cuerpo[TAM]=cuerpo[TAM-1];
        PUNTOS1= PUNTOS1 + 10;
        if(PUNTOS1 % 100 == 0 && PUNTOS1 > 0 && VEL > 30){
            VEL-=15;
        }
    }
    if(cuerpo[0].x<1||cuerpo[0].x>(LIMITE_X-2)||cuerpo[0].y<2||cuerpo[0].y>(LIMITE_Y-2)){
        perder_vida1();
        generar_snake();
        DIR=DERECHA;
    }
    for(int i=1;i<=TAM;i++){
        if(cuerpo[0].x==cuerpo[i].x && cuerpo[0].y == cuerpo[i].y){
            perder_vida1();
            generar_snake();
            DIR=DERECHA;
            break;
        }
    }
}
void comenzar_partida1(){
    clear(screen);
    textout_ex(screen,font,"on commence avec le joueur 1",40*10,40*9,0xFFFFFF,0x00000);
    rest(2000);
}
void comenzar_partida2(){
    clear(screen);
    textout_ex(screen,font,"puis joueur 2",40*10,40*9,0xFFFFFF,0x00000);
    rest(2000);
}
void dibujar_puntos(){

    char str_puntos[10];
    sprintf(str_puntos, "%d", PUNTOS2);
    textout_ex(buffer, font, str_puntos, 25, 8, 0xFFFFFF, 0x000000);
}
void dibujar_puntos2(){

    char str_puntos[10];
    sprintf(str_puntos, "%d", PUNTOS1);
    textout_ex(buffer, font, str_puntos, 25, 8, 0xFFFFFF, 0x000000);
}

void dibujar_vidas(){
    textout_ex(buffer,font,"VIES:",LARGO-120,7,0xFFFFFF,0x000000);
    for(int i=0;i<VIDAS;i++){
        draw_sprite(buffer,corazon,i*20+(LARGO-60),5);
    }
}
void partida_1(){
    while(VIDAS>0){
        clear(buffer);
        actualizar_direction();
        mover_snake();
        dibujar_bordes();
        dibujar_snake();
        dibujar_comida();
        dibujar_puntos();
        dibujar_vidas();
        actualizar_pantalla();
        rest(VEL);
    }
}
void partida_2(){
    while(VIDAS>0){
        clear(buffer);
        actualizar_direction();
        mover_snake2();
        dibujar_bordes();
        dibujar_snake();
        dibujar_comida();
        dibujar_puntos2();
        dibujar_vidas();
        actualizar_pantalla();
        rest(VEL);
    }
}
void salida(){
    if(PUNTOS2>PUNTOS1){
        clear(screen);
        textout_ex(screen,font,"joueur 1 a GAGNE !!!!!!",LARGO/2,ANCHO/2,0xFFFFFF,0x000000);
        rest(5000);
    }
    else if(PUNTOS1>PUNTOS2){
        clear(screen);
        textout_ex(screen,font,"joueur 2 a GAGNE !!!!!!",LARGO/2,ANCHO/2,0xFFFFFF,0x000000);
        rest(5000);
    }
    else{
        clear(screen);
        textout_ex(screen,font,"EGALITE !!!!!!",LARGO/2,ANCHO/2,0xFFFFFF,0x000000);
        rest(5000);
    }

    clear(screen);
    stretch_blit(game, screen, 0, 0, game->w, game->h,
                 0, 0, LARGO, ANCHO);
    play_sample(success, 255, 128, 1000, 0);
    while(!key[KEY_ESC]){
        textout_ex(screen,font,"ESC POUR QUITTER",LARGO/8,ANCHO/15,0xFFFFFF,-1);
        textprintf_ex(screen,font,LARGO/8,ANCHO/8,0xFFFFFF,-1,"resume des points joueur 1 : %d points , joueur 2 : %d points",PUNTOS2,PUNTOS1);
    }

    rest(1000);

}
void snake_game(){
    setup();
    dibujar_game();
    dibujar_bordes();
    generar_snake();
    dibujar_snake();
    generar_comida();
    dibujar_comida();
    dibujar_puntos();
    dibujar_vidas();
    actualizar_pantalla();
    comenzar_partida1();
    partida_1();
    VIDAS=3;
    TAM=3;
    VEL=100;
    comenzar_partida2();
    partida_2();
    salida();
}
void personnage() {
    allegro_init();
    install_keyboard();

    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1000, 720, 0, 0);

    BITMAP *buffer = create_bitmap(1000, 720);

    BITMAP *background = load_bitmap("../Map.bmp", NULL);
    if (!background) {
        allegro_message("Erreur lors du chargement de 'background.bmp'\n");

    }

    BITMAP *sprite = load_bitmap("../sprite.bmp", NULL);
    if (!sprite) {
        allegro_message("Erreur lors du chargement de 'sprite.bmp'\n");
        destroy_bitmap(background);
        destroy_bitmap(buffer);

    }

    t_personnage personnage;
    personnage.direction = 0;
    personnage.frame = 0;
    personnage.x = SCREEN_W/2;
    personnage.y = SCREEN_H/2;

    separer_bitmap_personnage(&personnage, sprite);

    int frame_counter = 0;

    while (!key[KEY_D]) {

        maj_personnage(&personnage);

        stretch_blit(background, buffer, 0, 0, background->w, background->h, 0, 0, buffer->w, buffer->h);

        dessiner_personnage(&personnage, buffer);

        blit(buffer, screen, 0, 0, 0, 0, buffer->w, buffer->h);
        textprintf_ex(screen,font,(SCREEN_W/2)+100,SCREEN_H-(SCREEN_H-100),0xFFFFFF,-1,"x: %d  y: %d ",personnage.x,personnage.y);
        frame_counter++;
        if (frame_counter >= 10) {
            if (personnage.en_deplacement) {
                personnage.frame = (personnage.frame + 1) % 3;
            } else {
                personnage.frame = 0;
            }
            frame_counter = 0;
        }
        rest(10);
        if(key[KEY_D]){

            setup();
            dibujar_game();
            dibujar_bordes();
            generar_snake();
            dibujar_snake();
            generar_comida();
            dibujar_comida();
            dibujar_puntos();
            dibujar_vidas();
            actualizar_pantalla();
            comenzar_partida1();
            partida_1();
            VIDAS=3;
            TAM=3;
            VEL=100;
            comenzar_partida2();
            partida_2();
            salida();
            destroy_bitmap(buffer);
        }

        rest(10);
    }





}

int main(){
    allegro_init();
    install_mouse();


    set_gfx_mode(GFX_AUTODETECT_WINDOWED,LARGO,ANCHO,0,0);
    BITMAP *buffer= create_bitmap(LARGO,ANCHO);
    BITMAP *fondo1= load_bitmap("../fond1.bmp",NULL);
    BITMAP *fondo2= load_bitmap("../fond2.bmp",NULL);
    BITMAP *fondo3= load_bitmap("../fond3.bmp",NULL);




    bool sortie = false;

    while (!sortie){
        if(mouse_x>445 && mouse_x<557 && mouse_y>318 && mouse_y<384){
            //blit(fondo2,buffer,0,0,0,0,LARGO,ANCHO);
            stretch_blit(fondo2,buffer,0,0,fondo1->w,fondo1->h,0,0,LARGO,ANCHO);
            if(mouse_b&1){
                personnage();
                break;

            }
        }else if (mouse_x>445 && mouse_x<557 && mouse_y>401 && mouse_y<485){
            stretch_blit(fondo3,buffer,0,0,fondo1->w,fondo1->h,0,0,LARGO,ANCHO);
            //blit(fondo3,buffer,0,0,0,0,LARGO,ANCHO);
            if(mouse_b&1){
                sortie = true;// como un exit 0
            }
        }else{//stretch_blit(game, screen, 0, 0, game->w, game->h,0, 0, LARGO, ANCHO);
            stretch_blit(fondo1,buffer,0,0,fondo1->w,fondo1->h,0,0,LARGO,ANCHO);
            //blit(fondo1,buffer,0,0,0,0,LARGO,ANCHO);
        }
        show_mouse(buffer);

        blit(buffer,screen,0,0,0,0,LARGO,ANCHO);

    }
    destroy_bitmap(buffer);

    return 0;
}END_OF_MAIN();

