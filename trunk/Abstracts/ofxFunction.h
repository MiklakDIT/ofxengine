#pragma once

#include "ofxEngine.h"
#include <exception>

using namespace std;

template <typename ReturnType, typename ArgumentType>
class ofxFunction {
public:
	typedef ofxFunction<ReturnType,ArgumentType> FunctionType;

	ofxFunction() :m_throwable(true) {}
	virtual ~ofxFunction() = 0;
	
	// Internal exception class, for handling arguments which don't fit the function.
	class InvalidException : public exception {
	public:
		InvalidException(FunctionType& func, ArgumentType& badarg)
		{
			m_funcPtr = func;
			m_badargPtr = badarg;
		}
		const char* what() const {
			string ret = string("Invalid argument(s) passed to function ");
			ret += string( m_funcPtr->getFunctionName() );
			ret += string(".\n\tArgument(s): ");
			ret += string( m_funcPtr->toString( getArgument() ) );
			return ret.c_str();
		}
		FunctionType& getFunction() const { return *m_funcPtr; }
		ArgumentType& getArgument() const { return *m_badargPtr; }
	protected:
		FunctionType* m_funcPtr;
		ArgumentType* m_badargPtr;
	};
	
	// NOTE: In order to properly set a function as throwable, you must define 
	void setThrowable(bool throwable) {
		m_throwable = throwable;
	}
	bool isThrowable() const {
		return m_throwable;
	}
	
	// Checks to see if the passed argument is a valid argument.
	// For a mathematical function, this could be used to check for limits or out-of-range arguments.
	// For an objective function, this could be used to prevent values which do not qualify.
	// For a string function, this could be used to prevent 0-length or NULL strings.
	// For a multipurpose function, this could be used for type-checking.
	// In summary, you can use this to make sure your function receives only safe values.
	virtual bool isValid(ArgumentType& arg) = 0;
	
	// Retrieves a function name, specified by the derived class.
	// It's probably a really good idea to have function names hardcoded into the derived class.
	virtual const char* getFunctionName() const = 0;

	// Converts an argument of the proper type to a C-style string, for error reporting.
	// NOTE: For integer or floating-point types, you can use "ofToString(arg).c_str()".
	virtual const char* toString(ArgumentType& argument) const = 0;
	
	// The public-access evaluation method. Also checks for invalid arguments, if specified.
	ReturnType operator(ArgumentType& argument) {
		if (isThrowable()) {
			if (!isValid(argument)) {
				throw FunctionType::InvalidException(*this,argument);
			}
		}
		return this->eval(argument);
	}
	
protected:
	// Evaluates the value of the function at (arg).
	// Calling this directly will bypass 
	// Note that calling `this()` (the unary parentheses operator of `this`) from within an overload of eval() results in recursion.
	virtual ReturnType eval(ArgumentType arg) = 0;
	
	bool m_throwable;
};
