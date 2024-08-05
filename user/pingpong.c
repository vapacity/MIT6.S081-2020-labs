#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char* argv[])
{
    if(argc>1){
        fprintf(2,"No argument is needed!\n");
        exit(1);
    }
    //create two arrays
    //0 read fd from pipe
    //1 write fd to pipe
    int pf2c[2],pc2f[2];//pipe father to child
    pipe(pf2c);
    pipe(pc2f);

    int pid;
    pid = fork();
    //fork returns child pid if father process
    //returns 0 if child process
    if(pid == -1)
    {
        printf("fork failed\n");
        exit(-1);
    }
    if(pid!=0){//father process
        write(pf2c[1],"1",1);//write a byte
        char buf;
        read(pc2f[0],&buf,1);
        printf("%d: received pong\n",getpid());
        wait(0);
    }else{
        char buf;
        read(pf2c[0],&buf,1);
        printf("%d: received ping\n",getpid());
        write(pc2f[1],&buf,1);
    }
    exit(0);


}