#include <iostream>
#include <string.h>
using namespace std;
#include "mpi.h"

#define MAXSMSG 100
#define NONE -1 //static const NONE for chopstick allocation

enum SyncMessageType { 
  QUIT, ACK, PRINT, ECHO, GETCHOP, PUTCHOP, GETCHOP_BLOCKING
};

class SyncServer {
public:
  static int run() {
    char buff[MAXSMSG+1];
    int count;
    MPI_Status status;  // holds status info from most recent MPI call
    int p; // Keep track of which philosopher / process is running
    int nprocs;

    int chop[MPI_Comm_size(MPI_COMM_WORLD, &nprocs) - 1]; // Allocating #philosophers amount of chopsticks (also setting count to #procs)
    for (int i = 0; i < count-1; i++){ chop[i] = NONE; }

    int waiting[nprocs - 1]; // Allocating #philosophers amount of waiting slots
    for (int i = 0; i < count-1; i++){ waiting[i] = NONE; }

    while (1) { 
      MPI_Recv(buff, MAXSMSG, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      MPI_Get_count(&status, MPI_CHAR, &count);

      buff[count] = '\0';

      switch ((enum SyncMessageType) status.MPI_TAG) { // Switch through different message types

        case QUIT: {
          char msg[] = "bye";
          MPI_Send(msg, strlen(msg), MPI_CHAR, status.MPI_SOURCE, ACK, MPI_COMM_WORLD);
          cout << "Shutting down MPI_COMM_WORLD" << endl;
          MPI_Abort(MPI_COMM_WORLD, MPI_SUCCESS);
          return 0;
          break;
        }

        // Adding new case: ECHO
        case ECHO: {
          // Send buffer (the content just received) back to the source.
          MPI_Send(buff, strlen(buff), MPI_CHAR, status.MPI_SOURCE, ACK, MPI_COMM_WORLD);
          break;
        }

        // Adding a new case: GETCHOP
        case GETCHOP: {
          p = status.MPI_SOURCE;

          // cout << "SERVER: philosopher " << p << " sent " << status.MPI_TAG << " with side " << buff[0] << endl;

          // LEFT chopstick requests
          if(buff[0] == 'L' || buff[0] == 'l'){
            if(chop[p] == NONE){ // If left chopstick isn't assigned
              chop[p] = p; // Assign that chopstick to the current process
              MPI_Send("", 0, MPI_CHAR, status.MPI_SOURCE, ACK, MPI_COMM_WORLD);
            }
            else if(chop[p] == p){ // If chopstick is already assigned to that process
              char msg[] = "ERROR";
              MPI_Send(msg, strlen(msg), MPI_CHAR, status.MPI_SOURCE, ACK, MPI_COMM_WORLD);
            }
            else if(chop[p] != NONE){ // If chopstick is assigned to some other process
              char msg[] = "NO";
              MPI_Send(msg, strlen(msg), MPI_CHAR, status.MPI_SOURCE, ACK, MPI_COMM_WORLD);
            }
            else{ // Something has gone horribly wrong
              char msg[] = "BIG ERROR";
              MPI_Send(msg, strlen(msg), MPI_CHAR, status.MPI_SOURCE, ACK, MPI_COMM_WORLD);
            }
          }

          // RIGHT chopstick requests
          else if(buff[0] == 'R' || buff[0] == 'r'){
            if((chop[((p+1)%(nprocs-1))]) == NONE){ // If right chopstick isn't assigned
              chop[((p+1)%(nprocs-1))] = p; // Assign that chopstick to the current process
              MPI_Send("", 0, MPI_CHAR, p, ACK, MPI_COMM_WORLD);
            }
            else if(chop[((p+1)%(nprocs-1))] == p){ // If chopstick is already assigned to that process
              char msg[] = "ERROR";
              MPI_Send(msg, strlen(msg), MPI_CHAR, p, ACK, MPI_COMM_WORLD);
            }
            else if(chop[((p+1)%(nprocs-1))] != NONE){ // If chopstick is assigned to some other process
              char msg[] = "NO";
              MPI_Send(msg, strlen(msg), MPI_CHAR, p, ACK, MPI_COMM_WORLD);
            }
            else{ // Something has gone horribly wrong
              char msg[] = "BIG ERROR";
              MPI_Send(msg, strlen(msg), MPI_CHAR, p, ACK, MPI_COMM_WORLD);
            }
          }

          break;
        }

        // Adding a new case: GETCHOP_BLOCKING
        case GETCHOP_BLOCKING: {
          p = status.MPI_SOURCE;

          // cout << "SERVER: philosopher " << p << " sent " << status.MPI_TAG << " with side " << buff[0] << endl;

          // LEFT chopstick requests
          if(buff[0] == 'L' || buff[0] == 'l'){
            if(chop[p] == NONE){ // If left chopstick isn't assigned
              chop[p] = p; // Assign that chopstick to the current process
              char msg[] = "";
              MPI_Send(msg, strlen(msg), MPI_CHAR, p, ACK, MPI_COMM_WORLD);
            }
            else if(chop[p] == p){ // If chopstick is already assigned to that process
              char msg[] = "ERROR";
              MPI_Send(msg, strlen(msg), MPI_CHAR, status.MPI_SOURCE, ACK, MPI_COMM_WORLD);
            }
            else if(chop[p] != NONE){ // If chopstick is assigned to some other process
              waiting[p] = p; // Assign philosopher to waiting position
              char msg[] = "Blocking";
              MPI_Send(msg, strlen(msg), MPI_CHAR, p, ACK, MPI_COMM_WORLD);
            }
            else{ // Something has gone horribly wrong
              char msg[] = "BIG ERROR";
              MPI_Send(msg, strlen(msg), MPI_CHAR, status.MPI_SOURCE, ACK, MPI_COMM_WORLD);
            }
          }

          // RIGHT chopstick requests
          else if(buff[0] == 'R' || buff[0] == 'r'){
            if((chop[((p+1)%(nprocs-1))]) == NONE){ // If right chopstick isn't assigned
              chop[((p+1)%(nprocs-1))] = p; // Assign that chopstick to the current process
              char msg[] = "";
              MPI_Send(msg, strlen(msg), MPI_CHAR, p, ACK, MPI_COMM_WORLD);
            }
            else if(chop[((p+1)%(nprocs-1))] == p){ // If chopstick is already assigned to that process
              waiting[((p+1)%(nprocs-1))] = p; // Assign philosopher to waiting position
              char msg[] = "Blocking";
              MPI_Send(msg, strlen(msg), MPI_CHAR, p, ACK, MPI_COMM_WORLD);
            }
            else if(chop[((p+1)%(nprocs-1))] != NONE){ // If chopstick is assigned to some other process
              char msg[] = "NO";
              MPI_Send(msg, strlen(msg), MPI_CHAR, p, ACK, MPI_COMM_WORLD);
            }
            else{ // Something has gone horribly wrong
              char msg[] = "BIG ERROR";
              MPI_Send(msg, strlen(msg), MPI_CHAR, p, ACK, MPI_COMM_WORLD);
            }
          }

          break;
        }

        // Adding a new case: PUTCHOP
        case PUTCHOP: {
          p = status.MPI_SOURCE;

          // cout << "SERVER: philosopher " << p << " sent " << status.MPI_TAG << " with side " << buff[0] << endl;

          // LEFT chopstick requests
          if(buff[0] == 'L' || buff[0] == 'l'){
            if(chop[p] == p){ // If left chopstick is assigned to that process
              if(waiting[p] != NONE){ // If there's a philosopher waiting for its turn
                chop[p] = waiting[p]; // Assign that waiting value to the chopstick
                char msg[] = "";
                MPI_Send(msg, strlen(msg), MPI_CHAR, p, ACK, MPI_COMM_WORLD); // Send an empty ACK message to the waiting philosopher
                waiting[p] = NONE; // Reset waiting value
              }
              else{
                chop[p] = NONE; // Assign NONE to that chopstick
              }
              char msg[] = "";
              MPI_Send(msg, strlen(msg), MPI_CHAR, p, ACK, MPI_COMM_WORLD); // Send an empty ACK message to philosopher putting chopstick down
            
            }
            else{ // Something else
              char msg[] = "ERROR";
              MPI_Send(msg, strlen(msg), MPI_CHAR, p, ACK, MPI_COMM_WORLD);
            }
          }

          // RIGHT chopstick requests
          else if(buff[0] == 'R' || buff[0] == 'r'){
            if((chop[((p+1)%(nprocs-1))]) == p){ // If right chopstick is assigned to that process
              if((waiting[((p+1)%(nprocs-1))]) != NONE){ // If there's a philosopher waiting for its turn
                chop[((p+1)%(nprocs-1))] = waiting[((p+1)%(nprocs-1))]; // Assign that waiting value to the chopstick
                char msg[] = "";
                MPI_Send(msg, strlen(msg), MPI_CHAR, waiting[((p+1)%(nprocs-1))], ACK, MPI_COMM_WORLD); // Send an empty ACK message to the waiting philosopher
                waiting[((p+1)%(nprocs-1))] = NONE; // Reset waiting value
              }
              else{
                chop[((p+1)%(nprocs-1))] = NONE; // Assign NONE to that chopstick
              }

              MPI_Send("", 0, MPI_CHAR, p, ACK, MPI_COMM_WORLD); // Send an empty ACK message to philosopher putting chopstick down
              
            }
            else{ // Something eslse
              char msg[] = "ERROR";
              MPI_Send(msg, strlen(msg), MPI_CHAR, p, ACK, MPI_COMM_WORLD);
            }
          }

          break;
        }

        case PRINT: {
          cout << buff << endl;
        }

        char empty[1];  empty[0] = '\0';
        MPI_Send(empty, 0, MPI_CHAR, status.MPI_SOURCE, ACK, MPI_COMM_WORLD);
        break;
        
        default: {
          char msg[] = "Unrecognized type ";
          cout << msg << status.MPI_TAG << endl;
          MPI_Send(msg, strlen(msg), MPI_CHAR, status.MPI_SOURCE, ACK, MPI_COMM_WORLD);
        }
      }
    }
  }
};
  
class Sync {
protected:
  int server;  // rank of server process

public:
  Sync(int server_rank) { 
    server = server_rank;
  }

  void send(enum SyncMessageType type, const char * msg) const {
    // cout << "send" << endl;
    int len = strlen(msg);
    if (len > MAXSMSG)
      len = MAXSMSG;
    MPI_Send((void *) msg, len, MPI_CHAR, server, type, MPI_COMM_WORLD); 
  }

  void send(enum SyncMessageType type) const {
    // cout << "send" << endl;
    char empty[1];  empty[0] = '\0';
    MPI_Send(empty, 0, MPI_CHAR, server, type, MPI_COMM_WORLD); 
  }

  // returns a dynamically allocated string, which must be deallocated
  char * recv(enum SyncMessageType type) {
    // cout << "recv" << endl;
    MPI_Status status;  // holds status info from most recent MPI call
    int count;
    char * buff = new char[MAXSMSG+1];
    MPI_Recv(buff, MAXSMSG, MPI_CHAR, server, type, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_CHAR, &count);
    buff[count] = '\0';
    return buff;
  }
};

