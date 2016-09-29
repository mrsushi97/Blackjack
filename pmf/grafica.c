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

// definition of some strings: they cannot be changed when the program is executed !
const char myName1[] = "Luis Ferreira";
const char myNumber1[] = "ist425422";
const char myName2[] = "Pedro Santos";
const char myNumber2[] = "ist425462";
const char * instructions[] = {"<a> new player, <b> bet", "<h> hit, <s> stand", "<d> double, <r> surrender", "<n> new game", "<q> quit"};
/**
 * RenderTable: Draws the table where the game will be played, namely:
 * -  some texture for the background
 * -  the right part with the IST logo and the student name and number
 * -  squares to define the playing positions of each player
 * -  names and the available money for each player
 * \param _money amount of money of each player
 * \param _img surfaces where the table background and IST logo were loaded
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderTable(node1* _head_player, node1* _player, TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer)
{
    SDL_Color black = { 0, 0, 0 }; // black
    SDL_Color white = { 255, 255, 255 }; // white
    SDL_Color green = { 0, 255, 0 }; // green
    SDL_Color yellow = { 255, 255, 0 }; // yellow
    SDL_Color orange = {255, 140 , 0};//orange
    char name_money_str[STRING_SIZE];
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest, playerRect;
    int separatorPos = (int)(0.95f*WIDTH_WINDOW); // seperates the left from the right part of the window
    int height;
    int i=0;

    node1*aux = _head_player;

    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

    // clear the window
    SDL_RenderClear( _renderer );


    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;

    tableDest.w = separatorPos;
    tableDest.h = HEIGHT_WINDOW;

    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);
    height = RenderLogo(separatorPos, 0, _img[1], _renderer);
    height += RenderText(separatorPos+3*MARGIN, height, myName1, _font, &black, _renderer);
    height += RenderText(separatorPos+3*MARGIN, height, myNumber1, _font, &black, _renderer);
    height += RenderText(separatorPos+3*MARGIN, height, myName2, _font, &black, _renderer);
    height += RenderText(separatorPos+3*MARGIN, height, myNumber2, _font, &black, _renderer);
    height += 10;

    // renders the squares + name for each player
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255 );

    // renders the areas for each player: names and money too !
    for ( i = 0; i < MAX_PLAYERS; i++)
    {

        //rendering of player rectangle
        playerRect.x = i*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);

        SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 255 );
        SDL_RenderDrawRect(_renderer, &playerRect);

        if(aux != NULL)
        {
            if(aux == _player && aux->active == true)
            {
                SDL_SetRenderDrawColor(_renderer, 255, 165, 0, 255 );
                SDL_RenderDrawRect(_renderer, &playerRect);
            }
            else
            {
                SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 255 );
                SDL_RenderDrawRect(_renderer, &playerRect);
            }

            if(aux != NULL && aux->active == true)
            {
                 //A cor dos jogadores varia de acordo com os seus pontos
                if(aux->points<21)
                {
                    if(aux->duplo==1)
                        sprintf(name_money_str,"%s--%d Euros(double)", aux->nome, aux->bet);
                    else
                        sprintf(name_money_str,"%s--%d Euros", aux->nome, aux->bet);
                    RenderText(playerRect.x+20, playerRect.y-50, name_money_str, _font, &white, _renderer);
                    if(aux->surrender==0)
                        sprintf(name_money_str,"%d Pontos",aux->points);
                    else
                        sprintf(name_money_str,"%d Pontos(surrender)",aux->points);
                    RenderText(playerRect.x+20, playerRect.y-30, name_money_str, _font, &white, _renderer);
                }
                if(aux->points>21)
                {
                    if(aux->duplo==1)
                        sprintf(name_money_str,"%s--%d Euros(double)", aux->nome, aux->bet);
                    else
                        sprintf(name_money_str,"%s--%d Euros", aux->nome, aux->bet);
                    RenderText(playerRect.x+20, playerRect.y-50, name_money_str, _font, &green, _renderer);
                    sprintf(name_money_str,"%d Pontos (Bust)",aux->points);
                    RenderText(playerRect.x+20, playerRect.y-30, name_money_str, _font, &green, _renderer);
                }
                if(aux->points==21 && aux->pos_player_hand == 2)
                {
                    sprintf(name_money_str,"%s--%d Euros", aux->nome, aux->bet);
                    RenderText(playerRect.x+20, playerRect.y-50, name_money_str, _font, &orange, _renderer);
                    sprintf(name_money_str,"%d Pontos (BlackJack)",aux->points);
                    RenderText(playerRect.x+20, playerRect.y-30, name_money_str, _font, &orange, _renderer);
                }
                if(aux->points==21 && aux->pos_player_hand != 2)
                {
                    if(aux->duplo==1)
                        sprintf(name_money_str,"%s--%d Euros(double)", aux->nome, aux->bet);
                    else
                        sprintf(name_money_str,"%s--%d Euros", aux->nome, aux->bet);

                    RenderText(playerRect.x+20, playerRect.y-50, name_money_str, _font, &yellow, _renderer);
                    sprintf(name_money_str,"%d Pontos (Twenty One)",aux->points);
                    RenderText(playerRect.x+20, playerRect.y-30, name_money_str, _font, &yellow, _renderer);
                }

            }


            //rendering of side information
            if(aux->human == 1)
            {
                sprintf(name_money_str, "%s HU - %d EUR", aux->nome, aux->money);
                height += RenderText(separatorPos+5, height, name_money_str, _font, &black, _renderer);
            }
            else
            {
                sprintf(name_money_str,"%s EA - %d EUR", aux->nome, aux->money);
                height += RenderText(separatorPos+5, height, name_money_str, _font, &black, _renderer);
            }
            aux = aux->next;
        }

    }

    height += 10;
    for(i=0; i<5; i++)
    {
        height += RenderText(separatorPos+3*MARGIN, height, instructions[i], _font, &black, _renderer);
    }
    // destroy everything
    SDL_DestroyTexture(table_texture);
}


/**
 * Render BlackJack or Bust
 * \param the player in cause and the top player
 * \param the font and a renderer to render all image within a window
 */
void RenderBB(node1* _head_player ,TTF_Font * _font, SDL_Renderer * _renderer){
    SDL_Color white = { 255, 255, 255 }; // white
    SDL_Color black = {0,0,0};
    SDL_Rect BB;
    char str[100];
    int separatorPos = (int)(0.95f*WIDTH_WINDOW);
    int i=0;
    node1 * aux = _head_player;

    i=0;
    while(aux != NULL){
        //params of the rectanle
        BB.x = i*(separatorPos/4-5)+10;
        BB.y = (int) (0.73f*HEIGHT_WINDOW);
        BB.w = separatorPos/4-5;
        BB.h = (int) (0.05f*HEIGHT_WINDOW);
        //color of the letters


        if(aux->points > 21 && aux->active==1){
            SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255 );
            SDL_RenderFillRect(_renderer, &BB);
            sprintf(str,"BUST");
            RenderText(BB.x + BB.w/3 + 13, BB.y, str, _font, &black, _renderer);
        }else if(aux->points == 21 && aux->pos_player_hand == 2 && aux->active==1){
            SDL_SetRenderDrawColor(_renderer, 255, 140 , 0, 255 );
            SDL_RenderFillRect(_renderer, &BB);
            sprintf(str,"BlackJack!");
            RenderText(BB.x + BB.w/3, BB.y, str, _font, &white, _renderer);
        }else if(aux->points == 21 && aux->active==1){
            SDL_SetRenderDrawColor(_renderer, 255, 255 , 0, 255 );
            SDL_RenderFillRect(_renderer, &BB);
            sprintf(str,"Twenty One!");
            RenderText(BB.x + BB.w/3, BB.y, str, _font, &black, _renderer);
        }
        aux = aux->next;
        i++;
    }

}

/**
 * RenderHouseCards: Renders cards of the house
 * \param _house vector with the house cards
 * \param _pos_house_hand position of the vector _house with valid card IDs
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderHouseCards(node2 * _head, int _pos_house_hand, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    int card=0, x=0, y=0;
    int div = WIDTH_WINDOW/CARD_WIDTH;
    node2* aux = NULL;

    aux = _head;
    // drawing all house cards
    while ( aux != NULL)
    {
        // calculate its position
        x = (div/2-_pos_house_hand/2+card)*CARD_WIDTH + 15;
        y = (int) (0.26f*HEIGHT_WINDOW);
        // render it !
        RenderCard(x, y, aux->cod,_cards, _renderer);
        aux = aux->next;
        card++;
    }
    // just one card ?: draw a card face down
    if (_pos_house_hand == 1)
    {
        x = (div/2-_pos_house_hand/2+1)*CARD_WIDTH + 15;
        y = (int) (0.26f*HEIGHT_WINDOW);
        RenderCard(x, y, MAX_DECK_SIZE, _cards, _renderer);
    }
}

/**
 * RenderPlayerCards: Renders the hand, i.e. the cards, for each player
 * \param _player_cards 2D array with the player cards, 1st dimension is the player ID
 * \param _pos_player_hand array with the positions of the valid card IDs for each player
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderPlayerCards(node1* _head_player, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    int pos=0, x=0, y=0, num_player=0, card=0;
    node1* aux = _head_player;
    node2* cards = NULL;

    num_player=0;
    // for every card of every player
    for ( num_player = 0 ; num_player<MAX_PLAYERS; num_player++)
    {
        if(aux != NULL)
        {
            if(aux->active == true)
            {
                card = 0;
                cards = aux->head;
                while ( cards != NULL)
                {
                    // draw all cards of the player: calculate its position: only 4 positions are available !
                    pos = card % 4;
                    x = (int) num_player*((0.95f*WIDTH_WINDOW)/4-5)+(card/4)*12+15;
                    y = (int) (0.55f*HEIGHT_WINDOW)+10;
                    if ( pos == 1 || pos == 3) x += CARD_WIDTH + 30;
                    if ( pos == 2 || pos == 3) y += CARD_HEIGHT+ 10;
                    // render it !
                    RenderCard(x, y, cards->cod, _cards, _renderer);
                    cards = cards->next;
                    card++;
                }
            }
            aux = aux->next;
        }
    }
}

/**
 * RenderCard: Draws one card at a certain position of the window, based on the card code
 * \param _x X coordinate of the card position in the window
 * \param _y Y coordinate of the card position in the window
 * \param _num_card card code that identifies each card
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderCard(int _x, int _y, int _num_card, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    SDL_Texture *card_text;
    SDL_Rect boardPos;

    // area that will be occupied by each card
    boardPos.x = _x;
    boardPos.y = _y;
    boardPos.w = CARD_WIDTH;
    boardPos.h = CARD_HEIGHT;

    // render it !
    card_text = SDL_CreateTextureFromSurface(_renderer, _cards[_num_card]);
    SDL_RenderCopy(_renderer, card_text, NULL, &boardPos);
}

/**
 * LoadCards: Loads all images of the cards
 * \param _cards vector with all loaded card images
 */
void LoadCards(SDL_Surface **_cards)
{
    int i=0;
    char filename[STRING_SIZE];

     // loads all cards to an array
    for (i = 0 ; i < MAX_DECK_SIZE; i++ )
    {
        // create the filename !
        sprintf(filename, ".//cartas//carta_%02d.png", i+1);
        // loads the image !
        _cards[i] = IMG_Load(filename);
        // check for errors: deleted files ?
        if (_cards[i] == NULL)
        {
            printf("Unable to load image: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }
    // loads the card back
    _cards[i] = IMG_Load(".//cartas//carta_back.jpg");
    if (_cards[i] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}


/**
 * UnLoadCards: unloads all card images of the memory
 * \param _cards vector with all loaded card images
 */
void UnLoadCards(SDL_Surface **_array_of_cards)
{
    // unload all cards of the memory: +1 for the card back
    for (int i = 0 ; i < MAX_DECK_SIZE + 1; i++ )
    {
        SDL_FreeSurface(_array_of_cards[i]);
    }
}

/**
 * RenderLogo function: Renders the IST Logo on the window screen
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _logoIST surface with the IST logo image to render
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer)
{
    SDL_Texture *text_IST;
    SDL_Rect boardPos;

    // space occupied by the logo
    boardPos.x = x;
    boardPos.y = y;
    boardPos.w = _logoIST->w;
    boardPos.h = _logoIST->h;

    // render it
    text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
    SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy associated texture !
    SDL_DestroyTexture(text_IST);
    return _logoIST->h;
}

/**
 * RenderText function: Renders the IST Logo on the window screen
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string where the text is written
 * \param font TTF font used to render the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect solidRect;

    solidRect.x = x;
    solidRect.y = y;
    // create a surface from the string text with a predefined font
    text_surface = TTF_RenderText_Blended(_font,text,*_color);
    if(!text_surface)
    {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    // create texture
    text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
    SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
    SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);

    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    return solidRect.h;
}

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _img surface to be created with the table background and IST logo
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);

    // load the table texture
    _img[0] = IMG_Load("table_texture.png");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // load IST logo
    _img[1] = SDL_LoadBMP("ist_logo.bmp");
    if (_img[1] == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitSDL: Initializes the SDL2 graphic library
 */
void InitSDL()
{
    // init SDL library
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
void InitFont()
{
    // Init font library
    if(TTF_Init()==-1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
    window = SDL_CreateWindow( "BlackJack", WINDOW_POSX, WINDOW_POSY, width+EXTRASPACE, height, 0 );
    // check for error !
    if ( window == NULL )
    {
        printf("Failed to create window : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
    renderer = SDL_CreateRenderer( _window, -1, 0 );

    if ( renderer == NULL )
    {
        printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // set size of renderer to the same as window
    SDL_RenderSetLogicalSize( renderer, width+EXTRASPACE, height );

    return renderer;
}