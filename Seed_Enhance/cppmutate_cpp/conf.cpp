#include "conf.hpp"
#include <fstream>
#include <json/reader.h>
#include <memory>

std::shared_ptr<conf_t> cur_conf;

void conf_init(const std::string& file_name)
{
    cur_conf = std::make_shared<conf_t>();
    Json::Value   root;
    std::ifstream file(file_name);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open configuration file: " + file_name);
    }
    Json::Reader reader;
    if (!reader.parse(file, root))
    {
        throw std::runtime_error("Failed to parse configuration file: " + file_name);
    }
    cur_conf->root = root;
}

conf_t& conf()
{
    if (!cur_conf)
    {
        throw std::runtime_error("Configuration not initialized. Call conf_init() first.");
    }
    return *cur_conf;
}
