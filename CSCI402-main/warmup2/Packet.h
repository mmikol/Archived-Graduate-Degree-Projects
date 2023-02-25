typedef struct Packet { 
    int id; 
    int tokensneeded;
    int servicetime_usec;
    struct timeval arrivaltime;
    struct timeval interarrivaltime;
    struct timeval departuretime;
    struct timeval q1enqueuetime;
    struct timeval q1dequeuetime;
    struct timeval timeinq1;
    struct timeval q2enqueuetime;
    struct timeval q2dequeuetime;
    struct timeval timeinq2;
    struct timeval servicestarttime;
    struct timeval serviceendtime;
    struct timeval timeinservice;
    struct timeval totaltransmissiontime;
} Packet;
