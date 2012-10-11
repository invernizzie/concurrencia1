#ifndef FORK_H_
    #define FORK_H_
    
    #include <unistd.h>
    #include <stdlib.h>
    #include <errno.h>
    
    class Process {
        public:
            Process() {};
            virtual ~Process() {};
            virtual void run() {};
    };
    
    void fork(Process& p);

#endif /* FORK_H_ */
