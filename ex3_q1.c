#include "ex3_q1_given.h"
#include "ex3_q1.h"

int added_items=0;
int proccessed_items=0;
int producers_done=0;
int consumers_done=0;

pthread_cond_t cond1=PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2=PTHREAD_COND_INITIALIZER;
pthread_cond_t cond3=PTHREAD_COND_INITIALIZER;
pthread_cond_t cond4=PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex4=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex5=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex6=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex7=PTHREAD_MUTEX_INITIALIZER;

list_node* list_head=NULL;
list_node* list_tail=NULL;

//------------------------------------------------------------------------
int main()
{
	pthread_t prods [N_PROD];
	pthread_t cons [N_CONS];
	
	for(int i=0;i<N_PROD;i++)
	{
		pthread_create(prods+i,NULL,produce,i+1);
	}
	
	pthread_mutex_lock(&mutex3);// lock printing ----------------------
	printf(ALL_PROD_CREATED);
	pthread_mutex_unlock(&mutex3);// unlock printing ----------------------
	
	for(int j=0;j<N_CONS;j++)
	{
		pthread_create(cons+j,NULL,consume,j+1);
	}
	
	pthread_mutex_lock(&mutex3);// lock printing ----------------------
	printf(ALL_CONS_CREATED);
	pthread_mutex_unlock(&mutex3);// unlock printing ----------------------
	
	int d;
	int c;
	
	pthread_mutex_lock(&mutex6);// lock ----------------------
	d=producers_done;
	while(d<N_PROD)
	{
		pthread_cond_wait(&cond2,&mutex6);
		d=producers_done;
	}	
	pthread_mutex_unlock(&mutex6);// unlock ----------------------
	
	
	
	pthread_mutex_lock(&mutex3);// lock printing ----------------------
	printf(PROD_TERMINATED);
	pthread_mutex_unlock(&mutex3);// unlock printing ----------------------
	
	
	
	pthread_mutex_lock(&mutex7);// lock ----------------------
	c=consumers_done;
	while(c<N_CONS)
	{
		pthread_cond_wait(&cond3,&mutex7);
		c=consumers_done;
	}	
	pthread_mutex_unlock(&mutex7);// unlock ----------------------
	
	
	
	pthread_mutex_lock(&mutex3);// lock printing ----------------------
	printf(CONS_TERMINATED);
	pthread_mutex_unlock(&mutex3);// unlock printing ----------------------
		
	print_list();
	pthread_cond_destroy(&cond1);
	pthread_cond_destroy(&cond2);
	pthread_cond_destroy(&cond3);
	pthread_cond_destroy(&cond4);
	pthread_mutex_destroy(&mutex1);
	pthread_mutex_destroy(&mutex2);
	pthread_mutex_destroy(&mutex3);
	pthread_mutex_destroy(&mutex4);
	pthread_mutex_destroy(&mutex5);
	pthread_mutex_destroy(&mutex6);
	pthread_mutex_destroy(&mutex7);
	free_list(list_head);
	
	exit(0);
}
//------------------------------------------------------------------------	
void produce(int id)
{
	int c;
	
	pthread_mutex_lock(&mutex4);// lock ----------------------
	c=added_items;
	added_items++;
	pthread_cond_broadcast(&cond1);
	pthread_mutex_unlock(&mutex4);// unlock ----------------------
	
	while(c<TOTAL_ITEMS)
	{
		item* item=malloc(sizeof(item));
		
		pthread_mutex_lock(&mutex2);// lock randoming ----------------------
		int random=get_random_in_range();
		pthread_mutex_unlock(&mutex2);// unlock randoming ----------------------
		
		while(is_prime(random)==1)
		{
			pthread_mutex_lock(&mutex2);// lock randoming ----------------------
			random=get_random_in_range();
			pthread_mutex_unlock(&mutex2);// unlock randoming ----------------------
		}
		item->prod=random;
		item->status=NOT_DONE;
		
		pthread_mutex_lock(&mutex1);// lock Reading/Writing ----------------------
		add_to_list(item);
		pthread_cond_broadcast(&cond4);
		pthread_mutex_unlock(&mutex1);// unlock Reading/Writing ----------------------
		
		
		
		pthread_mutex_lock(&mutex3);// lock printing ----------------------
		write_adding_item(id, item);
		pthread_mutex_unlock(&mutex3);// unlock printing ----------------------
		
		
		pthread_mutex_lock(&mutex4);// lock ----------------------
		c=added_items;
		added_items++;
		pthread_cond_broadcast(&cond1);
		pthread_mutex_unlock(&mutex4);// unlock ----------------------
	}
	
	pthread_mutex_lock(&mutex3);// lock printing ----------------------
	write_producer_is_done(id);
	pthread_mutex_unlock(&mutex3);// unlock printing ----------------------
	
	
	
	pthread_mutex_lock(&mutex6);// lock ----------------------
	producers_done++;
	pthread_cond_signal(&cond2);
	pthread_mutex_unlock(&mutex6);// unlock ----------------------
	
	pthread_exit(0);
}
//------------------------------------------------------------------------
void consume(int id)
{
	int d;
	
	pthread_mutex_lock(&mutex4);// lock ----------------------
	d=added_items;
	while(d<ITEM_START_CNT)
	{
		pthread_cond_wait(&cond1,&mutex4);
		d=added_items;
	}
    pthread_mutex_unlock(&mutex4);// unlock ----------------------
	
	int c;
	
	pthread_mutex_lock(&mutex5);// lock ----------------------
	c=proccessed_items;
	proccessed_items++;
	pthread_mutex_unlock(&mutex5);// unlock ----------------------
	
	while(c<TOTAL_ITEMS)
	{
		item* item=NULL;
		
		pthread_mutex_lock(&mutex1);// lock Reading/Writing ----------------------
		item=get_undone_from_list();
		while(item==NULL)
		{
			pthread_cond_wait(&cond4,&mutex1);
			item=get_undone_from_list();
		}
		pthread_mutex_unlock(&mutex1);// unlock Reading/Writing ----------------------
		
		set_two_factors(item);
		item->status=DONE;	
		
		pthread_mutex_lock(&mutex3);// lock printing ----------------------
		write_getting_item(id, item);
		pthread_mutex_unlock(&mutex3);// unlock printing ----------------------
		
	
		
		pthread_mutex_lock(&mutex5);// lock ----------------------
		c=proccessed_items;
		proccessed_items++;
		pthread_mutex_unlock(&mutex5);// unlock ----------------------
	}
	
	
	
	pthread_mutex_lock(&mutex3);// lock printing ----------------------
	write_consumer_is_done(id);
	pthread_mutex_unlock(&mutex3);// unlock printing ----------------------
	
	
	
	pthread_mutex_lock(&mutex7);// lock ----------------------
	consumers_done++;
	pthread_cond_signal(&cond3);
	pthread_mutex_unlock(&mutex7);// unlock ----------------------
	
	pthread_exit(0);
}
//------------------------------------------------------------------------
void free_list(list_node* node)
{
	if(node!=NULL)
	{
		free_list(node->next);
		free(node->item);
		free(node);
	}
}
//------------------------------------------------------------------------


