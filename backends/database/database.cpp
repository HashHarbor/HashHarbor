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
#include "../src/imageHandler/imagePath.h"
#include "userProfile/userProfile.h"

database::database() : client(), db(){}
database::~database() {}

database& database::getInstance()
{
    static database instance;
    return instance;
}

void database::connect()
{
    imagePath imgPth = imagePath();

    auto config = cpptoml::parse_file(imgPth.absolutePath + "assets/api.toml");
    auto apikey = config->get_table("mongodb");

    auto config_Q = cpptoml::parse_file(imgPth.absolutePath + "assets/api.toml");
    auto apikey_Q = config_Q->get_table("questions");

    const auto uri = mongocxx::uri{*apikey->get_as<string>("uri")};

    const auto uri_Q = mongocxx::uri{*apikey_Q->get_as<string>("uri")};
    // Set the version of the Stable API on the client
    mongocxx::options::client client_options;
    const auto api = mongocxx::options::server_api{mongocxx::options::server_api::version::k_version_1};
    client_options.server_api_opts(api);

    client = mongocxx::client { uri, client_options };
    db = client["HashHarbor"];

    client_Q = mongocxx::client { uri_Q, client_options };
    db_Q = client_Q["question"];
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
        auto collectionProgress = db["UserProgress"];

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

        int w,h;
#if defined(__APPLE__)
        w = 1320;
        h = 768;
#else
        w = 1760;
        h = 1024;
#endif

        auto insertProgress = collectionProgress.insert_one(make_document(
                kvp("_id", insertUsr->inserted_id().get_oid()), // use user oid number to relate data to user
                kvp("join", bsoncxx::types::b_date(std::chrono::system_clock::now())),
                kvp("character", make_array(0,5,8,0,2,2,0,3)),
                kvp("settings", make_array(w,h))
        ));
        assert(insertProgress);

        profile._id = insertUsr->inserted_id().get_oid().value.to_string();
        return true;

    }catch(const std::exception& e)
    {
        cout << "Database Failure:: " << e.what() << endl;
    }
    return false;
}

bool database::getUserData()
{
    try {
        userProfile& usrProfile = userProfile::getInstance();
        auto collectionProgress = db["UserProgress"];

        auto findData = collectionProgress.find_one(make_document(kvp("_id", bsoncxx::oid(usrProfile.getId()))));
        if(findData)
        {
            bsoncxx::document::element db_join = findData.value()["join"]; // date
            assert(db_join.type() == bsoncxx::type::k_date);
            bsoncxx::types::b_date date = db_join.get_date();
            auto time_point = std::chrono::system_clock::time_point(std::chrono::milliseconds(date.to_int64()));
            auto time = std::chrono::system_clock::to_time_t(time_point);
            std::tm* tmPtr = std::localtime(&time);
            std::stringstream ss;
            ss << std::setfill('0') << std::setw(2) << tmPtr->tm_mon + 1 << "/" << std::setw(2) << tmPtr->tm_mday << "/" << std::setw(4) << tmPtr->tm_year + 1900;
            usrProfile.setJoinDate(ss.str());

            bsoncxx::document::element db_character = findData.value()["character"]; // array for character
            assert(db_character.type() == bsoncxx::type::k_array);
            bsoncxx::array::view arr = db_character.get_array();

            int i = 0;
            for(auto iter : arr)
            {
                assert(iter.type() == bsoncxx::type::k_int32);
                usrProfile.setCharacter(i, iter.get_int32().value);
                i++;
            }

            bsoncxx::document::element db_settings = findData.value()["settings"]; // array for character
            assert(db_settings.type() == bsoncxx::type::k_array);
            arr = db_settings.get_array();
            i = 0;
            int w,h;
            for(auto iter : arr)
            {
                assert(iter.type() == bsoncxx::type::k_int32);
                switch(i)
                {
                    case 0:
                        w = iter.get_int32().value;
                        break;
                    case 1:
                        h = iter.get_int32().value;
                        break;
                }
                i++;
            }
            usrProfile.setSettings(w,h);
            assert(findData);
            return true;
        }
    }catch(const std::exception& e)
    {
        cout << "Database Failure:: " << e.what() << endl;
    }
    return false;
}

bool database::updateCharacter()
{
    try {
        userProfile& usrProfile = userProfile::getInstance();
        auto collectionProgress = db["UserProgress"];

        auto update_one_result = collectionProgress.update_one(
                make_document(kvp("_id", bsoncxx::oid(usrProfile.getId()))),
                make_document(kvp("$set", make_document(kvp("character", make_array(usrProfile.getCharacter()[0],
                                                                                    usrProfile.getCharacter()[1],
                                                                                    usrProfile.getCharacter()[2],
                                                                                    usrProfile.getCharacter()[3],
                                                                                    usrProfile.getCharacter()[4],
                                                                                    usrProfile.getCharacter()[5],
                                                                                    usrProfile.getCharacter()[6],
                                                                                    usrProfile.getCharacter()[7]))))));

        assert(update_one_result);  // Acknowledged writes return results.
        return true;
    }catch(const std::exception& e)
    {
        cout << "Database Failure:: " << e.what() << endl;
    }
    return false;
}

bool database::updatePassword(usrProfile& profile)
{
    try{
        auto collection = db["UserAuth"];

        auto update_one_result = collection.update_one(
                make_document(kvp("_id", bsoncxx::oid(profile._id))),
                make_document(kvp("$set", make_document(kvp("password", profile.hash), kvp("salt", profile.salt)))));

        assert(update_one_result);  // Acknowledged writes return results.
        assert(update_one_result->modified_count() == 1);
        return true;
    }catch(const std::exception& e)
    {
        cout << "Database Failure:: " << e.what() << endl;
    }
    return false;
}

bool database::updateUsername(string newUsr)
{
    try{
        userProfile& usrProfile = userProfile::getInstance();

        auto collection = db["UserAuth"];

        auto findUsr = collection.find_one(make_document(kvp("username", newUsr)));
        if(findUsr) // make sure this username has not been used
        {
            assert(findUsr);
            return false;
        }

        auto update_one_result = collection.update_one(
                make_document(kvp("_id", bsoncxx::oid(usrProfile.getId()))),
                make_document(kvp("$set", make_document(kvp("username", newUsr)))));

        assert(update_one_result);  // Acknowledged writes return results.
        assert(update_one_result->modified_count() == 1);

        usrProfile.setUsername(newUsr);
        return true;
    }catch(const std::exception& e)
    {
        cout << "Database Failure:: " << e.what() << endl;
    }
    return false;
}

bool database::getQuestion(int num, questionData& data)
{
    try {
        auto collection = db_Q["questions"];

        auto findQuestion= collection.find_one(make_document(kvp("num", num)));
        if(findQuestion)
        {
            data.id = num;

            bsoncxx::document::element db_question = findQuestion.value()["question"];
            data.question = db_question.get_string().value.to_string();

            bsoncxx::document::element db_boiler = findQuestion.value()["boilerPlate"];
            assert(db_boiler.type() == bsoncxx::type::k_array);
            bsoncxx::array::view boiler = db_boiler.get_array();
            for (auto iter: boiler) {
                assert(iter.type() == bsoncxx::type::k_string); // if string
                data.boiler.push_back(iter.get_string().value.data());
            }

            bsoncxx::document::element db_cases = findQuestion.value()["testInput"];
            assert(db_cases.type() == bsoncxx::type::k_array);
            bsoncxx::array::view cases = db_cases.get_array();
            for (auto iter: cases) {
                assert(iter.type() == bsoncxx::type::k_string); // if string
                data.cases.push_back(iter.get_string().value.data());
            }

            bsoncxx::document::element db_res = findQuestion.value()["testResult"];
            assert(db_res.type() == bsoncxx::type::k_array);
            bsoncxx::array::view res = db_res.get_array();
            for (auto iter: res) {
                assert(iter.type() == bsoncxx::type::k_string); // if string
                data.cases.push_back(iter.get_string().value.data());
            }

            assert(findQuestion);
            return true;
        }
    }catch(const std::exception& e)
    {
        cout << "Failed to get collection. :: " << e.what() << endl;
    }
    return false;
}

bool database::updateSettings()
{
    try {
        userProfile& usrProfile = userProfile::getInstance();
        auto collectionProgress = db["UserProgress"];

        auto update_one_result = collectionProgress.update_one(
                make_document(kvp("_id", bsoncxx::oid(usrProfile.getId()))),
                make_document(kvp("$set", make_document(kvp("settings", make_array(usrProfile.getResolution()->first,
                                                                                   usrProfile.getResolution()->second))))));

        assert(update_one_result);  // Acknowledged writes return results.
        return true;
    }catch(const std::exception& e)
    {
        cout << "Database Failure:: " << e.what() << endl;
    }
    return false;
}