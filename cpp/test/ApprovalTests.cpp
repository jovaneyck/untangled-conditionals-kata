#include "ApprovalTests.hpp"
#include "catch2/catch.hpp"

TEST_CASE("PipelineApprovals")
{
    ApprovalTests::Approvals::verify("Hello. Please approve me!");
}