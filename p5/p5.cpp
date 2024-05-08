#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <cctype>
#include <stack>

using namespace std;

struct Node {
    int val;
    string huff = "";
    struct Node *left, *right;
};

Node* newNode(int val)
{
    Node* node = new Node;
    node->val = val;
    node->left = node->right = NULL;
    return (node);
}

Node* tree(vector<int> lo, vector<int> io);
int searchIndex(vector<int> vect,int val);
vector<int> getL(vector<int> vect, int val);
vector<int> getR(vector<int> vect, int val);
void printVect(vector<int> vect);
void huffman(Node* node, string huff, vector<vector<string>>& nodes);
void huffmanR(Node* node, string huff, bool dir,vector<string>& v);
vector<int> overlap(vector<int> vect1, vector<int> vect2);
void decode(Node* node, string fileName);



int main(int argc, char *argv[]) {
    string huff = "";
   if (argc != 4) {
    cout<<"Incorrect number of cmd-line args"<<endl;
    return 1;
   } 
   vector<int> levelOrder;
   vector<int> inOrder;

   ifstream inputFile(argv[1]);

   if (inputFile) {
    int value;

    while ( inputFile >> value ) {
        inOrder.push_back(value);
    }
   }

   ifstream inputFile2(argv[2]);

   if (inputFile2) {
    int value;

    while ( inputFile2 >> value ) {
        levelOrder.push_back(value);
    }
   }

    Node* root = tree(levelOrder,inOrder);
    vector<vector<string>> nodes;
    huffman(root, huff, nodes);

    decode(root, argv[3]);
}



Node* tree(vector<int> lo, vector<int> io) {
    Node* root = newNode(lo[0]);
    int rootIndex = searchIndex(io,lo[0]);
    vector<int> leftVect = getL(io,rootIndex); 
    vector<int> rightVect = getR(io,rootIndex);
    vector<int> LOR = overlap(lo,rightVect);
    vector<int> LOL = overlap(lo,leftVect);

    if ((lo.size() == 1) && io.size() == 1) {return root;}
    root->left = tree(LOL,leftVect);
    root->right = tree(LOR,rightVect);
    return root;
    
    
    
   


}

int searchIndex(vector<int> vect, int val) {
    for (int i = 0; i < vect.size(); i++) {
        if (vect[i] == val) {
            return i;
        }
    }
    return -1;
}

vector<int> getL(vector<int> vect, int val) {
    vector<int> ret;
    for (int i = 0;i<=val-1;i++){
        ret.push_back(vect[i]);
    }
    return ret;
}

vector<int> getR(vector<int> vect, int val) {
    vector<int> ret;
    for (int i = val+1;i<=vect.size()-1;i++){
        ret.push_back(vect[i]);
    }
return ret; 
}

void printVect(vector<int> vect) {
    for (int i = 0; i<vect.size(); i++) {
        cout<<vect[i]<<" ";
    }
    cout<<endl;
}

vector<int> overlap(vector<int> vect1, vector<int> vect2) {
    vector<int> ret;

    for (int i = 0; i<vect1.size(); i++) {
        for (int j = 0; j<vect2.size(); j++) {
            if(vect1[i] == vect2[j]) {
                ret.push_back(vect1[i]);
            }
    }
    }
    return ret;
}

void huffman(Node* node, string huff,vector<vector<string>>& nodes) {
    if (node == NULL)
        return;
    node->huff = huff;
    
    vector<string> leaf;
    huffmanR(node->left, huff,true, leaf);
    for (int i = 0; i<(leaf.size());i+=2) {
        vector<string> n;
        n.push_back(leaf[i]);
        n.push_back(leaf[i+1]);
        nodes.push_back(n);
    }
    leaf.clear();

    huffmanR(node->right, huff,false, leaf);
    for (int i = 0; i<(leaf.size());i+=2) {
        vector<string> n;
        n.push_back(leaf[i]);
        n.push_back(leaf[i+1]);
        nodes.push_back(n);
    }
    leaf.clear();
}

void huffmanR(Node* node, string huff, bool dir,vector<string>& v) {

    if (node == NULL)
        return;
    if (dir) {
        huff = huff + '0';
    }
    else {
        huff = huff + '1';
    }
    node->huff = huff;
    huffmanR(node->left, huff,true, v);
    huffmanR(node->right, huff,false, v);
    if (node->left == NULL && node->right == NULL) {
        v.push_back(to_string(node->val));
        v.push_back(node->huff);
    }
    
}

void decode(Node* node, string fileName) {
    std::ifstream file(fileName);
    if (!file) {
        std::cout << "Unable to open file\n";
        return;
    }
    Node* root = node;
    std::string buffer;
    while (getline(file, buffer)) {
        for (char bit : buffer) {
            if (bit == '1') {
                if (node->right != nullptr) {
                    node = node->right;
                } else {
                    std::cout << "Unexpected end of file.\n";
                    return;
                }
            } else if (bit == '0') {
                if (node->left != nullptr) {
                    node = node->left;
                } else {
                    std::cout << "Unexpected end of file.\n";
                    return;
                }
            }
            if (node->left == nullptr && node->right == nullptr) {
                std::cout << char(node->val);
                node = root;
            }
        }
    }
}