/* This program sums the numbers from 0 to n
   to validate the result the ... formulae is
   used: ... */

#define FUNCTION_NAME "constructed"
#define FUNCTION_RETURN_TYPE jit_type_ulong
#define FUNCTION_RETURN_CTYPE jit_ulong
#define FUNCTION_RETURN_PRINT "%llu"
#define FUNCTION_ARGC 0
#include "../main.c"

void build(jit_function_t func, int argc, char **argv)
{
	jit_value_t conditions[10];

	jit_ulong *mem = malloc(sizeof(jit_ulong));
	if(argc >= 2)
		*mem = strtoll(argv[1], NULL, 0);
	else
		*mem = 0x99;

	jit_ulong iterations;
	if(argc >= 3)
		iterations = strtoll(argv[2], NULL, 0);
	else
		iterations = 1000;

	jit_value_t ptr = const(func, void_ptr, (jit_nuint)mem);
	jit_value_t bitset = jit_insn_load_relative(func, ptr, 0, jit_type_ulong);

	jit_value_t sum = jit_value_create(func, jit_type_ulong);
	jit_insn_store(func, sum, constl(func, ulong, 0));

	jit_value_t iterator = jit_value_create(func, jit_type_ulong);
	jit_insn_store(func, iterator, constl(func, ulong, iterations));

	jit_label_t loopEnd = jit_label_undefined;
	jit_label_t loop = jit_label_undefined;
	jit_insn_label(func, &loop);

	jit_value_t loopCondition = jit_insn_eq(func, iterator, constl(func, ulong, 0));
	jit_insn_branch_if(func, loopCondition, &loopEnd);
	jit_insn_store(func, iterator, jit_insn_sub(func, iterator, constl(func, ulong, 1)));

	for(int i = 0; i < sizeof(conditions) / sizeof(jit_value_t); i++)
	{
		jit_value_t mask = constl(func, ulong, (jit_ulong)1 << i);
		conditions[i] = jit_insn_and(func, bitset, mask);
	}

	for(int i = 0; i < sizeof(conditions) / sizeof(jit_value_t); i++)
	{
		jit_label_t after = jit_label_undefined;
		jit_insn_branch_if_not(func, conditions[i], &after);

		jit_value_t val = constl(func, ulong, i + 1);
		jit_insn_store(func, sum, jit_insn_add(func, sum, val));

		jit_insn_label(func, &after);
	}

	jit_insn_branch(func, &loop);
	jit_insn_label(func, &loopEnd);

	jit_insn_return(func, sum);
}