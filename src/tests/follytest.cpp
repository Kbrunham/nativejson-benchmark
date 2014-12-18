/*
 * follytest.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: kbrunham
 */

#include "../test.h"

#define FOLLY_NO_CONFIG
#define FOLLY_NO_GLOG
#define FOLLY_NO_CLOCKID

#define DCHECK_GT(a,b)
#define DCHECK(a)
#define DCHECK_EQ(a,b)
#define DCHECK_LE(a,b)
#define CHECK(a) std::cout
#define LOG(a) std::cout

#include <iostream>
#include <algorithm>

#include "folly/Conv.cpp"
#include "folly/dynamic.h"
#include "folly/json.h"
#include "folly/json.cpp"
#include "folly/dynamic.cpp"
#include "folly/String.cpp"
#include "folly/Unicode.cpp"
#include "folly/Malloc.cpp"
#include "folly/Range.h"
#include "folly/Range.cpp"
#include "folly/detail/MallocImpl.cpp"
#include "folly/Demangle.cpp"
#include "folly/Format.cpp"
#include "folly/FormatTables.cpp"

#include "double-conversion/double-conversion.cc"
#include "double-conversion/fast-dtoa.cc"
#include "double-conversion/fixed-dtoa.cc"
#include "double-conversion/bignum.cc"
#include "double-conversion/bignum-dtoa.cc"
#include "double-conversion/cached-powers.cc"
#include "double-conversion/diy-fp.cc"
#include "double-conversion/strtod.cc"


static void GenStat(Stat& stat, const folly::dynamic& v) {
    if (v.isObject())
	{
		for (auto itr : v.keys()) {
			stat.stringLength += itr.getString().size();
			GenStat(stat, v[itr.getString()]);
		}
		stat.objectCount++;
		stat.memberCount +=  v.size();
		stat.stringCount +=  v.size();
	}
    else if (v.isArray())
	{
		for (auto o : v)
		{
			GenStat(stat, o);
		}
		stat.arrayCount++;
		stat.elementCount += std::distance(v.begin(), v.end());
	}
    else if (v.isString())
    {
		stat.stringCount++;
		stat.stringLength += v.getString().size();
    }
    else if (v.isNumber())
    {
        stat.numberCount++;
    }
    else if (v.isBool())
    {
    	if (v.getBool())
            stat.trueCount++;
        else
            stat.falseCount++;
    }
    else if (v.isNull())
    {
        stat.nullCount++;
    }
}

class FollyParseResult : public ParseResultBase {
public:
	folly::dynamic root;

	FollyParseResult() : root("") {}

	~FollyParseResult() {}

};

class FollyStringResult : public StringResultBase {
public:
    virtual const char* c_str() const { return s.c_str(); }

    std::string s;
};

class FollyTest : public TestBase {
public:
#if TEST_INFO
    virtual const char* GetName() const override { return "Folly (C++)"; }
    virtual const char* GetFilename() const override { return __FILE__; }
    virtual unsigned long GetMaxSize() const override {return 2500000;}
#endif

#if TEST_PARSE
    virtual ParseResultBase* Parse(const char* json, size_t length) const override {
        FollyParseResult* pr = new FollyParseResult;

        pr->root = folly::parseJson(json);

    	return pr;
    }
#endif

#if TEST_STRINGIFY
    virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const override {
        const FollyParseResult* pr = static_cast<const FollyParseResult*>(parseResult);
        FollyStringResult* sr = new FollyStringResult;

        sr->s = folly::toJson(pr->root).c_str();

        return sr;
    }
#endif

#if TEST_PRETTIFY
    virtual StringResultBase* Prettify(const ParseResultBase* parseResult) const {
        const FollyParseResult* pr = static_cast<const FollyParseResult*>(parseResult);
        FollyStringResult* sr = new FollyStringResult;

        sr->s = folly::toPrettyJson(pr->root).c_str();

        return sr;
    }
#endif


#if TEST_STATISTICS
    virtual bool Statistics(const ParseResultBase* parseResult, Stat* stat) const override {
        const FollyParseResult* pr = static_cast<const FollyParseResult*>(parseResult);
        memset(stat, 0, sizeof(Stat));
        GenStat(*stat, pr->root);
        return true;
    }
#endif
};

REGISTER_TEST(FollyTest);












#undef DCHECK_GT
#undef DCHECK
#undef CHECK
#undef LOG
#undef DCHECK_EQ

#undef FOLLY_NO_CONFIG
#undef FOLLY_NO_GLOG
#undef FOLLY_NO_CLOCKID


