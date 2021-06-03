#ifndef H_LIBTHRD
#define H_LIBTHRD


void *recup_param_Thread(void *arg); // fonction qui récupere les arguents pour le thread
int lanceThread(void (*fonction) (void*), void *param1, int param2); 
void P(int);
void V(int);

#endif
