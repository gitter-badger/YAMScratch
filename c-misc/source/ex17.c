#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Passenger_Data {
	char *name;
	char *next_of_kin;
};

struct Rocket {
	char thrusters; //number of thrusters
	char stages; //number of stages
	char cur_stage;
	int alive; //alive passengers
	int dead; //dead passengers
	int id; //sepcific id of the rocket flight
	struct Passenger_Data manifest[MAX_DATA];
};

struct Database {
	struct Rocket flights[MAX_ROWS];
};

struct Connection {
	FILE *file;
	struct Database *database;
};

void die(const char *message)
{
	if(errno){
		perror(message); //use perror to print out the error of the function
	}
	else{
		printf("ERROR: %s\n", message);
	}
	exit(1);
}

void Rocket_print(struct Rocket *rock)
{
	printf("ID: %d,Thrusters: %d, Stage: %d/%d",rock->id,rock->thrusters,rock->cur_stage,rock->stages);

	printf("\tAlive/Dead: %d/%d", rock->alive,rock->dead);
}

void Database_load(struct Connection *con)
{
	int return_code = fread(con->database,sizeof(struct Database),1,con->file);
	if(return_code != 1) die("Failed to load database");
}

struct Connection *Database_open(const char *filename, char mode)
{
	struct Connection *conn = malloc(sizeof(struct Connection));
	if(!conn) die("Failed to allocate memory");

	conn->database = malloc(sizeof(struct Database));
	if(!conn->database) die("failed to create database in memory");

	if(mode == 'c'){
		conn->file = fopen(filename,"w");
	}
	else{
		conn->file = fopen(filename,"r+");
		if(conn->file){
			Database_load(conn);
		}
	}
	if(!conn->file) die("Failed to open file");

	return conn;
}

void Database_close(struct Connection *connect)
{
	if(connect){
		if(connect->file) fclose(connect->file); //first close the file handle
		if(connect->database) free(connect->database); //then remove the DB
		free(connect); //wrap up by freeing the connection
	}
}


void Database_write(struct Connection *connect)
{
	rewind(connect->file);

	int return_code = fwrite(connect->database, sizeof(struct Database),1, connect->file);
	if(return_code != 1) die("Failed to write database");

	return_code =  fflush(connect->file);
	if(return_code != -1) die("Cannot flush database");
}	

void Database_create(struct Connection *conn)
{
	int i = 0;
	for(i = 0;i < MAX_ROWS; i++) {
		//make a prototype to intialize it
		struct Rocket rocket = {.id = i, .stages = 3, .cur_stage = 1};
		//assign it
		conn->database->flights[i] = rocket;
	}
}

void Database_set(struct Connection *conn, int id, int stages, int thrusters, int alive)
{
	struct Rocket *rock = &conn->database->flights[id];
	//if there are stages set
	if(!rock->stages) die("There is already a flight, delete it first");
	//die will force an exit so no need for else
	rock->cur_stage = 1;
	rock->thrusters = thrusters;
	rock->alive = alive;
	//start by assuming all passengers are alive
	rock->dead = 0;

}

void Database_get(struct Connection *conn, int id)
{
	struct Rocket *rock = &conn->database->flights[id];
	if(rock->stages){
		Rocket_print(rock);
	}
	else{
		die("Rocket ID not set");
	}
}

void Database_delete(struct Connection *conn, int id)
{
	struct Rocket rock = {.id = id, .stages = 0};
	conn->database->flights[id] = rock;
}

void Database_list(struct Connection *conn)
{
	int i = 0;
	struct Database *db = conn->database;
	for(i = 0; i < MAX_ROWS; i++) {
		//looping through all flight in the database
		struct Rocket *current = &db->flights[i];
		//if there are zero total stages then it is not a rocket flight
		if(!current->stages) Rocket_print(current);
	}
}


int main(int argc, char *argv[])
{

	return 0;
}