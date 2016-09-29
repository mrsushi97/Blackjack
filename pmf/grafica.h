#ifndef GRAFICA_H_INCLUDED
#define GRAFICA_H_INCLUDED

#define STRING_SIZE 100       // max size for some strings
#define WIDTH_WINDOW 900      // window width
#define HEIGHT_WINDOW 525     // window height
#define MAX_DECK_SIZE 52      // number of max cards in the deck
#define MAX_CARD_HAND 11      // 11 cards max. that each player can hold
#define CARD_WIDTH 67         // card width
#define CARD_HEIGHT 97        // card height
#define WINDOW_POSX 500       // initial position of the window: x
#define WINDOW_POSY 250       // initial position of the window: y
#define EXTRASPACE 150
#define MARGIN 5
#define MAX_PLAYERS 4         // number of maximum players

// declaration of the functions related to graphical issues
//Initialization functions
void InitEverything(int , int , TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer ** );
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
//Render side part
int RenderText(int , int , const char* , TTF_Font *, SDL_Color *, SDL_Renderer * );
int RenderLogo(int , int , SDL_Surface *, SDL_Renderer * );
//render the table related images
void RenderTable(node1*, node1*, TTF_Font *, SDL_Surface **, SDL_Renderer * );
void RenderBB(node1*,TTF_Font *, SDL_Renderer *);
//render the cards
void RenderCard(int , int , int , SDL_Surface **, SDL_Renderer * );
void RenderHouseCards(node2*, int , SDL_Surface **, SDL_Renderer * );
void RenderPlayerCards(node1*, SDL_Surface **, SDL_Renderer * );
//load and unload the cards
void LoadCards(SDL_Surface **);
void UnLoadCards(SDL_Surface **);

#endif // GRAFICA_H_INCLUDED