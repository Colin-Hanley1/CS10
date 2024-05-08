#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <cctype>



using namespace std;

uint64_t djb2(const std::string& key) {
        uint64_t hash = 5381;
        for (char c : key) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash;
    }

class HashTable {
    private:
    struct Item {
        string value;
        int key;
        vector<int> lineNums;
        
    };
    int h1(string& key) {
         unsigned long pt = djb2(key);
         return  pt %size;
      }
      int h2(string& key) {
         unsigned long pt = djb2(key);
         return (a - (pt % a));
      }
      int linearprobe(string& key, int j) {
        return (h1(key) +j) % size;
      }
      int quadprobe(string& key, int j) {
         return (h1(key)+ j*j) % size;
      }
      int doublehash(string& key, int j) {
         return (h1(key) + j*h2(key)) % size;
      }
    public:
    int size;
    string probe;
    int uniques = 0;
    int a;
    vector<Item> table;
    int collisions = 0;
    void insert(string value, int lineNum) {
        table.resize(size);
        int j = 0;
        int repeat = false;
        int index = h1(value);
        while (!repeat && !(table[index].value.empty())) {
            if (value != table[index].value) {
                while(!(table[index].value.empty()) && (table[index].value != value)) { //checks if the initial index is empty
                if (probe == "lp") {
                    j++;
                    index = linearprobe(value, j);
                }
                else if (probe =="qp") {
                    j++;
                    index = quadprobe(value, j);
                }
                else if (probe == "dh") {
                    j++;
                    index = doublehash(value, j);
                }
                collisions++;
            } 
            }else {
                while(!(table[index].value.empty()) && (table[index].value != value)) { //checks if the initial index is empty
                if (probe == "lp") {
                    j++;
                    index = linearprobe(value, j);
                }
                else if (probe =="qp") {
                    j++;
                    index = quadprobe(value, j);
                }
                else if (probe == "dh") {
                    j++;
                    index = doublehash(value, j);
                }
                collisions++;
            } 
                table[index].lineNums.push_back(lineNum);
                repeat = true;
                break;
            }
        }

        if (!repeat) {
        uniques++;
        while(!(table[index].value.empty())) { //checks if the initial index is empty
                if (probe == "lp") {
                    j++;
                    index = linearprobe(value, j);
                }
                else if (probe =="qp") {
                    j++;
                    index = quadprobe(value, j);
                }
                else if (probe == "dh") {
                    j++;
                    index = doublehash(value, j);
                }
                collisions++;
            }
        table[index].value = value;
        table[index].key = index;
        table[index].lineNums.push_back(lineNum);
        
        }

    }



    void search(string value) {
        int index = h1(value);
        bool found = false;
        int j = 0;
    cout<<endl;
        while (!found && !table[index].value.empty()) {
            if (table[index].value != value) {
                if (probe == "lp") {
                    j++;
                    index = linearprobe(value, j);
                }
                else if (probe =="qp") {
                    j++;
                    index = quadprobe(value, j);
                }
                else if (probe == "dh") {
                    j++;
                    index = doublehash(value, j);
                }
            }
            else if(table[index].value == value) {
                found = true;
            }
        }
        if (found) {
            cout<<value<<" appears on lines [";
        for (int i = 0; i<int(table[index].lineNums.size()-1); i++) {
            cout<<table[index].lineNums[i]<<",";
        }
        cout<<table[index].lineNums[table[index].lineNums.size()-1]<<"]"<<endl;
        cout<<"The search had "<<j<<" collisions"<<endl;
        }
        else {
        cout<<value<<" appears on lines []"<<endl;
        cout<<"The search had "<<j<<" collisions"<<endl;
        }
    
    }
   
    int getCollisions() {
        return collisions;
    }

    int getUniques() {
        return uniques;
};
};
int main(int argc, char* argv[]) {
    string fileName = string(argv[1]);
    string qFile = string(argv[2]);
    int tableSize = atoi(argv[3]);
    string collResoution = string(argv[4]);
    int a = 0;
    int numWords = 0;
    if (collResoution == "dh") {
        a = atoi(argv[5]);
    }

    int lineNum = 0;

   ifstream inputFile(fileName);
    if (!inputFile) {
        cerr << "Error: Unable to open the input file." << endl;
        return 1;
    }

    HashTable hash;
    hash.size = tableSize;
    hash.probe = collResoution;
    hash.a = a;


    
   string line;
   while (getline(inputFile, line)) {
      lineNum++;
      stringstream ss(line);
      string word;
      while (ss >> word) {
        int p = word.length();
         string cleanWord;
         for(char ch: word) {
            if(isalpha(ch)) {
               cleanWord += tolower(ch);
               p++;
            }
            else if(ispunct(ch)){
               hash.insert(cleanWord, lineNum);
               numWords++;
               cleanWord = "";
            }
         }
         if (!cleanWord.empty()) {
            hash.insert(cleanWord, lineNum);
            numWords++;
         }
      }
   }
    inputFile.close();
   cout<<"The number of words found in the file was "<<numWords<<endl;
   cout<<"The number of unique words found in the file was "<<hash.getUniques()<<endl;
   cout<<"The number of collisions was "<<hash.getCollisions()<<endl;
 
ifstream queryFile(qFile);
    if (!queryFile) {
        cerr << "Error: Unable to open the input file." << endl;
        return 1;
    }

while (getline(queryFile, line)) {

      lineNum++;
      stringstream ss(line);
      string word;
      while (ss >> word) {
         string cleanWord;
         for(char ch: word) {
            if(isalpha(ch)) {
               cleanWord += tolower(ch);
            }
         }
         if (!cleanWord.empty()) {
            hash.search(cleanWord);
           
         }
      }
   }

}

