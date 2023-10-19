#include <stdio.h>
#include <stdlib.h>
#include "monty.h"

/**
 * push - Pushes an element onto the stack
 * @stack: A pointer to the stack
 * @line_number: The current line number in the bytecode file
 * @value: The integer value to push onto the stack
 */
void push(stack_t **stack, unsigned int line_number, int value)
{
	stack_t *new_node = malloc(sizeof(stack_t));

	if (!new_node)
	{
		fprintf(stderr, "Error: malloc failed\n");
		exit(EXIT_FAILURE);
	}

	new_node->n = value;
	new_node->prev = NULL;
	new_node->next = *stack;

	if (*stack)
		(*stack)->prev = new_node;
	*stack = new_node;
}

/**
 * pall - Prints all values on the stack
 * @stack: A pointer to the stack
 * @line_number: The current line number in the bytecode file
 */
void pall(stack_t **stack, unsigned int line_number)
{
	stack_t *current = *stack;

	(void)line_number;

	while (current)
	{
		printf("%d\n", current->n);
		current = current->next;
	}
}
/**
  * main - entry point
  * @argc: number of command-line arguments
  * @argv: an array of command-line arguments
  * Return: return value is 0
  **/
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "USAGE: monty file\n");
		return (EXIT_FAILURE);
	}

	FILE *file = fopen(argv[1], "r");

	if (!file)
	{
		fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
		return (EXIT_FAILURE);
	}

	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	stack_t *stack = NULL;

	unsigned int line_number = 0;

	while ((read = getline(&line, &len, file)) != -1)
	{
		line_number++;
		char *opcode, *arg;
		int value = 0;

		opcode = strtok(line, " \t\n");
		if (!opcode || opcode[0] == '#')
			continue;

		if (strcmp(opcode, "push") == 0)
		{
			arg = strtok(NULL, " \t\n");
			if (!arg)
			{
				fprintf(stderr, "L%d: usage: push integer\n", line_number);
				free(line);
				fclose(file);
				free_stack(stack);
				return (EXIT_FAILURE);
			}

			value = atoi(arg);
			if (value == 0 && arg[0] != '0')
			{
				fprintf(stderr, "L%d: usage: push integer\n", line_number);
				free(line);
				fclose(file);
				free_stack(stack);
				return (EXIT_FAILURE);
			}

			push(&stack, line_number, value);
		}
		else if (strcmp(opcode, "pall") == 0)
		{
			pall(&stack, line_number);
		}
		else
		{
			fprintf(stderr, "L%d: unknown instruction %s\n", line_number, opcode);
			free(line);
			fclose(file);
			free_stack(stack);
			return (EXIT_FAILURE);
		}
	}

	free(line);
	fclose(file);
	free_stack(stack);

	return (EXIT_SUCCESS);
}

