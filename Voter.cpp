#include "Voter.hpp"

#define NUM_DATA 1000

Voter::Voter(char* argv){
    string input = string(argv);
    istringstream ss(input);
    string num;
    getline(ss, num, ',');
    this->num_linear_classifiers = atoi(num.c_str());
    std::getline(ss, num, '\n');
    pipefd =  atoi(num.c_str());
    // cout<<num_linear_classifiers<<" "<<pipefd <<endl; 
    start = 0;
}

void Voter::get_result_from_linears(){
    for(int i=0;i<1000;i++){

    }
    fstream file(FIFO_NAMED_PATH);
    int cnt=0;
    while(true){
        if (cnt == num_linear_classifiers){
            break;
        }
        string train_name;
        string result;
        getline(file,train_name,',');
        // cout<<train_name<<endl;
        getline(file,result,'$');
        // cout<<result<<endl<<"***********************************"<<endl;
        linear_result.push_back(result);
        cnt+=1;
    }
    file.close();
}

int Voter::find_element_with_highest_frequency(vector<int> v){
    unordered_map<int, int> hash; 
    for (int i = 0; i < v.size(); i++) {
        // cout<<"hash[v[i]"<<v[i]<<" "<<hash[v[i]]<<endl;
        hash[v[i]]++; 
    }
    int max_count = 0, res = -1; 
    for (auto i : hash) { 
        if (max_count < i.second) { 
            res = i.first; 
            max_count = i.second; 
        } 
        else if(max_count==i.second){
            if (i.first<res){
                res = i.first;
            }
        }
    } 
    // cout<<"res:"<<res<<endl;
    return res;
}

void Voter::decide_class_for_each_sample(){
    int i = 0;
    int index_data = 0;
    while(i<linear_result[0].size()){
        vector<int> prediction_class_type_for_sample;
        for (int j=0 ;j<linear_result.size();j++){
            string n ;
            n+= linear_result[j][i];
            int x = atoi(n.c_str());
            prediction_class_type_for_sample.push_back(x);
        }
        sort(prediction_class_type_for_sample.begin(),prediction_class_type_for_sample.end());
        int class_type = find_element_with_highest_frequency(prediction_class_type_for_sample);
        // judge = judge + to_string(index_data+1) + ":";
        judge.append(to_string(class_type));
        i+=2;
        index_data+=1;
        judge+=",";  
    }
    judge+="$";
    // cout<<judge<<endl;
  
    this->calc_print_accuracy();
  
}

void Voter::send_judgement_result_to_ensemble(){
    int fd = open(FIFO_RESULT,O_RDWR);
    write(fd,judge.c_str(),judge.size());
    close(fd);
    cout<<"voter send to ensemble his its result"<<endl;
}


void Voter::calc_print_accuracy(){
    double correct = 0;
    ifstream file ("Assets/validation/labels.csv"); 
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
        label_index+=1;
    }

    istringstream s1(base);
    istringstream s2(judge);
    string num1;
    string num2;
    while(getline(s1, num1, ',')){
        getline(s2,num2,',');
        if ( atoi(num1.c_str()) == atoi(num2.c_str()) ){
            correct+=1;
        }
    }
    // cout<<label_index<<endl;
    printf("Accuracy:%.2f\n",correct*100/label_index);
}
