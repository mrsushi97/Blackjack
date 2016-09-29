#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "functions.h"
#include "grafica.h"

/**
 * main function: entry point of the program
 * only to invoke other functions !
 */
int main( int argc, char* argv[] )
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *serif = NULL;
    SDL_Surface *cards[MAX_DECK_SIZE+1], *imgs[2];
    SDL_Event event;
    int delay = 300;
    int quit = 0;
    int atraso = 1000;

    int nbaralhos = 1, njogadores = 1, baralhosemjogo=0, sair= 0, i =0, value=0;
    int do_hit = 0, do_stand = 0, do_surrender = 0, do_double = 0,numcarta=0;
    int check_hit = 0, check_player = 0, check_house =0, check_house_play = 0;

    node1 *player, *head_player = NULL;
    node2 *card, *head_card = NULL;
    table house;
    char ** matrix;

    srand(time(NULL));

    //read the parameters
    head_player = lerparams(head_player, &njogadores, &nbaralhos,argv[1]);
    baralhosemjogo=nbaralhos;

    // initialize everything
    InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, &serif, imgs, &window, &renderer);
    LoadCards(cards);
    house.money = 0;

    //fill the deck, shuffle, and point the top of the deck to the head
    head_card = ShuffleDeck(nbaralhos);
    SDL_Log("Filled and shuffled");
    card = head_card;

    //EA parameters
    matrix = malloc(17*sizeof(char*));
    for(i=0;i<17;i++)
        matrix[i]=malloc(10*sizeof(char));

    lerdocEA(matrix);

    while(quit == 0)
    {
        sair=0;

        //The start of the hand distribution
        player = head_player;
        while(player != NULL)
        {
            if(player->active == true)
            {
                 if(player->money <= 0)
                {
                    player->active = false;
                    player = player->next;
                    continue;
                }

                check_player = player_distribuition(player,&card,&numcarta,&value);
                if(check_player == 0)
                {
                    free_deck(head_card);
                    head_card = ShuffleDeck(nbaralhos);
                    SDL_Log("Filled and shuffled");
                    if(numcarta>52)
                    {
                        baralhosemjogo--;
                        numcarta=0;
                    }
                    if(baralhosemjogo==0)
                        baralhosemjogo=nbaralhos;
                    card = head_card;
                    check_player = player_distribuition(player, &card,&numcarta,&value);
                    check_player = 0;
                }
            }
            else
            {
                //if a valid player was created
                if(player->money >= 0 && player->money >= player->bet)
                {
                    player->active = true;
                    continue;
                }
            }
            player = player->next;
        }


        check_house = house_distribuition(&house,&card,&numcarta, &value);
        //in case there is no more cards in the deck
        if(check_house == 0)
        {
            free_deck(head_card);
            head_card = ShuffleDeck(nbaralhos);
            SDL_Log("Filled and shuffled");
            if(numcarta>52)
            {
                baralhosemjogo--;
                numcarta=0;
            }
            if(baralhosemjogo==0)
                baralhosemjogo=nbaralhos;
            card = head_card;
            check_house = house_distribuition(&house, &card, &numcarta, &value);
            check_house = 0;
        }

        player = head_player;
        while( quit == 0 && player != NULL)
        {
             // render all image
            RenderTable(head_player, player, serif, imgs, renderer);
            RenderHouseCards(house.head, house.pos_house_hand, cards, renderer);
            RenderPlayerCards(head_player, cards, renderer);
            RenderBB(head_player , serif, renderer);
            SDL_RenderPresent(renderer);
            SDL_Delay( delay );

            // if the player has blackjack or is inactive, passes
            if(player->blackjack == 1 || player->active == false)
            {
                player = player->next;
                continue;
            }

            //EA playing time
            if(player->human == 0)
            {
                //The EA player is "thinking"
                playEA(matrix,player,house,head_card,nbaralhos,&card,&do_hit,&do_stand,&do_surrender,&do_double, &numcarta);
                SDL_Delay(atraso);
            }

            //human playing time
            //only humans do events
            while( SDL_PollEvent( &event ) && player->active == true)
            {
                if( event.type == SDL_QUIT )
                {
                    quit = 1;
                    break;
                }
                else if ( event.type == SDL_KEYDOWN && player->human == 1)
                {
                    switch ( event.key.keysym.sym )
                    {
                        case SDLK_UP:
                            atraso += 500;
                            SDL_Log("Delay increased by 0.5s");
                            break;
                        case SDLK_DOWN:
                            if(atraso > 500)
                            {
                                atraso -= 500;
                                SDL_Log("Delay decreased by 0.5s");
                            }
                            break;
                        case SDLK_d:
                            //double
                            do_double = 1;
                            break;
                        case SDLK_r:
                            //surrender
                            do_surrender = 1;
                            break;
                        case SDLK_s:
                            // stand !
                            do_stand = 1;
                            break;
                        case SDLK_h:
                            // hit !
                            do_hit = 1;
                            break;
                        case SDLK_q:
                            quit = 1;
                            break;
                        default:
                            break;
                    }
                }
            }
            //the stand
            if(do_stand == 1)
            {
                player = player->next;
                do_stand = 0;
            }
            //surrender only if its first move
            else if(do_surrender == 1 && player->pos_player_hand == 2)
            {
                player->surrender = 1;
                player = player->next;
                do_surrender = 0;

            }
            //double only if its first move
            else if(do_double == 1 && player->pos_player_hand == 2)
            {
		player->duplo=1;
                if(player->bet*2>player->money)
                {
                    player->duplo=0;
                    do_hit=1;
                }

                else
                {
        
                    player->bet += player->bet;
                    check_hit = Hit(player,&card,&numcarta, &value);
                    if(check_hit == 0 && card == NULL){
                        free_deck(head_card);
                        head_card = ShuffleDeck(nbaralhos);
                        SDL_Log("Filled and shuffled");
                        if(numcarta>=52)
                        {
                            baralhosemjogo--;
                            numcarta=0;
                        }
                        if(baralhosemjogo==0)
                            baralhosemjogo=nbaralhos;
                        card = head_card;
                        check_hit = Hit(player, &card, &numcarta,&value);
                    }
                    player = player->next;
                    check_hit = 0;
                }
                do_double = 0;
            }

            //hit
            else if(do_hit == 1)
            {
                check_hit = Hit(player,&card,&numcarta, &value);
                if(player->points >= 21){
                    player = player->next;
                }
                if(check_hit == 0 && card == NULL){
                    free_deck(head_card);
                    head_card = ShuffleDeck(nbaralhos);
                    SDL_Log("Filled and shuffled");
                    if(numcarta>52)
                    {
                        baralhosemjogo--;
                        numcarta=0;
                    }
                    if(baralhosemjogo==0)
                        baralhosemjogo=nbaralhos;
                    card = head_card;
                    check_hit = Hit(player, &card,&numcarta, &value);
                }
                check_hit = 0;
                do_hit = 0;
            }
            //to erase bug of clocking and the next player doubles/surrenders
            do_surrender =0;
            do_double = 0;

        }

        //check if all players left
        player=head_player;
        while(player->next!=NULL)
        {
            if(player->active==false)
                sair++;
            player=player->next;
        }
        if(sair==njogadores){
            SDL_Log("Game Over");
            quit=1;
        }

        //Turn the final house card and finish the play
        house.pos_house_hand = 2;
        check_house_play = house_play(&house, &card, &numcarta, &value);
        if(check_house_play == 0){
            free_deck(head_card);
            head_card = ShuffleDeck(nbaralhos);
            check_house_play = house_play(&house, &card, &numcarta, &value);
            check_house_play = 0;
        }

        if(house.pos_house_hand == 2 && house.points == 21)
            house.blackjack = 1;

        if(quit == 0)
            cashmoney( &house,head_player);

        //The game has ended so no one is playing
        player = NULL;
        // render all image
        RenderTable(head_player, player, serif, imgs, renderer);
        RenderHouseCards(house.head, house.pos_house_hand, cards, renderer);
        RenderPlayerCards(head_player, cards, renderer);
        RenderBB(head_player , serif, renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay( delay );

        //wait for new game (or bet)
        wait(&quit, &head_player, &njogadores, baralhosemjogo, &atraso, &value);

        //free the player cards in the end of the turn
        if(head_player != NULL){
             free_player_cards(head_player);
        }
        free_house_cards(house);
    }
    //write the file
    writefile(house,head_player);

    // free memory allocated
    if(head_player != NULL)
        free_players(head_player);
    //free matrix
    for(i=0;i<17;i++)
    {
        free(matrix[i]);
    }
    free(matrix);
    free_deck(head_card);
    UnLoadCards(cards);
    TTF_CloseFont(serif);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
