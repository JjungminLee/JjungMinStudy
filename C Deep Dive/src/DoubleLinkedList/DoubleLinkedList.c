#include "DoubleLinkedList.h"

// 노드 생성
// Node*는 Node가리키는 포인터고 이걸 수정하려면 주소를 건들여야함
// 그때 Node**!
// **은 포인터 변수의 주소를 가르키는 포인터

// Node**는 구조체배열 Node가 아니라 Node*를 쓴거는 동적할당 하기 위함
Node* DLL_CreateNode(ElementType NewData){
    Node* NewNode =(Node*)malloc(sizeof(Node));
    NewNode->Data=NewData;
    NewNode->PrevNode=NULL;
    NewNode->NextNode=NULL;
    return NewNode; //노드의 주소 반환
}

// 노드 소멸
void DLL_DestroyNode(Node* Node){
    free(Node);
}

// 노드 추가
void SLL_AppendNode(Node** Head,Node* NewNode){
    //헤드 노드가 NUll이면 새로운 노드가 Head가 된다.
    if( (*Head)==NULL){
        *Head=NewNode;
    }else{
        // 테일을 찾아 NewNode를 연결
        Node* Tail=(*Head);
        while(Tail->NextNode!=NULL){
            Tail = Tail->NextNode;
        }
        Tail->NextNode=NewNode;
        // 기존 테일을 새로운 테일의 PrevNode가 가리키게
        NewNode->PrevNode=Tail;
    }
}

//노드 삽입

void DLL_InsertAfter(Node* Current,Node* NewNode){
    NewNode->NextNode=Current->NextNode;
    NewNode->PrevNode=Current;
    if(Current->NextNode!=NULL){// 현재 시점에서 뒤에 다른 노드가 있었다면 뒤에 노드랑 새로 삽입되는 노드 이어주기
        Current->NextNode->PrevNode=NewNode;
        Current->NextNode=NewNode;
    }
}


//노드제거
void DLL_RemoveNode(Node** Head,Node* Remove){
    /*
    삭제할 노드의 NextNode포인터가 가리키던 노드를 이전 노드의 NextNode포인터가 가리키게 바꾸고
    삭제할 노드의 PrevNode포인터가 가리키던  노드를 다음 노드의 PrevNode포인터가 가리키게 바꾼다.
    삭제할 노드의 NextNode와 PrevNode는 NULL로 초기화!
    */
    if(*Head == Remove){
        *Head = Remove->NextNode; // 헤드를 현재 헤드 다음의 애로
        if((*Head)!=NULL){
            (*Head)->PrevNode=NULL;
        }
        Remove->PrevNode= NULL;
        Remove->NextNode=NULL;
    }else{
        Node* Temp = Remove;
        if(Remove->PrevNode!=NULL){
            // 삭제하는 애의 앞의 노드와 삭제하는 애의 뒤의 노드 연결
            Remove->PrevNode->NextNode=Temp->NextNode;

        }
        if(Remove->NextNode!=NULL){
            Remove->NextNode->PrevNode=Temp->PrevNode;

        }
        Remove->PrevNode=NULL;
        Remove->NextNode=NULL;
    }
}

// 노드 탐색
// Head부터 차근차근 Location에 간다!
Node* DLL_GetNodeAt(Node* Head, int Location){
    Node* Current = Head;

    while(Current!=NULL && (--Location)>=0){
        Current = Current->NextNode;

    }
}

//노드 개수 세기
int DLL_GetNodeCount(Node* Head){
    unsigned int Count = 0;
    Node* Current = Head;

    while(Current!=NULL){
        Current = Current->NextNode;
        Count++;
    }
    return Count;
}

int main(){
    Node* NewNode = NULL;
    Node* List = NULL;

    for(int i=0;i<5;i++){
        NewNode = DLL_CreateNode(i);
        DLL_AppendNode(&List,NewNode);
    }
}