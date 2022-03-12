// Melisa Yilmaz 26777
// The aim of this homework is that solving a puzzle that involves a list of words.
// Given a word lets call it as A and try to figure out whether A is transformed to B by a series of one-character substitions or not.
// If it is transformed, then the input would be corresponding sequence of words.
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>

using namespace std;


struct Vertex
{
    string vertexWord;
    string vertexSentence;
    bool known;
    int dist;
    string path;
};

struct position
{
    string name;
    string sentence;
    
    bool const operator==(const position &lhs) const
    {
        return this->name == lhs.name && this->sentence == lhs.sentence;
    };
};

unordered_map<string, unordered_set<position>> map;
unordered_map<string, Vertex> graph;
bool check = true;

template <>
struct hash<position>
{
    size_t operator()(const position & x) const
    {
        hash<string> hasher;
        return hasher(x.sentence);
    };
    
    size_t operator()(position & x) const
    {
        hash<string> hasher;
        return hasher(x.sentence);
    };
};


//take each word from unordered_map and try to substitute operation
//if the newly created word is found in unordered_map, add it to the word's unordered_set after substitution operation
void substition(string word)
{
    int index = 0;
    while(index < word.length())
    {
        string eachIndex;
        string newWord;
        for (int i = 39; i >= 39 && i <=122; i++)
        {
            eachIndex = char(i);
            newWord = word.substr(0,index) + eachIndex + word.substr(index+1);
           
            if(map.find(newWord) != map.end() && newWord != word)
            {
                position set;
                set.name = newWord;
                string space = "";
                set.sentence = space + " (change " + word[index] + " at position " + to_string(index+1) + " to " + eachIndex + ")";
                
                if(check == true)
                {
                    unordered_set<position> set2 ={set};
                    map[word].swap(set2);
                    check = false;
                }
                else
                    map[word].insert(set);
            }
            if (i == 47)
                i = 96;
        
        }
   
        index++;
       
    }
}

//take each word from unordered_map and try to insert operation
//if the newly created word is found in unordered_map, add it to the word's unordered_set after insert operation
void insertions(string word)
{
    int index = 0;
    while(index <= word.length())
    {
        string eachIndex;
        string newWord;
        
        for (int i = 39; i >= 39 && i <=122; i++)
        {
            eachIndex = char(i);
           
            if(index == word.length())
                newWord = word + char(i);
            
            else
                newWord = word.substr(0,index) + char(i) + word.substr(index);
            
            if(map.find(newWord) != map.end())
            {
                position set;
                set.name = newWord;
                string space = "";
                set.sentence = space + " (insert " + char(i) + " after position " + to_string(index) + ")";
                
                if(check == true)
                {
                    unordered_set<position> set2 ={set};
                    map[word].swap(set2);
                    check = false;
                }
                else
                    map[word].insert(set);
            }
           
            if (i == 47)
                i = 96;
           
        }
        index++;
    }
  
}

//take each word from unordered_map and try to delete operation
//if the newly created word is found in unordered_map, add it to the word's unordered_set after delete operation
void deletions(string word)
{
    int index = 0;
    
    while (index < word.length())
    {
        string eachIndex;
        string newWord;
        
        newWord = word.substr(0,index) +  word.substr(index+1);
        if(map.find(newWord) != map.end())
        {
            position set;
            set.name = newWord;
            string space = "";
            set.sentence = space + " (delete " + word[index] + " at position " + to_string(index+1) + ")";
            
            if(check == true)
            {
                unordered_set<position> set2 ={set};
                map[word].swap(set2);
                check = false;
            }
            else
                map[word].insert(set);
        }
        index++;
    }
}

//take all the words from unordered_map and create it's unordered_set as making substitution, insertion, and delete operations
void implementingAdjacencyList()
{
    fstream input;
    string word;
    input.open("words.txt");
    while(getline(input,word))
    {
        substition(word);
        insertions(word);
        deletions(word);
    }
    input.close();
}

//add all the elements of the words.txt to the unordered_map
void insertingUnorderedMap(fstream & input)
{
    string word;
    while(getline(input,word))
    {
        position s;
        map[word].insert(s);
        
    }
    input.close();
}


//takes word1 as an input in main, and makes the graph of it as using Breadth First Search Approach
void bfs(string word1)
{
    queue<Vertex> Q;
    Vertex s,v;
    s.vertexWord = word1;
    s.vertexSentence = "";
    s.known = false;
    s.dist = 0;
    
    Q.push(s);
    graph[word1] = s;

    while(!Q.empty())
    {
        v = Q.front();
        Q.pop();
        v.known = true;
        
        for(auto adjacent :map[v.vertexWord])
        {
            if(graph.find(adjacent.name) == graph.end())
            {
                Vertex w;
                w.vertexWord = adjacent.name;
                w.vertexSentence = adjacent.sentence;
                w.dist = v.dist + 1;
                w.path = v.vertexWord;
                graph[w.vertexWord] = w;
                Q.push(w);
            }
        }
    }
}

//takes word2 and try to reach word1 from it
void shortestPathFinding(string word1, string word2)
{
    stack<Vertex> print;
    //if there is no word2 in graph, it means that there is no such a path from word1 to word2
    if(graph.find(word2) == graph.end())
    {
        cout << "There is no such a path. " << endl;
    }
    else
    {
        print.push(graph[word2]);
        string searchW1 = graph[word2].path;
       
        while(searchW1 != word1)
        {
            print.push(graph[searchW1]);
            searchW1 = graph[searchW1].path;
        }
        print.push(graph[word1]);
        Vertex v;
        while(print.size()!=0)
        {
            v = print.top();
            print.pop();
            cout << v.vertexWord << v.vertexSentence << endl;
        }
    }

}

int main(int argc, const char * argv[]) {
    fstream input;
    input.open("words.txt");
    insertingUnorderedMap(input);
    implementingAdjacencyList();
    cout << "Please enter words: ";
    string word1, word2;
    cin >> word1 >> word2;
    while(word1[0] != '*')
    {
        if(map.find(word1) != map.end() && map.find(word2) != map.end())
        {
            bfs(word1);
            shortestPathFinding(word1,word2);
            graph.clear();
            check = true;
            cout << endl;
     
        }
        else // if words are not found in the unordered_hash map, ask for new words
            cout << "Invalid word(s)" << endl;

        cout << "Please enter words: ";
        cin >> word1 >> word2;
      
    }
    
    return 0;
}
