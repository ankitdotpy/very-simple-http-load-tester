#include<iostream>
#include<cstring>
#include<curl/curl.h>

size_t WriteCallback(void* content, size_t size, size_t num_elements, std::string* output){
    size_t total_size = size * num_elements;
    output->append(static_cast<char*>(content),total_size);
    return total_size;
}

int main(int argc, char* argv[]){
    if(argc<2){
        std::cerr<<"Usage: "<<argv[0]<<" <url>\nOption:\n-u : URL\n-n : Number of requests to make";
        return 1;
    }
    
    int num_request = 1;
    const char* url = "";
    
    if(argc == 2){
        url = argv[1];
    }
    else{
        for(int i=1;i<argc;i++){
            if(std::strcmp(argv[i],"-u") == 0){
                url = argv[++i];
            } 
            else if(std::strcmp(argv[i],"-n") == 0){
                num_request = std::atoi(argv[++i]);
            }
            else{
                std::cerr<<"Invalid Option: "<<argv[i]<<"\n";
                return 1;
            }
        }
    }

    //std::cout<<url<<" "<<num_request<<"\n";
    CURL* curl = curl_easy_init();
    if(curl){
        curl_easy_setopt(curl,CURLOPT_URL,url);
        
        std::string response_data;
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,WriteCallback);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,&response_data);

        CURLcode res = curl_easy_perform(curl);

        if(res != CURLE_OK){
            std::cerr<<"curl_easy_perform() failed: "<<curl_easy_strerror(res)<<"\n";
            return 1;
        }
        else{
            int http_code = 0;
            curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&http_code);
            std::cout<<"Response Code: "<<http_code<<"\n";
        }

        curl_easy_cleanup(curl);
    }

    return 0;
}
