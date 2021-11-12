#include "command_history.h"


void command_vector_init(command_vector* c_v)
{
    c_v->head = NULL;
    c_v->history_nums = 0;
    c_v->vector_size = 20;
    command_history_nums_dila(&(*c_v));
    return ;
}
void command_history_nums_dila(command_vector* c_v)
{
    if(c_v->head == NULL)
    {
       
        c_v->head = (command_node*)malloc(sizeof(command_node)*20);
        return ;
    }
    else if(c_v->history_nums > (c_v->vector_size/2))
    {
       
        command_node* newhead = (command_node*)malloc(sizeof(command_node)*2*c_v->vector_size);
        int i=0;
        int j=0;
        command_node* p2 =c_v->head;
        for(i=0 ;i<c_v->history_nums;i++)
        {
            for(j=0 ;j<256;j++)
            {
                (newhead+i)->command_buf[j] = (p2+i)->command_buf[j];
            }
        }
        free(c_v->head);
        c_v->head = newhead;
        newhead = NULL;
        c_v->vector_size = 2*c_v->vector_size;
        return ;
    }
}

void delete_command_vector(command_vector* c_v)
{
    free(c_v->head);
}