#include "highscores.h"

CHighScores::CHighScores(void)
{
	this->num_scores = 0;
	for( int i=0; i<NUM_HIGH_SCORES; ++i )
	{
		this->high_scores[i].high_score = 0;
	}
}

CHighScores::~CHighScores(void)
{
}


///////////////////////////////////////////////////////////////////////////////////////////
//
// inserts the score into the highscores list.
// returns -1 if the score is too low or if the passed-in name is too long
// Otherwise returns the rank of the score (1-10)
//
//
///////////////////////////////////////////////////////////////////////////////////////////
int CHighScores::InsertHighScore( const char* name, int score )
{
	// check size of name
	if( MAX_NAME_LENGTH < strlen( name ) || score < this->high_scores[9].high_score )
		return -1;

	if( ++num_scores > NUM_HIGH_SCORES ) 
		num_scores = NUM_HIGH_SCORES;

	// locate the area where score should be inserted. then iteratively
	// move lower scores down the array.
	for( int i=0; i< NUM_HIGH_SCORES; ++i )
	{
		if( score >= this->high_scores[i].high_score )
		{
			this->Cascade(i, name, score);
			return i+1;  // return the rank of the inserted score
		}
	}
	return -1;
}


///////////////////////////////////////////////////////////////////////////////////////////
//
// moves the lower scores down the array one step (the lowest score falls off the array)
//
///////////////////////////////////////////////////////////////////////////////////////////
void CHighScores::Cascade( int index, const char* name, int score )
{
	// base case, we have reached the end of the array
	if( index >= NUM_HIGH_SCORES || score == 0)
		return;

    // recursively move the next one down.
	Cascade( index+1, this->high_scores[index].name, this->high_scores[index].high_score );
	
	// insert the new item
	strcpy( this->high_scores[index].name, name );
	this->high_scores[index].high_score = score;

}

///////////////////////////////////////////////////////////////////////////////////////////
//
// returns the score listed at the input index
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CHighScores::HighScoreAtIndex( int in_index, char* out_name, int& out_score )
{
	// if the index is outside the bounds of the array, return false
	if( in_index < 0 || in_index >= NUM_HIGH_SCORES )
		return false;

	// retrieve the high score at index;
	strcpy( out_name, this->high_scores[in_index].name );
	out_score = this->high_scores[in_index].high_score;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// computes the score given the number of rows that were simultatenously
// completed.  
//
///////////////////////////////////////////////////////////////////////////////////////////
int CHighScores::ComputeScore( int rows, int cols_in_row )
{
	int row_value = rows*cols_in_row;
	return row_value*row_value;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// Determines if the passed in score is high enough to be included in the top 10.
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CHighScores::ScoreInTopTen( int in_score )
{
	// if the score is less than the lowest score in the top 10, return -1
	if( in_score < this->high_scores[9].high_score )
		return false;
	return true;
}