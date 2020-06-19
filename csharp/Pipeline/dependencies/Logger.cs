namespace Pipeline.dependencies
{
    public interface Logger
    {
        void Info(string message);

        void Error(string message);
    }
}