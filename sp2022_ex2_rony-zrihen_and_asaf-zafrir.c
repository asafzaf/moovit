

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
enum bool { FALSE, TRUE };

/*
exercise 2 
by:
Asaf zafrir
ID: 205929029

rony zrihen
ID: 318318917549
*/


/**********************************************************************************************************************/
                /*******************************  ---  Helpers  --- *******************************/
/**********************************************************************************************************************/

#define TABLE(DO)                                                              \
  DO(SCHEDULE_SUCCESS)                                                         \
  DO(SCHEDULE_NULL_ARG)                                                        \
  DO(SCHEDULE_OUT_OF_MEMORY)                                                   \
  DO(SCHEDULE_ALREADY_EXISTS)                                                  \
  DO(SCHEDULE_INVALID_LINE_TYPE)                                               \
  DO(SCHEDULE_INVALID_LINE_NUMBER)                                             \
  DO(SCHEDULE_INVALID_PRICE)                                                   \
  DO(SCHEDULE_LINE_DOESNT_EXIST)                                               \
  DO(SCHEDULE_STATION_DOESNT_EXIST)

#define CODE_AS_STR(code) #code,

#define CODE_AS_STRUCT(code) code,

const char *code_strings[] = {TABLE(CODE_AS_STR)};

typedef enum { TABLE(CODE_AS_STRUCT) LAST_ERROR } schedule_error_code;

void print_error_code_func(schedule_error_code code, const char *func) {
  FILE *out = stderr;
  if (code < SCHEDULE_SUCCESS || code >= LAST_ERROR) {
    fprintf(out, "Invalid error code %d in function %s\n", code, func);
    exit(1);
  }
  fprintf(out, "[%s]: %s\n", func, code_strings[code]);
}

/**********************************************************************************************************************/
      /*******************************  ---  Definitions for your usage  --- *******************************/
/**********************************************************************************************************************/

typedef struct stations_t {
  char *name;
  struct stations_t *next_station;
} stations;

typedef struct lines_t {
  int type;
  int number;
  float price;
  stations *stations;
  struct lines_t *next_line;
} lines;

typedef struct schedule_line_node_s {
  lines *line;
  // stations *station;
} schedule_line_node_t;

typedef enum type { BUS, METRO, TRAIN } type;
static const char *const type_name[] = {
    [BUS] = "Bus", [METRO] = "Metro", [TRAIN] = "Train"};

/**********************************************************************************************************************/

/* Use the following macro to print the error_codes (SCHEDULE_SUCCESS as well)
 */

/* MACRO: schedule_print_error_code
 *      Prints the error code to stderr
 * Example:
 *      // Your code here
 *      // ...
 *      print_error_code(SCHEDULE_OUT_OF_MEMORY);
 *      // ...
 *      // Continue of your code
 *
 */
#define print_error_code(code) print_error_code_func(code, __FUNCTION__)
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/*************************************  ---  Functions you need to implement ---
 * **************************************/
/**********************************************************************************************************************/

/* function: schedule_create
 * parameters:
 *      None
 * errors:
 *      SCHEDULE_SUCCESS
 *      SCHEDULE_OUT_OF_MEMORY
 * return value:
 *      - Pointer to a new allocated linked list
 *      - NULL incase of failure
 */
schedule_line_node_t *schedule_create() {
  schedule_line_node_t *new_schedule;
  new_schedule = (schedule_line_node_t *)malloc(sizeof(schedule_line_node_t));
  if (new_schedule == NULL) {
    print_error_code(SCHEDULE_OUT_OF_MEMORY);
    return NULL;
  }
  new_schedule->line = NULL;
  print_error_code(SCHEDULE_SUCCESS);
  return new_schedule;
}

/**********************************************************************************************************************/
void delete_stations(lines *head) {
  stations *curr, *prev;
  curr = head->stations;
  if (curr == NULL) { // there are no stations - delete line
    print_error_code(SCHEDULE_NULL_ARG);
    return;
  }
  while (curr != NULL) {
    prev = curr;
    curr = curr->next_station;
    free(prev->name);
    free(prev);
  }
}
/**********************************************************************************************************************/
/*
 * function: schedule_destroy
 * parameters:
 *      schedule - Pointer to the linked list to be destroyed
 * errors:
 *      *SCHEDULE_SUCCESS
 *      *SCHEDULE_NULL_ARG
 * return value:
 *      None
 */

void schedule_destroy(schedule_line_node_t *head) {
  lines *curr_line, *prev_line;
  if (head == NULL || head->line == NULL) {
    print_error_code(SCHEDULE_NULL_ARG);
    return;
  }
  curr_line = head->line;
  if (curr_line == NULL) { // there are no lines!
    print_error_code(SCHEDULE_NULL_ARG);
    return;
  }

  while (curr_line != NULL) {
    prev_line = curr_line;
    delete_stations(curr_line);
    curr_line = curr_line->next_line;
    free(prev_line);
  }
  free(head);
  print_error_code(SCHEDULE_SUCCESS);
}
/**********************************************************************************************************************/

// find line function
lines *find_line(schedule_line_node_t *head, int line_number) {

  lines *curr_line = head->line;

  while (curr_line != NULL && curr_line->number != line_number) {
    curr_line = curr_line->next_line;
  }
  return curr_line;
}
/**********************************************************************************************************************/

int is_price_valid(float price) {
  price *= 1000;
  if ((int)price % 10) {
    return FALSE;
  }
  return TRUE;
}
/**********************************************************************************************************************/
int is_create_line_valid(schedule_line_node_t *head, int line_type,
                         unsigned int line_number, float price) {
  if (head == NULL) {
    print_error_code(SCHEDULE_NULL_ARG);
    return TRUE;
  }
  if (!is_price_valid(price)) {
    print_error_code(SCHEDULE_INVALID_PRICE);
    return TRUE;
  }
  if (line_type > 3 || line_type < 0) {
    print_error_code(SCHEDULE_INVALID_LINE_TYPE);
    return TRUE;
  }
  if (line_number > 100) {
    print_error_code(SCHEDULE_INVALID_LINE_NUMBER);
    return TRUE;
  }
  return FALSE;
}
/**********************************************************************************************************************/
/*
 * function: schedule_add_line
 * parameters:
 *      schedule - Pointer to the linked list
 *      line_type - Type of the line to be added
 *      line_number - Number of the line to be added
 *      price - Price of the line
 * errors:
 *      *SCHEDULE_SUCCESS
 *      *SCHEDULE_OUT_OF_MEMORY
 *      SCHEDULE_NULL_ARG
 *      *SCHEDULE_INVALID_LINE_TYPE
 *      *SCHEDULE_INVALID_LINE_NUMBER
 *      *SCHEDULE_ALREADY_EXISTS
 *      *SCHEDULE_INVALID_PRICE
 * return value:
 *      - Pointer to the new head of the list
 *     - NULL incase of failure
 */
schedule_line_node_t *schedule_add_line(schedule_line_node_t *head,
                                        int line_type, unsigned int line_number,
                                        float price)

{

  if (is_create_line_valid(head, line_type, line_number, price)) {
    return NULL;
  }

  if (find_line(head, line_number) != NULL) {
    print_error_code(SCHEDULE_ALREADY_EXISTS);
    return NULL;
  }

  lines *new_line = (lines *)malloc(sizeof(lines));

  if (new_line == NULL) {
    print_error_code(SCHEDULE_OUT_OF_MEMORY);
    return NULL;
  }

  lines *curr_line;
  curr_line = head->line;

  new_line->type = line_type;
  new_line->number = line_number;
  new_line->price = price;
  new_line->next_line = NULL;
  new_line->stations = NULL;

  if (head->line == NULL) {
    head->line = new_line;
    print_error_code(SCHEDULE_SUCCESS);
    return head;
  }

  while (curr_line->next_line != NULL) {
    curr_line = curr_line->next_line;
  }
  curr_line->next_line = new_line;
  print_error_code(SCHEDULE_SUCCESS);
  return head;
}
/**********************************************************************************************************************/

/*
 * function: schedule_remove_line
 * parameters:
 *      schedule - Pointer to the linked list
 *      line_number - Number of the line to be removed
 * errors:
 *      *SCHEDULE_SUCCESS
 *      *SCHEDULE_NULL_ARG
 *      *SCHEDULE_LNVALID_LINE_NUMBER
 *      *SCHEDULE_LINE_DOESNT_EXIST
 * return value:
 *      - Pointer to the new head of the list
 *     - NULL incase of failure
 */
schedule_line_node_t *schedule_remove_line(schedule_line_node_t *head,
                                           unsigned int line_number) {
  lines *curr_line, *prev_line;
  if (line_number > 100) {
    print_error_code(SCHEDULE_INVALID_LINE_NUMBER);
    return NULL;
  }

  if (head == NULL || head->line == NULL) {
    print_error_code(SCHEDULE_NULL_ARG);
    return NULL;
  }

  curr_line = find_line(head, line_number); // looking for line
  prev_line = head->line;

  if (curr_line == NULL) { // couldent find a node to delete
    print_error_code(SCHEDULE_LINE_DOESNT_EXIST);
    return NULL;
  }

  if (curr_line == head->line) { // the first node is the one to be deleted
    head->line = prev_line->next_line;
    free(curr_line);
    print_error_code(SCHEDULE_SUCCESS);
    return head;
  }

  while (prev_line->next_line != curr_line) {
    prev_line = prev_line->next_line;
  }
  prev_line->next_line = curr_line->next_line; // delete selected node
  free(curr_line);
  print_error_code(SCHEDULE_SUCCESS);
  return head;
}
/**********************************************************************************************************************/

/*
 * function: schedule_add_station
 * parameters:
 *      schedule - Pointer to the linked list
 *      line_number - Number of the line to add station to
 *      station_name - Name of the station to be added
 * errors:
 *      *SCHEDULE_SUCCESS
 *      *SCHEDULE_NULL_ARG
 *      *SCHEDULE_OUT_OF_MEMORY
 *      *SCHEDULE_INVALID_LINE_NUMBER
 *      *SCHEDULE_LINE_DOESNT_EXIST
 * return value:
 *      - Pointer to the new head of the list
 *      - NULL incase of failure
 */
void *
schedule_add_station(schedule_line_node_t *head, unsigned int line_number,
                     const char *station) // Y VOID BUT NO DOUBLE POINTER?!?!?
{
  stations *new_station, *curr_station;
  lines *The_line;
  char *station_name;
  if (head == NULL || head->line == NULL) { // there arn't any lines yet
    print_error_code(SCHEDULE_NULL_ARG);
    return NULL;
  }
  if (line_number > 100) { // validate line number
    print_error_code(SCHEDULE_INVALID_LINE_NUMBER);
    return NULL;
  }

  // The_line = head->line;
  The_line = find_line(head, line_number); // looking for line
  if (The_line == NULL)                    // couldn't find the chosen line
  {
    print_error_code(SCHEDULE_LINE_DOESNT_EXIST);
    return NULL;
  }
  curr_station = The_line->stations;

  new_station = (stations *)malloc(
      sizeof(station)); // memory allocation to new struct of station
  if (new_station == NULL) {
    print_error_code(SCHEDULE_OUT_OF_MEMORY);
    return NULL;
  }

  station_name = (char *)malloc(
      (strlen(station) + 1) * sizeof(char)); // memory allocaion to station name
  if (station_name == NULL) {
    print_error_code(SCHEDULE_OUT_OF_MEMORY);
    return NULL;
  }

  strcpy(station_name, station);
  new_station->name = station_name;
  new_station->next_station = NULL;

  if (The_line->stations == NULL) {
    print_error_code(SCHEDULE_SUCCESS);
    The_line->stations = new_station;
    return head;
  }

  while (curr_station->next_station != NULL) // skipping to the last station
  {
    curr_station = curr_station->next_station;
  }

  curr_station->next_station = new_station;
  print_error_code(SCHEDULE_SUCCESS);
  return head;
}
/**********************************************************************************************************************/

/*
 * function: schedule_remove_station
 * parameters:
 *      schedule - Pointer to the linked list
 *      line_number - Number of the line to remove station from
 *      station_index - The pointion of the station in the bus route (starting
 * at 0) errors:
 *      *SCHEDULE_SUCCESS
 *      *SCHEDULE_NULL_ARG
 *      *SCHEDULE_INVALID_LINE_NUMBER
 *      *SCHEDULE_LINE_DOESNT_EXIST
 *      *SCHEDULE_STATION_DOESNT_EXIST
 * return value:
 *      - Pointer to the new head of the list
 *      - NULL incase of failure
 */
schedule_line_node_t *schedule_remove_station(schedule_line_node_t *head,
                                              unsigned int line_number,
                                              unsigned int station_index) {
  if (line_number > 100) {
    print_error_code(SCHEDULE_INVALID_LINE_NUMBER);
    return NULL;
  }
  if (head == NULL || head->line == NULL) {
    print_error_code(SCHEDULE_NULL_ARG);
    return NULL;
  }
  stations *curr_station, *prev_station;
  lines *The_line = head->line;

  The_line = find_line(head, line_number); // looking for the line
  if (The_line == NULL) {
    print_error_code(SCHEDULE_LINE_DOESNT_EXIST);
    return NULL;
  }

  curr_station = The_line->stations;
  prev_station = curr_station;
  if (curr_station == NULL) {
    print_error_code(SCHEDULE_NULL_ARG);
    return NULL;
  }

  if (station_index == 0) {
    The_line->stations = curr_station->next_station;
    free(curr_station->name);
    free(curr_station);
    print_error_code(SCHEDULE_SUCCESS);
    return head;
  }

  curr_station = curr_station->next_station;

  for (int i = 1; i < station_index;
       ++i) // skipping to the (index-1) station ( if index not equal to 0 ) :
            // // prev and curr
  {
    prev_station = curr_station;
    curr_station = curr_station->next_station;
  }
  if (curr_station == NULL) // for next iteration
  {
    print_error_code(SCHEDULE_STATION_DOESNT_EXIST);
    return NULL;
  }

  prev_station->next_station =
      curr_station
          ->next_station; // also working if curr station is null (last index)
  free(curr_station->name);
  free(curr_station);
  print_error_code(SCHEDULE_SUCCESS);
  return head;
}
/**********************************************************************************************************************/

/*
 * function: schedule_get_station_index
 * parameters:
 *      schedule - Pointer to the linked list
 *      line_number - Number of the line to print it's path
 * errors:
 *      *SCHEDULE_SUCCESS
 *      *SCHEDULE_NULL_ARG
 *      *SCHEDULE_INVALID_LINE_NUMBER
 * return value:
 *      - None
 */
void schedule_print_line_route(schedule_line_node_t *head,
                               unsigned int line_number) {
  stations *curr_station;
  lines *The_line;
  int counter = 0;

  if (head == NULL || head->line == NULL) {
    print_error_code(SCHEDULE_NULL_ARG);
    return;
  }

  if (line_number > 100) {
    print_error_code(SCHEDULE_INVALID_LINE_NUMBER);
    return;
  }

  The_line = head->line;
  while (line_number != The_line->number) // looking for the line
  {
    The_line = The_line->next_line;
  }
  curr_station = The_line->stations;

  if (curr_station == NULL) {
    print_error_code(SCHEDULE_NULL_ARG);
    return;
  }

  printf("\n\t| Line number | type \t | price  \n"
         "\t=================================\n"
         "\t| \t%-3d    | %-5s\t | %.2f \n"
         "\t/////////////////////////////////\n",
         The_line->number, type_name[The_line->type], The_line->price);
  printf("\t| No. | Station name\n"
         "\t|=====|=============\n");
  while (curr_station != NULL) {
    printf("\t| %-3d | %s\n", counter++, curr_station->name);
    curr_station = curr_station->next_station;
  }
  printf("\n\n");
  print_error_code(SCHEDULE_SUCCESS);
}
/**********************************************************************************************************************/

/*
 * function: schedule_print_all_lines
 * parameters:
 *     schedule - Pointer to the linked list
 * errors:
 *      *SCHEDULE_SUCCESS
 *     *SCHEDULE_NULL_ARG
 * return value:
 *     - None
 */
void schedule_print_all_lines(schedule_line_node_t *head) {

  if (head == NULL || head->line == NULL) {
    print_error_code(SCHEDULE_NULL_ARG);
    return;
  }
  lines *curr_line = head->line;
  stations *curr_station;

  printf("\t| Type\t| No. | Price\t| First Stop\t| Last stop\n"
         "\t==================================================\n"); // headlines
  while (curr_line != NULL) // loop for all of the lines
  {
    printf("\t| %s\t| %-3d | %.2f\t", type_name[curr_line->type],
           curr_line->number,
           curr_line->price); // printing part 1 of the line

    curr_station = curr_line->stations;
    if (curr_station == NULL) {
      printf("| N/A\t\t| N/A\n"); // if there is no stations printing N/A
      curr_line = curr_line->next_line;
      continue;
    }
    printf("| %-13s", curr_line->stations->name);
    while (curr_station->next_station != NULL) {
      curr_station = curr_station->next_station; // skip to the last station
    }
    printf("\t| %s\n", curr_station->name); // printing part 2 of the line
    curr_line = curr_line->next_line;
  }
  print_error_code(SCHEDULE_SUCCESS);
}
/**********************************************************************************************************************/
stations *find_station(stations *head, const char *station) {
  stations *curr_station = head;
  if (curr_station == NULL) {
    print_error_code(SCHEDULE_NULL_ARG);
    return NULL;
  }
  while (curr_station != NULL) {
    if (!strcmp(curr_station->name, station)) {
      return curr_station;
    }
    curr_station = curr_station->next_station;
  }
  return NULL;
}
/**********************************************************************************************************************/
/*
 * function: schedule_print_lines_for_station
 * parameters:
 *    schedule - Pointer to the linked list
 *    from - Name of the origin station
 *    to - Name of the destination station
 * errors:
 *     SCHEDULE_SUCCESS
 *    SCHEDULE_NULL_ARG
 * return value:
 *    - None
 */


void schedule_print_lines_for_stations(schedule_line_node_t *head,
                                       const char *from, const char *to) {
  stations *curr_station, *from_station, *to_station;
  lines *curr_line = head->line;
  if (curr_line == NULL) {
    print_error_code(SCHEDULE_NULL_ARG);
    return;
  }

  printf("\t-------------------------------------------------------\n"
         "\t| Type  | No. | Price | First Station\t | Last Station\n"
         "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  while (curr_line != NULL) {
    curr_station = curr_line->stations;

    from_station = find_station(curr_station, from);
    if (from_station != NULL) {

      to_station = find_station(from_station->next_station, to);
      if (to_station != NULL) {
        while (to_station->next_station != NULL) {
          to_station = to_station->next_station;
        }
         printf("\t| %s | %-3d | %.2f  | %-11s\t | %s\n",
               type_name[curr_line->type], curr_line->number, curr_line->price,
               curr_line->stations->name, to_station->name);
      }
    }
    curr_line = curr_line->next_line;
  }
}
/**********************************************************************************************************************/