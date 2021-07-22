#ifndef CONSTANT_HPP
#define CONSTANT_HPP
#include <string>

using namespace std;

namespace object {
enum class OBJECT_TYPE {
    NULL_OBJ,  //  "NULL"
    ERROR_OBJ, //  "ERROR"

    STRING_OBJ, // "STRING"

    INTEGER_OBJ, //  "INTEGER"
    BOOLEAN_OBJ, //  "BOOLEAN"

    RETURN_VALUE_OBJ, //  "RETURN_VALUE"

    FUNCTION_OBJ, //  "FUNCTION"

    BUILTIN_OBJ, // "BUILTIN"

    ARRAY_OBJ, //= "ARRAY"
    HASH_OBJ,  //= "HASH"
};

const string OBJECT_TYPE_VALUE[] = {

    "NULL",  // NULL_OBJ
    "ERROR", // ERROR_OBJ
    "STRING",

    "INTEGER", // INTEGER_OBJ
    "BOOLEAN", // BOOLEAN_OBJ

    "RETURN_VALUE", // RETURN_VALUE_OBJ

    "FUNCTION", // FUNCTION_OBJ
    "BUILTIN",

    "ARRAY",
    "HASH"
};

} // namespace object

#endif /* CONSTANT_HPP */
