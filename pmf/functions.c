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
 * FillDeck: fills the deck accordingly to the number of decks
 * \param nbaralhos: number of decks
 * \param head_card: the begining of the deck (independent from its' use)
 *
 */

node2* FillDeck(int nbaralhos)
{
    node2 *vcarta = NULL;
    node2 *ncarta = NULL;
    int cart=0, naip=0;
    int i=0,j=0;

    for(i = nbaralhos ; i > 0; i--)
    {
        for(j = 51; j >= 0; j--)
        {
            ncarta = (node2*)malloc(sizeof(node2));
            if(ncarta == NULL)
            {
                printf("Erro a alocar memoria");
                exit(EXIT_FAILURE);
            }
            ncarta->cod = j;
            ncarta->next = vcarta;
            characterize(j, &cart, &naip);
            ncarta->carta = cart;
            ncarta->naipe = naip;
            vcarta = ncarta;
        }
    }

    return vcarta;
}

/**
 *  Auxiliary function to shuffle the deck
 * \param i: number of cards in left in organized deck
 * \param head: the top card of the organized deck
 */


node2* removecard(int i,node2 **head)
{
    node2 *pos = *head;
    node2 *prev = NULL;
    int j=0;
    for(j=0;j<i;j++)
    {
        pos=pos->next;
    }

    if(pos==*head)
        *head=(*head)->next;

    else
    {
        prev=*head;
        while(prev->next!=pos)
            prev=prev->next;

        pos=prev->next;
        prev->next=pos->next;
        pos->next=NULL;
    }
    return pos;
}
/**
 *  Creates and Shuffles the deck
 * \param nbaralhos: number of decks
 */

node2* ShuffleDeck(int nbaralhos)
{
    node2 *head=NULL;
    node2 *carta=NULL;
    node2 *headbaralho=NULL;
    node2 *prev=NULL;
    int j=0,i=0;
    head=FillDeck(nbaralhos);
    for(i=52*nbaralhos;i>0;i--)
    {
        j=rand()%i;
        carta=removecard(j,&head);
        if(i==52*nbaralhos)
            {
               headbaralho=carta;
               prev=carta;
            }

        carta->next=NULL;
        prev->next=carta;
        prev=prev->next;
    }
    return headbaralho;
}

/**
 * characterize: characterizes the function with the value and the suit based on the codification
 * \param _cod : card codification
 * \param _cart and _naipe: card value and suit
 */

void characterize(int _cod, int *_cart, int *_naipe)
{
    *_cart = _cod%13 +1;
    *_naipe = _cod/13;
}



/**
 * The first distribuition each game
 * \param the player on screen
 * \param the top card of the deck
 */
int player_distribuition(node1* _player, node2** _card,int* ncarta, int *value)
{
    node2 * aux=NULL;
    //first card
    _player->aces = 0;

    if(*_card != NULL)
    {
        aux = (node2*)malloc(sizeof(node2));
         if(aux == NULL)
         {
            printf("Erro a alocar memoria");
            exit(EXIT_FAILURE);
        }
        _player->head = aux;
        *aux = **_card;
        aux->next = NULL;
        _player->pos_player_hand = 1;
        *_card = (*_card)->next;
        if(aux->carta == 13)
            _player->aces++;
        hilo(aux->carta, value);

    }
    else
    {
        return 0;
    }


    //second card
    if(*_card != NULL)
    {
        aux = (node2*)malloc(sizeof(node2));
        if(aux == NULL)
        {
            printf("Erro a alocar memoria");
            exit(EXIT_FAILURE);
        }
        _player->head->next = aux;
        *aux = **_card;
        aux->next = NULL;
        _player->pos_player_hand = 2;
        *_card = (*_card)->next;
        if(aux->carta == 13)
            _player->aces++;
        hilo(aux->carta,value);
    }
    else
    {
        free(_player->head);
        return 0;
    }


    //first calculation of the points
    _player->points = pointing(_player->head,ncarta, &(_player->aces));
    if(_player->points == 21 && _player->pos_player_hand == 2)
    {
        _player->blackjack = 1;
    }
    return 1;

}
/**
 * Distribution of the first two cards for the house
 * \param reference to the hous writefile(house,head_player);e structure
 * \param top card of the deck
 */
int house_distribuition(table * _house, node2 ** _card,int* ncarta, int*value)
{
    node2* aux=NULL;
    int ace;
    _house->aces = 0;
    //first card
    if(*_card != NULL)
    {
        aux = (node2*)malloc(sizeof(node2));
        if(aux == NULL)
        {
            printf("Erro a alocar memoria");
            exit(EXIT_FAILURE);
        }
        _house->head = aux;
        *aux = **_card;
        aux->next = NULL;
        _house->pos_house_hand = 1;
        *_card = (*_card)->next;
         if(aux->carta == 13)
            _house->aces++;
        hilo(aux->carta,value);
    }
    else
    {
        return 0;
    }

    //second card
    if(*_card != NULL)
    {
        aux = (node2*)malloc(sizeof(node2));
        if(aux == NULL)
        {
            printf("Erro a alocar memoria");
            exit(EXIT_FAILURE);
        }
        _house->head->next = aux;
        *aux = **_card;
        aux->next = NULL;
        *_card = (*_card)->next;
        hilo(aux->carta, value);
    }
    else
    {
        free(_house->head);
        return 0;
    }


    //first calculation of the points
    _house->points = 0;
    _house->points = pointing(_house->head,ncarta, &ace);

    return 1;

}

/**
 * Hit function, calculates the points in the mid term
 * \param the current player
 * \param the top card of the deck
 */
int Hit(node1 * player, node2 ** _card, int* numcarta, int* value)
{
    node2 *ncarta = NULL;
    node2 *aux = NULL;

    aux = player->head;
    while(aux->next != NULL)
    {
        aux = aux->next;
    }

    if(*_card != NULL)
    {
         ncarta = (node2*)malloc(sizeof(node2));
        if(ncarta == NULL)
        {
            printf("Erro a alocar memoria");
            exit(EXIT_FAILURE);
        }
        aux->next = ncarta;
        *ncarta = **_card;
        *_card = (*_card)->next;
        ncarta->next = NULL;
        aux = ncarta;

        if(ncarta->carta == 13)
            player->aces++;
        hilo(ncarta->carta, value);
        player->points = pointing(player->head,numcarta, &(player->aces));
        player->pos_player_hand++;
        return 1;

    }
    else
    {
        return 0;
    }
}

/**
 * Pointing the players
 * \param the first card of the player
 * \param the current points
 */
 int value_card(node2 * aux,int *ncarta)
 {
     int pontos = 0;

    if(aux->carta < 10)
        pontos += aux->carta + 1;
    else if(aux->carta < 13)
        pontos += 10;
    else
        pontos += 11;

    return pontos;
 }

int pointing(node2* _head,int *ncarta, int * _ace)
{
    node2 *aux = _head;
    int pontos = 0;
    int rest = 0;
    *_ace = 0;

    if(aux == NULL)
    {
        return 0;
    }

    while( aux != NULL)
    {
        pontos += value_card(aux,ncarta);
        aux = aux->next;
    }

    aux = _head;
    while(aux != NULL)
    {
        if(aux->carta == 13)
            *_ace = *_ace + 1;
        if(pontos > 21 && aux->carta == 13)
        {
            pontos -= 10;
            rest++;

        }
        aux= aux->next;
    }
    *_ace -= rest;
    return pontos;
}

/**
 * House playing time
 *
 */

int house_play(table * _house, node2 ** _card,int* ncarta, int* value)
{
    node2 *aux1 = _house->head;
    node2 *aux2 = NULL;
    int ace;

    while(aux1->next != NULL)
    {
        aux1 = aux1->next;
    }

    while(_house->points < 17)
    {
        if(*_card != NULL){
            aux2 = (node2*)malloc(sizeof(node2));
            aux1->next = aux2;
            *aux2 = **_card;
            *_card = (*_card)->next;
            aux2->next = NULL;
            aux1 = aux2;
            ace = _house->aces;
            _house->points = pointing(_house->head,ncarta, &ace);
            _house->aces = ace;
            hilo(aux2->carta, value);
        }
        else
        {
            //free all allocated memory because new deck will be inserted
            aux2 = aux1;
            while(aux2 != NULL)
            {
                aux1 = aux2;
                aux2 = aux1->next;
                free(aux1);
            }
            return 0;
        }
    }
    return 1;

}

/**
 * Escreve o documento
 */
void writefile(table house,node1 *head_player)
{
    node1 * player=head_player;
    FILE *f = fopen("STATS.txt", "w");

    if (f == NULL)
    {
        printf("ERROR: file not opening, exiting");
        exit(EXIT_FAILURE);

    }

    else
    {
        while(player!=NULL)
        {
            fprintf(f,"Player stats:\n\n");
            fprintf(f,"Player name:%s \n",player->nome);
            if(player->human==1)
              fprintf(f,"The player is human\n");

            else
              fprintf(f,"The player is a machine\n");
            fprintf(f,"Wins: %d\n",player->results.wins);
            fprintf(f,"Ties: %d\n",player->results.ties);
            fprintf(f,"Losses: %d\n",player->results.losses);
            fprintf(f,"Money left in the bank: %d euros\n\n",player->money);

            player=player->next;
        }
        if(house.money>0)
            fprintf(f,"The house has gained %d euros",house.money);
        else if(house.money<0)
            fprintf(f,"The house has lost %d euros",-house.money);
        else
            fprintf(f,"The house has not made or lost any money");
    }
}

/**
 * Freeing functions of heap memory
 * \first free the player list
 * \second free the player cards lists
 * \third free the deck list
 */
 void free_players(node1* _head_player)
 {
    node1 *aux1, *aux2;

    aux1 = _head_player;
    aux2 = aux1;
    while(aux2 != NULL){
        aux1 = aux2;
        aux2 = aux1->next;
        free(aux1);
    }
 }

 void free_house_cards(table _house)
 {
    node2 *aux1, *aux2;
    aux1 = _house.head;
    aux2 = _house.head;
    while(aux1 ->next != NULL)
    {
        aux1 = aux2;
        aux2 = aux1->next;
        free(aux1);
    }
 }


 void free_player_cards(node1* _head_player)
 {
    node2 *aux1, *aux2;
    node1 *_player = _head_player;

    while(_player != NULL && _player->active == true)
    {
        aux1 = _player->head;
        aux2 = _player->head;
        while(aux1 ->next != NULL)
        {
            aux1 = aux2;
            aux2 = aux1->next;
            free(aux1);
        }
        _player = _player->next;
    }


 }

 void free_deck(node2* _head_card)
 {
    node2 *aux3, *aux4;

    aux3 = _head_card;
    aux4 = aux3;
    while(aux3->next != NULL)
    {
        aux3 = aux4;
        aux4 = aux3->next;
        free(aux3);
    }
 }

/**
 * Reads the parameters on the file
 * \param the head of the players that is gonna be returned
 * \param the number of players
 * \param the number of decks
 */

node1* lerparams(node1 *_head_player,int *njogadores,int *nbaralhos,char *argv)
{
    char input[100];
    char tipo[2];
    int i=0,erro=0;
    node1 *aux = NULL, *auxf = NULL, *aux1 = NULL;

        FILE *f=fopen(argv,"r");
        if(f == NULL)
        {
            SDL_Log("Error finding file, place it on main.c");
            exit(EXIT_FAILURE);
        }
        else
        {
            fgets(input,100,f);
            sscanf(input,"%d-%d",nbaralhos,njogadores);
            if(*nbaralhos<4 || *nbaralhos>8)
            {
                SDL_Log("Invalid number of decks");
                fclose(f);
                exit(EXIT_FAILURE);
            }

            if(*njogadores<1 || *njogadores>4)
            {
                SDL_Log("Invalid number of players.");
                fclose(f);
                exit(EXIT_FAILURE);
            }

            for(i=0;i<*njogadores;i++)
            {
                aux =(node1 *)malloc(sizeof(node1));
                if(aux == NULL)
                {
                    printf("Error in memory allocation");
                    exit(EXIT_FAILURE);
                }
                aux->active = true;
                aux->value = 0;
                aux->duplo = 0;
                aux->surrender = 0;
                aux->aces = 0;
                fgets(input,100,f);

                //usamos os %int para indicar que o numero de caracteres lidos irá ser igual a int
                //usamos o [^-] para a string parar de analisar quando chegar a um '-'
                sscanf(input,"%[^-]-%[^-]-%d-%d",tipo,aux->nome,&aux->money,&aux->bet);
                aux->initbet = aux->bet;

                if(strlen(aux->nome)>8)
                {
                    SDL_Log("Invalid name.\n");
                    erro++;
                }

                if(strcmp(tipo,"HU")!=0 && strcmp(tipo,"EA")!=0)
                {
                    SDL_Log("Invalid type.\n");
                    erro++;
                }
                else if(strcmp(tipo,"HU")==0)
                    aux->human=1;

                else if(strcmp(tipo,"EA")==0)
                    aux->human=0;

                if((aux->money)<10 || (aux->money)>500)
                {
                    SDL_Log("Invalid money quantity");
                    erro++;
                }

                 if((aux->bet)<2 || (aux->bet)>0.25*(aux->money))
                {
                    SDL_Log("Invalid bet");
                    erro++;
                }
                //criação das ligações entre listas
                if(erro==0)
                {
                    if(_head_player == NULL)
                    {
                        _head_player = aux;
                    }
                    aux->next=NULL;
                    if(aux1 != NULL)
                    {
                        aux1->next = aux;
                    }
                    aux1 = aux;
                }
                else
                {
                    fclose(f);
                    aux = _head_player;
                    while(aux != NULL)
                    {
                        auxf = aux;
                        aux = aux->next;
                        free(auxf);
                    }
                    exit(EXIT_FAILURE);
                }
            }
        }
    return _head_player;
}

/**
 * Returns the money from house to players (and vice-versa) according to the points
 * \param the house
 * \param the head player
 */

void cashmoney( table* house,node1 *head_player)
 {
    node1 * player=head_player;
    while(player!=NULL)
    {
        if(player->active==1 && player->surrender == 0)
        {
             if(((player->points>house->points && player->points<=21) || (house->points>21 && player->points<=21)) && player->blackjack!=1 )
             {
                 player->money = player->money+player->bet;
                 player->results.wins++;
                 house->money = house->money-player->bet;
             }
             else if((player->points<house->points && house->points<=21) || player->points>21)
             {
                player->money = player->money-player->bet;
                player->results.losses++;
                house->money = house->money+player->bet;
             }

             else if(player->points==house->points && player->points<=21)
             {
                player->results.ties++;
             }
             else if(player->blackjack==1 && house->blackjack!=1)
             {
                player->money=player->money+1.5*player->bet;
                house->money=house->money-1.5*player->bet;
                player->results.wins++;
             }

        }
        else if(player->surrender == 1)
        {
        player->money -= 0.5*player->bet;
        house->money += 0.5*player->bet;
        player->surrender = 0;
        }

        player->blackjack = 0;
        player=player->next;
     }

     house->blackjack = 0;
 }


/**
 * waits for new game or bet or add player
 */

 void wait( int *_quit, node1 ** _head_player, int * _njogadores, int baralhosemjogo, int * _atraso, int* value)
 {
     SDL_Event event;
     int betting=0, addp=0,w=0,truecnt=0;
     int x=0,y=0, up=0, down = 0;
     bool new_game;
     node1 * aux = *_head_player;
     int betEA;

     new_game = false;

    //reset das apostas dos jogadores que fizeram double
    aux=*_head_player;
    while(aux!=NULL)
    {
        if( aux->duplo==1)
        {
            aux->bet=(aux->bet)/2;
            aux->duplo=0;
        }

        if(aux->active==1)
        {
            aux->value = aux->value + *value;
        }
        aux=aux->next;
    }
    *value=0;


    while(*_quit == 0 && new_game == false)
    {   
        betting=0;
        addp=0;
        up=0;
        down=0;
        *_quit=0;
        while(SDL_PollEvent(&event))
        {
            if( event.type == SDL_QUIT )
            {
                *_quit = 1;
                break;
            }

            else if(event.type == SDL_KEYDOWN)
            {
                switch ( event.key.keysym.sym )
                {
                    case SDLK_UP:
                        up = 1;
                        break;
                    case SDLK_DOWN:
                        down = 1;
                        break;
                    case SDLK_a:
                        SDL_PumpEvents();
                        SDL_GetMouseState(&x,&y);
                        addp = 1;
                        break;
                    case SDLK_b:
                        betting=1; 
                        break;
                    case SDLK_n:
                        new_game = true;
                        break;
                    case SDLK_q:
                        *_quit = 1;
                        break;
                    default:
                        break;
                }
             }
        }
        
        if(up == 1)
        {
            *_atraso += 500;
            SDL_Log("Delay increased by 0.5s");
        }
        else if(down == 1)
        {
            if(*_atraso > 500)
            {
                *_atraso -= 500;
                SDL_Log("Delay decreased by 0.5s");
            }
        }
        else if(betting == 1)
        {
            bet_change(*_head_player);
        }
        else if(addp == 1)
        {
            add(_head_player, x,y);
            (*_njogadores)++;
        }
        //verificar se têm dinheiro para novo jogo
        else if(new_game == true)
        {
            aux = *_head_player;
            while(aux != NULL)
            {
                if(aux->human == 1)
                {
                    if(aux->money==0)
                        aux->bet=0;
                    if(aux->bet > aux->money)
                    {
                     SDL_Log("You must change %s bet to a lower value", aux->nome);
                     new_game = false;
                    }
                }
                else
                {
                    break;
                }
                aux = aux->next;
            }
        }
        SDL_Delay(300);
    }



    //ajuste das apostas das EA's
    if(new_game==true)
    {
        aux=*_head_player;
        while(aux!=NULL)
        {
            if(aux->human==0)
            {
                betEA=aux->initbet;
                truecnt=aux->value/baralhosemjogo;
                if(truecnt>0)
                 {
                    for(w=0;w<truecnt;w++)
                        betEA=betEA*2;
                    if(betEA>aux->money)
                        betEA=aux->money;
                 }
                if(truecnt<0)
                {
                    for(w=0;w<-truecnt;w++)
                        betEA=betEA/2;
                    if(betEA<2)
                        betEA=2;
                }
		if(truecnt == 0)
			betEA = aux->bet;

                aux->bet=betEA;
             }
        aux=aux->next;
        }
    }
 }

void lerdocEA(char **matrix)
{
    int i=0,j=0;
    FILE *f;

    f=fopen("EA.txt","r");
        if(f==NULL)
        {
            printf("O ficheiro de configuração das EA's não abre");
            exit(EXIT_FAILURE);
        }
    for(i=0;i<17;i++)
    {
        for(j=0;j<10;j++)
        {
            if(fscanf(f,"%s", &matrix[i][j]) == 0)
                break;
        }
    }
    fclose(f);
}

void playEA(char **matrix, node1 *player, table house,node2* head_card,int nbaralhos,node2 **card,int *do_hit,int *do_stand,int *do_surrender, int *do_double, int*ncarta)
{
    char action;

    //Hard hand
    if (player->points>8 && player->points<17 && player->aces == 0)
            action=matrix[player->points-8][value_card(house.head,ncarta)-2];

    if(player->points<9 && player->aces == 0)
            action='H';

    if(player->points>16 && player->aces == 0)
            action='S';

    //soft hand
    if(player->points<19 && player->aces > 0)
            action=matrix[player->points-3][value_card(house.head,ncarta)-2];

    if(player->points>18 && player->aces > 0)
            action='S';

    //decidir ação a tomar
    //Hit
    if(action=='H')
    {
        *do_hit=1;
    }
    //Stand
    if(action=='S')
    {
        *do_stand=1;
    }
    //Surrender
    if(action=='R' && player->pos_player_hand == 2)
    {
        *do_surrender=1;
        player->surrender=1;
    }
    //Can't surrender? Just stand
    if(action=='R' && player->pos_player_hand != 2)
    {
        *do_stand=1;
    }
    //Double
    if(action=='D' && player->pos_player_hand == 2 && player->bet*2<=player->money)
    {
        *do_double=1;
        player->duplo=1;
    } 
    //Can't double? Just hit
    if(action=='D' && (player->pos_player_hand != 2 || player->bet*2>player->money))
    {
        *do_hit=1;
    }
    //or stand
    if(action=='D' && (player->pos_player_hand != 2 || player->bet*2>player->money) && player->points == 18 && player->aces == 1)
    {
        *do_stand=1;
    }

}

/**
 * Change your bet
 */
void bet_change(node1* aux)
{
    char str[100];
    char name[9];
    int i=0;

    printf("Which player will change its' bet?\n");
    fflush(stdout);
    fgets(str, 100, stdin);
    sscanf(str,"%s", name);
    for(i=8; i>=0; i--)
    {
        if(name[i] == '\n')
            name[i] = '\0';
    }
    while(aux != NULL)
    {
        if(aux->human == 0)
            aux = aux->next;
        if(strcmp(aux->nome, name) == 0)
        {
            printf("New bet value?\n");
            fgets(str, 100, stdin);
            sscanf(str, "%i", &(aux->bet));
            if(aux->bet > aux->money)
            {
                printf("Goodbye\n");
                fflush(stdout);
            }
            else
                SDL_Log("Bet changed successfully");
            break;
        }
        aux = aux->next;
    }
}

/**
 * Add a player where the mouse is
 */
void add(node1** _head_player, int x, int y)
{
    int separatorPos = (int)(0.95f*WIDTH_WINDOW);
    int i=0, j=0, checkscan=0;
    char str[200];
    char tipo[2];
    SDL_Rect playerRect;
    node1 * aux = *_head_player;
    node1 * replace1 = *_head_player, * replace2 = *_head_player;
    node1 * new_player = NULL;

    //goes to the last player
    while(aux->next != NULL){
        aux = aux->next;
    }


    for(i = 0; i < MAX_PLAYERS; i++)
    {
        //delimitation of the player rectangles
        playerRect.x = i*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);

        if((playerRect.x < x && x < playerRect.x + playerRect.w) && (playerRect.y < y && y < playerRect.y + playerRect.h))
        {
            for(j = 0; j<i; j++)
            {
                if(replace1 != NULL)
                    replace1 = replace1->next;
                else
                    break;
            }

            if(replace1 == *_head_player)
            {
                replace2 = NULL;
            }
            else
            {
                while(replace2->next != replace1)
                    replace2 = replace2->next;
            }
            //if its between the last square and the first
            if(replace2 !=NULL || replace1 != NULL)
            {
                //Allocate new player and put him in the end of the list
                new_player = (node1*)malloc(sizeof(node1));
		if(new_player == NULL)
		{
			SDL_Log("Error allocating memory");
			exit(1);
		}
                aux->next = new_player;
                new_player->next = NULL;
                new_player->active = false;
		new_player->surrender = 0;
		new_player->duplo = 0;

                //fill data
                SDL_Log("New player definition:\n");
                while(1)
                {
                    SDL_Log("Name?\n");
                    fgets(str,200,stdin);
                    checkscan=sscanf(str, "%s", new_player->nome);
                    if(checkscan==1 && strlen(new_player->nome)< 8)
                        break;
                    else
                        SDL_Log("Invalid name");
                }
                while(1)
                {
                    SDL_Log("Type?\n");
                    fgets(str,200,stdin);
                    sscanf(str,"%s", tipo);
                    if(strcmp(tipo,"EA") == 0)
                    {
                        new_player->human = 0;
                        break;
                    }
                    else if(strcmp(tipo, "HU") == 0)
                    {
                        new_player->human = 1;
                        break;
                    }
                    else
                        SDL_Log("Invalid type");
                }

                while(1)
                {
                    SDL_Log("Money-Bet?\n");
                    fgets(str,200,stdin);
                    checkscan=sscanf(str,"%d-%d",&new_player->money,&new_player->bet);
                    if(new_player->money >= 10 && new_player->money <= 500 && checkscan==2 && new_player->bet>=2 && new_player->bet<0.25*new_player->money)
                    {
                        aux->initbet = aux->bet;
                        break;
                    }
                    else
                        SDL_Log("Invalid value for money/bet");

                }
                new_player->value=0;
                //Replace the player in the square with the new player
                //if the trading player is the head
                if(replace2 == NULL)
                {
                    if(replace1->active == false)
                    {
                        new_player->next = replace1->next;
                        replace1->next = NULL;
                        aux->next = replace1;
                        *_head_player = new_player;
                    }
                    else
                    {
                        SDL_Log("POSITION OCCUPIED");
                        free(new_player);
                        aux->next = NULL;
                    }

                }
                //if the player changing is not the head
                else
                {
                    //if its a occupied square (false or true?)
                    if(replace1 != NULL)
                    {
                        //if the player is penniless, trade them
                        if(replace1->active == false)
                        {
                            new_player->next = replace1->next;
                            replace1->next = NULL;
                            aux->next = replace1;
                            replace2->next = new_player;
                        }
                        else
                        {
                            SDL_Log("POSITION OCCUPIED");
                            free(new_player);
                            aux->next = NULL;
                        }
                    }
                    //if replace1==NULL means its the end of the list, which is already the spot where the new player is
                }

            }
            else
            {
                SDL_Log("ERROR CREATING NEW PLAYER");
            }
        SDL_Log("Player successfully created");
        break;
        }
    }
}
//estratégia HI-lo, contar o valor das cartas
void hilo(int ncarta,int* valor)
{
    if(ncarta<6)
    {
        (*valor)++;
    }

    else if(ncarta>=9)
    {
        (*valor)--;
    }
    ncarta++;
}
