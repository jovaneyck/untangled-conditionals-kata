
#include "Config.h"
#include "Emailer.h"
#include "Logger.h"
#include "Project.h"

class Pipeline {
    Config &config;
    Emailer &emailer;
    Logger &log;

    void send_email_if_enabled(const std::string& msg) const;
    void deploy(bool succeeded) const;

  public:
    Pipeline(Config &config, Emailer &emailer, Logger &log)
        : config(config)
        , emailer(emailer)
        , log(log)
    {
    }

    void run(Project project);
};
