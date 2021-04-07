#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>

using namespace std;
void reorder(vector <string>& lines, int linecnt)
{
	int cntr = 0;
	linecnt = linecnt - 1;
	vector <string> reorder;
	string orderid, customer, salesrep, region, product, unitprice, unitsold, date;
	for (int i = 0; i < lines.size(); i++) //find the items in the list to reorder them
	{
		if (i % linecnt == 0)
		{
			orderid = lines[i];
		}
		else if (i % linecnt == 1)
		{
			customer = lines[i];
		}
		else if (i % linecnt == 2)
		{
			salesrep = lines[i];
		}
		else if (i % linecnt == 3)
		{
			region = lines[i];
		}
		else if (i % linecnt == 4)
		{
			product = lines[i];
		}
		else if (i % linecnt == 5)
		{
			unitprice = lines[i];
		}
		else if (i % linecnt == 6)
		{
			unitsold = lines[i];
		}
		else if (i % linecnt == 7)
		{
			date = lines[i];
		}
		cntr++;
		if (cntr == 8) // reorder the list of items
		{
			cntr = 0;
			reorder.push_back(orderid);
			reorder.push_back(product);
			reorder.push_back(unitprice);
			reorder.push_back(unitsold);
			reorder.push_back(customer);
			reorder.push_back(salesrep);
			reorder.push_back(region);
			reorder.push_back(date);
		}
	}
	lines.clear();
	for (int i = 0; i < reorder.size(); i++) //clear vector and pushback new items reorganized
	{
		lines.push_back(reorder[i]);
	}
}
void capitalize(string &word)
{
	transform(word.begin(), word.end(), word.begin(), toupper); // make all capitalized
	transform(word.begin() + 1, word.end(), word.begin() + 1, tolower);//all but the first letter lower case
	return;
}
void numbercheck(string &num, vector <string> errors)
{
	string temp;
	if (isdigit(num[1]))
	{
		temp = num;
		for (int i = 0; i <= num.size(); i++)
		{
			if (isdigit(num[i]) == false)
			{
				num.erase(num.begin() + i, num.end()); //find the period and delete all that comes after to standardize number format.
				num.shrink_to_fit(); // once numbers have been erased, string must be resized.
				errors.push_back("changed digit: " + temp + " to " + num);
				return;
			}

		}
		
	}
	else
		return;
}
void datemanip(string &date, vector <string> &errors)//change the date to a standardized format
{
	string slash = "/";
	string tempmonth;
	string pushmonth;
	int datecnt = 0;
	if (isdigit(date[0]))
	{
		pushmonth = date;
		while (isdigit(date[datecnt]))//to discover length of first set of numbers be it 1/nov/16 or 14/nov/16
			datecnt++;
		datecnt = datecnt + 1; // skip the date seperation / or -
		tempmonth.assign(date, datecnt, 3); //datecnt is the location of the three word month. 3 is the length of the word assigned to tempmonth
		int stop = date.size() - 1;//stop the loop one before end so as not to place a slash at the end of the date
		for (int i = 0; i <= stop; i++)
		{
			if (isdigit(date[i]) == false)
			{
				date.replace(i, 1, slash); //replace whatever is in the location of the / so as to standardize
			}
		}
		if (tempmonth == "nov" || tempmonth == "Nov")//find the month
		{
			tempmonth = "11";
			date.replace(datecnt, 3, tempmonth);//assing the correct month to standardize the dates
			date.shrink_to_fit();//shrink the string length to fit correctly.
			errors.push_back("Changed date formant from: " + pushmonth + " to " + date);
		}

		
	}
	else
		return;
}


bool wordcheck(string s)
{
	string::iterator it = s.begin();
	while (it != s.end()) 
	{
		if (isdigit(*it))
		{
			return true;
		}
		it++;
	}
	return false;
}
void split(vector <string>& lines, vector <string> &errors, vector <string> &ids)
{

	string temp;
	int cntr = 0;
	int date = 0;
	for (int i = 0; i <= lines.size(); i++) // since the first digit in both files is the ID if we find that number it will be the counter for the new line in the vector.
	{
		temp = lines[i];
		if (wordcheck(temp)) //check if temp is a digit
		{
			cntr = i;
			i = lines.size(); //break loop
		}
	}
	date = cntr - 1; //receive position of date
	for (int i = 0; i < lines.size(); i++)
	{
		if (i % cntr == 0 && i != 0) //since cntr is the first number it must be the id, IDs get pushed into the id vector to check their validity
		{
			if (find(ids.begin(), ids.end(), lines[i]) != ids.end()) //the vector of id's is searched for lines[i]
			{
				errors.push_back("Similar ID's: " + lines[i]); //flag the error of similar ID's
			}
			ids.push_back(lines[i]); //if the remainder of cntr and i == 0 then the i must be divisable by seven which makes it an id.
		}
		else if (i % cntr == 0 + date)//laction of date is found and date is checked in the vector
		{
			datemanip(lines[i], errors);//if there is a line that is shorter than the others it will not push back this date as it is outside the vector
		}
		else if (i % cntr == 0 + date - 1)
		{
			if (wordcheck(lines[i]))
			{
				lines.insert(lines.begin() + i, "NULL");
			}
			else
				capitalize(lines[i]);
		}
		else
		{
			numbercheck(lines[i], errors); //standardize number formats
		}
	}
}
void output(vector <string> s, int outputcnt, int linecnt)
{ //need to get an iterator to count the amount of words in a line and input outputcnt at the begining of the line in order to distinguish the two data sets
	int i = 0;
	ofstream outFile, errorfile;
	string stuff;
	outFile.open("ETLLegacyRefined.csv", ios::app);
	if (outputcnt == 1)
	{
		for (int it = 0; it != s.size(); ++it)
		{
			stuff = s[it];
			if (it == 0)//append outputcnt at begining of each line in order to distinguish between data sets and ID's
			{
				outFile << "IDKey, " << stuff;
			}
			else if (it == 5)
			{
				outFile << ", SalesRep";//insert column identifier on the first row in data
			}
			else if (it % linecnt == 0)//this will loop through every line so that the new line is out puted to the csv document
			{
				outFile << endl << outputcnt << ", " << stuff;
			}
			else
			{
				outFile << ", " << stuff;
			}

		}
	}
	if (outputcnt == 2)
	{
		linecnt = linecnt - 1;
		for (int it = linecnt; it != s.size(); ++it)
		{
			stuff = s[it];

			if (it % linecnt == 0)//this will loop through every line so that the new line is out puted to the csv document
			{
				outFile << endl << outputcnt << ", " << stuff;
			}
			else
			{
				outFile << ", " << stuff;
			}

		}
	}
	outFile.close();
	
	if (outputcnt == 3)
	{
		int cnt = 0;
		errorfile.open("Errors.csv");
		for (int i = 0; i < s.size(); i++)
		{
			cnt++;
			errorfile << cnt << ": " << s[i] << endl;
		}

	}


}
int main() {

	vector <string> prelines;
	vector <string> lines;
	vector <string> errors;
	vector <string> ids;
	vector <string> dates;
	string line, words;
	fstream csvfile;
	fstream txtfile;
	string tempbeg, tempend;
	bool found = false;
	int linecnt = 0, prelinecnt = 0, errorcnt = 0, outputlinecnt = 0, filecnt = 1;

	txtfile.open("ETLLegacyDBData.txt");
	if (txtfile)
	{
		while (getline(txtfile, line)) // while there is a line to get it will continue to get the line out of the txt
		{
			
			stringstream copyline(line);//copy line to break down
			while (getline(copyline, words, ',')) //break down the line between commas
			{
				words.erase(remove(words.begin(), words.end(), ' '), words.end());//remove the spaces between the words
				if (linecnt == 5)
				{
					lines.push_back("NULL"); // insert the missing category into the data
					linecnt++;
					errors.push_back("Missing SalesRep: " + line);
				}
				lines.push_back(words);
				linecnt++;
			}
			if (linecnt < prelinecnt)//if linecnt is less than the previous linecnt, there is something missing from the line.
			{
				errors.push_back("Missing Data in line: " + line); //flag error and count
				linecnt = 0;
			}
			else
			{
				prelinecnt = linecnt;
				linecnt = 0;
			}
			
		}
		outputlinecnt = prelinecnt;
	}
	txtfile.close();

	split(lines, errors, ids);
	output(lines, filecnt, outputlinecnt);//send all data to an output file
	filecnt++;
	lines.clear();


	csvfile.open("ETLManualData.csv");
	if (csvfile)
	{
		while (getline(csvfile, line)) // while there is a line to get it will continue to get the line out of the txt
		{
			stringstream copyline(line);//copy line to break down
			while (getline(copyline, words, ',')) //break down the line between commas
			{
				words.erase(remove(words.begin(), words.end(), ' '), words.end());//remove the spaces between the words
				if(linecnt != 7)//push back all but the 7th row
					lines.push_back(words);
				linecnt++;
			}
			if (linecnt < prelinecnt)//if linecnt is less than the previous linecnt, there is something missing from the line.
			{
				errors.push_back(" Line error " + line); //flag error and count
				linecnt = 0;
			}

			else
			{
				prelinecnt = linecnt;
				linecnt = 0;
			}
		}
		outputlinecnt = prelinecnt;
	}
	reorder(lines, outputlinecnt);
	split(lines, errors, ids);
	output(lines, filecnt, outputlinecnt);
	filecnt++;
	output(errors, filecnt, outputlinecnt);

	return 0;
}
