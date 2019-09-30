#include "list.h"
#include "../kernel/kmalloc.h"
#include "../kernel/kterm.h"

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
    
    if(!tmp)
	return NULL;
    
    for(int i = 0; i < index; i++)
    {
	tmp = tmp->next;
	/*if(!tmp)
	    while(1) {k_print("LISTERR");} //out of range, hang TODO: actual err
	*/
    }

    return tmp;
}


void* list_elem_at(List_t* list, int index)
{
    return list_node_at(list, index)->data;
}

void list_append(List_t* list, void* elem)
{
    ListNode_t* new = kmalloc(sizeof(ListNode_t));
    new->data = elem;
    new->next = NULL;

    ListNode_t* end = list_node_at(list, list->size - 1);
    if(!end)
    {
	//list is empty
	list->first = new;
    }
    else
    {
	end->next = new;	
    }
    
    list->size++;
}

void list_pop(List_t* list)
{
    ListNode_t* tmp = list_node_at(list, list->size);
    kfree(tmp);

    list_node_at(list, list->size - 1)->next = NULL;

    list->size--;
}
