#include <stdio.h>
#include <stdlib.h>
#include <time.h>



struct card {
	int card_value;
	char card_name;
	char card_suit;
};

struct card cards[52];
char turnP;
char turnD;

void setGame(){
	/*
	Establishing playing cards
	*/
	char suits[4] = {'S', 'C', 'D', 'H'};
	for(int s = 0; s < 4; s++){
		for (int i = 0; i < 13; i++){
			cards[(s*13)+i].card_value = i+1;		
			if (i+1 == 10){
				cards[(s*13)+i].card_name = 'T';
			}else if(i+1 == 11){
				cards[(s*13)+i].card_name = 'J';
			}else if(i+1 == 12){
				cards[(s*13)+i].card_name = 'Q';
			}else if(i+1 == 13){
				cards[(s*13)+i].card_name = 'K';
			}else if(i+1 == 1){
				cards[(s*13)+i].card_name = 'A';
			}else
				cards[(s*13)+i].card_name = (i+1) + '0';	
			cards[(s*13)+i].card_suit = suits[s];
		}	
	}

	/*
	for(int i = 0; i < 52; i++){
		printf("Value: %d, Card: %c, Suit: %c)\n", cards[i].card_value, cards[i].card_name, cards[i].card_suit);
	}
	*/

	
		
}

struct card drawCard(){
	bool t = true;
	struct card tmp;
	while(t){
		
		int ran = rand() % 52;
		if (cards[ran].card_value != 0){
			t = false;
			tmp = cards[ran];
			cards[ran].card_value = 0;
		}
	}
	
	return tmp;
	
	

}

int cardCount(struct card hands[20]){
	int count = 0;
	int Aces = 0;
	for (int i = 0; hands[i].card_value != 0; i++){
		if (hands[i].card_name == 'A')
			Aces++;
		count = count + hands[i].card_value;
	}
	if (count > 21 && Aces > 0){
		count = count - Aces * 10;
	}
	return count;
}	

void dispCards(struct card hands[20]){
	printf("Player: \n");
	for (int i = 0; i < 20; i++){
		if (hands[i].card_value != 0)
			printf("[%c%c] ", hands[i].card_name, hands[i].card_suit);
		else
			i=20;
	}
	printf("\n");
}


int player(struct card hands[20]){	
	char c;
	struct card newCard;
	int pCount;
	printf("What would you like to do? (H, S)\n");
	scanf(" %c", &c);
	dispCards(hands);
	if (c == 'H'){
		newCard = drawCard();
		printf("Player Drew Card: %c  Suit: %c\n", newCard.card_name, newCard.card_suit);
		for (int i = 0; i < 20; i++){
			if (hands[i].card_value == 0){
				hands[i] = newCard;
				i = 20;
			}
		}
		dispCards(hands);				
	}else
		turnP = 'S';
	return cardCount(hands);
}



int dealer(struct card hands[20]){
	struct card newCard;	

	if (cardCount(hands) <= 12){
		newCard = drawCard();
		for (int i = 0; i < 20; i++){
			if (hands[i].card_value == 0){
				hands[i] = newCard;
				i=20;
			}
		}
		turnD = 'H';
		printf("Dealer Drew A Card\n");
	}else if (cardCount(hands) <=17){
		bool chance = rand() % 2;
		if (chance){
			newCard = drawCard();
			for (int i = 0; i < 20; i++){
				if(hands[i].card_value == 0){
					hands[i] = newCard;
					i = 20;
				}
			}
		}
		turnD = 'H';
		printf("Dealer Drew A Card\n");
	}else{
		turnD = 'S';
		printf("Dealer Stayed\n");
	}

	printf("Dealer :\n");
	for(int i =0; i < 20; i++){
		if (hands[i].card_value != 0)
			printf("[%c%c] ", hands[i].card_name, hands[i].card_suit);
		else
			i = 20;
	}
	printf("\n");
	

	printf("Dealer's Hand: %d\n", cardCount(hands));
	
	return cardCount(hands);	

}


int check(int dealer, int player, int balance){
	if (player > 21 && dealer <= 21 && dealer != 0){
		printf("Bust! Dealer Won This.\n");
		return balance;
	}else if (((dealer > 21 && player <21) || (player < 21 &&  player > dealer)) && dealer != 0 && player != dealer){
		printf("Won!\n");
		return (balance + 100);
	}else if (player == 21 && (dealer < 21 || dealer > 21) && dealer != 0){
		printf("Black Jack!\n");
		return (balance + 100);
	}else if (player == dealer|| (player > 21 && dealer > 21)){
		printf("Split Pot, Draw\n");		
		return (balance + 50);
	}else if (dealer > player && dealer != 0){
		printf("Dealer Won\n");
		return balance;
	}

}




int playGame(){
	int balance = 500;
	bool inProg = true;
	bool game = true;
	char end;
	struct card phand[20];
	int condition;
	//printf("Test Card: %d", phand[1].card_value);
	struct card dhand[20];
	int playerC, dealerC;
	srand (time(NULL));	
	while(game){
		if (balance < 50){
			printf("You're Broke! Game Over");
			break;
		}else
			balance -= 50;

		setGame();
		for (int i = 0; i < 20; i++){
			phand[i].card_value = 0;
			dhand[i].card_value = 0;
		}

		for (int i = 0; i < 2; i++){
			phand[i] = drawCard();
			dhand[i] = drawCard();

		}
		dispCards(phand);
		while (inProg){
			playerC = player(phand);
			dealerC = dealer(dhand);
			if ((turnP == 'S' && turnD == 'S') || (playerC > 21 || dealerC > 21)){
				balance = check(dealerC, playerC, balance);
				printf("New Balance: %d\n", balance);
				inProg = false;
			}
		}
		printf("Continue? (50 Credits)\n");
		scanf(" %c", &end);
		if (end != 'Y')
			game = false;
		else
			inProg = true;
	}
}



int main(){
	char name[20];
	
	printf("Welcome to Black Jack v1.0. You'll be playing against the Dealer. \n");
	printf("Enter your name(Keep it below 20 characters): \n");
	scanf(" %s", name);
	printf("Hello %s. Enjoy your stay.\n", name);
 
	playGame();

	return 0;
}

