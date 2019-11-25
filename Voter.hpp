#ifndef Voter_HPP
#define Voter_HPP
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
#include <fstream>
#include <algorithm>
#include <string.h> 
#include <sys/types.h> 
#include <bits/stdc++.h> 
#include <wait.h>


#define FIFO_NAMED_PATH "namedPipe/myfifo"
#define FIFO_RESULT "namedPipe1/result"
using namespace std;






class Voter{
    public:
        Voter(char* argv);
        void get_result_from_linears();
        void decide_class_for_each_sample();
        static int find_element_with_highest_frequency(vector<int> v);
        void send_judgement_result_to_ensemble();
        void calc_print_accuracy();

    private:
        vector<string> linear_result;
        string judge ;
        int num_linear_classifiers;
        int start;
        int pipefd;

};


#endif