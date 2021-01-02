#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"


int main(int argc, char *argv[])
{
    int i;
    node_t * list = NULL;
    for (i = 0; i < 10; ++i) {
	node_t * node = create_node(i);
	list = append_node(list, node);
    }

    list = remove_node(list, get_node_by_index(list, 0));
    list = append_node(list, create_node(55));
    list = insert_node(list, 0, create_node(77));

    if (find_node(list, 77) != NULL)
    {
	printf("77 is found\n");
    } else {
	printf("77 is not found\n");
    }

    if (find_node(list, 99) != NULL)
    {
	printf("99 is found\n");
    } else {
	printf("99 is not found\n");
    }

    printf("== RUN ==\n");
    print_list(list);
    destroy_list(list);
    
    return 0;
}
