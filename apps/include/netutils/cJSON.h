/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * apps/netutils/json/cJSON.c
 *
 * This file is a part of NuttX:
 *
 *   Copyright (C) 2011 Gregory Nutt. All rights reserved.
 *   Ported by: Darcy Gong
 *
 * And derives from the cJSON Project which has an MIT license:
 *
 *   Copyright (c) 2009 Dave Gamble
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 ****************************************************************************/

#ifndef __APPS_INCLUDE_NETUTILS_JSON_H
#define __APPS_INCLUDE_NETUTILS_JSON_H

#ifdef __cplusplus
// *INDENT-OFF*
extern "C"
{
// *INDENT-ON*
#endif

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define cJSON_False  0
#define cJSON_True   1
#define cJSON_NULL   2
#define cJSON_Number 3
#define cJSON_String 4
#define cJSON_Array  5
#define cJSON_Object 6

#define cJSON_IsReference 256

#define cJSON_AddNullToObject(object, name) \
	cJSON_AddItemToObject(object, name, cJSON_CreateNull())
#define cJSON_AddTrueToObject(object, name) \
	cJSON_AddItemToObject(object, name, cJSON_CreateTrue())
#define cJSON_AddFalseToObject(object, name) \
	cJSON_AddItemToObject(object, name, cJSON_CreateFalse())
#define cJSON_AddBoolToObject(object, name, b) \
	cJSON_AddItemToObject(object, name, cJSON_CreateBool(b))
#define cJSON_AddNumberToObject(object, name, n) \
	cJSON_AddItemToObject(object, name, cJSON_CreateNumber(n))
#define cJSON_AddStringToObject(object, name, s) \
	cJSON_AddItemToObject(object, name, cJSON_CreateString(s))

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* The cJSON structure: */

typedef struct cJSON {
	/* next/prev allow you to walk array/object chains. Alternatively, use
	 * GetArraySize/GetArrayItem/GetObjectItem
	 */

	struct cJSON *next, *prev;

	/* An array or object item will have a child pointer pointing to a chain
	 * of the items in the array/object.
	 */

	struct cJSON *child;

	int type;				/* The type of the item, as above. */
	char *valuestring;		/* The item's string, if type==cJSON_String */
	int valueint;			/* The item's number, if type==cJSON_Number */
	double valuedouble;		/* The item's number, if type==cJSON_Number */

	/* The item's name string, if this item is the child of, or is in the list
	 * of subitems of an object.
	 */

	char *string;
} cJSON;

typedef struct cJSON_Hooks {
	void *(*malloc_fn)(size_t sz);
	void (*free_fn)(void *ptr);
} cJSON_Hooks;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/* Supply malloc, realloc and free functions to cJSON */

void cJSON_InitHooks(cJSON_Hooks *hooks);

/* Supply a block of JSON, and this returns a cJSON object you can
 * interrogate. Call cJSON_Delete when finished.
 */

cJSON *cJSON_Parse(const char *value);

/* Render a cJSON entity to text for transfer/storage. Free the char* when
 * finished.
 */

char *cJSON_Print(cJSON *item);

/* Render a cJSON entity to text for transfer/storage without any
 * formatting. Free the char* when finished.
 */

char *cJSON_PrintUnformatted(cJSON *item);

/* Delete a cJSON entity and all subentities. */

void cJSON_Delete(cJSON *c);

/* Returns the number of items in an array (or object). */

int cJSON_GetArraySize(cJSON *array);

/* Retrieve item number "item" from array "array". Returns NULL if
 * unsuccessful.
 */

cJSON *cJSON_GetArrayItem(cJSON *array, int item);

/* Get item "string" from object. Case insensitive. */

cJSON *cJSON_GetObjectItem(cJSON *object, const char *string);

/* For analysing failed parses. This returns a pointer to the parse error.
 * You'll probably need to look a few chars back to make sense of it.
 * Defined when cJSON_Parse() returns 0. 0 when cJSON_Parse() succeeds.
 */

const char *cJSON_GetErrorPtr(void);

/* These calls create a cJSON item of the appropriate type. */

cJSON *cJSON_CreateNull(void);
cJSON *cJSON_CreateTrue(void);
cJSON *cJSON_CreateFalse(void);
cJSON *cJSON_CreateBool(int b);
cJSON *cJSON_CreateNumber(double num);
cJSON *cJSON_CreateString(const char *string);
cJSON *cJSON_CreateArray(void);
cJSON *cJSON_CreateObject(void);

/* These utilities create an Array of count items. */

cJSON *cJSON_CreateIntArray(const int *numbers, int count);
cJSON *cJSON_CreateFloatArray(const float *numbers, int count);
cJSON *cJSON_CreateDoubleArray(const double *numbers, int count);
cJSON *cJSON_CreateStringArray(const char **strings, int count);

/* Append item to the specified array/object. */

void cJSON_AddItemToArray(cJSON *array, cJSON *item);
void cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item);

/* Append reference to item to the specified array/object. Use this when you
 * want to add an existing cJSON to a new cJSON, but don't want to corrupt
 * your existing cJSON.
 */

void cJSON_AddItemReferenceToArray(cJSON *array, cJSON *item);
void cJSON_AddItemReferenceToObject(cJSON *object, const char *string, cJSON *item);

/* Remove/Detatch items from Arrays/Objects. */

cJSON *cJSON_DetachItemFromArray(cJSON *array, int which);
void cJSON_DeleteItemFromArray(cJSON *array, int which);
cJSON *cJSON_DetachItemFromObject(cJSON *object, const char *string);
void cJSON_DeleteItemFromObject(cJSON *object, const char *string);

/* Update array items. */

void cJSON_ReplaceItemInArray(cJSON *array, int which, cJSON *newitem);
void cJSON_ReplaceItemInObject(cJSON *object, const char *string, cJSON *newitem);

#ifdef __cplusplus
// *INDENT-OFF*
}
// *INDENT-ON*
#endif
#endif							/* __APPS_INCLUDE_NETUTILS_JSON_H */
