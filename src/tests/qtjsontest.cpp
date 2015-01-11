/*
 * Qt5 JSON test
 *
 *  Created on: Jan 10, 2015
 *      Author: kbrunham
 */

#include "../test.h"

#include <string>
#include <iostream>
#include <QtCore/QtCore>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

static void GenStat(Stat& stat, const QJsonValue& v) {
    if (v.isObject())
	{
		for (auto itr : v.toObject().keys()) {
			stat.stringLength += itr.size();
			GenStat(stat, v.toObject()[itr]);
		}
		stat.objectCount++;
		stat.memberCount +=  v.toObject().size();
		stat.stringCount +=  v.toObject().size();
	}
    else if (v.isArray())
	{
		for (auto o : v.toArray())
		{
			GenStat(stat, o);
		}
		stat.arrayCount++;
		stat.elementCount += v.toArray().size();
	}
    else if (v.isString())
    {
		stat.stringCount++;
		stat.stringLength += v.toString().size();
    }
    else if (v.isDouble())
    {
        stat.numberCount++;
    }
    else if (v.isBool())
    {
    	if (v.toBool())
            stat.trueCount++;
        else
            stat.falseCount++;
    }
    else if (v.isNull())
    {
        stat.nullCount++;
    }

}

static void GenStat(Stat& stat, const QJsonDocument& v) {
    if (v.isObject())
	{
		for (auto itr : v.object().keys()) {
			stat.stringLength += itr.size();
			GenStat(stat, v.object()[itr]);
		}
		stat.objectCount++;
		stat.memberCount +=  v.object().size();
		stat.stringCount +=  v.object().size();
	}
    else if (v.isArray())
	{
		for (auto o : v.array())
		{
			GenStat(stat, o);
		}
		stat.arrayCount++;
		stat.elementCount += v.array().size();
	}
}

class QtJsonParseResult : public ParseResultBase {
public:
	QJsonDocument root;

	QtJsonParseResult() = default;

	~QtJsonParseResult() = default;

};

class QtJsonStringResult : public StringResultBase {
public:
    virtual const char* c_str() const { return s.c_str(); }

    std::string s;
};

class QTJsonTest : public TestBase {
public:
#if TEST_INFO
    virtual const char* GetName() const override { return "Qt Json (C++)"; }
    virtual const char* GetFilename() const override { return __FILE__; }
    virtual unsigned long GetMaxSize() const override {return 0;}
#endif

#if TEST_PARSE
    virtual ParseResultBase* Parse(const char* json, size_t length) const override {
        QtJsonParseResult* pr = new QtJsonParseResult;
        QJsonParseError json_err;
        pr->root = QJsonDocument::fromJson(QString(json).toUtf8(), &json_err);
        if (json_err.error != QJsonParseError::NoError)
        {
        	delete pr;
        	pr = nullptr;
        	std::cout << qPrintable(json_err.errorString()) << endl;
        }


    	return pr;
    }
#endif

#if TEST_STRINGIFY
    virtual StringResultBase* Stringify(const ParseResultBase* parseResult) const override {
        const QtJsonParseResult* pr = static_cast<const QtJsonParseResult*>(parseResult);
        QtJsonStringResult* sr = new QtJsonStringResult;

        QByteArray arr = pr->root.toJson(QJsonDocument::Compact);

        sr->s = std::string(qPrintable(QString::fromUtf8(arr)));

        return sr;
    }
#endif

#if TEST_PRETTIFY
    virtual StringResultBase* Prettify(const ParseResultBase* parseResult) const {
        const QtJsonParseResult* pr = static_cast<const QtJsonParseResult*>(parseResult);
        QtJsonStringResult* sr = new QtJsonStringResult;

        QByteArray arr = pr->root.toJson(QJsonDocument::Indented);

        sr->s = std::string(qPrintable(QString::fromUtf8(arr)));

        return sr;
    }
#endif


#if TEST_STATISTICS
    virtual bool Statistics(const ParseResultBase* parseResult, Stat* stat) const override {
        const QtJsonParseResult* pr = static_cast<const QtJsonParseResult*>(parseResult);
        memset(stat, 0, sizeof(Stat));
        GenStat(*stat, pr->root);
        return true;
    }
#endif
};

REGISTER_TEST(QTJsonTest);


