//
// Created by zackoric on 2/15/21.
//

#ifndef RESEARCH_GRPC_KV_STORE_SERVER_H
#define RESEARCH_GRPC_KV_STORE_SERVER_H

#include <string>

using namespace std;

typedef string Key;
typedef string Value;

#define SUCCESS_CODE 200
#define SUCCESS_PUT_NEW_CODE 200
#define SUCCESS_PUT_REPLACE_CODE 202
#define FAIL_GET_NOT_EXIST_CODE 404
#define FAIL_DEL_NOT_EXIST_CODE 405

#endif //RESEARCH_GRPC_KV_STORE_SERVER_H
