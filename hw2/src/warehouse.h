#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <stdint.h>

struct art_collection {
    char* name;
    int size;
    int price;
};

struct warehouse {
    int id;
    int size;
    struct art_collection* art_collection;
};

/* This defines the internal structure of meta_info field. The two least significant bits
 * represents if the warehouse is occupied and if it is private or public. The rest of the
 * bits are for its size
structure of meta_info
    uint64_t size:62; // Size of the warehouse and should be multiple of 2 and at least 4
    uint64_t occupied:1; // Bit that indicates of the warehouse is occupied or not (1 is occupied and 0 is free)
    uint64_t is_private:1; // Defines ownership of the warehouse (1 is private and 0 is public)
*/

//inner list
struct warehouse_list {
    uint64_t meta_info; // Meta information about warehouse node; it is mimicking memory block header
    struct warehouse* warehouse; // Useful information about actual warehouse; think of payload
    struct warehouse_list* next_warehouse;
};

//outer list
struct warehouse_sf_list {
    // 'class_size' represents warehouse sizes that correspond to the list this node points to
    int class_size;
    struct warehouse_list* warehouse_list_head;
    struct warehouse_sf_list* sf_next_warehouse;
};

struct art_collection_list {
	struct art_collection* art_collection;
	struct art_collection_list* next_art_collection;
};

struct warehouse_sf_list* sf_head;
struct art_collection_list* art_head;

struct art_collection* new_art_collection(char* name_p, int size_p, int price_p);
void free_art_collection(struct art_collection* collection);
struct warehouse* new_warehouse(int id_p, int size_p);
void free_warehouse(struct warehouse* warehouse);

struct warehouse_list* new_warehouse_list(struct warehouse* warehouse_p, int type_p);
void free_warehouse_list(struct warehouse_list* warehouse_list_head);

struct warehouse_sf_list* new_warehouse_sf_list(struct warehouse_list* warehouse_list_h);
void free_warehouse_sf_list(struct warehouse_sf_list* warehouse_sf_list_head);

void sortedInsert_sf_list(struct warehouse_sf_list** sf_head_ref, 
								struct warehouse_sf_list* new_sf_node);
int dupe_id_exist(struct warehouse_sf_list** sf_head_ref, int id_p);
int size_exist(struct warehouse_sf_list** sf_head_ref, int size_p);
void insert_warehouse_list(struct warehouse_sf_list** sf_head_ref,
								struct warehouse_list* new_warehouse_list);
int insert_art(struct warehouse_sf_list** sf_head_ref,
					struct art_collection* the_collection);
int delete_art(struct warehouse_sf_list** sf_head_ref, char* name_p);
int utilization(struct warehouse_sf_list** sf_head_ref);


struct art_collection_list* new_art_collection_list(struct art_collection* collection);
void free_art_list(struct art_collection_list* art_head);

void sort_art_insert(struct art_collection_list** art_head, struct art_collection_list* art,
						char mode);

void print_art(struct art_collection* art);
void print_art_list(struct art_collection_list* art_head);
int print_list(struct warehouse_sf_list** sf_head, int type, char mode, 
										struct art_collection_list** art_head);

#endif /* WAREHOUSE_H */
