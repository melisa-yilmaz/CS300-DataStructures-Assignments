
//  Created by Melisa Yilmaz on 28.10.2021.
//  CS 300 HOMEWORK 1
//  The aim of this homework to achieve the one of the most popular scheduling algorithm which is Multi Level Feedback Queue

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ostream>
#include <vector>
#include <queue>

using namespace std;

//to take the folderName and read configuration.txt line by line
void readingConfiguration(string &folderName,int &queue_Size,int &process_Size,int &S)
{
    cout << "Please enter the process folder name: ";
    cin >> folderName;
    fstream input;
    input.open(  "./" +folderName + "/configuration.txt");
    while(input.fail())
    {
        cout << "File cannot be opened.\n";
        cin.clear();
        cout << "Please enter the process folder name: ";
        cin >> folderName;
        input.open(  "./" +folderName + "/configuration.txt");
    }
    cout << "When all processes are completed, you can find execution sequence in \""+  folderName + "/output.txt\"" << endl << endl;
    cout << "Given Files for the Sample Run: "<< endl << endl;
    cout << folderName + "/configuration.txt: " << endl;
    string line;
    string configInput;
    int i = 0;
    bool check = true;
    while(getline(input,line) && check == true)
    {
        istringstream s(line);
        s >> configInput;
        if (i == 0 && configInput != "")
        {
            queue_Size= stoi(configInput);
            i++;
        }
            
        else if (i == 1 && configInput != "")
        {
            process_Size =stoi(configInput);
            i++;
        }
        else if(i > 1 && configInput != ""){
            S =stoi(configInput);
            check=false;
        }
        configInput = "";
    }
    cout << queue_Size << endl;
    cout << process_Size << endl;
    cout << S << endl;
    cout << endl;
}

// to create all the cycle of processes
vector<vector<string>> createProcessMatrix(int process_Size, string folderName)
{
    string processNo,processFiles;
    vector<vector<string>> processVector;
    for(int i = 0; i < process_Size; i++)
    {
        processNo = "P" + to_string(i+1);
        processFiles = processNo + ".txt";
        
        fstream processInput;
        string processLines;
        processInput.open(processFiles.c_str());
        vector<string> processNoVector;
        cout <<  folderName +"/p" + to_string(i+1)+  ".txt:" << endl;
        processInput.open(  "./" +folderName + "/p"+  to_string(i+1) + ".txt");
        while(getline(processInput,processLines))
        {
            if(processLines!=""){
                cout << processLines << endl;
                processNoVector.push_back(processLines);
            }
        }
        cout << endl;
        processVector.push_back(processNoVector);
    }
    return processVector;
}

vector<queue<string>> createQueueMatrix( vector<vector<string>> processMatrix,int queue_Size, int process_Size)
{
    vector<queue<string>> queueMatrix;
    for(int i = 0; i < queue_Size; i++)
    {
        queue<string> processQueue;
        if (i == 0)
        {
            for (int k = 0 ; k < process_Size; k++)
            {
                processQueue.push(to_string(k+1));
            }
            queueMatrix.push_back(processQueue);
        }
        else
        {
            queueMatrix.push_back(processQueue);
        }
    }
    return queueMatrix;
    
}
void MultipleFeedbackQueue(vector<vector<string>> processMatrix,vector<queue<string>> queueMatrix, int S, string folderName)
{
    ofstream output;
    output.open( "./" +folderName + "/output.txt");

    cout <<  "Output File for the Sample Run: \n"<< endl;
    cout << folderName +"/output.txt:" << endl;
    int s = 0;
    if (S != 0) // if there is time slice
    {
        for (int i = 0; i < queueMatrix.size(); i++) // search for the size of the queue matrix
        {
            if (s != S) //keep moving to decide the priority level of the process and change their queue if needed
            {
                if (s == 0)
                {
                    i = 0;
                }
       
                if (!queueMatrix[i].empty()) // if the queue in that row does not empty
                {
                    unsigned long l = queueMatrix[i].size();
                    for (int k = 0; k < l && s!= S; k++) // search every element in that Queue such as Q4
                    {
                        string processNumberInQueue = queueMatrix[i].front(); //takes the process number from front at that queue
                        queueMatrix[i].pop();// and pop it and put it into the CPU
                        int number = stoi(processNumberInQueue) - 1; // if the process number = 1, 1 means that 0.row in the processMatrix
                        bool check = true;
                        for (int t = 0; t <= processMatrix[number].size() - 1  &&  check == true; t++)
                        {
                            if(processMatrix[number][t] != "" && processMatrix[number][t] != "-")
                            {
                                
                                if (processMatrix[number][t] == "1" && processMatrix[number][t+1] != "-") // 1 in the process matrix means that, the process in the CPU runs entire time slice
                                {
                                    if (i == queueMatrix.size()-1)
                                    {
                                        queueMatrix[i].push(processNumberInQueue); // keeps the priority level of this process same as Q1-->Q1
                                        output << "1, PC" + processNumberInQueue + ", Q" + to_string(queueMatrix.size()- (i+1)+1) << endl;
                                        l = l+1; // due to adding the back of the same queue, we need to increase the size of that queue by 1
                                    }
                                    else {
                                        queueMatrix[i+1].push(processNumberInQueue); // decrease the priority level of this process by one Q4-->Q3
                                        output << "1, PC" + processNumberInQueue + ", Q" + to_string(queueMatrix.size()- (i+1)) << endl;
                                    }
                             
                                    
                                }
                                else if (processMatrix[number][t] == "0" && processMatrix[number][t+1] != "-") //0 in the process matrix means that, the process in the CPU gives up before the entire time slice
                                {
                                    
                                    queueMatrix[i].push(processNumberInQueue); //put this process into the same priority level, so put back it into back of the same queue Q4-->Q4
                                    l = l+1; // due to adding the back of the same queue, we need to increase the size of that queue by 1
                                    output << "0, PC" + processNumberInQueue + ", Q" + to_string(queueMatrix.size()- (i+1)+1) << endl;
                                }
                                else if  (processMatrix[number][t+1] == "-")
                                {
                                    processMatrix[number][t+1] = "";
                                    output << "E, PC" + processNumberInQueue + ", QX"<< endl;
                                }
                    
                                processMatrix[number][t] = "";
                                s++;
                                check= false;
                            }
                        }
                    }
                }
            }
            else {
                for (int p = 1; p < queueMatrix.size(); p++)
                {
                    if (!queueMatrix[p].empty())
                    {
                        unsigned long l = queueMatrix[p].size();
                        for (int k = 0; k < l; k++)
                        {
                            queueMatrix[0].push(queueMatrix[p].front());
                            output << "B, PC" + queueMatrix[p].front() + ", Q" + to_string(queueMatrix.size()) << endl;
                            queueMatrix[p].pop();
                            
                        }
                    }
                 }
               
                i = 0;
                s = 0;
            }
        }
       
    }
    output.close();
}

int main() {
    int queue_Size,process_Size,S;
    string folderName;
    readingConfiguration(folderName,queue_Size, process_Size,S);
    vector<vector<string>> processMatrix = createProcessMatrix(process_Size, folderName);
    vector<queue<string>> queueMatrix = createQueueMatrix(processMatrix,queue_Size, process_Size);
    MultipleFeedbackQueue(processMatrix,queueMatrix, S,folderName);
    return 0;
}
