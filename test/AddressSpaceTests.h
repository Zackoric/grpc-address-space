//
// Created by zackoric on 4/22/21.
//

#ifndef GRPC_ADDRESS_SPACE_ADDRESSSPACETESTS_H
#define GRPC_ADDRESS_SPACE_ADDRESSSPACETESTS_H

#include "gtest/gtest.h"

class AddressSpaceTests : public ::testing::Test {
 protected:
  virtual void SetUp()
  {
    ASSERT_EQ(1, GetThreadCount());
  }

  virtual void TearDown()
  {
    // NOTE: need to wait until all internal threads created by the
    // grpc library have completed because some of our tests are death
    // tests which fork.
    while (GetThreadCount() != 1) {}
  }

  size_t GetThreadCount()
  {
    // TODO(benh): Don't rely on the internal 'GetThreadCount()'.
    return testing::internal::GetThreadCount();
  }};


#endif //GRPC_ADDRESS_SPACE_ADDRESSSPACETESTS_H
