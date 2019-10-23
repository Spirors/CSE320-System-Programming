#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

#include "warehouse.h"


int count_word(char* s){
	int count = 1;
	int i;
	for (i = 0; s[i] != '\0';i++){
		if (s[i] == ' '){
			count++;
		}
	}
	return count;
}

int main(int argc, char** argv) {

	int opt;
	int qflag = 0;
	int wflag = 0;
	int aflag = 0;
	int sflag = 0;

	char* w_filename;
	char* a_filename;
	
	char* s_mode;
	char* s_art = "s";
	char* s_price = "p";

	extern char *optarg;
	extern int optind, optopt;
	int print_mode;
	
	while((opt = getopt(argc, argv, "qw:a:s:")) != -1){
		switch(opt){
			case 'q':
				qflag++;
				if(qflag > 1){
					printf("multiple -q flag\n");
					return 1;
				}
				break;
			case 'w':
				wflag++;
				if(wflag > 1){
					printf("Multiple -w flag\n");
					return 1;
				}
				w_filename = optarg;
				break;
			case 'a':
				aflag++;
				if(aflag > 1){
					printf("Multiple -a flag\n");
					return 1;
				}
				a_filename = optarg;
				break;
			case 's':
				sflag++;
				if(sflag > 1){
					printf("Multiple -s flag\n");
					return 1;
				}
				s_mode = optarg;
				if(strcmp(s_mode, s_art) == 0){ 
					print_mode = 's';
				}else if(strcmp(s_mode, s_price) == 0){
					print_mode = 'p';
				}else{	
					printf("Incorrect -s argument\n");
					return 1;
				}
				break;
			case ':':
				printf("Option -%c requires an filename\n", optopt);
				return 1;
				break;
			case '?':
				printf("Unrecognized option: -%c\n", optopt);
				return 1;
				break;
			default:
				return 1;
		}
	}
	//note if qflag is 0, and wflag+aflag != 0 we have a problem
	FILE* fp;
	char line[255];
	int e_print;
	int e_insert_art;
	struct warehouse* warehouse;
	struct warehouse_list* warehouse_list;
	struct warehouse_sf_list* warehouse_sf_list;
	struct art_collection* art_collection;

	char name[255];
	int size;
	int price;

	int id;
	int size_w;
	char type[255];
	
	const char* public = "public";
	const char* private = "private";
	int type_num;

	if(qflag == 0 && wflag+aflag > 0){
		printf("quiet mode not enable\n");	
		return 1;
	}
	if(qflag == 1 && wflag == 1 && aflag == 0){
		printf("missing a flag\n");
		return 1;
	}
	if(qflag == 1 && wflag == 0 && aflag == 1){
		printf("missing w flag\n");
		return 1;
	}
	if(qflag == 1 && wflag == 1 && aflag == 1){
		fp = fopen(w_filename, "r");
		if(fp == NULL){
			printf("file cannot be open: %s\n", w_filename);
		}else{
			while(fgets(line, 255, fp)){
				sscanf(line, "%d %d %s", &id, &size_w, type);
				if(id <= 0){															
					printf("ID is required to be positive\n");					
					break;
				}
				if(dupe_id_exist(&sf_head, id) == 0){ 
					printf("Duplicate id: %d\n", id); 
					break; 
				}
				if(size_w < 4 || size_w % 2 != 0){ 
					printf("Invalid warehouse size\n"); 
					break; 
				}
				if(strcmp(type, public) == 0){ 
					type_num = 0x0; 
				}else if(strcmp(type, private) == 0){ 
					type_num = 0x2; }
				else{ 
					printf("Invalid warehouse type\n"); 
					break; 
				} 
				if(size_exist(&sf_head, size_w) != 0){ 
					warehouse = new_warehouse(id, size_w); 
					warehouse_list = new_warehouse_list(warehouse, type_num); 
					warehouse_sf_list = new_warehouse_sf_list(warehouse_list); 
					sortedInsert_sf_list(&sf_head, warehouse_sf_list); }
				else{ 
					warehouse = new_warehouse(id, size_w); 
					warehouse_list = new_warehouse_list(warehouse, type_num); 
					insert_warehouse_list(&sf_head, warehouse_list); 
				} 
			} 
			fclose(fp);
		}
		
		fp = fopen(a_filename, "r"); 
		if(fp == NULL){ 
			printf("file cannot be open: %s\n", a_filename); 
		}else{ 
			while(fgets(line, 255, fp)){ 
				if(count_word(line) >= 4){ 
					sscanf(line, "\"%[^\"]\" %d %d", name, &size, &price); 
					art_collection = new_art_collection(name, size, price); 
					e_insert_art = insert_art(&sf_head, art_collection); 
					if(e_insert_art != 0){ 
						printf("Error with inserting art collection due to lack of space\n"); 
						break; 
					} 
				}else if(count_word(line) == 3){ 
					sscanf(line, "%s %d %d", name, &size, &price); 
					art_collection = new_art_collection(name, size, price); 
					e_insert_art = insert_art(&sf_head, art_collection); 
					if(e_insert_art != 0){ 
						printf("Error with inserting art collection due to lack of space\n");
						break; 
					} 
				}else{ 
					printf("Error with parsing a line from the art file\n");
					return 0;
				}
			} 
			fclose(fp); 
		}
		e_print = print_list(&sf_head, 2, print_mode, &art_head); 
		if(e_print != 0){ 
			printf("print error, possibly due to uninitialize of warehouses and or of art collection\n"); 
		}
		art_head = NULL;

		free_warehouse_sf_list(sf_head);
		return 0;
	}

	const char* help_cmd = "help";
	const char* load_warehouse_cmd = "load warehouse";
	const char* load_art_cmd = "load art";
	const char* printall_cmd = "printall";
	const char* print_public_cmd = "print public";
	const char* print_private_cmd = "print private";
	const char* add_art_cmd = "add art";
	const char* delete_art_cmd = "delete art";
	const char* utilization_cmd = "utilization";
	const char* exit_cmd = "exit";

	char buffer[255];

	char cmd_w1[255];
	char cmd_w2[255];

	char filename[255];

	int exit = 0;
	while(exit == 0){
			fgets(buffer, 255, stdin);
			buffer[strcspn(buffer, "\n")] = 0;
			if(strcmp(buffer, exit_cmd) == 0){
				exit = 1;	
				free_warehouse_sf_list(sf_head);
			}else if(strcmp(buffer, help_cmd) == 0){
				printf("help - This command print a help message giving a brief description of\neach command.\n\n");
				printf("load warehouse - This command populate your linked list of warehouses\nwith a require filename argument. Each line in the file represents\ninformation about warehouse and have the following format:\nID SIZE TYPE\n\n");
				printf("load art - This command populate your warehouses with art collections\nwith a require filename argument. Each line in the file represents\ninformation about art collection and have the following format:\nNAME SIZE PRICE\n\n");
				printf("printall - This command print information about all art collection in the\ndatabase. Each art collection is on a separate line and adhere to the following format:\nNAME SIZE PRICE\n\n");
				printf("print public - This command print information about all art collections in\nthe database that are stored in public warehouses. Each art collection is\non a separate line and adhere to the following format:\nNAME SIZE PRICE\n\n");
				printf("print private - This command print information about all art collection in\nthe database that are stored in private warehouses. Each art collection is\non a separate line and adhere to the following format:\nNAME SIZE PRICE\n\n");
				printf("add art - This command should attempt to add a new art collection to the\ndatabase with provided attributes:\nNAME SIZE PRICE\n\n");
				printf("delete art - This command attempt takes an attribute name in order to\ndelete all art collections with matching name.\n\n");
				printf("utilization - This command print two numbers each on a separate line.\nFirst number is the ratio of occupied warehouses versus total number of\nwarehouses. The second number is the ratio of total size of all art\ncollections versus total capacity of all warehouses.\n\n");
				printf("exit - This command exit out of this program.\n\n");
			}else if(strcmp(buffer, printall_cmd) == 0){
				e_print = print_list(&sf_head, 2, print_mode, &art_head);
				if(e_print != 0){
					printf("print error, possibly due to uninitialize of warehouses and or of art collection\n");
				}
				art_head = NULL;
			}else if(strcmp(buffer, utilization_cmd) == 0){
				e_print = utilization(&sf_head);
				if(e_print != 0){
					printf("No warehouse available\n");
				}
			}else if(strcmp(buffer, print_public_cmd) == 0){
				e_print = print_list(&sf_head, 0, print_mode, &art_head);
				if(e_print != 0){
					printf("print error, possibly due to uninitialize of warehouses and or of art collection\n");
				}
				art_head = NULL;
			}else if(strcmp(buffer, print_private_cmd) == 0){
				e_print = print_list(&sf_head, 1, print_mode, &art_head);
				if(e_print != 0){
					printf("print error, possibly due to uninitialize of warehouses and or of art collection\n");
				}
				art_head = NULL;
			}else if(count_word(buffer) == 3){
				if(strncmp(buffer, load_warehouse_cmd, 14) == 0){

					sscanf(buffer, "%s %s %s", cmd_w1, cmd_w2, filename);

					fp = fopen(filename, "r");
					if(fp == NULL){
						printf("file cannot be open: %s\n", filename);
					}else{
						while(fgets(line, 255, fp)){
							sscanf(line, "%d %d %s", &id, &size_w, type);
							if(id <= 0){
								printf("ID is required to be positive\n");
								break;
							}
							if(dupe_id_exist(&sf_head, id) == 0){
								printf("Duplicate id: %d\n", id);
								break;
							}
							if(size_w < 4 || size_w % 2 != 0){
								printf("Invalid warehouse size\n");
								break;
							}
							if(strcmp(type, public) == 0){
								type_num = 0x0;
							}else if(strcmp(type, private) == 0){
								type_num = 0x2;
							}else{
								printf("Invalid warehouse type\n");
								break;
							}
							if(size_exist(&sf_head, size_w) != 0){
								warehouse = new_warehouse(id, size_w);
								warehouse_list = 
										new_warehouse_list(warehouse, type_num);
								warehouse_sf_list = 
										new_warehouse_sf_list(warehouse_list);
								sortedInsert_sf_list(&sf_head, warehouse_sf_list);
							}else{
								warehouse = new_warehouse(id, size_w);
								warehouse_list =
										new_warehouse_list(warehouse, type_num);
								insert_warehouse_list(&sf_head, warehouse_list);
							}
						}
						fclose(fp);
					}
				}else if(strncmp(buffer, load_art_cmd, 8) == 0){
					sscanf(buffer, "%s %s %s", cmd_w1, cmd_w2, filename);
					
					fp = fopen(filename, "r");
					if(fp == NULL){
						printf("file cannot be open: %s\n", filename);
					}else{
						while(fgets(line, 255, fp)){
							if(count_word(line) >= 4){
								sscanf(line, "\"%[^\"]\" %d %d", name, &size, &price);
								art_collection = new_art_collection(name, size, price);
								e_insert_art = insert_art(&sf_head, art_collection);
								if(e_insert_art != 0){
									printf("Error with inserting art collection due to lack of space\n");
									break;
								}
							}else if(count_word(line) == 3){
								sscanf(line, "%s %d %d", name, &size, &price);
								art_collection = new_art_collection(name, size, price);
								e_insert_art = insert_art(&sf_head, art_collection);
								if(e_insert_art != 0){
									printf("Error with inserting art collection due to lack of space\n");
									break;
								}
							}else{
								exit = 1;
								free_warehouse_sf_list(sf_head);
								printf("Error with parsing a line from the art file\n");
								break;
							}
						}
						fclose(fp);
					}
				}else if(strncmp(buffer, delete_art_cmd, 10) == 0){
					sscanf(buffer, "%s %s %s", cmd_w1, cmd_w2, name);
					e_insert_art = delete_art(&sf_head, name);
					if(e_insert_art != 0){
						printf("Name not found\n");
					}
				}else{
					printf("Invalid command, please use help for further infomation\n");
				}
			}else if(count_word(buffer) > 3 ){
				if(strncmp(buffer, add_art_cmd, 7) == 0){
					if(count_word(buffer) >= 6){
						sscanf(buffer, "%s %s \"%[^\"]\" %d %d", 
										cmd_w1, cmd_w2, name, &size, &price);
						art_collection = new_art_collection(name, size, price);
						e_insert_art = insert_art(&sf_head, art_collection);
						if(e_insert_art != 0){
							printf("Error with inserting art collection\n");
						}
					}else if(count_word(buffer) == 5){
						sscanf(buffer, "%s %s %s %d %d", 
										cmd_w1, cmd_w2, name, &size, &price);
						art_collection = new_art_collection(name, size, price);
						e_insert_art = insert_art(&sf_head, art_collection);
						if(e_insert_art != 0){
							printf("Error with inserting art collection\n");
						}
					}else{
						printf("Error with attribute\n");
					}
				}else if(strncmp(buffer, delete_art_cmd, 10) == 0){
					sscanf(buffer, "%s %s \"%[^\"]\"", cmd_w1, cmd_w2, name);
					e_insert_art = delete_art(&sf_head, name);
					if(e_insert_art != 0){
						printf("Name not found\n");
					}
				}else{
					printf("Invalid command, please use help for further infomation\n");
				}
			}else{
				printf("Invalid command, please use help for further infomation\n");
			}
	}
	return 0;
}
