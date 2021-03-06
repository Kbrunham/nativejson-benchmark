#pragma once
#include "config.h"
#include "memorystat.h"

#include <vector>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <math.h>

using std::isfinite;

struct Stat {
    size_t objectCount;
    size_t arrayCount;
    size_t numberCount;
    size_t stringCount;
    size_t trueCount;
    size_t falseCount;
    size_t nullCount;

    size_t memberCount;   // Number of members in all objects
    size_t elementCount;  // Number of elements in all arrays
    size_t stringLength;  // Number of code units in all strings
};


struct TestJson {
    TestJson() : fullpath(), filename(), json(), length(), stat(), statUTF16() {}

    char* fullpath;
    char* filename;
    char* json;
    size_t length;
    Stat stat;           // Reference statistics
    Stat statUTF16;      // Reference statistics

    bool operator<(const TestJson& rhs) const {
        return strcmp(filename, rhs.filename) < 0;
    }

    bool operator==(const TestJson& rhs) const {
        return strcmp(filename, rhs.filename) == 0;
    }

};

typedef std::vector<TestJson> TestJsonList;


class TestBase;
typedef std::vector<const TestBase *> TestList;

class TestManager {
public:
    static TestManager& Instance() {
        static TestManager* singleton = new TestManager;
        return *singleton;
    }

    static void DestroyInstance() {
        delete &Instance();
    }

    void AddTest(const TestBase* test) {
        mTests.push_back(test);
    }

    const TestList& GetTests() const {
        return mTests;
    }

    TestList& GetTests() {
        return mTests;
    }

private:
    TestList mTests;
};

// Each test can customize what to be stored in parse result, 
// which will be passed to Stringify()/Prettify()/Statistics()
class ParseResultBase {
public:
    virtual ~ParseResultBase() {}
};

// Stringify()/Prettify() returns object derived from this class.
// So that it can prevents unncessary strdup().
class StringResultBase {
public:
    virtual ~StringResultBase() {}

    // The test framework call this function to get a null-terminated string.
    virtual const char* c_str() const = 0;
};

class TestBase {
public:
    TestBase() : name_()
	{
        TestManager::Instance().AddTest(this);
    }

    virtual ~TestBase() {}

    bool operator<(const TestBase& rhs) const {
        return strcmp(name_, rhs.name_) < 0;
    }

    bool operator==(const TestBase& rhs) const {
        return strcmp(name_, rhs.name_) == 0;
    }

    // What is the max JSON file size supported by this test
    virtual unsigned long GetMaxSize() const {return 0;}

    virtual void ignoreJson(const TestJson& json) const final {ignore_list.push_back(json);}
    virtual bool should_ignore(const TestJson& json) const final {return std::find(ignore_list.begin(), ignore_list.end(), json) != ignore_list.end();}

#if TEST_INFO
    virtual const char* GetName() const = 0;
    virtual const char* GetFilename() const = 0;
#endif

#if TEST_PARSE
    virtual ParseResultBase* Parse(const char* json, size_t length) const {
        (void)json;
        (void)length;
        return 0;
    }
#endif

#if TEST_STRINGIFY
    virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const {
        (void)parseResult;
        return 0;
    }
#endif

#if TEST_PRETTIFY
    virtual StringResultBase* Prettify(const ParseResultBase* parseResult) const {
        (void)parseResult;
        return 0;
    }
#endif

#if TEST_STATISTICS
    virtual bool Statistics(const ParseResultBase* parseResult, Stat* stat) const {
        (void)parseResult;
        (void)stat;
        return false;
    }
#endif

#if TEST_SAXROUNDTRIP
    virtual StringResultBase* SaxRoundtrip(const char* json, size_t length) const {
        (void)json;
        (void)length;
        return 0;
    }
#endif

#if TEST_SAXSTATISTICS
    virtual bool SaxStatistics(const char* json, size_t length, Stat* stat) const {
        (void)json;
        (void)length;
        (void)stat;
        return false;
    }
#endif

#if TEST_SAXSTATISTICSUTF16
    virtual bool SaxStatisticsUTF16(const char* json, size_t length, Stat* stat) const {
        (void)json;
        (void)length;
        (void)stat;
        return false;
    }
#endif

protected:
    const char* name_;
    mutable std::vector<TestJson> ignore_list;
};

#define REGISTER_TEST(cls) static cls gRegister##cls
