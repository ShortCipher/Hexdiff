#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "range.h"

s_range rangeFromString(char *string) {
	s_range r = {0,0};
	long length = strlen(string);
	if (string == NULL || length == 0) {
		return r;
	}
	long offset = -1;
	for (long i = length - 1; i >= 0; i--) {
		if (string[i] == RANGE_SEPARATOR) {
			offset = i;
			break;
		}
	}
	if (offset <= 0) return r;
/*
printf("string: %s\n", string);
printf("        ");
for (long i = 0; i < offset; i++) {printf(" ");}
printf("^\n");
printf("l=%ld,o=%ld,d=%ld", length, offset, length-1-offset);
*/
	long l;
	char *p;
//printf(" '%c' ",string[offset]);
	//r.start
	l = offset;
	p = (char *)malloc((l + 1) * sizeof(char));
	for (long i = 0; i < l; i++) {
		p[i] = string[i];
	}
	p[l] = 0;
//printf("{%ld,%s-", l, p);
	r.start = atol(p);
	free(p);

	//r.stop
	l = length-1-offset;
	p = (char *)malloc((l + 1) * sizeof(char));
	for (long i = 0; i < l; i++) {
		p[i] = string[offset+1+i];
	}
	p[l] = 0;
//printf("%ld,%s}\n", l, p);
	r.stop = atol(p);
	free(p);

	return r;
}

void sortRanges(long size, s_range *list) {
	if (size <= 0 || list == NULL) {
		return;
	}
	s_range t;
	for (long j = 0; j < size - 1; j++) {
		for (long i = 0; i < size - 1 - j; i++) {
			if (list[i].start > list[i+1].start) {
				t = list[i];
				list[i] = list[i+1];
				list[i+1] = t;
			}
		}
	}
	return;
}

//#define print_range(r) printf("{%ld,%ld}", (r).start, (r).stop)
s_range *consolidateRanges(long *size, s_range *list) {
	if (*size <= 0 || list == NULL) {
		return NULL;
	}
	long c_count = 0;
	s_range *c_ranges;

	c_ranges = (s_range *)malloc(sizeof(s_range));
	c_ranges[c_count] = list[0];

	for (long i = 1; i < *size; i++) {
//print_range(c_ranges[c_count]);printf(" vs.");print_range(list[i]);printf("\n");
		if (c_ranges[c_count].stop >= list[i].start) {
			if (c_ranges[c_count].stop < list[i].stop) {
				c_ranges[c_count].stop = list[i].stop;
			}
		} else {
			c_count++;
			c_ranges = (s_range *)realloc(c_ranges, (c_count + 1) * sizeof(s_range));
			c_ranges[c_count] = list[i];
		}
//printf("~");print_range(c_ranges[c_count]);printf(" vs.");print_range(list[i]);printf("\n");
	}

	*size = c_count + 1;
	return c_ranges;
}

char valueIsWithinRanges(long size, s_range *list, long t) {
	char r = 0;
	if (size <= 0 || list == NULL) {
		r = 1;
	} else {
		for (long i = 0; i < size; i++) {
//printf("(%ld:%ld:%ld)", list[i].start, t, list[i].stop);
			if (t >= list[i].start && t <= list[i].stop) {
				r = 1;
				break;
			}
		}
	}
	return r;
}