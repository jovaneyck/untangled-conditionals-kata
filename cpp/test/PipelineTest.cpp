#include "catch.hpp"

#include <vector>

#include "CapturingEmailer.h"
#include "CapturingLogger.h"
#include "Config.h"
#include "Emailer.h"
#include "Pipeline.h"
#include "Project.h"

TEST_CASE("Test with email enabled")
{
    Config config;
    CapturingEmailer emailer;
    CapturingLogger logger;
    Pipeline pipeline(config, emailer, logger);

    SECTION("Tests passed, deployment passed, email enabled")
    {
        Project project(true, PASSING_TESTS);
        pipeline.run(project);

        SECTION("Test logging")
        {
            std::vector<std::string> expected;
            expected.emplace_back("INFO: Tests passed");
            expected.emplace_back("INFO: Deployment successful");
            expected.emplace_back("INFO: Sending email");
            REQUIRE(expected == logger.getLoggedLines());
        }

        SECTION("Test email")
        {
            std::vector<std::string> expected;
            expected.emplace_back("Deployment completed successfully");
            REQUIRE(expected == emailer.getMail());
        }
    }

    SECTION("Tests passed, build failed, email enabled")
    {
        Project project(false, PASSING_TESTS);
        pipeline.run(project);

        SECTION("Test logging")
        {
            std::vector<std::string> expected;
            expected.emplace_back("INFO: Tests passed");
            expected.emplace_back("ERROR: Deployment failed");
            expected.emplace_back("INFO: Sending email");
            REQUIRE(expected == logger.getLoggedLines());
        }

        SECTION("Test email")
        {
            std::vector<std::string> expected;
            expected.emplace_back("Deployment failed");
            REQUIRE(expected == emailer.getMail());
        }
    }

    SECTION("Test failed test, deploy passed, email enabled")
    {
        Project project(true, FAILING_TESTS);
        pipeline.run(project);

        SECTION("Test logging")
        {
            std::vector<std::string> expected;
            expected.emplace_back("ERROR: Tests failed");
            expected.emplace_back("INFO: Sending email");
            REQUIRE(expected == logger.getLoggedLines());
        }
        SECTION("Test email")
        {
            std::vector<std::string> expected;
            expected.emplace_back("Tests failed");
            REQUIRE(expected == emailer.getMail());
        }
    }

    SECTION("No tests, build passed, email enabled")
    {
        Project project(true, NO_TESTS);
        pipeline.run(project);

        SECTION("Test logging when deploy passed") {
            std::vector<std::string> expected;
            expected.emplace_back("INFO: No tests");
            expected.emplace_back("INFO: Deployment successful");
            expected.emplace_back("INFO: Sending email");
            REQUIRE(expected == logger.getLoggedLines());
        }
        SECTION("Test email with no tests and deploy passed")
        {
            std::vector<std::string> expected;
            expected.emplace_back("Deployment completed successfully");
            REQUIRE(expected == emailer.getMail());
        }
    }

    SECTION("No tests, build failed, send email")
    {
        Project project(false, NO_TESTS);
        pipeline.run(project);

        SECTION("Test logging when deploy passed") {
            std::vector<std::string> expected;
            expected.emplace_back("INFO: No tests");
            expected.emplace_back("ERROR: Deployment failed");
            expected.emplace_back("INFO: Sending email");
            REQUIRE(expected == logger.getLoggedLines());
        }
        SECTION("Test email with no tests and deploy passed")
        {
            std::vector<std::string> expected;
            expected.emplace_back("Deployment failed");
            REQUIRE(expected == emailer.getMail());
        }
    }
}

TEST_CASE("Test with email disabled")
{
    Config config(false);
    CapturingEmailer emailer;
    CapturingLogger logger;
    Pipeline pipeline(config, emailer, logger);

    SECTION("Tests passed, build passed, email disabled")
    {
        Project project(true, PASSING_TESTS);
        pipeline.run(project);

        SECTION("Test logging")
        {
            std::vector<std::string> expected;
            expected.emplace_back("INFO: Tests passed");
            expected.emplace_back("INFO: Deployment successful");
            expected.emplace_back("INFO: Email disabled");
            REQUIRE(expected == logger.getLoggedLines());
        }
        SECTION("Test email")
        {
            REQUIRE(emailer.getMail().empty());
        }
    }

    SECTION("Tests passed, build failed, with email disabled")
    {
        Project project(false, PASSING_TESTS);
        pipeline.run(project);

        SECTION("Test logging")
        {
            std::vector<std::string> expected;
            expected.emplace_back("INFO: Tests passed");
            expected.emplace_back("ERROR: Deployment failed");
            expected.emplace_back("INFO: Email disabled");
            REQUIRE(expected == logger.getLoggedLines());
        }
        SECTION("Test email")
        {
            REQUIRE(emailer.getMail().empty());
        }
    }

    SECTION("Tests failed, deployment failed, email disabled")
    {
        Project project(false, FAILING_TESTS);
        pipeline.run(project);

        SECTION("Test logging")
        {
            std::vector<std::string> expected;
            expected.emplace_back("ERROR: Tests failed");
            expected.emplace_back("INFO: Email disabled");
            REQUIRE(expected == logger.getLoggedLines());
        }
        SECTION("Test email")
        {
            REQUIRE(emailer.getMail().empty());
        }
    }

    SECTION("No tests, build passed, email disabled")
    {
        Project project(true, NO_TESTS);
        pipeline.run(project);

        SECTION("Test logging")
        {
            std::vector<std::string> expected;
            expected.emplace_back("INFO: No tests");
            expected.emplace_back("INFO: Deployment successful");
            expected.emplace_back("INFO: Email disabled");
            REQUIRE(expected == logger.getLoggedLines());
        }
        SECTION("Test email")
        {
            REQUIRE(emailer.getMail().empty());
        }
    }

    SECTION("No tests, build failed, email disabled")
    {
        Project project(false, NO_TESTS);
        pipeline.run(project);

        SECTION("Test logging")
        {
            std::vector<std::string> expected;
            expected.emplace_back("INFO: No tests");
            expected.emplace_back("ERROR: Deployment failed");
            expected.emplace_back("INFO: Email disabled");
            REQUIRE(expected == logger.getLoggedLines());
        }
        SECTION("Test email")
        {
            REQUIRE(emailer.getMail().empty());
        }
    }
}
