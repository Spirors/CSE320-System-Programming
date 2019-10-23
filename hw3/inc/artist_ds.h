#ifndef ARTIST_DS_H
#define ARTIST_DS_H

#include <sys/types.h>

struct artist_list {
	pid_t pid;
	char* status;
	struct artist_list* next_artist;
};

extern struct artist_list* head;

struct artist_list* new_artist(pid_t pidX);
void free_artist_list(struct artist_list* head);
void sortedInsert_artist(struct artist_list** head_ref, struct artist_list* new_artist);
void delete_artist(struct artist_list** head_ref, pid_t pidX);
void update_status(struct artist_list** head_ref, pid_t pidX, char* status_p);
void print_list(struct artist_list* head);
char* getstatus(struct artist_list* head, pid_t pidX);
int pid_exist(struct artist_list* head, pid_t pidX);

#endif

