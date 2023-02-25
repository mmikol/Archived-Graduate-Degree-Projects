// Author: Miliano Mikol (mmikol@usc.edu)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include "cs402.h"
#include "my402list.h"
#include "my402transaction.h"

const int MAX_LINE_BUF_LENGTH=2000;
const int MAX_LINE_LENGTH=1024;
const int NUM_REQUIRED_TABS=3;
const int MAX_TIME_STAMP_LENGTH=11;
const int NUM_EXPECTED_FIELDS=4;
const int DESCRIPTION_SIZE=1024;
const int DATE_STR_LEN=15;
const int DESCRIPTION_STR_LEN=25;
const int NUMBER_STR_LEN=15;

/* -- UTILITIES -- */

// From https://stackoverflow.com/a/122721
char* TrimWhitespace(char *str) {
  char *end;

  while(isspace((unsigned char)*str)) str++;

  if(*str == 0) return str;

  end = str + strlen(str) - 1;
  
  while(end > str && isspace((unsigned char)*end)) end--;

  end[1] = '\0';

  return str;
}

/* -- HELPERS -- */

void CheckTypeField(char* type, int linenumber) {
    if (*type != '+' && *type != '-') {
        fprintf(stderr, "Error: invalid transaction type \"%s\" found in transaction %d (must be + or -).\n", type, linenumber);
        exit(EXIT_FAILURE);
    }
}

void CheckDateField(char* date, int linenumber) {
    for (int i=0; i < strlen(date); i++) {
        if(!isdigit(date[i])){
            fprintf(stderr, "Error: transaction %d contains non-numeric date.\n", linenumber);
            exit(EXIT_FAILURE);
        }
    }

    time_t transdate=atoi(date);
    time_t currdate=time(NULL);
    if (transdate < 0) {
        fprintf(stderr, "Error: non-positive timestamp found in transaction %d.\n", linenumber);
        exit(EXIT_FAILURE);
    } 

    if (transdate > currdate) {
        fprintf(stderr, "Error: timestamp found in transaction %d is greater than current date.\n", linenumber);
        exit(EXIT_FAILURE);
    }

    if (strlen(date) >= MAX_TIME_STAMP_LENGTH) {
        fprintf(stderr, "Error: invalid timestamp found in transaction %d.\n", linenumber);
        exit(EXIT_FAILURE);
    }
}

void CheckAmountField(char* amount, int linenumber) {
    for (int i=0; i < strlen(amount); i++) {
        if(!isdigit(amount[i]) && amount[i] != '.'){
            fprintf(stderr, "Error: transaction %d contains non-numeric amount.\n", linenumber);
            exit(EXIT_FAILURE);
        }
    }

    int leadingzero=0, allzeroes=0, decimalpointreached=0, count=0;    
    for (int i=0; i < strlen(amount); i++) {        
        if (leadingzero && amount[i] != '0' && amount[i] != '.' && !decimalpointreached) {       
            fprintf(stderr, "Error: transaction %d leading zero before non-zero number in amount field.\n", linenumber);
            exit(EXIT_FAILURE);
        }

        if (!decimalpointreached && count > 7 && !allzeroes) { 
            fprintf(stderr, "Error: transaction %d contains more than 7 digits before decimal point in amount field.\n", linenumber);
            exit(EXIT_FAILURE); 
        }

        if (decimalpointreached && count >= 2) {
            fprintf(stderr, "Error: transaction %d contains more than two digits after decimal point in amount field.\n", linenumber);
            exit(EXIT_FAILURE);
        }

        count++;

        if (amount[i] != '0' && decimalpointreached) {
            allzeroes=FALSE;
        }

        if (amount[i] == '0' && i < 1) {
            leadingzero=TRUE;
            allzeroes=TRUE;
        }

        if (amount[i] == '.') {
            decimalpointreached=TRUE; 
            count=0;
        }        
    }

    if (decimalpointreached && count < 2) {
        fprintf(stderr, "Error: transaction %d amount field does not have exactly two digits after decimal point.\n", linenumber);
        exit(EXIT_FAILURE);
    }

    if (!decimalpointreached) {
        fprintf(stderr, "Error: transaction %d does not contain required decimal point in amount field.\n", linenumber);
        exit(EXIT_FAILURE); 
    }

    if (allzeroes) {
        fprintf(stderr, "Error: transaction %d amount is 0.\n", linenumber);
        exit(EXIT_FAILURE);
    }
}

void CheckDescriptionField(char* description, int linenumber) {
    if (strlen(description) == 0) {
        fprintf(stderr, "Error: transaction %d contains an empty description.\n", linenumber);
        exit(EXIT_FAILURE); 
    }
}

void FormatDollarsWithOneComma(int dollars, char dollarbuf[NUMBER_STR_LEN]) {
    sprintf(dollarbuf, "%d,", dollars);
    for (int i=strlen(dollarbuf)-1; i > strlen(dollarbuf)-4; i--) {
        char temp=dollarbuf[i];
        dollarbuf[i]=dollarbuf[i-1];
        dollarbuf[i-1]=temp;
    }
}

void FormatDollarsWithTwoCommas(int dollars, char dollarbuf[NUMBER_STR_LEN]) {
    sprintf(dollarbuf, "%d,,", dollars);
    
    for (int i=strlen(dollarbuf)-1; i > strlen(dollarbuf)-5; i--) {
        char temp=dollarbuf[i];
        dollarbuf[i]=dollarbuf[i-1];
        dollarbuf[i-1]=temp;
    }

    for (int i=strlen(dollarbuf)-1; i > strlen(dollarbuf)-8; i--) {
        char temp=dollarbuf[i];
        dollarbuf[i]=dollarbuf[i-1];
        dollarbuf[i-1]=temp;
    }
}

void FormatCents(int centamount, char buf[NUMBER_STR_LEN]) {
    if (centamount >= 1000000000 || centamount <= -1000000000) {
        snprintf(buf, NUMBER_STR_LEN, "\?,\?\?\?,\?\?\?.\?\?");
    } else {
        int cents=(centamount < 0) ? -centamount % 100 : centamount % 100;
        int dollars=(centamount < 0 ? -centamount : centamount) / 100;
        if (dollars >= 1000000 || dollars <= -1000000) {
            char dollarbuf[NUMBER_STR_LEN];
            FormatDollarsWithTwoCommas(dollars, dollarbuf);
            snprintf(buf, NUMBER_STR_LEN, "%s.%02d", dollarbuf, cents);
        } else if (dollars >= 1000 || dollars <= -1000) {
            char dollarbuf[NUMBER_STR_LEN];
            FormatDollarsWithOneComma(dollars, dollarbuf);
            snprintf(buf, NUMBER_STR_LEN, "%s.%02d", dollarbuf, cents);
        } else {
            snprintf(buf, NUMBER_STR_LEN, "%d.%02d", dollars, cents);
        }
    }
}

/* -- PROCESSES -- */

void ReadInput(FILE* fp, My402List* list) {
    char buf[MAX_LINE_BUF_LENGTH];
    int linenumber=0;

    while (fgets(buf, sizeof(buf), fp)) {
        Transaction *transaction=(Transaction*)malloc(sizeof(Transaction));
        linenumber++;

        if (transaction == NULL) {
            fprintf(stderr, "Error: memory for transaction %d could not be allocated.\n", linenumber);
            exit(EXIT_FAILURE);
        }

        // Verify line size doesn't exceed spec limit
        if (strlen(buf) > MAX_LINE_LENGTH) { 
            fprintf(stderr, "Error: given tfile contains more than 1,204 characters on line %d.\n", linenumber);
            exit(EXIT_FAILURE);
        }
        
        // Verify correct number of fields/tabs
        int count=0;
        for (int i=0; i <= strlen(buf); i++) {
            count+=(buf[i] == '\t');
        }

        if (count > NUM_REQUIRED_TABS) {
            fprintf(stderr, "Error: given tfile does not contain exactly 3 tabs on line %d.\n", linenumber);
            exit(EXIT_FAILURE);
        }

        // Define each field/tab as a unique string
        char *start_ptr=buf, *tab_ptr=NULL;
        char *fields[NUM_EXPECTED_FIELDS];
        for (int i=0; i <= NUM_REQUIRED_TABS; i++) {
            tab_ptr=strchr(start_ptr, '\t');

            if (tab_ptr != NULL) {
                *tab_ptr++='\0'; 
            }

            fields[i]=start_ptr;

            if (strcmp(TrimWhitespace(fields[i]), "") == 0) {
                fprintf(stderr, "Error: given tfile contains an empty field on line %d.\n", linenumber);
                exit(EXIT_FAILURE);
            }

            start_ptr=tab_ptr;
        }

        CheckTypeField(fields[0], linenumber);
        CheckDateField(fields[1], linenumber);
        CheckAmountField(fields[2], linenumber);
        CheckDescriptionField(fields[3], linenumber);
        
        char* wholepart=strsep(&fields[2], ".");
        char* halfpart=fields[2];

        transaction->index=linenumber;
        transaction->type=*fields[0];
        transaction->date=atoi(fields[1]);

        if (atoi(wholepart) < 0) {
            transaction->amount=(atoi(wholepart)*100)-atoi(halfpart);
        } else {
            transaction->amount=(atoi(wholepart)*100)+atoi(halfpart);
        }

        transaction->balance=0;

        strncpy(transaction->description, fields[3], DESCRIPTION_STR_LEN-1);

        My402ListAppend(list, (Transaction*)transaction);
    }

    if (My402ListLength(list) <= 0) {
        fprintf(stderr, "Error: given tfile contains 0 valid transactions.\n");
        exit(EXIT_FAILURE); 
    }
}

static void BubbleForward(My402List *pList, My402ListElem **pp_elem1, My402ListElem **pp_elem2) {
    My402ListElem *elem1=(*pp_elem1), *elem2=(*pp_elem2);
    void *obj1=elem1->obj, *obj2=elem2->obj;
    My402ListElem *elem1prev=My402ListPrev(pList, elem1);
    My402ListElem *elem2next=My402ListNext(pList, elem2);

    My402ListUnlink(pList, elem1);
    My402ListUnlink(pList, elem2);
    if (elem1prev == NULL) {
        (void)My402ListPrepend(pList, obj2);
        *pp_elem1=My402ListFirst(pList);
    } else {
        (void)My402ListInsertAfter(pList, obj2, elem1prev);
        *pp_elem1=My402ListNext(pList, elem1prev);
    }
    if (elem2next == NULL) {
        (void)My402ListAppend(pList, obj1);
        *pp_elem2=My402ListLast(pList);
    } else {
        (void)My402ListInsertBefore(pList, obj1, elem2next);
        *pp_elem2=My402ListPrev(pList, elem2next);
    }
}

static void BubbleSortForwardList(My402List *pList, int num_items) {
    My402ListElem *elem=NULL;
    int i=0;

    if (My402ListLength(pList) != num_items) {
        fprintf(stderr, "List length is not %1d in BubbleSortForwardList().\n", num_items);
        exit(EXIT_FAILURE);
    }
    
    for (i=0; i < num_items; i++) {
        int j=0, something_swapped=FALSE;
        My402ListElem *next_elem=NULL;

        for (elem=My402ListFirst(pList), j=0; j < num_items-i-1; elem=next_elem, j++) {
            time_t cur_val=((Transaction*)elem->obj)->date, next_val=0;

            next_elem=My402ListNext(pList, elem);
            next_val=((Transaction*)next_elem->obj)->date;

            if (cur_val == next_val) {
                fprintf(stderr, "Error: non-unique date found on line %d of given tfile (dates must be unique)\n", ((Transaction*)elem->obj)->index);
                exit(EXIT_FAILURE);
            }

            if (cur_val > next_val) {
                BubbleForward(pList, &elem, &next_elem);
                something_swapped=TRUE;
            }
        }
        if (!something_swapped) break;
    }
}

void PrintOutput(My402List* list) {
    My402ListElem *elem;
    Transaction *trans;

    fprintf(stdout, "+-----------------+--------------------------+----------------+----------------+\n");
    fprintf(stdout, "|       Date      | Description              |         Amount |        Balance |\n");
    fprintf(stdout, "+-----------------+--------------------------+----------------+----------------+\n");

    for (elem=My402ListFirst(list); elem != NULL; elem=My402ListNext(list, elem)) {
        trans=(Transaction*)elem->obj;
        
        // Print date
        char* date=(char*)ctime(&(trans->date));
        fprintf(stdout, "| ");
		fprintf(stdout, "%.*s", DATE_STR_LEN-4, date);
		fprintf(stdout, "%.*s", DATE_STR_LEN-11, date+20);
        fprintf(stdout, " | ");
        
        // Print description  
        fprintf(stdout, "%s", trans->description);      
        if (strlen(trans->description) < DESCRIPTION_STR_LEN) {
            int spaceused=max(DESCRIPTION_STR_LEN-strlen(trans->description)-1, 0);
            for (int i=0; i < spaceused; i++) {
                fprintf(stdout, "%c", ' ');
            }
        }
        
        // Print amount
        char amount[NUMBER_STR_LEN];
        FormatCents(trans->amount, amount);
        fprintf(stdout, " | ");
        if (trans->type == '+') {
            int spaceused=max(NUMBER_STR_LEN-strlen(amount)-2, 0);
            for (int i=0; i < spaceused; i++) {
                fprintf(stdout, "%c", ' ');
            }
            fprintf(stdout, "%s", amount);
            fprintf(stdout, "%c", ' ');
        } else if (trans->type == '-') {
            fprintf(stdout, "%c", '(');
            int spaceused=max(NUMBER_STR_LEN-strlen(amount)-3, 0);
            for (int i=0; i < spaceused; i++) {
                fprintf(stdout, "%c", ' ');
            }
            fprintf(stdout, "%s", amount);
            fprintf(stdout, "%c", ')');
        }
        
		// Print balance
        char balance[NUMBER_STR_LEN];
        FormatCents(trans->balance, balance);
        fprintf(stdout, " | ");        
        if (trans->balance > 0) {
            int spaceused=max(NUMBER_STR_LEN-strlen(balance)-2, 0);
            for (int i=0; i < spaceused; i++) {
                fprintf(stdout, "%c", ' ');
            }
            fprintf(stdout, "%s", balance);
            fprintf(stdout, "%c", ' ');
        } else {
            fprintf(stdout, "%c", '(');
            int spaceused=max(NUMBER_STR_LEN-strlen(balance)-3, 0);
            for (int i=0; i < spaceused; i++) {
                fprintf(stdout, "%c", ' ');
            }
            fprintf(stdout, "%s", balance);
            fprintf(stdout, "%c", ')');
        }       

        fprintf(stdout, " |\n");
    }
    
    fprintf(stdout, "+-----------------+--------------------------+----------------+----------------+\n");
}

void ComputeBalances(My402List* list) {
    for (My402ListElem* elem=My402ListFirst(list); elem != NULL; elem=My402ListNext(list, elem)) {
        Transaction* trans=(Transaction*)elem->obj;     
        if (elem == My402ListFirst(list)) {
            if (trans->type == '+') {
                trans->balance=trans->amount;
            } else if (trans->type == '-') {
                trans->balance=-trans->amount;
            }
        } else {
            My402ListElem* prevelem=My402ListPrev(list, elem);
            Transaction* prevtrans=(Transaction*)prevelem->obj;        
            if (trans->type == '+') {
                trans->balance=prevtrans->balance+trans->amount;
            } else if (trans->type == '-') {
                trans->balance=prevtrans->balance-trans->amount;
            }
        }
    } 
}

/* -- MAIN -- */

int main(int argc, char *argv[]) { 
    if (argc < 2) {
        fprintf(stderr, "Error: malformed command; at least two arguments must be supplied.\n");
        fprintf(stdout, "Usage: warmup1 sort [tfile].\n");
        exit(EXIT_FAILURE);
    } 
    
    if (strcmp(argv[1], "sort") != 0) {
        fprintf(stderr, "Error: malformed command; \"%s\" is an invalid command line option.\n", argv[1]);
        fprintf(stdout, "Usage: warmup1 sort [tfile].\n");
        exit(EXIT_FAILURE);
    }

    if (argc > 3) {
        fprintf(stderr, "Error: malformed command; at most three arguments must be supplied.\n");
        fprintf(stdout, "Usage: warmup1 sort [tfile].\n");
        exit(EXIT_FAILURE);
    }

    My402List transactions;
    memset(&transactions, 0, sizeof(My402List));

    if (My402ListInit(&transactions) != 1) { 
        fprintf(stderr, "%s", "Error: list initialization failed.`\n");
        exit(EXIT_FAILURE);
    }

    if (argc == 3) {        
        FILE *fp=NULL;
        fp=fopen(argv[2], "r");
        if (fp == NULL) {
            fprintf(stderr, "Error: cannot open \"%s\" for reading.\n", argv[2]);
            fprintf(stdout, "Usage: warmup1 sort [tfile].\n");
            exit(EXIT_FAILURE);
        }

        struct stat sb;
        stat(argv[2], &sb);
        if (S_ISDIR(sb.st_mode)) {
            fprintf(stderr, "Error: \"%s\" is a directory.\n", argv[2]);
            fprintf(stdout, "Usage: warmup1 sort [tfile].\n");
            exit(EXIT_FAILURE);
        }
        ReadInput(fp, &transactions);    
        fclose(fp);
    } else {
        ReadInput(stdin, &transactions);    
    }

    BubbleSortForwardList(&transactions, My402ListLength(&transactions));
    ComputeBalances(&transactions);
    PrintOutput(&transactions);
    My402ListUnlinkAll(&transactions);

    return 1;
}
