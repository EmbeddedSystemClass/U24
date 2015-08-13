/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define _FILE_OFFSET_BITS 64
#define _LARGEFILE64_SOURCE 1
#define _GNU_SOURCE

#include <fcntl.h>
#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "unistd.h"
#include "sparse.h"


#ifndef O_BINARY
#define O_BINARY 0
#endif

void usage()
{
  fprintf(stderr, "Usage: simg2simg <sparse image file> <sparse_image_file> <max_size>\n");
}
/*
int main(int argc, char *argv[])
{
    int fd;
    struct sparse_file *s;
    int files;
    struct sparse_file **out_s;
	int64_t max_size;
	int64_t sz64;
	int nTmp = 0 ;
	struct sparse_file **s1 ;

	max_size = atol(argv[3]);
    fd = open(argv[1], O_RDONLY | O_BINARY);
    if (fd < 0) {
		fprintf(stderr, "Cannot open input file %s\n", argv[1]);
    }else
	{
		fprintf(stderr, "open  file ok %s\n", argv[1]);
	}

    s = sparse_file_import_auto(fd, false);
    if (!s) {
		fprintf(stderr, "cannot sparse read file '%s'\n", argv[1]);
	}else{
		fprintf(stderr, "sparse_file_import_auto s =  '%d' \n", s);
	}
	
    files = sparse_file_resparse(s, max_size, NULL, 0);
    if (files < 0) {
		fprintf(stderr, "Failed to resparse '%s'\n", argv[1]);
    }

    out_s = calloc(sizeof(struct sparse_file *), files + 1);
    if (!out_s) {
		fprintf(stderr,"Failed to allocate sparse file array\n",  argv[1]);
    }

    files = sparse_file_resparse(s, max_size, out_s, files);
    if (files < 0) {
		fprintf(stderr,"Failed to resparse '%s'\n", argv[1]);
    }
	fprintf(stderr, "Successful '%lu' \n", out_s);
	fprintf(stderr, "Successful '%d' \n", out_s);
	close(fd);

	s1  =out_s;
	if (s1 == NULL) {
		fprintf(stderr, "cannot sparse load  '%d' \n", out_s);
	}
	else{
		sz64 = sparse_file_len(*s1++, true, false);
		fprintf(stderr, "[%d] *s1 '%lld'  \n",nTmp, sz64);
		//sz64 = sparse_file_len(*s1, true, false);
		//fprintf(stderr, "[%d] *s1 '%d' \n",nTmp, sz64);
			exit(0);
		while ( *s1 ) {
			fprintf(stderr, "[%d] *s1 '%d' \n",nTmp, *s1);
			sz64 = sparse_file_len(*s1, true, false);
			*s1++;
			fprintf(stderr, "[%d] sparse_file_len '%d' \n",nTmp, sz64);
			if (nTmp++ >10 )break;
		}	
	}
	fprintf(stderr, "Successful\n", out_s);
	exit(0);
}
*/
/*
struct sparse_file **load_sparse_files(const char *fname, int max_size)
{
    int fd;
    struct sparse_file *s;
    int files;
    struct sparse_file **out_s;

	fprintf(stderr, "max_size '%d'\n", max_size);//lion

    fd = open(fname, O_RDONLY | O_BINARY);
    if (fd < 0) {
        fprintf(stderr, "cannot open '%s'\n", fname);
    }

    s = sparse_file_import_auto(fd, false);
    if (!s) {
        fprintf(stderr, "cannot sparse read file '%s'\n", fname);
    }

    files = sparse_file_resparse(s, max_size, NULL, 0);
    if (files < 0) {
        fprintf(stderr, "Failed to resparse '%s'\n", fname);
    }

    out_s = calloc(sizeof(struct sparse_file *), files + 1);
    if (!out_s) {
        fprintf(stderr, "Failed to allocate sparse file array\n");
    }

    files = sparse_file_resparse(s, max_size, out_s, files);
    if (files < 0) {
       fprintf(stderr, "Failed to resparse '%s'\n", fname);
    }

    return out_s;
}*/
int main(int argc, char *argv[])
{
   int64_t sz64;
    void *data;
    int64_t limit;
	int nTmp = 0 ;

    sz64 = 549755813888;
	limit = argv[2];

	fprintf(stderr, "limit = ['%s'] \n",limit);
	fprintf(stderr, "limit = ['%lu'] \n",(unsigned)atol(limit));
    if (limit) {
        struct sparse_file **s = load_sparse_files(argv[1],atoi(limit));
		fprintf(stderr, "limit = ['%d'] main Successful '%lu' \n",limit, s);
        if (s == NULL) {
			fprintf(stderr, "cannot sparse load '%s'\n",argv[1]);
        }
        while (*s) {
            sz64 = sparse_file_len(*s, true, false);
			fprintf(stderr, "[%d] *s1 '%d' \n",nTmp, sz64);
			 *s++;
        }
	}

	exit(0);
}

