#include <bits/stdc++.h>
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

    if(!sampleTest){
        return "this is a frog";
    }
    else{
        return testSampleCode;
    }
}

string testFrameCpp::testTwo(bool sampleTest){
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