#include "list.h"

#ifdef LISP_TEST
#include "../tests/kernel_mappings.h"
#endif

#include "../kernel/kmalloc.h"
#include "../kernel/kterm.h"

List_t* make_list()
{
    List_t* ret = malloc(sizeof(List_t));
    ret->size = 0;
    ret->first = NULL;
    return ret;
}

void free_list(List_t* list)
{
    ListNode_t* nextnode = list->first;
    ListNode_t* tmp = nextnode;

    //waht the fuck is this
    while(nextnode)
    {
	tmp = nextnode;
	nextnode = nextnode->next;
	free(tmp->data);
	free(tmp);
    }

    free(list);
}

ListNode_t* list_node_at(List_t* list, int index)
{
    ListNode_t* tmp = list->first;
    for(int i = 0; i < index; i++)
    {
	tmp = tmp->next;
	if(!tmp)
	    while(1) {k_print("LISTERR");} //out of range, hang TODO: actual err
    }

    return tmp;
}


void* list_elem_at(List_t* list, int index)
{
    ListNode_t* tmp = list->first;
    for(int i = 0; i < index; i++)
    {
	tmp = tmp->next;
	if(!tmp)
	    while(1) {k_print("LISTERR");} //out of range, hang TODO: actual err
    }

    return tmp->data;
}

void list_append(List_t* list, void* elem)
{
    ListNode_t* end = list_node_at(list, list->size);
    
    ListNode_t* new = malloc(sizeof(ListNode_t));
    new->data = elem;
    new->next = NULL;
    
    end->next = new;
    list->size++;
}

void list_pop(List_t* list)
{
    ListNode_t* tmp = list_node_at(list, list->size);
    free(tmp);

    list_node_at(list, list->size - 1)->next = NULL;

    list->size--;
}
