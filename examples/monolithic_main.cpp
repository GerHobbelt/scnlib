
#define BUILD_MONOLITHIC 1
#include "monolithic_examples.h"

#define USAGE_NAME   "scanf_demo"

#include "monolithic_main_internal_defs.h"

MONOLITHIC_CMD_TABLE_START()
	{ "ex1", {.fa = scanf_example_01_main } },
	{ "ex2", {.fa = scanf_example_02_main } },
	{ "ex3", {.fa = scanf_example_03_main } },
	{ "getline", {.fa = scanf_getline_example_main } },
	{ "json", {.fa = scanf_json_example_main } },
	{ "positional", {.fa = scanf_positional_example_main } },
	{ "readme", {.fa = scanf_readme_example_main } },
	{ "tuple", {.fa = scanf_tuple_example_main } },
MONOLITHIC_CMD_TABLE_END();

#include "monolithic_main_tpl.h"
