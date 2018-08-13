/* This program sums the numbers from 0 to n
   to validate the result the ... formulae is
   used: ... */

#define FUNCTION_NAME "constructed"
#define FUNCTION_RETURN_TYPE jit_type_void
#define FUNCTION_ARGC 1
#define FUNCTION_ARG_TYPES jit_type_void_ptr
#include "../main.c"

#define WIDTH 120
#define HEIGHT 40

void build(jit_function_t func, int argc, char **argv)
{
	jit_int iterations;
	if(argc >= 2)
		iterations = strtoll(argv[1], NULL, 0);
	else
		iterations = 1000;

	jit_value_t output = jit_value_get_param(func, 0);

	jit_value_t px = jit_value_create(func, jit_type_int);
	jit_value_t py = jit_value_create(func, jit_type_int);

	jit_value_t condition;

	jit_label_t xLoop = jit_label_undefined;
	jit_label_t yLoop = jit_label_undefined;

	jit_insn_store(func, py, const(func, int, 0));

	jit_insn_label(func, &yLoop);
	jit_insn_store(func, px, const(func, int, 0));

	jit_insn_label(func, &xLoop);

	jit_value_t x0 = jit_insn_convert(func, px, jit_type_float64, 0);
	x0 = jit_insn_mul(func, x0, constf(func, 3.5));
	x0 = jit_insn_div(func, x0, constf(func, WIDTH));
	x0 = jit_insn_sub(func, x0, constf(func, 2));
	jit_value_t y0 = jit_insn_convert(func, py, jit_type_float64, 0);
	y0 = jit_insn_mul(func, y0, constf(func, 2));
	y0 = jit_insn_div(func, y0, constf(func, HEIGHT));
	y0 = jit_insn_sub(func, y0, constf(func, 1));

	jit_value_t x = jit_value_create(func, jit_type_float64);
	jit_insn_store(func, x, constf(func, 0));
	jit_value_t y = jit_value_create(func, jit_type_float64);
	jit_insn_store(func, y, constf(func, 0));
	jit_value_t i = jit_value_create(func, jit_type_uint);
	jit_insn_store(func, i, const(func, uint, 0));

	jit_label_t loop = jit_label_undefined;
	jit_label_t done = jit_label_undefined;

	jit_insn_label(func, &loop);

	jit_value_t newX = jit_insn_sub(func,
		jit_insn_mul(func, x, x),
		jit_insn_mul(func, y, y)
	);
	newX = jit_insn_add(func, newX, x0);

	jit_value_t newY = jit_insn_mul(func, x, y);
	newY = jit_insn_mul(func, newY, constf(func, 2));
	newY = jit_insn_add(func, newY, y0);
	jit_insn_store(func, y, newY);

	jit_insn_store(func, x, newX);

	jit_insn_store(func, i, jit_insn_add(func, i, const(func, uint, 1)));

	condition = jit_insn_add(func,
		jit_insn_mul(func, x, x),
		jit_insn_mul(func, y, y)
	);
	condition = jit_insn_lt(func, condition, constf(func, 4));
	jit_insn_branch_if_not(func, condition, &done);

	condition = jit_insn_lt(func, i, const(func, int, iterations));
	jit_insn_branch_if_not(func, condition, &done);

	jit_insn_branch(func, &loop);
	jit_insn_label(func, &done);


	jit_value_t index = jit_insn_mul(func, py, const(func, int, WIDTH));
	index = jit_insn_add(func, index, px);
	jit_insn_store_elem(func, output, index, i);


	jit_insn_store(func, px, jit_insn_add(func, px, const(func, int, 1)));
	condition = jit_insn_lt(func, px, const(func, int, WIDTH));
	jit_insn_branch_if(func, condition, &xLoop);


	jit_insn_store(func, py, jit_insn_add(func, py, const(func, int, 1)));
	condition = jit_insn_lt(func, py, const(func, int, HEIGHT));
	jit_insn_branch_if(func, condition, &yLoop);
}

void run(jit_function_t func, int argc, char **argv)
{
	jit_uint *output = calloc(WIDTH * HEIGHT, sizeof(jit_uint));

	printf("Running...\n");
	void *argptr = &output;
	jit_function_apply(func, &argptr, NULL);

	for(int i = 0; i < WIDTH * HEIGHT; i++)
	{
		jit_uint val = output[i];
		while(val > 9)
			val /= 10;

		if(val <= 1)
			putc(' ', stdout);
		else
			putc('0' + val, stdout);

		if(i % WIDTH == WIDTH - 1)
			putc('\n', stdout);
	}
}
