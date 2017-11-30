/*****************scanner.cpp**************/
//Author: Aaron Brunette, Erik Leung, Paul Rowe
//Last updated: 2017/11/30
//Compiled with g++
//Written on vim & visual studio
//Purpose: To scan a file of romanji words and check if they are
// valid japanese words.
//
//	 	MIT License
/***************************************/

//=====================================================
// File scanner.cpp written by: Group Number: 3 
//=====================================================

#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
using namespace std;

//Enumerated token types
enum tokentype{
ERROR, WORD1, WORD2, PERIOD, 
VERB, VERBNEG, VERBPAST, VERBPASTNEG, 
IS, WAS, OBJECT, SUBJECT, DESTINATION, 
PRONOUN, CONNECTOR, EOFM};

//global file streams
fstream toRead;

//reservedwords array initialization
const int arraySize = 38;
string reservedwords[arraySize] = {
"masu", "VERB", "masen", "VERBNEG", "mashita", "VERBPAST", 
"masendeshita", "VERBPASTNEG", "desu", "IS", "deshita", "WAS", 
"o", "OBJECT", "wa", "SUBJECT", "ni", "DESTINATION", 
"watashi", "PRONOUN", "anata", "PRONOUN", "kare", "PRONOUN", 
"kanojo", "PRONOUN", "sore", "PRONOUN", "mata", "CONNECTOR", 
"soshite", "CONNECTOR", "shikashi", "CONNECTOR", 
"dakara", "CONNECTOR", "eofm", "EOFM"};

//enum to string
string conversion[16] = {
"ERROR", "WORD1", "WORD2", "PERIOD", 
"VERB", "VERBNEG", "VERBPAST", "VERBPASTNEG", 
"IS", "WAS", "OBJECT", "SUBJECT", "DESTINATION", 
"PRONOUN", "CONNECTOR", "EOFM" };

//word checker prototype
int scanner(tokentype&, string&);
void dictionary(tokentype&, string);
bool startstate(string);
bool vowels(string, int&);
bool consonants(string, int&);
bool sRoot(string, int&);
bool zRoot(string, int&);
bool jRoot(string, int&);
bool tRoot(string, int&);
bool dRoot(string, int&);
bool cRoot(string, int&);
bool wRoot(string, int&);
bool yRoot(string, int&);

// The test driver to call the scanner repeatedly  
// ** Done by:  Aaron & Erik
int main()
{
  	tokentype thetype;	//hole the type of the word being scanned
  	string theword; 	//hold the word being scanned
	int eof = 1;		//eof int

	string inputfile;	//user input filename
	printf("Please enter the name of file to be scanned: ");
	getline(cin, inputfile);

	toRead.open(inputfile.c_str());	//open user input file

	if(!toRead.is_open())	//file failed to open
	{
		cout << inputfile << " failed to open.\nEnding program...\n";
		exit(EXIT_FAILURE);
	}

	while (true)	//while eof has not been reached
	{
		eof = scanner(thetype, theword);  // call the scanner

		if(eof == -1)
			return 0;
	
		//optimize this with operator overload
		if(thetype == ERROR)	
		{
			cout << "Lexical error: " << theword << " is not a valid token" << endl;
			return 1;
		}
		cout << "Type is: " << conversion[thetype-1] << endl;
		cout << "Word is: " << theword << endl << endl;   
    	}
	toRead.close();		//close user input file
}//the end

// Scanner processes only one word each time it is called
// ** Done by: Aaron & Erik
int scanner(tokentype& a, string& w)
{
  	toRead >> w;  //read word

	bool result = true;	//default, word is assumed valid

	if(w == "eofm")		//if end of file
		return -1;	
	else if(w == ".")	//period calls period DFA
		a = PERIOD;
 
	result = startstate(w);		//enter DFA

	if(result == false)	//result of DFA is false, word is invalid
		a = ERROR;
	else	//word is valid
	{
		if(isupper(w[w.size()-1]))	//check last character in word for Uppercase
		{
			a = WORD2;
		}	
		else				//not uppercase so default WORD1, check against reservedwords.txt
		{
			a = WORD1;
			dictionary(a, w);
		}
	}
}//the end

//Done by: Aaron & Erik
void dictionary(tokentype &a, string w)
{	
	//while reservedwords can read in
	for(int i; i < arraySize; i++)
	{
		if(reservedwords[i] == w)
		{
			i++;
			for(int step = 0; step < 16; step++)
			{
				if(conversion[step] == reservedwords[i])	//if reservedtype == giventype
				{
					
					a = static_cast<tokentype>(step);	//set type to array position, convert int to enum
					return;
				} 
			}
		}i++;
	}
}

bool startstate(string w)	//also final state
{
	int charpos = 0;
	bool result = true;		//result of going through bools

	while(w[charpos] != '\0')
	{
		switch(w[charpos])
		{
			case 'a': case 'i': case 'u': case 'e': case 'o':
				result = vowels(w, charpos);
				break;
			case 'k': case 'n': case 'h': case 'm': case 'r': case 'g': case 'b': case 'p':
				result = consonants(w, charpos);
				break;
			case 's':
				result = sRoot(w, charpos);
				break;
			case 'z':
				result = zRoot(w, charpos);
				break;
			case 'j':
				result = jRoot(w, charpos);
				break;
			case 't':
				result = tRoot(w, charpos);
				break;
			case 'd':
				result = dRoot(w, charpos);
				break;
			case 'c':
				result = cRoot(w, charpos);
				break;
			case 'w':
				result = wRoot(w, charpos);
				break;
			case 'y':
				result = yRoot(w, charpos);
				break;
			default:		//invalid character
				return false;
		}
		if(w[charpos] == 'E' || w[charpos] == 'I')	//WORD2 check
			return true;
		if(result == false)				//failed in bools
			return false;
	}
	return true;
}

//Bools, done by: Paul & Aaron
//check vowels
bool vowels(string w, int& charpos)
{
	int state = 0;
	
	if(w[charpos + 1] == 'n')	//Vn
	{
		charpos++; charpos++;
		return true;
	}
	else
	{
		charpos++;
		return true;
	}
}

//check consonants
bool consonants(string w, int& charpos)
{	
	int state = 0;
	charpos++;

	if(state == 0 && (w[charpos] == 'a' || w[charpos] == 'i' || w[charpos] == 'u' || w[charpos] == 'e' || w[charpos] == 'o'))	//CV
	{	
		state = 2;
		charpos++;
	}
	if(state == 0 && w[charpos] == 'y')	//Cy
	{
		state = 1;
		charpos++;
	}
	if(state == 1 && (w[charpos] == 'a' || w[charpos] == 'u' || w[charpos] == 'o')) //CyV
	{
		state = 2;
		charpos++;
	}
	if(state == 2 && w[charpos] == 'n')	//-n
	{
		charpos++;
		state = 3;
	}
  	if(state == 2 || state == 3) 
		return true;
   	else 
		return false;
}

bool sRoot(string w, int& charpos)
{
	int state = 0;
	charpos++;
	
	if(state == 0 && w[charpos] == 'h')	//sh
	{
		state = 1;
		charpos++;
	}
	if(state == 1 && (w[charpos] == 'a' || w[charpos] == 'i' || w[charpos] == 'u' || w[charpos] == 'o'))	//sha shi shu sho
	{
		state = 2;
		charpos++;
	}
	if(state == 0 && (w[charpos] == 'a' || w[charpos] == 'u' || w[charpos] == 'e' || w[charpos] == 'o'))	//sa su se so
	{
		state = 2;
		charpos++;
	}
	if(state == 2 && w[charpos] == 'n')	//-n
	{
		state = 3;
		charpos++;
	}
	if(state == 2 || state == 3)
		return true;
	else
		return false;
}

bool zRoot(string w, int& charpos)
{
	int state = 0;
	charpos++;

	if(state == 0 && (w[charpos] == 'a' || w[charpos] == 'u' || w[charpos] == 'e' || w[charpos] == 'o'))	//sa su se so
	{
		state = 1;
		charpos++;
	}
	if(state == 1 && w[charpos] == 'n')	//-n
	{
		state = 2;
		charpos++;
	}
	if(state == 1 || state == 2)
		return true;
	else
		return false;
}

bool jRoot(string w, int& charpos)
{
	int state = 0;
	charpos++;

	if(state == 0 && w[charpos] == 'i')	//ji
	{
		state = 1;
		charpos++;
	}
	if(state == 1 && w[charpos] == 'n')	//jin
	{
		state = 2;
		charpos++;
	}
	if(state == 1 || state == 2)
		return true;
	else 
		return false;
}

bool tRoot(string w, int& charpos)
{
	int state = 0;
	charpos++;
	
	if(state == 0 && w[charpos] == 's')	//ts
	{
		state = 1;
		charpos++;
	}
	if(state == 1 && w[charpos] == 'u')	//tsu
	{
		state = 2;
		charpos++;
	}
	if(state == 0 && (w[charpos] == 'a' || w[charpos] == 'e' || w[charpos] == 'o'))	//ta te to
	{
		state = 2;
		charpos++;
	}
	if(state == 2 && w[charpos] == 'n')	//-n
	{
		state = 3;
		charpos++;
	}
	if(state == 2 || state == 3)
		return true;
	else
		return false;
}

bool dRoot(string w, int& charpos)
{
	int state = 0;
	charpos++;

	if(state == 0 && (w[charpos] == 'a' || w[charpos] == 'e' || w[charpos] == 'o'))	//da de do
	{
		state = 1;
		charpos++;
	}
	if(state == 1 && w[charpos] == 'n')
	{
		state = 2;
		charpos++;
	}
	if(state == 1 || state == 2)
		return true;
	else
		return false;
}

bool cRoot(string w, int& charpos)
{
	int state = 0;
	charpos++;
	
	if(state == 0 && w[charpos] == 'h')	//ch
	{
		state = 1;
		charpos++;
	}
	if(state == 1 && w[charpos] == 'i')	//chi
	{
		state = 2;
		charpos++;
	}
	if(state == 2 && w[charpos] == 'n')	//-n
	{
		state = 3;
		charpos++;
	}

	if(state == 2 || state == 3)
		return true;
	else
		return false;
}

bool wRoot(string w, int& charpos)
{
	int state = 0;
	charpos++;

	if(state == 0 && w[charpos] == 'a')	//wa
	{
		state = 1;
		charpos++;
	}
	if(state == 1 && w[charpos] == 'n')	//wan
	{
		state = 2;
		charpos++;
	}
	if(state == 1 || state == 2)
		return true;
	else
		return false;
}

bool yRoot(string w, int& charpos)
{
	int state = 0;
	charpos++;
	
	if(state == 0 && (w[charpos] == 'a' || w[charpos] == 'u' || w[charpos] == 'o'))	//ya yu yo
	{
		state = 1;
		charpos++;
	}
	if(state == 1 && w[charpos] == 'n')	//yan yun yon
	{
		state = 2;
		charpos++;
	}
	if(state == (1 || 2))
		return true;
	else
		return false;
}
