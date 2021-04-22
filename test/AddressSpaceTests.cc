//
// Created by zackoric on 4/22/21.
//

#include "AddressSpaceTests.h"
#include <iostream>
#include "kv_store/AddressSpace.h"
#include "kv_store/kv_store.h"

void hi() {
  std::cout << "hello world!" << std::endl;
  Reference<int> ref;
  AddressSpace space("localhost");
  kv_store::MyStruct stru;
  stru.set_astring("a string");
  stru.set_anumber(42);
  auto ref = space.Put(stru);
  kv_store::MyStruct fetchedStru = space.Get<kv_store::MyStruct>(ref);
  EXPECT_EQ(stru.anumber(), fetchedStru.anumber());
  EXPECT_EQ(stru.astring(), fetchedStru.astring());
}

//TEST_F(AddressSpaceTests) {
//
//    AddressSpace space("localhost");
//    kv_store::MyStruct stru;
//    stru.set_astring("a string");
//    stru.set_anumber(42);
//    auto ref = space.Put(stru);
//    kv_store::MyStruct fetchedStru = space.Get<kv_store::MyStruct>(ref);
//    EXPECT_EQ(stru.anumber(), fetchedStru.anumber());
//    EXPECT_EQ(stru.astring(), fetchedStru.astring());
//
////    std::string cache_value;
////    EXPECT_FALSE(store.Get("my_key", cache_value));
////    store.Set("my_key", "my_value");
////    EXPECT_TRUE(store.Get("my_key", cache_value));
////    EXPECT_EQ("my_value", cache_value);
////    store.Set("my_key", "another_value");
////    EXPECT_TRUE(store.Get("my_key", cache_value));
////    EXPECT_EQ("another_value", cache_value);
//}
