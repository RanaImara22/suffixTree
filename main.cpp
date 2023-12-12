#include <iostream>
#include <utility>
using namespace std;
struct Child;
class Children;
class TreeNode;
class Child {
public:
    TreeNode* node;
    Child* next;
    Child() {
        next = nullptr;
    }
    //    ~Child() {
    //        delete node;
    //        delete next;
    //    }
    Child(TreeNode* node, Child* next) : node(node), next(next) {}
};
class Children {
public:
    Child* head;
    int size = 0;
    void deleteList(Child* node) {
        if (node != nullptr) {
            deleteList(node->next);
            delete node;
        }
    }

    //    ~Children() {
    //        deleteList(head);
    //    }
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
        int len = 0;
        while (word[len] != '\0') {
            len++;
        }
        wordLength = len;

    }
    void constructTree() {
        for (int i = 0; i < wordLength; ++i) {
            addNode(i, wordLength, root);
        }
    }
    void addNode(int index, int current_index, TreeNode* currentNode) {
        // check if there are no children for current node

        // current node has children


        current_index-=index;
        while(current_index>0){
            if (currentNode->children.size == 0) {
                Child* child = currentNode->children.addChild(new TreeNode());
                child->node->strIndex = index;
                child->node->suffixIndex =wordLength - current_index;
                return;
            }
            Child* childNode = currentNode->children.head;
            while (childNode != nullptr) {
                int edgeStart = childNode->node->strIndex;
                int edgeEnd = (childNode->node->suffixIndex == -1) ? current_index : childNode->node->suffixIndex;
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
                    // Case 1.2: Split the edge
                    //splitted suffix
                    TreeNode* splittedSuffix = new TreeNode();
                    splittedSuffix->strIndex = edgeStart + commonLen;
                    splittedSuffix->suffixIndex = edgeEnd;
                    //old suffix node
                    childNode->node->strIndex = edgeStart;
                    childNode->node->suffixIndex = -1;
                    //new suffix node
                    TreeNode* newNode = new TreeNode();
                    newNode->strIndex = index + commonLen;
                    newNode->suffixIndex = index;
                    childNode->node->children.addChild(splittedSuffix);
                    childNode->node->children.addChild(newNode);
                    return;
                }
                childNode = childNode->next;
            }
            // Case 1: Suffix is a prefix of the edge
            Child* child = currentNode->children.addChild(new TreeNode());
            child->node->strIndex = index;
            child->node->suffixIndex = index;
            break;
        }
        // Case 3: Suffix forms a new leaf node
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
    const char* text = "banana$";  // Replace with your actual input text

    // Create an instance of the SuffixTree class
    SuffixTree suffixTree(text);

    // Build the suffix tree
    suffixTree.constructTree();
    suffixTree.printSuffixTree(suffixTree);
    // Your additional operations on the suffix tree, if needed
//    suffixTree.print();
}