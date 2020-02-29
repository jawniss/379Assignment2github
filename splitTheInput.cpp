#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdio.h>

#include "splitTheInput.h"

using namespace std;

//------------------------------------------------------------------------------------
/*
How to take input and split it into a string vector, user "kev"
https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string
*/
vector<string> splitInputCommand( string inputcommand )
{
  string buffer; // Have a buffer string
  stringstream ss( inputcommand ); // Insert the string into a stream
  vector<string> splitinput; // Create vector to hold our words

  while ( ss >> buffer )
  {
    splitinput.push_back( buffer );
  }

  return splitinput;
}
//------------------------------------------------------------------------------------
