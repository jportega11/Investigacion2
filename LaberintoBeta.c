/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/

#include "simpletools.h"                      // Include simpletools header
#include "abdrive.h"                          // Include abdrive header
#include "ping.h"

#include<stdio.h>
#include<stdlib.h>

//Variables globales a utilizar
  int izq = 0;
  int der = 0;
  int ade = 0;
  int contador=0;
  
  
/* Stack has three properties. capacity stands for the maximum number of elements stack can hold.
   Size stands for the current size of the stack and elements is the array of elements */
typedef struct Stack
{
        int capacity;
        int size;
        int *elements;
}Stack;
/* crateStack function takes argument the maximum number of elements the stack can hold, creates
   a stack according to it and returns a pointer to the stack. */
Stack * createStack(int maxElements)
{
        /* Create a Stack */
        Stack *S;
        S = (Stack *)malloc(sizeof(Stack));
        /* Initialise its properties */
        S->elements = (int *)malloc(sizeof(int)*maxElements);
        S->size = 0;
        S->capacity = maxElements;
        /* Return the pointer */
        return S;
}
void pop(Stack *S)
{
        /* If stack size is zero then it is empty. So we cannot pop */
        if(S->size==0)
        {
                printf("Stack is Empty\n");
                return;
        }
        /* Removing an element is equivalent to reducing its size by one */
        else
        {
                S->size--;
        }
        return;
}
int top(Stack *S)
{
        if(S->size==0)
        {
                printf("Stack is Empty\n");
                exit(0);
        }
        /* Return the topmost element */
        return S->elements[S->size-1];
}
void push(Stack *S,int element)
{
        /* If the stack is full, we cannot push an element into it as there is no space for it.*/
        if(S->size == S->capacity)
        {
                printf("Stack is Full\n");
        }
        else
        {
                /* Push an element on the top of it and increase its size by one*/ 
                S->elements[S->size++] = element;
        }
        return;
}
//Funciones con las que gira el robot

void izquierda2(){
    drive_goto(-25,26);     
  }
void derecha2(){
    drive_goto(26,-25); 
  }
  
//Funciones para revisar a un lado
void izquierda(){
  drive_goto(-25,26);
  pause(10);
  if(ping_cm(8)>15){
      izq = 1;           
    }
    else
   {
     izq=0;
   } 

  }
  
void derecha(){
  drive_goto(52,-50);
  pause(10);
  if(ping_cm(8)>15){
      der = 1;
    }
    else
   {
     der=0;
   } 
  pause(10);
  drive_goto(-26,28);

  }  

void adelante(){
  if(ping_cm(8)>10){
      ade = 1;
    }
   else
   {
     ade=0;
   }     
  }

void paso(){
  int paso = 0;
  while(paso<40){
    paso = paso + 5;
    drive_goto(5,5);
    contador=contador+5;
    if(ping_cm(8)<7){
      break;
      }
   } 
}  
    
void suma(Stack *stade,Stack *stder,Stack *stizq,Stack *stcamino)
{
  int result=0;
  result=der+izq+ade;
  if(result>1)
  {
    push(stade,0);
    push(stizq,izq);
    if(ade==0)
    {
      push(stder,0);
       push(stcamino,contador);
       contador=0;
        
        
       push(stcamino,contador);
      push(stcamino,-2);
    } 
    else
    {
       push(stcamino,contador);
    contador=0;
    push(stcamino,contador);
      push(stder,der);
    } 
      
  }
  if(ade==0&&result==1)
  {
    if(izq==1)
    {
       push(stcamino,contador);
       push(stcamino,-1);
       contador=0;
    }
    if(der==1)
    {
       push(stcamino,contador);
       push(stcamino,-2);
       contador=0;
    }   
  }    
  if(result==0)
  {
    printf(" top camino es = %d cm\n"            // Display distance
           , top(stcamino));
    printf(" stder es = %d cm\n"            // Display distance
           , top(stder));
    printf(" stizq es = %d cm\n"            // Display distance
           , top(stizq));
    printf(" EL CONTADOR es = %d cm\n"            // Display distance
           , contador);
    push(stcamino,contador);
    contador=0;
    backtracking(stder,stizq,stcamino);
    //drive_goto(200,-200);
  }   
  printf(" la suma es = %d cm\n"            // Display distance
           , result);      
}
void backtracking(Stack *stder,Stack *stizq,Stack *stcamino)
{
  int valor=0;
  while(top(stcamino)!=0)
  {
    if(top(stcamino)>0)
    {
      valor=top(stcamino);
      pop(stcamino);
      valor=valor*(-1);
      drive_goto(valor,valor);
    }  
    if(top(stcamino)==-1)
    {
      pop(stcamino);
      derecha2();
    }
    if(top(stcamino)==-2)
    {
      pop(stcamino);
      izquierda2();
    }    
  }
  revisarStacks(stder,stizq,stcamino);   
}

void revisarStacks(Stack *stder,Stack *stizq,Stack *stcamino){
  if(top(stder)==1&&top(stizq)==0) 
  {
    pop(stcamino);
    derecha2();
    paso();
  } 
  if(top(stder)==0&&top(stizq)==1) 
  {
    pop(stcamino);
    izquierda2();
    paso();
  }
  if(top(stder)==1&&top(stizq)==1) 
  {
    pop(stder);
    push(stder,0);
    derecha2();
    paso();
  }
     
}   

int main()                                    // Main function
{
  //Se crean los stacks
  Stack *stder = createStack(100); //crea un stack de tamaño 20
  Stack *stizq = createStack(100);
  Stack *stade = createStack(100);
  Stack *stcamino = createStack(100);
  //Stack *st = createStack(150);
  while(1){
   
    izquierda();
    derecha();
    adelante();
    suma(stade,stder,stizq,stcamino);
    printf(" adelante = %d cm\n"            // Display distance
           , ade);
    printf(" derecha = %d cm\n"           // Display distance
           , der);
    printf(" izquierda = %d cm\n"            // Display distance
           , izq);
    
    if(ade==1){
      paso();
      }
    else if(der==1){
      derecha2();
      paso();
      }
    else if(izq==1){
      izquierda2();
      paso();
      }  
  }   
}
