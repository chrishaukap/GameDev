#pragma once

#include <string.h>

#define MAX_NAME_LENGTH 16
#define NUM_HIGH_SCORES 10

struct HighScore
{
	char name[MAX_NAME_LENGTH];
	int high_score;
};



class CHighScores
{
public:
	CHighScores(void);
	~CHighScores(void);
	// if the score is high enough, inserts score into high_scores array
	// and returns true;
	// else returns false;
	int InsertHighScore( const char* in_name, int in_score );
	// retrieves the name and score at the given index
	bool HighScoreAtIndex( int in_index, char* out_name, int& out_score );
	int GetNumScores(){return num_scores;}
	//void SetNumScores(int num_scores){this->num_scores = num_scores; }

	// computes the score given the number of rows that were completed
	int ComputeScore( int rows, int cols_in_row );

	// determines if the passed in score is high enough to be included in the top 10
	// if score is too low, returns -1, else returns the rank (1-10) of the score.
	bool ScoreInTopTen( int in_score );
private:
	int num_scores;
	HighScore high_scores[NUM_HIGH_SCORES];
	// recursively moves all lower scores further down the array
	// until the lowest score is dropped from the array entirely
	void Cascade( int index, const char* name, int score );
	
};
