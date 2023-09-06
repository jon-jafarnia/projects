/* 
q.c
Team 15
CS50 Fall 2022
*/

#include "q.h"

#define max_player_points 18		// 4 through 21
#define max_dealer_points 10		// 2 through 11
#define number_using_hard_ace 2		// true or false
#define number_actions 2			// HIT or STAND 


/**************** struct ****************/
typedef struct q {
	float averages[max_player_points][max_dealer_points][number_using_hard_ace][number_actions];
	int counts[max_player_points][max_dealer_points][number_using_hard_ace][number_actions];
} q_t;


/**************** local function declarations ****************/
static void insert_reward(q_t* q, hand_t* hand, int action, int reward);


/**************** public functions ****************/
q_t* q_new() {
	q_t* q = malloc(sizeof(q_t));
	for (int c = 0; c < number_using_hard_ace; c++) {
		for (int d = 0; d < number_actions; d++) {
			for (int a = 0; a < max_player_points; a++) {
				for (int b = 0; b < max_dealer_points; b++) { 
					q->averages[a][b][c][d] = 0.0;
					q->counts[a][b][c][d] = 0;
				}
			}
		}
	}
	return q;
}

void q_save(q_t* q, char* file_path) {
	if (q == NULL) {
		return;
	}
	FILE* fp = fopen(file_path, "w");
	if (fp == NULL) {
		fprintf(stderr, "Error. Could not open 'file_path' for writing.\n");
		return;
	}
	for (int c = 0; c < number_using_hard_ace; c++) {
		for (int d = 0; d < number_actions; d++) {
			for (int a = 0; a < max_player_points; a++) {
				for (int b = 0; b < max_dealer_points; b++) {
					fprintf(fp, "%f ", q->averages[a][b][c][d]);
					fprintf(fp, "%d ", q->counts[a][b][c][d]);
				}
				fprintf(fp, "\n");
			}
			fprintf(fp, "\n");
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

q_t* q_load(char* file_path) {
	q_t* q = q_new();
	FILE* fp = fopen(file_path, "r");
	if (fp == NULL) {
		fprintf(stderr, "Error. Could not open 'file_path' for reading.\n");
		return NULL;
	}
	for (int c = 0; c < number_using_hard_ace; c++) {
		for (int d = 0; d < number_actions; d++) {
			for (int a = 0; a < max_player_points; a++) {
				for (int b = 0; b < max_dealer_points; b++) { 
					fscanf(fp, "%f ", &q->averages[a][b][c][d]);
					fscanf(fp, "%d ", &q->counts[a][b][c][d]);
				}
				fscanf(fp, "\n");
			}
			fscanf(fp, "\n");
		}
		fscanf(fp, "\n");
	}
	fclose(fp);
	return q;
}

void q_record_hand(q_t* q, hand_t* hand, char* result) {
	if (hand == NULL) {
		return;
	}
	// get info from result
	bool busted = (strncmp(result, "BUST", 4) == 0);
	int reward;
	if (strncmp(result, "LOOSE", 5) == 0 || busted)	reward = -1;
	if (strncmp(result, "PUSH", 4) == 0)			reward = 0;
	if (strncmp(result, "WIN", 3) == 0)				reward = 1;
	// if player didn't bust, then they chose STAND after receiving their last card
	// if player busted, then they did not HIT or STAND after receiving their last card
	if (!busted) {
		insert_reward(q, hand, 0, reward);
	}
	// in either of the above cases, the last card in the hand has now been properly handed
	hand_remove_last_player_card(hand);

	// for every remaining situation, the player hit (every situation when there were >=2 cards)
	while (hand_get_number_player_cards(hand) >= 2) {
		insert_reward(q, hand, 1, reward);
		hand_remove_last_player_card(hand);
	}
}

int q_get_decision(q_t* q, hand_t* hand) {
	// if average for HIT and STAND are tied, return STAND
	int player_value = hand_get_player_value(hand);
	int dealer_value = hand_get_dealer_value(hand);
	int player_using_hard_ace = hand_is_player_using_hard_ace(hand);
	if (q->averages[player_value][dealer_value][player_using_hard_ace][1] > q->averages[player_value][dealer_value][player_using_hard_ace][0]) {
		return 1;
	}
	return 0;
}

void q_delete(q_t* q) {
	free(q);
}


/**************** local functions ****************/
static void insert_reward(q_t* q, hand_t* hand, int action, int reward) {
	int player_value_index = hand_get_player_value(hand) - 4;
	int dealer_value_index = hand_get_dealer_value(hand) - 2;
	int using_hard_ace = hand_is_player_using_hard_ace(hand);
	// update and store count for average computation
	int count = ++q->counts[player_value_index][dealer_value_index][using_hard_ace][action]; 	
	// update average
	q->averages[player_value_index][dealer_value_index][using_hard_ace][action] = 
			/* old average scaled down */ 	q->averages[player_value_index][dealer_value_index][using_hard_ace][action] * (count - 1) / count +
			/* new reward scaled down */ 	(double) reward / count;
}
