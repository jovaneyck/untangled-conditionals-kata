#include "CapturingEmailer.h"
#include "CapturingLogger.h"
#include <Config.h>
#include <Pipeline.h>

#include "ApprovalTests.hpp"
#include "catch2/catch.hpp"

static std::string run(bool email_enabled, bool build_successful,
                       TestStatus test_status)
{
    // Application setup
    Config config(email_enabled);
    CapturingEmailer emailer;
    CapturingLogger logger;

    Pipeline pipeline(config, emailer, logger);
    Project project(build_successful, test_status);

    // Run the scenario
    pipeline.run(project);

    // Get results
    std::string output = "\n";
    for(const std::string &msg : logger.getLoggedLines()) {
        output += msg + "\n";
    }
    for(const std::string &mail : emailer.getMail()) {
        output += mail + "\n";
    }

    return output;
}

TEST_CASE("PipelineApprovals")
{
    // Permutations
    std::vector<bool> email_enabled{ true, false };
    std::vector<bool> build_successful{ true, false };
    std::vector<TestStatus> test_status{ TestStatus::PASSING_TESTS,
                                         TestStatus::FAILING_TESTS,
                                         TestStatus::NO_TESTS };

    ApprovalTests::CombinationApprovals::verifyAllCombinations(
        [](bool email_enabled, bool build_successful, TestStatus test_status) {
            return run(email_enabled, build_successful, test_status);
        },
        email_enabled, build_successful, test_status);
}

inline std::ostream &operator<<(std::ostream &os, TestStatus ts)
{
    switch(ts) {
    case NO_TESTS:
        os << "No Tests";
        break;
    case PASSING_TESTS:
        os << "Tests Passed";
        break;
    case FAILING_TESTS:
        os << "Tests Failed";
        break;
    }
    return os;
}

