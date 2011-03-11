/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010 Abyx. MIT License.
 *
 * Some helpers
 */
#pragma once

#pragma managed(push, off)

namespace hook {

/* Aux class for hooks by VA
 */
template<int BASE>
struct VA
{
    static void* ptr(int va) { return (char*)BASE + va; }
};

} // namespace hook

#pragma managed(pop)
