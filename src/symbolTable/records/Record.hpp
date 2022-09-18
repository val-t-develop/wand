#pragma once
#include <Defs.hpp>

class Record {
public:
  string id;
  string type;
  shared_ptr<Record> next;

  Record(string id, string type);

  bool equals(shared_ptr<Record> r);
};