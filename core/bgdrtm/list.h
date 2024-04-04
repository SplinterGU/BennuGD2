/*
 *  Copyright (C) SplinterGU (Fenix/BennuGD) (Since 2006)
 *
 *  This file is part of Bennu Game Development
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty. In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *     1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *
 *     2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *
 *     3. This notice may not be removed or altered from any source
 *     distribution.
 *
 */

/* --------------------------------------------------------------------------- */

#ifndef LIST_H
#define LIST_H

// Callback prototype to free the memory occupied by the data
typedef void (*FreeFunction)(void *);

// Structure to represent the list
typedef struct List {
    struct Node *head;
    struct Node *pool;
    int size;
    int pool_size;
    int max_pool_size;
} List;

// Function prototypes for list operations
extern List *list_create(int max_poolsize);
extern void list_free(List *list, FreeFunction freeFunction);
extern void list_empty(List *list, FreeFunction freeFunction);
extern void list_insertItem(List *list, void *data);
extern void list_removeItem(List *list, void *data, FreeFunction freeFunction);
extern void *list_walk(List *list, void **context);
extern int list_size(List *list);

#endif /* LIST_H */
