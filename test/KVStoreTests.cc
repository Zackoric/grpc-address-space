//
// Created by zackoric on 4/5/21.
//

#include <iostream>
#include "kv_store/kv_store.h"
#include "gtest/gtest.h"
#include "KVStoreTests.h"

TEST_F(KVStoreTests, Basic) {
  MemoryKVStore store;
  std::string cache_value;
  EXPECT_FALSE(store.Get("my_key", cache_value));
  store.Set("my_key", "my_value");
  EXPECT_TRUE(store.Get("my_key", cache_value));
  EXPECT_EQ("my_value", cache_value);
  store.Set("my_key", "another_value");
  EXPECT_TRUE(store.Get("my_key", cache_value));
  EXPECT_EQ("another_value", cache_value);
}

TEST_F(KVStoreTests, Stress) {
  MemoryKVStore store;
  std::string cache_value;
  const int iter = 999;
  for (int i = 0; i < iter; i++) {
    const std::string key = std::to_string(i), value = key;
    std::string cache_value;
    EXPECT_FALSE(store.Get(key, cache_value));
    store.Set(key, value);
    EXPECT_TRUE(store.Get(key, cache_value));
    EXPECT_EQ(cache_value, value);
  }
  for (int i = 0; i < iter; i++) {
    const std::string key = std::to_string(i), value = key;
    const std::string alt_value = std::to_string(iter - i);
    std::string cache_value;
    EXPECT_TRUE(store.Get(key, cache_value));
    EXPECT_EQ(cache_value, value);
    store.Set(key, alt_value);
    EXPECT_TRUE(store.Get(key, cache_value));
    EXPECT_EQ(cache_value, alt_value);
  }
  for (int i = 0; i < iter; i++) {
    const std::string key = std::to_string(i), value = std::to_string(iter - i);
    std::string cache_value;
    EXPECT_TRUE(store.Get(key, cache_value));
    EXPECT_EQ(cache_value, value);
  }
}