****hashmap from https://github.com/petewarden
使用方法:
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "hashmap.h"

#define KEY_MAX_LENGTH (256)
#define KEY_PREFIX ("somekey")
#define KEY_COUNT (1024*1024)

typedef struct data_struct_s
{
    char key_string[KEY_MAX_LENGTH];
    int number;
} data_struct_t;

int main(char* argv, int argc)
{
    int index;
    int error;
    map_t mymap;
    char key_string[KEY_MAX_LENGTH];
    data_struct_t* value;
    
    mymap = hashmap_new();

    /* First, populate the hash map with ascending values */
    for (index=0; index<KEY_COUNT; index+=1)
    {
        /* Store the key string along side the numerical value so we can free it later */
        value = malloc(sizeof(data_struct_t));
        snprintf(value->key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);
        value->number = index;

        error = hashmap_put(mymap, value->key_string, value);
        assert(error==MAP_OK);
    }

    /* Now, check all of the expected values are there */
    for (index=0; index<KEY_COUNT; index+=1)
    {
        snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);

        error = hashmap_get(mymap, key_string, (void**)(&value));
        
        /* Make sure the value was both found and the correct number */
        assert(error==MAP_OK);
        assert(value->number==index);
    }
    
    /* Make sure that a value that wasn't in the map can't be found */
    snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, KEY_COUNT);

    error = hashmap_get(mymap, key_string, (void**)(&value));
        
    /* Make sure the value was not found */
    assert(error==MAP_MISSING);

    /* Free all of the values we allocated and remove them from the map */
    for (index=0; index<KEY_COUNT; index+=1)
    {
        snprintf(key_string, KEY_MAX_LENGTH, "%s%d", KEY_PREFIX, index);

        error = hashmap_get(mymap, key_string, (void**)(&value));
        assert(error==MAP_OK);

        error = hashmap_remove(mymap, key_string);
        assert(error==MAP_OK);

        free(value);
    }
    
    /* Now, destroy the map */
    hashmap_free(mymap);

    return 1;
}


************ btree from  https://github.com/antirez/otree
使用方法
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "btree.h"

#define OP_ALLOC 0
#define OP_FREE 1
#define OP_ALLOCFREE 2
#define OP_ADD 3
#define OP_WALK 4
#define OP_FILL 5
#define OP_FIND 6

int main(int argc, char **argv) {
    struct btree *bt;
    uint64_t ptr;
    int j, count, op, arg;

    if (argc != 4) {
        fprintf(stderr,"Usage: btree_example <op> <size/ptr> <count>\n");
        exit(1);
    }
    count = atoi(argv[3]);
    arg = atoi(argv[2]);
    if (!strcasecmp(argv[1],"alloc")) {
        op = OP_ALLOC;
    } else if (!strcasecmp(argv[1],"free")) {
        op = OP_FREE;
    } else if (!strcasecmp(argv[1],"allocfree")) {
        op = OP_ALLOCFREE;
    } else if (!strcasecmp(argv[1],"add")) {
        op = OP_ADD;
    } else if (!strcasecmp(argv[1],"walk")) {
        op = OP_WALK;
    } else if (!strcasecmp(argv[1],"fill")) {
        op = OP_FILL;
    } else if (!strcasecmp(argv[1],"find")) {
        op = OP_FIND;
    } else {
        printf("not supported op %s\n", argv[1]);
        exit(1);
    }

    bt = btree_open(NULL, "./btree.db", BTREE_CREAT);
    btree_clear_flags(bt,BTREE_FLAG_USE_WRITE_BARRIER);
    if (bt == NULL) {
        perror("btree_open");
        exit(1);
    }
   
    for (j = 0; j < count; j++) {
        if (op == OP_ALLOC) {
            ptr = btree_alloc(bt,arg);
            printf("PTR: %llu\n", ptr);
        } else if (op == OP_FREE) {
            btree_free(bt,arg);
        } else if (op == OP_ALLOCFREE) {
            ptr = btree_alloc(bt,arg);
            printf("PTR: %llu\n", ptr);
            btree_free(bt,ptr);
        }
    }

    if (op == OP_ADD) {
        int retval;
        char key[16];
        memset(key,0,16);
        strcpy(key,argv[2]);

        retval = btree_add(bt,(unsigned char*)key,
            (unsigned char*)argv[3],strlen(argv[3]),1);
        printf("retval %d\n", retval);
        if (retval == -1) {
            printf("Error: %s\n", strerror(errno));
        }
    } else if (op == OP_WALK) {
        btree_walk(bt,bt->rootptr);
    } else if (op == OP_FILL) {
        for (j = 0; j < count; j++) {
            int r = random()%arg;
            int retval;
            char key[64];
            char val[64];

            memset(key,0,64);
            snprintf(key,64,"k%d",r);
            snprintf(val,64,"val:%d",r);
            retval = btree_add(bt,(unsigned char*)key,
                            (unsigned char*)val, strlen(val), 1);
            if (retval == -1) {
                printf("Error: %s\n", strerror(errno));
                goto err;
            }
        }
    } else if (op == OP_FIND) {
        int retval;
        char key[16], *data;
        memset(key,0,16);
        strcpy(key,argv[2]);
        uint64_t voff;
        uint32_t datalen;

        retval = btree_find(bt,(unsigned char*)key,&voff);
        if (retval == -1) {
            if (errno == ENOENT) {
                printf("Key not found\n");
                exit(0);
            } else {
                perror("Error searching for key");
                exit(1);
            }
        }
        printf("Key found at %llu\n", voff);

        btree_alloc_size(bt,&datalen,voff);
        data = malloc(datalen+1);
        btree_pread(bt,(unsigned char*)data,datalen,voff);
        data[datalen] = '\0';
        printf("Value: %s\n", data);
        free(data);
    }
    btree_close(bt);
    return 0;

err:
    btree_close(bt);
    return 1;
}


*********** https://github.com/fmela/libdict  比较全能的样子


