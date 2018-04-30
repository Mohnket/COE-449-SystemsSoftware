////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// File Name: mymalloc.c
///
/// @par Full Description
/// COE 449 Project 3
/// Implements a replacement of malloc
///
/// @version
/// 25-Mar-2018 Tyler Mohnke    Initial implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// System Includes
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

// Local Includes
#include "mymalloc.h"

// Constant Values

// Forward References
void *sbrk(intptr_t increment);
typedef struct ListNode ListNode;

// File scoped variables
ListNode* f_pLinkedList = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Struct Name: ListNode
///
/// @par Full Description
/// Used to keep track of memory usage in the heap
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct ListNode
{
    ListNode* m_pNextNode;
    ListNode* m_pPrevNode;
    int m_Bytes;
    uint8_t m_IsFree;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function Name: print_list
///
/// @par Full Description
/// Prints out each node in f_pLinkedList
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void print_list(void)
{
    uint32_t count = 0U;
    ListNode* pIterator = NULL;
    for(pIterator = f_pLinkedList; pIterator != NULL; pIterator = pIterator->m_pNextNode)
    {
        printf("Address = %x    Bytes = %d    Free = %d\n", (uint32_t)pIterator, pIterator->m_Bytes, pIterator->m_IsFree);
        count++;
    }
    printf("%d\n\n", count);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function Name: my_worstfit_malloc
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void* my_worstfit_malloc(int size)
{
    if(size <= 0)
    {
        size = 1;
    }
    
    void* retval = NULL;
    // No memory chuncks are allocated, allocate one and save the head in f_pLinkedList
    if(f_pLinkedList == NULL)
    {
        f_pLinkedList = (ListNode*)sbrk(sizeof(ListNode) + size);
        if(f_pLinkedList == (void*)-1)
        {
            f_pLinkedList = NULL;
            return NULL;
        }
        
        f_pLinkedList->m_pNextNode = NULL;
        f_pLinkedList->m_pPrevNode = NULL;
        f_pLinkedList->m_Bytes = size;
        f_pLinkedList->m_IsFree = 0U;
        
        retval = (void*)(f_pLinkedList + 1);
    }
    else
    {
        // Find the largest open space
        ListNode* pMaxFree = NULL;
        ListNode* pIterator =NULL;
        for(pIterator = f_pLinkedList; pIterator != NULL; pIterator = pIterator->m_pNextNode)
        {
            if((pIterator->m_IsFree) && (pIterator->m_Bytes >= size))
            {
                if(pMaxFree == NULL)
                {
                    pMaxFree = pIterator;
                }
                else if(pIterator->m_Bytes > pMaxFree->m_Bytes)
                {
                    pMaxFree = pIterator;
                }
                else
                {
                    
                }
            }
            
        }
        
        // No space found large enough for an allocation, add a new allocation to the end of the list
        if(pMaxFree == NULL)
        {
            ListNode* pTail = f_pLinkedList;
            while(pTail->m_pNextNode != NULL)
            {
                pTail = pTail->m_pNextNode;
            }
            
            pMaxFree = (ListNode*)sbrk(sizeof(ListNode) + size);
            if(pMaxFree == (void*)-1)
            {
                return NULL;
            }
        
            pMaxFree->m_pNextNode = NULL;
            pMaxFree->m_pPrevNode = pTail;
            pTail->m_pNextNode = pMaxFree;
            pMaxFree->m_Bytes = size;
            pMaxFree->m_IsFree = 0U;
            
            retval = (void*)(pMaxFree + 1);
        }
        else if((size <= pMaxFree->m_Bytes) && (pMaxFree->m_Bytes < (size + sizeof(ListNode))))
        {
            // space allocated in pMaxFree, but there is not enough space leftover for a node to keep track of the space
            pMaxFree->m_IsFree = 0U;
            retval = (void*)(pMaxFree + 1);
        }
        else
        {
            // Make a new free node representing the remaining free space at the end of the allocation
            ListNode* pNewFree = (ListNode*)((void*)pMaxFree + sizeof(ListNode) + size);
            
            pNewFree->m_pNextNode = pMaxFree->m_pNextNode;
            if(pNewFree->m_pNextNode != NULL)
            {
                pNewFree->m_pNextNode->m_pPrevNode = pNewFree;
            }
            
            pMaxFree->m_pNextNode = pNewFree;
            pNewFree->m_pPrevNode = pMaxFree;
            pNewFree->m_Bytes = pMaxFree->m_Bytes - size - sizeof(ListNode);
            pMaxFree->m_Bytes = size + sizeof(ListNode);
            pNewFree->m_IsFree = 1U;
            pMaxFree->m_IsFree = 0U;
            
            retval = (void*)(pMaxFree + 1);
        }
    }
    
    // print_list();
    return retval;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function Name: my_free
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void my_free(void *ptr)
{
    ListNode* pNodeToRemove = ((ListNode*)ptr - 1);
    pNodeToRemove->m_IsFree = 1U;
    
    ListNode* pPrevNode = pNodeToRemove->m_pPrevNode;
    ListNode* pNextNode = pNodeToRemove->m_pNextNode;
    
    // Consider the previous Node
    if(pPrevNode == NULL)
    {
        // assert(pNodeToRemove == f_pLinkedList);
    }
    else if(pPrevNode->m_IsFree)
    {
        // merge current Node into prev Node
        pPrevNode->m_Bytes += (pNodeToRemove->m_Bytes + sizeof(ListNode));
        
        pNodeToRemove = pPrevNode;
        
        pNodeToRemove->m_pNextNode = pNextNode;
        if(pNextNode != NULL)
        {
            pNextNode->m_pPrevNode = pNodeToRemove;
        }
        
        pPrevNode = pNodeToRemove->m_pPrevNode;
    }
    else
    {
        
    }
    
    // Consider the next Node
    if(pNextNode == NULL)
    {
        if(pNodeToRemove == f_pLinkedList)
        {
            sbrk(-(pNodeToRemove->m_Bytes) - sizeof(ListNode));
            f_pLinkedList = NULL;
        }
        else
        {
            // assert(pPrevNode != NULL);
            sbrk(-(pNodeToRemove->m_Bytes) - sizeof(ListNode));
            pPrevNode->m_pNextNode = NULL;
        }
    }
    else if(pNextNode->m_IsFree)
    {
        // merge next Node into current Node
        pNodeToRemove->m_pNextNode = pNextNode->m_pNextNode;
        if(pNodeToRemove->m_pNextNode != NULL)
        {
            pNodeToRemove->m_pNextNode->m_pPrevNode = pNodeToRemove;
        }
    
        pNodeToRemove->m_Bytes += (pNextNode->m_Bytes + sizeof(ListNode));
    }
    else
    {
        
    }
    
    // print_list();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
