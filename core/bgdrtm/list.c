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

#include "list.h"

#define DEFAULT_MAX_POOL_SIZE   1024

#include <stdio.h>
#include <stdlib.h>

/* --------------------------------------------------------------------------- */
// Structure to represent a node of the list
typedef struct Node {
    void *data;
    struct Node *next;
} Node;

/* --------------------------------------------------------------------------- */
/** Create a new list.
 *  This function creates a new empty list.
 *  \param max_pool_size Max pool size, -1 is default 1024
 *  \return A pointer to the newly created list, or NULL if memory allocation fails.
 */
List *list_create( int max_pool_size ) {
    List *list = (List *)malloc(sizeof(List));
    if (!list) {
        return NULL;
    }
    list->head = NULL;
    list->pool = NULL;
    list->size = 0;
    list->pool_size = 0;
    list->max_pool_size = ( max_pool_size == -1 ) ? DEFAULT_MAX_POOL_SIZE : max_pool_size;
    return list;
}

/* --------------------------------------------------------------------------- */
/** Free memory occupied by the list.
 *  This function frees the memory occupied by the list and its nodes.
 *  \param list Pointer to the list to be freed.
 *  \param freeFunction Pointer to the function used to free the data contained in each node.
 */
void list_free(List *list, FreeFunction freeFunction) {
    if (!list) {
        return;
    }
    Node *current = list->head;
    while (current) {
        Node *temp = current;
        current = current->next;
        if ( freeFunction ) {
            freeFunction(temp->data);
            temp->data = NULL;
        }
        free(temp);
    }
    current = list->pool;
    while (current) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}

/* --------------------------------------------------------------------------- */
/** Empty the list.
 *  This function removes all items from the list without deallocating the list itself. 
 *  The removed items are optionally passed to a pool for reuse.
 *  \param list Pointer to the list to be emptied.
 *  \param freeFunction Pointer to the function used to free the data contained in each node.
 */
void list_empty(List *list, FreeFunction freeFunction) {
    if (!list) {
        return;
    }
    Node *current = list->head;
    while (current) {
        Node *next = current->next;
        if ( freeFunction ) {
            freeFunction(current->data);
            current->data = NULL;
        }
        if ( list->pool_size >= list->max_pool_size ) {
            free(current);
        } else {
            current->next = list->pool;
            list->pool = current;
            list->pool_size++;
        }
        list->size--;
        current = next;
    }
    list->head = NULL;
}

/* --------------------------------------------------------------------------- */
/** Insert an item into the list.
 *  This function inserts a new item into the list.
 *  \param list Pointer to the list.
 *  \param data Pointer to the data to be inserted.
 */
void list_insertItem(List *list, void *data) {
    if (!list) {
        return;
    }

    Node *newNode = NULL;
    if (list->pool) {
        newNode = list->pool;
        list->pool = newNode->next;
        list->pool_size--;
    } else {
        newNode = (Node *)malloc(sizeof(Node));
        if (!newNode) {
            return;
        }
    }

    newNode->data = data;
    newNode->next = list->head;
    list->head = newNode;
    list->size++;
}

/* --------------------------------------------------------------------------- */
/** Remove an item from the list.
 *  This function removes an item from the list.
 *  \param list Pointer to the list.
 *  \param data Pointer to the data to be removed.
 *  \param freeFunction Pointer to the function used to free the data contained in each node.
 */
void list_removeItem(List *list, void *data, FreeFunction freeFunction) {
    if (!list || !data) {
        return;
    }
    Node *current = list->head;
    Node *prev = NULL;

    while (current) {
        if (current->data == data) {
            if (prev) {
                prev->next = current->next;
            } else {
                list->head = current->next;
            }
            list->size--;
            if ( freeFunction ) {
                freeFunction(current->data);
                current->data = NULL;
            }
            if ( list->pool_size >= list->max_pool_size ) {
                free(current);
            } else {
                current->next = list->pool;
                list->pool = current;
                list->pool_size++;
            }
            break;
        }
        prev = current;
        current = current->next;
    }
}

/* --------------------------------------------------------------------------- */
/** Walk through the list.
 *  This function allows iterating through the list.
 *  \param list Pointer to the list.
 *  \param context Pointer to the context used for iteration.
 *  \return Pointer to the data of the current node, or NULL if the end of the list is reached.
 */
void *list_walk(List *list, void **context) {
    if (!list || !context) {
        return NULL;
    }

    Node *current = (Node *)*context;
    if (!current) {
        current = list->head;
    } else {
        current = current->next;
    }

    *context = current;
    if (current) {
        return current->data;
    }
    return NULL;
}

/* --------------------------------------------------------------------------- */
/** Get the size of the list.
 *  This function returns the size of the list.
 *  \param list Pointer to the list.
 *  \return The size of the list.
 */
int list_size(List *list) {
    if (!list) {
        return 0;
    }
    return list->size;
}

/* --------------------------------------------------------------------------- */
