/* This program sums the numbers from 0 to n
   to validate the result the ... formulae is
   used: ... */

#define FUNCTION_NAME "sumOfNumbers"
#define FUNCTION_RETURN_TYPE jit_type_ulong
#define FUNCTION_RETURN_CTYPE jit_ulong
#define FUNCTION_RETURN_PRINT "%llu"
#define FUNCTION_ARGC 0
#include "../main.c"

void build(jit_function_t func, int argc, char **argv)
{
	jit_ulong end;
	if(argc >= 2)
		end = strtoll(argv[1], NULL, 0);
	else
		end = 10000;

	jit_value_t sum = jit_value_create(func, jit_type_ulong);
	jit_value_t i = jit_value_create(func, jit_type_ulong);

	jit_insn_store(func, sum, constl(func, ulong, 0));
	jit_insn_store(func, i, constl(func, ulong, 0));

	jit_label_t loop = jit_label_undefined;
	jit_label_t done = jit_label_undefined;

	jit_insn_label(func, &loop);
	jit_value_t condition = jit_insn_lt(func, i, constl(func, ulong, end));
	jit_insn_branch_if_not(func, condition, &done);

	jit_insn_store(func, i, jit_insn_add(func, i, constl(func, ulong, 1)));
	jit_insn_store(func, sum, jit_insn_add(func, sum, i));

	jit_insn_branch(func, &loop);

	jit_insn_label(func, &done);
	jit_insn_return(func, sum);
}
