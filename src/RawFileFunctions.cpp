//
// Created by justin on 10/23/21.
//

#include "RawFileFunctions.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include "hashes.h"
using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

json loadRaw(const char *file) {
    std::string f = file;

    json res;
    //read the file
    std::ifstream in(file, std::ios_base::in);
    res = json::parse(in);
    //throw and error if no name

    //perform the merge.

    if (res.contains("MERGE")) {
        const size_t last_slash_idx = std::string_view(file).rfind('/');
        std::string directory;
        std::string type = res["MERGE"];
        if (std::string::npos != last_slash_idx) {
            directory = std::string_view(file).substr(0, last_slash_idx);
        }
        json parent = loadRaw((directory + "/" + type).c_str());
        parent.update(res);
        res = parent;
    }
    if (!res.contains("name") || !res.contains("cat")) {
        std::cerr << "Forgot a name/cat field in " << file << " please fix the raws" << std::endl;
    }
    return res;
}

std::list<std::pair<uint16_t , json>> loadRaws(const char *dir) {
    std::list<std::pair<uint16_t , json>> res;
    for (const auto &dirEntry : recursive_directory_iterator(dir)) {
        if (dirEntry.is_regular_file()) {
            json raw = loadRaw(dirEntry.path().generic_string().c_str());
            std::cout << "Inserting : " << raw["cat"].get<std::string>() + "/" + raw["name"].get<std::string>()
                      << std::endl;
            res.push_front(
                    std::pair<uint16_t , json>(stringHash(
                            (raw["cat"].get<std::string>() + "/" +
                                                      raw["name"].get<std::string>()).c_str()),
                                                 raw));
        }
    }
    return res;
}
