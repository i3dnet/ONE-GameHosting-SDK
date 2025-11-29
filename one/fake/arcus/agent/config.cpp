#include <one/fake/arcus/agent/config.h>

#include <one/arcus/internal/rapidjson/document.h>
#include <one/arcus/internal/rapidjson/istreamwrapper.h>
#include <one/arcus/internal/rapidjson/prettywriter.h>
#include <one/arcus/internal/rapidjson/stringbuffer.h>
#include <fstream>
#include <iostream>

namespace i3d {
namespace one {

bool LoadConfiguration(const i3d::one::String& filename, Configuration& configuration)
{
    std::ifstream file(filename.c_str());
    if (file.is_open())
    {
        // Parse the settings file.
        rapidjson::IStreamWrapper jsonStream(file);
        JSONMessageHandler jsonHandler;
        rapidjson::Reader jsonReader;

        jsonReader.IterativeParseInit();
        while (!jsonReader.IterativeParseComplete())
        {
            jsonReader.IterativeParseNext<rapidjson::kParseDefaultFlags>(jsonStream, jsonHandler);
            if (jsonHandler.type == 1)
            {
                if (strcmp(jsonHandler.name, "port") == 0)
                {
                    jsonReader.IterativeParseNext<rapidjson::kParseDefaultFlags>(jsonStream, jsonHandler);
                    if (jsonHandler.type == 3)
                    {
                        configuration.port = jsonHandler.intValue;
                    }
                }
                if (strcmp(jsonHandler.name, "address") == 0)
                {
                    jsonReader.IterativeParseNext<rapidjson::kParseDefaultFlags>(jsonStream, jsonHandler);
                    if (jsonHandler.type == 2)
                    {
                        configuration.address = jsonHandler.stringValue;
                    }
                }
                if (strcmp(jsonHandler.name, "stressTest") == 0)
                {
                    jsonReader.IterativeParseNext<rapidjson::kParseDefaultFlags>(jsonStream, jsonHandler);
                    if (jsonHandler.type == 2)
                    {
                        configuration.stressTest = jsonHandler.boolValue;
                    }
                }                
                if (strcmp(jsonHandler.name, "sendPragmaTokens") == 0)
                {
                    jsonReader.IterativeParseNext<rapidjson::kParseDefaultFlags>(jsonStream, jsonHandler);
                    if (jsonHandler.type == 2)
                    {
                        configuration.sendPragmaTokens = jsonHandler.boolValue;
                    }
                }
                if (strcmp(jsonHandler.name, "pragmaGameToken") == 0)
                {
                    jsonReader.IterativeParseNext<rapidjson::kParseDefaultFlags>(jsonStream, jsonHandler);
                    if (jsonHandler.type == 3)
                    {
                        configuration.pragmaGameToken = jsonHandler.stringValue;
                    }
                }
                if (strcmp(jsonHandler.name, "pragmaSocialToken") == 0)
                {
                    jsonReader.IterativeParseNext<rapidjson::kParseDefaultFlags>(jsonStream, jsonHandler);
                    if (jsonHandler.type == 3)
                    {
                        configuration.pragmaSocialToken = jsonHandler.stringValue;
                    }
                }                           
            }
        }
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}

void CreateDefaultConfiguration(const i3d::one::String& filename, const Configuration& configuration)
{
    // Create JSON string
    rapidjson::StringBuffer s;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);
    writer.StartObject();
    writer.Key("port");
    writer.Int(configuration.port);
    writer.Key("address");
    writer.String(configuration.address.c_str());
    writer.Key("stressTest");
    writer.Bool(configuration.stressTest);
    writer.Key("sendPragmaTokens");
    writer.Bool(configuration.sendPragmaTokens);
    writer.Key("pragmaGameToken");
    writer.String(configuration.pragmaGameToken.c_str());
    writer.Key("pragmaSocialToken");
    writer.String(configuration.pragmaSocialToken.c_str());
    writer.EndObject();

    // Save settings file.
    std::ofstream outfile(filename.c_str());
    outfile << s.GetString();
    outfile.close();
}

}
}
