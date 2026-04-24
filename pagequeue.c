/** pagequeue.c
 * ===========================================================
 * Name: Diego Castellanos, 04 22 2026
 * Section: CS483 / M3
 * Project: PEX3 - Page Replacement Simulator
 * Purpose: Implementation of the PageQueue ADT — a doubly-linked
 *          list for LRU page replacement.
 *          Head = LRU (eviction end), Tail = MRU end.
 * =========================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "pagequeue.h"

/**
 * @brief Create and initialize a page queue with a given capacity
 */
PageQueue *pqInit(unsigned int maxSize) {
    // TODO: malloc a PageQueue, set head and tail to NULL,
    //       size to 0, maxSize to maxSize, and return the pointer
    PageQueue *pq = (PageQueue *)malloc(sizeof(PageQueue));
    pq -> head = NULL;
    pq -> tail = NULL;
    pq -> size = 0;
    pq -> maxSize = maxSize;
    
    return pq;
}

/**
 * @brief Access a page in the queue (simulates a memory reference)
 */
long pqAccess(PageQueue *pq, unsigned long pageNum) {
    // TODO: Search the queue for pageNum (suggest searching tail->head
    //       so you naturally count depth from the MRU end).
    //
    // HIT path (page found at depth d):
    //   - Remove the node from its current position and re-insert
    //     it at the tail (most recently used).
    //   - Return d.
    //
    // MISS path (page not found):
    //   - Allocate a new node for pageNum and insert it at the tail.
    //   - If size now exceeds maxSize, evict the head node (free it).
    //   - Return -1.
    PqNode *current = pq -> tail;

    while (current != NULL) {
        //HIT Path
        if (current -> pageNum == pageNum) {
            if (current != pq -> tail) {
                if (current -> prev != NULL) {
                    current -> prev -> next = current -> next;
                }
                if (current -> next != NULL) {
                    current -> next -> prev = current -> prev;
                }

                // Insert at the tail
                current -> next = NULL;
                current -> prev = pq->tail;
                pq -> tail -> next = current;
                pq -> tail = current;
            }
            return pq -> size - 1;  // Return the depth of the page from MRU end
        }

        current = current -> prev;
    }

    //MISS Path
    PqNode *new = (PqNode *)malloc(sizeof(PqNode));
    new -> pageNum = pageNum;
    new -> next = NULL;
    new -> prev = pq -> tail;

    if (pq -> tail != NULL) {
        pq -> tail -> next = new;
    }
    else {
        pq -> head = new;
    }
    pq -> tail = new;
    pq -> size++;

    if (pq -> size > pq -> maxSize) {
        PqNode *oldHead = pq -> head;
        pq -> head = pq -> head -> next;
        pq -> head -> prev = NULL;
        free(oldHead);
        pq -> size--;
    }


    return -1;
}

/**
 * @brief Free all nodes in the queue and reset it to empty
 */
void pqFree(PageQueue *pq) {
    // TODO: Walk from head to tail, free each node, then free
    //       the PageQueue struct itself.
    PqNode *current = pq -> head;

    while (current != NULL) {
        PqNode *nextNode = current -> next;
        free(current);
        current = nextNode;
    }

    free(pq);
}

/**
 * @brief Print queue contents to stderr for debugging
 */
void pqPrint(PageQueue *pq) {
    // TODO (optional): Print each page number from head to tail,
    //                  marking which is head and which is tail.
    //                  Useful for desk-checking small traces.
}
