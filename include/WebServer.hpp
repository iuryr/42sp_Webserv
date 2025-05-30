#ifndef _WEBSERVER_HPP_
#define _WEBSERVER_HPP_

#define MAX_EVENTS 10
#define TIMEOUT 60
#define CGI_TIMEOUT 20
//4MB
#define MAX_BODY_SIZE 4000000

#include "Config.hpp"
#include "Connection.hpp"
#include "Logger.hpp"
#include "VirtualServer.hpp"
#include "Cgi.hpp"
#include "utils.hpp"

#include <map>
#include <set>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <fstream> //ifstream
#include <fcntl.h>
#include <dirent.h> //opendir
#include <errno.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <csignal>

class WebServer
{
  private:
    int _epollFd;
    Config _config;
    static bool _running;
    std::set<uint16_t> _ports;
    std::map<int, std::pair<uint32_t, uint16_t> > _socketsToPairs;
    std::map<int, Cgi*> _cgiMap;
    std::map<std::pair<uint32_t, uint16_t>,
             std::map<std::string, VirtualServer> >
        _virtualServersLookup;
    std::map<std::pair<uint32_t, uint16_t>, VirtualServer*>
        _virtualServersDefault;

    std::map<int, Connection> _connectionsMap;

    // provisory
    std::set<std::string> _implementedMethods;
    std::set<std::string> _unimplementedMethods;

    Logger _logger;
    static void signalHandler(int signum);

  public:
    WebServer(const std::string& configFile);
    ~WebServer(void);
    void init(void);
    void run(void);

    void setUpSockets(void);
    void bindSocket(void);
    void startListening(void);

    void addSocketsToEpoll(void);
    void modifyEventInterest(int epollFd, int eventFd, uint32_t event);

    int acceptConnection(int epollFd, int eventFd);
    void checkConnectionTimeouts(void);
    void checkCgiTimeouts(void);

    void parseRequest(Connection& connection);
    void parseRequestLine(std::string& connectionBuffer, Request& request);
    void parseMethod(std::string& requestLine, Request& request);
    void parseTarget(std::string& requestLine, Request& request);
    void parseQueryString(std::string& requestTarget, Request& request);
    void parseVersion(std::string& requestLine, Request& request);

    void parseHeader(std::string& connectionBuffer, Request& request);
    void identifyVirtualServer(Connection& connection);
    std::string captureFieldName(std::string& fieldLine);
    std::string captureFieldValues(std::string& fieldLine);
    void validateHeader(Connection& connection);

    void parseBody(Connection& connection);

	void fillBodyWithErrorPage(Connection& connection);
    void buildResponseBuffer(Connection& connection);
    void fillResponse(Connection& connection);
    void handleGET(Connection& connection);
    void handlePOST(Connection& connection);
    void handleDELETE(Connection& connection);

    bool isCgiRequest(Connection& connection, Location& location);
    void registerCgiPipe(int pipeFd, Cgi* cgiInstance, uint32_t events);
    void buildCgiResponse(Response& response, std::string& cgiOutput);
    int consumeNetworkBuffer(int connectionFd, std::string& connectionBuffer);
    void cleanup(void);
};

#endif //_WERBSERVER_HPP_
