
// for Json::value
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>
#include <string>
#include <string.h>
#include <fstream>
// for JsonRPCCPP
#include <iostream>
#include "leaderboard_server.h"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <stdio.h>
#include <stdlib.h>


using namespace jsonrpc;
using namespace std;

class MyLeaderboardServer : public leaderboard_server
{
public:
  MyLeaderboardServer(AbstractServerConnector &connector, serverVersion_t type);
  virtual Json::Value submit(const std::string& submission);
};

MyLeaderboardServer::MyLeaderboardServer(AbstractServerConnector &connector, serverVersion_t type)
  : leaderboard_server(connector, type)
{
  std::cout << "Leaderboard Server created" << std::endl;
}

double choose2( int n   )
{
    if (n < 2) return 0;
    return (double) n*(n-1)/2;
}


// member function

Json::Value
MyLeaderboardServer::submit
(const std::string& submission)
{




  //std::cout << action << " " << " " << vsID << std::endl;
  
  FILE *jf = fopen("record.json","r");

  if (jf == NULL) return -1;

  int i, j;
  long lSize;
  size_t lresult;

  // obtain file size:
  fseek(jf, 0 , SEEK_END);
  lSize = ftell(jf);
  rewind(jf);

  // allocate memory to contain the whole file:
  char *jf_ptr = (char *) malloc(sizeof(char)*lSize);
  // copy the file into the buffer:

  lresult = fread(jf_ptr, 1, lSize, jf);    
  if (lresult != lSize)
    {
      fputs("Reading error", stderr);
      exit (-3);
    }

  fclose( jf );
  
  Json::Value myv_message;
  Json::CharReaderBuilder builder;
  Json::CharReader* reader = builder.newCharReader();
  std::string errors;
  bool parsingSuccessful = reader->parse
    (jf_ptr, jf_ptr + lSize, &myv_message, &errors);
  delete reader;
  free(jf_ptr);

  if (!parsingSuccessful) {
    std::cout << "Failed to parse the content JSON, errors:" << std::endl;
    std::cout << errors << std::endl;
  }
  
  jf = fopen("key.json","r");

  // obtain file size:
  fseek(jf, 0 , SEEK_END);
  lSize = ftell(jf);
  rewind(jf);

  // allocate memory to contain the whole file:
  jf_ptr = (char *) malloc(sizeof(char)*lSize);
  // copy the file into the buffer:

  lresult = fread(jf_ptr, 1, lSize, jf);    
  if (lresult != lSize)
    {
      fputs("Reading error", stderr);
      exit (-3);
    }

  fclose( jf );
  

  Json::Value key_message;
  reader = builder.newCharReader();
  parsingSuccessful = reader->parse
    (jf_ptr, jf_ptr + lSize, &key_message, &errors);
  delete reader;
  free(jf_ptr);
  if (!parsingSuccessful) {
    std::cout << "Failed to parse the content JSON, errors:" << std::endl;
    std::cout << errors << std::endl;
  }
  


  int Nsize;
  int Nsize_reduced;
  int nclass;
  int max_submission_times = 10;


  if( key_message["size"].isNull()!=true )
  {
    Nsize = key_message["size"].asInt();
  }else{
    std::cout<<"No size specified!" <<std::endl;
    return 1;
  }

  if( key_message["reduced_size"].isNull()!=true )
  {
    Nsize_reduced = key_message["reduced_size"].asInt();
  }else{
    std::cout<<"No reduced size found!" <<std::endl;
    return 1;
  }

  if( key_message["number_of_clusters"].isNull()!=true )
  {

    nclass = key_message["number_of_clusters"].asInt();

  }else{
    std::cout<<"No number_of_clusters!" <<std::endl;
    return 1;
  }

  if( key_message["max_submission_times"].isNull()!=true )
  {

    max_submission_times = key_message["max_submission_times"].asInt();

  }else{
    std::cout<<"No max_submission_times!" <<std::endl;
    return 1;
  }

  vector<int > classes (Nsize) ;//{0, 1, 1, 2, 0, 2, 0, 1, 1, 1, 0, 2, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 2, 2, 0, 1, 2, 1, 1, 1, 0, 2, 2, 0, 0, 1, 0, 1, 0, 1, 0, 0, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 0, 1, 2, 2, 0, 1, 1, 1, 0, 2, 2, 2, 0, 2, 1, 0, 0, 2, 2, 2, 2, 0, 0, 0, 1, 0, 0, 0, 2, 1, 2, 2, 1, 2, 1, 0};
  int labels[Nsize];


  if( key_message["labels"].isNull()!=true )
  {
    Json::Value myarray0 = key_message["labels"];
    if( myarray0.size()!=Nsize){
      cout<< "Size of labels not equal to " << Nsize << "!"<<endl;
      return 1;
    }
    for(Json::Value::ArrayIndex i=0; i!= myarray0.size() ; i++){
    classes[(int)i]= myarray0[i].asInt() ;
  }

  }else{
    std::cout<<"No labels!" <<std::endl;
    return 1;
  }

 

  Json::Value submit_message;
  reader = builder.newCharReader();
  parsingSuccessful = reader->parse(submission.c_str(),
            submission.c_str() + submission.size(),
            &submit_message, &errors);
  delete reader;


  if (!parsingSuccessful) {
    std::cout << "Failed to parse the content of the first JSON, errors:" << std::endl;
    std::cout << errors << std::endl;
    // should not continue
  }


  std::string url;
  std::string vsID;
  int nums; // number of clusters
  double ARI=0.0, ARI_reduced=0.0;

  
  if( submit_message["url"].isNull()!=true )
  {
    url = submit_message["url"].asString();
  }else{
    std::cout<<"No address for Server!" <<std::endl;
    return 1;
  }

  if( submit_message["vsID"].isNull()!=true )
  {
    vsID = submit_message["vsID"].asString();
  }else{
    std::cout<<"No vsID found!" <<std::endl;
    return 1;
  }

  if( submit_message["number_of_clusters"].isNull()!=true )
  {

    nums = submit_message["number_of_clusters"].asInt();

  }else{
    std::cout<<"No number_of_clusters!" <<std::endl;
    return 1;
  }


  if( submit_message["labels"].isNull()!=true )
  {
    Json::Value myarray = submit_message["labels"];
    if( myarray.size()!=Nsize){
      cout<< "Size of labels not equal to " <<Nsize << "!"<<endl;
      return 1;
    }
    for(Json::Value::ArrayIndex i=0; i!= myarray.size() ; i++){
    labels[(int)i]= myarray[i].asInt() ;
  }

  }else{
    std::cout<<"No labels!" <<std::endl;
    return 1;
  }




  int cmat[nclass+1][nums+1];
  int cmat_reduced[nclass+1][nums+1];
  for(i=0;i<=nclass;i++){
    for(j=0;j<=nums;j++){
      cmat[i][j]=0;
      cmat_reduced[i][j]=0;
    }
  }
  int id,jd;

  for(i=0;i<Nsize;i++)
  {
    id=classes[i];
    jd=labels[i];
    cmat[id][jd]++;
  }
  for(i=0;i<nclass;i++){
    for(j=0;j<nums;j++){
      cmat[i][nums]+=cmat[i][j];
      cmat[nclass][j]+=cmat[i][j];
    }
  }

  for(i=0;i<Nsize_reduced;i++)
  {
    id=classes[i];
    jd=labels[i];
    cmat_reduced[id][jd]++;
  }
  for(i=0;i<nclass;i++){
    for(j=0;j<nums;j++){
      cmat_reduced[i][nums]+=cmat_reduced[i][j];
      cmat_reduced[nclass][j]+=cmat_reduced[i][j];
    }
  }


  double temp=0.0,temp1=0.0,temp2=0.0;

  for(i=0;i<nclass;i++){
    for(j=0;j<nums;j++){
      temp+=choose2( cmat[i][j] );
    }
  }
  for(i=0;i<nclass;i++){
    temp1 += choose2( cmat[i][nums] );
  }

  for(j=0;j<nums;j++){
    temp2+= choose2(cmat[nclass][j]);
  }

  ARI = ( temp - temp1*temp2/choose2(Nsize)  )/( 0.5*(temp1+temp2) - temp1*temp2/choose2(Nsize)        ) ;

  temp=0.0;
  temp1=0.0;
  temp2=0.0;

  for(i=0;i<nclass;i++){
    for(j=0;j<nums;j++){
      temp+=choose2( cmat_reduced[i][j] );
    }
  }

  for(i=0;i<nclass;i++){
    temp1 += choose2( cmat_reduced[i][nums] );
  }
  for(j=0;j<nums;j++){
    temp2+= choose2(cmat_reduced[nclass][j]);
  }

  ARI_reduced = ( temp - temp1*temp2/choose2(Nsize_reduced)  )/( 0.5*(temp1+temp2) - temp1*temp2/choose2(Nsize_reduced)        ) ;



  vector<std::string> vsID_vec;
  vector<double > ARI_vec;
  vector<double > ARI_reduced_vec;
  vector<int > submission_times_vec;
  int group_num=myv_message.size();
  for(i=1;i<=group_num;i++){
    Json::Value l_jv = myv_message[ to_string(i)];
    vsID_vec.push_back( l_jv["vsID"].asString()  );
    ARI_vec.push_back( l_jv["ARI"].asDouble() );
    ARI_reduced_vec.push_back(  l_jv["ARI_reduced"].asDouble()   );
    submission_times_vec.push_back( l_jv["submission_times"].asInt() );
  }


  string myid=vsID;
  string subs = myid.substr(1,2);
  int group_index = stoi(subs)-1;
  string group_name = "Group" + to_string(group_index+1);
  submission_times_vec[group_index]+=1;

  if( submission_times_vec[group_index]>max_submission_times){
    submission_times_vec[group_index]=max_submission_times;
    ARI_reduced = ARI_reduced_vec[group_index];
  }

  int rank=1;
  for(i=0;i<group_num;i++){
    if(i!=group_index){
      if(ARI_reduced <ARI_reduced_vec[i]){
        rank++;
      }
    }
  }
  
  ARI_vec[group_index] = ARI;
  ARI_reduced_vec[group_index] = ARI_reduced;

Json::Value * record_ptr = new Json::Value();
for(i=0;i<group_num;i++){
  Json::Value * group_ptr = new Json::Value();
  (*group_ptr)["vsID"]=vsID_vec[i];
  (*group_ptr)["ARI"]=ARI_vec[i];
  (*group_ptr)["ARI_reduced"]=ARI_reduced_vec[i];
  (*group_ptr)["submission_times"]=submission_times_vec[i];
  (*record_ptr)[to_string(i+1)] = *group_ptr;
}

ofstream file_id("record.json");
file_id<< (*record_ptr).toStyledString() <<endl;
file_id.close();

Json::Value *result_ptr = new Json::Value();
(*result_ptr)["name"]=group_name;
(*result_ptr)["vsID"]=vsID;
(*result_ptr)["ARI"]=ARI_reduced;
(*result_ptr)["class_ranking"]=rank;
(*result_ptr)["submission_times"]=submission_times_vec[group_index];

cout<< (*result_ptr).toStyledString() <<endl;

return *result_ptr;
}




int main(int argc, char* argv[]) {
  if(argc<2){
    std::cout<<"missing port number!"<<endl;
    exit(-1);
  }

  long arg1 = strtol(argv[1], NULL, 10);

  HttpServer httpserver( arg1 );
  MyLeaderboardServer s(httpserver,
		JSONRPC_SERVER_V1V2); // hybrid server (json-rpc 1.0 & 2.0)
  s.StartListening();
  std::cout << "Hit enter to stop the server" << endl;
  getchar();

  s.StopListening();
  return 0;
}
