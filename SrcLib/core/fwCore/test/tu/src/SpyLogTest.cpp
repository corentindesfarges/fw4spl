/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "SpyLogTest.hpp"

#include <fwCore/spyLog.hpp>
#include <fwCore/mt/types.hpp>

#include <fwTest/Exception.hpp>

#include <iostream>
#include <exception>
#include <streambuf>
#include <thread>
#include <regex>
#include <string>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ::fwCore::ut::SpyLogTest );

namespace fwCore
{
namespace ut
{

static ::fwTest::Exception e("");

void SpyLogTest::setUp()
{
    ::fwCore::log::SpyLogger& log = ::fwCore::log::SpyLogger::getSpyLogger();
    log.addStreamAppender(m_ostream);
}

//-----------------------------------------------------------------------------

void SpyLogTest::tearDown()
{
}

//-----------------------------------------------------------------------------

void SpyLogTest::logMessageTest()
{
    m_ostream.clear();
    ::fwCore::log::SpyLogger& log = ::fwCore::log::SpyLogger::getSpyLogger();

    std::vector< std::string > logs;

    logs.push_back("trace message");
    log.trace(logs.back(), __FILE__, __LINE__);
    this->checkLog(logs, this->logToVector(m_ostream));

    logs.push_back("debug message");
    log.debug(logs.back(), __FILE__, __LINE__);
    this->checkLog(logs, this->logToVector(m_ostream));

    logs.push_back("info message");
    log.info(logs.back(),   __FILE__, __LINE__);
    this->checkLog(logs, this->logToVector(m_ostream));

    logs.push_back("warn message");
    log.warn(logs.back(),   __FILE__, __LINE__);
    this->checkLog(logs, this->logToVector(m_ostream));

    logs.push_back("error message");
    log.error(logs.back(), __FILE__, __LINE__);
    this->checkLog(logs, this->logToVector(m_ostream));

    logs.push_back("fatal message");
    log.fatal(logs.back(), __FILE__, __LINE__);
    this->checkLog(logs, this->logToVector(m_ostream));
}

//-----------------------------------------------------------------------------

struct LogProducerThread
{
    typedef std::shared_ptr< LogProducerThread > sptr;
    typedef std::vector< std::string > LogContainerType;

    LogProducerThread()
    {
    }

    void run(LogContainerType& logs, size_t nbLogs, size_t offset)
    {
        ::fwCore::log::SpyLogger& log = ::fwCore::log::SpyLogger::getSpyLogger();
        for(size_t i = offset; i < nbLogs + offset; ++i)
        {
            std::stringstream ss;
            ss << "msg n ";
            ss.width(10);
            ss.fill('0');
            ss << i;
            logs[i] = ss.str();
            log.fatal(logs[i], __FILE__, __LINE__);
        }
    }
};

//-----------------------------------------------------------------------------

struct RegexLogCompare
{
    bool operator() (std::string a, std::string b)
    {
        std::regex re(".*(msg n [[:digit:]]+)$");
        std::smatch matchA;
        std::smatch matchB;
        bool doMatchA = std::regex_match(a, matchA, re);
        bool doMatchB = std::regex_match(b, matchB, re);
        CPPUNIT_ASSERT_MESSAGE( std::string("Regex do not match ") + a, doMatchA);
        CPPUNIT_ASSERT_MESSAGE( std::string("Regex do not match ") + b, doMatchB);

        std::string strA(matchA[1].first, matchA[1].second);
        std::string strB(matchB[1].first, matchB[1].second);

        return strA < strB;
    }
} regex_compare;

void SpyLogTest::threadSafetyTest()
{
    m_ostream.clear();
    const size_t NB_THREAD(20);
    const size_t NB_LOG(20);
    LogProducerThread::LogContainerType logs(NB_THREAD * NB_LOG, "test");
    std::vector< std::thread > tg;
    for(size_t i = 0; i < NB_THREAD; ++i)
    {
        LogProducerThread::sptr ct = std::make_shared<LogProducerThread>();
        size_t offset              = i * NB_LOG;
        tg.push_back(std::thread(std::bind(&LogProducerThread::run, ct, std::ref(logs), NB_LOG, offset) ));
    }
    for(auto& t : tg)
    {
        t.join();
    }

    LogProducerThread::LogContainerType logMessages = this->logToVector(m_ostream);

    std::sort( logMessages.begin(), logMessages.end(), regex_compare);
    this->checkLog(logs, logMessages);
}

//-----------------------------------------------------------------------------

std::vector<std::string> SpyLogTest::logToVector(const std::stringstream &logsStream)
{
    std::vector<std::string> lines;
    std::string line;
    std::istringstream input;
    input.str(logsStream.str());
    while(std::getline(input, line))
    {
        lines.push_back(line);
    }
    return lines;
}

//-----------------------------------------------------------------------------

void SpyLogTest::checkLog(const std::vector<std::string> &logMessagesRef, const std::vector<std::string> &logMessages)
{
    CPPUNIT_ASSERT_EQUAL(logMessagesRef.size(), logMessages.size());

    const std::string linePattern("(\\[[0-9]+\\])");
    const std::string timePattern("(\\[.+\\])");
    const std::string levelPattern("( *\\[[a-z]+\\])");
    const std::string filePattern("( .*:)");
    const std::string fileLinePattern("([0-9]+: )");
    const std::string messagePattern("(.*)$");

    std::regex re(
        linePattern
        + timePattern
        + levelPattern
        + filePattern
        + fileLinePattern
        + messagePattern );

    std::smatch match;
    std::string regexMessage;
    size_t i = 0;

    for(const std::string &log :  logMessages)
    {
        bool doMatch = std::regex_match(log, match, re);

        CPPUNIT_ASSERT_MESSAGE(log + " don't match regex.", doMatch);

        regexMessage.assign(match[6].first, match[6].second);

        CPPUNIT_ASSERT_EQUAL(logMessagesRef[i], regexMessage);
        ++i;
    }
}

//-----------------------------------------------------------------------------

} //namespace ut
} //namespace fwCore
