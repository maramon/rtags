#ifndef PROCESS_H
#define PROCESS_H

#include "ByteArray.h"
#include <signalslot.h>
#include <list>
#include <deque>

class Process
{
public:
    Process();
    ~Process();

    void start(const ByteArray& command, const std::list<ByteArray>& arguments);

    void write(const ByteArray& data);
    void closeStdIn();

    ByteArray readAllStdOut();
    ByteArray readAllStdErr();

    int returnCode() const { return mReturn; }

    signalslot::Signal0 readyReadStdOut() const { return mReadyReadStdOut; }
    signalslot::Signal0 readyReadStdErr() const { return mReadyReadStdErr; }
    signalslot::Signal0 finished() const { return mFinished; }

private:
    static void processCallback(int fd, unsigned int flags, void* userData);

    void closeStdOut();
    void closeStdErr();

    void handleInput(int fd);
    void handleOutput(int fd, ByteArray& buffer, int& index, signalslot::Signal0& signal);
    void handleTerminated();

private:
    int mStdIn[2];
    int mStdOut[2];
    int mStdErr[2];

    pid_t mPid;
    int mReturn;

    std::deque<ByteArray> mStdInBuffer;
    ByteArray mStdOutBuffer, mStdErrBuffer;
    int mStdInIndex, mStdOutIndex, mStdErrIndex;

    signalslot::Signal0 mReadyReadStdOut, mReadyReadStdErr, mFinished;
};

#endif
