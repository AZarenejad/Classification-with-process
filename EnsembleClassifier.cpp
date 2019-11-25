#include "EnsembleClassifier.hpp"



EnsembleClassifier::EnsembleClassifier(string validation_folder,string weight_vectors_folder){
    this->validation_folder = validation_folder;
    this->weight_vectors_folder = weight_vectors_folder;
    this-> dataset_path = this->validation_folder + "/dataset.csv";
    this->labels_path = this->validation_folder + "/labels.csv";
    this->num_linear_classifier = 0;
}

void EnsembleClassifier::wait_for_linears(){
    for(int i = 0 ; i < linear_classifiers.size() ; i++){
        // cout<<i<<endl;
        // int status;
        cout<<"wait for linear number"<<i<<":"<<wait(NULL)<<endl;
        // cout<<"wait return:"<<wait(&status)<<endl;
        // cout<<status<<endl;
        // cout<<"******"<<endl;
        // waitpid(linear_classifiers[i], NULL, WNOHANG);
    }
}

void EnsembleClassifier::wait_for_voter(){
    cout<<"wait for voter:"<<wait(NULL)<<endl;
}


void EnsembleClassifier::open_weight_vectors_directory_set_file_names(){
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (weight_vectors_folder.c_str())) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
        string fileName;
        fileName = ent -> d_name;
        size_t found;
        found = fileName.find(".csv");
        if (found != string::npos){ 
            linear_classifier_filenames.push_back(fileName);
            num_linear_classifier++; 
        }  
    }
    closedir (dir);
    } else {
        cerr << "could not open weight_vectors directory" << endl;
    } 
}

void EnsembleClassifier::create_linear_classifiers_pipes(){
    for(int i = 0 ; i < num_linear_classifier ; i++){
        int fd[2];
        if(pipe(fd) < 0 ){
            cout << "can not create pipe " << i << endl ;
            return ;
        }
        vector < int > fds;
        fds.push_back(fd[0]);
        fds.push_back(fd[1]);
        linear_classifier_pipes.push_back(fds);
    }

    int fd[2];
    if(pipe(fd) < 0 ){
        cout << "can not create pipe for voter\n"<<endl ;
        return ;
    }
    voter_pipe.push_back(fd[0]);
    voter_pipe.push_back(fd[1]);
}

void EnsembleClassifier::create_name_pipe(){
    mkfifo(FIFO_NAMED_PATH, 0666);
    mkfifo(FIFO_RESULT, 0666);
}

void EnsembleClassifier::fork_linear_classifiers(){
    for( int i = 0 ; i < num_linear_classifier ; i++){
        pid_t pid = fork();
        if(pid < 0){
            cout << "can not fork" << endl;
        }
        else if(pid == 0){
            close(linear_classifier_pipes[i][WRITE]);
            char * argv[3];
            argv[0] = (char*) LINEAR_CLASSIFIER_EXEC_PATH;
            argv[1] = (char*) to_string(linear_classifier_pipes[i][READ]).c_str();
            argv[2] = NULL;
            execv(argv[0], argv);
        }
        else{
            close(linear_classifier_pipes[i][READ]);
            linear_classifiers.push_back(pid);
            // wait(NULL);
        }
    }
    // this->wait_for_linears();
}

void EnsembleClassifier::fork_voter(){
    pid_t pid = fork();
    if(pid < 0)
        cerr << "unable to fork voter" << endl;
    else if(pid == 0){
        close(voter_pipe[WRITE]);
        char * argv[3];
        argv[0] = (char*) VOTER_EXEC_PATH;
        string send = to_string(num_linear_classifier) + "," + to_string(voter_pipe[READ]);
        argv[1] = (char*)send.c_str();
        argv[2] = NULL;
        execv(argv[0],argv);
    }
    else if(pid > 0){
        this->voter = pid;
    }
}

void EnsembleClassifier::write_on_classifiers_pipes_their_proper_filenames(){
    for(int i = 0 ; i < num_linear_classifier; i++){
        string send_str = this-> weight_vectors_folder + "/" + 
                linear_classifier_filenames[i] + "," + this->dataset_path;
        char* buff = (char*)send_str.c_str();
        write(linear_classifier_pipes[i][WRITE], buff,strlen(buff));
        close(linear_classifier_pipes[i][WRITE]);
    }
}

void EnsembleClassifier::write_for_voter_start(){
    string send = "1";
    char* buff = (char*)send.c_str();
    write(voter_pipe[WRITE], buff,strlen(buff));
    close(voter_pipe[WRITE]);
}


void EnsembleClassifier::get_string_from_voter(){
    int fd = open(FIFO_RESULT,O_RDWR);
    char buff[2000];
    int size = read(fd,buff,sizeof(buff));
    buff[size] ='\0';
    cout<<buff<<endl;
    // fstream file(FIFO_RESULT);
    // string res;
    // getline(file,res,'$');
    // cout<<res<<endl;
    this->voter_result = string(buff);
}


void EnsembleClassifier::calc_print_accuracy(){
    double correct = 0;
    ifstream file (this->labels_path); 
    string title;
    string class_number;
    int res_index = 0;
    int label_index = 0;
    // cout<<voter_result<<endl;
    string base;
    string index;
    getline(file,title,'\n');
    while(getline(file,index,'\n')){
        base = base + index + ",";
    }

    istringstream s1(base);
    istringstream s2(voter_result);
    string num1;
    string num2;
    while(getline(s1, num1, ',')){
        getline(s2,num2,',');
        if ( atoi(num1.c_str()) == atoi(num2.c_str()) ){
            correct+=1;
        }
        label_index+=1;
    }
    printf("Accuracy:%.2f\n",correct*100/label_index);
}
