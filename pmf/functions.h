#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

typedef struct card{
    int cod;
    int carta;
    int naipe;
    struct card * next;
}node2;

typedef struct results{
    int wins;
    int ties;
    int losses;
}end;

typedef struct player{
    char nome[9];
    int money;
    int bet;
    int initbet;
    int human;
    int points;
    int blackjack;
    int surrender;
    int pos_player_hand;
    int aces;
    int duplo;
    int value;
    bool active;
    node2 * head; //ponteiro para a primeira carta
    end results; //a cada jogador estão associados resultados no jogo
    struct player * next;
}node1;

typedef struct house{
    int points;
    int blackjack;
    int money;
    int pos_house_hand;
    int aces;
    node2 * head;
}table; //being that house structure is not gonna be a list, has it's own different name

//non graphical functions
//reading the parameters
node1* lerparams(node1* , int*,int*, char*);
void lerdocEA(char **matrix);
//EA playing
void playEA(char **matrix, node1 *player, table house,node2* head_card,int nbaralhos,node2 **card,int *do_hit,int *do_stand,int *do_surrender, int *do_double,int *ncarta);
void hilo(int ncarta, int *);
//fill and shuffle the deck
node2* FillDeck(int);
void characterize(int, int* , int*);
node2* ShuffleDeck(int);
node2* removecard(int ,node2 **);
//first distribution to house and players
int player_distribuition(node1*, node2**,int* , int*);
int house_distribuition(table*, node2**, int *, int *);
//functions of players playing, calculating points, and house playing
int Hit(node1* ,node2**,int* , int* );
int pointing(node2*,int* , int *);
int value_card(node2 *, int*);
int house_play(table*,node2**,int* , int* );
//writing the file
void writefile(table ,node1 *);
//money related issues
void cashmoney(table*,node1 *);
//freeing functions
void free_player_cards(node1*);
void free_players(node1*);
void free_deck(node2*);
void free_house_cards(table );
//new game / bet / option A function
void wait( int*,  node1**, int*, int, int*, int*);
void add(node1**, int, int);
void bet_change(node1*);

#endif // FUNCTIONS_H_INCLUDED
