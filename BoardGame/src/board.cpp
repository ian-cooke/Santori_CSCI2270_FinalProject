#include "board.h"
#include <cstdlib>
#include "time.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

board::board()
{
    boardSize = 0;
    cardRem = 0;
}

board::~board()
{
    //dtor
}

void board::createBoard(int s){ // simple creation of board
    boardSize = s;
}

void board::addPlayer(string name,int numP,int bot){ //adds a player into a player linked list
    player * p = new player; //creates struct and adds correct values
    p->pos = 1;
    p->name = name;
    p->bot = bot; // says if a player is a bot or not
    p->next = NULL;
    if(first == NULL){ // sets the first player
        first = p;
        p->next = NULL;
        return;
    }
    player * cur = first;
    int counter = 2;
    while(cur->next != NULL){ // finds the proper location to add next player
        cur = cur->next;
        counter++;
    }
    cur->next = p;
    cur = cur->next;
    if(counter == numP){ // if last player, links back to first for turn taking
        cur->next = first;
    }
    curr = first;
}

int board::roll(){ // returns a random number from 1 to 6 ass part of the roll
    srand(time(NULL));
    int t = rand()%dieSize+1; //changed from 6 to dieSize
    std::cout<<curr->name<<" moved "<<t<<" spaces ahead"<<std::endl;
    return t;
}
void board::display(int numP){
    player * cur = first;
    cout<<endl;
    for(int i = 0;i<5;i++){
        switch(i){
        case 0:
            cout<<"S |"; // displays starting line
            break;
        case 1:
            cout<<"T |";
            break;
        case 2:
            cout<<"A |";
            break;
        case 3:
            cout<<"R |";
            break;
        case 4:
            cout<<"T |";
            break;
        }
        for(int j = 1;j<=boardSize;j++){ // displays player at correct location on board
            if(j != cur->pos && i < numP-1){
                cout<<" ";
            }else if(j == cur->pos && i<numP){
                cout<<cur->name[0];
            }else{
                cout<<" ";
            }
        }
        switch(i){
        case 0: // displays finish line
            cout<<"| ~"<<endl;
            break;
        case 1:
            cout<<"| E"<<endl;;
            break;
        case 2:
            cout<<"| N"<<endl;;
            break;
        case 3:
            cout<<"| D"<<endl;
            break;
        case 4:
            cout<<"| ~"<<endl;
            break;
        }
        if(cur->next != first){
            cur = cur->next;
        }
    }
    cout<<endl;
}
void board::loadDeck(){ // reads cards from a text file and puts them in a linked list
    ifstream inFile;
    string t,m;
    inFile.open("cards.txt");
    if(inFile.good()){
        int i = 0;
        card * temp;
        while(!inFile.eof()){
            card * c = new card;
            getline(inFile,c->desc); // stores card values in a card struct
            getline(inFile,t);
            c->type = atoi(t.c_str());
            getline(inFile,m);
            c->mod = atoi(m.c_str());
            if(i == 0){ // puts a card at the top of the deck
                deckS = c;
                temp = c;
                c->next = NULL;
                c->previous = NULL;
                cardRem++;
                i++;
            }else{ // puts a card after the first card
                cardRem++;
                temp->next = c;
                c->previous = temp;
                c->previous->next = c;
                temp = c;
            }
        }
        inFile.close();
    }else{
        cout<<"Error: file not found."<<endl;
    }
}

void board::movePlayer(int roll){ // function to move player throughout the board
    if(curr->pos+roll<boardSize){
        curr->pos = curr->pos+roll;
    }else{
        curr->pos = boardSize;
    }
    if(curr->pos%5 == 1){ // if a player lands on correct spot, draw a card
        drawCard(curr);
    }
}

void board::drawCard(player * p){ // picks a random card and adds it into the correct player's deck
    int cPos = rand()%cardRem;
    card * c = deckS;
    for(int i = 0;i<cPos;i++){
        c = c->next;
    }
    cout<<curr->name<<" drew a card! "<<c->desc<<endl;
    if(c->next!= NULL && c->previous != NULL){ // removes card from deck
        c->next->previous = c->previous;
        c->previous->next = c->next;
    }else if(c->next == NULL && c->previous != NULL){
        c->previous->next == NULL;
    }else if(c->next != NULL && c->previous == NULL){
        c->next->previous == NULL;
        deckS = c->next;
    }else{
        deckS == NULL;
    }
    c->next = NULL;
    card * pc = curr->deckT;
    if(pc == NULL){ //Inserts card into player deck
        p->deckT = c;
    }else{
        while(pc->next != NULL){
            pc = pc->next;
        }
        pc->next = c;
        c->previous = pc;
    }
    cardRem--; // decrements a counter counting the number of cards still in the deck
}

int board::viewDeck(){ // function to view all the cards that are in a player's hand
    card * c = curr->deckT;
    int index=1;
    if(c!=NULL){
        while(c!=NULL){ // travels through the list and displays card descriptions
            cout<<index<<". "<<c->desc<<endl;
            index++;
            c=c->next;
        }
        cout<<index<<". Cancel"<<endl;
    }else{
        cout<<"You have no cards"<<endl; // if the player has no cards display this
        index = 0;
    }
    return index; // returns tot number of cards
}

int board::playCard(){ // lets the user pick a card from their hand and play it
    int numC = viewDeck(); // gets number of cards and displays all cards
    int action = 1;
    if(numC != 0){
        cout<<"Choose a card to play by inserting the corresponding number."<<endl;
        int cardSelect = -1;
        card * selected = curr->deckT;
        while(cardSelect < 1 || cardSelect > numC-1){
            cin>>cardSelect; // gets user choice
            if(cardSelect == numC){
                return 1;
            }else if(cardSelect > 0 && cardSelect < numC){ // makes sure answer is viable
                for(int i = 1;i<cardSelect;i++){
                    selected = selected->next;
                }
            }else{
                cout<<"Please select one of the options"<<endl;
            }
        }
        switch(selected->type){ // determines correct effect based on type of card
        case 1:
            movePlayer(selected->mod); // moves player correct number of spots
            break;
        case 2:
            movePlayer(roll()); // lets user roll twice
            break;
        case 3:
            cout<<curr->next<<" has been skipped!"<<endl; // skips selected player
            action = 2;
            break;
        case 4:
            cout<<endl;
            cout<<"Choose a player"<<endl;
            player * choose = curr->next;
            int index = 0;
            while(choose!=curr){ // displays all players and allows selection
                index++;
                cout<<index<<". "<<choose->name<<endl;
                choose = choose->next;
            }
            int psel = 0;
            while(psel < 1 || psel>index){
                cin>>psel;
                if(psel < 1 || psel>index){
                    cout<<"Please pick a player"<<endl;
                }
            }
            choose = curr->next;
            for(int k = 1;k<psel;k++){
                choose = choose->next;
            }
            cout<<choose->name<<" was knocked back "<<selected->mod<<" spaces!"<<endl;

            break;
        }
        if(selected == curr->deckT){ // removes used card from players hand
            curr->deckT = selected->next;
            selected->previous = NULL;
        }else if(selected -> previous != NULL && selected->next != NULL){

        }else if(selected -> next != NULL){
            selected -> previous ->next = NULL;
        }
        delete selected;
    }
    return action;
}

bool board::winCheck(){ // checks to see if a player has reached the end
    if(curr->pos == boardSize){
        return true;
    }else{
        return false;
    }
}

//  Here is my contribution. I simply added the functionality to double the size of the die if the player wants.
void board::setDieSize(int dSize)
{
    dieSize = dSize;
}
