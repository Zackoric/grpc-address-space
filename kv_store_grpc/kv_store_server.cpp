/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <iostream>
#include "rocksdb/db.h"
//#include <rock>

#include "kv_store_server.h"

#include "stout/grpc/server.h"
#include "protos/kv_store.grpc.pb.h"

using grpc::Status;
using std::chrono::system_clock;
using stout::grpc::Server;
using stout::grpc::ServerBuilder;
using stout::grpc::Stream;

using namespace kv_store;

map<Key, Value> kvDict;

int put(const Key &key, const Value &value)
{
	int retKey = SUCCESS_PUT_NEW_CODE;
	if (kvDict.find(key) != kvDict.end()) retKey = SUCCESS_PUT_REPLACE_CODE;
	kvDict[key] = value;
	return retKey;
}

int get(const Key &key, Value *value)
{
	if (kvDict.find(key) != kvDict.end())
	{
		*value = kvDict[key];
		return SUCCESS_CODE;
	}
	return FAIL_GET_NOT_EXIST_CODE;
}

int del(const Key &key)
{
	size_t erased = kvDict.erase(key);
	if (erased)
	{
		return SUCCESS_CODE;
	}
	return FAIL_DEL_NOT_EXIST_CODE;
}

class KV_StoreImpl final
{
public:
	explicit KV_StoreImpl()
	{}

	auto Serve(Server *server)
	{
		auto status = server->Serve<KV_Store, PutRequest, PutResponse>(
		  "Put",
		  [this](auto *call, auto &&putRequest)
		  {
			  auto key = putRequest->key(), value = putRequest->value();
			  cout << "Put Req: " << key << ", " << value << ";" << endl;
			  auto code = put(key, value);
			  PutResponse putResponse;
			  putResponse.set_rescode(code);
			  cout << "Put Res: " << code << ";" << endl;

			  call->WriteAndFinish(putResponse, Status::OK);
		  },
		  [](auto *, bool)
		  {});

		if (!status.ok())
		{
			cout << "Failed instantiating Put service;" << endl;
			return status;
		}

		status = server->Serve<KV_Store, GetRequest, GetResponse>(
		  "Get",
		  [this](auto *call, auto &&getRequest)
		  {
		  	auto key = getRequest->key();
			cout << "Get Req: " << key << ";" << endl;
			Value value;
			int code = get(key, &value);
			GetResponse getResponse;
			getResponse.set_rescode(code);
			getResponse.set_value(value);
			cout << "Get Res: " << code << ";" << endl;
			cout << "Get data: " << value << ";" << endl;
			call->WriteAndFinish(getResponse, Status::OK);
		  },
		  [](auto *, bool)
		  {});

		if (!status.ok())
		{
			cout << "Failed instantiating Del service;" << endl;
			return status;
		}

		status = server->Serve<KV_Store, DelRequest, DelResponse>(
		  "Del",
		  [this](auto *call, auto &&delRequest)
		  {
			  auto key = delRequest->key();
			  cout << "Get Req: " << key << ";" << endl;
			  Value value;
			  int code = del(key);
			  DelResponse delResponse;
			  delResponse.set_rescode(code);
			  cout << "Del Res: " << code << ";" << endl;
			  call->WriteAndFinish(delResponse, Status::OK);
		  },
		  [](auto *, bool)
		  {});

		if (!status.ok())
		{
			cout << "Failed instantiating Del service;" << endl;
			return status;
		}
	}
};

int RunServer()
{
	std::string server_address("0.0.0.0:50051");
	KV_StoreImpl service;

	ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

	auto build = builder.BuildAndStart();

	if (!build.status.ok())
	{
		std::cerr << "Failed to build and start server: "
		          << build.status.error() << std::endl;
		return -1;
	}
	std::unique_ptr<Server> server(std::move(build.server));
	std::cout << "Server listening on " << server_address << std::endl;

	auto status = service.Serve(server.get());

	if (!status.ok())
	{
		std::cerr << "Failed to serve: " << status.error() << std::endl;
		return -1;
	}

	server->Wait();

	return 0;
}

int main(int argc, char **argv)
{
	// Expect only arg: --db_path=path/to/route_guide_db.json.
//    std::string db = routeguide::GetDbFileContent(argc, argv);
	rocksdb::DB* db;
	rocksdb::Options options;
	options.create_if_missing = true;
	rocksdb::Status status = rocksdb::DB::Open(options, "/tmp/saywhereisthisfile", &db);
	assert(status.ok());
	string value;
//	cout << "i am alive!" << endl;
//	db->Get(rocksdb::ReadOptions(), "key2", &value);
//	cout << "value: " << value << endl;
//	db->Put(rocksdb::WriteOptions(), "key2", "value123");
//	return 0;
	return RunServer();
}
