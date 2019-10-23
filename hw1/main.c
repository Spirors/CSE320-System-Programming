#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

int compute_int(char* b){
	int a = 0;
	while(*b != '\0'){
		if(*b<48 || *b>57){
			return -1;
		}
		else{
			a=a*10;
			a=a + (*b-48);
		}
		b++;
	}
	return a;
}

int strcmp(const char* s1, const char* s2){
	while(*s1 && *s1==*s2){
		s1++;
		s2++;
	}
	return *s1 - *s2;
}
char* strcpy(char *d, const char *s){
	char *saved = d;
	while(*s){
		*d++ = *s++;
	}
	*d = 0;
	return saved;
}

//Return 1 when there is non-alphabetic character, 0 otherwise
int char_alpha(char* s){
	char* t;
	for(t = s; *t != '\0'; t++){
		//if isalpha() = 0, then detected non-alphabetic character
		if(isalpha(*t) == 0){
			return 1;
		}
	}
	return 0;
}

int length(char* str){
	char* t;
	int len = 0;
	
	while(*str != '\0'){
		str++;
		len++;
	}
	return len;
}

int type_check(char* type){
	int len = length(type);
	if(len > 10){
		return 1;
	}

	if(char_alpha(type) == 1){
		return 1;
	}

	return 0;
}

int isalpha_and_space(char* s){
	char* t;
	for(t = s; *t != '\0'; t++){
		//if isalpha() = 0, then detected non-alphabetic character
		if(isalpha(*t) == 0 && *t != ' '){
			return 1;
		}
	}
	return 0;
}

int art_check(char* art_name){
	int len = length(art_name);
	if(len > 50 || len < 3){
		return 1;
	}
	if(isalpha_and_space(art_name) == 1){
		return 1;
	}
	return 0;
}

int artist_check(char* artist){
	int len = length(artist);
	if(len > 20 || len < 3){
		return 1;
	}
	if(isalpha_and_space(artist) == 1){
		return 1;
	}
	return 0;
}

void all_cap(char* s){
	char* t;
	for(t=s; *t=toupper(*t); ++t);
}

void cap_initial(char* s){
	char* t;
	t = s;
	if(islower(*t)){
		*t = toupper(*t);
	}
	t++;
	while(*t != '\0'){
		if(isupper(*t)){
			*t = tolower(*t);
		}
		t++;
	}
}

typedef struct {	
	int id_num;
	char* art_type;
	char* art_name;
	char* artist;
	int price;
} art_piece;

art_piece newData(int id, char* type, char* art, char* artist_n, int art_price){
	art_piece new_data;
	new_data.art_type = (char*)malloc(11);
	new_data.art_name = (char*)malloc(51);
	new_data.artist = (char*)malloc(21);

	new_data.id_num = id;
	strcpy(new_data.art_type, type);
	strcpy(new_data.art_name, art);
	strcpy(new_data.artist, artist_n);
	new_data.price = art_price;

	return new_data;
}
void freeData(art_piece a){
	free(a.art_type);
	free(a.art_name);
	free(a.artist);
}

struct art_pieces { 
	art_piece data; 
	struct art_pieces *next; 
};

struct art_pieces* newNode(art_piece new_data){
	struct art_pieces* new_node = (struct art_pieces*)malloc(sizeof(struct art_pieces));
	new_node->data = new_data;
	new_node->next = NULL;
	return new_node;
}

typedef struct{
	int iflag;
        int idArg;   
	int tflag;
	char* typeArg;
	int nflag;
	char* artistArg;
}f_args;

f_args newArg(int i_flag, int t_flag, int n_flag, int id, char* type, char* artist){
	f_args newArg;
	newArg.typeArg = (char*)malloc(11);
	newArg.artistArg = (char*)malloc(21);

	newArg.iflag = i_flag;
	newArg.tflag = t_flag;
	newArg.nflag = n_flag;
	
	if(i_flag == 0){
		newArg.idArg = 0;
	}else{
		newArg.idArg = id;
	}
	if(t_flag == 0){
		newArg.typeArg = NULL;
	}else{
		strcpy(newArg.typeArg, type);
		all_cap(newArg.typeArg);
	}
	if(n_flag == 0){
		newArg.artistArg = NULL;
	}else{
		strcpy(newArg.artistArg, artist);
		cap_initial(newArg.artistArg);
	}

	return newArg;
}

void freeArg(f_args f){
	free(f.typeArg);
	free(f.artistArg);
}

//returns 0 if id exist, 1 if id does not exit
int id_exist(int id, struct art_pieces *head){
	struct art_pieces *temp = head;
	while(temp != NULL){
		if(temp->data.id_num == id){
			return 0;
		}
		temp = temp->next;
	}
	return 1;
}

//Return 0 for success, 1 for "ID NOT UNIQUE"
int sortedInsert(struct art_pieces **head_ref, struct art_pieces* new_node){
	struct art_pieces* current;

	if(id_exist(new_node->data.id_num, *head_ref) == 0){
		return 1;
	}

	if(*head_ref == NULL || (*head_ref)->data.id_num > new_node->data.id_num){
		new_node->next = *head_ref;
		*head_ref = new_node;
	}
	else{
		current = *head_ref;
		while(current->next != NULL &&
		      current->next->data.id_num < new_node->data.id_num){
			current = current->next;
		}
		new_node->next = current->next;
		current->next = new_node;
	}
	return 0;
}

void printArt(art_piece a){
	printf("%d %s %s %s %d\n", a.id_num, a.art_type, a.art_name, a.artist, a.price);
}

void printList(struct art_pieces *head) { 
	struct art_pieces *temp = head;
	while(temp != NULL) {          
		printArt(temp->data);
		temp = temp->next; 
	}
}

//Head used to traverse since this is the last thing you do.
void freeList(struct art_pieces *head){
	struct art_pieces *temp;
	while(head != NULL) {  
		temp = head;	
		head = head->next;
		freeData(temp->data);
		free(temp);
	}
}

//Return 0 for success, 1 for "RECORD CANNOT BE DELETED NOR UPDATED"
int deleteID(struct art_pieces **head_ref, int id){
	struct art_pieces *temp = *head_ref;
	struct art_pieces *prev;

	//when head is the targeted node
	if(temp != NULL && temp->data.id_num == id){
		*head_ref = temp->next;
		freeData(temp->data);
		free(temp);
		return 0;
	}

	while(temp != NULL && temp->data.id_num != id){
		prev = temp;
		temp = temp->next;
	}
	
	if(temp == NULL){
		return 1;
	}

	prev->next = temp->next;
	freeData(temp->data);
	free(temp);
	return 0;
}

//Return 0 for success, 1 for "RECORD CANNOT BE DELETED NOR UPDATED"
int updateData(struct art_pieces *head, art_piece a){
	struct art_pieces *temp = head;
	while(temp != NULL && temp->data.id_num != a.id_num){
		temp = temp->next;
	}
	
	if(temp == NULL){
		return 1;
	}
	temp->data = a;
	return 0;
}

int sum_of_collection(struct art_pieces *head){
	int sum = 0;
	struct art_pieces *temp = head;
	while(temp != NULL){
		sum+=temp->data.price;
		temp = temp->next;
	}
	return sum;
}

int filter_by_flag(struct art_pieces *head, f_args f){
	struct art_pieces *temp = head;
	int found = 0;
	if(f.iflag + f.tflag + f.nflag == 3){
		while(temp != NULL){
			if(	temp->data.id_num == f.idArg && 
				strcmp(temp->data.art_type, f.typeArg) == 0 && 
				strcmp(temp->data.artist, f.artistArg) == 0){
				printf("%d %s %s %s %d\n", temp->data.id_num, temp->data.art_type,
					       		   temp->data.art_name, temp->data.artist,
					       		   temp->data.price);
				found++;
			}
			temp = temp->next;
		}
	}
	else if(f.iflag + f.tflag == 2){
		while(temp != NULL){
			if(	temp->data.id_num == f.idArg && 
				strcmp(temp->data.art_type, f.typeArg) == 0){
				printf("%d %s %s %s %d\n", temp->data.id_num, temp->data.art_type,
					       		   temp->data.art_name, temp->data.artist,
					       		   temp->data.price);
				found++;
			}
			temp = temp->next;
		}
	}
	else if(f.iflag + f.nflag == 2){
		while(temp != NULL){
			if(	temp->data.id_num == f.idArg && 
				strcmp(temp->data.artist, f.artistArg) == 0){
				printf("%d %s %s %s %d\n", temp->data.id_num, temp->data.art_type,
					       		   temp->data.art_name, temp->data.artist,
					       		   temp->data.price);
				found++;
			}
			temp = temp->next;
		}
	}
	else if(f.tflag + f.nflag == 2){
		while(temp != NULL){
			if(	strcmp(temp->data.art_type, f.typeArg) == 0 && 
				strcmp(temp->data.artist, f.artistArg) == 0){
				printf("%d %s %s %s %d\n", temp->data.id_num, temp->data.art_type,
					       		   temp->data.art_name, temp->data.artist,
					       		   temp->data.price);
				found++;
			}
			temp = temp->next;
		}
	}
	else if(f.iflag == 1){
		while(temp != NULL){
			if(temp->data.id_num == f.idArg) {
				printf("%d %s %s %s %d\n", temp->data.id_num, temp->data.art_type,
					       		   temp->data.art_name, temp->data.artist,
					       		   temp->data.price);
				found++;
			}
			temp = temp->next;
		}
	}
	else if(f.tflag == 1){
		while(temp != NULL){
			if(strcmp(temp->data.art_type, f.typeArg)==0){
				printf("%d %s %s %s %d\n", temp->data.id_num, temp->data.art_type,
					       		   temp->data.art_name, temp->data.artist,
					       		   temp->data.price);
				found++;
			}
			temp = temp->next;
		}
	}
	else if(f.nflag == 1){
		while(temp != NULL){
			if(strcmp(temp->data.artist, f.artistArg)==0){
				printf("%d %s %s %s %d\n", temp->data.id_num, temp->data.art_type,
					       		   temp->data.art_name, temp->data.artist,
					       		   temp->data.price);
				found++;
			}
			temp = temp->next;
		}
	}
	return found;
}

int main(int argc, char** argv) {
	/*
	*   * Dummy values
	*    
	int id = 1;
	char* art_type = "Painting";
	char* art_name = "Girl on the ball";
	char* artist_name = "Picasso";
	int price = 100;
	*/

	struct art_pieces* head = NULL;

	char* filename;
	int monies=10;
	int id;
	char* type;
	char* artist_name;
	char* filename_o;

	int i;
	char *p;

	int bflag = 0;
	int vflag = 0;
	int iflag = 0;
	int tflag = 0;
	int nflag = 0;
	int oflag = 0;

	for(i=2; i<argc; i++){
		p = *(argv+i);
		if(*p == '-'){
			p++;
			while(*p != '\0'){
				switch(*p){
					case 'b':
						if(*(*(argv+i)+2) != '\0'){
							printf("OTHER ERROR");
							return -1;
						}
						if(bflag == 0){
							bflag++;
						}
						else{
							printf("OTHER ERROR");
							return -1;
						}
						char* b_pointer;
						b_pointer = *(argv+i+1);
						monies = compute_int(b_pointer);
						if(monies == -1){
							printf("OTHER ERROR");
							return -1;
						}

						b_pointer = NULL;
						break;
					case 'v':
						if(*(*(argv+i)+2) != '\0'){
							printf("OTHER ERROR");
							return -1;
						}
						if(vflag == 0){
							vflag++;
						}
						else{
							printf("OTHER ERROR");
							return -1;
						}
						break;
					case 'i':
						if(*(*(argv+i)+2) != '\0'){
							printf("OTHER ERROR");
							return -1;
						}
						if(iflag == 0){
							iflag++;
						}
						else{
							printf("OTHER ERROR");
							return -1;
						}					
						char* i_pointer;
						i_pointer = *(argv+i+1);
						id = compute_int(i_pointer);
						if(id == -1){
							printf("OTHER ERROR");
							return -1;
						}
						i_pointer = NULL;
						break;
					case 't':
						if(*(*(argv+i)+2) != '\0'){
							printf("OTHER ERROR");
							return -1;
						}
						if(tflag == 0){
							tflag++;
						}
						else{
							printf("OTHER ERROR");
							return -1;
						}					
						type = *(argv+i+1);
						if(type_check(type)==1){
							printf("OTHER ERROR");
							return -1;
						} 
						break;
					case 'n':
						if(*(*(argv+i)+2) != '\0'){
							printf("OTHER ERROR");
							return -1;
						}
						if(nflag == 0){
							nflag++;
						}
						else{
							printf("OTHER ERROR");
							return -1;
						}
						artist_name = *(argv+i+1);
						if(artist_check(artist_name) == 1){
							printf("OTHER ERROR");
							return -1;
						}
						break;
					case 'o':
						if(*(*(argv+i)+2) != '\0'){
							printf("OTHER ERROR");
							return -1;
						}
						if(oflag == 0){
							oflag++;
						}
						else{
							printf("OTHER ERROR");
							return -1;
						}
						filename_o = *(argv+i+1);
						break;
					default:
						printf("OTHER ERROR");
						return -1;
				}
				p++;
			}
		}
	}
	int amount_of_flag = vflag+iflag+tflag+nflag;
	if(amount_of_flag == 0){
		printf("NO QUERY PROVIDED");
		return -1;
	}
	if(vflag == 1){
		if(iflag == 1){
			printf("OTHER ERROR");
			return -1;
		}
		if(tflag == 1){
			printf("OTHER ERROR");
			return -1;
		}
		if(nflag == 1){
			printf("OTHER ERROR");
			return -1;
		}
	}

	filename = *(argv+1);	
	FILE *pf;
	pf = fopen(filename, "r");
	if(pf == NULL){
		printf("OTHER ERROR");
	}else{
		char* line;
		line =(char*)malloc(256);

		char* buy = "BUY";
		char* update = "UPDATE";
		char* sell = "SELL";

		char* cmd;
		cmd = (char*)malloc(7);

		int id_s;

		char* type_s;
		type_s = (char*)malloc(51);

		char* art_s;
		art_s = (char*)malloc(11);
		
		char* artist_s;
		artist_s = (char*)malloc(21);
		
		int price_s;

		struct art_pieces* new_node;
		art_piece a1;
		while(fgets(line, 256, pf)){
			sscanf(line, "%s", cmd);
			if(strcmp(cmd, buy) == 0){
				int a;
				
				//No Quoted string
				a = sscanf(line, "%s %d %s %[^\" ] %[^\" ] %d", cmd, &id_s, 
						type_s, art_s, artist_s, &price_s);
				if(a != 6){
					//"Quoted Art Name"
					a = sscanf(line, "%s %d %s \"%[^\"]\" %[^\" ] %d", cmd, 
						&id_s, type_s, art_s, artist_s, &price_s);
				}
				if(a != 6){
					//"Quoted Art Name" "Quoted Artist Name"
					a = sscanf(line, "%s %d %s \"%[^\"]\" \"%[^\"]\" %d", cmd, 
						&id_s, type_s, art_s, artist_s, &price_s);
				}
				if(a != 6){
					//"Quoted Artist Name"
					a = sscanf(line, "%s %d %s %[^\" ] \"%[^\"]\" %d", cmd, 
						&id_s, type_s, art_s, artist_s, &price_s);
				}
				if(a != 6){
					printf("FAILED TO PARSE FILE");
					return -1;
				}else{
					if(id_s <= 0){
						printf("FAILED TO PARSE FILE");
						return -1;
					}
					if(type_check(type_s)==1){
						printf("FAILED TO PARSE FILE");
						return -1;
					}
					if(art_check(art_s)==1){
						printf("FAILED TO PARSE FILE");
						return -1;
					}
					if(artist_check(artist_s)==1){
						printf("FAILED TO PARSE FILE");
						return -1;
					}
					if(price_s <= 0){
						printf("FAILED TO PARSE FILE");
						return -1;
					}
					if(price_s > monies){
						printf("NOT ENOUGH MONIES");
						return -1;
					}

					a1=newData(id_s, type_s, art_s, artist_s, price_s);
					
					all_cap(a1.art_type);
					cap_initial(a1.art_name);
					cap_initial(a1.artist);

					new_node = newNode(a1);
					sortedInsert(&head, new_node);
					monies -= price_s;
				}
			}else if(strcmp(cmd, update) == 0){
				int a;
				
				//No Quoted string
				a = sscanf(line, "%s %d %s %[^\" ] %[^\" ] %d", cmd, &id_s, 
						type_s, art_s, artist_s, &price_s);
				if(a != 6){
					//"Quoted Art Name"
					a = sscanf(line, "%s %d %s \"%[^\"]\" %[^\" ] %d", cmd, 
						&id_s, type_s, art_s, artist_s, &price_s);
				}
				if(a != 6){
					//"Quoted Art Name" "Quoted Artist Name"
					a = sscanf(line, "%s %d %s \"%[^\"]\" \"%[^\"]\" %d", cmd, 
						&id_s, type_s, art_s, artist_s, &price_s);
				}
				if(a != 6){
					//"Quoted Artist Name"
					a = sscanf(line, "%s %d %s %[^\" ] \"%[^\"]\" %d", cmd, 
						&id_s, type_s, art_s, artist_s, &price_s);
				}
				if(a != 6){
					printf("FAILED TO PARSE FILE");
					return -1;
				}else{
					if(id_s <= 0){
						printf("FAILED TO PARSE FILE");
						return -1;
					}
					if(type_check(type_s)==1){
						printf("FAILED TO PARSE FILE");
						return -1;
					}
					if(art_check(art_s)==1){
						printf("FAILED TO PARSE FILE");
						return -1;
					}
					if(artist_check(artist_s)==1){
						printf("FAILED TO PARSE FILE");
						return -1;
					}
					if(price_s <= 0){
						printf("FAILED TO PARSE FILE");
						return -1;
					}

					a1=newData(id_s, type_s, art_s, artist_s, price_s);
					
					all_cap(a1.art_type);
					cap_initial(a1.art_name);
					cap_initial(a1.artist);

					int er = updateData(head, a1);
					if(er == 1){
						printf("RECORD CANNOT BE DELETED NOR UPDATED");
						return -1;
					}
				}	
			}else if(strcmp(cmd, sell) == 0){
				int a;
				a = sscanf(line, "%s %d %d", cmd, &id_s, &price_s);
				if(a != 3){
					printf("FAILED TO PARSE FILE");
					return -1;
				}else{
					if(id_s <= 0){
						printf("FAILED TO PARSE FILE");
						return -1;
					}
					if(price_s <= 0){
						printf("FAILED TO PARSE FILE");
						return -1;
					}
					int er = deleteID(&head, id_s);
					if(er == 1){
						printf("RECORD CANNOT BE DELETED NOR UPDATED");
						return -1;
					}
					monies += price_s;
				}
			}else{
				printf("FAILED TO PARSE FILE");
				return  -1;
			}
		}
		fclose(pf);
	}
	
	if(oflag == 1){
		FILE *pf;
		if(access(filename_o, F_OK) != -1){
			char* yes = "yes";
			char* y = "y";
			char* no = "no";
			char* n = "n";
			
			printf("Do you want to overwrite the file, yes or no?");
			char* input;
			input = (char*)malloc(255);
			scanf("%s", input);

			if(strcmp(input, no) == 0){
				printf("FILE EXISTS");
				return -1;
			}
			if(strcmp(input, n) == 0){
				printf("FILE EXISTS");
				return -1;
			}
			if(strcmp(input, yes) == 0 || strcmp(input, y) == 0){
				pf = fopen(filename_o, "w");
			}
		}else{
			pf = fopen(filename_o, "w");
		}

		if(vflag == 1){
			struct art_pieces *temp = head;
			while(temp != NULL){
				fprintf(pf, "%d %s %s %s %d\n", temp->data.id_num, 
								temp->data.art_type,
								temp->data.art_name, 
								temp->data.artist, 
								temp->data.price);
				temp = temp->next;
			}
			fprintf(pf, "Budget: %d\n", monies);
			int sum = sum_of_collection(head);
			fprintf(pf, "Worth of Collection: %d\n", sum);
			fclose(pf);
			freeList(head);
			return 0;
		}
		else{
			f_args f;
			f = newArg(iflag, tflag, nflag, id, type, artist_name);	

			struct art_pieces *temp = head;
			int count = 0;

			if(f.iflag + f.tflag + f.nflag == 3){
				while(temp != NULL){
					if(temp->data.id_num == f.idArg && 
					   strcmp(temp->data.art_type, f.typeArg)==0 && 
					   strcmp(temp->data.artist, f.artistArg)==0){
						fprintf(pf, "%d %s %s %s %d\n", temp->data.id_num, 
									   	temp->data.art_type,
					       		   	           	temp->data.art_name,										    temp->data.artist,
					       		                  	temp->data.price);
						count++;
					}
					temp=temp->next;
				}
			}
			else if(f.iflag + f.tflag == 2){
				while(temp != NULL){
					if(temp->data.id_num == f.idArg && 
					   strcmp(temp->data.art_type, f.typeArg)==0){
						fprintf(pf, "%d %s %s %s %d\n", temp->data.id_num, 
									    	temp->data.art_type,
										temp->data.art_name,
										temp->data.artist,
					       		  		    	temp->data.price);
						count++;
					}
					temp=temp->next;
				}	
			}
			else if(f.iflag + f.nflag == 2){
				while(temp != NULL){
					if(temp->data.id_num == f.idArg && 
					   strcmp(temp->data.artist, f.artistArg)==0){
						fprintf(pf, "%d %s %s %s %d\n", temp->data.id_num, 
										temp->data.art_type,
					       		   			temp->data.art_name,
										temp->data.artist,
					       		   			temp->data.price);
						count++;
					}
					temp=temp->next;
				}
			}
			else if(f.tflag + f.nflag == 2){
				while(temp != NULL){
					if(strcmp(temp->data.art_type, f.typeArg)==0 && 
					   strcmp(temp->data.artist, f.artistArg)==0){
						fprintf(pf, "%d %s %s %s %d\n", temp->data.id_num, 
										temp->data.art_type,
										temp->data.art_name, 										    temp->data.artist,
					       		   			temp->data.price);
						count++;
					}
					temp=temp->next;
				}
			}
			else if(f.iflag == 1){
				while(temp != NULL){
					if(temp->data.id_num == f.idArg){
						fprintf(pf, "%d %s %s %s %d\n", temp->data.id_num, 
										temp->data.art_type,
					       		   			temp->data.art_name,
										temp->data.artist,
					       		   			temp->data.price);
						count++;
					}
					temp=temp->next;
				}
			}
			else if(f.tflag == 1){
				while(temp != NULL){
					if(strcmp(temp->data.art_type, f.typeArg)==0){
						fprintf(pf, "%d %s %s %s %d\n", temp->data.id_num, 
										temp->data.art_type,
					       		   			temp->data.art_name,
										temp->data.artist,
					    		   		   	temp->data.price);
						count++;
					}
					temp=temp->next;
				}
			}
			else if(f.nflag == 1){
				while(temp != NULL){
					if(strcmp(temp->data.artist, f.artistArg)==0){
						fprintf(pf, "%d %s %s %s %d\n", temp->data.id_num, 
										temp->data.art_type,
					       		   			temp->data.art_name,
										temp->data.artist,
					       		   			temp->data.price);
						count++;
					}
					temp=temp->next;
				}
			}
			if(count == 0){
				printf("RECORD NOT FOUND");
				return -1;
			}

			fprintf(pf, "Budget: %d\n", monies);
			int sum = sum_of_collection(head);
			fprintf(pf, "Worth of Collection: %d\n", sum);
			
			fclose(pf);
			freeArg(f);
			freeList(head);
			return 0;
		}
	}else{
		if(vflag == 1){
			printList(head);
			
			printf("Budget: %d\n", monies);
			int sum = sum_of_collection(head);
			printf("Worth of Collection: %d\n", sum);
				
			freeList(head);
			return 0;
		}else{
			f_args f;
			f = newArg(iflag, tflag, nflag, id, type, artist_name);
			int found = filter_by_flag(head, f);
			if(found == 0){
				printf("RECORD NOT FOUND");
				return -1;
			}
			
			printf("Budget: %d\n", monies);
			int sum = sum_of_collection(head);
			printf("Worth of Collection: %d\n", sum);
			
			freeArg(f);
			freeList(head);
			return 0;
		}
	}
	return 0;
}
