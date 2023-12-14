#include <iostream>
#include <cstring>

using namespace std;
struct Child;
class Children;
class TreeNode;
//child is a linked list(children) node that carrys a tree node inside it
class Child {
public:
    TreeNode* node; //pointer to the node to carry
    Child* next; //pointer to next sibling
    Child() {
        node = nullptr;
        next = nullptr;
    }
    Child(TreeNode* node, Child* next) : node(node), next(next) {}
};
// children is a linked list of children of a node
class Children {
public:
    Child* head; //pointer to head(first element in list)
    int size = 0; //size of linked list(initially = 0)
    void deleteList() {
        this->head = nullptr;
        size = 0;
    }
    Children() : head(nullptr), size(0) {}
    //function to add a new child to a node
    Child* addChild(TreeNode* node) {
        Child* newChild = new Child(node, nullptr);
        if (this->head == nullptr) {
            this->head = newChild;
            size++;
            return this->head;
        }
        else {
            newChild->next = this->head;
            this->head = newChild;
            size++;
            return newChild;
        }
    }
};
// treenode is then node that has children, start index, suffix index
class TreeNode {
public:
    Children children; // linked list of children
    int strIndex; //start index in string
    int suffixIndex; // suffix index in string
    TreeNode() {
        strIndex = -1;
        suffixIndex = -1;
        children.head = nullptr;
    }
};
class SuffixTree {
public:
    TreeNode* root; //the root node of the tree
    const char* word; // the word stored
    int wordLength; // stores length of word
    SuffixTree() {
        root = new TreeNode();
        int len = 0;
        while (word[len] != '\0') {
            len++;
        }
        wordLength = len;
    }
    SuffixTree(const char* word) : word(word) {
        root = new TreeNode();
        wordLength = strlen(word);
    }
    //finds which node starts with desired char
    TreeNode* findMatchingNodes(TreeNode* node,char c){
        Child* temp = node->children.head;
        while(temp != nullptr){
            int edgeStart = temp->node->strIndex;
            if(word[edgeStart] == c){
                return temp->node;
            }
            temp = temp->next;
        }
        return nullptr;
    }
    //gets minimum start index of children
    int getMinChildIndex(Children children){
        int min = children.head->node->strIndex;
        Child* temp = children.head->next;
        while(temp != nullptr){
            if(temp->node->strIndex < min){
                min = temp->node->strIndex;
            }
            temp = temp->next;
        }
        return min;
    }
    //constructs the tree by calling addNode for each suffix in word
    void constructTree() {
        for (int i = 0; i < wordLength; ++i) {
            addNode(i, wordLength,i, root);
        }
    }
    //adds nodes to tree in correct place
    void addNode(int index, int current_index,int real, TreeNode* currentNode) {
        current_index-=index;
        while(current_index>0){
            //case 1: no children for the given node
            if (currentNode->children.size == 0) {
                //adds new node as a child(leaf)
                Child* child = currentNode->children.addChild(new TreeNode());
                child->node->strIndex = index;
                child->node->suffixIndex = index;
                return;
            }
            //case 2: node has children
            Child* childNode = currentNode->children.head;
            while (childNode != nullptr) {
                //get start index of the suffix in node
                int edgeStart = childNode->node->strIndex;
                int min;
                //gets min start index of children to get the length of suffix in node
                if(childNode->node->suffixIndex == -1){
                    min = getMinChildIndex(childNode->node->children);
                }
                //get end index of suffix or egge
                int edgeEnd = (childNode->node->suffixIndex == -1) ? (min-childNode->node->strIndex-1)+childNode->node->strIndex : wordLength-1;
                //test first character for match
                if (word[index] == word[edgeStart]) {
                    //found a match and now is getting common prefix of suffix
                    int commonLen = 0;
                    while (commonLen < edgeEnd-edgeStart+1 && word[index + commonLen] == word[edgeStart + commonLen]) {
                        commonLen++;
                        current_index--;
                    }
                    // Case 2.1: the whole edge/suffix is covered by the suffix
                    if (commonLen == edgeEnd-edgeStart+1) {
                        //recursively calls function to test for its children
                        addNode(index + commonLen, wordLength,real, childNode->node);
                        return;
                    }
                    //case2.2: node split
                    TreeNode* newNode = new TreeNode(); //create node for new suffix
                    newNode->strIndex = index+commonLen; //set its start index
                    newNode->suffixIndex = real; //set its suffix index
                    //if splitted node is a leaf
                    if(childNode->node->suffixIndex != -1){
                        //splitted suffix
                        TreeNode* splittedSuffix = new TreeNode();
                        splittedSuffix->strIndex = edgeStart + commonLen; // new start index
                        splittedSuffix->suffixIndex = childNode->node->suffixIndex; // suffix index did not change
                        //old suffix node
                        childNode->node->strIndex = edgeStart; // start index did not change
                        childNode->node->suffixIndex = -1; // suffix index becomes -1 (not a leaf anymore)
                        //add the 2 created nodes to be children of the old node
                        childNode->node->children.addChild(splittedSuffix);
                        childNode->node->children.addChild(newNode);
                        return;
                    }
                        //splitted node is an internal node(splitted edge)
                    else{
                        //new me (new old node -_-)
                        TreeNode *newMe = new TreeNode();
                        newMe->strIndex = childNode->node->strIndex + commonLen; //set new start index
                        newMe->suffixIndex = -1;
                        //add children of old parent to become new Node's children
                        Child *add = childNode->node->children.head;
                        for (int i = 0; i < childNode->node->children.size; ++i) {
                            newMe->children.addChild(add->node);
                            add = add->next;
                        }
                        //delete children of old parent
                        childNode->node->children.deleteList();
                        childNode->node->children.addChild(newMe);
                        //add new suffix node
                        childNode->node->children.addChild(newNode);
                        return;
                    }

                }
                childNode = childNode->next;
            }
            // Case 3: Suffix forms a new leaf node
            Child* child = currentNode->children.addChild(new TreeNode());
            child->node->strIndex = index;
            child->node->suffixIndex = real;
            break;
        }

    }
    void traverseAndPrint(TreeNode* node, const char* word, int level = 0) {
        if (!node) {
            return;
        }
        for (int i = 0; i < level; ++i) {
            cout << "  ";
        }
        cout << "(" << node->strIndex << ", " << node->suffixIndex << ")";
        if (node->children.size != 0) {
            cout << " -> children: ";
            Child* childNode = node->children.head;
            while (childNode != nullptr) {
                cout << "(" << childNode->node->strIndex << ", " << childNode->node->suffixIndex << ") ";
                childNode = childNode->next;
            }
        }
        cout << endl;
        if (node->children.size != 0) {
            Child* childNode = node->children.head;
            while (childNode != nullptr) {
                traverseAndPrint(childNode->node, word, level + 1);
                childNode = childNode->next;
            }
        }
    }
    void search(char* str){
        cout<<"word: "<<this->word<<"   substring: " <<str <<" :";
        int index = 0;
        //create node that points to root
        TreeNode* tempNode = root;
        //iterate over the query string
        while(index < strlen(str)){
            //get not that starts with char in query string
            TreeNode* childNode = findMatchingNodes(tempNode,str[index]);
            //if no matching node -> no pattern found
            if(childNode == nullptr){
                cout <<"pattern not found\n";
                return;
            }
            //get start index of edge
            int edgeStart = childNode->strIndex;
            int min;
            if(childNode->suffixIndex == -1){
                min = getMinChildIndex(childNode->children);
            }
            //get end index of edge
            int edgeEnd = (childNode->suffixIndex == -1) ? (min-childNode->strIndex-1)+childNode->strIndex : wordLength-1;
            //loop over edge
            for(int m = edgeStart; m <= edgeEnd; m++){
                if(index >= strlen(str)){
                    break;
                }
                //if match -> increment index
                if(word[m]==str[index]){
                    index++;
                }
                    //no match -> pattern not found
                else{
                    cout << "pattern not found\n";
                    return;
                }
            }
            //make tempNode points to current node to iterate over children(children of children)
            tempNode = childNode;
        }
        //print nodes with their children(suffixes that match)
        printMatchingNodes(tempNode);
        cout << endl;
    }
    //prints suffixes of matching nodes in search
    void printMatchingNodes(TreeNode* node){
        //if one matching node (no children)
        if(node->children.size == 0){
            cout << node->suffixIndex <<" ";
            return;
        }
        //recursively prints suffixes of children
        Child * child = node->children.head;
        while(child != nullptr){
            printMatchingNodes(child->node);
            child = child->next;
        }
    }
};
int main() {
    //testcase #1
    cout << "testcase #1" << endl;
    char word1[] = "bananabanaba$";
    SuffixTree tree1(word1);
    tree1.constructTree();
    tree1.search("ana");
    tree1.search("naba");
    tree1.search("meme");

    //testcase #2
    cout <<endl<< "testcase #2" << endl;
    char word2[] = "banana$";
    SuffixTree tree2(word2);
    tree2.constructTree();
    tree2.search("banaaanaaa");
    tree2.search("banana");
    tree2.search("ana$");
    //testcase #3
    cout <<endl<< "testcase #3" << endl;
    char word3[] = "mamamia$";
    SuffixTree tree3(word3);
    tree3.constructTree();
    tree3.search("amia$");
    tree3.search("ama");
    tree3.search("papapia");

    //testcase #4
    cout<<endl << "testcase #4" << endl;
    char word4[] = "hellohihehehe$";
    SuffixTree tree4(word4);
    tree4.constructTree();
    tree4.search("he");
    tree4.search("llo");
    tree4.search("help");

    //testcase #5
    cout <<endl<< "testcase #5" << endl;
    char word5[] = "doyoulovegrapes?$";
    SuffixTree tree5(word5);
    tree5.constructTree();
    tree5.search("gram");
    tree5.search("o");
    tree5.search("?$");

    //testcase #6
    cout <<endl<< "testcase #6" << endl;
    char word6[] = "ilovecoffeeee$";
    SuffixTree tree6(word6);
    tree6.constructTree();
    tree6.search("e");
    tree6.search("fe");
    tree6.search("cofi");

    //testcase #7
    cout <<endl<< "testcase #7" << endl;
    char word7[] = "peaspeanutbutterbuffs$";
    SuffixTree tree7(word7);
    tree7.constructTree();
    tree7.search("pea");
    tree7.search("bu");
    tree7.search("buffer");

    //testcase #8
    cout <<endl<< "testcase #8" << endl;
    char word8[] = "freedomforpalestinefreepalestine$";
    SuffixTree tree8(word8);
    tree8.constructTree();
    tree8.search("ree");
    tree8.search("free");
    tree8.search("palestine");

    //testcase #9
    cout <<endl<< "testcase #9" << endl;
    char word9[] = "icerollicecubes$";
    SuffixTree tree9(word9);
    tree9.constructTree();
    tree9.search("ice");
    tree9.search("ce");
    tree9.search("bes$");
    //testcase #10
    cout <<endl<< "testcase #10" << endl;
    char word10[] = "cornsyrupcornflackesunicorns$";
    SuffixTree tree10(word10);
    tree10.constructTree();
    tree10.search("uni");
    tree10.search("corn");
    tree10.search("addda");

}