#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "stack.h"

#define DUMP_HEAD(var, ok) fprintf(stderr, "stack_t \"%s\" (%s) [%p]\n", #var, ok, &var);
#define DUMP_DATA(size, data) fprintf(stderr, "    data[%d] = %p\n", (size), (data));
#define DUMP_COUNT(count) fprintf(stderr, "    count = %d\n", (count));
#define ERR_MSG(msg) fprintf(stderr, "!ERROR! in %s function: %s\n", __FUNCTION__, msg);

#define BAD_VAL (-666666)

/*
typedef struct {
    int count;
    int *data;
    int size;
} stack_t;
*/

int swar(const stack_t stk)
{
	int i = 0;
	int j = 0;
	int hash = 0;
	
	for (j = 0; j <= stk.size-1; j++) {
		i = stk.data[j];
		i = i - ((i >> 1) & 0x55555555);
		i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
		hash += (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
	}
	
	return hash;
}

int stack_ctor(stack_t *stk, int n_size)
{
    stk->data = (int *)calloc(n_size, sizeof(int));
    if (stk->data == NULL) {
        #ifdef DEBUG
            ERR_MSG("calloc error");
            exit(EXIT_FAILURE);
        #endif
        
        return -1;
    }
    
    stk->count = 0;
    stk->size = n_size;
    stk->hash = 0;
    
    return 0;
}


int stack_dtor(stack_t *stk)
{
    if ( !(stack_ok(*stk)) ) {
        #ifdef DEBUG
            ERR_MSG("");
            exit(EXIT_FAILURE);
        #endif
        
        return -1;
    }
    
    stk->count = -1;
    free(stk->data);
    stk->data = NULL;
    stk->size = -1;
    stk->hash = -1;
    
    return 0;
}


int stack_ok(const stack_t stk) 
{
    int ret = ( (stk.data) && (stk.count >= 0) && (stk.size > 0) && (stk.count <= stk.size) && (swar(stk) == stk.hash));
    
    if (!ret) {
         #ifdef DEBUG
            ERR_MSG("stack is not OK");
         
            if(stk.data == NULL) {
                ERR_MSG("stack data is NULL");
            }
            
            if(stk.count < 0) {
                ERR_MSG("stack count < 0");
            }
            
            if(stk.size <= 0) {
                ERR_MSG("stack size <= 0");
            }
            
            if(stk.count > stk.size) {
                ERR_MSG("stack count <> size");
            }
            
            if(swar(stk.data) != stk.hash) {
				ERR_MSG("stack hash error");
			}
            
            exit(EXIT_FAILURE);
        #endif
        
        errno = EPROTO;
    }
    
    return ret;
}


int stack_push(stack_t *stk, const int val) 
{
    if ( !(stack_ok(*stk)) ) {
        #ifdef DEBUG
            ERR_MSG("");
            exit(EXIT_FAILURE);
        #endif
        
        return -1;
    }
    
    if (stk->count >= stk->size) {
        #ifdef DEBUG
            ERR_MSG("stack.count >= stack.size");
            exit(EXIT_FAILURE);
        #endif
        
        return -2;
    }

    stk->data[stk->count] = val;
    stk->count++;
    stk->hash = swar(*stk);
    
    return 0;
}


int stack_pop(stack_t *stk, int *val) 
{
    if ( !(stack_ok(*stk)) ) {
        #ifdef DEBUG
            ERR_MSG("");
            exit(EXIT_FAILURE);
        #endif
        
        return -1;
    }
    
    if (val == NULL) {
        #ifdef DEBUG
            ERR_MSG("val is NULL");
            exit(EXIT_FAILURE);
        #endif
        
        return -2;
    }
    
    if (stk->count <= 0) {
        #ifdef DEBUG
            ERR_MSG("nothing to pop");
            exit(EXIT_FAILURE);
        #endif
        
        return -3;
    }
    
    *val = stk->data[stk->count-1];
    stk->data[stk->count-1] = BAD_VAL;
    stk->count--;
    stk->hash = swar(*stk);
    
    return 0;
}


int stack_dump(const stack_t stk) 
{   
    int i = 0;
    
    if ( !(stack_ok(stk)) ) {
        if (stk.data == NULL) {
            DUMP_HEAD(stk, "data error");
        
            return 1;
        }
        
        if ((stk.count < 0) && (stk.size <= 0)) {
            DUMP_HEAD(stk, "count AND size error");
            fprintf(stderr, "{\n");
                DUMP_DATA(stk.size, stk.data);
                fprintf(stderr, "    {\n");
                fprintf(stderr, "    }\n");
                DUMP_COUNT(stk.count);
            fprintf(stderr, "}\n");
            
            return 1;
        }
        
        if ((stk.count < stk.size) && (stk.size > 0)) {
            DUMP_HEAD(stk, "count error");
            fprintf(stderr, "{\n");
                DUMP_DATA(stk.size, stk.data);
                fprintf(stderr, "    {\n");
                for (i = 0; i <= stk.size-1; i++) {
                    if (stk.data[i] == BAD_VAL) {
                        fprintf(stderr, "         [%d] = %d POISON!\n", i, stk.data[i]);
                    }
                    else {
                        fprintf(stderr, "         [%d] = %d\n", i, stk.data[i]);
                    }
                }
                fprintf(stderr, "    }\n");
                DUMP_COUNT(stk.count);
            fprintf(stderr, "}\n");
            
            return 1;
        }
        
        if ((stk.size <= 0)) {
            DUMP_HEAD(stk, "size error");
            fprintf(stderr, "{\n");
                DUMP_DATA(stk.size, stk.data);
                fprintf(stderr, "    {\n");
                fprintf(stderr, "    }\n");
                DUMP_COUNT(stk.count);
            fprintf(stderr, "}\n");
            
            return 1;
        }
    }
    

    
    DUMP_HEAD(stk, "ok");
    fprintf(stderr, "{\n");
        DUMP_DATA(stk.size, stk.data);
            printf("    {\n");
            //printf("		hash = %d, swar(stk.data) = %d\n", stk.hash,
            for (i = 0; i <= stk.count-1; i++) {
                if (stk.data[i] == BAD_VAL) {
                    fprintf(stderr, "         [%d] = %d POISON!\n", i, stk.data[i]);
                }
                else {
                    fprintf(stderr, "        *[%d] = %d\n", i, stk.data[i]);
                }
            }
            for (i = stk.count; i <= stk.size-1; i++) {
                if(stk.data[i] == BAD_VAL) {
                    fprintf(stderr, "         [%d] = %d POISON!\n", i, stk.data[i]);
                }
                else {
                    fprintf(stderr, "         [%d] = %d\n", i, stk.data[i]);
                }
            }
            fprintf(stderr, "    }\n");
            DUMP_COUNT(stk.count);
    fprintf(stderr, "}\n");
    
    return 0;
}
