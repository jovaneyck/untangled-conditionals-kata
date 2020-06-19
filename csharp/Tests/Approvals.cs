using System;
using System.Linq;
using System.Text;
using ApprovalTests.Reporters;
using ApprovalTests.Reporters.TestFrameworks;
using NSubstitute;
using Pipeline.dependencies;
using Xunit;

namespace Tests
{
    [UseReporter(typeof(XUnit2Reporter))]
    public class Approvals
    {
        [Fact]
        public void Scenario()
        {
            ApprovalTests.Combinations.CombinationApprovals.VerifyAllCombinations(
                RunPipeline,
                new []{true,false},
                new[]{true,false},
                Enum.GetValues(typeof(TestStatus)).OfType<TestStatus>());
        }

        private static string RunPipeline(bool sendEmails, bool buildsSuccessfully, TestStatus testStatus)
        {
            var log = new StringBuilder();

            var config = Substitute.For<Config>();

            config
                .SendEmailSummary()
                .Returns(sendEmails);

            var emailer = Substitute.For<Emailer>();
            emailer
                .When(_ => _.Send(Arg.Any<string>()))
                .Do(call => log.AppendLine(
                    $"Sending mail with message <{string.Join(",", call.Args().Select(_ => _.ToString()))}>"));

            var logger = Substitute.For<Logger>();
            logger
                .When(_ => _.Info(Arg.Any<string>()))
                .Do(call => log.AppendLine($"Logging INFO: <{string.Join(",", call.Args().Select(_ => _.ToString()))}>"));
            logger
                .When(_ => _.Error(Arg.Any<string>()))
                .Do(call => log.AppendLine($"Logging ERROR: <{string.Join(",", call.Args().Select(_ => _.ToString()))}>"));

            var pipeline = new Pipeline.Pipeline(config, emailer, logger);

            var project = new Project.ProjectBuilder()
                .SetDeploysSuccessfully(buildsSuccessfully)
                .SetTestStatus(testStatus)
                .Build();

            pipeline.Run(project);

            return log.ToString();
        }
    }
}
