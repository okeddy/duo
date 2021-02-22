#include "../base/LogFile.h"
#include "../base/Logging.h"

boost::scoped_ptr<duo::LogFile> g_logFile;

void outputFunc(const char* msg, int len) {
    g_logFile->append(msg, len);
}

void flushFunc() {
    g_logFile->flush();
}

int main(int argc, char* argv[]) {
    char name[256];
    strncpy(name, argv[0], 256);
    g_logFile.reset(new duo::LogFile(::basename(name), 256 * 1024));
    duo::Logger::setOutput(outputFunc);
    duo::Logger::setFlush(flushFunc);

    duo::string line = "1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (int i = 0; i < 5000; ++i) {
        LOG_INFO << line << i;

        usleep(1000);
    }
}
