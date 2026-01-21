#pragma once
#include <gmock/gmock.h>

#include "INoise.hh"

class MockNoise : public INoise {
 public:
  MockNoise() : INoise(0.0) {}
  MOCK_METHOD(float, addNoise, (), (const, override));
};