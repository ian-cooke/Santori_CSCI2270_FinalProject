#include <iostream>
#include "board.h"
#include <ctime>

using namespace std;

void sleep(float sec);

int main(){
    string replay; // declaration of variables
    bool won = false;
    won = false;
    string name;
    board bg;
    int numPH = 0;
    int numPB = -1;
    int action = 1;
    cout << "0000    000     0    0000   0000      00000    0    0   0  00000" << endl;
    cout << "0   0  0   0   0 0   0   0  0   0     0       0 0   00 00  0" << endl;
    cout << "0   0  0   0  0   0  0   0  0   0     0      0   0  0 0 0  0" << endl;
    cout << "0000   0   0  00000  0000   0   0     0  00  00000  0   0  0000" << endl;
    cout << "0   0  0   0  0   0  0   0  0   0     0   0  0   0  0   0  0" << endl;
    cout << "0   0  0   0  0   0  0   0  0   0     0   0  0   0  0   0  0"  << endl;
    cout << "0000    000   0   0  0   0  0000      00000  0   0  0   0  00000" << endl;
    cout<<endl;
    cout<< "How many human players are there? (1-5)" << endl; // gets the number of players
    while(numPH<1 || numPH>5 ){
        cin>>numPH;
        if(numPH<1 || numPH>5){
            cout<<"Error: please enter a number between 2 and 5"<<endl;
        }
    }
    if(numPH>1&&numPH<5){
        cout<<"How many bot players do you want? (0-"<<5-numPH<<")"<<endl; // gets the number of bots
        while(numPB<0 || numPB>5-numPH ){
            cin>>numPB;
            if(numPB<0 || numPB>5-numPH){
            cout<<"Error: please enter a number between 0 and"<<5-numPH<<endl;
            }
        }
    }else if(numPH == 1){
        cout<<"How many bot players do you want? (1-"<<5-numPH<<")"<<endl; // special cases determined by # humans
        while(numPB<1 || numPB>5-numPH ){
            cin>>numPB;
            if(numPB<1 || numPB>5-numPH){
            cout<<"Error: please enter a number between 1 and"<<5-numPH<<endl;
            }
        }
    }else if(numPH == 5){
        numPB = 0;
    }
    cin.ignore();
    int totP = numPH + numPB;
    for(int i = 0;i<numPH;i++){
        cout<<"Please enter your name: "; // gets names of all human players and makes a player struct for them
        getline(cin,name);
        bg.addPlayer(name,totP,0);
        name = "";

    }
    if(numPB>0){
        for(int i = 1;i<numPB+1;i++){ // puts
            name = "Bot";
            name = char(48+i)+name+char(48+i);
            bg.addPlayer(name,totP,1);
        }
    }

    // This is also part of my contribution
    int dieSize = 0;
    cout<<endl;
    cout<<"What size die would you like to use?(4-20)"<<endl; // gets user input of die size
    while (dieSize < 4 || dieSize > 20)
    {
        cin>>dieSize;
        if (dieSize < 4 || dieSize > 20)
        {
            cout<<"Please enter either '6' or '12'"<<endl;
        }
    }

    int bSize = 0;
    cout<<endl;
    cout<<"How big do you want the game board to be?(20-70)"<<endl; // gets user inputted board length
    while(bSize < 20 || bSize>70){
        cin>>bSize;
        if(bSize < 20 || bSize>70){
            cout<<"Please pick a number between 20 and 70"<<endl;
        }
    }
    bg.createBoard(bSize); // creates board
    bg.display(totP); // displays the board and player locations
    bg.loadDeck(); // loads cards from text file
    bg.setDieSize(dieSize);
    int choice = -1;
    while(choice != 3){
        if(bg.curr->bot == 0){
            cout << bg.curr->name <<" is up!" << endl; // displays menu
            cout<<endl;
            cout << "1. Roll Dice(Ends Turn)" << endl;
            cout << "2. Use Card" <<endl;
            cout << "3. Quit"<<endl;
            while(choice < 1 || choice > 3){
                cin>>choice;
                if(choice < 1 || choice > 3){
                    cout<<"Please choose one of the options."<<endl;
                }
            }
            switch(choice){
            case 1: // case for when the player wants to roll the dice
                cout<<endl;
                cout<<"Rolling..."<<endl;
                sleep(.4);
                bg.movePlayer(bg.roll());
                bg.display(totP);
                cout<<endl;
                choice = -1;
                won = bg.winCheck();
                if(!won){
                for(int l = 0;l<action;l++ ){ // jumps ahead correct # of players for if a player gets skipped
                    bg.curr = bg.curr->next;
                }
                action = 1;
                }
                break;
            case 2: // uses a card if the player has any cards
                action = bg.playCard();
                won = bg.winCheck();
                bg.display(totP);
                choice = -1;
                break;
            case 3:
                cout<<"Thank you for playing!"<<endl; // leaves game if quit is chosen
                break;
            }
            sleep(1);
        }else{ // simple bot AI
            cout<<bg.curr->name<<" is going..."<<endl;
            sleep(1.5);
            bg.movePlayer(bg.roll());
            bg.display(totP);
            won = bg.winCheck();
            if(!won){
                if(bg.curr->pos!=bSize){
                    bg.curr = bg.curr->next;
                }else{
                    won = true;
                }
            }
        }
        if(won){
            cout<<bg.curr->name<< " wins!"<<endl; // win detection
            cout<<"Thanks for playing!"<<endl;
            choice = 3;
        }
    }
}


void sleep(float sec){ // simple aesthetic wait function
    clock_t temp;
    temp = clock () + sec * CLOCKS_PER_SEC ;
    while (clock() < temp) {}
}
