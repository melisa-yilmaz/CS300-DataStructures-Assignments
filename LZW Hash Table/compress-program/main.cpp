
//  Created by Melisa Yilmaz on 14.11.2021.
//  The aim in this homework is that implement a data compression which takes a file A and, within a reasonable amount of time, transforms it into another file B.
//  For this aim, I used the Ziv-Lempel Algorithm which maps strings of input characters into numeric codes.
//  The mapping between character strings and their codes is stored in a dictionary.
//  This part includes the compress part of the Ziv-Lempel Algorithm.



#include <iostream>
#include <string>
#include "HashTable.h"
#include <fstream>
#include <vector>
using namespace std;

int indexF = 255;

//to implement all the ASCII character to the hash table
void implementASCII( OtherHashedObj& element, HashTable<OtherHashedObj> &H)
{
    char ch;
    string letter;
    for(int i = 0; i <= 255; i++)
    {
        ch = char(i);
        element.key= ch;
        element.code = i;
        H.insert(element);
    }
}

int main()
{
    OtherHashedObj element;
    OtherHashedObj notFound;
    HashTable<OtherHashedObj> Hash(notFound,4099);
    implementASCII(element,Hash);
    
    ifstream input;  ofstream output;
    char ch;
    string txt;
    input.open("compin");
    output.open("compout");
    string p;
    while(!input.eof()) //add all the element in the compin to the newly created string which is called txt
    {
        input.get(ch);
        txt += ch;
       
        if (Hash.find(txt) == notFound) //if the element is not found in the HashTable,
        {
            p = txt.substr(0,txt.length()-1); //take the longest prefix that occurs in the dictionary,
            int found = Hash.find(p).code; //and find the code of it in the HashTable.
            output << found << " ";
            indexF++;
            element.key = txt;
            element.code = indexF;
            if (indexF < 4096) //if the element that we added to the HashTable not exceed the 4096, insert it to the HashTable
            {
                Hash.insert(element);
            }
            txt = txt[txt.length()-1];
        }
    }

    if ((Hash.find(txt).key != "") && (Hash.find(txt.substr(0, (txt.length() - 1))).code != 0)) {
        output << to_string(Hash.find(txt.substr(0, (txt.length() - 1))).code) << " ";
    }

    input.close();
    output.close();
    return 0;
}



