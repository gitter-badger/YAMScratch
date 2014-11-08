#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Car {
	char *name;
	int age;
	int weight;
	int speed;
};

struct Car *Car_create(char *name, int age, int weight, int speed)
{
	struct Car *something = malloc(sizeof(struct Car));//make a new size
	assert(something != NULL); // check that malloc returned a pointer to valid memory

	something->name = strdup(name);
	something->age = age;
	something->weight = weight;
	something->speed = speed;

	return something;
}

void Car_destroy(struct Car *blah)
{
	assert(blah != NULL);
	free(blah->name);
	free(blah);
}

void Car_print(struct Car *something)
{
	printf("Name: %s\n", something->name);
	printf("\tAge: %d\n", something->age);
	printf("\tWeight: %d\n", something->weight);
	printf("\tSpeed: %d\n", something->speed);
}

int main(int argc, char *argv[])
{
	struct Car *mine = Car_create("Lambo",1,2000,180);
	struct Car *yours = Car_create("Volvo 850 Turbo", 19,3000, 120);

	printf("My car is at %p\n",mine);
	Car_print(mine);

	printf("Your car is at %p\n", yours);
	Car_print(yours);

	mine->age = 26;
	mine->speed = 20;
	Car_print(mine);

	yours->age = 26;
	yours->speed = 25;
	Car_print(yours);

	Car_destroy(mine);
	Car_destroy(yours);

	return 0;
}
