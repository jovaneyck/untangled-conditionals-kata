#include "Pipeline.h"

void Pipeline::run(Project project)
{
    if(!project.hasTests()) {
        log.info("No tests");
        deploy("success" == project.deploy());
        return;
    }

    if(!("success" == project.runTests())) {
        log.error("Tests failed");
        send_email_if_enabled("Tests failed");
        return;
    }

    log.info("Tests passed");
    deploy("success" == project.deploy());
}

void Pipeline::deploy(bool succeeded) const
{
    if(!succeeded) {
        log.error("Deployment failed");
        send_email_if_enabled("Deployment failed");
        return;
    }

    log.info("Deployment successful");
    send_email_if_enabled("Deployment completed successfully");
}

void Pipeline::send_email_if_enabled(const std::string &msg) const
{
    if(!config.sendEmailSummary()) {
        log.info("Email disabled");
        return;
    }

    log.info("Sending email");
    emailer.send(msg);
}
