#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
	
	Program Definition: A program which creates two binary trees of word order and frequency optimum order from the words
	that has been given to program with input.txt file in the same directory. The program uses alphabetical and height calculation
	functions to create the optimal trees for both situations.
	
	Used Libraries:
		stdlib.h for memory allocation functions.
		string.h for strtok function to create data from input lines.
		math.h for power function.
	
	Program is explained by details in the following functions: newFrequencyNodeByTree(156), addFrequencyNodeByTree(176).
	
	A detailed explanation can also be found in CSE report.
*/

//Create binary search tree node
struct treeNode{
	int id;
	char word[50];
	int frequency;
	struct treeNode *leftNode;
	struct treeNode *rightNode;
};

typedef struct treeNode treeNode;

int minHeight = 0, totalNodes = 0, currentHeight = 0;

//Variables to be used in total access time
int height = 0, totalHeight = 0;

int alphabeticalOrder(char *word1, char *word2){
	int i = 0;
	//Look for every character to compare the words' priority to each other
	for(i = 0; i < 256; i++){
		//If the word is uppercase convert it to lowercase
		if(word1[i] >= 65 && word1[i] <= 90) word1[i] += 32;
		if(word1[i] != word2[i]){
			if(word1[i] > word2[i]) return 2; 
			else return 1;
		}
	}
	return 0;
}

//Adds a word node to the binary tree
treeNode *addWordNode(treeNode *node ,int id, char *word, int frequency){
	if(node == NULL){
		//Create a root node and update its data
		treeNode *newWordNode = (treeNode*) malloc(sizeof(treeNode));
		newWordNode->id = id;
		strcpy(newWordNode->word, word);
		newWordNode->frequency = frequency;
		newWordNode->leftNode = newWordNode->rightNode = NULL;
		//Return the pointer of the new node
		return newWordNode;
	}
	//If the word is alphabetically prior to the current node update left node if it is not then update the right node
	if(alphabeticalOrder(word, node->word) == 1) node->leftNode = addWordNode(node->leftNode, id, word, frequency);
	else node->rightNode = addWordNode(node->rightNode, id, word, frequency);
	return node;
}

//Adds a frequency node to the binary tree
treeNode *addFrequencyNode(treeNode *node ,int id, char *word, int frequency){
	if(node == NULL){
		//Create a root node and update its data
		treeNode *newWordNode = (treeNode*) malloc(sizeof(treeNode));
		newWordNode->id = id;
		strcpy(newWordNode->word, word);
		newWordNode->frequency = frequency;
		newWordNode->leftNode = newWordNode->rightNode = NULL;
		//Return the pointer of the new node
		return newWordNode;
	}
	//If the frequency is lower than the current node's frequency left node will be updated otherwise update right node
	if(node->frequency > frequency) node->leftNode = addFrequencyNode(node->leftNode, id, word, frequency);
	else node->rightNode = addFrequencyNode(node->rightNode, id, word, frequency);
	return node;
}

int readFile(treeNode **rootNode, int readMode){
	FILE *file;
	file = fopen("content_548894379652", "r");
	//If the file is not opened return false
	if(!file) return 0;
	char inputString[255], word[50], *token;
	int id, frequency;
	//Read input file line by line
	while(fgets(inputString, 255, file) != NULL){
		//Get the id from line string
		token = strtok(inputString, ",");
		id = atoi(token);
		//Get the word from line string
		token = strtok(NULL, ",");
		strcpy(word, token);
		//Get the frequency from line string
		token = strtok(NULL, ",");
		frequency = atoi(token);
		//Add new node to binary tree
		if(readMode == 1) *rootNode = addWordNode(*rootNode, id, word, frequency);
		else *rootNode = addFrequencyNode(*rootNode, id, word, frequency);
	}
	return 1;
}

void calculateHeight(treeNode *node){
	//Calculate height function will increase the height in every left node and height will be decreased after the right node
	if(node==NULL) return;
	//Increase height by 1
	height++;
	//Update total height
	totalHeight += (height * node->frequency);
	//Look to the left node
	calculateHeight(node->leftNode);
	//Print the binary tree
	printf("%s %d \n", node->word, node->frequency);
	//Look to the right node
	calculateHeight(node->rightNode);
	//Decrease height by 1
	height--;
}

void calculateTotalAccessTime(treeNode **node){
	//Calculate the total height
	height = totalHeight = 0;
	calculateHeight(*node);
	//Print access time
	printf("\nAccess Time: %d\n\n", (totalHeight));
}

treeNode *updateFrequencyNodeByTree(treeNode *newNode, treeNode *node){
	if(newNode == NULL && node->id != 0){
		//Create a new node and update its data
		newNode = (treeNode*) malloc(sizeof(treeNode));
		newNode->id = node->id;
		strcpy(newNode->word, node->word);
		newNode->frequency = node->frequency;
		newNode->leftNode = NULL;
		newNode->rightNode = NULL;
		//Set node id as zero to avoid using the node again
		node->id = 0;
		free(node);
		//In every level of the tree, min height power of two nodes
		if(++totalNodes == ((int)pow(2.0, minHeight))) {
			//If the node limit is reached, update the min height and set total nodes to zero
			totalNodes = 0;
			minHeight++;
		}
		return newNode;
	}
	return newNode;
}

treeNode *newFrequencyNodeByTree(treeNode *newNode, treeNode *node){
	/*
		This function gets the node that will be added to the new binary tree (Optimum binary tree).
		Function looks for the best location for the node to be placed.
		Since the best location is the closest location to the root node, the nodes will be placed with some height calculations.
		New binary tree's nodes have been read from left to right while their height is updated to the current height variable.
		If the min height is lower than the current height recursive function will be returned.
		For other heights which is lower than min height the node can be placed in left to right order.
		If there is an empty node in left to right order the node will be updated.
		For min height calculations look to the update frequency node by tree function.
	*/
	if(minHeight < currentHeight || node == NULL) return newNode;
	currentHeight++;
	if(newNode != NULL && node != NULL) newNode->leftNode = newFrequencyNodeByTree(newNode->leftNode, node);
	newNode = updateFrequencyNodeByTree(newNode, node);
	if(newNode != NULL && node != NULL) newNode->rightNode = newFrequencyNodeByTree(newNode->rightNode, node);
	currentHeight--;
	return newNode;
}

treeNode *addFrequencyNodeByTree(treeNode *node, treeNode **newNode){
	/*
		This function reads the root node of the first frequency tree (BST tree of frequency).
		Function starts reading the nodes from right to left which gives the first root as the highest frequency.
		The highest frequency will be send to new frequency node by tree function to be added to the optimum binary tree.
		After all nodes have been read, the function will return the new tree's root node the main function.
	*/
	if(node==NULL) return *newNode;
	addFrequencyNodeByTree(node->rightNode, newNode);
	*newNode = newFrequencyNodeByTree(*newNode, node);
	addFrequencyNodeByTree(node->leftNode, newNode);
	return *newNode;
}

int main(int argc, char *argv[]) {
	//Create root node
	treeNode *rootNode = NULL;
	treeNode *newRootNode = NULL;
	//If file is not opened read error will be printed.
	printf("BINARY TREE OF WORD ORDER\n--------------------------\n");
	if(readFile(&rootNode, 1) == 0) printf("File read error");
	calculateTotalAccessTime(&rootNode);
	rootNode = NULL;
	printf("BINARY TREE OF OPTIMUM FREQUENCY ORDER\n-------------------------------\n");
	if(readFile(&rootNode, 2) == 0) printf("File read error");
	newRootNode = addFrequencyNodeByTree(rootNode, &newRootNode);
	calculateTotalAccessTime(&newRootNode);
}
