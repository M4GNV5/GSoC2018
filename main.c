/* Do not compile this file directly.
   It will be #include-ed by one of the files in programs/ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void jit_optimize(jit_function_t func);

#define const(func, type, val) (jit_value_create_nint_constant(func, jit_type_##type, val))
#define constl(func, type, val) (jit_value_create_long_constant(func, jit_type_##type, val))
#define constf(func, val) (jit_value_create_float64_constant(func, jit_type_float64, val))

void build(jit_function_t func, int argc, char **argv);
void run(jit_function_t func, int argc, char **argv);

int main(int argc, char **argv)
{
	jit_context_t ctx = jit_context_create();
	jit_context_build_start(ctx);

#ifdef FUNCTION_ENABLE_DUMP
	char *dump = getenv("DUMP");
#endif

#if FUNCTION_ARGC == 0
	jit_type_t signature = jit_type_create_signature(jit_abi_cdecl,
		FUNCTION_RETURN_TYPE, NULL, 0, 1);
#else 
	jit_type_t args[FUNCTION_ARGC] = {FUNCTION_ARG_TYPES};
	jit_type_t signature = jit_type_create_signature(jit_abi_cdecl,
		FUNCTION_RETURN_TYPE, args, FUNCTION_ARGC, 1);
#endif

	jit_function_t func = jit_function_create(ctx, signature);
	jit_type_free(signature);

	build(func, argc, argv);

	jit_optimize(func);

#ifdef FUNCTION_ENABLE_DUMP
#ifdef FUNCTION_HAS_LIVE_RANGES
	if(dump != NULL && strstr(dump, "liveranges") != NULL)
		jit_dump_live_ranges(stdout, func);
#endif

	if(dump != NULL && strstr(dump, "jit") != NULL)
		jit_dump_function(stdout, func, FUNCTION_NAME);
#endif

	jit_function_compile(func);
	jit_context_build_end(ctx);

#ifdef FUNCTION_ENABLE_DUMP
	if(dump != NULL && strstr(dump, "asm") != NULL)
		jit_dump_function(stdout, func, FUNCTION_NAME);

	if(dump != NULL && strstr(dump, "noexec") != NULL)
		return 0;
#endif

#if FUNCTION_ARGC == 0
	FUNCTION_RETURN_CTYPE result;
	jit_function_apply(func, NULL, &result);
	printf(FUNCTION_NAME "() = " FUNCTION_RETURN_PRINT "\n", result);
#else
	run(func, argc, argv);
#endif

	return 0;
}
