#include <stdlib.h>

#include "scanner.cpp"
#include <unordered_map>
using namespace std;
//prototyping
void s();
void story();
void after_subject();
void after_object();
void after_noun();
void verb();
void tense();
void be();
void noun();

/* INSTRUCTION:  Complete all ** parts.
      You may use any method to connect this file to scanner.cpp
         that you had written.
	   e.g. You can copy scanner.cpp here by:
	             cp ../ScannerFiles/scanner.cpp .
		            and then append the two files into one:
			              cat scanner.cpp parser.cpp > myparser.cpp
*/

//=================================================
// File parser.cpp written by Group Number: 2
//=================================================

string saved_lexeme = "";

tokentype saved_token = EOFM;

bool token_available = false;

string saved_E_word;//this is the saved english word

unordered_map<string,string> lexicon;//to store the definitions of japanese to english from lexcion.txt

ofstream file("traslated.txt");//file where will send our output
//ofstream translation= "translation.txt";
// ----- Four Utility Functions and Globals -----------------------------------

// ** Need syntaxerror1 and syntaxerror2 functions (each takes 2 args)
//    to display syntax error messages as specified by me.


//PURPOSE:to store the definitions from the file lexicon.txt
//PARAMETER: map lexicon will be used as a hashed map in order to store the values
void translations(){
  ifstream fin ("lexicon.txt", ios::in);
  string JAPword;
  string ENGword;
  while(fin>>JAPword)
    {
      fin>>ENGword;
      lexicon[JAPword]=ENGword;}

    unordered_map<string, string>:: iterator itr;
    /*to print the map
    for (itr = lexicon.begin(); itr != lexicon.end(); itr++)
        {
        cout << itr->first << "\t" << itr->second << endl;
        }*/    
}

//this will generate the english word from save_E_word or
//it will do so from saved_token
//PURPOSE:to send translation to translated.txt
//PARAMETER: saved_Eword a string and saved_toke of type tokentype
void gen(string line_type)
{
  if(line_type!="TENSE"){
    file<<line_type<<" "<<saved_E_word<<endl;
    }
  else{file<<line_type<<" "<<tokenName[saved_token]<<endl<<endl;}
}
//just to get the english word to the map we haved created from lexicon
//if the map does not march we use the saved_lexeme
void getEword(){
  
  unordered_map<string, string>:: iterator itr;
  itr=lexicon.find(saved_lexeme);
  //
  if(itr != lexicon.end()) { saved_E_word = itr->second; } else {saved_E_word = saved_lexeme;}   
}

// Type of error: Bad token
// Done by: Issac Ferrel
void syntaxerror1(tokentype expected, string save_lexeme)
{
  cout << "SYNTAX ERROR: expected " <<tokenName[expected] << ", but found " << save_lexeme << endl;
  /*ofstream errors;
  errors.open("errors.txt");
  errors << "SYNTAX ERROR: expected " <<tokenName[expected] << ", but found " << save_lexeme << endl;
  errors.close();
  cout<<"replace/skip token? Y/N: ";
  char response;
  cin>>response;
  if(response=='Y'||'y')
    {
      if(tokenName[expected]=="PERIOD")
	{
	  token_available=false;
	  match(expected);
	}
      else
	{
	  cout<<"replace with: ";
	  cin>>saved_lexeme;
	}
	}*/
  // else{
exit(1);
//} // exit
}

// Type of error: Not the correct word
// Done by: Issac Ferrel
void syntaxerror2(string save_lexeme, string parseFunction)
{
  cout << "SYNTAX ERROR: unexpected " << save_lexeme << ", found in " << parseFunction << endl;
  exit(1); // exit
}

// ** Need the updated match and next_token with 2 global vars
// saved_token and saved_lexeme

// Purpose: information for next token
// Done by: Ting Wei Chien(Willy)
tokentype next_token()
{
  if (!token_available)
    {
      //cout << "BEFORE: " << tokenName[saved_token] << endl;
      scanner(saved_token, saved_lexeme);
       cout << "Scanner called using word " << "\"" + saved_lexeme + "\"" << endl;
      //cout << "AFTER: " << tokenName[saved_token] << endl;
      token_available = true;
    }
  return saved_token;
}

// Purpose: bool function for match type when it succeeds
// Done by: Ting Wei Chien(Willy)
bool match(tokentype expected)
{
  if (next_token() != expected)
    {
      syntaxerror1(expected, saved_lexeme);
      return false;
    }
  else
    {
      cout << "matched " << tokenName[expected] << endl;
      token_available = false;
      return true;
    }
}

/// ----- RDP functions - one per non-term -------------------

// ** Make each non-terminal into a function here
// ** Be sure to put the corresponding grammar rule above each function
// ** Be sure to put the name of the programmer above each function

// Grammar: <story> ::= <s> { <s> }
// Done by: Issac Ferrel
void story()
{
  s();
  while (true)
    {
      switch (next_token())
	{
	case CONNECTOR:
	case WORD1:
	case PRONOUN:
	  s();
	  break;
	default:
	  return;
	}
    }
}
// Grammar:<s> ::= [CONNECTOR] <noun> SUBJECT <after subject>
// Grammar updated:<s>::=[CONNECTOR #getEword# #gen(CONNECTOR)#]<noun> #getEword# SUBJECT #gen(ACTOR)# <after subject>
// Done by: Issac Ferrel
void s()
{
  if (next_token() == CONNECTOR)
    {
      match(CONNECTOR);
      getEword();
      gen("CONNECTOR");
    }
  noun();
  getEword();
  match(SUBJECT);
  gen("ACTOR");
  after_subject();
}

// Grammar:<after subject> ::= <verb> <tense> PERIOD | <noun> <after noun>
// Grammar updated:<after subject> ::= <verb>  #getEword# #gen(ACTION)# <tense> #gen(TENSE)# PERIOD | <noun>  #getEword#  <after noun>
// Done by: Ting Wei Chien(Willy)
void after_subject()
{
  switch (next_token())
    {
    case WORD2:
      verb();
      getEword();
      gen("ACTION");
      tense();
      gen("TENSE");
      match(PERIOD);
      break;
    case WORD1:
    case PRONOUN:
      noun();
      getEword();
      after_noun();
      break;
    default:
      syntaxerror2(saved_lexeme, "after_subject");
    }
}
// Grammar:<after noun> ::= <be> PERIOD|DESTINATION <verb><tense>PERIOD|OBJECT<after object>
// Grammar updated:<after noun> ::= <be>#gen(DESCRIPTION)##gen(TENSE)# PERIOD|
//DESTINATION #gen(TO)# <verb> #getEword# #gen(ACTION)# <tense> #gen(TENSE) PERIOD| 
//OBJECT #gen(Object)# <after object>
// Done by: Ting Wei Chien(Willy)
void after_noun()
{
  switch (next_token())
    {
    case IS:
    case WAS:
      be();
      gen("DESCRIPTION");
      gen("TENSE");
      match(PERIOD);
      break;
    case DESTINATION:
      match(DESTINATION);
      gen("TO");
      verb();
      getEword();
      gen("ACTION");
      tense();
      gen("TENSE");
      match(PERIOD);
      break;
    case OBJECT:
      match(OBJECT);
      gen("OBJECT");
      after_object();
      break;
    default:
      syntaxerror2(saved_lexeme, "after_noun");
    }
}
// Grammar:<afterobject> ::=<verb><tense>PERIOD|<noun>DESTINATION<verb><tense>PERIOD
// Grammar updated:<afterobject> ::=<verb> #getEword# #gen(ACTION)# <tense> #gen(TENSE)# PERIOD|
//<noun>#getEword#DESTINATION #gen(TO)# <verb> #getEword# #gen(ACTION)# <tense> #gen(TENSE)# PERIOD
// Done by: Issac Ferrel
void after_object()
{
  switch (next_token())
    {
    case WORD2:
      verb();
      getEword();
      gen("ACTION");
      tense();
      gen("TENSE");
      match(PERIOD);
      break;
    case WORD1:
    case PRONOUN:
      noun();
      getEword();
      match(DESTINATION);
      gen("TO");
      verb();
      getEword();
      gen("ACTION");
      tense();
      gen("TENSE");
      match(PERIOD);
      break;
    default:
      syntaxerror2(saved_lexeme, "after_object");
    }
}
// Grammar:<noun> ::= WORD1 | PRONOUN
// Done by: Ting Wei Chien(Willy)
void noun()
{
  switch (next_token())
    {
    case WORD1:
      match(WORD1);
      break;
    case PRONOUN:
      match(PRONOUN);
      break;
    default:
      syntaxerror2(saved_lexeme, "noun");
    }
}
// Grammar:<verb> ::= WORD2
// Done by: Issac Ferrel
void verb()
{
  match(WORD2);
}
// Grammar:<be> ::=   IS | WAS
// Done by: Christian Tobin
void be()
{
  switch (next_token())
    {
    case IS:
      match(IS);
      break;
    case WAS:
      match(WAS);
      break;
    default:
      syntaxerror2(saved_lexeme, "be");
    }
}
// Grammar:<tense> := VERBPAST  | VERBPASTNEG | VERB | VERBNEG
// Done by: Christian Tobin
void tense()
{
  switch (next_token())
    {
    case VERBPAST:
      match(VERBPAST);
      break;
    case VERBPASTNEG:
      match(VERBPASTNEG);
      break;
    case VERB:
      match(VERB);
      break;
    case VERBNEG:
      match(VERBNEG);
      break;
    default:
      syntaxerror2(saved_lexeme, "tense");
    }
}

//string filename;

//----------- Driver ---------------------------

// The new test driver to start the parser
// Done by:  Christian Tobin
/*int main()
{
  setup_table();
  cout << "Enter the input file name: ";
  cin >> filename;
  fin.open(filename.c_str());

  //** calls the <story> to start parsing
  story();
  cout<<"Successfully parsed <story>."<<endl;
  //** closes the input file
  fin.close();
  }*/
// end
//** require no other input files!
//** syntax error EC requires producing errors.txt of error messages
//** tracing On/Off EC requires sending a flag to trace message output functions
