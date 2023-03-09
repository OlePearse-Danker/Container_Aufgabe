#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_CRANE_COUNT 8

typedef struct HARBOUR_DAY
{
    int crane_id;
    int total_containers;
    int containers_loaded;
    char crane_status[8];
}
HARBOUR_DAY;

short get_short(char text[], short MIN, short MAX);

int main(int argc, char const *argv[])
{

system("clear");

FILE *fp; 
FILE *fp_out;

HARBOUR_DAY *sp;

sp = malloc(MAX_CRANE_COUNT * sizeof(HARBOUR_DAY));

const size_t BUFFER_SIZE = 1024;

size_t crane_count = 0;

char *buffer = malloc(sizeof(*buffer) * BUFFER_SIZE);
// checking for memory allocation error
    if (!buffer)
    {
        fprintf(stderr, "Memory allocation error! Exiting!");
        exit(EXIT_FAILURE);
    }


// Declare a harbour pointer and initialize the pointer with allocating memory
HARBOUR_DAY *harbour_action = malloc(sizeof(*harbour_action) * MAX_CRANE_COUNT);

    if (!harbour_action)
    {
        fprintf(stderr, "Memory allocation error! Exiting!");
        exit(EXIT_FAILURE);
    }



// opening the csv file in read mode
fp = fopen("data.csv", "r");

// error checking, whether the file could be opened or not
    if (!fp)
    {
        fprintf(stderr, "File could not be opened. Exiting..\n");
        exit(EXIT_FAILURE);
    }

// reading in the first line
fgets(buffer, BUFFER_SIZE-1, fp);

    for (size_t i = 0; i < MAX_CRANE_COUNT; i++)
    {
        fgets(buffer, BUFFER_SIZE-1, fp);
        crane_count++;

            sscanf(buffer, "%d,%d,%d,%s",
            &sp[i].crane_id,
            &sp[i].total_containers,
            &sp[i].containers_loaded,
            &sp[i].crane_status);

            // if we reached the end of file -> stop the program

            if (feof(fp)) break;    

    }

    // variable to get user input
    int ch;

    // struct with the changes of the crane_status from user
    HARBOUR_DAY status_input;

    do
    {

        // variable to repeat while loop
        int user_input = get_short("Enter the number of crane to see status", 1, 7);
        user_input--;

        printf("\n");
        printf("crane ID: %d\n", sp[user_input].crane_id);
        printf("containers loaded: %d\n", sp[user_input].containers_loaded);
        printf("containers left to load: %d\n", (sp[user_input].total_containers - sp[user_input].containers_loaded));
        printf("crane_Status: %s\n", sp[user_input].crane_status);
        printf("\n");


        if (strcmp(sp[user_input].crane_status, "warning") == 0 || strcmp(sp[user_input].crane_status, "error") == 0)
        {
            printf("Do you want to continue operation and clear the error/warning? (y/n): ");
            ch = getchar();

            if (ch == 'y')
            {
                // if containers_loaded is equal to total containers -> status change to idle
                if (sp[user_input].containers_loaded == sp[user_input].total_containers)
                {
                    strcpy(sp[user_input].crane_status, "idle");
                    printf("Crane status changed to: %s", sp[user_input].crane_status );
                    
                    // copying the whole struct into status_input, so that we can overwrite afterwods

                    status_input = sp[MAX_CRANE_COUNT];
                    strcpy(status_input.crane_status, sp[user_input].crane_status); 

                }else if (sp[user_input].containers_loaded != 0)
                {
                    strcpy(sp[user_input].crane_status, "okay");
                    printf("Crane status changed to: %s", sp[user_input].crane_status );

                    // copying the whole struct into status_input, so that we can overwrite afterwods

                    status_input = sp[MAX_CRANE_COUNT];
                    strcpy(status_input.crane_status, sp[user_input].crane_status); 
                }
                }else
                {
                    printf("Going back to menue..\n\n");
                }

                fp_out = fopen("data_out.csv", "w");

            // printing header
            fprintf(fp_out, "crane_id,total_containers,containers_loaded,crane_status\n");

            for (size_t i = 0; i < MAX_CRANE_COUNT-1; i++)
            {
                fprintf(fp_out, "%04d,%d,%d,%s\n",
                    sp[i].crane_id,
                    sp[i].total_containers,
                    sp[i].containers_loaded,
                    sp[i].crane_status
                );
            }
                
        
        }

    }while(ch != 'y');
        
    fclose(fp_out);
    fclose(fp);
    
    free(sp);
    free(buffer);
    
    return 0;

    
}

short get_short(char text[], short MIN, short MAX)
{
    //declare working variables 
    short value;
    int finished = 0; //0 f�r FALSE
    char ch;
    int retVal;

    do
    {
        printf("%s: ", text); //Abk�rzung "s" f�r "string" / Zeichenkette

        ch = '\0'; // \0 wird als ein Buchstabe gewertet; O ist nicht die Zahl 0 sondern ein "Null Character"; Null Character hat in der Ascii Tabelle auch den Wert 0

        retVal = scanf("%hd%c", &value, &ch); // -> "hd" steht f�r short -> Adresse ist "Value"; scanf gibt auch einen Wert zur�ck, diesen speichern wir in retVal 

        // check for valid user input
        if (retVal != 2) printf("Das war keine korrekte Zahl!\n");
        else if (ch != '\n') printf("Unerwartete Zeichen hinter der Zahl!\n");
        else if (value < MIN) printf("Zahl ist zu klein (MIN: %hd)\n", MIN);
        else if (value > MAX) printf("Zahl ist zu gro%c (MAX: %hd)\n", 225, MAX);
        else finished = 1; // falls die Variable tats�chlich 2 ist; und der character ein newline, ist alles richtig und die loop wird beendet

        //Variable finished wird auf 1 gesetzt -> weil 1 f�r TRUE

        //clear input stream buffer
        while (ch != '\n') scanf("%c", &ch); //Variation mit scanf von getchar

    } while (!finished); //repeat if not finished 

    //return user input
    return value; //wer auch immer die Funktion get_short aufruft bekomm "value" zur�ck
}
