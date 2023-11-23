#include <stdio.h>
#include <string.h>

#include "levelstack.h"

void initStack(LevelStack *stack) {
    stack->top = -1;
}

int getStackLevel(LevelStack *stack) {
    return stack->top;
}

void incrementLevel(LevelStack *stack) {
    stack->top++;
    if (stack->top > -1) {
        stack->levels[stack->top] = 0;
    }
}

void decrementLevel(LevelStack *stack) {
    if (stack->top > -1) {
        stack->top--;
    }
}

void incrementCounter(LevelStack *stack) {
    if (stack->top > -1) {
        stack->levels[stack->top]++;
    }
}

char* getLevelCounters(LevelStack *stack, int top) {
    static char result[1024];
    char temp[10];

    if ( top > stack->top ) top = stack->top;

    strcpy(result, "");

    for (int i = 0; i < top; ++i) {
        sprintf(temp, "#%d", stack->levels[i]);
        strcat(result, temp);
    }

    return result;
}
