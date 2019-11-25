#include "LinearClassifier.hpp"




LinearClassifier :: LinearClassifier(int pipeFd){
    char buff[2048];
    int size = read(pipeFd, buff, 1000);
    buff[size] = '\0';
    string input = string(buff);
    istringstream ss(input);
    string token;
    getline(ss, token, ',');
    this->train_filename = token;
    std::getline(ss, token, ',');
    this->dataset_path =  token;
    close(pipeFd);
 
}



void LinearClassifier::calc_scores(){
    ifstream file (this->dataset_path); 
    string title;
    string length , width ;
    getline (file, title, '\n');
    int index = 0;
    while (getline (file, length, ',' )){
        getline (file , width , '\n');
        calc_score_of_each_sample(stold(length,NULL) , stold(width,NULL) , index);
        index+=1;
    }
    num_sample_data = index;
    if (index!=1000){
        cout<<"error n linear reading dataset"<<endl;
    }
    if (score_samples.size()!=1000){
        cout<<"error in calc class in linear"<<endl;
    }
}

void LinearClassifier::calc_score_of_each_sample(long double length , long double width , int sample_index){
    ifstream file (this->train_filename); 
    string title;
    string w1 , w2 , b;
    long double beta1 , beta2 , bias;
    getline (file, title, '\n'); 
    vector<long double> scores;
    int num_class = 0;
    while (getline(file, w1,',')){
        getline(file, w2,',');
        getline(file, b,'\n');
        num_class+=1;
        beta1 = stold(w1,NULL);
        beta2 = stold(w2,NULL);
        bias = stold(b,NULL);
        long double score = length * beta1 + width * beta2 + bias ;
        scores.push_back(score);
    }
    if (num_class!=3){
        cout<<"erroer in reading train data in linear"<<endl;
    }
    int max_elem_index = max_element( scores.begin(),scores.end()) -  scores.begin();
    // if (train_filename =="Assets/weight_vectors/classifier_1.csv")
    //     cout<<sample_index<<" "<<max_elem_index<<endl;
    score_samples.push_back(max_elem_index);

}


void  LinearClassifier::send_class_of_each_sample_to_voter(){
    string send_to_voter;
    send_to_voter.append(this->train_filename);
    send_to_voter.append(",");
    for (int i=0 ; i < score_samples.size(); i++){
        // send_to_voter.append(to_string(i+1));
        // send_to_voter.append(":");
        send_to_voter.append(to_string(score_samples[i]));
        if (i!=score_samples.size()-1)
            send_to_voter +=",";
    }
    //end charactors
    send_to_voter.append("$");    
    send_to_voter.append("\n");
    // cout<<send_to_voter<<"*******************"<<endl<<endl;
    int fd = open(FIFO_NAMED_PATH, O_WRONLY);
    write(fd,send_to_voter.c_str(),send_to_voter.size());
    close(fd);
}
