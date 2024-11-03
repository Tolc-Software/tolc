#include <IR/ir.hpp>
#include <Parser/Parse.hpp>

int main() {
	if (auto parsed = Parser::parseString("void f();")) {
		auto& globalNamespace = parsed.value();
		auto& functions = globalNamespace.m_functions;
		if (functions.size() == 1 && functions[0].m_name == "f") {
			// Success!
			return 0;
		}
	}

	// Fail
	return 1;
}
