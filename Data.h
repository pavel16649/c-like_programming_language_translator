#pragma once
#include "includes.h"

enum class DataTypesEnum
{
    BOOL,  CHAR,  INT,  FLOAT,  STRING,  ARRAY,  VOID,  VAR
};

class Data;
class DataInt;
class DataFloat;
class DataBool;
class DataChar;
class DataString;
class DataArray;


class Data
{
public:
    Data() : type_(DataTypesEnum::VOID) {}
    Data(DataTypesEnum type) : type_(type) { }
    DataTypesEnum Type() { return type_; }
    virtual void Show() {}
protected:
    DataTypesEnum type_;
};

class DataInt : public Data {
public:
    DataInt() : Data(DataTypesEnum::INT), data_(0) {}
    DataInt(int data) : Data(DataTypesEnum::INT),  data_(data) {}
    int Get() { return data_; }
    void Set(int data) { data_ = data; }
    virtual void Show() { std::cout << data_; }
protected:
    int data_;
};

class DataFloat : public Data {
public:
    DataFloat() : Data(DataTypesEnum::FLOAT), data_(0) {}
    DataFloat(double data) : Data(DataTypesEnum::FLOAT),  data_(data) {}
    double Get() { return data_; }
    virtual void Show() { std::cout << data_; }
    void Set(double data) { data_ = data; }
protected:
    double data_;
};

class DataBool : public Data {
public:
    DataBool() : Data(DataTypesEnum::BOOL), data_(0) {}
    DataBool(bool data) : Data(DataTypesEnum::BOOL),  data_(data) {}
    bool Get() { return data_; }
    virtual void Show() { std::cout << (data_ ? "true" : "false"); }
    void Set(bool data) { data_ = data; }
protected:
    bool data_;
};

class DataChar : public Data {
public:
    DataChar() : Data(DataTypesEnum::CHAR), data_(0) {}
    DataChar(char data) : Data(DataTypesEnum::CHAR),  data_(data) {}
    char Get() { return data_; }
    virtual void Show() { std::cout << char(data_); }
    void Set(char data) { data_ = data; }
protected:
    char data_;
};

class DataString : public Data {
public:
    DataString() : Data(DataTypesEnum::STRING), data_("") {}
    DataString(std::string data) : Data(DataTypesEnum::STRING),  data_(data) {}
    std::string Get() { return data_; }
    virtual void Show() { std::cout << data_; }
    void Set(std::string data) { data_ = data; }
protected:
    std::string data_;
};

class DataArray : public Data {
public:
    DataArray(std::vector<Data*> data) : Data(DataTypesEnum::ARRAY),  data_(data), base_type_(DataTypesEnum::VOID) {
        if (data_.size() != 0) {
            base_type_ = data[0]->Type();
        }
    }
    DataArray(DataTypesEnum base_type, int dimension) : base_type_(base_type), dimension_(dimension) {}

    std::vector<Data*> Get() { return data_; }
    int Dimension() { return dimension_; }
    virtual void Show() {
        std::cout << "{ ";
        for (int i = 0; i < data_.size() - 1; ++i) {
            data_[i]->Show();
            std::cout << ",  ";
        }
        data_[data_.size() - 1]->Show();
        std::cout << " }";
    }
    void Push(Data* data) {
        data_.push_back(data);
    }
    void Set(std::vector<Data*> data) { data_ = data; }
protected:
    std::vector<Data*> data_;
    int dimension_;
    DataTypesEnum base_type_;
};

class DataVar : public Data {
public:
    DataVar(Data*& data) : data_(data),  Data(DataTypesEnum::VAR) {}
    Data* Get() { return data_; }
    Data*& GetLink() { return data_; }
    void Set(Data* data) { data_ = data; }
    void Show() { data_->Show(); }
private:
    Data* data_;
};