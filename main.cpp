#include <iostream>
#include <utility>
#include <cstring>

using namespace std;
struct Child;
class Children;
class TreeNode;
class Child {
public:
    TreeNode* node;
    Child* next;
    Child() {
        node = nullptr;
        next = nullptr;
    }
//        ~Child() {
//            delete node;
//            delete next;
//        }
    Child(TreeNode* node, Child* next) : node(node), next(next) {}
};
class Children {
public:
    Child* head;
    int size = 0;
    void deleteList() {
        this->head = nullptr;
        size = 0;
    }

//        ~Children() {
//            deleteList(head);
//        }
    Child* addChild(Child* newChild) {
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
class TreeNode {
public:
    Children children;
    int strIndex;
    int suffixIndex;
    TreeNode() {
        strIndex = -1;
        suffixIndex = -1;
        children.head = nullptr;
    }
};
class SuffixTree {
public:
    TreeNode* root;
    const char* word;
    int wordLength;

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
    void constructTree() {
        for (int i = 0; i < wordLength; ++i) {
            addNode(i, wordLength, root);
        }
    }
    void addNode(int index, int current_index, TreeNode* currentNode) {
        current_index-=index;

        while(current_index>0){
            if (currentNode->children.size == 0) {
                Child* child = currentNode->children.addChild(new TreeNode());
                child->node->strIndex = index;
                child->node->suffixIndex =wordLength - current_index;
                return;
            }
            Child *currentChild = reinterpret_cast<Child *>(currentNode);
            Child* childNode = currentNode->children.head;
            while (childNode != nullptr) {
                currentChild = childNode;
                int edgeStart = childNode->node->strIndex;
                int edgeEnd = (childNode->node->suffixIndex == -1) ? childNode->node->children.head->node->strIndex-childNode->node->children.head->node->suffixIndex : childNode->node->suffixIndex;
                if (word[index] == word[edgeStart]) {
                    int commonLen = 0;
                    while (word[index + commonLen] == word[edgeStart + commonLen]) {
                        commonLen++;
                        current_index--;
                    }
                    // Case 1.1: Suffix is completely covered by the current edge
                    if (commonLen == edgeEnd - edgeStart) {
                        addNode(index + commonLen, current_index, childNode->node);
                        return;
                    }
                    //new suffix node
                    TreeNode* newNode = new TreeNode();
                    newNode->strIndex = index + commonLen;
                    newNode->suffixIndex = index;
                    if(childNode->node->suffixIndex != -1){
                        //splitted suffix
                        TreeNode* splittedSuffix = new TreeNode();
                        splittedSuffix->strIndex = edgeStart + commonLen;
                        splittedSuffix->suffixIndex = edgeEnd;
                        //old suffix node
                        childNode->node->strIndex = edgeStart;
                        childNode->node->suffixIndex = -1;
                        childNode->node->children.addChild(splittedSuffix);
                        childNode->node->children.addChild(newNode);

                        return;
                    }
                    else{
                        //new Me
                        TreeNode *newMe = new TreeNode();
                        newMe->strIndex = childNode->node->strIndex + commonLen;
                        newMe->suffixIndex = -1;
                        //add children of old parent to become new Node's children
                        Child *add = childNode->node->children.head;
                        for (int i = 0; i < childNode->node->children.size; ++i) {
                            newMe->children.addChild(add->node);
                            add = add->next;
                        }
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
            child->node->suffixIndex = index;
            break;
        }

    }
    void traverseAndPrint(TreeNode* node, const char* word, int level = 0) {
        if (!node) {
            return;
        }

        // Print the current node
        for (int i = 0; i < level; ++i) {
            cout << "  ";
        }

        cout << "[" << node->strIndex << ", " << node->suffixIndex << "]";

        if (node->children.size != 0) {
            cout << " -> children: ";
            Child* childNode = node->children.head;
            while (childNode != nullptr) {
                cout << "[" << childNode->node->strIndex << ", " << childNode->node->suffixIndex << "] ";
                childNode = childNode->next;
            }
        }

        cout << endl;

        // Recursively traverse children
        if (node->children.size != 0) {
            Child* childNode = node->children.head;
            while (childNode != nullptr) {
                traverseAndPrint(childNode->node, word, level + 1);
                childNode = childNode->next;
            }
        }
    }

    TreeNode* getRoot() const {
        return root;
    }

    void setRoot(TreeNode* root) {
        SuffixTree::root = root;
    }

    void setWord(char* word) {
        SuffixTree::word = word;
    }

    int getWordLength() const {
        return wordLength;
    }

    void setWordLength(int wordLength) {
        SuffixTree::wordLength = wordLength;
    }

    // Function to initiate traversal and printing
    void printSuffixTree(SuffixTree& suffixTree) {
        cout << "Suffix Tree:" << endl;
        traverseAndPrint(suffixTree.getRoot(), suffixTree.word);
    }

};
int main() {
    const char* text = "bananabanaba$";  // Replace with your actual input text

    // Create an instance of the SuffixTree class
    SuffixTree suffixTree(text);

    // Build the suffix tree
    suffixTree.constructTree();
    suffixTree.printSuffixTree(suffixTree);
    // Your additional operations on the suffix tree, if needed
//    suffixTree.print();
}