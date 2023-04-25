#define MAXCOMLEN 256
#define SERVERCHAR 'S'

#define STOP 1
#define LIST 2
#define _2ALL 3
#define _2ONE 4
#define INIT 5
#define MESSAGE 6


struct msgbuf {
  long mtype;         /* typ komunikatu   */
  int id;                  //id sendera
  int intData;                 //dodatkowa dana typu int
  char charData[MAXCOMLEN];      /* tresc komunikatu */
};



