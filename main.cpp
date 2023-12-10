#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <iostream>
using namespace std;

class Node;
class SuffixTreeNode;
class Node {
public:
    SuffixTreeNode *value;
    Node *Next = nullptr;
    Node(SuffixTreeNode *value, Node *Next) : value(value), Next(Next) {}
    ~Node() {
        delete value;
        delete Next;
    }
};

class Children {
private:
    void deleteList(Node *node) {
        if (node != nullptr) {
            deleteList(node->Next);
            delete node;
        }
    }

public:

    Node *head = nullptr;
    int size;
    ~Children() {
        deleteList(head);
    }
    void add(SuffixTreeNode *node){
        Node *node1= new Node(node, nullptr);
        if(head == nullptr){
            head = node1;
        }
        else{
            Node *current= head ;
            while(current != nullptr){
                current=current->Next;
            }
            current=node1;
        }
    }
//    Node *node;
};
class SuffixTreeNode {
public:
    Children *children;
    int str_index;  // to store the starting index of the substring
    int suffix_index;

    SuffixTreeNode() : children(nullptr), str_index(-1), suffix_index(-1) {}
    ~SuffixTreeNode() {
        delete children;
    }

    void print(int depth = 0) {
        for (int i = 0; i < depth; ++i) {
            std::cout << "  ";
        }

        std::cout << "Edge: [" << str_index << ", " << suffix_index << "]\n";

        if (children != nullptr) {
            Node *childNode = children->head;
            while (childNode != nullptr) {
                childNode->value->print(depth + 1);
                childNode = childNode->Next;
            }
        }
    }
};

class SuffixTree {
private:
    char *input_txt;
    SuffixTreeNode *root;
//public:
//    // Constructor
//    SuffixTree(char *txt) {
//        input_txt = strdup(txt);
//        root = new SuffixTreeNode();
//    }

    void insertSuffix(int suffix_index, int current_index, SuffixTreeNode *currentNode) {
        // Check if there are no children for the current node
        if (currentNode->children == nullptr) {
            currentNode->children = new Children();
            currentNode->children->add(new SuffixTreeNode());
        }

        // Traverse through the children to find the correct insertion point
        Node *childNode = currentNode->children->head;
        while (childNode != nullptr) {
            int edgeStart = childNode->value->str_index;
            int edgeEnd = (childNode->value->suffix_index == -1) ? current_index : childNode->value->suffix_index;

            // Case 1: Suffix is a prefix of the edge
            if (input_txt[suffix_index] == input_txt[edgeStart]) {
                int commonLength = 0;
                while (input_txt[suffix_index + commonLength] == input_txt[edgeStart + commonLength]) {
                    commonLength++;
                }

                // Case 1.1: Suffix is completely covered by the current edge
                if (commonLength == edgeEnd - edgeStart) {
                    insertSuffix(suffix_index + commonLength, current_index, childNode->value);
                    return;
                }

                // Case 1.2: Split the edge
//                SuffixTreeNode *newNode = new SuffixTreeNode();
//                newNode->str_index = edgeStart + commonLength;
//                newNode->suffix_index = edgeEnd;
//                childNode->value->str_index = edgeStart;
//                childNode->value->suffix_index = edgeStart + commonLength;
//                newNode->children = new Children();
//                newNode->children->add(new SuffixTreeNode());
//                newNode->children->add(new SuffixTreeNode());
//                newNode->children->head->value->str_index = suffix_index + commonLength;
//                newNode->children->head->value->suffix_index = current_index;
//                childNode->value->children = newNode->children;
//                return;
            }

            // Case 2: Suffix shares only part of the edge
//            if (input_txt[suffix_index] != input_txt[edgeStart] &&
//                input_txt[suffix_index] == input_txt[edgeStart + 1]) {
            // Split the edge
//                SuffixTreeNode *newNode = new SuffixTreeNode();
//                newNode->str_index = edgeStart + 1;
//                newNode->suffix_index = edgeEnd;
//                childNode->value->str_index = edgeStart;
//                childNode->value->suffix_index = edgeStart + 1;
//                newNode->children = childNode->value->children;
//                childNode->value->children = new Children();
//                childNode->value->children->add(newNode);
//                childNode->value->children->add(new SuffixTreeNode());
//                childNode->value->children->head->value->str_index = suffix_index + 1;
//                childNode->value->children->head->value->suffix_index = current_index;
//                return;
            //           }

            // Move to the next child
            childNode = childNode->Next;
        }

        // Case 3: Suffix forms a new leaf node
        currentNode->children->add(new SuffixTreeNode());
        currentNode->children->head->value->str_index = suffix_index;
        currentNode->children->head->value->suffix_index = current_index;
    }

public:
    // Constructor
    SuffixTree(char *txt) {
        input_txt = strdup(txt);
        root = new SuffixTreeNode();
        buildSuffixTree();
    }
    ~SuffixTree() {
        delete root;
        free(input_txt);
    }
    // Public function to build the suffix tree
    void buildSuffixTree() {
        int n = strlen(input_txt);
        for (int i = 0; i < n; ++i) {
            insertSuffix(i, n - 1, root);
        }
    }
    void print() {
        root->print();
    }

};
int main() {
    char text[] = "Banana$";  // Replace with your actual input text

    // Create an instance of the SuffixTree class
    SuffixTree suffixTree(text);

    // Build the suffix tree
    suffixTree.buildSuffixTree();

    // Your additional operations on the suffix tree, if needed
//    suffixTree.print();
    return 0;
}
