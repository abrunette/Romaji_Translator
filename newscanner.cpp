#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<vector>
using namespace std;

//God Bless this spaghetti mess

//=====================================================
// File scanner.cpp written by: Group Number: 3 
//=====================================================

//Enumerated token types
typedef enum tokentype {ERROR, WORD1, WORD2, PERIOD, VERB, VERBNEG, VERBPAST, VERBPASTNEG, IS, WAS, OBJECT, SUBJECT, DESTINATION, PRONOUN, CONNECTOR, EOFM};

vector <string> reservedWords;	//vector holds reserved words
vector <string> reservedTypes;	//hold reserved word types

bool dictionary(tokentype&, string);	//search dictionary for valid reserved words

// Period found, returns token
// ** Done by: Aaron & Erik
void period(tokentype& a)
{
	a = PERIOD;	//sets token
	return;
}

/*****************************************************/
//bools

bool vowels(string w, int& charpos)
{
	int state = 0;
	
	if(w[charpos + 1] == 'n')
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

bool consonants(string w, int& charpos)
{	
	int state = 0;
	charpos++;

	if(state == 0 && (w[charpos] == 'a' || w[charpos] == 'i' || w[charpos] == 'u' || w[charpos] == 'e' || w[charpos] == 'o'))	//cv
	{	
		state = 2;
		charpos++;
	}
	if(state == 0 && w[charpos] == 'y')	//Cy
	{
		state = 1;
		charpos++;
	}
	if(state == 1 && (w[charpos] == 'a' || w[charpos] == 'u' || w[charpos] == 'o'))
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
		return true;  // end in a final state
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

bool jRoot(string w, int& charpos)
{
	int state = 0;
	charpos++;

	if(state == 0 && w[charpos] == 'i')	//ji
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

bool startstate(string w)	//also final state
{
	int charpos = 0;
	bool result = true;		//result of going through bools

	while(w[charpos] != '\0')	//maybe switch statement
	{
		if(w[charpos] == 'a' || w[charpos] == 'i' || w[charpos] == 'u' || w[charpos] == 'e' || w[charpos] == 'o')	//vowels
			result = vowels(w, charpos);
		else 
		if(w[charpos] == 'k' || w[charpos] == 'n' || w[charpos] == 'h' || w[charpos] == 'm' || w[charpos] == 'r' || w[charpos] == 'g' || w[charpos] == 'b' || w[charpos] == 'p')
			result = consonants(w, charpos);
		else
		if(w[charpos] == 's')
			result = sRoot(w, charpos);
		else
		if(w[charpos] == 'z')
			result = zRoot(w, charpos);
		else
		if(w[charpos] == 'j')
			result = jRoot(w, charpos);
		else
		if(w[charpos] == 't')
			result = tRoot(w, charpos);
		else
		if(w[charpos] == 'd')
			result = dRoot(w, charpos);
		else
		if(w[charpos] == 'c')
			result = cRoot(w, charpos);
		else
		if(w[charpos] == 'w')
			result = wRoot(w, charpos);
		else
		if(w[charpos] == 'y')
			result = yRoot(w, charpos);
		else			//invalid character
		{
			cout << "invalid character..." << endl
				<< "character is: " << w[charpos] << endl;
			return false;	
		}

		/*cout << "charpos: " << charpos << endl;
		cout << "char: " << w[charpos] << endl;
		cout << "result: " << result << endl; */

		if(w[charpos] == 'I' || w[charpos] == 'E')
			return true;

		if(result == false)	//failed inside somewhere
		{
			cout << "failed inside bools..." << endl;
			return false;
		}
	}
	return true;
}

/*******************************************************************/

// Scanner processes only one word each time it is called
// ** Done by: Aaron & Erik
int scanner(tokentype& a, string& w)
{
	bool result = true;	//default, word is assumed valid
	int i = 0;
     	
	if(w == "eofm")
		return -1;	
	else if(w == ".")
	{
		period(a);
		return 1;
	} 
	result = startstate(w);
	if(result == false)
	{
		a = ERROR;
	}
	else	//valid
	{
		if(isupper(w[w.size()-1]))	//word2
		{
			a = WORD2;
		}
		else
		{
			a = WORD1;
			result = dictionary(a, w);
		}
	}
}//the end

//
//Done by: Aaron & Erik
bool dictionary(tokentype &a, string w)
{	
	for(int count = 0; count <= (reservedWords.size() - 1); count++)
	{
		if(reservedWords[count] == w)
		{
			if(reservedTypes[count] == "VERB")
				a = VERB;
			else if(reservedTypes[count] == "VERBNEG")
				a = VERBNEG;
			else if(reservedTypes[count] == "VERBPAST")
				a = VERBPAST;
			else if(reservedTypes[count] == "VERBPASTNEG")
				a = VERBPASTNEG;
			else if(reservedTypes[count] == "IS")
				a = IS;
			else if(reservedTypes[count] == "WAS")
				a = WAS;
			else if(reservedTypes[count] == "OBJECT")
				a = OBJECT;
			else if(reservedTypes[count] == "SUBJECT")
				a = SUBJECT;
			else if(reservedTypes[count] == "DESTINATION")
				a = DESTINATION;
			else if(reservedTypes[count] == "PRONOUN")
				a = PRONOUN;
			else if(reservedTypes[count] == "CONNECTOR")
				a = CONNECTOR;
			
			return true;
		}
	}
	return false;
}

// The test driver to call the scanner repeatedly  
// ** Done by:  Aaron & Erik
int main()
{
	int eof = 1;
  	tokentype thetype;
  	string theword; 
	
	string inputfile;
	printf("Please enter the name of file to be scanned: ");
	getline(cin, inputfile);

	fstream toRead;
	toRead.open(inputfile.c_str());
	
	string reserved;
	fstream dictionary;
	dictionary.open("reservedwords.txt");
	
	while(dictionary)	//fill reserved word vector
	{
		string rWord; 
		string rType;
		dictionary >> rWord >> rType;
		
		reservedWords.push_back(rWord);
		reservedTypes.push_back(rType);
	}
		
	dictionary.close();

	while (toRead)
	{
		toRead >> theword;
		eof = scanner(thetype, theword);  // call the scanner

		if(eof == -1)
			break;

		if(thetype == ERROR)	
	       		cout << "Type is: " << "ERROR" << endl;
		else if(thetype == WORD1)
			cout << "Type is: " << "WORD1" << endl;
		else if(thetype == WORD2)
			cout << "Type is: " << "WORD2" << endl;
	       	else if(thetype == PERIOD)
			cout << "Type is: " << "PERIOD" << endl;
		else if(thetype == VERB)
			cout << "Type is: " << "VERB" << endl;
		else if(thetype == VERBPAST)
			cout << "Type is: " << "VERPAST" << endl;
		else if(thetype == VERBNEG)
			cout << "Type is: " << "VERBNEG" << endl;
		else if(thetype == VERBPASTNEG)
			cout << "Type is: " << "VERBPASTNEG" << endl;
		else if(thetype == IS)
			cout << "Type is: " << "IS" << endl;
		else if(thetype == WAS)
			cout << "Type is: " << "WAS" << endl;
		else if(thetype == OBJECT)
			cout << "Type is: " << "OBJECT" << endl;
		else if(thetype == SUBJECT)
			cout << "Type is: " << "SUBJECT" << endl;
		else if(thetype == DESTINATION)
			cout << "Type is: " << "DESTINATION" << endl;
		else if(thetype == PRONOUN)
			cout << "Type is: " << "PRONOUN" << endl;
		else if(thetype == CONNECTOR)
			cout << "Type is: " << "CONNECTOR" << endl;
		cout << "Word is: " << theword << endl << endl;   
    	}
	toRead.close();
}// end