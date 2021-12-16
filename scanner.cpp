#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;
// ** Update the tokentype to be WORD1, WORD2, PERIOD, ERROR, EOFM, etc.
enum tokentype
  {
    ERROR,
    WORD1,
    WORD2,
    PERIOD,
    VERB,
    VERBNEG,
    VERBPAST,
    VERBPASTNEG,
    IS,
    WAS,
    OBJECT,
    SUBJECT,
    DESTINATION,
    PRONOUN,
    CONNECTOR,
  EOFM
  };

bool word(string);
bool period(string);
void setup_table();
int scanner(tokentype &, string &);

ifstream fin;
string tokenName[30] = {"ERROR", "WORD1", "WORD2", "PERIOD", "VERB", "VERBNEG", "VERBPAST", "VERBPASTNEG", "IS", "WAS", "OBJECT", "SUBJECT", "DESTINATION", "PRONOUN", "CONNECTOR", "EOFM"};
/* Look for all **'s and complete them */

//=====================================================
// File scanner.cpp written by: Group Number: Christian Tobin, Issac Ferrel, Ting Wei Chien(Willy)
//=====================================================

// --------- Two DFAs ---------------------------------

// WORD DFA
// Done by: Ting Wei Chien(Willy) and Christian Tobin
// RE: (V | dwzyj V | c h V | s V | s h V | t V | t h V | bghkmnpr V | bghkmnpr y V | V n | dwzyj V n | c h V n | s V n | s h V n | t V n | t h V n | bghkmnpr V n | bghkmnpr y V n)^+
// state 0 = q0, state 1 = q0q1, state 2 = qsa, state 3 = qy, state 4 = qt
// state 5 = qs, state 6 = qc, state 7 = q0qy
bool word(string s)
{

  string VOWELS = "aeiouIE";

  int state = 0;
  int charpos = 0;
  /* replace the following todo the word dfa  */
  while (s[charpos] != '\0')
    {
      char t;
      if (VOWELS.find(s[charpos]) != -1)
	t = 'V';
      else
	t = s[charpos];

      // q0
      if (state == 0)
	{
	  switch (t)
	    {
	    case 'V':
	      state = 1;
	      break;
	    case 'd':
	    case 'w':
	    case 'z':
	    case 'y':
	    case 'j':
	      state = 2;
	      break;
	    case 'b':
	    case 'g':
	    case 'h':
	    case 'k':
	    case 'm':
	    case 'p':
	    case 'r':
	    case 'n':
	      state = 3;
	      break;
	    case 't':
	      state = 4;
	      break;
	    case 's':
	      state = 5;
	      break;
	    case 'c':
	      state = 6;
	      break;
	    default:
	      return (false);
	    }
	} // end of q0

      // q0q1
      else if (state == 1)
	{
	  switch (t)
	    {
	    case 'V':
	      state = 1;
	      break;
	    case 'd':
	    case 'w':
	    case 'z':
	    case 'y':
	    case 'j':
	      state = 2;
	      break;
	    case 'b':
	    case 'g':
	    case 'h':
	    case 'k':
	    case 'm':
	    case 'p':
	    case 'r':
	      state = 3;
	      break;
	    case 't':
	      state = 4;
	      break;
	    case 's':
	      state = 5;
	      break;
	    case 'c':
	      state = 6;
	      break;
	    case 'n':
	      state = 7;
	      break;
	    default:
	      return (false);
	    }
	} // end of q0q1

      // qsa
      else if (state == 2)
	{
	  switch (t)
	    {
	    case 'V':
	      state = 1;
	      break;
	    default:
	      return (false);
	    }
	} //end of qsa

      // qy
      else if (state == 3)
	{
	  switch (t)
	    {
	    case 'V':
	      state = 1;
	      break;
	    case 'y':
	      state = 2;
	      break;
	    default:
	      return (false);
	    }
	} //end of qsa

      // qt
      else if (state == 4)
	{
	  switch (t)
	    {
	    case 'V':
	      state = 1;
	      break;
	    case 's':
	      state = 2;
	      break;
	    default:
	      return (false);
	    }
	} //end of qt

      // qs
      else if (state == 5)
	{
	  switch (t)
	    {
	    case 'V':
	      state = 1;
	      break;
	    case 'h':
	      state = 2;
	      break;
	    default:
	      return (false);
	    }
	} //end of qt

      // qc
      else if (state == 6)
	{
	  switch (t)
	    {
	    case 'V':
	      state = 1;
	      break;
	    case 'h':
	      state = 2;
	      break;
	    default:
	      return (false);
	    }
	} //end of qt

      // q0qy
      else if (state == 7)
	{
	  switch (t)
	    {
	    case 'V':
	      state = 1;
	      break;
	    case 'd':
	    case 'w':
	    case 'z':
	    case 'y':
	    case 'j':
	      state = 2;
	      break;
	    case 'b':
	    case 'g':
	    case 'h':
	    case 'k':
	    case 'm':
	    case 'p':
	    case 'r':
	    case 'n':
	      state = 3;
	      break;
	    case 't':
	      state = 4;
	      break;
	    case 's':
	      state = 5;
	      break;
	    case 'c':
	      state = 6;
	      break;
	    default:
	      return (false);
	    }
	} //end of q0qy

      //error case
      else
	{
	  cout << "I am stuck in state " << state << endl;
	  return (false);
	} //end of error case

      charpos++; //next char

    } //end of while

  // where did I end up????
  if (state == 0 || state == 1 || state == 7)
    return (true); // end in a final state
  else
    return (false);
}

// PERIOD DFA
// Done by: Ting Wei Chien(Willy)
bool period(string s)
{
  if (s[0] == '.')
    return (true);
  else
    return (false);
}
// ------ Three  Tables -------------------------------------

// TABLES Done by: Ting Wei Chien(Willy)

// ** Need the reservedwords table to be set up here.
// ** Do not require any file input for this. Hard code the table.
// ** a.out should work without any additional files.
// reference from https://www.cplusplus.com/reference/map/map/find/
map<string, tokentype> reservedWords;

// set up table function. //Copy from the reservedWords.txt
void setup_table()
{
  reservedWords["masu"] = VERB;
  reservedWords["masen"] = VERBNEG;
  reservedWords["mashita"] = VERBPAST;
  reservedWords["masendeshita"] = VERBPASTNEG;

  reservedWords["desu"] = IS;
  reservedWords["deshita"] = WAS;

  reservedWords["o"] = OBJECT;
  reservedWords["wa"] = SUBJECT;

  reservedWords["ni"] = DESTINATION;

  reservedWords["watashi"] = PRONOUN;
  reservedWords["anata"] = PRONOUN;
  reservedWords["kare"] = PRONOUN;
  reservedWords["kanojo"] = PRONOUN;
  reservedWords["sore"] = PRONOUN;

  reservedWords["mata"] = CONNECTOR;
  reservedWords["soshite"] = CONNECTOR;
  reservedWords["shikashi"] = CONNECTOR;
  reservedWords["dakara"] = CONNECTOR;

  reservedWords["eofm"] = EOFM;
}

// ------------ Scaner and Driver -----------------------
// Scanner processes only one word each time it is called
// Gives back the token type and the word itself
// ** Done by: Ting Wei Chien(Willy) and Christian Tobin
int scanner(tokentype &tt, string &w)
{
  // ** Grab the next word from the file via fin
  fin >> w;
  // 1. If it is eofm, return right now.
  if (w == "eofm")
  {
    tt = EOFM;
    return -1;
  }

  /* 
     2. Call the token functions (word and period) 
     one after another (if-then-else).
     Generate a lexical error message if both DFAs failed.
     Let the tokentype be ERROR in that case.
  */
  if (period(w))
  {
    tt = PERIOD;
  }

  else if (word(w))
  {

    if (reservedWords.find(w) != reservedWords.end())
    {
      tt = reservedWords.find(w)->second; // get the tokentype
    }
    else if (w[w.size() - 1] == 'I' || w[w.size() - 1] == 'E')
    {
      tt = WORD2;
    }
    else
    {
      tt = WORD1;
    }

    /*
    3. If it was a word,
     check against the reservedwords list.
     If not reserved, tokentype is WORD1 or WORD2
     decided based on the last character.
  */
  }

  else
  {
    tt = ERROR;
    cerr << "Lexical error: '" << w << "' is not a valid token" << endl;
  }

  /*
    4. Return the token type & string  (pass by reference)
  */
  return 0;

} //the end of scanner

// The temporary test driver to just call the scanner repeatedly
// This will go away after this assignment
// DO NOT CHANGE THIS!!!!!!
// Done by:  Rika
// int main()
// {
//   tokentype thetype;
//   string theword;
//   string filename;

//   setup_table();

//   cout << "Enter the input file name: ";
//   cin >> filename;

//   fin.open(filename.c_str());

//   // the loop continues until eofm is returned.
//   while (true)
//   {
//     scanner(thetype, theword); // call the scanner which sets
//                                // the arguments
//     if (theword == "eofm")
//       break; // stop now

//     cout << "Type is:" << tokenName[thetype] << endl;
//     cout << "Word is:" << theword << endl;
//   }

//   cout << "End of file is encountered." << endl;
//   fin.close();

// } // end
