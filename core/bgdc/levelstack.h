#ifndef __LEVELSTACK_H
#define __LEVELSTACK_H

    #define MAX_LEVELS 512

    typedef struct {
        int levels[MAX_LEVELS];
        int top;
    } LevelStack;

    extern void initStack(LevelStack *stack);
    extern int getStackLevel(LevelStack *stack);
    extern void incrementLevel(LevelStack *stack);
    extern void decrementLevel(LevelStack *stack);
    extern void incrementCounter(LevelStack *stack);
    extern char* getLevelCounters(LevelStack *stack, int top);

#endif
