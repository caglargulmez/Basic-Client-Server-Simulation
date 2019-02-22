#include<stdio.h>
#include<stdlib.h>

#define MS 5

typedef struct Item{
  int queueSize;
  int stackSize;
  int top;
  int logCount;
  int rear;
  int front;
  char* queue;
  char* stack;
}Item;

int enQueue(Item *myItem, char val);
int deQueue(Item *myItem);
int isFull(Item *myItem);
int isEmpty(Item *myItem);
int front(Item *myItem);
int push(Item *myItem, char val);
int isStackFull(Item *myItem);
int isEmpty(Item *myItem);
int pop(Item *myItem);
int pushHistory(Item *myItem, char val);
int isStackEmpty(Item *myItem);

//int argc, char* argv[]
int main(int argc, char* argv[]){
  char** logHistory;
  int i, j, number;
  int count;
  int queue_size, stack_size;
  FILE *fptr;
  fptr = fopen(argv[1], "r");
  fscanf(fptr,"%d\n",&count);
  Item* items;
  items = (Item*)malloc(count * sizeof(Item));
  int *queueStackSize;
  queueStackSize = (int *)malloc(count * sizeof(int));
  for(i = 0; i < count ; i++){
  		queueStackSize = (int *) malloc(2*sizeof(int));
  }

  for (number = 0; number < count; number++) {
      fscanf(fptr,"%d %d\n", &queue_size, &stack_size );
      queueStackSize[0] = queue_size;
      queueStackSize[1] = stack_size;
      //creating clients & a server
      items[number].rear = -1;
      items[number].front = -1;
      items[number].top = -1;
      items[number].queueSize = queue_size;
      items[number].stackSize = stack_size;
      items[number].logCount = 0;
      items[number].queue = (char*)malloc(queue_size * sizeof(char));
      items[number].stack = (char*)malloc(stack_size * sizeof(char));
  }
  int count2;
  char command;
  char client_server;
  char element;
  FILE *fptr2;
  fptr2 = fopen(argv[2], "r");
  fscanf(fptr2,"%d\n",&count2); //stores number of operation

  //creates 2-d array for logHistory for each member of server
  logHistory = (char**)malloc(count * sizeof(char*));
  for(i = 0; i < count ; i++){
  		logHistory[i] = (char*) malloc(count2*sizeof(char));
  }

  //starts doing operations
  for(number = 0; number < count2; number++){
    fscanf(fptr2, "%c %c %c\n", &command, &client_server, &element);
    int cS = client_server - 48; //converts char to integer
    //cS is middle input of each lines of input2.txt
    if(command == 'A'){ //adding to clients' queues
      if(isFull(&items[cS-1]) == 1){
        logHistory[cS-1][items[cS-1].logCount] = '1';
        items[cS-1].logCount++;
      }else{
        enQueue(&items[cS-1], element);
      }
    }else if(command == 'I'){ //adding to clients' stacks
      if(isStackFull(&items[cS-1]) == 1){
        //error character when client's stack is full
        logHistory[cS-1][items[cS-1].logCount] = '2';
        items[cS-1].logCount++;
      }else{
        push(&items[cS-1], element);
      }
    }else if(command == 'S'){ //command 'S' case
      if(isStackEmpty(&items[cS-1]) == 0){
        //stack has more priority,
        //then first of all check if stack is not empty
        if(isFull(&items[count-1]) == 0){
          enQueue(&items[count-1], items[cS-1].stack[items[cS-1].top]);
        }else{
          logHistory[count-1][items[count-1].logCount] = '1';
          items[count-1].logCount++;
        }
        logHistory[cS-1][items[cS-1].logCount] = items[cS-1].stack[items[cS-1].top];
        items[cS-1].logCount++;
        pop(&items[cS-1]);
      }else if(isEmpty(&items[cS-1]) == 0){
        //secondly, check if queue is not empty
        char process = items[cS-1].queue[items[cS-1].front];
        if(isFull(&items[count-1]) == 0){
          enQueue(&items[count-1], process);
        }else{
          logHistory[count-1][items[count-1].logCount] = '1';
          items[count-1].logCount++;
        }
        logHistory[cS-1][items[cS-1].logCount] = process;
        items[cS-1].logCount++;
        deQueue(&items[cS-1]);
      }
      else{ //
        logHistory[cS-1][items[cS-1].logCount] = '3';
        items[cS-1].logCount++;
      }
    }else{
      if(isStackEmpty(&items[count-1]) == 0){
        char processS = items[count-1].stack[items[count-1].top];
        logHistory[count-1][items[count-1].logCount] = processS;
        items[count-1].logCount++;
        pop(&items[count-1]);
      }else if(isEmpty(&items[count-1]) == 0){
        char processQ = items[count-1].queue[items[count-1].front];
        logHistory[count-1][items[count-1].logCount] = processQ;
        items[count-1].logCount++;
        deQueue(&items[count-1]);
      }else{
        logHistory[count-1][items[count-1].logCount] = '3';
        items[count-1].logCount++;
      }
    }//end of Operate Commands
  }//end of for loop for starting all commands
  FILE *outFile;
  outFile = fopen(argv[3], "w");
  int k;
  for(number = 0; number < count; number++){
    for(k = 0; k<items[number].logCount; k++){
      fprintf(outFile, "%c ", logHistory[number][k]);
      printf("%c ", logHistory[number][k]);
    }
    fprintf(outFile, "\n");
    printf("\n");
  }
  fclose(fptr);
  fclose(fptr2);
  fclose(outFile);
  return 0;
}
//Stack implementation
int isStackFull(Item *myItem){
  if(myItem -> top == myItem -> stackSize -1){
    return 1;
  }
  return 0;
}
int isStackEmpty(Item *myItem){
  if(myItem -> top == -1){
    return 1;
  }
  return 0;
}
int push(Item *myItem, char val){
    if(isStackFull(myItem)){
      return 0;
    }else{
      myItem -> top ++;
    }
  myItem -> stack[myItem -> top] = val;
  return 1;
}
int pop(Item *myItem){
  char data;
  if(isStackEmpty(myItem)){
    return 0;
  }else{
    data = myItem -> stack[myItem -> top];
    myItem -> top--;
    return data;
  }
}
char top(Item *myItem){
  if(!isStackEmpty(myItem)){
    return myItem -> stack[myItem -> top];
  }
}

// Queue implementation
int isFull(Item *myItem){
  if(((myItem -> rear) + 1) % myItem -> queueSize == myItem -> front){
    return 1;
  }
  return 0;
}
int isEmpty(Item *myItem){
  if(myItem -> front == -1 && myItem -> rear == -1){
    return 1;
  }
  return 0;
}
int enQueue(Item *myItem, char val){
  if(((myItem -> rear) + 1) % myItem -> queueSize == myItem -> front){ //isfull check
    return 0;
  }else if(isEmpty(myItem)){
    myItem -> front = 0;
    myItem -> rear = 0;
  }
  else{
    myItem -> rear = ((myItem -> rear) + 1) % myItem -> queueSize;
  }
  myItem -> queue[myItem -> rear] = val;
  return 1;
}
int deQueue(Item *myItem){
  if(isEmpty(myItem)){
    return 0;
  }else if(myItem -> rear == myItem -> front){
    myItem -> rear = -1;
    myItem -> front = -1;
  }else{
    myItem -> front = ((myItem -> front) + 1) % myItem -> queueSize;
  }
  return 1;
}
int front(Item *myItem){
  if(isEmpty(myItem)){
    return -1;
  }else{
    return myItem -> queue[myItem -> front];
  }
}
