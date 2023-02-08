#pragma once
#include "includes.h"

class RPNData
{
public:
  RPNData(std::string type) : type_(type) {}
  std::string type() { return type_; }
  virtual ~RPNData() {
    type_ = "";
  }
  virtual std::string Show() { return "0"; }
protected:
  std::string type_;
};

class RPNDataInt : public RPNData {
public:
  RPNDataInt(int data) : data_(data), RPNData("int") {}
  void Set(int data) { data_ = data; }
  int Get() { return data_; }
  ~RPNDataInt() {
    data_ = 0;
  }
  std::string Show() {
    return std::to_string(data_);
  }
protected:
  int data_;
};

class RPNDataFloat : public RPNData {
public:
  RPNDataFloat(double data) : data_(data), RPNData("float") {}
  void Set(double data) { data_ = data; }
  double Get() { return data_; }
  ~RPNDataFloat() {
    data_ = 0;
  }
  std::string Show() {
    return std::to_string(data_);
  }
protected:
  double data_;
};

class RPNDataChar : public RPNData {
public:
  RPNDataChar(char data) : data_(data), RPNData("char") {}
  void Set(char data) { data_ = data; }
  char Get() { return data_; }
  ~RPNDataChar() {
    data_ = 0;
  }
  std::string Show() {
    return " " + data_;
  }
private:
  char data_;
};

class RPNDataString : public RPNData {
public:
  RPNDataString(std::string data) : data_(data), RPNData("string") {}
  void Set(std::string data) { data_ = data; }
  std::string Get() { return data_; }
  ~RPNDataString() {
    data_ = "";
  }
  std::string Show() { return data_; }
protected:
  std::string data_;
};

class RPNDataBool : public RPNData {
public:
  RPNDataBool(int data) : data_(data), RPNData("bool") {}
  bool Get() { return data_; }
  void Set(bool data) { data_ = data; }
  ~RPNDataBool() {
    data_ = 0;
  }
  std::string Show() {
    if (data_) return "true";
    return "false";
  }
protected:
  bool data_;
};

class RPNDataAddress : public RPNData {
public:
  RPNDataAddress(int address) : address_(address), RPNData("address") {}
  int address() { return address_; }
  ~RPNDataAddress() {
    address_ = 0;
  }
  std::string Show() {
    return std::to_string(address_);
  }
protected:
  int address_;
};

class RPNDataType : public RPNData {
public:
  RPNDataType(std::string type) : type_(type), RPNData("type") {}
  std::string type() {
    return type_;
  }
  ~RPNDataType() {
    type_ = "";
  }
  std::string Show() {
    return type_;
  }
protected:
  std::string type_;
};

class RPNDataVariable : public RPNData {
public:
  RPNDataVariable(std::string name) : name_(name), RPNData("var") {}
  std::string name() { return name_; }
  ~RPNDataVariable() {
    name_ = "";
  }
  std::string Show() {
    return name_;
  }
private:
  std::string name_;
};