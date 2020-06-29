
// leaderboard submit

#include <iostream>
#include "leaderboard_submit.h"
#include <jsonrpccpp/client/connectors/httpclient.h>
#include <jsonrpccpp/client.h>

using namespace jsonrpc;
using namespace std;




int main(int argc, char* argv[]) {

  //if (argc != 2 ) return -1;


  //FILE *jf = fopen(argv[1], "r");

  FILE *jf = fopen("submission.json","r");
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


  Json::Value myv_message;
  Json::CharReaderBuilder builder;
  Json::CharReader* reader = builder.newCharReader();
  std::string errors;
  bool parsingSuccessful = reader->parse
    (jf_ptr, jf_ptr + lSize, &myv_message, &errors);
  delete reader;

  if (!parsingSuccessful) {
    std::cout << "Failed to parse the content JSON, errors:" << std::endl;
    std::cout << errors << std::endl;
  }
  std::string url;

  
  if( myv_message["url"].isNull()!=true )
  {
    url = myv_message["url"].asString();
  }else{
    std::cout<<"No address for Server!" <<std::endl;
    return 1;
  }


  HttpClient httpclient( url.c_str() );
  leaderboard_submit myClient(httpclient, JSONRPC_CLIENT_V2);
  Json::Value myv;

  try {
    myv = myClient.submit( myv_message.toStyledString() );
    cout << myv.toStyledString() << endl;
  } catch (JsonRpcException &e) {
    cerr << e.what() << endl;
  }


  return 0;
}
