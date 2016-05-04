#ifndef BOARD_H
#define BOARD_H
#include <iostream>

struct card{ // declaration of card struct for a linked list
    card * next;
    card * previous;
    int type;
    std::string desc;
    int mod;
};

struct player{ // declaration of a player struct for a linked list
    std::string name;
    player * next;
    card * deckT = NULL;
    int pos;
    bool bot;
};


class board
{
    public:
        board();
        virtual ~board();
        int roll();
        void createBoard(int s);
        void display(int numP);
        void addPlayer(std::string name,int numP,int bot);
        void drawCard(player * p);
        int viewDeck();
        int playCard();
        void loadDeck();
        void movePlayer(int roll);
        bool winCheck();
        player * curr;

        // part of contribution
        void setDieSize(int dSize);


    protected:
    private:
        int boardSize; // various variables that are needed throughout the code
        card * deckS; // card at top of deck
        int cardRem; // num of cards in the deck
        player * first = NULL; // first player created

        // Part of contribution
        int dieSize;

};

#endif // BOARD_H
