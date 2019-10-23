#include "defs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void cse320_print(char* message) {
	printf("%s", message);
}

void cse320_printf(const char *fmt, ...) { 
	int size = 0; 
	char *p = NULL; 
	va_list ap; 
	
	/* Determine required size */ 
	va_start(ap, fmt); 
	size = vsnprintf(p, size, fmt, ap); 
	va_end(ap); 
	if (size < 0) 
		return; 
		
	size++; /* for '\0' */ 
	p = malloc(size * sizeof(*p)); 
	if (!p) 
		return; 
		
	va_start(ap, fmt); 
	size = vsnprintf(p, size, fmt, ap); 
	va_end(ap); 
	
	if (size < 0) { 
		free(p); 
		return; 
	} 
	
	cse320_print(p); 
	free(p); 
}
