#ifndef ARTIST_DS_H
#define ARTIST_DS_H

#include <sys/types.h>

struct artist_list {
	pthread_t tid;
	char* status;
	struct artist_list* next_artist;
};

extern struct artist_list* head;

struct artist_list* new_artist(pthread_t tidX);
void free_artist_list(struct artist_list* head);
void sortedInsert_artist(struct artist_list** head_ref, struct artist_list* new_artist);
void delete_artist(struct artist_list** head_ref, pthread_t tidX);
void update_status(struct artist_list** head_ref, pthread_t tidX, char* status_p);
void print_list(struct artist_list* head);
char* getstatus(struct artist_list* head, pthread_t tidX);
int tid_exist(struct artist_list* head, pthread_t tidX);

#endif

