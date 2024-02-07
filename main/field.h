/*
 *
 *  Copyright (c) 2015, Red Hat, Inc.
 *  Copyright (c) 2015, Masatake YAMATO
 *
 *  Author: Masatake YAMATO <yamato@redhat.com>
 *
 *   This source code is released for free distribution under the terms of the
 *   GNU General Public License version 2 or (at your option) any later version.
 *
 */
#ifndef CTAGS_MAIN_FIELD_H
#define CTAGS_MAIN_FIELD_H

/*
*   INCLUDE FILES
*/

#include "general.h"
#include "types.h"

#include "vstring.h"

/*
*   DATA DECLARATIONS
*/

typedef enum eFieldType { /* extension field content control */
	FIELD_UNKNOWN = -1,

	/* BASIC FIELDS */
	FIELD_NAME,
	FIELD_INPUT_FILE,
	FIELD_PATTERN,

	FIELD_ECTAGS_START,
	FIELD_COMPACT_INPUT_LINE = FIELD_ECTAGS_START,

	/* EXTENSION FIELDS */
	FIELD_JSON_LOOP_START,
	FIELD_FILE_SCOPE = FIELD_JSON_LOOP_START,
	FIELD_KIND_LONG,
	FIELD_KIND,
	FIELD_LANGUAGE,
	FIELD_LINE_NUMBER,
	FIELD_SCOPE,
	FIELD_TYPE_REF,
	FIELD_KIND_KEY,
	FIELD_ECTAGS_LOOP_START,
	FIELD_INHERITANCE = FIELD_ECTAGS_LOOP_START,
	FIELD_ACCESS,
	FIELD_IMPLEMENTATION,
	FIELD_SIGNATURE,
	FIELD_ECTAGS_LOOP_LAST = FIELD_SIGNATURE,

	/* Extension fields newly introduced in Universal Ctags. */
	FIELDS_UCTAGS_START,
	FIELD_REF_MARK = FIELDS_UCTAGS_START,
	FIELD_SCOPE_KEY,
	FIELD_SCOPE_KIND_LONG,
	FIELD_UCTAGS_LOOP_START,
	FIELD_ROLES = FIELD_UCTAGS_LOOP_START,
	FIELD_EXTRAS,
	FIELD_XPATH,
	FIELD_END_LINE,
	FIELD_EPOCH,
	FIELD_NTH,

	FIELD_BUILTIN_LAST = FIELD_NTH,
} fieldType ;

#define fieldDataTypeFlags "sib" /* used in --list-fields */
typedef enum eFieldDataType {
	FIELDTYPE_STRING  = 1 << 0,
	FIELDTYPE_INTEGER = 1 << 1,
	FIELDTYPE_BOOL    = 1 << 2,

	/* used in --list-fields */
	FIELDTYPE_END_MARKER = 1 << 3,
} fieldDataType;
/* Interpretation of VALUE of field
 *
 * With attachParserField() declared in entry.h, you can set a C string
 * as a VALUE for the specified field.
 *
 * The VALUE is interpreted very differently depending on the output
 * format: ctags, xref, and json. See field.h.
 *
 * For FIELDTYPE_STRING:
 * Both json writer and xref writer print it as-is.
 *
 * For FIELDTYPE_STRING|FIELDTYPE_BOOL:
 * If VALUE points "" (empty C string), the json writer prints it as
 * false, and the xref writer prints it as -.
 * If VALUE points a non-empty C string, Both json writer and xref
 * writers print it as-is.
 *
 * For FIELDTYPE_BOOL
 * The json writer always prints true.
 * The xref writer always prints the name of field.
 * Set "" explicitly though the value pointed by VALUE is not referred,
 *
 * The other data type and the combination of types are not implemented yet.
 */

typedef const char* (*fieldRenderer)(const tagEntryInfo *const,
									 const char *,
									 vString *);

struct sFieldDefinition {
	/* letter, and ftype are initialized in the main part,
	   not in a parser. */
#define NUL_FIELD_LETTER '\0'
	unsigned char letter;
	const char* name;
	const char* description;
	bool enabled;

	fieldRenderer render;
	fieldRenderer renderNoEscaping;
	bool (* doesContainAnyChar) (const tagEntryInfo *const, const char*, const char *);

	bool (* isValueAvailable) (const tagEntryInfo *const, const fieldDefinition *);

	const char * getterValueType;
	struct _EsObject * (* getValueObject) (const tagEntryInfo *, const fieldDefinition *);
	const char * setterValueType;

	/* Return es_false if passed value is acceptable.
	   Return an error object is unacceptable. */
	struct _EsObject * (* checkValueForSetter) (const fieldDefinition *, const struct _EsObject *);
	struct _EsObject * (* setValueObject) (tagEntryInfo *, const fieldDefinition *, const struct _EsObject *);

	fieldDataType dataType; /* used in json output */

	unsigned int ftype;	/* Given from the main part */
};


/*
*   FUNCTION PROTOTYPES
*/

extern bool isFieldEnabled (fieldType type);

#endif	/* CTAGS_MAIN_FIELD_H */
