#include "list.h"
#include "../kernel/kmalloc.h"

List_t* make_list()
{
    List_t* ret = kmalloc(sizeof(List_t));
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
	kfree(tmp->data);
	kfree(tmp);
    }

    kfree(list);
}

ListNode_t* list_node_at(List_t* list, int index)
{
    ListNode_t* tmp = list->first;
    for(int i = 0; i < index; i++)
    {
	tmp = tmp->next;
	if(!tmp)
	    while(1) { } //out of range, hang TODO: actual err
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
	    while(1) { } //out of range, hang TODO: actual err
    }

    return tmp->data;
}

void list_append(List_t* list, void* elem)
{
    ListNode_t* end = list_node_at(list, list->size);
    
    ListNode_t* new = kmalloc(sizeof(ListNode_t));
    new->data = elem;
    new->next = NULL;
    
    end->next = new;
}

void list_pop(List_t* list)
{
    ListNode_t* tmp = list_node_at(list, list->size);
    kfree(tmp);

    list_node_at(list, list->size - 1)->next = NULL;

    list->size--;
}
