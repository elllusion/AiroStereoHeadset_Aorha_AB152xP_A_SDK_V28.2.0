#ifndef _OS_LIST_H_
#define _OS_LIST_H_
/*
 * list.h - the API functios for single link list structure
 * Copyright (C) 2013~ Airoha Technology Corp. All rights reserved.
 *
 * This program is used to provide the API functions to use list structure.
 *
 * Designer : Charles, +886-3-612-8800 ext.2882, CharlesSu@airoha.com.tw,
 * 5F., No.6-5, Dusing Road, Hsinchu Science Park, Hsinchu 300, Taiwan.
 *
 * History :
 */
#include "config.h"
#include "cpu.h"
//-
#include "os_types.h"



////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*
 * LIST_STRU - single link list structure
 * This data structure performs a single link list structure.
 */
typedef struct list_stru
{
    struct list_stru OS_PTR next;

} LIST_STRU, OS_PTR LIST_STRU_PTR;

/*
 * LIST_HEAD - list head of single link list structure.
 * This structure is a structure of double link list.
 */
typedef struct list_head_stru
{
    struct list_stru head, tail;

} LIST_HEAD, OS_PTR LIST_HEAD_PTR;

/*
 * DLIST_STRU - double link list structure
 * This data structure performs a double link list structure.
 */
typedef struct dlist_stru
{
    struct dlist_stru OS_PTR prev;
    struct dlist_stru OS_PTR next;

} DLIST_STRU, OS_PTR DLIST_STRU_PTR;

/*
 * DLIST_HEAD - list head of double link list structure.
 * This structure is a structure of double link list.
 */
typedef DLIST_STRU DLIST_HEAD;
typedef DLIST_STRU_PTR DLIST_HEAD_PTR;



////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * list_entry - get the container structure of given node
 * @ptr:        the &LIST_STRU pointer.
 * @type:       the data structure of container
 * @member:     member of container structure
 */
#define OS_list_entry(ptr,type,member) OS_CONTAINER_OF(ptr,type,member)

/**
 * OS_list_init - init a list entry
 * @list_ptr:     list need to initialize
 *
 * Initialize the specific list entry.
 */
#define OS_list_init(list_ptr)                                                  \
    do                                                                          \
    {                                                                           \
        (list_ptr)->next = OS_NULL_DATA_PTR;                                    \
    } while (0)

/**
 * OS_list_head_init - init a list head
 * @head_ptr:          head need to initialize
 *
 * Initialize the specific list entry.
 */
#define OS_list_head_init(head_ptr)                                             \
    do                                                                          \
    {                                                                           \
        (head_ptr)->head.next = OS_NULL_DATA_PTR;                               \
        (head_ptr)->tail.next = OS_NULL_DATA_PTR;                               \
    } while (0)

/**
 * OS_list_prepend - add a node into list front
 * @new:             new node to be added
 * @head:            list head to add it after
 *
 * Insert a new node just after the specified head.
 */
EXTERN void OS_list_prepend(
    IN      LIST_STRU_PTR new,
    IN      LIST_HEAD_PTR head);

/**
 * OS_list_append - add a node into list back
 * @new:            new node to be added
 * @head:           list head to add it before
 *
 * Insert a new node just before the specified head.
 */
EXTERN void OS_list_append(
    IN      LIST_STRU_PTR new,
    IN      LIST_HEAD_PTR head);

/**
 * OS_list_getfront - get first node from list
 * @head:             head of a list.
 *
 * Obtain the first node from a specified list.
 */
EXTERN LIST_STRU_PTR OS_list_getfront(
    IN      LIST_HEAD_PTR head);

/**
 * OS_list_getrear -  get last node from list
 * @head:             head of a list.
 *
 * Obtain the last node from a specified list.
 */
EXTERN LIST_STRU_PTR OS_list_getrear(
    IN      LIST_HEAD_PTR head);



////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * dlist_entry - get the container structure of given node
 * @ptr:        the &LIST_STRU pointer.
 * @type:       the data structure of container
 * @member:     member of container structure
 */
#define OS_dlist_entry(ptr,type,member) OS_CONTAINER_OF(ptr,type,member)

/**
 * OS_dlist_init - init a list entry
 * @list_ptr:     list need to initialize
 *
 * Initialize the specific list entry.
 */
#define OS_dlist_init(plist)                                                    \
    do                                                                          \
    {                                                                           \
        (plist)->next = (plist)->prev = plist;                                  \
    } while (0)

/**
 * OS_dlist_head_init - init a list head
 * @head_ptr:          head need to initialize
 *
 * Initialize the specific list entry.
 */
#define OS_dlist_head_init(phead) OS_dlist_init(phead)

/**
 * OS_dlist_is_empty - check whether a list is empty
 * @list:            pointer of list
 *
 * Initialize the specific list entry.
 */
#define OS_dlist_is_empty(list) ((list) == (list)->next)

/**
 * OS_dlist_insert - insert into a list
 * @list:            pointer of list
 * @prev:            pointer of prev list
 * @next:            pointer of next list
 *
 * Initialize the specific list entry.
 */
#define OS_dlist_insert(list,prev_list,next_list)                                         \
    do                                                                          \
    {                                                                           \
        (list)->prev = (prev_list);                                                  \
        (list)->next = (next_list);                                                  \
        (prev_list)->next = (list);                                                  \
        (next_list)->prev = (list);                                                  \
    } while (0)

/**
 * OS_dlist_remove - remove from a list
 * @list:            pointer of list
 *
 * Initialize the specific list entry.
 */
#define OS_dlist_remove(list)                                                   \
    do                                                                          \
    {                                                                           \
        (list)->prev->next = (list)->next;                                      \
        (list)->next->prev = (list)->prev;                                      \
        OS_dlist_init(list);                                                    \
    } while (0)

/**
 * OS_dlist_prepend - add a node into list front
 * @new:             new node to be added
 * @head:            list head to add it after
 *
 * Insert a new node just after the specified head.
 */
EXTERN VOID OS_dlist_prepend(
    IN      DLIST_STRU_PTR new,
    IN      DLIST_HEAD_PTR head);

/**
 * OS_dlist_append - add a node into list back
 * @new:            new node to be added
 * @head:           list head to add it before
 *
 * Insert a new node just before the specified head.
 */
EXTERN VOID OS_dlist_append(
    IN      DLIST_STRU_PTR new,
    IN      DLIST_HEAD_PTR head);

/**
 * OS_dlist_getfront - get first node from list
 * @head:             head of a list.
 *
 * Obtain the first node from a specified list.
 */
EXTERN DLIST_STRU_PTR OS_dlist_getfront(
    IN      DLIST_HEAD_PTR head);

#endif /* _OS_LIST_H_ */

