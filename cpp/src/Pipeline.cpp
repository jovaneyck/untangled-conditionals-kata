#include "Pipeline.h"

void Pipeline::run(Project project)
{
    if(project.hasTests()) {
        if("success" == project.runTests()) {
            log.info("Tests passed");
            deploy("success" == project.deploy());
        }
        else {
            log.error("Tests failed");
            send_email_if_enabled("Tests failed");
        }
    }
    else {
        log.info("No tests");
        deploy("success" == project.deploy());
    }
}

void Pipeline::deploy(bool succeeded) const
{
    if(succeeded) {
        log.info("Deployment successful");
        send_email_if_enabled("Deployment completed successfully");
    }
    else {
        log.error("Deployment failed");
        send_email_if_enabled("Deployment failed");
    }
}

void Pipeline::send_email_if_enabled(const std::string &msg) const
{
    if(config.sendEmailSummary()) {
        log.info("Sending email");
        emailer.send(msg);
    }
    else {
        log.info("Email disabled");
    }
}
