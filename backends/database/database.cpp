#include "database.h"

#include <bsoncxx/builder/stream/document.hpp>
//#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

#if defined(__APPLE__)
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::pair;
#else
#include <bits/stdc++.h>
#include <utility>
using std::pair;
#endif
using std::string;
using std::vector;

#include "cpptoml.h"

database::database() : client(), db(){}
database::~database() {}

database& database::getInstance()
{
    static database instance;
    return instance;
}

void database::connect()
{
    auto config = cpptoml::parse_file("/Users/david/CLionProjects/HashHarbor/backends/database/db.toml");
    auto apikey = config->get_table("mongodb");

    const auto uri = mongocxx::uri{*apikey->get_as<string>("uri")};
    // Set the version of the Stable API on the client
    mongocxx::options::client client_options;
    const auto api = mongocxx::options::server_api{mongocxx::options::server_api::version::k_version_1};
    client_options.server_api_opts(api);

    client = mongocxx::client { uri, client_options };
    db = client["HashHarbor"];
    std::cout << "Connected to MongoDB" << std::endl;
}

bool database::getUserAuth(string usr, database::usrProfile& profile)
{
    try {
        auto collection = db["UserAuth"];

        auto findUsr = collection.find_one(make_document(kvp("username", usr)));
        if(findUsr)
        {
            bsoncxx::document::element db_id = findUsr.value()["_id"];
            profile._id = db_id.get_oid().value.to_string();

            bsoncxx::document::element db_usr = findUsr.value()["username"];
            profile.username = db_usr.get_string().value.data();

            bsoncxx::document::element db_passwd = findUsr.value()["password"];
            profile.hash = db_passwd.get_string().value.data();

            bsoncxx::document::element db_salt = findUsr.value()["salt"];
            profile.salt = db_salt.get_string().value.data();

            assert(findUsr);
            return true;
        }
        assert(findUsr);
    }catch(const std::exception& e)
    {
        cout << "Failed to get collection. :: " << e.what() << endl;
    }
    return false;
}

bool database::makeUser(database::usrProfile &profile)
{
    try {
        auto collection = db["UserAuth"];

        auto findUsr = collection.find_one(make_document(kvp("username", profile.username)));
        if(findUsr) // make sure this username has not been used
        {
            assert(findUsr);
            return false;
        }

        auto insertUsr = collection.insert_one(make_document(
                kvp("username", profile.username),
                kvp("password", profile.hash),
                kvp("salt", profile.salt)
        ));
        assert(insertUsr);

    }catch(const std::exception& e)
    {
        cout << "Database Failure:: " << e.what() << endl;
    }
    return false;
}

