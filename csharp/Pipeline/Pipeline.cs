using Pipeline.dependencies;

namespace Pipeline
{
    public class Pipeline
    {
        private readonly Config config;
        private readonly Emailer emailer;
        private readonly Logger log;

        public Pipeline(Config config, Emailer emailer, Logger log)
        {
            this.config = config;
            this.emailer = emailer;
            this.log = log;
        }

        public void Run(Project project)
        {
            bool testsPassed;
            bool deploySuccessful;

            if (project.HasTests())
            {
                if ("success".Equals(project.RunTests()))
                {
                    log.Info("Tests passed");
                    testsPassed = true;
                }
                else
                {
                    log.Error("Tests failed");
                    testsPassed = false;
                }
            }
            else
            {
                log.Info("No tests");
                testsPassed = true;
            }

            if (testsPassed)
            {
                if ("success".Equals(project.Deploy()))
                {
                    log.Info("Deployment successful");
                    deploySuccessful = true;
                }
                else
                {
                    log.Error("Deployment failed");
                    deploySuccessful = false;
                }
            }
            else
            {
                deploySuccessful = false;
            }

            if (config.SendEmailSummary())
            {
                log.Info("Sending email");
                if (testsPassed)
                {
                    if (deploySuccessful)
                    {
                        emailer.Send("Deployment completed successfully");
                    }
                    else
                    {
                        emailer.Send("Deployment failed");
                    }
                }
                else
                {
                    emailer.Send("Tests failed");
                }
            }
            else
            {
                log.Info("Email disabled");
            }
        }
    }
}
