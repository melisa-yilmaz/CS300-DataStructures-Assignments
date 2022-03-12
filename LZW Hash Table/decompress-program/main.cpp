//

//  decompresshw2
//
//  Created by Melisa Yilmaz on 16.11.2021.
//

#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
#include <sstream>

using namespace std;

//implement all the character to the newly created dictionary
void implementASCII(vector<string> &dictionary)
{
    char ch;
    string letter;
    for (int i = 0; i <= 255; i++)
    {
        ch = char(i);
        letter = ch;
        dictionary.push_back(letter);
    }
}


void readingCompout(fstream & input, vector<int> & decompress)
{
    string line;
    
    while(getline(input,line))
    {
        istringstream s(line);
        int index;
        while(s >> index) //implement all the index values to decompress vector
        {
            decompress.push_back(index);
        }
    }
}
int main(int argc, const char * argv[])
{

    vector<string> dictionary;
    vector<int> decompress;

    implementASCII(dictionary);
    
    fstream input;
    ofstream output;
    
    input.open("compout");
    output.open("decompout");
    readingCompout(input, decompress);
   
    
    
    int number,previousNumber;
    string prefix,fc,text;

    for (int i = 0; i < decompress.size();i++)
    {
        if (i == 0) // take the first index in the decompress and find the key of it in the dictionary
        {
            number = decompress[i];
            cout << dictionary[number] << "";
            output << dictionary[number];
        }
        else
        {
            number = decompress[i];
            // if the number which is taken is not yet dictionary, create it, push it to dictionary and decompress it
            if (number > dictionary.size() - 1)
            {
                previousNumber = decompress[i-1];
                text= dictionary[previousNumber];
                fc = dictionary[previousNumber][0];
                text= text + fc;
                dictionary.push_back(text);
                cout <<  text <<  "";
                output << text;
               
            }
            // if the number is defined, decompress it and create newly adding string to the dictionary and push it
            else
            {
                number = decompress[i];
                cout << dictionary[number] <<  "";
                output << dictionary[number];
                
                previousNumber = decompress[i-1];
                text= dictionary[previousNumber];
                fc = dictionary[number][0];
                text = text + fc;
                
                dictionary.push_back(text);
            }
        }
    }
    cout << endl;
  

    return 0;
}
