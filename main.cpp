#include "SystemManager.h"
#include "CampusException.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string dataDir = "data";
    std::string outputDir = ".";

    if (argc >= 2) dataDir = argv[1];
    if (argc >= 3) outputDir = argv[2];

    try {
        SystemManager manager;
        manager.loadUsers(dataDir + "/users.csv");
        manager.loadResources(dataDir + "/resources.csv");
        manager.loadMaintenance(dataDir + "/maintenance.csv");
        manager.loadRequests(dataDir + "/requests.csv");
        manager.processAll();
        manager.writeOutputs(outputDir);
        std::cout << "CampusOps Core finished. Check output files.\n";
    } catch (const CampusException& e) {
        std::cerr << "CampusException(" << e.code() << "): " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << "\n";
        return 2;
    }

    return 0;
}
