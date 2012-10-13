#ifndef FORK_H_
    #define FORK_H_
    
    #include <unistd.h>
    #include <stdlib.h>
    #include <errno.h>
    
    class Proceso {
        public:
            Proceso() {};
            virtual ~Proceso() {};
            virtual void ejecutar() {};
    };
    
    void fork(Proceso& p);

#endif /* FORK_H_ */
