#include <string>

void Usage(char *progname);
void CleanUp();
int InitAndConn(int argc, char **argv);
std::string SendAndWaitResponse(std::string str);
std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);
