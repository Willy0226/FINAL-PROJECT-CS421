#include <iostream>
#include <fstream>
#include <string>
#include "parser.cpp"
using namespace std;

/* INSTRUCTION:  copy your parser.cpp here
      cp ../ParserFiles/parser.cpp .
   Then, insert or append its contents into this file and edit.
   Complete all ** parts.
*/

//=================================================
// File translator.cpp written by Group Number: 2
//=================================================

// ----- Additions to the parser.cpp ---------------------

// ** Declare Lexicon (i.e. dictionary) that will hold the content of lexicon.txt
// Make sure it is easy and fast to look up the translation.
// Do not change the format or content of lexicon.txt
//  Done by: Issac Ferrel

// ** Additions to parser.cpp here:
//    getEword() - using the current saved_lexeme, look up the English word
//                 in Lexicon if it is there -- save the result
//                 in saved_E_word
//  Done by: Issac Ferrel
//    gen(line_type) - using the line type,
//                     sends a line of an IR to translated.txt
//                     (saved_E_word or saved_token is used)
//  Done by: Ting Wei Chien(Willy)

// ----- Changes to the parser.cpp content ---------------------

// ** Comment update: Be sure to put the corresponding grammar
//    rule with semantic routine calls
//    above each non-terminal function

// ** Each non-terminal function should be calling
//    getEword and/or gen now.

// ---------------- Driver ---------------------------

// The final test driver to start the translator
// Done by:  Christian Tobin
int main()
{
  //necessary do not delete look at setup_table for details.
  setup_table();
  //** opens the lexicon.txt file and reads it into Lexicon
  dictionary();
  //** closes lexicon.txt
  //** opens the output file translated.txt

  cout << "Enter the input file name: ";
  string filename;
  cin >> filename;
  fin.open(filename.c_str());

  //** calls the <story> to start parsing
  //** calls the <story> to start parsing
  story();
  cout << "Successfully parsed <story>." << endl;
  //** closes the input file
  fin.close();
  //** closes the input file
  //** closes traslated.txt

} // end
//** require no other input files!
//** syntax error EC requires producing errors.txt of error messages
//** tracing On/Off EC requires sending a flag to trace message output functions
