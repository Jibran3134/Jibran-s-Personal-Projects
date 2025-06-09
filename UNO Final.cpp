#include<iostream>
#include<iomanip>
#include<cstdlib>
#include<ctime>
#include<windows.h>
#include<conio.h>
#include<fstream>
using namespace std;
#define ROWS 8
#define COLS 15
int deck[8][15];
int staticdeck[8][15];
int player1[8][15]= {0};
int player2[8][15]= {0};
int topcard;
static int player1score=0;
static int player2score=0;
int playerwin=0;;
int drawncards=0;
int playturn=1;
bool unoPlayer1 = false;
bool unoPlayer2 = false;
int discardPile[ROWS][COLS] = {0}; // 2D discard pile array

void initializeDeck(int[ROWS][COLS]);
void shuffleDeck(int[ROWS][COLS]);
void dealCards(int[ROWS][COLS],int[ROWS][COLS]);
void PrintBoard();
void printplayerhand(int, int [108], int[108], int );
void clearScreen();
void setColor(int, int );
void printdiscardpile(int,int);
void startgame();
void saveLoadGameResult();
void openfile();
void printmenu();
void printgrid(int);
void reshuffleFromDiscard();
void shuffleDeckDirectly(int target[ROWS][COLS]);
void UpdateDiscardPile(int);
void playTurn(int);
bool isValidPlay(int, int);
bool valid_draw();
int win();
string colortostring(int);
void handleSpecialCard(int );
bool callUno();
bool wild_draw_4();

int main() {
	openfile();
	printmenu();
}
void startgame() {
	int option,result;
	bool uno;
	bool topspecial=false;
	initializeDeck(deck); // at first the deck is initialized
	shuffleDeck(deck);    // then the deck is shuffled
	for(int i=0 ; i<8 ; i++) {
		for(int j=0 ; j<15; j++) {
			staticdeck[i][j]=deck[i][j]; // deck will be copied into a static deck whose values will remain same through out 
		}
	}
	dealCards(player1,player2);  // 7 cards will be dealt to each player 
	
		for( int i=0 ; i<=8 ; i++) {
		for(int j=0 ; j<15 ; j++) {
			if(staticdeck[i][j]==topcard) { // if top card is a special card
				if(j>9){
					clearScreen();
	            	PrintBoard();
					handleSpecialCard(j);
					topspecial=true;
					if(j==13 && playturn==1)
					playturn=2;                 // for wild card, the turn will be of the same person
					else if(j==13 && playturn==2)
					playturn=1;
				}
					
			}
			if(topspecial)
			break;
		}
	}
	while(playerwin!=1 || playerwin!=2) { // game will run until a player wins
		clearScreen();
		PrintBoard();      // every time a player makes a move. the board will be printed again 
		playTurn(playturn);
		result=win();
		if(result==1) {
			clearScreen();
			PrintBoard();
			cout<<"\n......Congratulations! Player 1 won......"<<endl<<endl;
			break;
		} else if(result==2) {
			clearScreen();
			PrintBoard();
			cout<<"\n......Congratulations! Player 2 won......"<<endl<<endl;
			break;
		}	else
			continue;
	}
	saveLoadGameResult();  // the game will be saved once a player wins
	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 15; j++) {
			player1[i][j] = 0;      // once the game ends, the players hands will be updated for the new game 
			player2[i][j] = 0;
		}
	}
	cout<<"1. Go to the main menu "<<endl;
	cout<<"2. View Leaderboard "<<endl;
	cout<<"3. Exit the game "<<endl;
	cout<<"Enter the option: ";
	cin>>option;
	switch(option) {
		case 1:
			clearScreen();
			printmenu();
			break;
		case 2:
			clearScreen();
			printgrid(2);
			cout<<"Player 1 : "<<player1score<<endl;
			cout<<"Player 2 : "<<player2score<<endl;
			cout<<"1. Go to the main menu "<<endl;
			cout<<"2. Exit the game "<<endl;
			cin>>option;
			if(option==1) {
				clearScreen();
				printmenu();
			} else if(option==2) {
				clearScreen();
				printgrid(3);
			}
			break;
		case 3:
			clearScreen();
			printgrid(3);
			break;
		default:
			cout<<"Invalid input";
			break;
	}
}
void printmenu() {
	int option;
	clearScreen();
	printgrid(1);
	cout<<"1. Start the game "<<endl;
	cout<<"2. View Leaderboard "<<endl;
	cout<<"3. Exit the game "<<endl;
	cout<<"Enter the option: ";
	cin>>option;
	switch(option) {
		case 1:
			clearScreen();
			startgame();
			break;
		case 2:
			clearScreen();
			printgrid(2);
			cout<<"Player 1 : "<<player1score<<endl;
			cout<<"Player 2 : "<<player2score<<endl;
			cout<<"1. Go to the main menu "<<endl;
			cout<<"2. Exit the game "<<endl;
			cin>>option;
			if(option==1)
				printmenu();
			else if(option==2) {
				clearScreen();
				printgrid(3);
				break;
			} else
				cout<<"Invalid Input";
			break;
		case 3:
			clearScreen();
			printgrid(3);
			break;
		default:
			cout<<"Invalid input";
			break;

	}

}
void printgrid(int grid) {
	if(grid==1) {
		cout << "****************************************************"<<endl;
		cout << "**                                                **"<<endl;
		cout << "**                   Welcome to Uno               **"<<endl;
		cout << "**                                                **"<<endl;
		cout << "****************************************************"<<endl;
	} else if(grid==2) {
		cout << "****************************************************"<<endl;
		cout << "**                                                **"<<endl;
		cout << "**                  Leaderboard                   **"<<endl;
		cout << "**                                                **"<<endl;
		cout << "****************************************************"<<endl;
	} else if(grid==3) {
		cout << "****************************************************"<<endl;
		cout << "**                                                **"<<endl;
		cout << "**                 Exiting the game               **"<<endl;
		cout << "**                                                **"<<endl;
		cout << "****************************************************"<<endl;
	}

}

// Jibran's main functions

void dealCards(int player1[8][15],int player2[8][15]) {


	for( int i=0 ; i<7 ; i++) {
		for(int j=0 ; j<8 ; j++) {
			for(int k=0 ; k<15 ; k++) {
				if(deck[j][k]==-1) {
					player1[j][k]=deck[j][k];  // in the starting all the positions that are invalid, will be assigned -1
					player2[j][k]=deck[j][k];
				}
			}
		}
	}

	for( int i=1 ; i<=7 ; i++) {
		for(int j=0 ; j<8 ; j++) {
			for(int k=0 ; k<15 ; k++) {
				if(deck[j][k]==i) {
					player1[j][k]=i;
					deck[j][k]=0;
				}
			}
		}

	}
	for( int i=8 ; i<=14; i++) {
		for(int j=0 ; j<8 ; j++) {
			for(int k=0 ; k<15 ; k++) {
				if(deck[j][k]==i) {
					player2[j][k]=i;
					deck[j][k]=0;
				}
			}
		}
	}
	drawncards=14;
	topcard=drawncards++;
	

	for(int j=0 ; j<8 ; j++) {
		for(int k=0 ; k<15 ; k++) {
			if(deck[j][k]==drawncards) {
				deck[j][k]=0;
			}
		}
	}
}
void initializeDeck(int deck[8][15]) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 15; j++) {
			if(i%2!=0)
				deck[i][j] = 1;  // there is only 1 card of 0 for each color
			else                 // so other rows will be assigned -1 indicating they are not accessable 
				deck[i][j] = -1;
		}
	}

	for(int i=0 ; i<8 ; i++)
		for(int j=1 ; j<=12 ; j++) {  // 1 is stored in all the places where there can be a card 
			deck[i][j]=1;
		}
	for (int i = 0; i < 8; i++) {
		for (int j = 13; j <= 14; j++) {
			if(i%2!=0)
				deck[i][j] = 1;  // as there can only be 4 wild and wild draw four cards 
			else                 // so other indexes are assigned -1
				deck[i][j] = -1;
		}
	}

}
void handleSpecialCard(int specialCardType) {
	string wildcolor;
	if(specialCardType==10) {
		cout<<"\n......The turns have been reversed......\n";
		if(playturn==1)
			playturn=2;
		else if(playturn==2)
			playturn=1;
	} else if(specialCardType==11) {
		cout<<"\n......The turn has been skipped......\n";
		if(playturn==1)
			playturn=2;
		else if(playturn==2)
			playturn=1;
	} else if(specialCardType==12) {
		cout<<"\n......Drawing two cards......\n";
		// as player turn has already been changed so
		for(int i=0 ; i<2 ; i++) {
			drawncards++;                   // the drawncards variable will increment everytime a card is picked from the deck
			for(int j=0 ; j<8 ; j++) {
				for(int k=0 ; k<15 ; k++) {
					if(deck[j][k]==drawncards) {
						if(playturn==1)               // cards are assigned based on the players turn
							player1[j][k]=drawncards;
						else if(playturn==2)
							player2[j][k]=drawncards;
						deck[j][k]=0;  // after a card is assigned, that position in the deck stores 0, indicating there is no card here
					}
				}
			}
		}
		if(playturn==1)
			playturn=2;
		else if(playturn==2)       // after wild card, the turns are reversed
			playturn=1;
	} else if(specialCardType==13 ) {
		do {

			cout<<"Enter the Color you want to proceed with: ";
			cin>>wildcolor;
			if(wildcolor!="red" && wildcolor!="green" && wildcolor!="yellow" && wildcolor!="blue")
				cout<<"Invalid Input"<<endl;
		} while(wildcolor!="red" && wildcolor!="green" && wildcolor!="yellow" && wildcolor!="blue");
		if(wildcolor=="red")
			topcard=1000;
		else if(wildcolor=="green")
			topcard=2000;
		else if(wildcolor=="yellow")  // based on the selected color, we pass special value to the topcard
			topcard=3000;
		else if(wildcolor=="blue")
			topcard=4000;
	} else if(specialCardType==14) {
		do {

			cout<<"Enter the Color you want to proceed with: ";
			cin>>wildcolor;
			if(wildcolor!="red" && wildcolor!="green" && wildcolor!="yellow" && wildcolor!="blue")
				cout<<"Invalid Input"<<endl;
		} while(wildcolor!="red" && wildcolor!="green" && wildcolor!="yellow" && wildcolor!="blue");
		if(wildcolor=="red")
			topcard=5000;
		else if(wildcolor=="green")
			topcard=6000;
		else if(wildcolor=="yellow")  // based on the selected color, we pass special value to the topcard
			topcard=7000;
		else if(wildcolor=="blue")
			topcard=8000;

		cout<<"\n......Drawing four cards......\n";
		for(int i=0 ; i<4 ; i++) {
			drawncards++;
			for(int j=0 ; j<8 ; j++) {
				for(int k=0 ; k<15 ; k++) {
					if(deck[j][k]==drawncards) {
						if(playturn==1)
							player1[j][k]=drawncards;
						else if(playturn==2)
							player2[j][k]=drawncards;
						deck[j][k]=0;
					}
				}
			}
		}
		if(playturn==1)
			playturn=2;
		else if(playturn==2)
			playturn=1;
	}
}
void PrintBoard() {
	int TCnum;   // top card number
	int TCcolor;   // top card color
	int no_of_cards_p2=0;
	int no_of_cards_p1=0;
	int k1=0,k2=0;
	int color1[108]= {0};  // array storing colors of player 1's cards
	int num1[108]= {0};     // array storing numbers of player 1's cards
	int color2[108]= {0};   // array storing colors of player 2's cards
	int num2[108]= {0};     // array storing numbers of player 2's cards

	for(int i=0 ; i<8 ; i++) {
		for(int j=0 ; j<15; j++) {
			if(	topcard==staticdeck[i][j]) {
				TCnum=j;      // we will check the top card from the static deck and extract its color and number 
				TCcolor=i;
				break;
			}
		}
	}

	if(topcard==1000) { 
		TCcolor=1;
		TCnum=13;
		topcard=staticdeck[TCcolor][TCnum];
	} else if(topcard==2000) {
		TCcolor=3;
		TCnum=13;
		topcard=staticdeck[TCcolor][TCnum];
	} else if(topcard==3000) {
		TCcolor=5;                                  // if special values are encountered, the respective color is assigned to the top card 
		TCnum=13;
		topcard=staticdeck[TCcolor][TCnum];
	} else if(topcard==4000) {
		TCcolor=7;
		TCnum=13;
		topcard=staticdeck[TCcolor][TCnum];
	} else if(topcard==5000) {
		TCcolor=1;
		TCnum=14;
		topcard=staticdeck[TCcolor][TCnum];
	} else if(topcard==6000) {
		TCcolor=3;
		TCnum=14;
		topcard=staticdeck[TCcolor][TCnum];       // if special values are encountered, the respective color is assigned to the top card 
	} else if(topcard==7000) {
		TCcolor=5;
		TCnum=14;
		topcard=staticdeck[TCcolor][TCnum];
	} else if(topcard==8000) {
		TCcolor=7;
		TCnum=14;
		topcard=staticdeck[TCcolor][TCnum];
	}

	if(playturn==1) {
		for(int i=0 ; i<8 ; i++) {
			for(int j=0 ; j<15 ; j++) {
				{
					if(player1[i][j]!=0 && player1[i][j]!=-1) {
						color1[k1]=i;
						num1[k1]=j;
						k1++;
						no_of_cards_p1++;                    // for each player's turn, we will store the information of the player's cards
					}                                          // in an array for numbers and an array for colors 
					if(player2[i][j]!=0 && player2[i][j]!=-1) {
						color2[k2]=i;
						num2[k2]=j;
						k2++;
						no_of_cards_p2++;
					}
				}
			}
		}
		printplayerhand(1,color1,num1,no_of_cards_p1 ); // 1 indicates to not hide player 1's cards 
		cout<<endl;
		printdiscardpile( TCcolor,TCnum);  // it will print the discard pile and the deck 
		cout<<endl;
		printplayerhand(0,color2,num2,no_of_cards_p2 ); // 0 indicates to hide player 2's cards 
	}
	if(playturn==2) {
		for(int i=0 ; i<8 ; i++) {
			for(int j=0 ; j<15 ; j++) {
				{
					if(player1[i][j]!=0 && player1[i][j]!=-1) {
						color1[k1]=i;
						num1[k1]=j;
						k1++;
						no_of_cards_p1++;                           // for each player's turn, we will store the information of the player's cards
					}                                              // in an array for numbers and an array for colors
					if(player2[i][j]!=0 && player2[i][j]!=-1) {
						color2[k2]=i;
						num2[k2]=j;
						k2++;
						no_of_cards_p2++;
					}
				}
			}
		}
		printplayerhand(0,color1,num1,no_of_cards_p1 );  // 0 indicates to hide player 1's cards 
		cout<<endl;
		printdiscardpile( TCcolor,TCnum);               // it will print the discard pile and the deck 
		cout<<endl;
		printplayerhand(1,color2,num2,no_of_cards_p2 );   // 1 indicates to not hide player 2's cards 
	}

}
void saveLoadGameResult() {
	ofstream myfile;                 // first we will open the file for writing
	myfile.open("leaderboard.txt");
	if(playerwin==1)
		player1score++;             // then we will increment the value of the global variables
	else if(playerwin==2)          // and store their value in the file as scores
		player2score++;

	myfile<<player1score<<" "<<player2score;  // this will store their value in the file as scores
	myfile.close();
}
void openfile() {
	ifstream myfile;                   // first we open the file for reading
	myfile.open("leaderboard.txt");
	if (myfile.is_open()) {
		myfile>>player1score>>player2score;// than we will take the players
		myfile.close();                    // score from the file and save it in our variables
	} else {
		ofstream myfile;                    // if file does not exist it will open the file for writing
		myfile.open("leaderboard.txt");    // which will automatically create a file
		myfile.close();        // after creating the file we will close it
	}
}

// Jibran's helper functions

void printplayerhand(int playturn, int color[108], int num[108], int no_of_cards ) {
	const int BLUE = 1;
	const int GREEN = 2;
	const int RED = 4;
	const int YELLOW = 6;
	const int BLACK = 0;
	const int WHITE = 7;
	string c10="REVERSE";
	string c11="SKIP";
	string c12="DRAW 2";   // these strings will be displayed on the player's cards
	string c13="WILD";
	string c14="WILD DRAW 4";

	if (playturn == 1) {
		int temp = 0;
		while (temp < no_of_cards) {
			for (int row = 0; row < 5; row++) {
				for (int i = temp; i < temp + 10 && i < no_of_cards; i++) { // print 10 cards per row
					cout <<" ";
					if (num[i] <= 12) {
						setColor(BLACK, color[i]);
						if (row == 0)
							cout << " ------- ";
						else if (row == 1)
							cout << " -     - ";
						else if (row == 2) {
							if (num[i] == 10)
								cout << " " << c10 << " ";
							else if (num[i] == 11)
								cout << " -" << c11 << " - ";
							else if (num[i] == 12)
								cout << "  " << c12 << " ";
							else
								cout << " -  " << num[i] << "  - ";
						} else if (row == 3)
							cout << " -     - ";
						else if (row == 4)
							cout << " ------- ";
						setColor(WHITE, 10);
					} else { // Special cards
						setColor(BLACK, 13);
						if (row == 0)
							cout << " ------- ";
						else if (row == 1) {
							if (num[i] == 14)
								cout << " -WILD - ";
							else
								cout << " -     - ";
						} else if (row == 2) {
							if (num[i] == 13)
								cout << " -" << c13 << " - ";
							else if (num[i] == 14)
								cout << " -DRAW - ";
						} else if (row == 3) {
							if (num[i] == 14)
								cout << " -  4  - ";
							else
								cout << " -     - ";
						} else if (row == 4)
							cout << " ------- ";
						setColor(WHITE, 10);
					}
				}
				cout << endl;
			}
			cout << endl;
			temp += 10;
		}
	}



	if(playturn==0) {
		int temp = 0;
		while (temp < no_of_cards) {
			for (int row = 0; row < 5; row++) {
				for (int i = temp; i < temp + 10 && i < no_of_cards; i++) {
					cout<<" ";
					setColor(BLACK, 8);
					if (row == 0)
						cout << " ------- " ;
					else if (row == 1 )
						cout << " -     - " ;
					else if (row == 2)
						cout << " - uno - " ;
					else if(row == 3)
						cout << " -     - " ;
					else if (row == 4)
						cout << " ------- " ;
					setColor(WHITE, 10);
					cout<<" ";
				}
				setColor(WHITE, 10);
				cout << endl;
			}
			cout << endl;
			temp+=10;
		}

	}

}
void clearScreen() {
//    system("clear");  //For Mac /Linux;
	system("CLS"); //For Windows

}
void printdiscardpile(int color,int num) {
	const int BLUE = 1;
	const int GREEN = 2;
	const int RED = 4;
	const int YELLOW = 6;
	const int BLACK = 0;
	const int WHITE = 7;
	string c10="REVERSE";
	string c11="SKIP";
	string c12="DRAW 2";
	string c13="WILD";
	string c14="WILD DRAW 4";
	for (int row = 0; row < 5; row++) {
		for(int i=0 ; i<2 ; i++) {
			cout<<"          ";
			if(num<=12) {
				if(i==0)
					setColor(BLACK, 8);
				else
					setColor(BLACK, color);
				if (row == 0)
					cout << " ------- " ;
				else if (row == 1 )
					cout << " -     - " ;
				else if (row == 2) {
					if(num==10 && i==1)
						cout<< " "<<c10<<" " ;
					else if(num==11 && i==1)
						cout<< " -"<<c11<<" - " ;
					else if(num==12 && i==1)
						cout<< "  "<<c12<<" " ;
					else {
						cout << " - ";
						if(i==0)
							cout<<"uno - " ;
						else
							cout<<" "<<num<<"  - " ;
					}
				}

				else if(row == 3)
					cout << " -     - " ;
				else if (row == 4)
					cout << " ------- " ;
				setColor(WHITE, 10);
				cout<<" ";
			} else {
				if(i==0)
					setColor(BLACK, 8);
				else
					setColor(BLACK, color);
				if (row == 0)
					cout << " ------- " ;       // based on the top card's color and number, the discard pile will be printed
				else if (row == 1 )
					if(num==14 && i==1)
						cout<< " -WILD - " ;
					else
						cout << " -     - " ;
				else if (row == 2) {
					if(num==13 && i==1)
						cout<< " -"<<c13<<" - " ;
					else if(num==14 && i==1)
						cout<< " -DRAW - " ;
					else {
						cout << " - ";
						if(i==0)
							cout<<"uno - " ;
						else
							cout<<num<<"  - " ;
					}
				} else if(row == 3)
					if(num==14 && i==1)
						cout<< " -  4  - " ;
					else
						cout << " -     - " ;
				else if (row == 4)
					cout << " ------- " ;
				setColor(WHITE, 10);
				cout<<" ";
			}
			setColor(WHITE, 10);
			cout<<"          ";
		}

		setColor(WHITE, 10);
		cout << endl;
	}


}
void setColor(int textColor, int bgColor) {
	switch(bgColor) {
		case 0:
		case 1:
			bgColor=4;
			break;
		case 2:
		case 3:
			bgColor=2;
			break;
		case 4:           // color codes are set according to the numbers indicaring colors in our deck
		case 5:
			bgColor=6;
			break;
		case 6:
		case 7:
			bgColor=1;
			break;
		case 10:
			bgColor=0;
			break;
	}

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}
int win() {
	int result;
	int count1=0,count2=0;
	for(int i=0 ; i<8 ; i++) {
		for(int j=0 ; j<15; j++) {
			if(player1[i][j]==0)   // both players arrays will be checked , if there are any cards left
				count1++;
			if(player2[i][j]==0)
				count2++;
		}
	}
	if(count1==108) {  // if whole array is empty than the player wins
		result=1;
		playerwin=1;
	} else if(count2==108) {

		result=2;
		playerwin=2;
	} else
		result=0;
	return result;
}

// Alishba's main functions

void shuffleDeck(int deck[ROWS][COLS]) {
	int num=1;
	for(int i=0; i<8; i++) {
		for(int j=0; j<15; j++) {
			if(deck[i][j]!=-1)     // first numbers 1 to 108 are assigned to the deck , leaving the places where -1 is stored
				deck[i][j]=num++;
		}
	}
	srand(time(NULL));

	for (int i = 1; i <= 108; i++) {
		// Start from index 0 and iterate forward
		int r1 = i / COLS;  // Row of the first selected card
		int c1 = i % COLS;  // Column of the first selected card

		// Generate a random index j in the range [i, ROWS * COLS - 1]
		if(deck[r1][c1]!=-1) {
			int j = rand() % (ROWS * COLS);

			int r2 = j / COLS;  // Row of the random selected card
			int c2 = j % COLS;  // Column of the random selected card
			if(deck[r2][c2]!=-1)
				// Swap the two selected cards
				std::swap(deck[r1][c1], deck[r2][c2]);
		}
	}
}
void playTurn(int playerturn) {
	int playerCard,cardnum,cardcolor;
	string cardcolorname;
	bool validplay;
	bool validdraw=valid_draw();
	bool carddrawn=false;
	bool uno;

	char op;
	cout<<"Enter 1 to play a card: "<<endl;
	cout<<"Enter 2 to draw a card: "<<endl;
// cout<<"Enter e to exit the game: "<<endl;
// cout<<"Enter 'u' to call uno: "<<endl;
	cin>>op;
	switch(op) {

		case '2':
			if(validdraw) {

				if(playturn==1) {
					for(int i=0 ; i<8 ; i++) {
						for(int j=0 ; j<15 ; j++) {
							if(deck[i][j]==drawncards+1) {
								player1[i][j]=deck[i][j];
								deck[i][j]=0;           // player 1 draws a card, add a card to player 1's array from the deck
								drawncards++;
								carddrawn=true;
								break;
							}
						}
						if(carddrawn==true)
							break;
					}
				}
				if(playerturn==2) {
					for(int i=0 ; i<8 ; i++) {
						for(int j=0 ; j<15 ; j++) {
							if(deck[i][j]==drawncards+1) {
								player2[i][j]=deck[i][j];// player 2 draws a card, add a card to player 2's array from the deck
								deck[i][j]=0;
								drawncards++;
								carddrawn=true;
								break;
							}
						}
						if(carddrawn==true)
							break;
					}
				}
				validdraw=valid_draw();
				if(playerturn==1 && validdraw) {
					playturn=2;              // valid draw means that their is no playable card
				} else	if(playerturn==2 && validdraw) {
					playturn=1;       // if valid draw fails than the player is allowed another turn 
				}
			} else{
			
				cout<<"Cannot draw a card. You have a playable card in deck"<<endl;
				getch();}
			break;
		case '1':
			if(!validdraw) {
// if player has a playable card
				do {
					do {

						cout<<"Player "<<playturn<<"\n Enter your card num(10.Reverse 11.Skip 12.Draw 2 13.Wild 14.Wild Draw 4): ";
						cin>>cardnum;
						if(cardnum<0 || cardnum>14)
							cout<<"....Invalid Input.... "<<endl;

					} while(cardnum<0 || cardnum>14); 
					if(cardnum<=12) {
						do {
							cout<<"Enter your card color: ";
							cin>>cardcolorname;   // color of card will be stored in an array 
							if(cardcolorname!="red" && cardcolorname!="green" && cardcolorname!="yellow" && cardcolorname!="blue")
								cout<<"Invalid Input"<<endl;
						} while(cardcolorname!="red" && cardcolorname!="green" && cardcolorname!="yellow" && cardcolorname!="blue");


						if(cardcolorname=="red") {
							if(playerturn==1) {
								if(player1[0][cardnum]!=0 && player1[0][cardnum]!=-1)
									cardcolor=0;   // based on the cards color and number, we will check on which index the card is stored
								else if(player1[1][cardnum]!=0 && player1[1][cardnum]!=-1)
									cardcolor=1;

							}
							if(playerturn==2) {
								if(player2[0][cardnum]!=0 && player2[0][cardnum]!=-1)
									cardcolor=0;// based on the cards color and number, we will check on which index the card is stored
								else if(player2[1][cardnum]!=0 && player2[1][cardnum]!=-1)
									cardcolor=1;

							}
						} else if(cardcolorname=="green") {
							if(playerturn==1) {
								if(player1[2][cardnum]!=0 && player1[2][cardnum]!=-1)
									cardcolor=2;// based on the cards color and number, we will check on which index the card is stored
								else if(player1[3][cardnum]!=0 && player1[3][cardnum]!=-1)
									cardcolor=3;

							}
							if(playerturn==2) {
								if(player2[2][cardnum]!=0 && player2[2][cardnum]!=-1)
									cardcolor=2;// based on the cards color and number, we will check on which index the card is stored
								else if(player2[3][cardnum]!=0 && player2[3][cardnum]!=-1)
									cardcolor=3;


							}
						} else if(cardcolorname=="yellow") {
							if(playerturn==1) {
								if(player1[4][cardnum]!=0 && player1[4][cardnum]!=-1)
									cardcolor=4;// based on the cards color and number, we will check on which index the card is stored
								else if(player1[5][cardnum]!=0 && player1[5][cardnum]!=-1)
									cardcolor=5;
							}
							if(playerturn==2) {
								if(player2[4][cardnum]!=0 && player2[4][cardnum]!=-1)
									cardcolor=4;// based on the cards color and number, we will check on which index the card is stored
								else if(player2[5][cardnum]!=0 && player2[5][cardnum]!=-1)
									cardcolor=5;


							}
						} else if(cardcolorname=="blue") {
							if(playerturn==1) {
								if(player1[6][cardnum]!=0 && player1[6][cardnum]!=-1)
									cardcolor=6;// based on the cards color and number, we will check on which index the card is stored
								else if(player1[7][cardnum]!=0 && player1[7][cardnum]!=-1)
									cardcolor=7;

							}
							if(playerturn==2) {
								if(player2[6][cardnum]!=0 && player2[6][cardnum]!=-1)
									cardcolor=6;// based on the cards color and number, we will check on which index the card is stored
								else if(player2[7][cardnum]!=0 && player2[7][cardnum]!=-1)
									cardcolor=7;

							}
						}
					} else { // for wild and wild draw 4 cards 
						if(playerturn==1) {
							if(player1[1][cardnum]!=0 && player1[1][cardnum]!=-1)
								cardcolor=1;
							else if(player1[3][cardnum]!=0 && player1[3][cardnum]!=-1)
								cardcolor=3;// based on the cards color and number, we will check on which index the card is stored
							else if(player1[5][cardnum]!=0 && player1[5][cardnum]!=-1)
								cardcolor=5;
							else if(player1[7][cardnum]!=0 && player1[7][cardnum]!=-1)
								cardcolor=7;

						} else if(playerturn==2) {

							if(player2[1][cardnum]!=0 && player2[1][cardnum]!=-1)
								cardcolor=1;
							else if(player2[3][cardnum]!=0 && player2[3][cardnum]!=-1)
								cardcolor=3;// based on the cards color and number, we will check on which index the card is stored
							else if(player2[5][cardnum]!=0 && player2[5][cardnum]!=-1)
								cardcolor=5;
							else if(player2[7][cardnum]!=0 && player2[7][cardnum]!=-1)
								cardcolor=7;

						}
					}
					if(playerturn==1)
						playerCard=player1[cardcolor][cardnum];
					else if(playerturn==2)  // the card is assigned to a variable playercard
						playerCard=player2[cardcolor][cardnum];
					else
						cout<<"\n....Invalid Player turn....\n";

					validplay=isValidPlay(playerCard,topcard);  // check if the card being played matches the top card
					if(playerCard==0 || playerCard==-1) {
						cout<<"\n....No such card in deck....\n"<<endl;
						cout<<"....Enter a valid card....\n"<<endl;
					} else if(validplay==0)
						cout<<"\n....Invalid Card entry....\n"<<endl;
				} while((playerCard==0 ||  validplay==0  ||playerCard==-1   )); // keep on asking for the right card to be played 

				if(playerturn==1) {
					player1[cardcolor][cardnum]=0; // as the card is played , so it is removed from the player's array 
					playturn=2;
				} else	if(playerturn==2) {
					player2[cardcolor][cardnum]=0;// as the card is played , so it is removed from the player's array 
					playturn=1;
				}
				if(cardnum>9 && validplay)
					handleSpecialCard(cardnum); // if user plays a special card 
				if(cardnum!=13 && cardnum!=14)
					UpdateDiscardPile(playerCard); // wild and wild draw 4 have different behaviours so we will not update discard pile for them 
			}
			break;
		case 'e':
		case 'E':
			char ch;
			cout<<"Are you sure you want to exit (Y/N)? ";
			cin>>ch;
			if(ch=='Y' || ch=='y') {
				clearScreen();
				printgrid(3);
				exit(0);
			}
			break;
		case 'u':
		case'U': {
			uno=callUno();  // call uno everytime u is pressed 
		}

		break;
		case 'p':
		case 'P': {
			string nul;
			do {

				cout<<".....Game is Paused.....";
				cin>>nul;             // pause game everytime p is pressed 
				if(nul!="p" && nul!="P")
					cout<<"Enter 'p' to go back to the game "<<endl;
			} while(nul!="p" && nul!="P");
		}
		break;
		default:
			cout<<"Invalid Input"<<endl;
			playTurn(playerturn);
			break;
	}
			int count = 0;
    if (playturn == 2) { // As turns have already been changed
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 15; j++) {
                if (player1[i][j] != 0 && player1[i][j] != -1)
                    count++;
            }
        }
        if (count == 1 && !unoPlayer1) { // Check if player 1 did not call UNO
            cout << "....Player 1 did not say UNO...." << endl;
            cout << "....Drawing two cards...." << endl;
            getch();
            for (int i = 0; i < 2; i++) { // 2 cards will be assigned as penalty 
                drawncards++;
                bool cardAssigned = false;
                for (int j = 0; j < 8; j++) {
                    for (int k = 0; k < 15; k++) {
                        if (deck[j][k] == drawncards) {
                            player1[j][k] = drawncards;
                            deck[j][k] = 0;
                            cardAssigned = true;
                            break;
                        }
                    }
                    if (cardAssigned) break;
                }
            }
        }
    } else if (playturn == 1) { // For player 2
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 15; j++) {
                if (player2[i][j] != 0 && player2[i][j] != -1)
                    count++;
            }
        }
        if (count == 1 && !unoPlayer2) { // Check if player 2 did not call UNO
            cout << "....Player 2 did not say UNO...." << endl;
            cout << "....Drawing two cards...." << endl;
            getch();
            for (int i = 0; i < 2; i++) {// 2 cards will be assigned as penalty 
                drawncards++;
                bool cardAssigned = false;
                for (int j = 0; j < 8; j++) {
                    for (int k = 0; k < 15; k++) {
                        if (deck[j][k] == drawncards) {
                            player2[j][k] = drawncards;
                            deck[j][k] = 0;
                            cardAssigned = true;
                            break;
                        }
                    }
                    if (cardAssigned) break;
                }
            }
        }
    }
    if (count > 2) {  // if there are more than 2 cards, update the boolean variables 
        if (playturn == 1)
            unoPlayer1 = true;
        else if (playturn == 2)
            unoPlayer2 = true;
    }
}
bool isValidPlay(int playerCard, int topCard) {
	int player_card_color;
	int player_card_number;
	int top_card_color;
	int top_card_number;
	if(topCard>=1000) {

		return true;  // if top card is a wild or wild draw 4. any card can be played 

	} else {

		for(int i=0 ; i<8 ; i++) {
			for(int j=0 ; j<15 ; j++) {

				if(staticdeck[i][j]==playerCard) {
					player_card_color=i;  // extract the player card's number and color from static deck 
					player_card_number=j;
				}
				if(staticdeck[i][j]==topCard) {
					top_card_color=i;    // extract the top card's number and color from static deck 
					top_card_number=j;
				}
			}
		}
		string pcstring=colortostring(player_card_color); // convert the player cards color from number to string
		string tcstring=colortostring(top_card_color);// convert the top cards color from number to string
if(pcstring!=tcstring && player_card_number!=13 && player_card_number!=14 &&player_card_number==top_card_number )
cout<<"\n....Not the right Color...."<<endl; // display the message if color doesnt match 

		if(( pcstring==tcstring) || (player_card_number==top_card_number) || (player_card_number==13 || player_card_number==14))
			return true; // if card is valid, return true
		else
			return false;
	}
}
void UpdateDiscardPile(int playerCard) {
    topcard = playerCard;

    // Add the current topcard to the discard pile
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (staticdeck[i][j] == topcard) {
                discardPile[i][j] = topcard;
            }
        }
    }

    // Count empty slots in the deck
    int emptySlots = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (deck[i][j] == 0 || deck[i][j] == -1) {
                emptySlots++;
            }
        }
    }

    // If only one card left, reshuffle discard pile 
    if (emptySlots >= ROWS * COLS) {
        cout << "Reshuffling discard pile without last topcard.\n";
        reshuffleFromDiscard();
    }
}
bool callUno() {
	int count=0;
	char uno;

	if(playturn==1) {
		for(int i=0 ; i<8 ; i++) {
			for(int j=0 ; j<15 ; j++) {
				if(player1[i][j]!=0 && player1[i][j]!=-1)
					count++;
			}                        // count variable checks if player has 2 cards left or not 
		}
	} else if(playturn==2) {
		for(int i=0 ; i<8 ; i++) {
			for(int j=0 ; j<15 ; j++) {
				if(player2[i][j]!=0 && player2[i][j]!=-1)
					count++;
			}
		}
	}
	if(count==2) {
		do {
			cout<<"Enter 'u' to confirm for calling uno: ";
			cin>>uno;
			if(uno!='U' && uno!='u')
				cout<<".....Incorrect input....."<<endl;
		} while(uno!='U' && uno!='u');
			cout<<"....Uno called successfully...."<<endl;
				getch();
	 if (playturn == 1)
            unoPlayer1 = true;
        else if (playturn == 2)
            unoPlayer2 = true;

        return true;
	} else 
	{
			cout<<"Incorrect uno call"<<endl;
		cout<<"....Drawing two cards...."<<endl;
			getch();
		for(int i=0 ; i<2 ; i++) {
			drawncards++;
			bool cardAssigned = false;
			for(int j=0 ; j<8 ; j++) {
				for(int k=0 ; k<15 ; k++) {
					if(deck[j][k]==drawncards) {  // if player calls uno before the 2nd last card, two cards will be assigned as penalty
						if(playturn==1)
							player1[j][k]=drawncards;
						else if(playturn==2)
							player2[j][k]=drawncards;
						deck[j][k]=0;
						cardAssigned = true;
						break;
					}
				}
				if (cardAssigned) break;
			}
		}
		return false;
	}
}

// Alishba's helper functions
void shuffleDeckDirectly(int target[ROWS][COLS]) {
    srand(time(0));

    // Clear the main deck and reshuffle discard pile directly into it
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            target[i][j] = 0; // Clear the deck first
        }
    }

    // Shuffle the discard pile directly into the deck
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (discardPile[i][j] != 0 && discardPile[i][j] != topcard) {
                int randRow, randCol;
                do {
                    randRow = rand() % ROWS;
                    randCol = rand() % COLS;
                } while (target[randRow][randCol] != 0); // Find an empty spot

                target[randRow][randCol] = discardPile[i][j];
                discardPile[i][j] = 0; // Clear the discard pile
            }
        }
    }
}
void reshuffleFromDiscard() {
    // Shuffle discard pile directly into the main deck, excluding the current topcard
    shuffleDeckDirectly(deck);
    cout << "Deck reshuffled successfully\n";
}
bool valid_draw() {
	int player_card_color;
	int top_card_color;
	int top_card_number;
	bool validdraw=true;
	for(int i=0 ; i<8 ; i++) {
		for(int j=0 ; j<15 ; j++) {
			if(staticdeck[i][j]==topcard) { // extract the top cards number and color from the static deck 
				top_card_color=i;
				top_card_number=j;
			}
		}
	}
	string tcstring=colortostring(top_card_color); // convert the color from number to string
	string pcstring;
	if(playturn==1) {
		for(int i=0 ; i<8 ; i++) {
			for(int j=0 ; j<15 ; j++) {
				if(player1[i][j]!=0 && player1[i][j]!=-1) {
					pcstring=colortostring(i);
					if(pcstring==tcstring || j==top_card_number || j==13 || j==14 )
						validdraw=false;
				}
			}
		}
	}                                       //if player has a playable card, the valid draw is false 
	if(playturn==2) {
		for(int i=0 ; i<8 ; i++) {
			for(int j=0 ; j<15 ; j++) {
				if(player2[i][j]!=0 && player2[i][j]!=-1) {
					pcstring=colortostring(i);
					if(pcstring==tcstring || j==top_card_number || j==13 || j==14 )
						validdraw=false;
				}
			}
		}
	}
	return validdraw;
}
string colortostring(int num) {
	string color;
	if(num==0 || num==1)
		color= "red";
	else if(num==2 || num==3)
		color= "green";
	else if(num==4 || num==5)
		color= "yellow";
	else if(num==6 || num==7)
		color= "blue";
	return color;
}