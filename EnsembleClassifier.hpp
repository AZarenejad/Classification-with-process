#ifndef EnsembleClassifier_HPP
#define EnsembleClassifier_HPP

#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <wait.h>



#define EXECUTE_LINERA_CLASSIFIER_MESSAGE(i) ( "exceuting linear_classifier " + to_string(i));
#define READ 0
#define WRITE 1
#define LINEAR_CLASSIFIER_EXEC_PATH "bin/LinearClassifier"
#define VOTER_EXEC_PATH "bin/Voter"
#define FIFO_NAMED_PATH "namedPipe/myfifo"
#define FIFO_RESULT "namedPipe1/result"
using namespace std;


class EnsembleClassifier {
    public:
        EnsembleClassifier(string validation_folder, string weight_vectors_folder);
        void wait_for_linears ();
        void open_weight_vectors_directory_set_file_names();
        void create_linear_classifiers_pipes();
        void fork_linear_classifiers();
        void write_on_classifiers_pipes_their_proper_filenames();
        void create_name_pipe();
        void fork_voter();
        void get_string_from_voter();
        void calc_print_accuracy();
        void wait_for_voter();
        void write_for_voter_start();
        pid_t get_voter()
        {
            return voter; 
        }
        
    private:
        string validation_folder;
        string weight_vectors_folder;
        string dataset_path;
        string labels_path;

        int num_linear_classifier;
        vector<string> linear_classifier_filenames;

        // linear_classifiers management
        vector<vector<int> > linear_classifier_pipes;
        vector<int> voter_pipe;
        vector< pid_t > linear_classifiers;
        
        // Presenter management
        pid_t voter;
        string voter_result;
};
#endif
