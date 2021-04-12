//
// Created by zackoric on 4/5/21.
//

#include <iostream>
#include "kv_store/kv_store.h"
#include "gtest/gtest.h"
#include "KVStoreTests.h"

TEST_F(KVStoreTests, Basic)
{
  KVInterface store;
  std::string cache_value;
  EXPECT_FALSE(store.Get("my_key", cache_value));
  store.Set("my_key", "my_value");
  EXPECT_TRUE(store.Get("my_key", cache_value));
  EXPECT_EQ("my_value", cache_value);
  store.Set("my_key", "another_value");
  EXPECT_TRUE(store.Get("my_key", cache_value));
  EXPECT_EQ("another_value", cache_value);
}

TEST_F(KVStoreTests, Stress)
{
  KVInterface store;
  std::string cache_value;
  for (int i = 0; i < 999999; i++) {
    const std::string key = i + "", value = key;
    std::string cache_value;
    EXPECT_FALSE(store.Get(key, cache_value));
    store.Set(key, value);
    EXPECT_TRUE(store.Get(key, cache_value));
    EXPECT_EQ(cache_value, value);
  }
  for (int i = 0; i < 999999; i++) {
    const std::string key = i + "", value = key;
    const std::string alt_value = (9999999 - i) + "";
    std::string cache_value;
    EXPECT_TRUE(store.Get(key, cache_value));
    EXPECT_EQ(cache_value, value);
    store.Set(key, alt_value);
    EXPECT_TRUE(store.Get(key, cache_value));
    EXPECT_EQ(cache_value, alt_value);
  }
  for (int i = 0; i < 999999; i++) {
    const std::string key = i + "", value = (99999 - i) + "";
    std::string cache_value;
    EXPECT_TRUE(store.Get(key, cache_value));
    EXPECT_EQ(cache_value, value);
  }
  printf("hi world!");
}