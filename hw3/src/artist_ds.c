#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "artist_ds.h"
#include "defs.h"

struct artist_list* new_artist(pid_t pidX){
	struct artist_list* new_artist = (struct artist_list*)malloc(sizeof(struct artist_list));

	new_artist->status = (char*)malloc(9*sizeof(char));
	strcpy(new_artist->status, "WAITING");

	new_artist->pid = pidX;
	new_artist->next_artist = NULL;

	return new_artist;
}

void sortedInsert_artist(struct artist_list** head_ref, struct artist_list* new_artist){
	struct artist_list* current;

	if(*head_ref == NULL || (*head_ref)->pid > new_artist->pid){
		new_artist->next_artist = *head_ref;
		*head_ref = new_artist;
	}else{
		current = *head_ref;
		while(current->next_artist != NULL && 
				current->next_artist->pid < new_artist->pid){
			current = current->next_artist;
		}
		new_artist->next_artist = current->next_artist;
		current->next_artist = new_artist;
	}
}

void free_artist_list(struct artist_list* head){
	struct artist_list* temp;
	while(head != NULL){
		temp = head;
		head = head->next_artist;
		free(temp->status);
		free(temp);
	}
}

void print_list(struct artist_list* head){
	struct artist_list* temp = head;
	while(temp != NULL){
		cse320_printf("%d %s\n", temp->pid, temp->status);
		temp = temp->next_artist;
	}
}

void delete_artist(struct artist_list** head_ref, pid_t pidX){
	struct artist_list* temp = *head_ref;
	struct artist_list* prev;
	
	if(temp != NULL && temp->pid == pidX){
		*head_ref = temp->next_artist;
		free(temp->status);
		free(temp);
		return;
	}
	while(temp != NULL && temp->pid != pidX){
		prev = temp;
		temp = temp->next_artist;
	}

	if(temp == NULL){
		return;
	}

	prev->next_artist = temp->next_artist;
	free(temp->status);
	free(temp);
}

void update_status(struct artist_list** head_ref, pid_t pidX, char* status_p){
	struct artist_list* temp = *head_ref;
	while(temp != NULL && temp->pid != pidX){
		temp = temp->next_artist;
	}
	if(temp == NULL){
		return;
	}
	strcpy(temp->status, status_p);
}

char* getstatus(struct artist_list* head, pid_t pidX){
	struct artist_list* temp = head;
	while(temp != NULL && temp->pid != pidX){
		temp = temp->next_artist;
	}
	if(temp == NULL){
		return "DNE";
	}
	return temp->status;
}

int pid_exist(struct artist_list* head, pid_t pidX){
	struct artist_list* temp = head;
	while(temp != NULL && temp->pid != pidX){
		temp = temp->next_artist;
	}
	if(temp == NULL){
		return -1;
	}
	return 0;
}
