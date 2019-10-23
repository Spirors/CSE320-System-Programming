#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include "warehouse.h"

struct art_collection* new_art_collection(char* name_p, int size_p, int price_p){
	struct art_collection* new_art_collection = 
			(struct art_collection*)malloc(sizeof(struct art_collection));
	
	new_art_collection->name = (char*)malloc(strlen(name_p)+1*sizeof(char));
	strcpy(new_art_collection->name, name_p);

	new_art_collection->size = size_p;
	new_art_collection->price = price_p;

	return new_art_collection;
}
void free_art_collection(struct art_collection* collection){
	if(collection != NULL){
		free(collection->name);
		free(collection);
	}
}
struct warehouse* new_warehouse(int id_p, int size_p){
	struct warehouse* new_warehouse = 
			(struct warehouse*)malloc(sizeof(struct warehouse));
	
	new_warehouse->id = id_p;
	new_warehouse->size = size_p;
	new_warehouse->art_collection = NULL;

	return new_warehouse;
}
void free_warehouse(struct warehouse* warehouse){
	if(warehouse != NULL){
		free_art_collection(warehouse->art_collection);
		free(warehouse);
	}
}
//type_p -> 0x0 or 0x2, for public or private respectively. Note its free when initialized.
struct warehouse_list* new_warehouse_list(struct warehouse* warehouse_p, int type_p){
	struct warehouse_list* new_warehouse_list =
			(struct warehouse_list*)malloc(sizeof(struct warehouse_list));

	new_warehouse_list->meta_info = warehouse_p->size;
	new_warehouse_list->meta_info <<= 2;
	new_warehouse_list->meta_info ^= type_p;

	new_warehouse_list->warehouse = warehouse_p;
	new_warehouse_list->next_warehouse = NULL;

	return new_warehouse_list;
}
void free_warehouse_list(struct warehouse_list* warehouse_list_head){
	struct warehouse_list* temp;
	while(warehouse_list_head != NULL){
		temp = warehouse_list_head;
		warehouse_list_head = warehouse_list_head->next_warehouse;
		free_warehouse(temp->warehouse);
		free(temp);
	}
}
struct warehouse_sf_list* new_warehouse_sf_list(struct warehouse_list* warehouse_list_h){
	struct warehouse_sf_list* new_warehouse_sf_list =
			(struct warehouse_sf_list*)malloc(sizeof(struct warehouse_sf_list));

	new_warehouse_sf_list->class_size = warehouse_list_h->warehouse->size;
	new_warehouse_sf_list->warehouse_list_head = warehouse_list_h;
	new_warehouse_sf_list->sf_next_warehouse = NULL;
	
	return new_warehouse_sf_list;
}
void free_warehouse_sf_list(struct warehouse_sf_list* warehouse_sf_list_head){
	struct warehouse_sf_list* temp;
	while(warehouse_sf_list_head != NULL){
		temp = warehouse_sf_list_head;
		warehouse_sf_list_head = warehouse_sf_list_head->sf_next_warehouse;
		free_warehouse_list(temp->warehouse_list_head);
		free(temp);
	}
}
void sortedInsert_sf_list(struct warehouse_sf_list** sf_head_ref, 
									struct warehouse_sf_list* new_sf_node){
	struct warehouse_sf_list* current;

	if(*sf_head_ref == NULL || (*sf_head_ref)->class_size > new_sf_node->class_size){
		new_sf_node->sf_next_warehouse = *sf_head_ref;
		*sf_head_ref = new_sf_node;
	}else{
		current = *sf_head_ref;
		while(current->sf_next_warehouse != NULL &&
				current->sf_next_warehouse->class_size < new_sf_node->class_size){
			current = current->sf_next_warehouse;
		}
		new_sf_node->sf_next_warehouse = current->sf_next_warehouse;
		current->sf_next_warehouse = new_sf_node;
	}
}
// 0 = Dupe found 
int dupe_id_exist(struct warehouse_sf_list** sf_head_ref, int id_p){
	struct warehouse_sf_list* current;
	struct warehouse_list* head_t;
	if(*sf_head_ref == NULL){
		return -1;
	}else{
		current = *sf_head_ref;
		while(current != NULL){
			head_t = current->warehouse_list_head;
			while(head_t != NULL){
				if(head_t->warehouse->id == id_p){
					return 0;
				}
				head_t = head_t->next_warehouse;
			}
			current = current->sf_next_warehouse;
		}
	}
	return -1;
}
// -1 = size exist, 0 = DNE
int size_exist(struct warehouse_sf_list** sf_head_ref, int size_p){
	struct warehouse_sf_list* current;
	if(*sf_head_ref == NULL){
		return -1;
	}else{
		current = *sf_head_ref;
		while(current != NULL){
			if(current->class_size == size_p){
				return 0;
			}
			current = current->sf_next_warehouse;
		}
	}
	return -1;
}
void insert_warehouse_list(struct warehouse_sf_list** sf_head_ref,
								struct warehouse_list* new_warehouse_list){
	struct warehouse_sf_list* current;
	struct warehouse_list* head_t;
	if(*sf_head_ref == NULL){ //Probably not gonna trigger but just in case
		return;
	}else{
		current = *sf_head_ref;
		while(current->sf_next_warehouse != NULL &&
				current->sf_next_warehouse->class_size != new_warehouse_list->warehouse->size){
			current = current->sf_next_warehouse;
		}

		head_t = current->warehouse_list_head;
		while(head_t->next_warehouse != NULL){
			head_t = head_t->next_warehouse;
		}
		head_t->next_warehouse = new_warehouse_list;
	}
}
// -1 = not enough space, 0 insert sucess
int insert_art(struct warehouse_sf_list** sf_head_ref,
					struct art_collection* the_collection){
	struct warehouse_sf_list* current;
	struct warehouse_list* head_t;
	int a;
	
	if(*sf_head_ref == NULL){
		return -1;
	}else{
		current = *sf_head_ref;
		while(current != NULL){
			if(current->class_size >= the_collection->size){
				head_t = current->warehouse_list_head;
				while(head_t != NULL){
					a = head_t->meta_info & 1;
					if(a == 0){
						head_t->warehouse->art_collection = the_collection;
						head_t->meta_info ^= (1<<0);
						return 0;
					}
					head_t = head_t->next_warehouse;
				}
			}
			current = current->sf_next_warehouse;
		}
	}
	return -1;
}
// -1 name not found, 0 delete sucessful
int delete_art(struct warehouse_sf_list** sf_head_ref, char* name_p){
	struct warehouse_sf_list* current;
	struct warehouse_list* head_t;
	int a;
	int d = 0;
	
	if(*sf_head_ref == NULL){
		return -1;
	}else{
		current = *sf_head_ref;
		while(current != NULL){
			head_t = current->warehouse_list_head;
			while(head_t != NULL){
				a = head_t->meta_info & 1;
				if(a != 0){
					if(strcmp(head_t->warehouse->art_collection->name, name_p) == 0){
						head_t->meta_info ^= (1<<0);
						free_art_collection(head_t->warehouse->art_collection);
						head_t->warehouse->art_collection = NULL;
						d++;
					}
				}
				head_t = head_t->next_warehouse;
			}
			current = current->sf_next_warehouse;
		}
	}
	if(d > 0){
		return 0;
	}
	return -1;
}

int utilization(struct warehouse_sf_list** sf_head_ref){
	struct warehouse_sf_list* current;
	struct warehouse_list* head_t;
	int a;
	int count_o = 0;
	int count_t = 0;
	int art_t_size = 0;
	int capacity = 0;
	
	if(*sf_head_ref == NULL){
		return 1;
	}else{
		current = *sf_head_ref;
		while(current != NULL){
			head_t = current->warehouse_list_head;
			while(head_t != NULL){
				a = head_t->meta_info & 1;
				if(a != 0){
					count_o++;
					art_t_size += head_t->warehouse->art_collection->size;
				}
				count_t++;
				capacity += head_t->warehouse->size;
				head_t = head_t->next_warehouse;
			}
			current = current->sf_next_warehouse;
		}
	}
	printf("Occupied/Total: %d/%d\n", count_o, count_t);
	printf("Total art size/Total capacity: %d/%d\n", art_t_size, capacity);
	return 0;
}

struct art_collection_list* new_art_collection_list(struct art_collection* collection){
	struct art_collection_list* new_art_list = 
				(struct art_collection_list*)malloc(sizeof(struct art_collection_list));
	new_art_list->art_collection = collection;
	new_art_list->next_art_collection = NULL;

	return new_art_list;
}
// Note we are not freeing art_collection because it will be free elsewhere
void free_art_list(struct art_collection_list* art_head){
	struct art_collection_list* temp;
	while(art_head != NULL){
		temp = art_head;
		art_head = art_head->next_art_collection;
		free(temp);
		temp = NULL;
	}
}
void sort_art_insert(struct art_collection_list** art_head, struct art_collection_list* art,
																char mode){
	struct art_collection_list* current;
	if(mode == 's'){
		if(*art_head == NULL || (*art_head)->art_collection->size > art->art_collection->size){
			art->next_art_collection = *art_head;
			*art_head = art;
		}else{
			current = *art_head;
			while(current->next_art_collection != NULL &&
					current->next_art_collection->art_collection->size <
					art->art_collection->size){
				current = current->next_art_collection;				
			}
			art->next_art_collection = current->next_art_collection;
			current->next_art_collection = art;
		}
	}else if(mode == 'p'){
		if(*art_head == NULL || (*art_head)->art_collection->price > 
									art->art_collection->price){
			art->next_art_collection = *art_head;
			*art_head = art;
		}else{
			current = *art_head;
			while(current->next_art_collection != NULL &&
					current->next_art_collection->art_collection->price <
					art->art_collection->price){
				current = current->next_art_collection;				
			}
			art->next_art_collection = current->next_art_collection;
			current->next_art_collection = art;
		}
	}
}
void print_art(struct art_collection* art){
	printf("%s %d %d\n", art->name, 
						 art->size,
					     art->price);
}
void print_art_list(struct art_collection_list* art_head){
	struct art_collection_list* temp = art_head;
	while(temp != NULL){
		print_art(temp->art_collection);
		temp = temp->next_art_collection;
	}
}

//type 0 = public, 1 = private, 2 = all
int print_list(struct warehouse_sf_list** sf_head, int type, char mode, 
									struct art_collection_list** art_head){
	struct warehouse_sf_list* current;
	struct warehouse_list* head_t;
	struct art_collection_list* art_list;
	int a;
	int b;
	int total = 0;
	if(type == 0){
		if(mode == 's'){
			if(*sf_head == NULL){
				return -1;
			}else{
				current = *sf_head;
				while(current != NULL){
					head_t = current->warehouse_list_head;
					while(head_t != NULL){
						a = head_t->meta_info & 1;
						if(a != 0){
							b = head_t->meta_info & (1<<1);
							if(b == 0){
								art_list = new_art_collection_list(head_t->
																warehouse->art_collection);
								sort_art_insert(art_head, art_list, mode);
								total += head_t->warehouse->art_collection->price;
							}
						}
						head_t = head_t->next_warehouse;
					}
					current = current->sf_next_warehouse;
				}
			}
			print_art_list(*art_head);
			free_art_list(*art_head);
			printf("Total Worth: %d\n", total);
		}else if(mode == 'p'){
			if(*sf_head == NULL){
				return -1;
			}else{
				current = *sf_head;
				while(current != NULL){
					head_t = current->warehouse_list_head;
					while(head_t != NULL){
						a = head_t->meta_info & 1;
						if(a != 0){
							b = head_t->meta_info & (1<<1);
							if(b == 0){
								art_list = new_art_collection_list(head_t->
																warehouse->art_collection);
								sort_art_insert(art_head, art_list, mode);
								total += head_t->warehouse->art_collection->price;
							}
						}
						head_t = head_t->next_warehouse;
					}
					current = current->sf_next_warehouse;
				}
			}
			print_art_list(*art_head);
			free_art_list(*art_head);
			printf("Total Worth: %d\n", total);
		}else{
			if(*sf_head == NULL){
				return -1;
			}else{
				current = *sf_head;
				while(current != NULL){
					head_t = current->warehouse_list_head;
					while(head_t != NULL){
						a = head_t->meta_info & 1;
						if(a != 0){
							b = head_t->meta_info & (1<<1);
							if(b == 0){
								print_art(head_t->warehouse->art_collection);
								total += head_t->warehouse->art_collection->price;
							}
						}
						head_t = head_t->next_warehouse;
					}
					current = current->sf_next_warehouse;
				}
			}
			printf("Total Worth: %d\n", total);
		}
	}else if(type == 1){
		if(mode == 's'){
			if(*sf_head == NULL){
				return -1;
			}else{
				current = *sf_head;
				while(current != NULL){
					head_t = current->warehouse_list_head;
					while(head_t != NULL){
						a = head_t->meta_info & 1;
						if(a != 0){
							b = head_t->meta_info & (1<<1);
							if(b != 0){
								art_list = new_art_collection_list(head_t->
																warehouse->art_collection);		
								sort_art_insert(art_head, art_list, mode);
								total += head_t->warehouse->art_collection->price;
							}
						}
						head_t = head_t->next_warehouse;
					}
					current = current->sf_next_warehouse;
				}
			}
			print_art_list(*art_head);
			free_art_list(*art_head);
			printf("Total Worth: %d\n", total);
		}else if(mode == 'p'){
			if(*sf_head == NULL){
				return -1;
			}else{
				current = *sf_head;
				while(current != NULL){
					head_t = current->warehouse_list_head;
					while(head_t != NULL){
						a = head_t->meta_info & 1;
						if(a != 0){
							b = head_t->meta_info & (1<<1);
							if(b != 0){
								art_list = new_art_collection_list(head_t->
																warehouse->art_collection);		
								sort_art_insert(art_head, art_list, mode);
								total += head_t->warehouse->art_collection->price;
							}
						}
						head_t = head_t->next_warehouse;
					}
					current = current->sf_next_warehouse;
				}
			}
			print_art_list(*art_head);
			free_art_list(*art_head);
			printf("Total Worth: %d\n", total);
		}else{
			if(*sf_head == NULL){
				return -1;
			}else{
				current = *sf_head;
				while(current != NULL){
					head_t = current->warehouse_list_head;
					while(head_t != NULL){
						a = head_t->meta_info & 1;
						if(a != 0){
							b = head_t->meta_info & (1<<1);
							if(b != 0){
								print_art(head_t->warehouse->art_collection);
								total += head_t->warehouse->art_collection->price;
							}
						}
						head_t = head_t->next_warehouse;
					}
					current = current->sf_next_warehouse;
				}
			}
			printf("Total Worth: %d\n", total);
		}
	}else{
		if(mode == 's'){
			if(*sf_head == NULL){
				return -1;
			}else{
				current = *sf_head;
				while(current != NULL){
					head_t = current->warehouse_list_head;
					while(head_t != NULL){
						a = head_t->meta_info & 1;
						if(a != 0){
							art_list = new_art_collection_list(head_t->
																warehouse->art_collection);	
							sort_art_insert(art_head, art_list, mode);
							total += head_t->warehouse->art_collection->price;
						}
						head_t = head_t->next_warehouse;
					}
					current = current->sf_next_warehouse;
				}
			}
			print_art_list(*art_head);
			free_art_list(*art_head);
			printf("Total Worth: %d\n", total);
		}else if(mode == 'p'){
			if(*sf_head == NULL){
				return -1;
			}else{
				current = *sf_head;
				while(current != NULL){
					head_t = current->warehouse_list_head;
					while(head_t != NULL){
						a = head_t->meta_info & 1;
						if(a != 0){
							art_list = new_art_collection_list(head_t->
																warehouse->art_collection);	
							sort_art_insert(art_head, art_list, mode);
							total += head_t->warehouse->art_collection->price;
						}
						head_t = head_t->next_warehouse;
					}
					current = current->sf_next_warehouse;
				}
			}
			print_art_list(*art_head);
			free_art_list(*art_head);
			printf("Total Worth: %d\n", total);
		}else{
			if(*sf_head == NULL){
				return -1;
			}else{
				current = *sf_head;
				while(current != NULL){
					head_t = current->warehouse_list_head;
					while(head_t != NULL){
						a = head_t->meta_info & 1;
						if(a != 0){
							print_art(head_t->warehouse->art_collection);
							total += head_t->warehouse->art_collection->price;
						}
						head_t = head_t->next_warehouse;
					}
					current = current->sf_next_warehouse;
				}
			}
			printf("Total Worth: %d\n", total);
		} 
	}
	return 0;
}
