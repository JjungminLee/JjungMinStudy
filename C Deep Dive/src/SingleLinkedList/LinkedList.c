#include "LinkedList.h"

// 노드 생성
// Node*는 Node가리키는 포인터고 이걸 수정하려면 주소를 건들여야함
// 그때 Node**!
// **은 포인터 변수의 주소를 가르키는 포인터

// Node**는 구조체배열 Node가 아니라 Node*를 쓴거는 동적할당 하기 위함
Node* SLL_CreateNode(ElementType NewData){
    Node* NewNode =(Node*)malloc(sizeof(Node));
    NewNode->Data=NewData;
    NewNode->NextNode=NULL;
    return NewNode; //노드의 주소 반환
}

// 노드 소멸
void SLL_DestroyNode(Node* Node){
    free(Node);
}

// 노드 추가
void SLL_AppendNode(Node** Head,Node* NewNode){
    //헤드 노드가 NUll이면 새로운 노드가 Head가 된다.
    if( (*Head)==NULL){
        *Head=NewNode;
    }else{
        Node* Tail=(*Head);
        while(Tail->NextNode!=NULL){
            Tail = Tail->NextNode;
        }
        Tail->NextNode=NewNode;
    }
}

//노드 삽입

void SLL_InsertAfter(Node* Current,Node* NewNode){
    NewNode->NextNode=Current->NextNode;
    Current->NextNode=NewNode;
}

void SLL_InsertNewHead(Node** Head,Node* NewHead){
    if(Head==NULL){
        (*Head)=NewHead;
    }else{
        NewHead->NextNode=(*Head);
        (*Head)=NewHead;
    }
}

//노드제거
void SLL_RemoveNode(Node** Head,Node* Remove){
    if(*Head == Remove){
        *Head = Remove->NextNode; // 헤드를 현재 헤드 다음의 애로
    }else{
        Node* Current = *Head;
        while(Current!=NULL && Current->NextNode!=Remove){
            Current=Current->NextNode;
        }
        if(Current!=NULL){
            Current->NextNode=Remove->NextNode;
        }
    }
}

// 노드 탐색
// Head부터 차근차근 Location에 간다!
Node* SLL_GetNodeAt(Node* Head, int Location){
    Node* Current = Head;

    while(Current!=NULL && (--Location)>=0){
        Current = Current->NextNode;

    }
}

//노드 개수 세기

int SLL_GetNodeCount(Node* Head){
    int Count = 0;
    Node* Current = Head;

    while(Current!=NULL){
        Current = Current->NextNode;
        Count++;
    }
    return Count;
}

int main(){
    Node* NewNode = NULL;
    NewNode = SLL_CreateNode(-1);
    Node* List = NULL;

    SLL_AppendNode(List,NewNode);
}