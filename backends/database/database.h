#pragma once

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

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

// database implemented as a singleton class
class database
{
    mongocxx::instance instance; // MongoDB instance
    mongocxx::client client; // MongoDB client
    mongocxx::database db; // MongoDB database

    mongocxx::client client_Q; // MongoDB client
    mongocxx::database db_Q; // MongoDB database

    database();
    ~database();
    database(const database&) = delete;
    database& operator=(const database&) = delete;

public:
    struct usrProfile
    {
    public:
        string _id = "";
        string username = "";
        string hash = "";
        string salt = "";

        void completeAuth()
        {
            hash.clear();
            salt.clear();
        }

    };

    struct questionData
    {
    public:
        int id;
        string question = "";
        vector<string> boiler;
        vector<string> cases;
        vector<string> results;
    };

    static database& getInstance();
    void connect();

     bool getUserAuth(string usr, usrProfile& profile);
     bool makeUser(usrProfile& profile);

     bool getUserData();
     bool updatePassword(usrProfile& profile);
     bool updateUsername(string newUsr);
     bool updateCharacter();
     bool updateSettings();

     bool getQuestion(int num, questionData& data);
};
