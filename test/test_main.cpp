/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010 Abyx. MIT License.
 *
 * Tests initialization
 */
#include <boost/test/unit_test.hpp>
#include <locale>

boost::unit_test::test_suite* init_unit_test_suite(int /*argc*/, char* /*argv*/[])
{
	setlocale(LC_ALL, "");
	return nullptr;
}
