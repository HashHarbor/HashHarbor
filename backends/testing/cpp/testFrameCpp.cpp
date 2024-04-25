#if defined(__APPLE__)
#include <iostream>
#include <vector>
#include <fstream>
#include <thread>
#include <chrono>
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::pair;
using std::ofstream;
#else
#include <bits/stdc++.h>
#endif
#include <string>
#include "testFrameCpp.h"
using namespace std;

string testFrameCpp::runTest(string code, int questionNumber, bool sampleTest){
    string exeCode = "";
    string setup = "#include <bits/stdc++.h> \n";
    setup += "using namespace std; \n";

    switch (questionNumber)
    {
    case 1:
        exeCode = testOne(sampleTest);
        break;
    
    case 2:
        exeCode = testTwo(sampleTest);
        break;
    
    case 3:
        exeCode = testThree(sampleTest);
        break;
    
    default:
        return "woops, something went wrong!";
    }

    code = setup + code + exeCode;

    std::ofstream file("temp.cpp");
    file << code;
    file.close();

    // Step 2: Invoke the compiler and capture output
    std::string compileCommand = "g++ -o temp temp.cpp 2>&1"; // Redirect stderr to stdout
    FILE* pipe = popen(compileCommand.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error invoking compiler command." << std::endl;
        return "Error invoking compiler command.";
    }

    char buffer[128];
    std::string compileOutput = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            compileOutput += buffer;
    }
    pclose(pipe);

    if (!compileOutput.empty()) {  //has error
        remove("temp.cpp");
        return compileOutput;
    }

    FILE* executePipe = popen("./temp 2>&1", "r");
    if (!executePipe) {
        std::cerr << "Error invoking execute command." << std::endl;
        remove("temp.cpp");
        remove("temp");
        return "Error invoking execute command.";
    }

    char executeBuffer[128];
    std::string executeOutput = "";
    while (!feof(executePipe)) {
        if (fgets(executeBuffer, 128, executePipe) != NULL)
            executeOutput += executeBuffer;
    }
    pclose(executePipe);

    // Clean up temporary files
    remove("temp.cpp");
    remove("temp");

    return executeOutput;
}

string testFrameCpp::testOne(bool sampleTest){
    string testSampleCode = "int main() {\n"
              "    vector<int> difficulties = {7, 5, 8, 2};\n"
              "    vector<string> topics = {\"Graphs\", \"Sorting\", \"Dynamic Programming\", \"HashHarbor\"};\n\n"
              "    string easiest = EasiestTopic(difficulties, topics);\n"
              "    cout << \"NPC should start with the topic: \" << easiest << endl;\n\n"
              "    return 0;\n"
              "}";

    string testCode = "int main() {\n"
                      "    int passed = 0;\n"
                      "    int failed = 0;\n\n"
                      "    vector<int> difficulties;\n"
                      "    vector<int> passTests;\n"
                      "    vector<string> topics;\n"
                      "    string expected;\n\n"
                      "    difficulties = {7, 5, 8, 2};\n"
                      "    topics = {\"Graphs\", \"Sorting\", \"Dynamic Programming\", \"HashHarbor\"};\n"
                      "    expected = \"HashHarbor\";\n"
                      "    if (EasiestTopic(difficulties, topics) == expected) {\n"
                      "        passed++;\n"
                      "        passTests.push_back(1);\n"
                      "    } else {\n"
                      "        failed++;\n"
                      "    }\n\n"
                      "    difficulties = {1};\n"
                      "    topics = {\"Easy Topic\"};\n"
                      "    expected = \"Easy Topic\";\n"
                      "    if (EasiestTopic(difficulties, topics) == expected) {\n"
                      "        passed++;\n"
                      "        passTests.push_back(2);\n"
                      "    } else {\n"
                      "        failed++;\n"
                      "    }\n\n"
                      "    difficulties = {100, 50, 75, 25};\n"
                      "    topics = {\"Hard\", \"Medium\", \"Difficult\", \"Easy\"};\n"
                      "    expected = \"Easy\";\n"
                      "    if (EasiestTopic(difficulties, topics) == expected) {\n"
                      "        passed++;\n"
                      "        passTests.push_back(3);\n"
                      "    } else {\n"
                      "        failed++;\n"
                      "    }\n\n"
                      "    difficulties = {5, 3, 8, 1, 2};\n"
                      "    topics = {\"Hash\", \"Bit\", \"Tree\", \"Easy\", \"Medium\"};\n"
                      "    expected = \"Easy\";\n"
                      "    if (EasiestTopic(difficulties, topics) == expected) {\n"
                      "        passed++;\n"
                      "        passTests.push_back(4);\n"
                      "    } else {\n"
                      "        failed++;\n"
                      "    }\n\n"
                      "    difficulties = {100, 50, 75, 25, 75, 50, 26, 100, 75, 50};\n"
                      "    topics = {\"A\", \"B\", \"C\", \"D\", \"E\", \"F\", \"G\", \"H\", \"I\", \"J\"};\n"
                      "    expected = \"D\";\n"
                      "    if (EasiestTopic(difficulties, topics) == expected) {\n"
                      "        passed++;\n"
                      "        passTests.push_back(5);\n"
                      "    } else {\n"
                      "        failed++;\n"
                      "    }\n\n"
                      "    difficulties = {-1, -2, -3, -4, -5, -60, -7, -8, -9, -10};\n"
                      "    topics = {\"A\", \"B\", \"C\", \"D\", \"E\", \"F\", \"G\", \"H\", \"I\", \"J\"};\n"
                      "    expected = \"F\";\n"
                      "    if (EasiestTopic(difficulties, topics) == expected) {\n"
                      "        passed++;\n"
                      "        passTests.push_back(6);\n"
                      "    } else {\n"
                      "        failed++;\n"
                      "    }\n\n"
                      "    difficulties = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};\n"
                      "    topics = {\"One\", \"Two\", \"Three\", \"Four\", \"Five\", \"Six\", \"Seven\", \"Eight\", \"Nine\", \"Ten\"};\n"
                      "    expected = \"One\";\n"
                      "    if (EasiestTopic(difficulties, topics) == expected) {\n"
                      "        passed++;\n"
                      "        passTests.push_back(7);\n"
                      "    } else {\n"
                      "        failed++;\n"
                      "    }\n\n"
                      "    difficulties = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};\n"
                      "    topics = {\"Hard\", \"Harder\", \"Hardest\", \"Difficult\", \"Challenging\", \"Moderate\", \"Easy\", \"Easier\", \"Easiest\", \"Trivial\"};\n"
                      "    expected = \"Trivial\";\n"
                      "    if (EasiestTopic(difficulties, topics) == expected) {\n"
                      "        passed++;\n"
                      "        passTests.push_back(8);\n"
                      "    } else {\n"
                      "        failed++;\n"
                      "    }\n\n"
                      "    difficulties = {100, 99, 98, 97, 96, 85, 94, 93, 92, 91};\n"
                      "    topics = {\"Extreme\", \"Extremely Difficult\", \"Very Hard\", \"Hard\", \"Difficult\", \"oink\", \"Moderate\", \"Easy\", \"Very Easy\", \"Trivial\"};\n"
                      "    expected = \"oink\";\n"
                      "    if (EasiestTopic(difficulties, topics) == expected) {\n"
                      "        passed++;\n"
                      "        passTests.push_back(9);\n"
                      "    } else {\n"
                      "        failed++;\n"
                      "    }\n\n"
                      "    difficulties = {100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 2, 75, 74, 73, 72, 71 };\n"
                      "    topics = {\"Extreme\", \"Extremely Difficult\", \"Very Hard\", \"Hard\", \"Difficult\", \"Challenging\", \"Moderate\", \"Easy\", \"Very Easy\", \"Trivial\", \n"
                      "              \"Extreme\", \"Extremely Difficult\", \"Very Hard\", \"Hard\", \"Difficult\", \"Challenging\", \"Moderate\", \"Easy\", \"Very Easy\", \"Trivial\", \n"
                      "              \"Extreme\", \"Extremely Difficult\", \"Very Hard\", \"Hard\", \"Trivial\", \"Challenging\", \"Moderate\", \"Easy\", \"Very Easy\", \"Trivial\"};\n"
                      "    expected = \"Trivial\";\n"
                      "    if (EasiestTopic(difficulties, topics) == expected) {\n"
                      "        passed++;\n"
                      "        passTests.push_back(10);\n"
                      "    } else {\n"
                      "        failed++;\n"
                      "    }\n\n"
                      "    cout << \"valid \" << passed << \" test cases passed out of \" << passed + failed << endl;\n"
                      "    for(auto x : passTests){\n"
                      "        cout << x << endl; \n"
                      "    }\n\n"
                      "    return 0;\n"
                      "}";

    if(!sampleTest){
        return testCode;
    }
    else{
        return testSampleCode;
    }
}

string testFrameCpp::testTwo(bool sampleTest){
    string testSampleCode = "int main() {\n"
                      "    string input = \"abcdefghijklmnopqrstuvwxyz\";\n"
                      "    bool valid = AlphabetPasta(input);\n"
                      "    if(valid){cout << \"Sample 1: It is [ \" << \"true\" << \" ] that this pasta has every alphabet!\" << endl;}\n"
                      "    else{cout << \"Sample 1: It is [ \" << \"false\" << \" ] that this pasta has every alphabet!\" << endl;}\n\n"
                      "    input = \"xaybzc\";\n"
                      "    bool expect = false;\n"
                      "    valid = AlphabetPasta(input);\n"
                      "    if(valid){cout << \"Sample 2: It is [ \" << \"true\" << \" ] that this pasta has every alphabet!\" << endl;}\n"
                      "    else{cout << \"Sample 2: It is [ \" << \"false\" << \" ] that this pasta has every alphabet!\" << endl;}\n"
                      "    return 0;\n"
                      "}";

    string testCode = "int main() {\n"
                      "    int passed = 0;\n"
                      "    int failed = 0;\n\n"
                      "    vector<int> passTests;\n\n"
                      "    string input = \"abcdefghijklmnopqrstuvwxyz\";\n"
                      "    bool expect = true;\n"
                      "    if(AlphabetPasta(input) == expect){\n"
                      "        passed++;\n"
                      "        passTests.push_back(1);\n"
                      "    }\n"
                      "    else{\n"
                      "        failed++;\n"
                      "    }\n\n"
                      "    input = \"xaybzc\";\n"
                      "    expect = false;\n"
                      "    if(AlphabetPasta(input) == expect){\n"
                      "        passed++;\n"
                      "        passTests.push_back(2);\n"
                      "    }\n"
                      "    else{\n"
                      "        failed++;\n"
                      "    }\n\n"
                      "    input = \"qwertyuiopasdfghjklzxcvbnm\";\n"
                      "    expect = true;\n"
                      "    if (AlphabetPasta(input) == expect) {\n"
                      "        passed++;\n"
                      "        passTests.push_back(3);\n"
                      "    } else {\n"
                      "        failed++;\n"
                      "    }\n\n"
                      "    input = \"abcdefghijklmnopqrstuvwxy\";\n"
                      "    expect = false;\n"
                      "    if (AlphabetPasta(input) == expect) {\n"
                      "        passed++;\n"
                      "        passTests.push_back(4);\n"
                      "    } else {\n"
                      "        failed++;\n"
                      "    }\n\n"
                      "    input = \"bcdefghijklmnopqrstuvwxyza\";\n"
                      "    expect = true;\n"
                      "    if (AlphabetPasta(input) == expect) {\n"
                      "        passed++;\n"
                      "        passTests.push_back(5);\n"
                      "    } else {\n"
                      "        failed++;\n"
                      "    }\n\n"
                      "    input = \"aabcdefghijklmnopqrstuvwxyz\";\n"
                      "    expect = true;\n"
                      "    if (AlphabetPasta(input) == expect) {\n"
                      "        passed++;\n"
                      "        passTests.push_back(6);\n"
                      "    } else {\n"
                      "        failed++;\n"
                      "    }\n\n"
                      "    input = \"abcdefghijklmnopqrstuvwxyzz\";\n"
                      "    expect = false;\n"
                      "    if (AlphabetPasta(input) == expect) {\n"
                      "        passed++;\n"
                      "        passTests.push_back(7);\n"
                      "    } else {\n"
                      "        failed++;\n"
                      "    }\n\n"
                      "    input = \"abcdefghijklmnopqrstuvwxy\";\n"
                      "    expect = false;\n"
                      "    if (AlphabetPasta(input) == expect) {\n"
                      "        passed++;\n"
                      "        passTests.push_back(8);\n"
                      "    } else {\n"
                      "        failed++;\n"
                      "    }\n\n"
                      "    input = \"bcdefghijklmnopqrstuvwxyza\";\n"
                      "    expect = true;\n"
                      "    if (AlphabetPasta(input) == expect) {\n"
                      "        passed++;\n"
                      "        passTests.push_back(9);\n"
                      "    } else {\n"
                      "        failed++;\n"
                      "    }\n\n"
                      "    input = \"zyxwvutsrqponmlkjihgfedcbaa\";\n"
                      "    expect = false;\n"
                      "    if (AlphabetPasta(input) == expect) {\n"
                      "        passed++;\n"
                      "        passTests.push_back(10);\n"
                      "    } else {\n"
                      "        failed++;\n"
                      "    }\n\n"
                      "    cout << \"valid \" << passed << \" test cases passed out of \" << passed + failed << endl;\n"
                      "    for(auto x: passTests) { cout << x << endl; }\n\n"
                      "    return 0;\n"
                      "}";

    if(!sampleTest){
        return testCode;
    }
    else{
        return testSampleCode;
    }
}


string testFrameCpp::testThree(bool sampleTest){
    string testSampleCode = "int main() {\n"
              "    vector<int> difficulties = {7, 5, 8, 2};\n"
              "    vector<string> topics = {\"Graphs\", \"Sorting\", \"Dynamic Programming\", \"HashHarbor\"};\n\n"
              "    string easiest = EasiestTopic(difficulties, topics);\n"
              "    cout << \"NPC should start with the topic: \" << easiest << endl;\n\n"
              "    return 0;\n"
              "}";

    if(!sampleTest){
        return "this is a frog";
    }
    else{
        return testSampleCode;
    }
}