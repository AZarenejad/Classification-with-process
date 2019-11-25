#ifndef LinearClassifier_HPP
#define LinearClassifier_HPP

#include <vector>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h> 
#include <fstream>
#include <stdlib.h> 
#include <sys/stat.h> 
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <sstream>
#include <algorithm>
#include <string.h> 
#include <sys/types.h> 



#define FIFO_NAMED_PATH "namedPipe/myfifo"
#define FIFO_RESULT "namedPipe1/result"
using namespace std;





class LinearClassifier {
    public:
        LinearClassifier(int pipeFd);
        void calc_score_of_each_sample(long double length , long double width , int sample_index);
        void calc_scores();
        void send_class_of_each_sample_to_voter();






        
    
    private:
        string train_filename;
        string dataset_path;
        vector <int> score_samples;
        int num_sample_data;

};


#endif
