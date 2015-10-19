#include <Process.h>
#include <MainProcess.h>
#include <ChildProcess.h>
#include <Pipe.h>
#include <Utils.h>

int main(int argc, char **argv) {
    Options options;
    options.assign(argc, argv);

    if (!options.parse()) {
        options.printUsage();
        return 1;
    }
    options.printArgs();

    MainProcess mainProcess;
    mainProcess.assign(options);
    return mainProcess.execute();
}
