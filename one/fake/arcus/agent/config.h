#pragma once

#include <one/arcus/internal/rapidjson/reader.h>
#include <one/arcus/types.h>

namespace rapidjson = RAPIDJSON_NAMESPACE;

namespace i3d {
namespace one {

struct Configuration {
    int port = 19001;               // Port number for the server to listen on.
    String address = "127.0.0.1";   // Address for the server to bind to.
    bool stressTest = false;        // Whether to run in stress test mode.
    bool sendPragmaTokens = false;  // Whether to send Pragma tokens as metadata payload with the allocated command.
    String pragmaGameToken = "";    // pragma game token.
    String pragmaSocialToken = "";  // pragma social token.
};

struct JSONMessageHandler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, JSONMessageHandler>
{
    int type;
    const char *name;
    bool boolValue;
    int intValue;
    i3d::one::String stringValue;

    JSONMessageHandler()
        : type(0),
          name(nullptr),
          boolValue(false),
          intValue(0),
          stringValue("")
    {
    }

    bool Key(const char *str, rapidjson::SizeType length, bool)
    {
        type = 1;
        name = str;
        return true;
    }

    bool Bool(bool b)
    {
        type = 2;
        boolValue = b;
        return true;
    }

    bool Int(int i)
    {
        type = 3;
        intValue = i;
        return true;
    }

    bool Uint(unsigned u)
    {
        type = 3;
        intValue = static_cast<int>(u);
        return true;
    }

    bool String(const char *str, rapidjson::SizeType length, bool copy)
    {
        type = 4;
        stringValue = i3d::one::String(str, length);
        return true;
    }

    bool Default()
    {
        type = 0;
        return true;
    }

private:
    JSONMessageHandler(const JSONMessageHandler &noCopyConstruction);
};

bool LoadConfiguration(const i3d::one::String& filename, Configuration& configuration);
void CreateDefaultConfiguration(const i3d::one::String& filename, const Configuration& configuration);


}  // namespace one
}  // namespace i3d