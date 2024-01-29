#ifndef LINEDLIST_H
    #define LINEDLIST_H


#include<stdio.h>
#include<stdlib.h>

typedef int ElementType;

typedef struct tagNode
{
    ElementType Data;
    struct tagNode* NextNode;
    /* data */
}Node;

Node* SLL_CreateNode(ElementType NewData);
void SLL_DestriyNode(Node* Node);
void SLL_AppendNode(Node** Head,Node* NewNode);
void SLL_InsertAfter(Node* Current,Node* NewNode);
void SLL_InsertNewHead(Node** Head,Node* NewNode);
void SLL_RemoveNode(Node** Head,Node* Remove);
Node* SLL_GetNodeAt(Node*Head, int Location);
int SLL_GetNodeCount(Node* Head);
#endif

