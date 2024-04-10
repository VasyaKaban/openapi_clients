#include "App/TestComponent.hpp"
#include "App/ClientApiTestClient.hpp"
#include "ClientControllerTest.h"

int main(int argc, char **argv)
{
	oatpp::base::Environment::init();
		OATPP_RUN_TEST(ClientControllerTest);
	oatpp::base::Environment::destroy();

	return 0;
}
