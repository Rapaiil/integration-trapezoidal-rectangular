#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<math.h>

typedef char str[1000]; //string data type

//create Stack type
typedef struct {
	int top;
	unsigned capacity;
	int* array;
} Stack;

struct initVariables {
	double a; //lower limit
	double b; //upper limit
	int n; //number of iterations
};

typedef struct {
	str function;
	str method;
	double integral;
} integralHistory;

/*
* createStack() function
* Param: unsigned capacity
* Return: struct Stack* type
* Desc: This function creates a stack for the postfix algorithm
*/
Stack* createStack( unsigned capacity ) 
{ 
    Stack* stack = (Stack*) malloc(sizeof(Stack)); 
  
    if (!stack)  
        return NULL; 
  
    stack->top = -1; 
    stack->capacity = capacity; 
  
    stack->array = (int*) malloc(stack->capacity * sizeof(int)); 
  
    if (!stack->array) 
        return NULL; 
    return stack; 
} 

/*
* isEmpty() function
* Param: Stack* stack 
* Return: int type
* Desc: this function checks if the given stack is empty (returns 1) or not (returns 0)
*/
int isEmpty(Stack* stack) {
	return stack->top==-1;
}

/*
* Top() function
* Param: Stack* stack 
* Return: char type
* Desc: this function checks for the value at the top of given stack
*/
char Top(Stack* stack) {
	return stack->array[stack->top];
}

/*
* pop() function
* Param: Stack* stack 
* Return: char type
* Desc: this function returns the value at the top of the given stack
* if not empty, else returns '$'
*/
char pop(Stack* stack) {
	if(!isEmpty(stack))
		return stack->array[stack->top--];
	return '$';
}

/*
* push() function
* Param: Stack* stack
* Param: char op
* Return: void
* Desc: this function inserts the given operand to the stack
*/
void push(Stack* stack, char op) {
	stack->array[++stack->top] = op;
}

/*
* isOperand() function
* Param: char op
* Return: int type
* Desc: this function checks if the character is an alphanumeric (1) or not (0)
*/
int isOperand(char ch) {
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') ;
}

/*
* Prec() function
* Param: char op
* Return: int type
* Desc: this function checks for the priority number of the given operator (precedence)
*/
int Prec(char ch) {
	switch(ch) {
		case '+':
		case '-': return 1;
		case '*':
		case '/': return 2;
		case '^': return 3;
	}
	return -1;
}

void infixToPostfix(str equation) {
	int i, j;
	Stack *stack = createStack(strlen(equation));
	
	if(!stack) {
		printf("CANNOT CREATE STACK!\n");
		return;
	}
	
	for(i=0, j=-1; equation[i]; ++i) {
		if(isOperand(equation[i]))
			equation[++j] = equation[i];
		else if(equation[i] == '(')
			push(stack, equation[i]);
		else if(equation[i] == ')') {
			while (!isEmpty(stack) && Top(stack) != '(')
				equation[++j] = pop(stack);
			if(!isEmpty(stack) && Top(stack) != '(') {
				printf("EXPRESSION IS INVALID!\n");
				return;
			} else
				pop(stack);
		} else {
			while(!isEmpty(stack) && Prec(equation[i]) <= Prec(Top(stack)))
				equation[++j] = pop(stack);
			push(stack, equation[i]);
		}
	}
	
	while(!isEmpty(stack))
		equation[++j] = pop(stack);
		
	equation[++j] = '\0';
}

double f(str equation, double x) {
	int i, ptr = 0;
	double stacker[strlen(equation)], val1, val2;
	
	for(i=0; equation[i]; i++) {
		if(equation[i] == 'x')
			stacker[ptr++] = x;
		else if(equation[i] >= '0' && equation[i] <= '9')
			stacker[ptr++] = equation[i] - '0';
		else {
			val1 = stacker[--ptr];
			val2 = stacker[--ptr];
			switch(equation[i]) {
				case '+': stacker[ptr++] = val2+val1; break;
				case '-': stacker[ptr++] = val2-val1; break;
				case '*': stacker[ptr++] = val2*val1; break;
				case '/': stacker[ptr++] = val2/val1; break;
				case '^': stacker[ptr++] = pow(val2, val1); break;
			}
		}
	}
	return stacker[--ptr];
}

void insertFunction(str cpyEquation, str equation) {
	str newEquation;
	int i, j;
	
	printf("Enter function to evaluate\nf(x) = ");
	scanf("%s", equation);
	strcpy(cpyEquation, equation);
	
	for(i=0, j=0; i <= strlen(equation); i++) {
		if(equation[i] == 'x' && i==0) {
			newEquation[j] = '1';
			newEquation[++j] = '*';
			newEquation[++j] = equation[i];
		} else if(equation[i] == 'x')  {
			newEquation[j] = '*';
			newEquation[++j] = equation[i];
		} else {
			newEquation[j] = equation[i];
		}
		j++;
	}
	strcpy(equation, newEquation);
	equation[++j] = '\0';
}

void doInitialization(struct initVariables *var) {
	printf("Enter lower bound: ");
	scanf("%lf", &var->a);
	printf("Enter upper bound: ");
	scanf("%lf", &var->b);
	printf("Enter number of iterations: ");
	scanf("%d", &var->n);
}

void showAnswer(double **integral) {
	printf("\n*******\nIntegral = %0.4lf", **integral);
}

void trapezoidalMethod(str equation, struct initVariables *var, double *finalIntegral) {
	int subN = var->n, 	//subN contains the number of interations of x and y
		ctr;	//for loop 
	double x[subN+1], y[subN+1], 	//array of x values and y values
		   deltaX, sum=0,	//sum for summation of heights (or bases)
		   a = var->a, b = var->b;
	
	deltaX = (b - a) / subN;
	 
	for(ctr=0; ctr<=subN; ctr++) {
		x[ctr] = (a + ctr) * deltaX;
		y[ctr] = f(equation, x[ctr]);
	}
	
	for(ctr=1; ctr<subN; ctr++) {
		sum = sum + (deltaX * y[ctr]);
	}
	
	*finalIntegral = deltaX / 2.0 * (y[0] + y[subN]) + sum;
	showAnswer(&finalIntegral);
}

void rectangularMethod(str equation, struct initVariables *var, double *finalIntegral) {
	int subN = var->n, 	//subN contains the number of interations of x and y
		ctr;	//for loop 
	double deltaX, sum=0,	//sum for approximation
		   a = var->a, b = var->b;
	
	deltaX = (b - a) / subN;
	
	for(ctr=1; ctr<=subN; ++ctr) {
		sum += deltaX * f(equation, a + (ctr - 1) * deltaX);
	}
	
	*finalIntegral = sum;
	showAnswer(&finalIntegral);
}

void viewHistory(integralHistory list[], int n) {
	int i;
	system("cls");
	if(n == 0) {
		printf("NOTHING TO SHOW!\n");
	} else {
		printf("PREVIOUS CALCULATIONS\n********************************\n");
		for(i=0; i < n; i++) {
			printf("%s\n", (list+i)->method);
			printf("f(x)=%s\n", (list+i)->function);
			printf("Answer: %0.4lf\n\n", (list+i)->integral);
		}
	}
}

void closeProgram() {
	system("cls");
	printf("\nThank you for using our program!\n");
}

void continueProg() {
	printf("\nPress any key to continue...");
	getch();
}

void menu() {
	system("cls");
	printf("\nINTEGRATION CALCULATOR\n");
	printf("**************************\n");
	printf("[A] Integrate using Trapezoidal Method\n");
	printf("[B] Integrate using Rectangular Method\n");
	printf("[C] History\n");
	printf("[D] Quit\n");
}

int main() {
	char userChoice;
	
	struct initVariables variables;
	double finalIntegral;	//finalIntegral is a variable for the final answer of integration
	integralHistory list[1000];
	int ptr = 0;	//ptr points to current entry in history
	str trapezoidString = "Trapezoidal Method", rectangleString = "Rectangular Method", cpyEquation, equation;
	
	do {
		menu();
		printf(">> ");
		scanf(" %c", &userChoice);
		
		switch(userChoice) {
			case 'A':
			case 'a': system("cls");
					  printf("Integration using Trapezoidal Method\n************************************\n");
					  insertFunction(cpyEquation, equation);
					  doInitialization(&variables);
					  infixToPostfix(equation);
					  trapezoidalMethod(equation, &variables, &finalIntegral);
					  continueProg();
					  
					  strcpy((list + ptr)->function, cpyEquation);
					  strcpy((list + ptr)->method, trapezoidString);
					  (list + ptr)->integral = finalIntegral;
					  ptr++;
					  break;
			case 'B':
			case 'b': system("cls");
					  printf("Integration using Rectangular Method\n************************************\n");
					  insertFunction(cpyEquation, equation);
					  doInitialization(&variables);
					  infixToPostfix(equation);
					  rectangularMethod(equation, &variables, &finalIntegral);
					  continueProg();
					  
					  strcpy((list + ptr)->function, cpyEquation);
					  strcpy((list + ptr)->method, rectangleString);
					  (list + ptr)->integral = finalIntegral;
					  ptr++;
					  break;
			case 'C':
			case 'c': viewHistory(list, ptr);
					  continueProg();
					  break;
			case 'D':
			case 'd': closeProgram();
					  break;
			default: printf("\nINVALID OPTION! Please try again.\n"); continueProg();
		}
	} while(userChoice != 'D' && userChoice != 'd');
	return 0;
}
