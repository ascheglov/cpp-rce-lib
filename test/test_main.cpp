/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010 Abyx. MIT License.
 *
 * Tests initialization
 */
#pragma warning(push)
#pragma warning(disable: 4265 4619 4548)
#include <boost/test/unit_test.hpp>
#pragma warning(pop)

#include <locale>

boost::unit_test::test_suite* init_unit_test_suite(int /*argc*/, char* /*argv*/[])
{
	setlocale(LC_ALL, "");
	return nullptr;
}
