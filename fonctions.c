#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "initialisation.h"
#include <SDL/SDL_getenv.h>
#include <fmodex/fmod.h>
#include "time.h"



FMOD_SOUND *musiqueJeu;
FMOD_SOUND *jeu;
FMOD_SOUND *creditos;
FMOD_RESULT resultat;
SDL_Surface *ecran, *copter, *tank, *background1, *blast, *hostage[4], *bunker[4],*cpu[30],*ot[16];
SDL_Surface *compteur_juif, *score1, *score2, *missiles[30];
SDL_Rect positionjuive, positionscore1, positionscore2, positionmissiles[30];
SDL_Surface *vie1, *vie2, *vie3;
SDL_Rect positionneeded, positioncopbg1,positionblast,oldpositionblast,positionenemy[30];
SDL_Rect positionbg1, positioncopter,positiontank,positionblast1,positionbunker[4],positionotage[4];
SDL_Rect positionvie1, positionvie2, positionvie3,oldpositionotage[4],positionotage_c[4],fauxbunker[4],vraiotage[4];
SDL_Rect clipper;
SDL_Rect positionblastt;
SDL_Event event;

SDL_Surface *barre, *danger1, *danger2, *alive1, *alive2;
SDL_Rect positionbarre, positiondanger1, positiondanger2, positionalive1, positionalive2;


int anime_missile[30];
int explosion_missile[30];
int anime_explosion[30];
int explosion_token[30];
int missile_token[30];
int moving_token[30];

int valeur_otage[4];
int otage_timer = 0;
int otage_golden = 64;
int invinci_otage = 0;
int otage_silver = 64;

int lazor = 0;
SDL_Rect positionblastbg1;


int explosion_bomb = 0;


int chopper_state = 1;
int enemy_pres = 0;
int firstbblast = 0;
int blast_pres = 0;
int otage_pres = 1;
int otage_tampux[4];
int otage_depose=0;
int nb_otage = 0;
int rand_cote[30];
int max_enemy[30]; //(à changer) BALISE SPAWN
int cpu_type[30]; //(à changer) BALISE SPAWN
int etat_missile[30];
int tank_speed[30];
int lifes;
int depo = 0, depoc = 0;
int dead=0,compteur=0;
int blaster =0, blastcompteur = 0,compt = 0, cpt = 0;
int boucle = 0;
int max_blast[20];
int pos_blast[20];
int bunker_state[4];
int depl_otage[4];
int provijuif = 0;

int antilag_token = 0;



void menu(void)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    int continuer = 1;
    putenv("SDL_VIDEO_CENTERED=1");


    SDL_WM_SetIcon(IMG_Load("Chopper.png"), NULL);          // Changement
    ecran = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);      // Rajout double buff
    SDL_WM_SetCaption("Choplifter", NULL);
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0,30,160));
	init();
}


int init()
{
	FMOD_SYSTEM *system;
    FMOD_SOUND *musiqueMenu;
    FMOD_SOUND *curseur;
    FMOD_SOUND *valide;

	SDL_Surface *cursor, *menu = NULL;
	SDL_Rect positionmenu, positioncursor;


	int i;

    for(i=0; i<30; i++)
    {
        max_enemy[i] = 0;
        anime_explosion[i] = 0;
        anime_missile[i] = 0;
        explosion_missile[i] = 0;
    }

    for(i=0; i<4; i++)
    {
        valeur_otage[i] = 16;
        bunker_state[i] = 1;
        depl_otage[i] = 1;
    }

    menu = IMG_Load("Menu_mk2.jpeg");

	positionmenu.x = 0;
	positionmenu.y = 0;

	SDL_BlitSurface(menu, NULL, ecran, &positionmenu);

	cursor = IMG_Load("Cursor.png");
	positioncursor.x = 26;
	positioncursor.y = 201;
    SDL_BlitSurface(cursor, NULL, ecran, &positioncursor);
    SDL_Flip(ecran); // Maj de l'écran


    FMOD_System_Create(&system);
    FMOD_System_Init(system, 10, FMOD_INIT_NORMAL, NULL);

    /* On ouvre la musique */
    resultat = FMOD_System_CreateSound(system, "Rocky.mp3", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musiqueMenu);
    resultat = FMOD_System_CreateSound(system, "Modern.mp3", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, 0, &musiqueJeu);
    resultat = FMOD_System_CreateSound(system, "cursor.wav", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &curseur);
    resultat = FMOD_System_CreateSound(system, "Credits.mp3", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &creditos);
    resultat = FMOD_System_CreateSound(system, "ready.wav", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &valide);
    resultat = FMOD_System_CreateSound(system, "load.wav", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &jeu);

    /* On active la répétition de la musique à l'infini */
    FMOD_Sound_SetLoopCount(musiqueMenu, -1);

    /* On joue la musique */

    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, musiqueMenu, 0, NULL);

	lifes = 3;



	 while (1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
					case SDLK_RETURN:
                    if (positioncursor.y == 201)
                    {
                    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, jeu, 0, NULL);
                    SDL_Delay(600);
                    FMOD_Sound_Release(musiqueMenu);
                    FMOD_Sound_SetLoopCount(musiqueJeu, -1);
                    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, musiqueJeu, 0, NULL);


                    SDL_FreeSurface(ecran);
                    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));


                    copter = IMG_Load("copter.png");


                    positioncopter.x = 516;
                    positioncopter.y = 472;

                    positionbg1.x = -1600;
                    positionbg1.y = 0;


                    positionbunker[0].x = 175;
					positionbunker[1].x = 435;
					positionbunker[2].x = 899;
					positionbunker[3].x = 1277;


                    for(i=0; i<4; i++)
                    {
                        positionotage[i].x = positionbunker[i].x;
                        positionotage[i].y = 385;
                        positionbunker[i].y = 385;
                    }


					vie1 = IMG_Load("copter.png");
                    vie2 = IMG_Load("copter.png");
                    vie3 = IMG_Load("copter.png");

					// CHANGEMENT ICI !!!
                    compteur_juif = IMG_Load("compteur-juif.png");
                    score1 = IMG_Load("0.png");
                    score2 = IMG_Load("0.png");

                    barre = IMG_Load("Scoring/barre.png");
                    alive1 = IMG_Load("Scoring-vert/0.png");
                    alive2 = IMG_Load("Scoring-vert/0.png");
                    danger1 = IMG_Load("Scoring-rouge/0.png");
                    danger2 = IMG_Load("Scoring-rouge/0.png");


                    positionvie1.x = 0;
                    positionvie1.y = 10;

                    positionvie2.x = 55;
                    positionvie2.y = 10;

                    positionvie3.x = 110;
                    positionvie3.y = 10;


                    positionjuive.x = 620;
                    positionjuive.y = 0;

                    positionscore1.x = 700;
                    positionscore1.y = 0;

                    positionscore2.x = 750;
                    positionscore2.y = 0;



                    positionalive1.x = 270;
                    positionalive1.y = 0;
                    positionalive2.x = 320;
                    positionalive2.y = 0;

                    positionbarre.x = 370;
                    positionbarre.y = 0;

                    positiondanger1.x = 420;
                    positiondanger1.y = 0;
                    positiondanger2.x = 470;
                    positiondanger2.y = 0;



                    background1 = IMG_Load("background1.png");
                    SDL_BlitSurface(background1, NULL, ecran, &positionbg1);
                    SDL_BlitSurface(copter, NULL, ecran, &positioncopter);


                    SDL_Flip(ecran);
                    anime();
					SDL_FreeSurface(cursor);
                    SDL_FreeSurface(menu);
                    }

                    if (positioncursor.y == 311)
                    {
                    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, valide, 0, NULL);
                    SDL_Delay(600);
                    FMOD_Sound_Release(musiqueMenu);
                    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, creditos, 0, NULL);
                    credits();
                    }

                    if (positioncursor.y == 421)
                    {
                    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, valide, 0, NULL);
                    SDL_Delay(600);
                    SDL_Quit();
                    return 0;
                    }

                    break;

                    case SDLK_UP:
                    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, curseur, 0, NULL);
                    if (positioncursor.y == 201)
                    {
                        positioncursor.y = 421;
                    }
                    else
                    {
                        positioncursor.y = positioncursor.y-110;
                    }
                    //printf("Cursor     => x= %d \t y= %d \n", positioncursor.x , positioncursor.y);
                    break;

                    case SDLK_DOWN:
                    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, curseur, 0, NULL);
                    if (positioncursor.y == 421)
                    {
                        positioncursor.y = 201;
                    }
                    else
                    {
                        positioncursor.y = positioncursor.y+110;
                    }
                    //printf("Cursor     => x= %d \t y= %d \n", positioncursor.x , positioncursor.y);
                    break;
                }
            break;
        }
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
        SDL_BlitSurface(menu, NULL, ecran, &positionmenu);
        SDL_BlitSurface(cursor, NULL, ecran, &positioncursor);

        SDL_Flip(ecran);

	 }
}


void input_handle(void)
{
    while (SDL_PollEvent(&event)) {

        switch (event.type) {
        case SDL_QUIT:
            SDL_Quit();
            exit(0);
            break;
        }
    }
}



void anime(void)
{

	Uint8 *keystates = SDL_GetKeyState( NULL );
	int width = 56, height = 16, anim_courante = 0, i = 0, j = 0, height_otage = 18, width_otage = 16, anime_otage = 0;
	SDL_Rect oldpositioncopter, position_anim;
    SDL_Rect positionbg1_c,positioncopter_c,positionblast_c,position_anim_otage;
    FMOD_SYSTEM *system;
    FMOD_SOUND *ahw;
    FMOD_SOUND *ouille;
    FMOD_SOUND *musiqueGo;
    FMOD_SOUND *musiqueWin;
    FMOD_SOUND *blast_tir;
    FMOD_SOUND *bomb_tir;

    FMOD_System_Create(&system);
    FMOD_System_Init(system, 10, FMOD_INIT_NORMAL, NULL);
    resultat = FMOD_System_CreateSound(system, "ahw.wav", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &ahw);
    resultat = FMOD_System_CreateSound(system, "blaster.wav", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &blast_tir);
    resultat = FMOD_System_CreateSound(system, "bomb.wav", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &bomb_tir);
    resultat = FMOD_System_CreateSound(system, "ouille.wav", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &ouille);
    resultat = FMOD_System_CreateSound(system, "Gameover.mp3", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musiqueGo);
    resultat = FMOD_System_CreateSound(system, "Victory.mp3", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musiqueWin);




	copter = IMG_Load("animation-copter-left.png");
	blast = IMG_Load("fire.png");


	for(i=0; i<4; i++)
    {
        hostage[i] = IMG_Load ("otage-right.png");
        bunker[i] = IMG_Load("bunker.png");
        SDL_BlitSurface(bunker[i], NULL, background1, &positionbunker[i]);
    }


	SDL_BlitSurface(copter, NULL, ecran, &positioncopter);

	SDL_Flip(ecran);

    positionbg1.x = -1600;
    positionbg1.y = 0;

    positioncopter.x = 477;
    positioncopter.y = 472;

    oldpositioncopter.x = 516;
    oldpositioncopter.y = 472;

	positionblast.x = 4000;
	positionblast.y = 4000;



    for (i=0; i<4; i++)
    {
        oldpositionotage[i].x = ecran->w - hostage[i]->w;
        oldpositionotage[i].y = 0;
    }



	SDL_EnableKeyRepeat(10, 10);
	while (1)
		{
			if (dead == 1)
            {
                if (chopper_state == 1)
                {
                    copter = IMG_Load("animation-copter-left-invinci.png");
                }
                if (chopper_state == 2)
                {
                    copter = IMG_Load("animation-copter-right-invinci.png");
                }
                if (chopper_state == 3 || chopper_state == 4)
                {
                    copter = IMG_Load("copter-face-invinci.png");
                }
            }
            if (dead == 0)
            {
                if (chopper_state == 1)
                {
                    copter = IMG_Load("animation-copter-left.png");
                }
                if (chopper_state == 2)
                {
                    copter = IMG_Load("animation-copter-right.png");
                }
                if (chopper_state == 3 || chopper_state == 4)
                {
                    copter = IMG_Load("copter-face.png");
                }
            }

			if (lifes == 0)
			{
			    FMOD_Sound_Release(musiqueJeu);
			    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, musiqueGo, 0, NULL);
				gameover();
			}

            if (otage_golden <= 0)
            {
                if ((chopper_state == 4) && (positioncopbg1.x >= 2000) && (positioncopbg1.x <= 2284))
                {
                    FMOD_Sound_Release(musiqueJeu);
                    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, musiqueWin, 0, NULL);
                    winner();
                }
            }

			boucle ++;
			positioncopbg1.x = (positionbg1.x - positioncopter.x) * -1;
			positioncopbg1.y = (positionbg1.y - positioncopter.y) * -1;


            fauxbunker[0].x = (-235 - positionbg1.x)*(-1);
            fauxbunker[1].x = (-495 - positionbg1.x)*(-1);
            fauxbunker[2].x = (-959 - positionbg1.x)*(-1);
            fauxbunker[3].x = (-1337 - positionbg1.x)*(-1);

            for (i=0; i<4; i++)
            {
                vraiotage[i].x = (positionbg1.x - positionotage[i].x) * -1;
                vraiotage[i].y = (positionbg1.y - positionotage[i].y) * -1;
                fauxbunker[i].y = positionbunker[i].y;
                otage_tampux[i] = fauxbunker[i].x;
            }


			clipper.x = 0;
			clipper.y = 0;
			clipper.h = ecran->h;
			clipper.w = ecran->w;
			SDL_SetClipRect(ecran,&clipper);

			positionbg1_c.x = positionbg1.x;
			positionbg1_c.y = positionbg1.y;

			positioncopter_c.x = positioncopter.x;
			positioncopter_c.y = positioncopter.y;

			SDL_BlitSurface(background1,NULL, ecran, &positionbg1_c);

			clipper.x = 0;
			clipper.y = 0;
			clipper.h = ecran->h;
			clipper.w = ecran->w;
			SDL_SetClipRect(ecran,&clipper);

			positionbg1_c.x = positionbg1.x;
			positionbg1_c.y = positionbg1.y;

			positionblast_c.x = positionblast.x;
			positionblast_c.y = positionblast.y;

			SDL_BlitSurface(background1,NULL, ecran, &positionbg1_c);

            for (i=0; i<4; i++)
            {
                clipper.x = oldpositionotage[i].x;
                clipper.y = oldpositionotage[i].y;
                clipper.h = hostage[i]->h;
                clipper.w = hostage[i]->w;
            }


			SDL_SetClipRect(ecran,&clipper);

			positionbg1_c.x = positionbg1.x;
			positionbg1_c.y = positionbg1.y;


            for (i=0; i<4; i++)
            {
                positionotage_c[i].x = positionotage[i].x;
                positionotage_c[i].y = positionotage[i].y;
            }




			SDL_BlitSurface(background1,NULL, ecran, &positionbg1_c);

			SDL_SetClipRect(ecran,NULL);

			SDL_BlitSurface(copter,&position_anim, ecran, &positioncopter_c);


			for (i=0; i<4; i++)
            {
                if(bunker_state[i] == 0)
                {
                    if (otage_timer >= 60)
                    {
                        if (valeur_otage[i] > 0)
                        {
                            recept_otage(i);
                            otage_timer = 0;
                        }
                    }
                }
            }
            otage_timer++;

            for (i=0; i<4; i++)
            {
                if(bunker_state[i] == 0)
                {
                    if (valeur_otage[i] > 0)
                    {
                        SDL_BlitSurface(hostage[i],&position_anim_otage,ecran,&positionotage[i]);
                    }
                }
            }



			SDL_BlitSurface(blast,NULL, ecran, &positionblast_c);
			SDL_BlitSurface(vie1, NULL, ecran, &positionvie1);
            SDL_BlitSurface(vie2, NULL, ecran, &positionvie2);
            SDL_BlitSurface(vie3, NULL, ecran, &positionvie3);
			SDL_BlitSurface(compteur_juif, NULL, ecran, &positionjuive);
            SDL_BlitSurface(score1, NULL, ecran, &positionscore1);
            SDL_BlitSurface(score2, NULL, ecran, &positionscore2);

            SDL_BlitSurface(barre, NULL, ecran, &positionbarre);
            SDL_BlitSurface(alive1, NULL, ecran, &positionalive1);
            SDL_BlitSurface(alive2, NULL, ecran, &positionalive2);
            SDL_BlitSurface(danger1, NULL, ecran, &positiondanger1);
            SDL_BlitSurface(danger2, NULL, ecran, &positiondanger2);


            for (i=0; i<4; i++)
            {
                SDL_BlitSurface(bunker[i], NULL, background1, &positionbunker[i]);
                oldpositionotage[i].x = positionotage[i].x;
                oldpositionotage[i].y = positionotage[i].y;
            }


			oldpositioncopter.x = positioncopter.x;
			oldpositioncopter.y = positioncopter.y;

			oldpositionblast.x = positionblast.x;
			oldpositionblast.y = positionblast.y;

			position_anim.x = 0;
			position_anim.y = anim_courante*height;
			position_anim.w = width;
			position_anim.h = height;

			position_anim_otage.x = 0;
			position_anim_otage.y = anime_otage*height_otage;
			position_anim_otage.h = height_otage;
			position_anim_otage.w = width_otage;

			spawn();

			for (i=0; i<30; i++)
            {
                moving_enemy(i);
                destruct_enemy(i);
                moving_missiles(i);

                if (collisionextrem(i, 0) == 1 || collisionextrem(i, 1) == 1 || collisionextrem(i, 2) == 1 || collisionextrem(i, 3) == 1)
                {
                    if (invinci_otage >= 100)
                    {
                        if (collisionextrem(i, 0) == 1 && valeur_otage[0] != 0)
                        {
                            valeur_otage[0]--;
                            FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, ouille, 0, NULL);
                            otage_golden--;
                            otage_silver--;
                        }
                        if (collisionextrem(i, 1) == 1 && valeur_otage[1] != 0)
                        {
                            valeur_otage[1]--;
                            FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, ouille, 0, NULL);
                            otage_golden--;
                            otage_silver--;
                        }
                        if (collisionextrem(i, 2) == 1 && valeur_otage[2] != 0)
                        {
                            valeur_otage[0]--;
                            FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, ouille, 0, NULL);
                            otage_golden--;
                            otage_silver--;
                        }
                        if (collisionextrem(i, 3) == 1 && valeur_otage[3] != 0)
                        {
                            valeur_otage[3]--;
                            FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, ouille, 0, NULL);
                            otage_golden--;
                            otage_silver--;
                        }


                        invinci_otage = 0;
                    }
                }
            }
            invinci_otage++;

            for (j=0; j<4; j++)
            {
                if (lazor == 3) // Rajouter collision missile
                {
                    if (collisionc(j) == 1)
                    {
                        if (bunker_state[j] == 1)
                        {
                           bunker_state[j] = 0;
                            SDL_FreeSurface(bunker[j]);
                            bunker[j] = IMG_Load("bunker-detruit.png");
                            otage(j);
                        }
                    }
                }
                deplacement_otage(j);
            }

			/*printf("Copter => x= %d \t y= %d \n", positioncopter.x , positioncopter.y);
			printf("Background => x= %d \t y= %d \n\n", positionbg1.x, positionbg1.y);
			printf("AVION => x= %d \t y= %d \n\n", positionmur.x, positionmur.y);
			printf("BLAST => x= %d \t y= %d \n\n", positionblast.x, positionblast.y);
			printf("ENEMY PRES => %d \t \n\n", enemy_pres);*/
		/*
			printf("LA VRAIE POS DU BLAST => x= %d \t y= %d \n\n", positionblast1.x, positionblast1.y);*/

		/*	printf("LA VRAIE POS DE COPTER => x= %d \t y= %d \n\n", positioncopbg1.x, positioncopbg1.y);*/
			/*printf("OTAGE => x= %d \t y= %d \n\n", positionotage.x, positionotage.y);*/
			/*printf("NOMBRE DOTAGES =>\t %d\n\n", provijuif);
			printf("LA VRAIE POS DE COPTER => x= %d \t y= %d \n\n", positioncopbg1.x, positioncopbg1.y);
			for (i = 0; i < 16; i++)
			{
				printf("POSITION OTAGE num [%d] => x %d \t y = %d \n\n",i,vraiotage[i].x,vraiotage[i].y);
			}*/
			/*
			printf("LA VRAIE POS DE OTAGE => x= %d \t y= %d \n\n", vraiotage.x, vraiotage.y);
			printf("OTAGE => x= %d \t y= %d \n\n", positionotage.x, positionotage.y);*/

			moving_tir();

			for (i = 0; i < 30; i++)
			{
				if (/*collision() == 1 ||*/ collisione(i) == 1 || collisionm(i) == 1)
				{
					//printf("\tLIFESSSSSSSSSSSSSS\n\n %d",dead);
					if (dead == 0)
					{
						FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, ahw, 0, NULL);
						dead = 1;
						lifes --;
						compteur = 0;
						otage_golden = otage_golden - provijuif;
                        otage_silver = otage_silver - provijuif;
                        provijuif = 0;
						if(lifes == 2)
						{
							vie3 = IMG_Load("die.png");
						}
						if(lifes == 1)
						{
							vie2 = IMG_Load("die.png");
						}
						if(lifes == 0)
						{
							vie1 = IMG_Load("die.png");
						}
					}
				}
			}

            compteur++;
            if (compteur >= 100)
            {
                dead = 0;
            }


			deplacement();


			tir();
			//tir_avion();

			collage_score(provijuif);
			danger_score(otage_silver);
			alive_score(otage_depose);


			depose_otage();

            //printf("Total vivants : %d\n", otage_golden);

		/*	printf("%d",collision());
			printf("%d",collision());
			printf("%d",collision());
			printf("%d",collision());
			printf("%d",collision());
			printf("%d",collision());
			printf("%d",collision());
			printf("%d",collision());
			printf("%d",collision());
			printf("%d",collision());*/
			/*printf("%d",collisionb());
			printf("%d",collisionb());
			printf("%d",collisionb());
			printf("%d",collisionb());
			printf("%d",collisionb());
			printf("%d",collisionb());
			printf("%d",collisionb());
			printf("%d",collisionb());
			printf("%d",collisionb());
			printf("%d",collisionb());*/
			if (lazor == 1 || lazor == 2)
            {
                if (antilag_token == 0)
                {
                    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, blast_tir, 0, NULL);
                    antilag_token = 1;
                }
            }
            if (antilag_token == 2)
            {
                FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, bomb_tir, 0, NULL);
                antilag_token = 1;
            }


			positionneeded.x = -positionbg1.x;
			if (chopper_state != 4)
			{
				anim_courante++;

				if (anim_courante == 8)
				{
					anim_courante=0;
				}

			}

            anime_otage++;

            if (anime_otage == 2)
            {
                anime_otage = 0;
            }
			SDL_Flip(ecran);
			input_handle();
		}
}


void spawn(void)
{
    int opponent, posi_plus, posi_moins, appa_enn, i, protec_base, y_avion;

    appa_enn = (rand()%1000)+1;
    opponent = (rand()%1000)+1;
    posi_plus = (rand()%950)+850;
    posi_moins = (rand()%150)+50;
    y_avion = (rand()%335)+54;

    //printf("\nTimer : %d\n", opponent);
    if(opponent > 978)
    {
        protec_base = positionneeded.x + posi_plus;

        for(i=0; i<30; i++) // à changer - BALISE SPAWN
        {
            if(max_enemy[i] == 0)
            {
                /*printf("Case %d : %d\n", i+1, max_enemy[i]);
                printf("\n\ncpu n#%d spawn\n", i+1);*/
                max_enemy[i] = 1;
                explosion_token[i] = 0;
                moving_token[i] = 1;
                rand_cote[i] = (rand()%2)+1;

                if (positioncopbg1.x < 800)
                {
                    rand_cote[i] = 2;
                }

                if (rand_cote[i] == 1)
                {
                    if (appa_enn <= 525)
                    {
                        tank_speed[i] = (rand()%5)+1;
                        cpu[i] = IMG_Load("Tank-right.png");
                        positionenemy[i].y = 460;
                        cpu_type[i] = 1;
                    }
                    if (appa_enn > 525 && appa_enn <= 850)
                    {
                        cpu[i] = IMG_Load("avion-right.png");
                        positionenemy[i].y = y_avion;
                        etat_missile[i] = 0;
                        cpu_type[i] = 2;
                    }
                    if (appa_enn > 850)
                    {
                        cpu[i] = IMG_Load("ufo-solo.png");
                        positionenemy[i].y = 100;
                        cpu_type[i] = 3;
                    }
                    positionenemy[i].x = positionneeded.x - posi_moins;
                    SDL_BlitSurface(cpu[i], NULL, background1, &positionenemy[i]);
                  /*  printf("\nposition : %d\n", positionenemy.x);*/
                }
                if (rand_cote[i] == 2)
                {
                    if (protec_base <= 1875)
                    {
                        if (appa_enn <= 525)
                        {
                            tank_speed[i] = (rand()%5)+1;
                            cpu[i] = IMG_Load("Tank-left.png");
                            positionenemy[i].y = 460;
                            cpu_type[i] = 1;
                        }
                        if (appa_enn > 525 && appa_enn <= 850)
                        {
                            cpu[i] = IMG_Load("avion-left.png");
                            etat_missile[i] = 0;
                            positionenemy[i].y = y_avion;
                            cpu_type[i] = 2;
                        }
                        if (appa_enn > 850)
                        {
                            cpu[i] = IMG_Load("ufo-solo.png");
                            positionenemy[i].y = 100;
                            cpu_type[i] = 3;
                        }
                        positionenemy[i].x = protec_base;
                        SDL_BlitSurface(cpu[i], NULL, background1, &positionenemy[i]);
                    }
                  /*  printf("\nposition : %d\n", positionenemy.x);*/
                }
                enemy_pres++;

                break;
            }
        }
    }
}


void tir(void)
{
	Uint8 *keystatest = SDL_GetKeyState( NULL );

    if (keystatest [SDLK_SPACE])
    {
        if (lazor == 0)
        {
            if (chopper_state == 3)
            {
                blast = IMG_Load("fire.png");
                positionblast.y = (positioncopter.y + 10);
                positionblast.x = (positioncopter.x + 20);
                positionblastbg1.y = positioncopbg1.y + 10;
                positionblastbg1.x = positioncopbg1.x + 20;
                lazor = 3;
            }

            if (chopper_state == 1)
            {
                blast = IMG_Load("fire-droite.png");
                positionblast.y = positioncopter.y;
                positionblast.x = positioncopter.x;
                positionblastbg1.y = positioncopbg1.y;
                positionblastbg1.x = positioncopbg1.x;
                lazor = 1;
            }

            if (chopper_state == 2)
            {
                blast = IMG_Load("fire-gauche.png");
                positionblast.y = positioncopter.y;
                positionblast.x = positioncopter.x;
                positionblastbg1.y = positioncopbg1.y;
                positionblastbg1.x = positioncopbg1.x;
                lazor = 2;
                }
            }
        }
}


void moving_tir()
{
    if(lazor == 1)
    {
        positionblast.x = positionblast.x - 20;
        SDL_BlitSurface(blast,NULL, ecran, &positionblast);
        SDL_Flip(ecran);
        positionblastbg1.x = (positionbg1.x - positionblast.x) * -1;
        positionblastbg1.y = (positionbg1.y - positionblast.y) * -1;
        if (positionblast.x <= 20)
        {
            lazor = 0;
            antilag_token = 0;
            blast = IMG_Load("clipper.png");
            positionblastbg1.x = 3000;
            positionblastbg1.y = 1000;
        }
    }

    if(lazor == 2)
    {
        positionblast.x = positionblast.x + 20;
        SDL_BlitSurface(blast,NULL, ecran, &positionblast);
        SDL_Flip(ecran);
        positionblastbg1.x = (positionbg1.x - positionblast.x) * -1;
        positionblastbg1.y = (positionbg1.y - positionblast.y) * -1;
        if (positionblast.x >= 780)
        {
            lazor = 0;
            antilag_token = 0;
            blast = IMG_Load("clipper.png");
            positionblastbg1.x = 3000;
            positionblastbg1.y = 1000;
        }
    }

    if(lazor == 3)
    {
        positionblast.y = positionblast.y + 20;
        SDL_BlitSurface(blast,NULL, ecran, &positionblast);
        SDL_Flip(ecran);
        positionblastbg1.x = (positionbg1.x - positionblast.x) * -1;
        positionblastbg1.y = (positionbg1.y - positionblast.y) * -1;
        if (positionblast.y >= 480)
        {
            blast = IMG_Load("clipper.png");
            if (explosion_bomb == 1)
            {
                antilag_token = 2;
                blast = IMG_Load("Explosions/explosion-1.png");
            }
            if (explosion_bomb == 2)
            {
                blast = IMG_Load("Explosions/explosion-2.png");
            }
            if (explosion_bomb == 3)
            {
                blast = IMG_Load("Explosions/explosion-3.png");
            }
            if (explosion_bomb == 4)
            {
                blast = IMG_Load("Explosions/explosion-4.png");
            }
            if (explosion_bomb == 5)
            {
                blast = IMG_Load("Explosions/explosion-5.png");
            }
            if (explosion_bomb == 6)
            {
                blast = IMG_Load("Explosions/explosion-6.png");
            }
            if (explosion_bomb == 7)
            {
                blast = IMG_Load("Explosions/explosion-7.png");
            }
            if (explosion_bomb == 8)
            {
                blast = IMG_Load("Explosions/explosion-8.png");
            }
            explosion_bomb++;

            if (explosion_bomb == 9)
            {
                antilag_token = 0;
                lazor = 0;
                blast = IMG_Load("clipper.png");
                explosion_bomb = 0;
                positionblastbg1.x = 3000;
                positionblastbg1.y = 1000;
            }
        }
    }
}


void deplacement(void)
{

 Uint8 *keystates = SDL_GetKeyState( NULL );
 int i=0;
 switch(event.type)

  {
     case SDL_KEYDOWN:
     /*switch(event.key.keysym.sym)
     {
     case SDLK_RCTRL:*/

     /*}*/

     /* if(event.key.keysym.sym ==  SDLK_UP)
      positioncopter.y = (positioncopter.y-4);
     if(event.key.keysym.sym == SDLK_DOWN)
      positioncopter.y = (positioncopter.y+4);
     if(event.key.keysym.sym == SDLK_RIGHT)
      positioncopter.x = (positioncopter.x+4);
     if(event.key.keysym.sym == SDLK_LEFT)
      positioncopter.x = (positioncopter.x-4);*/

     if( keystates[ SDLK_UP ] ) // Si haut est pressé
     {
         if(chopper_state == 4) // Si haut est pressé
                        {
                            chopper_state = 3;
                        }
      if (positioncopter.y > 54)
      {
            positioncopter.y = (positioncopter.y-4);
      }
     }

     if( keystates[ SDLK_DOWN ] ) //Si Bas est pressé
     {
      positioncopter.y = (positioncopter.y+4);
    /*  if (collision()==1)
        positioncopter.y = (positioncopter.y-8);*/
     }

     if( keystates[ SDLK_LEFT ] ) //Si Gauche est pressé
     {
      copter = IMG_Load("animation-copter-left.png");
      SDL_SetColorKey(copter, SDL_SRCCOLORKEY, SDL_MapRGB(copter->format, 0, 255, 0));
      chopper_state = 1;
      /*printf("Chopper state : %d", chopper_state);*/
       if (positionbg1.x < 4)
      {
       if (positioncopter.x <= 200)
       {
			positionbg1.x = (positionbg1.x+4);
			for (i=0; i<4; i++)
            {
                if(fauxbunker[i].x > 0)
                {
                    positionotage[i].x = (positionotage[i].x + 4);
                }
            }
       }
       else
       {
        positioncopter.x = (positioncopter.x-4);
      /*  if (collision()==1)
        positioncopter.x = (positioncopter.x+8);*/
       }
      }
        else
        {
            positioncopter.x = (positioncopter.x-4);
        }
     }

     if( keystates[ SDLK_RIGHT ] ) //Si Droite est pressé
     {
      copter = IMG_Load("animation-copter-right.png");
      chopper_state = 2;
      SDL_SetColorKey(copter, SDL_SRCCOLORKEY, SDL_MapRGB(copter->format, 0, 255, 0));
      /*printf("Chopper state : %d", chopper_state);*/
      if (positionbg1.x > -1600)
      {
       if (positioncopter.x >= 600)
       {
			positionbg1.x = (positionbg1.x-4);
			for (i=0; i<4; i++)
            {
                if(fauxbunker[i].x > 0)
                {
                    positionotage[i].x = (positionotage[i].x - 4);
                }
            }
       }
       else
       {
        positioncopter.x = (positioncopter.x+4);
       /* if (collision()==1)
       positioncopter.x = (positioncopter.x-8);*/
       }
      }
      else
      {
       positioncopter.x = (positioncopter.x+4);
       /*if (collision()==1)
       positioncopter.x = (positioncopter.x-8);*/
      }
     }

     if( keystates[ SDLK_RCTRL ] )
     {
      if(chopper_state == 1 || chopper_state ==2)
       {
        copter = IMG_Load("copter-face.png");
        chopper_state = 3;

       }
     }


     if (positioncopter.x >= ecran->w - copter->w - 4)
     {
      positioncopter.x = (positioncopter.x-4);
     }
     else if (positioncopter.x <= 5)
     {
      positioncopter.x = (positioncopter.x+4);
     }
     if (positioncopter.y >= ecran->h - copter->h - 4)
     {
     positioncopter.y = (positioncopter.y-4);
     copter = IMG_Load("copter-face.png");
     chopper_state = 4;
     }
     else if (positioncopter.y == 0)
     {
     positioncopter.y = (positioncopter.y+4);
     }
     break;


    }
}


int collisionb(enn) // Tir sur ennemi
{
	 if ((positionblastbg1.x) >= (positionenemy[enn].x-8)
    && positionblastbg1.x < (positionenemy[enn].x+8) + positionenemy[enn].w
    && (positionblastbg1.y) >= (positionenemy[enn].y-8)
    && positionblastbg1.y < (positionenemy[enn].y+8) + positionenemy[enn].h)

          return (1);
   else
          return (0);
}


int collisionc(bunk)
{
	 if ((positionblastbg1.x) >= (positionbunker[bunk].x-8)
    && positionblastbg1.x < (positionbunker[bunk].x+8) + positionbunker[bunk].w
    && (positionblastbg1.y) >= (positionbunker[bunk].y-8)
    && positionblastbg1.y < (positionbunker[bunk].y+8) + positionbunker[bunk].h)

          return (1);
   else
          return (0);
}


int collisione(enn)     // Collision avec ennemi
{
	if ((positioncopbg1.x+50) >= (positionenemy[enn].x-8)        // Rajout
    && positioncopbg1.x < (positionenemy[enn].x+8) + positionenemy[enn].w
    && (positioncopbg1.y+14) >= (positionenemy[enn].y-8)
    && positioncopbg1.y < (positionenemy[enn].y+8) + positionenemy[enn].h)
    {
        return (1);
    }

    else
    {
        return (0);
    }
}


int collisionm(enn)     // Collision avec missile
{
	if ((positioncopbg1.x+30) >= (positionmissiles[enn].x-8)        // Rajout
    && positioncopbg1.x < (positionmissiles[enn].x+8) + positionmissiles[enn].w
    && (positioncopbg1.y+10) >= (positionmissiles[enn].y-8)
    && positioncopbg1.y < (positionmissiles[enn].y+8) + positionmissiles[enn].h)
    {
        return (1);
    }

    else
    {
        return (0);
    }
}


int collisiono(ota)
{
	 if ((positioncopbg1.x+56) >= (vraiotage[ota].x-8)
    && (positioncopbg1.x) < (vraiotage[ota].x+8) + vraiotage[ota].w
    && (positioncopbg1.y+16) >= (vraiotage[ota].y-8)
    && positioncopbg1.y < (vraiotage[ota].y+8) + vraiotage[ota].h)

          return (1);
   else
          return (0);
}


int collisionextrem(enn, ota)
{
    if (bunker_state[ota] == 0 && max_enemy[enn] == 1 && cpu_type[enn] == 1)
    {
         if ((positionenemy[enn].x+56) >= (vraiotage[ota].x-8)
        && (positionenemy[enn].x) < (vraiotage[ota].x+8) + vraiotage[ota].w
        && (positionenemy[enn].y+16) >= (vraiotage[ota].y-8)
        && positionenemy[enn].y < (vraiotage[ota].y+8) + vraiotage[ota].h)

              return (1);
    }
   else
          return (0);
}


void moving_enemy(enn)
{
    if (max_enemy[enn] == 1) // à changer - BALISE SPAWN
    {
        if (moving_token[enn] == 1)
        {
            if (cpu_type[enn] == 1) // Tank
            {
                if (rand_cote[enn] == 1)    // A droite
                {
                    positionenemy[enn].x = positionenemy[enn].x + tank_speed[enn];
                }
                if (rand_cote[enn] == 2)    // A gauche
                {
                    positionenemy[enn].x = positionenemy[enn].x - tank_speed[enn];
                }
            }


            if (cpu_type[enn] == 2) // Avion
            {
                if (rand_cote[enn] == 1)    // A droite
                {
                    if (positioncopbg1.y > positionenemy[enn].y && positionenemy[enn].y < 390)    // Descente de l'avion
                    {
                        positionenemy[enn].y = positionenemy[enn].y + 1;
                    }
                    if (positioncopbg1.y < positionenemy[enn].y)    // Montée de l'avion
                    {
                        positionenemy[enn].y = positionenemy[enn].y - 1;
                    }
                    positionenemy[enn].x = positionenemy[enn].x + 3;

                    if (positioncopbg1.x > positionenemy[enn].x)
                    {
                        if (etat_missile[enn] == 0)
                        {
                            missiles[enn] = IMG_Load("missiles-right-1.png");
                            positionmissiles[enn].x = positionenemy[enn].x;
                            positionmissiles[enn].y = positionenemy[enn].y;
                            SDL_BlitSurface(missiles[enn], NULL, background1, &positionmissiles[enn]);
                            SDL_Flip(ecran);
                            etat_missile[enn] = 1;  // Missile droite
                        }
                    }
                }
                if (rand_cote[enn] == 2)    // A gauche
                {
                    if (positioncopbg1.y > positionenemy[enn].y && positionenemy[enn].y < 390)    // Descente de l'avion
                    {
                        positionenemy[enn].y = positionenemy[enn].y + 1;
                    }
                    if (positioncopbg1.y < positionenemy[enn].y)    // Montée de l'avion
                    {
                        positionenemy[enn].y = positionenemy[enn].y - 1;
                    }
                    positionenemy[enn].x = positionenemy[enn].x - 3;

                    if (positioncopbg1.x < positionenemy[enn].x)
                    {
                        if (etat_missile[enn] == 0)
                        {
                            missiles[enn] = IMG_Load("missiles-left-1.png");
                            positionmissiles[enn].x = positionenemy[enn].x;
                            positionmissiles[enn].y = positionenemy[enn].y;
                            SDL_BlitSurface(missiles[enn], NULL, background1, &positionmissiles[enn]);
                            SDL_Flip(ecran);
                            etat_missile[enn] = 2;  // Missile gauche
                        }
                    }
                }
            }


            if (cpu_type[enn] == 3) // UFO
            {
                if (positioncopbg1.y > positionenemy[enn].y && positionenemy[enn].y < 390)    // Descente de l'UFO
                {
                    positionenemy[enn].y = positionenemy[enn].y + 2;
                }
                if (positioncopbg1.y < positionenemy[enn].y)    // Montée de l'UFO
                {
                    positionenemy[enn].y = positionenemy[enn].y - 2;
                }

                if (positioncopbg1.x > positionenemy[enn].x)    // Déplacement gauche de l'UFO
                {
                    positionenemy[enn].x = positionenemy[enn].x + 2;
                }
                if (positioncopbg1.x < positionenemy[enn].x)    // Déplacement droite de l'UFO
                {
                    positionenemy[enn].x = positionenemy[enn].x - 2;
                }
            }

            SDL_BlitSurface(cpu[enn],NULL, background1, &positionenemy[enn]);
            SDL_Flip(ecran);
        }
    }
}


void moving_missiles(enn)
{
    if (etat_missile[enn] == 1) // Déplacement vers la droite
    {
        positionmissiles[enn].x = positionmissiles[enn].x + 5 ;

        if (anime_missile[enn] == 1)
        {
            missiles[enn] = IMG_Load("Missiles/missiles-right-1.png");
        }
        if (anime_missile[enn] == 2)
        {
            missiles[enn] = IMG_Load("Missiles/missiles-right-2.png");
        }
        if (anime_missile[enn] == 3)
        {
            missiles[enn] = IMG_Load("Missiles/missiles-right-3.png");
            anime_missile[enn] = 1;
        }

        SDL_BlitSurface(missiles[enn], NULL, background1, &positionmissiles[enn]);
        SDL_Flip(ecran);
        anime_missile[enn]++;

        if (positionmissiles[enn].x >= 1875)
        {
            etat_missile[enn] = 3;  // Missile détruit
        }
    }

    if (etat_missile[enn] == 2) // Déplacement vers la gauche
    {
        positionmissiles[enn].x = positionmissiles[enn].x - 5 ;

        if (anime_missile[enn] == 1)
        {
            missiles[enn] = IMG_Load("Missiles/missiles-left-1.png");
        }
        if (anime_missile[enn] == 2)
        {
            missiles[enn] = IMG_Load("Missiles/missiles-left-2.png");
        }
        if (anime_missile[enn] == 3)
        {
            missiles[enn] = IMG_Load("Missiles/missiles-left-3.png");
            anime_missile[enn] = 1;
        }

        SDL_BlitSurface(missiles[enn], NULL, background1, &positionmissiles[enn]);
        SDL_Flip(ecran);
        anime_missile[enn]++;

        if (positionmissiles[enn].x <= 10)
        {
            etat_missile[enn] = 3;  // Missile détruit
        }
    }

    if (etat_missile[enn] == 3)
    {
        missiles[enn] = IMG_Load("disparition-ciel.png");
        SDL_BlitSurface(missiles[enn], NULL, background1, &positionmissiles[enn]);
        SDL_Flip(ecran);
        if (explosion_missile[enn] == 1)
        {
            missiles[enn] = IMG_Load("Explosions/explosion-1.png");
        }
        if (explosion_missile[enn] == 2)
        {
            missiles[enn] = IMG_Load("Explosions/explosion-2.png");
        }
        if (explosion_missile[enn] == 3)
        {
            missiles[enn] = IMG_Load("Explosions/explosion-3.png");
        }
        if (explosion_missile[enn] == 4)
        {
            missiles[enn] = IMG_Load("Explosions/explosion-4.png");
        }
        if (explosion_missile[enn] == 5)
        {
            missiles[enn] = IMG_Load("Explosions/explosion-5.png");
        }
        if (explosion_missile[enn] == 6)
        {
            missiles[enn] = IMG_Load("Explosions/explosion-6.png");
        }
        if (explosion_missile[enn] == 7)
        {
            missiles[enn] = IMG_Load("Explosions/explosion-7.png");
        }
        if (explosion_missile[enn] == 8)
        {
            missiles[enn] = IMG_Load("Explosions/explosion-8.png");
        }
        SDL_BlitSurface(missiles[enn], NULL, background1, &positionmissiles[enn]);
        SDL_Flip(ecran);
        explosion_missile[enn]++;

        if (explosion_missile[enn] == 9)
        {
            missiles[enn] = IMG_Load("disparition-ciel.png");
            SDL_BlitSurface(missiles[enn], NULL, background1, &positionmissiles[enn]);
            SDL_Flip(ecran);
            explosion_missile[enn] = 0;
            positionmissiles[enn].x = 3000;
            positionmissiles[enn].y = 1000;
        }
    }
}


void destruct_enemy(enn)
{
    FMOD_SYSTEM *system;
    FMOD_SOUND *destruct;

    FMOD_System_Create(&system);
    FMOD_System_Init(system, 10, FMOD_INIT_NORMAL, NULL);

    resultat = FMOD_System_CreateSound(system, "money.wav", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &destruct);


    if (collisionb(enn) == 1 || positionenemy[enn].x >= 1870 || positionenemy[enn].x < 6)
    {
        explosion_token[enn] = 1;
        moving_token[enn] = 0;
    }

    if (explosion_token[enn] == 1)
    {
        if (cpu_type[enn] == 1)
        {
            cpu[enn] = IMG_Load("disparition-sable.png");
            SDL_BlitSurface(cpu[enn], NULL, background1, &positionenemy[enn]);
            SDL_Flip(ecran);
            if (anime_explosion[enn] == 1)
            {
                cpu[enn] = IMG_Load("Explosions/explosion-1.png");
            }
            if (anime_explosion[enn] == 2)
            {
                cpu[enn] = IMG_Load("Explosions/explosion-2.png");
            }
            if (anime_explosion[enn] == 3)
            {
                cpu[enn] = IMG_Load("Explosions/explosion-3.png");
            }
            if (anime_explosion[enn] == 4)
            {
                cpu[enn] = IMG_Load("Explosions/explosion-4.png");
            }
            if (anime_explosion[enn] == 5)
            {
                cpu[enn] = IMG_Load("Explosions/explosion-5.png");
            }
            if (anime_explosion[enn] == 6)
            {
                cpu[enn] = IMG_Load("Explosions/explosion-6.png");
            }
            if (anime_explosion[enn] == 7)
            {
                cpu[enn] = IMG_Load("Explosions/explosion-7.png");
            }
            if (anime_explosion[enn] == 8)
            {
                cpu[enn] = IMG_Load("Explosions/explosion-8.png");
            }
            SDL_BlitSurface(cpu[enn], NULL, background1, &positionenemy[enn]);
            SDL_Flip(ecran);
            anime_explosion[enn]++;

            if (anime_explosion[enn] == 9)
            {
                cpu[enn] = IMG_Load("disparition-sable.png");
                SDL_BlitSurface(cpu[enn], NULL, background1, &positionenemy[enn]);
                SDL_Flip(ecran);
                explosion_token[enn] = 0;
                anime_explosion[enn] = 0;
                max_enemy[enn] = 0;
                cpu_type[enn] = 0;
                positionenemy[enn].x = 3000;
                positionenemy[enn].y = 1000;
            }
        }

        if (cpu_type[enn] == 2 || cpu_type[enn] == 3)
        {
            cpu[enn] = IMG_Load("disparition-ciel.png");
            SDL_BlitSurface(cpu[enn], NULL, background1, &positionenemy[enn]);
            SDL_Flip(ecran);
            if (cpu_type[enn] == 2)
            {
                if (anime_explosion[enn] == 1)
                {
                    cpu[enn] = IMG_Load("Explosions/explosion-1.png");
                }
                if (anime_explosion[enn] == 2)
                {
                    cpu[enn] = IMG_Load("Explosions/explosion-2.png");
                }
                if (anime_explosion[enn] == 3)
                {
                    cpu[enn] = IMG_Load("Explosions/explosion-3.png");
                }
                if (anime_explosion[enn] == 4)
                {
                    cpu[enn] = IMG_Load("Explosions/explosion-4.png");
                }
                if (anime_explosion[enn] == 5)
                {
                    cpu[enn] = IMG_Load("Explosions/explosion-5.png");
                }
                if (anime_explosion[enn] == 6)
                {
                    cpu[enn] = IMG_Load("Explosions/explosion-6.png");
                }
                if (anime_explosion[enn] == 7)
                {
                    cpu[enn] = IMG_Load("Explosions/explosion-7.png");
                }
                if (anime_explosion[enn] == 8)
                {
                    cpu[enn] = IMG_Load("Explosions/explosion-8.png");
                }
            }
            if (cpu_type[enn] == 3)
            {
                if (anime_explosion[enn] == 1)
                {
                    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, destruct, 0, NULL);
                    cpu[enn] = IMG_Load("Explosions/explosion-ufo-1.png");
                }
                if (anime_explosion[enn] == 2)
                {
                    cpu[enn] = IMG_Load("Explosions/explosion-ufo-2.png");
                }
                if (anime_explosion[enn] == 3)
                {
                    cpu[enn] = IMG_Load("Explosions/explosion-ufo-3.png");
                }
                if (anime_explosion[enn] == 4)
                {
                    cpu[enn] = IMG_Load("Explosions/explosion-ufo-4.png");
                }
                if (anime_explosion[enn] == 5)
                {
                    cpu[enn] = IMG_Load("Explosions/explosion-ufo-5.png");
                }
                if (anime_explosion[enn] == 6)
                {
                    cpu[enn] = IMG_Load("Explosions/explosion-ufo-6.png");
                }
                if (anime_explosion[enn] == 7)
                {
                    cpu[enn] = IMG_Load("Explosions/explosion-ufo-7.png");
                }
                if (anime_explosion[enn] == 8)
                {
                    cpu[enn] = IMG_Load("Explosions/explosion-ufo-8.png");
                }
            }
            SDL_BlitSurface(cpu[enn], NULL, background1, &positionenemy[enn]);
            SDL_Flip(ecran);
            anime_explosion[enn]++;

            if (anime_explosion[enn] == 9)
            {
                cpu[enn] = IMG_Load("disparition-ciel.png");
                SDL_BlitSurface(cpu[enn], NULL, background1, &positionenemy[enn]);
                SDL_Flip(ecran);
                explosion_token[enn] = 0;
                anime_explosion[enn] = 0;
                max_enemy[enn] = 0;
                cpu_type[enn] = 0;
                positionenemy[enn].x = 3000;
                positionenemy[enn].y = 1000;
            }
        }
    }
}



void gameover(void)
{
	SDL_Surface *gameoverd = NULL;
	SDL_Rect positiongo;

	FMOD_Sound_Release(musiqueJeu);

	gameoverd = IMG_Load("gameover.png");
	positiongo.x = 0;
	positiongo.y = 0;

	SDL_BlitSurface(gameoverd, NULL, ecran, &positiongo);
	SDL_Flip(ecran);
	 while (1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
					case SDLK_RETURN:
                        SDL_Delay(3000);
						SDL_EnableKeyRepeat(10, 1000);
				}
		}
	 }

}


void otage(ota)
{
		positionotage[ota].x = fauxbunker[ota].x;
		positionotage[ota].y = 464;
}


void deplacement_otage(ota)
{
	vraiotage[ota].x = (positionbg1.x - positionotage[ota].x) * -1;
    vraiotage[ota].y = (positionbg1.y - positionotage[ota].y) * -1;

   /* printf("depl_otage => %d \n\n", depl_otage);
    printf("position_otage => %d \n\n",positionotage.x);
    printf("Copter a portee => %d \n\n", copter_portee());
    printf("Portee => %d \n\n", positionotage.x - positioncopbg1.x);*/
	/*printf("\nBunker %d\t\t%d\n",fauxbunker.x,fauxbunker.y);*/
    if (bunker_state[ota] == 0)
    {
        if (copter_portee(ota) == 0) // IF copter pas à portée
        {
            if (depl_otage[ota] == 1)    // Déplacement à droite
            {
                if(positionotage[ota].x >= otage_tampux[ota]+30)
                {
                    depl_otage[ota] = 2;
					hostage[ota] = IMG_Load("otage-left.png");
                }
                positionotage[ota].x = positionotage[ota].x ++;
            }

            if (depl_otage[ota] == 2)    // Déplacement à gauche
            {
                if(positionotage[ota].x <= otage_tampux[ota]-30)
                {
                    depl_otage[ota] = 1;
					hostage[ota] = IMG_Load("otage-right.png");
                }
                positionotage[ota].x = positionotage[ota].x --;
            }
        }

        if (copter_portee(ota) == 1 || copter_portee(ota) == 2 || copter_portee(ota) == 3)  // IF copter à portée
        {
            otage_tampux[ota] = positionotage[ota].x;

            if (copter_portee(ota) == 1)   // Déplacement à droite de l'otage
            {
                hostage[ota] = IMG_Load("otage-left.png");
                positionotage[ota].x = positionotage[ota].x - 2;
            }

            if (copter_portee(ota) == 2)   // Déplacement à gauche de l'otage
            {
                hostage[ota] = IMG_Load("otage-right.png");
                positionotage[ota].x = positionotage[ota].x + 2;
            }

            if (copter_portee(ota) == 3)   // Arrêt de l'otage
            {
                positionotage[ota].x = positionotage[ota].x;
            }
        }
    }
}


int recept_otage(ota)
{
	if (chopper_state == 4) // CHANGEMENT ICI
	{
        if (collisiono(ota) == 1)
        {
            if (provijuif < 16)
            {
                provijuif ++;
                printf("Juif : %d\n", provijuif);
                valeur_otage[ota]--;
                return (1);
            }
        }
	}
	else
	{
		return (0);
	}
}


int copter_portee(enn)     // copter à portée
{
    if (chopper_state == 4)  // IF copter posé
    {
        if (vraiotage[enn].x - positioncopbg1.x >= -175 && vraiotage[enn].x - positioncopbg1.x < -4)   // IF copter à portée sur les x à gauche
        {
            return (2);
        }
        if (vraiotage[enn].x - positioncopbg1.x <= 175 && vraiotage[enn].x - positioncopbg1.x > 4)   // IF copter à portée sur les x à droite
        {
            return (1);
        }
        if (vraiotage[enn].x - positioncopbg1.x <= 4 && vraiotage[enn].x - positioncopbg1.x >= -4)   // IF copter à portée sur les x au même-endroit
        {
            return (3);
        }

        else
        {
            return (0);
        }
    }
    else
    {
        return (0);
    }
}



void collage_score(provijuif)
{
    if (provijuif == 0)
    {
        score1 = IMG_Load("Scoring/0.png");
        score2 = IMG_Load("Scoring/0.png");
    }
    if (provijuif == 1)
    {
        score1 = IMG_Load("Scoring/0.png");
        score2 = IMG_Load("Scoring/1.png");
    }
    if (provijuif == 2)
    {
        score1 = IMG_Load("Scoring/0.png");
        score2 = IMG_Load("Scoring/2.png");
    }
    if (provijuif == 3)
    {
        score1 = IMG_Load("Scoring/0.png");
        score2 = IMG_Load("Scoring/3.png");
    }
    if (provijuif == 4)
    {
        score1 = IMG_Load("Scoring/0.png");
        score2 = IMG_Load("Scoring/4.png");
    }
    if (provijuif == 5)
    {
        score1 = IMG_Load("Scoring/0.png");
        score2 = IMG_Load("Scoring/5.png");
    }
    if (provijuif == 6)
    {
        score1 = IMG_Load("Scoring/0.png");
        score2 = IMG_Load("Scoring/6.png");
    }
    if (provijuif == 7)
    {
        score1 = IMG_Load("Scoring/0.png");
        score2 = IMG_Load("Scoring/7.png");
    }
    if (provijuif == 8)
    {
        score1 = IMG_Load("Scoring/0.png");
        score2 = IMG_Load("Scoring/8.png");
    }
    if (provijuif == 9)
    {
        score1 = IMG_Load("Scoring/0.png");
        score2 = IMG_Load("Scoring/9.png");
    }
    if (provijuif == 10)
    {
        score1 = IMG_Load("Scoring/1.png");
        score2 = IMG_Load("Scoring/0.png");
    }
    if (provijuif == 11)
    {
        score1 = IMG_Load("Scoring/1.png");
        score2 = IMG_Load("Scoring/1.png");
    }
    if (provijuif == 12)
    {
        score1 = IMG_Load("Scoring/1.png");
        score2 = IMG_Load("Scoring/2.png");
    }
    if (provijuif == 13)
    {
        score1 = IMG_Load("Scoring/1.png");
        score2 = IMG_Load("Scoring/3.png");
    }
    if (provijuif == 14)
    {
        score1 = IMG_Load("Scoring/1.png");
        score2 = IMG_Load("Scoring/4.png");
    }
    if (provijuif == 15)
    {
        score1 = IMG_Load("Scoring/1.png");
        score2 = IMG_Load("Scoring/5.png");
    }
    if (provijuif >= 16)
    {
        score1 = IMG_Load("Scoring/1.png");
        score2 = IMG_Load("Scoring/6.png");
    }
}


void depose_otage()
{
	if ((chopper_state == 4) && (positioncopbg1.x >= 2000) && (positioncopbg1.x <= 2284))
	{
		if (provijuif > 0)
		{
			if (depo == 0)
			{

					depo = 1;
					provijuif --;
					otage_depose ++;
					otage_golden --;
					printf("NB OTAGE DEPOSE : %d\n\n\n",otage_depose);
			}
			else
			{
				depoc ++;
				if (depoc == 25)
				{
					depo = 0;
					depoc = 0;
				}
			}
		}


	}
}


void winner(void)
{
	SDL_Surface *win;
	SDL_Rect positionwin;

	win = IMG_Load("win.jpg");
	positionwin.x = 0;
	positionwin.y = 0;

	SDL_BlitSurface(win, NULL, ecran, &positionwin);
	SDL_Flip(ecran);
	 while (1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
					case SDLK_RETURN:
                        SDL_Delay(3000);
                        SDL_EnableKeyRepeat(10, 1000);
				}
		}
	 }

}


void alive_score(provijuif)
{
    if (provijuif == 0)
    {
        alive1 = IMG_Load("Scoring-vert/0.png");
        alive2 = IMG_Load("Scoring-vert/0.png");
    }
    if (provijuif == 1)
    {
        alive1 = IMG_Load("Scoring-vert/0.png");
        alive2 = IMG_Load("Scoring-vert/1.png");
    }
    if (provijuif == 2)
    {
        alive1 = IMG_Load("Scoring-vert/0.png");
        alive2 = IMG_Load("Scoring-vert/2.png");
    }
    if (provijuif == 3)
    {
        alive1 = IMG_Load("Scoring-vert/0.png");
        alive2 = IMG_Load("Scoring-vert/3.png");
    }
    if (provijuif == 4)
    {
        alive1 = IMG_Load("Scoring-vert/0.png");
        alive2 = IMG_Load("Scoring-vert/4.png");
    }
    if (provijuif == 5)
    {
        alive1 = IMG_Load("Scoring-vert/0.png");
        alive2 = IMG_Load("Scoring-vert/5.png");
    }
    if (provijuif == 6)
    {
        alive1 = IMG_Load("Scoring-vert/0.png");
        alive2 = IMG_Load("Scoring-vert/6.png");
    }
    if (provijuif == 7)
    {
        alive1 = IMG_Load("Scoring-vert/0.png");
        alive2 = IMG_Load("Scoring-vert/7.png");
    }
    if (provijuif == 8)
    {
        alive1 = IMG_Load("Scoring-vert/0.png");
        alive2 = IMG_Load("Scoring-vert/8.png");
    }
    if (provijuif == 9)
    {
        alive1 = IMG_Load("Scoring-vert/0.png");
        alive2 = IMG_Load("Scoring-vert/9.png");
    }
    if (provijuif == 10)
    {
        alive1 = IMG_Load("Scoring-vert/1.png");
        alive2 = IMG_Load("Scoring-vert/0.png");
    }
    if (provijuif == 11)
    {
        alive1 = IMG_Load("Scoring-vert/1.png");
        alive2 = IMG_Load("Scoring-vert/1.png");
    }
    if (provijuif == 12)
    {
        alive1 = IMG_Load("Scoring-vert/1.png");
        alive2 = IMG_Load("Scoring-vert/2.png");
    }
    if (provijuif == 13)
    {
        alive1 = IMG_Load("Scoring-vert/1.png");
        alive2 = IMG_Load("Scoring-vert/3.png");
    }
    if (provijuif == 14)
    {
        alive1 = IMG_Load("Scoring-vert/1.png");
        alive2 = IMG_Load("Scoring-vert/4.png");
    }
    if (provijuif == 15)
    {
        alive1 = IMG_Load("Scoring-vert/1.png");
        alive2 = IMG_Load("Scoring-vert/5.png");
    }
    if (provijuif == 16)
    {
        alive1 = IMG_Load("Scoring-vert/1.png");
        alive2 = IMG_Load("Scoring-vert/6.png");
    }
    if (provijuif == 17)
    {
        alive1 = IMG_Load("Scoring-vert/1.png");
        alive2 = IMG_Load("Scoring-vert/7.png");
    }
    if (provijuif == 18)
    {
        alive1 = IMG_Load("Scoring-vert/1.png");
        alive2 = IMG_Load("Scoring-vert/8.png");
    }
    if (provijuif == 19)
    {
        alive1 = IMG_Load("Scoring-vert/1.png");
        alive2 = IMG_Load("Scoring-vert/9.png");
    }
    if (provijuif == 20)
    {
        alive1 = IMG_Load("Scoring-vert/2.png");
        alive2 = IMG_Load("Scoring-vert/0.png");
    }
    if (provijuif == 21)
    {
        alive1 = IMG_Load("Scoring-vert/2.png");
        alive2 = IMG_Load("Scoring-vert/1.png");
    }
    if (provijuif == 22)
    {
        alive1 = IMG_Load("Scoring-vert/2.png");
        alive2 = IMG_Load("Scoring-vert/2.png");
    }
    if (provijuif == 23)
    {
        alive1 = IMG_Load("Scoring-vert/2.png");
        alive2 = IMG_Load("Scoring-vert/3.png");
    }
    if (provijuif == 24)
    {
        alive1 = IMG_Load("Scoring-vert/2.png");
        alive2 = IMG_Load("Scoring-vert/4.png");
    }
    if (provijuif == 25)
    {
        alive1 = IMG_Load("Scoring-vert/2.png");
        alive2 = IMG_Load("Scoring-vert/5.png");
    }
    if (provijuif == 26)
    {
        alive1 = IMG_Load("Scoring-vert/2.png");
        alive2 = IMG_Load("Scoring-vert/6.png");
    }
    if (provijuif == 27)
    {
        alive1 = IMG_Load("Scoring-vert/2.png");
        alive2 = IMG_Load("Scoring-vert/7.png");
    }
    if (provijuif == 28)
    {
        alive1 = IMG_Load("Scoring-vert/2.png");
        alive2 = IMG_Load("Scoring-vert/8.png");
    }
    if (provijuif == 29)
    {
        alive1 = IMG_Load("Scoring-vert/2.png");
        alive2 = IMG_Load("Scoring-vert/9.png");
    }
    if (provijuif == 30)
    {
        alive1 = IMG_Load("Scoring-vert/3.png");
        alive2 = IMG_Load("Scoring-vert/0.png");
    }
    if (provijuif == 31)
    {
        alive1 = IMG_Load("Scoring-vert/3.png");
        alive2 = IMG_Load("Scoring-vert/1.png");
    }
    if (provijuif == 32)
    {
        alive1 = IMG_Load("Scoring-vert/3.png");
        alive2 = IMG_Load("Scoring-vert/2.png");
    }
    if (provijuif == 33)
    {
        alive1 = IMG_Load("Scoring-vert/3.png");
        alive2 = IMG_Load("Scoring-vert/3.png");
    }
    if (provijuif == 34)
    {
        alive1 = IMG_Load("Scoring-vert/3.png");
        alive2 = IMG_Load("Scoring-vert/4.png");
    }
    if (provijuif == 35)
    {
        alive1 = IMG_Load("Scoring-vert/3.png");
        alive2 = IMG_Load("Scoring-vert/5.png");
    }
    if (provijuif == 36)
    {
        alive1 = IMG_Load("Scoring-vert/3.png");
        alive2 = IMG_Load("Scoring-vert/6.png");
    }
    if (provijuif == 37)
    {
        alive1 = IMG_Load("Scoring-vert/3.png");
        alive2 = IMG_Load("Scoring-vert/7.png");
    }
    if (provijuif == 38)
    {
        alive1 = IMG_Load("Scoring-vert/3.png");
        alive2 = IMG_Load("Scoring-vert/8.png");
    }
    if (provijuif == 39)
    {
        alive1 = IMG_Load("Scoring-vert/3.png");
        alive2 = IMG_Load("Scoring-vert/9.png");
    }
    if (provijuif == 40)
    {
        alive1 = IMG_Load("Scoring-vert/4.png");
        alive2 = IMG_Load("Scoring-vert/0.png");
    }
    if (provijuif == 41)
    {
        alive1 = IMG_Load("Scoring-vert/4.png");
        alive2 = IMG_Load("Scoring-vert/1.png");
    }
    if (provijuif == 42)
    {
        alive1 = IMG_Load("Scoring-vert/4.png");
        alive2 = IMG_Load("Scoring-vert/2.png");
    }
    if (provijuif == 43)
    {
        alive1 = IMG_Load("Scoring-vert/4.png");
        alive2 = IMG_Load("Scoring-vert/3.png");
    }
    if (provijuif == 44)
    {
        alive1 = IMG_Load("Scoring-vert/4.png");
        alive2 = IMG_Load("Scoring-vert/4.png");
    }
    if (provijuif == 45)
    {
        alive1 = IMG_Load("Scoring-vert/4.png");
        alive2 = IMG_Load("Scoring-vert/5.png");
    }
    if (provijuif == 46)
    {
        alive1 = IMG_Load("Scoring-vert/4.png");
        alive2 = IMG_Load("Scoring-vert/6.png");
    }
    if (provijuif == 47)
    {
        alive1 = IMG_Load("Scoring-vert/4.png");
        alive2 = IMG_Load("Scoring-vert/7.png");
    }
    if (provijuif == 48)
    {
        alive1 = IMG_Load("Scoring-vert/4.png");
        alive2 = IMG_Load("Scoring-vert/8.png");
    }
    if (provijuif == 49)
    {
        alive1 = IMG_Load("Scoring-vert/4.png");
        alive2 = IMG_Load("Scoring-vert/9.png");
    }
    if (provijuif == 50)
    {
        alive1 = IMG_Load("Scoring-vert/5.png");
        alive2 = IMG_Load("Scoring-vert/0.png");
    }
    if (provijuif == 51)
    {
        alive1 = IMG_Load("Scoring-vert/5.png");
        alive2 = IMG_Load("Scoring-vert/1.png");
    }
    if (provijuif == 52)
    {
        alive1 = IMG_Load("Scoring-vert/5.png");
        alive2 = IMG_Load("Scoring-vert/2.png");
    }
    if (provijuif == 53)
    {
        alive1 = IMG_Load("Scoring-vert/5.png");
        alive2 = IMG_Load("Scoring-vert/3.png");
    }
    if (provijuif == 54)
    {
        alive1 = IMG_Load("Scoring-vert/5.png");
        alive2 = IMG_Load("Scoring-vert/4.png");
    }
    if (provijuif == 55)
    {
        alive1 = IMG_Load("Scoring-vert/5.png");
        alive2 = IMG_Load("Scoring-vert/5.png");
    }
    if (provijuif == 56)
    {
        alive1 = IMG_Load("Scoring-vert/5.png");
        alive2 = IMG_Load("Scoring-vert/6.png");
    }
    if (provijuif == 57)
    {
        alive1 = IMG_Load("Scoring-vert/5.png");
        alive2 = IMG_Load("Scoring-vert/7.png");
    }
    if (provijuif == 58)
    {
        alive1 = IMG_Load("Scoring-vert/5.png");
        alive2 = IMG_Load("Scoring-vert/8.png");
    }
    if (provijuif == 59)
    {
        alive1 = IMG_Load("Scoring-vert/5.png");
        alive2 = IMG_Load("Scoring-vert/9.png");
    }
    if (provijuif == 60)
    {
        alive1 = IMG_Load("Scoring-vert/6.png");
        alive2 = IMG_Load("Scoring-vert/0.png");
    }
    if (provijuif == 61)
    {
        alive1 = IMG_Load("Scoring-vert/6.png");
        alive2 = IMG_Load("Scoring-vert/1.png");
    }
    if (provijuif == 62)
    {
        alive1 = IMG_Load("Scoring-vert/6.png");
        alive2 = IMG_Load("Scoring-vert/2.png");
    }
    if (provijuif == 63)
    {
        alive1 = IMG_Load("Scoring-vert/6.png");
        alive2 = IMG_Load("Scoring-vert/3.png");
    }
    if (provijuif == 64)
    {
        alive1 = IMG_Load("Scoring-vert/6.png");
        alive2 = IMG_Load("Scoring-vert/4.png");
    }
}


void danger_score(provijuif)
{
    if (provijuif == 0)
    {
        danger1 = IMG_Load("Scoring-rouge/0.png");
        danger2 = IMG_Load("Scoring-rouge/0.png");
    }
    if (provijuif == 1)
    {
        danger1 = IMG_Load("Scoring-rouge/0.png");
        danger2 = IMG_Load("Scoring-rouge/1.png");
    }
    if (provijuif == 2)
    {
        danger1 = IMG_Load("Scoring-rouge/0.png");
        danger2 = IMG_Load("Scoring-rouge/2.png");
    }
    if (provijuif == 3)
    {
        danger1 = IMG_Load("Scoring-rouge/0.png");
        danger2 = IMG_Load("Scoring-rouge/3.png");
    }
    if (provijuif == 4)
    {
        danger1 = IMG_Load("Scoring-rouge/0.png");
        danger2 = IMG_Load("Scoring-rouge/4.png");
    }
    if (provijuif == 5)
    {
        danger1 = IMG_Load("Scoring-rouge/0.png");
        danger2 = IMG_Load("Scoring-rouge/5.png");
    }
    if (provijuif == 6)
    {
        danger1 = IMG_Load("Scoring-rouge/0.png");
        danger2 = IMG_Load("Scoring-rouge/6.png");
    }
    if (provijuif == 7)
    {
        danger1 = IMG_Load("Scoring-rouge/0.png");
        danger2 = IMG_Load("Scoring-rouge/7.png");
    }
    if (provijuif == 8)
    {
        danger1 = IMG_Load("Scoring-rouge/0.png");
        danger2 = IMG_Load("Scoring-rouge/8.png");
    }
    if (provijuif == 9)
    {
        danger1 = IMG_Load("Scoring-rouge/0.png");
        danger2 = IMG_Load("Scoring-rouge/9.png");
    }
    if (provijuif == 10)
    {
        danger1 = IMG_Load("Scoring-rouge/1.png");
        danger2 = IMG_Load("Scoring-rouge/0.png");
    }
    if (provijuif == 11)
    {
        danger1 = IMG_Load("Scoring-rouge/1.png");
        danger2 = IMG_Load("Scoring-rouge/1.png");
    }
    if (provijuif == 12)
    {
        danger1 = IMG_Load("Scoring-rouge/1.png");
        danger2 = IMG_Load("Scoring-rouge/2.png");
    }
    if (provijuif == 13)
    {
        danger1 = IMG_Load("Scoring-rouge/1.png");
        danger2 = IMG_Load("Scoring-rouge/3.png");
    }
    if (provijuif == 14)
    {
        danger1 = IMG_Load("Scoring-rouge/1.png");
        danger2 = IMG_Load("Scoring-rouge/4.png");
    }
    if (provijuif == 15)
    {
        danger1 = IMG_Load("Scoring-rouge/1.png");
        danger2 = IMG_Load("Scoring-rouge/5.png");
    }
    if (provijuif == 16)
    {
        danger1 = IMG_Load("Scoring-rouge/1.png");
        danger2 = IMG_Load("Scoring-rouge/6.png");
    }
    if (provijuif == 17)
    {
        danger1 = IMG_Load("Scoring-rouge/1.png");
        danger2 = IMG_Load("Scoring-rouge/7.png");
    }
    if (provijuif == 18)
    {
        danger1 = IMG_Load("Scoring-rouge/1.png");
        danger2 = IMG_Load("Scoring-rouge/8.png");
    }
    if (provijuif == 19)
    {
        danger1 = IMG_Load("Scoring-rouge/1.png");
        danger2 = IMG_Load("Scoring-rouge/9.png");
    }
    if (provijuif == 20)
    {
        danger1 = IMG_Load("Scoring-rouge/2.png");
        danger2 = IMG_Load("Scoring-rouge/0.png");
    }
    if (provijuif == 21)
    {
        danger1 = IMG_Load("Scoring-rouge/2.png");
        danger2 = IMG_Load("Scoring-rouge/1.png");
    }
    if (provijuif == 22)
    {
        danger1 = IMG_Load("Scoring-rouge/2.png");
        danger2 = IMG_Load("Scoring-rouge/2.png");
    }
    if (provijuif == 23)
    {
        danger1 = IMG_Load("Scoring-rouge/2.png");
        danger2 = IMG_Load("Scoring-rouge/3.png");
    }
    if (provijuif == 24)
    {
        danger1 = IMG_Load("Scoring-rouge/2.png");
        danger2 = IMG_Load("Scoring-rouge/4.png");
    }
    if (provijuif == 25)
    {
        danger1 = IMG_Load("Scoring-rouge/2.png");
        danger2 = IMG_Load("Scoring-rouge/5.png");
    }
    if (provijuif == 26)
    {
        danger1 = IMG_Load("Scoring-rouge/2.png");
        danger2 = IMG_Load("Scoring-rouge/6.png");
    }
    if (provijuif == 27)
    {
        danger1 = IMG_Load("Scoring-rouge/2.png");
        danger2 = IMG_Load("Scoring-rouge/7.png");
    }
    if (provijuif == 28)
    {
        danger1 = IMG_Load("Scoring-rouge/2.png");
        danger2 = IMG_Load("Scoring-rouge/8.png");
    }
    if (provijuif == 29)
    {
        danger1 = IMG_Load("Scoring-rouge/2.png");
        danger2 = IMG_Load("Scoring-rouge/9.png");
    }
    if (provijuif == 30)
    {
        danger1 = IMG_Load("Scoring-rouge/3.png");
        danger2 = IMG_Load("Scoring-rouge/0.png");
    }
    if (provijuif == 31)
    {
        danger1 = IMG_Load("Scoring-rouge/3.png");
        danger2 = IMG_Load("Scoring-rouge/1.png");
    }
    if (provijuif == 32)
    {
        danger1 = IMG_Load("Scoring-rouge/3.png");
        danger2 = IMG_Load("Scoring-rouge/2.png");
    }
    if (provijuif == 33)
    {
        danger1 = IMG_Load("Scoring-rouge/3.png");
        danger2 = IMG_Load("Scoring-rouge/3.png");
    }
    if (provijuif == 34)
    {
        danger1 = IMG_Load("Scoring-rouge/3.png");
        danger2 = IMG_Load("Scoring-rouge/4.png");
    }
    if (provijuif == 35)
    {
        danger1 = IMG_Load("Scoring-rouge/3.png");
        danger2 = IMG_Load("Scoring-rouge/5.png");
    }
    if (provijuif == 36)
    {
        danger1 = IMG_Load("Scoring-rouge/3.png");
        danger2 = IMG_Load("Scoring-rouge/6.png");
    }
    if (provijuif == 37)
    {
        danger1 = IMG_Load("Scoring-rouge/3.png");
        danger2 = IMG_Load("Scoring-rouge/7.png");
    }
    if (provijuif == 38)
    {
        danger1 = IMG_Load("Scoring-rouge/3.png");
        danger2 = IMG_Load("Scoring-rouge/8.png");
    }
    if (provijuif == 39)
    {
        danger1 = IMG_Load("Scoring-rouge/3.png");
        danger2 = IMG_Load("Scoring-rouge/9.png");
    }
    if (provijuif == 40)
    {
        danger1 = IMG_Load("Scoring-rouge/4.png");
        danger2 = IMG_Load("Scoring-rouge/0.png");
    }
    if (provijuif == 41)
    {
        danger1 = IMG_Load("Scoring-rouge/4.png");
        danger2 = IMG_Load("Scoring-rouge/1.png");
    }
    if (provijuif == 42)
    {
        danger1 = IMG_Load("Scoring-rouge/4.png");
        danger2 = IMG_Load("Scoring-rouge/2.png");
    }
    if (provijuif == 43)
    {
        danger1 = IMG_Load("Scoring-rouge/4.png");
        danger2 = IMG_Load("Scoring-rouge/3.png");
    }
    if (provijuif == 44)
    {
        danger1 = IMG_Load("Scoring-rouge/4.png");
        danger2 = IMG_Load("Scoring-rouge/4.png");
    }
    if (provijuif == 45)
    {
        danger1 = IMG_Load("Scoring-rouge/4.png");
        danger2 = IMG_Load("Scoring-rouge/5.png");
    }
    if (provijuif == 46)
    {
        danger1 = IMG_Load("Scoring-rouge/4.png");
        danger2 = IMG_Load("Scoring-rouge/6.png");
    }
    if (provijuif == 47)
    {
        danger1 = IMG_Load("Scoring-rouge/4.png");
        danger2 = IMG_Load("Scoring-rouge/7.png");
    }
    if (provijuif == 48)
    {
        danger1 = IMG_Load("Scoring-rouge/4.png");
        danger2 = IMG_Load("Scoring-rouge/8.png");
    }
    if (provijuif == 49)
    {
        danger1 = IMG_Load("Scoring-rouge/4.png");
        danger2 = IMG_Load("Scoring-rouge/9.png");
    }
    if (provijuif == 50)
    {
        danger1 = IMG_Load("Scoring-rouge/5.png");
        danger2 = IMG_Load("Scoring-rouge/0.png");
    }
    if (provijuif == 51)
    {
        danger1 = IMG_Load("Scoring-rouge/5.png");
        danger2 = IMG_Load("Scoring-rouge/1.png");
    }
    if (provijuif == 52)
    {
        danger1 = IMG_Load("Scoring-rouge/5.png");
        danger2 = IMG_Load("Scoring-rouge/2.png");
    }
    if (provijuif == 53)
    {
        danger1 = IMG_Load("Scoring-rouge/5.png");
        danger2 = IMG_Load("Scoring-rouge/3.png");
    }
    if (provijuif == 54)
    {
        danger1 = IMG_Load("Scoring-rouge/5.png");
        danger2 = IMG_Load("Scoring-rouge/4.png");
    }
    if (provijuif == 55)
    {
        danger1 = IMG_Load("Scoring-rouge/5.png");
        danger2 = IMG_Load("Scoring-rouge/5.png");
    }
    if (provijuif == 56)
    {
        danger1 = IMG_Load("Scoring-rouge/5.png");
        danger2 = IMG_Load("Scoring-rouge/6.png");
    }
    if (provijuif == 57)
    {
        danger1 = IMG_Load("Scoring-rouge/5.png");
        danger2 = IMG_Load("Scoring-rouge/7.png");
    }
    if (provijuif == 58)
    {
        danger1 = IMG_Load("Scoring-rouge/5.png");
        danger2 = IMG_Load("Scoring-rouge/8.png");
    }
    if (provijuif == 59)
    {
        danger1 = IMG_Load("Scoring-rouge/5.png");
        danger2 = IMG_Load("Scoring-rouge/9.png");
    }
    if (provijuif == 60)
    {
        danger1 = IMG_Load("Scoring-rouge/6.png");
        danger2 = IMG_Load("Scoring-rouge/0.png");
    }
    if (provijuif == 61)
    {
        danger1 = IMG_Load("Scoring-rouge/6.png");
        danger2 = IMG_Load("Scoring-rouge/1.png");
    }
    if (provijuif == 62)
    {
        danger1 = IMG_Load("Scoring-rouge/6.png");
        danger2 = IMG_Load("Scoring-rouge/2.png");
    }
    if (provijuif == 63)
    {
        danger1 = IMG_Load("Scoring-rouge/6.png");
        danger2 = IMG_Load("Scoring-rouge/3.png");
    }
    if (provijuif == 64)
    {
        danger1 = IMG_Load("Scoring-rouge/6.png");
        danger2 = IMG_Load("Scoring-rouge/4.png");
    }
}



void credits()
{
	SDL_Surface *creditus = NULL;
	SDL_Rect positioncredits;

    creditus = IMG_Load("Rocky.png");

	positioncredits.x = 0;
	positioncredits.y = 0;

	SDL_BlitSurface(creditus, NULL, ecran, &positioncredits);
    SDL_Flip(ecran); // Maj de l'écran

	 while (1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
					case SDLK_RETURN:
                    FMOD_Sound_Release(creditos);
                    init();
                 }
        }
    }

}
