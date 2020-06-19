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
            var log = new StringBuilder();

            var config = Substitute.For<Config>();
            config
                .SendEmailSummary()
                .Returns(true);

            var emailer = Substitute.For<Emailer>();
            emailer
                .When(_ => _.Send(Arg.Any<string>()))
                .Do(call => log.AppendLine($"Sending mail with message <{string.Join(",", call.Args().Select(_=>_.ToString()))}>"));

            var logger = Substitute.For<Logger>();
            logger
                .When(_=>_.Info(Arg.Any<string>()))
                .Do(call => log.AppendLine($"Logging INFO: <{string.Join(",", call.Args().Select(_ => _.ToString()))}>"));
            logger
                .When(_=>_.Error(Arg.Any<string>()))
                .Do(call => log.AppendLine($"Logging ERROR: <{string.Join(",", call.Args().Select(_ => _.ToString()))}>"));

            var pipeline = new Pipeline.Pipeline(config, emailer, logger);

            var project = new Project.ProjectBuilder()
                .SetDeploysSuccessfully(true)
                .SetTestStatus(TestStatus.PASSING_TESTS)
                .Build();

            pipeline.Run(project);

            ApprovalTests.Approvals.Verify(log.ToString());
        }
    }
}
