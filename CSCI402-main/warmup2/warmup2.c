// Author: Miliano Mikol (mmikol@usc.edu)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <signal.h>
#include <math.h>
#include "cs402.h"
#include "my402list.h"
#include "packet.h"

/* -- CONSTANTS -- */

#define _BSD_SOURCE
#define MAX_TOKEN_VALUE 2147483647
#define MAX_NUM_FIELDS 3 
#define MAX_LINE_SIZE 1024
#define MAX_SLEEP_TIME_sec 10.0

/* -- GLOBAL VALUES -- */
pthread_t packetarrivalthread, tokenthread, serverthread1, serverthread2, ctrlcthread;
pthread_mutex_t mutex;
pthread_cond_t cv;
sigset_t set;
My402List Q1, Q2;
struct timeval starttime, timeelapsed=(struct timeval){0}, previouspacketarrivaltime=(struct timeval){0}, totalemulationtime=(struct timeval){0}; // Emulation structs
struct timeval interarrivaltimesum=(struct timeval){0}, servicetimesum=(struct timeval){0}, q1timesum=(struct timeval){0}, q2timesum=(struct timeval){0}, s1timesum=(struct timeval){0}, s2timesum=(struct timeval){0}, timeinsystemsum=(struct timeval){0}; // Stats structs
double timeinsystemsumsquared=0.0;
FILE* fp=NULL;
char* tsfile=NULL;
char currentlineinfile[MAX_LINE_SIZE]; 
double mu_packetpersec=0.35, r_tokenpersec=1.5;
int B=10, P=3, num=20, tokenbucket=0, lambda_packetpersec=1;
int linenumber=0, tokencount=0, packetarrivalcount=0, packetcompletioncount=0, packettransmissioncount=0, tokendropcount=0, packetdropcount=0;
int timetoquit=FALSE;


/* -- UTILITIES -- */

char* TrimWhitespace(char *str) {
  char *end;
  while(isspace((unsigned char)*str)) str++;
  if(*str == 0) return str;
  end=str+strlen(str)-1;
  while(end > str && isspace((unsigned char)*end)) end--;
  end[1]='\0';
  return str;
}

int IsNumeric(char* str) {
    for (char* c=str; *c != '\0'; c++) if (!isdigit(*c)) return FALSE;
    return TRUE;
}

/* -- HELPERS -- */

void SetTimeElapsed(struct timeval* result) {
    gettimeofday(result, NULL);
    timersub(result, &starttime, result);
}

double TimevalToDouble_msec(struct timeval result) {
    return (1000.0*result.tv_sec)+(result.tv_usec/1000.0);
}

char ReadNextLine() {
    char result=*fgets(currentlineinfile, sizeof(currentlineinfile), fp);
    if (result) {
        linenumber++;
        if (strlen(currentlineinfile) > MAX_LINE_SIZE) { 
            fprintf(stderr, "Error: given tsfile contains more than 1,204 characters on line %d.\n", linenumber);
            exit(EXIT_FAILURE);
        }
    }
    return result;
}

void CheckCommands(int argc, char* argv[]) {
    if (argc > 15) { 
        fprintf(stderr, "Error: Malformed command\n");
        printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
        exit(EXIT_FAILURE);
    } 
    for (int i=1; i < argc; i+=2) {
        if (strcmp(argv[i], "-lambda") == 0) {
            if (i+1 < argc) {
                if (argv[i+1][0] != '-') {
                    if (!sscanf(argv[i+1], "%d", &lambda_packetpersec)) {
                        fprintf(stderr, "Error: Invalid input; value for %s command must be a positive real number.\n", argv[i]);
                        printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
                        exit(EXIT_FAILURE);
                    }
                } else {
                    fprintf(stderr, "Error: Invalid input; value for %s command must be a positive real number.\n", argv[i]);
                    printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
                    exit(EXIT_FAILURE);
                }
            } else {
                fprintf(stderr, "Error: Missing argument; a positive real number must be supplied to %s command.\n", argv[i]);
                printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(argv[i], "-mu") == 0) {
            if (i+1 < argc) {
                if (argv[i+1][0] != '-') {
                    if (!sscanf(argv[i+1], "%lf", &mu_packetpersec)) {
                        fprintf(stderr, "Error: Invalid input; value for %s command must be a positive real number.\n", argv[i]);
                        printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
                        exit(EXIT_FAILURE);
                    }
                } else {
                    fprintf(stderr, "Error: Invalid input; value for %s command must be a positive real number.\n", argv[i]);
                    printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
                    exit(EXIT_FAILURE);
                }
            } else {
                fprintf(stderr, "Error: Missing argument; a positive real number must be supplied to %s command.\n", argv[i]);
                printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(argv[i], "-r") == 0) {
            if (i+1 < argc) {
                if (argv[i+1][0] != '-') {
                    if (!sscanf(argv[i+1], "%lf", &r_tokenpersec)) {
                        fprintf(stderr, "Error: Invalid input; value for %s command must be a positive real number.\n", argv[i]);
                        printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
                        exit(EXIT_FAILURE);
                    }
                } else {
                    fprintf(stderr, "Error: Invalid input; value for %s command must be a positive real number.\n", argv[i]);
                    printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
                    exit(EXIT_FAILURE);
                }
            } else {
                fprintf(stderr, "Error: Missing argument; a positive real number must be supplied to %s command.\n", argv[i]);
                printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(argv[i], "-B") == 0) {
            if (i+1 < argc) {
                if (argv[i+1][0] != '-') {
                    if (sscanf(argv[i+1], "%d", &B)) {
                        if (B > MAX_TOKEN_VALUE) {
                            fprintf(stderr, "Error: Invalid input; value for %s command must be a positive whole number with max value of 2147483647.\n", argv[i]);
                            printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
                            exit(EXIT_FAILURE);
                        }
                    } else {
                        fprintf(stderr, "Error: Invalid input; value for %s command must be a positive whole number.\n", argv[i]);
                        printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
                        exit(EXIT_FAILURE);
                    }
                } else {
                    fprintf(stderr, "Error: Invalid input; value for %s command must be a positive whole number.\n", argv[i]);
                    printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
                    exit(EXIT_FAILURE);
                }
            } else {
                fprintf(stderr, "Error: Missing argument; a positive whole number must be supplied to %s command.\n", argv[i]);
                printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(argv[i], "-P") == 0) {
            if (i+1 < argc) {
                if (argv[i+1][0] != '-') {
                    if (sscanf(argv[i+1], "%d", &P)) {
                        if (P > MAX_TOKEN_VALUE) {
                            fprintf(stderr, "Error: Invalid input; value for %s command must be a positive whole number with max value of 2147483647.\n", argv[i]);
                            printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
                            exit(EXIT_FAILURE);
                        }
                    } else {
                        fprintf(stderr, "Error: Invalid input; value for %s command must be a positive whole number.\n", argv[i]);
                        printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
                        exit(EXIT_FAILURE);
                    }
                } else {
                    fprintf(stderr, "Error: Invalid input; value for %s command must be a positive whole number.\n", argv[i]);
                    printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
                    exit(EXIT_FAILURE);
                }
            } else {
                fprintf(stderr, "Error: Missing argument; a positive whole number must be supplied to %s command.\n", argv[i]);
                printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(argv[i], "-n") == 0) {
            if (i+1 < argc) {
                if (argv[i+1][0] != '-') {
                    if (sscanf(argv[i+1], "%d", &num)) {
                        if (num > MAX_TOKEN_VALUE) {
                            fprintf(stderr, "Error: Invalid input; value for %s command must be a positive whole number with max value of 2147483647.\n", argv[i]);
                            printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
                            exit(EXIT_FAILURE);
                        }
                    } else {
                        fprintf(stderr, "Error: Invalid input; value for %s command must be a positive whole number.\n", argv[i]);
                        printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
                        exit(EXIT_FAILURE);
                    }
                }
            } else {
                fprintf(stderr, "Error: Missing argument; a positive whole number must be supplied to %s command.\n", argv[i]);
                printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(argv[i], "-t") == 0) {
            if (i+1 < argc) {
                tsfile=argv[i+1];
            } else {
                fprintf(stderr, "Error: Missing argument; a tsfile must be supplied to %s command.\n", argv[i]);
                printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
                exit(EXIT_FAILURE);
            }
        } else {
            fprintf(stderr, "Error: Malformed command; \"%s\" is an invalid command.\n", argv[i]);
            printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
            exit(EXIT_FAILURE);
        }
    }
}

void CheckFile(char* filename) {
    if (!fp) {
        fprintf(stderr, "Error: cannot open \"%s\" for reading.\n", filename);
        printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
        exit(EXIT_FAILURE);
    }
    struct stat sb;
    stat(filename, &sb);
    if (S_ISDIR(sb.st_mode)) {
        fprintf(stderr, "Error: \"%s\" is a directory.\n", filename);
        printf("Usage: warmup2 [-lambda lambda] [-mu mu] [-r r] [-B B] [-P P] [-n num] [-t tsfile]\n");
        exit(EXIT_FAILURE);
    }    
}

/* -- THREADS -- */

void* Monitor() {
    int sig;
    while (1) {
        sigwait(&set, &sig);
        pthread_mutex_lock(&mutex);
        timetoquit=TRUE;
        pthread_cancel(packetarrivalthread);
        pthread_cancel(tokenthread);
        SetTimeElapsed(&timeelapsed);
        printf("\n%012.3fms: SIGINT caught, no new packets or tokens will be allowed\n", TimevalToDouble_msec(timeelapsed));
        pthread_cond_broadcast(&cv);

        while (!My402ListEmpty(&Q1)) {
            Packet* removedpacket=(Packet*)My402ListFirst(&Q1)->obj;
            My402ListUnlink(&Q1, My402ListFirst(&Q1));
            SetTimeElapsed(&timeelapsed);
            printf("%012.3fms: p%d removed from Q1\n", TimevalToDouble_msec(timeelapsed), removedpacket->id);
        }

        while (!My402ListEmpty(&Q2)) {
            Packet* removedpacket=(Packet*)My402ListFirst(&Q2)->obj;
            My402ListUnlink(&Q2, My402ListFirst(&Q2));
            SetTimeElapsed(&timeelapsed);
            printf("%012.3fms: p%d removed from Q2\n", TimevalToDouble_msec(timeelapsed), removedpacket->id);
        }
        
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}

void* ExecutePacketThread(void* nothing) {
    int sleeptime_msec, servicetime_msec;
    while (packetarrivalcount < num) {
        if (tsfile) {
            if (ReadNextLine()) {
                char* field=strtok(currentlineinfile, " \t");

                if (!TrimWhitespace(field)) {
                    fprintf(stderr, "Error: given tsfile contains an empty field on line %d.\n", 1);
                    exit(EXIT_FAILURE);
                }

                if (!IsNumeric(field)) {
                    fprintf(stderr, "Error: non-numeric value supplied for sleep time on line %d.\n", linenumber);
                    exit(EXIT_FAILURE);
                }

                sleeptime_msec=atoi(field);
                
                if (sleeptime_msec < 0) {
                    fprintf(stderr, "Error: negative value supplied for sleep time on line %d.\n", linenumber);
                    exit(EXIT_FAILURE);
                }

                field=strtok(NULL, " \t");

                if (!TrimWhitespace(field)) {
                    fprintf(stderr, "Error: given tsfile contains an empty field on line %d.\n", 1);
                    exit(EXIT_FAILURE);
                }

                if (!IsNumeric(field)) {
                    fprintf(stderr, "Error: non-numeric value supplied for P on line %d.\n", linenumber);
                    exit(EXIT_FAILURE);
                }

                P=atoi(field);

                if (P < 0) {
                    fprintf(stderr, "Error: negative value supplied for P on line %d.\n", linenumber);
                    exit(EXIT_FAILURE);
                }

                field=strtok(NULL, " \t");

                if (!TrimWhitespace(field)) {
                    fprintf(stderr, "Error: given tsfile contains an empty field on line %d.\n", 1);
                    exit(EXIT_FAILURE);
                }

                if (!IsNumeric(field)) {
                    fprintf(stderr, "Error: non-numeric value supplied for service time on line %d.\n", linenumber);
                    exit(EXIT_FAILURE);
                }

                servicetime_msec=atoi(field);

                if (servicetime_msec < 0) {
                    fprintf(stderr, "Error: negative value supplied for service time on line %d.\n", linenumber);
                    exit(EXIT_FAILURE);
                }
                
                packetarrivalcount=linenumber-1;
            }
        } else {
            sleeptime_msec=(int)min(1000.0/lambda_packetpersec, 1000.0*MAX_SLEEP_TIME_sec);
            servicetime_msec=(int)min(1000.0/mu_packetpersec, 1000.0*MAX_SLEEP_TIME_sec);
            packetarrivalcount++;
        }

        Packet* packet=(Packet*)malloc(sizeof(Packet));

        if (!packet) {
            fprintf(stderr, "Error: memory for packet %d could not be allocated.\n", packetarrivalcount);
            exit(EXIT_FAILURE);
        }

        packet->id=packetarrivalcount;
        packet->tokensneeded=P;
        packet->servicetime_usec=1000.0*servicetime_msec;

        if (sleeptime_msec > 0) usleep(1000.0*sleeptime_msec);

        pthread_mutex_lock(&mutex);        

        SetTimeElapsed(&packet->arrivaltime);
        timersub(&packet->arrivaltime, &previouspacketarrivaltime, &packet->interarrivaltime);
        memcpy(&previouspacketarrivaltime, &packet->arrivaltime, sizeof(struct timeval));

        if (packet->tokensneeded <= B) {
            printf("%012.3fms: p%d arrives, needs %d token%s, inter-arrival time = %.6gms\n", TimevalToDouble_msec(packet->arrivaltime), packet->id, packet->tokensneeded, packet->tokensneeded != 1 ? "s" : "", TimevalToDouble_msec(packet->interarrivaltime));
            My402ListAppend(&Q1, packet);
            SetTimeElapsed(&packet->q1enqueuetime);
            printf("%012.3fms: p%d enters Q1\n", TimevalToDouble_msec(packet->q1enqueuetime), packet->id);
        } else {
            packetdropcount++;
            packetcompletioncount++;
            printf("%012.3fms: p%d arrives, needs %d token%s, inter-arrival time = %.6gms, dropped\n",  TimevalToDouble_msec(packet->arrivaltime), packet->id, packet->tokensneeded, packet->tokensneeded != 1 ? "s" : "", TimevalToDouble_msec(packet->interarrivaltime));
        }
        
        timetoquit=packetcompletioncount >= num;

        pthread_cond_broadcast(&cv);
        pthread_mutex_unlock(&mutex);
    } 
    pthread_exit(NULL);
}

void* ExecuteTokenThread(void* nothing) {
    while (!timetoquit) {
        int sleeptime_msec=min(1000.0/r_tokenpersec, 1000.0*MAX_SLEEP_TIME_sec);

        if (sleeptime_msec > 0) usleep(1000.0*sleeptime_msec);
        
        pthread_mutex_lock(&mutex);

        tokencount++;

        if (tokenbucket < B)  {
            tokenbucket++;
            SetTimeElapsed(&timeelapsed);
            printf("%012.3fms: token t%d arrives, token bucket now has %d token%s\n",  TimevalToDouble_msec(timeelapsed), tokencount, tokenbucket, tokenbucket != 1 ? "s" : "");
        } else {
            tokendropcount++;
            SetTimeElapsed(&timeelapsed);
            printf("%012.3fms: token t%d arrives, dropped\n", TimevalToDouble_msec(timeelapsed), tokencount);
        }

        while (!My402ListEmpty(&Q1)) {
            My402ListElem* topelement=My402ListFirst(&Q1);
            Packet* packet=(Packet*)topelement->obj;
            if (tokenbucket >= packet->tokensneeded) {
                My402ListUnlink(&Q1, topelement);
                SetTimeElapsed(&packet->q1dequeuetime);
                tokenbucket-=packet->tokensneeded;
                timersub(&packet->q1dequeuetime, &packet->q1enqueuetime, &packet->timeinq1); // Store total time in Q1
                printf("%012.3fms: p%d leaves Q1, time in Q1 = %.6gms, token bucket now has %d token%s\n", TimevalToDouble_msec(packet->q1dequeuetime), packet->id, TimevalToDouble_msec(packet->timeinq1), tokenbucket, tokenbucket != 1 ? "s" : "");
                My402ListAppend(&Q2, packet);
                SetTimeElapsed(&packet->q2enqueuetime);
                printf("%012.3fms: p%d enters Q2\n", TimevalToDouble_msec(packet->q2enqueuetime), packet->id);
                packetcompletioncount++;
                pthread_cond_broadcast(&cv);
            } else {
                pthread_cond_broadcast(&cv);
                break;
            }
        }
        timetoquit=packetcompletioncount >= num;
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void* ExecuteServerThread(void* serverid) {
    for (;;) {
        Packet* packet;

        pthread_mutex_lock(&mutex);

        while (My402ListEmpty(&Q2) && !timetoquit) pthread_cond_wait(&cv, &mutex);
                
        if (!My402ListEmpty(&Q2)){ 
            My402ListElem* topelement=My402ListFirst(&Q2);
            packet=(Packet*)topelement->obj;
            My402ListUnlink(&Q2, topelement);
            SetTimeElapsed(&packet->q2dequeuetime);

            timersub(&packet->q2dequeuetime, &packet->q2enqueuetime, &packet->timeinq2);
            
            printf("%012.3fms: p%d leaves Q2, time in Q2 = %.6gms\n", TimevalToDouble_msec(packet->q2dequeuetime), packet->id, TimevalToDouble_msec(packet->timeinq2));
            SetTimeElapsed(&packet->servicestarttime);
            printf("%012.3fms: p%d begins service at S%u, requesting %.6gms of service\n", TimevalToDouble_msec(packet->servicestarttime), packet->id, (uintptr_t)serverid, packet->servicetime_usec/1000.0);

            pthread_mutex_unlock(&mutex);

            int sleeptime_usec=packet->servicetime_usec;

            if (sleeptime_usec > 0) usleep(sleeptime_usec);

            pthread_mutex_lock(&mutex);

            SetTimeElapsed(&packet->serviceendtime);

            timersub(&packet->serviceendtime, &packet->servicestarttime, &packet->timeinservice);
            timersub(&packet->serviceendtime, &packet->arrivaltime, &packet->totaltransmissiontime);
            printf("%012.3fms: p%d departs from S%u, service time = %.6gms, time in system = %.6gms\n", TimevalToDouble_msec(packet->serviceendtime), packet->id, (uintptr_t)serverid, TimevalToDouble_msec(packet->timeinservice), TimevalToDouble_msec(packet->totaltransmissiontime));
            
            packettransmissioncount++;

            timeradd(&interarrivaltimesum, &packet->interarrivaltime, &interarrivaltimesum);
            timeradd(&q1timesum, &packet->timeinq1, &q1timesum);
            timeradd(&q2timesum, &packet->timeinq2, &q2timesum);
            timeradd(&servicetimesum, &packet->timeinservice, &servicetimesum);
            timeradd(&timeinsystemsum, &packet->totaltransmissiontime, &timeinsystemsum);
            timeinsystemsumsquared+=pow(TimevalToDouble_msec(packet->totaltransmissiontime), 2);
            if ((uintptr_t)serverid == 1) timeradd(&s1timesum, &packet->timeinservice, &s1timesum);
            if ((uintptr_t)serverid == 2) timeradd(&s2timesum, &packet->timeinservice, &s2timesum);

            pthread_mutex_unlock(&mutex);
        } else {
            pthread_mutex_unlock(&mutex);
            break;
        }
    }
    pthread_exit(NULL);
}

/* -- PROCESSES -- */

void PrintParams(char* tsfilename) {
    printf("Emulation Parameters:\n");
    printf("\tnumber to arrive = %d\n", num);
    if (!tsfilename) printf("\tlambda = %d\n\tmu = %.6g\n", lambda_packetpersec, mu_packetpersec);
    printf("\tr = %.6g\n", r_tokenpersec);
    printf("\tB = %d\n", B);
    if (!tsfilename) printf("\tP = %d\n", P);
    if (tsfilename) printf("\ttsfile = %s\n", tsfilename);
    printf("\n");
}

void PrintStatistics() {
    double starttoendtime=TimevalToDouble_msec(timeelapsed);
    printf("\nStatistics:\n");
    printf("\taverage packet inter-arrival time = %.6g\n", (packettransmissioncount > 0 ? TimevalToDouble_msec(interarrivaltimesum)/(double)packettransmissioncount : 0.0)/1000.0);
    printf("\taverage packet service time = %.6g\n",  (packettransmissioncount > 0 ? TimevalToDouble_msec(servicetimesum)/(double)packettransmissioncount : 0.0)/1000.0);
    printf("\taverage number of packets in Q1 = %.6g\n", (starttoendtime > 0 ? TimevalToDouble_msec(q1timesum)/starttoendtime : 0.0));
    printf("\taverage number of packets in Q2 = %.6g\n", (starttoendtime > 0 ? TimevalToDouble_msec(q2timesum)/starttoendtime : 0.0));
    printf("\taverage number of packets at S1 = %.6g\n", (starttoendtime > 0? TimevalToDouble_msec(s1timesum)/starttoendtime : 0.0));
    printf("\taverage number of packets at S2 = %.6g\n", (starttoendtime > 0? TimevalToDouble_msec(s2timesum)/starttoendtime : 0.0));
    printf("\taverage time a packet spent in system = %.6g\n", TimevalToDouble_msec(timeinsystemsum)/1000.0);
    printf("\tstandard deviation for time spent in system = %.6g\n", sqrt((timeinsystemsumsquared/(double)packettransmissioncount)-pow(TimevalToDouble_msec(timeinsystemsum)/(double)packettransmissioncount, 2))/1000.0);
    printf("\ttoken drop probability = %.6g\n", (tokendropcount > 0 ? (double)tokendropcount/(double)tokencount : 0.0));
    printf("\tpacket drop probability = %.6g\n", (packetdropcount > 0? (double)packetdropcount/(double)num : 0.0));
}

void Emulate(char* tsfile) {
    if (tsfile) {
        fp=fopen(tsfile, "r");
        CheckFile(tsfile);
        if (ReadNextLine()) {
            if (strcmp(TrimWhitespace(currentlineinfile), "") == 0) {
                fprintf(stderr, "Error: given tsfile contains an empty field on line %d.\n", linenumber);
                exit(EXIT_FAILURE);
            }
            if (sscanf(currentlineinfile, "%d", &num) != 1) {
                fprintf(stderr, "Error: given tsfile does not contain a whole number on line %d.\n", linenumber);
                exit(EXIT_FAILURE);
            }
        };
    } else {
        if (lambda_packetpersec < 1) lambda_packetpersec=1;
		if (mu_packetpersec < 0.1) mu_packetpersec=0.1;
		if (r_tokenpersec < 0.1) r_tokenpersec=0.1;
    }

    PrintParams(tsfile);

    memset(&Q1, 0, sizeof(My402List));
    memset(&Q2, 0, sizeof(My402List));

    if (My402ListInit(&Q1) != 1 || My402ListInit(&Q2) != 1) { 
        fprintf(stderr, "Error: Q%c initialization failed.\n", My402ListInit(&Q1) != 1 ? '1' : '2');
        exit(EXIT_FAILURE);
    }
    
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigprocmask(SIG_BLOCK, &set, 0);

    printf("%012.3fms: emulation begins\n", 0.0);

    gettimeofday(&starttime, NULL);

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cv, NULL);
    pthread_create(&ctrlcthread, 0, Monitor , NULL);
    pthread_create(&serverthread1, 0, ExecuteServerThread, (int*)1);
    pthread_create(&serverthread2, 0, ExecuteServerThread, (int*)2);
    pthread_create(&packetarrivalthread, 0, ExecutePacketThread, (int*)1);
    pthread_create(&tokenthread, 0, ExecuteTokenThread, (int*)2);
    pthread_join(packetarrivalthread, NULL);
    pthread_join(tokenthread, NULL);
    pthread_join(serverthread1, NULL);
    pthread_join(serverthread2, NULL);
    pthread_cancel(ctrlcthread);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cv);

    if (tsfile) fclose(fp);

    My402ListUnlinkAll(&Q1);
    My402ListUnlinkAll(&Q2);
    
    SetTimeElapsed(&timeelapsed);
    printf("%012.3fms: emulation ends\n", TimevalToDouble_msec(timeelapsed));
    timersub(&timeelapsed, &starttime, &totalemulationtime); 
    PrintStatistics();
}

int main(int argc, char *argv[]) { 
    CheckCommands(argc, argv);
    Emulate(tsfile);
    return 1;
}
