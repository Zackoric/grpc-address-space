//
// Created by zackoric on 2/15/21.
//


#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>
#include <iostream>

#include "kv_store_client.h"

#include "stout/notification.h"
#include "stout/grpc/client.h"

#include "protos/kv_store.grpc.pb.h"

using grpc::Status;
using stout::Notification;

using stout::grpc::Client;
using stout::grpc::ClientCallStatus;
using stout::grpc::Stream;

using namespace std;
using namespace kv_store;

class RouteGuideClient
{
public:
	RouteGuideClient(Client *client) : client_(client)
	{
	}

	void putKVPair(string key, string value)
	{
		Notification<Status> done;
		PutRequest req;
		req.set_key(key);
		req.set_value(value);
		cout << "Calling put with (" << key << ", " << value << ")" << endl;
		client_->Call<KV_Store, PutRequest, PutResponse>(
		  "Put",
		  &req,
		  [&](auto* call, shared_ptr<PutResponse> response) {
		  	cout << "Put call returned with response code: " << response.get()->rescode() << endl;
		  	call->Finish();
		  },
		  [&](auto* call, const Status& status)
		  {
			  done.Notify(status);
		  });
		Status status = done.Wait();
		if (!status.ok()) {
			std::cout << "Put rpc failed: " << status.error_message() << std::endl;
		}
	}

	void getKVPair(string key)
	{
		Notification<Status> done;
		GetRequest req;
		req.set_key(key);
		cout << "Calling Get with key: " << key << endl;
		client_->Call<KV_Store, GetRequest, GetResponse>(
		  "Get",
		  &req,
		  [&](auto* call, shared_ptr<GetResponse> response) {
			  cout << "Put call returned with response code: " << response.get()->rescode() << endl;
			  cout << "Put call returned with response value: " << response.get()->value() << endl;
			  call->Finish();
		  },
		  [&](auto* call, const Status& status)
		  {
			  done.Notify(status);
		  });
		Status status = done.Wait();
		if (!status.ok()) {
			std::cout << "Get rpc failed: " << status.error_message() << std::endl;
		}
	}

	void delKVPair(string key)
	{
		Notification<Status> done;
		DelRequest req;
		req.set_key(key);
		cout << "Calling del with key: " << key << endl;
		client_->Call<KV_Store, DelRequest, DelResponse>(
		  "Del",
		  &req,
		  [&](auto* call, shared_ptr<DelResponse> response) {
			  cout << "Del call returned with response code: " << response.get()->rescode() << endl;
			  call->Finish();
		  },
		  [&](auto* call, const Status& status)
		  {
			  done.Notify(status);
		  });
		Status status = done.Wait();
		if (!status.ok()) {
			std::cout << "Del rpc failed: " << status.error_message() << std::endl;
		}
	}

private:
	Client *client_;
};

int main()
{
	cout << "Hello world!" << endl;
	Client client("localhost:50051", grpc::InsecureChannelCredentials());
	RouteGuideClient service(&client);
	bool done = false;
	int state = 0;
	string cmd, key, value, mode;
	do
	{
		if (cmd == "exit") break;
		switch(state)
		{
			case 0:
			{
				cout << "Mode: ";
				getline(cin, mode);
				char m = tolower(mode[0]);
				if (m == 'p') state = 1;
				if (m == 'g') state = 2;
				if (m == 'd') state = 3;
				break;
			}
			case 1:
			{
				cout << "Key: ";
				getline(cin, key);
				cout << "Value: ";
				getline(cin, value);
				service.putKVPair(key, value);
				state = 0;
				break;
			}
			case 2:
			{
				cout << "Key: ";
				getline(cin, key);
				service.getKVPair(key);
				state = 0;
				break;
			}
			case 3:
			{
				cout << "Key: ";
				getline(cin, key);
				service.delKVPair(key);
				state = 0;
				break;
			}
			default:
				state = 0;
		}
	}
	while(cmd != "exit");
	cout << "Client end." << endl;
	return 0;
}