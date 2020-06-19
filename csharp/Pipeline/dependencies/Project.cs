namespace Pipeline.dependencies
{
    public class Project
    {
        private readonly bool buildsSuccessfully;
        private readonly TestStatus testStatus;

        public static ProjectBuilder Builder()
        {
            return new ProjectBuilder();
        }

        private Project(bool buildsSuccessfully, TestStatus testStatus)
        {
            this.buildsSuccessfully = buildsSuccessfully;
            this.testStatus = testStatus;
        }

        public bool HasTests()
        {
            return testStatus != TestStatus.NO_TESTS;
        }

        public string RunTests()
        {
            return testStatus == TestStatus.PASSING_TESTS ? "success" : "failure";
        }

        public string Deploy()
        {
            return buildsSuccessfully ? "success" : "failure";
        }

        public class ProjectBuilder
        {
            private bool buildsSuccessfully;
            private TestStatus testStatus;

            public ProjectBuilder SetTestStatus(TestStatus testStatus)
            {
                this.testStatus = testStatus;
                return this;
            }

            public ProjectBuilder SetDeploysSuccessfully(bool buildsSuccessfully)
            {
                this.buildsSuccessfully = buildsSuccessfully;
                return this;
            }

            public Project Build()
            {
                return new Project(buildsSuccessfully, testStatus);
            }
        }
    }
}