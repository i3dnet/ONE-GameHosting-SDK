#include <chrono>
#include <thread>

#include <one/arcus/array.h>
#include <one/arcus/error.h>
#include <one/arcus/object.h>
#include <one/arcus/types.h>

#include <one/fake/arcus/agent/agent.h>
#include <one/fake/arcus/agent/log.h>
#include <one/fake/arcus/agent/config.h>

using namespace std::chrono;
using namespace i3d::one;

Configuration configuration;
String configFile = "agent.json";

void sleep(int ms) {
    std::this_thread::sleep_for(milliseconds(ms));
}

int main() {
    log_info("-----------------------");
    log_info("agent startup");

    // Default configuration values.
    configuration.port = 19001;
    configuration.address = "127.0.0.1";
    configuration.stressTest = false;
    configuration.sendPragmaData = false;
    configuration.pragmaBackendAddress = "";
    configuration.pragmaGameInstanceId = "";
    configuration.pragmaGameToken = "";
    configuration.pragmaSocialToken = "";

    if (!LoadConfiguration(configFile, configuration))
    {
        CreateDefaultConfiguration(configFile, configuration);
        log_info("configuration file " + configFile + " not found, creating with default values.");
    }
    else
    {
        log_info("configuration file " + configFile + " found.");
    }

    log_info("initializing agent with the following settings:");
    log_info("  address: " + configuration.address);
    log_info("  port: " + String(std::to_string(configuration.port).c_str()));
    log_info("  stressTest: " + String(configuration.stressTest ? "true" : "false"));
    log_info("  sendPragmaData: " + String(configuration.sendPragmaData ? "true" : "false"));
    log_info("  pragmaBackendAddress: " + configuration.pragmaBackendAddress);
    log_info("  pragmaGameInstanceId: " + configuration.pragmaGameInstanceId);
    log_info("  pragmaGameToken: " + configuration.pragmaGameToken);
    log_info("  pragmaSocialToken: " + configuration.pragmaSocialToken);

    Agent agent;
    auto err = agent.init(configuration.address.c_str(), configuration.port);
    if (is_error(err)) {
        log_error("failed to init agent.");
        return 1;
    }

    log_info("agent is initialized.");
    log_info("running update loop.");

    auto log_status = [](Client::Status status) {
        String status_str = Client::status_to_string(status);
        log_info("status: " + status_str);
    };
    auto status = agent.client().status();
    log_status(status);

    // Soft stop behavior.
    const auto time_zero = steady_clock::time_point(steady_clock::duration::zero());
    steady_clock::time_point time_to_send_soft_stop = time_zero;
    bool did_send_soft_stop = false;

    int messages_counter = 0;

    while (true) {
        sleep(configuration.stressTest ? 1 : 100);

        if (agent.client().status() == Client::Status::ready) {
            if (configuration.stressTest) {
                Array metadata;

                Object map_object;
                map_object.set_val_string("key", "map");
                map_object.set_val_string("value", "fake map");
                Object mode_object;
                mode_object.set_val_string("key", "mode");
                mode_object.set_val_string("value", "fake mode");
                Object type_object;
                type_object.set_val_string("key", "type");
                type_object.set_val_string("value", "fake type");

                metadata.push_back_object(map_object);
                metadata.push_back_object(mode_object);
                metadata.push_back_object(type_object);

                agent.send_metadata(metadata);
            } else {
                // Trigger a soft stop when game enters ready state.
                if (time_to_send_soft_stop == time_zero) {
                    time_to_send_soft_stop = steady_clock::now() + seconds(10);
                } else if (!did_send_soft_stop) {
                    if (steady_clock::now() > time_to_send_soft_stop) {
                        agent.send_soft_stop(5);
                        did_send_soft_stop = true;
                    }
                }

                // Randomly tell the game server to become allocated.
                bool shouldSend = std::rand() / ((RAND_MAX + 1u) / 50) == 0;
                if (shouldSend) {
                    switch (messages_counter) {
                        case 0: {
                            Array array;
                            Object players;
                            players.set_val_string("key", "players");
                            players.set_val_string("value", "16");
                            Object duration;
                            duration.set_val_string("key", "duration");
                            duration.set_val_string("value", "20");

                            array.push_back_object(players);
                            array.push_back_object(duration);

                            if (configuration.sendPragmaData) {
                                Object pragma_backend_address;
                                pragma_backend_address.set_val_string("key", "pragmaBackendAddress");
                                pragma_backend_address.set_val_string("value", configuration.pragmaBackendAddress);
                                Object pragma_game_instance_id;
                                pragma_game_instance_id.set_val_string("key", "pragmaGameInstanceId");
                                pragma_game_instance_id.set_val_string("value", configuration.pragmaGameInstanceId);
                                Object pragma_game_token;
                                pragma_game_token.set_val_string("key", "pragmaGameToken");
                                pragma_game_token.set_val_string("value", configuration.pragmaGameToken);
                                Object pragma_social_token;
                                pragma_social_token.set_val_string("key", "pragmaSocialToken");
                                pragma_social_token.set_val_string("value", configuration.pragmaSocialToken);                               

                                array.push_back_object(pragma_backend_address);
                                array.push_back_object(pragma_game_instance_id);    
                                array.push_back_object(pragma_game_token);
                                array.push_back_object(pragma_social_token);
                            }

                            log_info("sending allocated");
                            agent.send_allocated(array);

                            Array data;
                            Object command;
                            command.set_val_string("key", "command");
                            command.set_val_string("value", "command-1");
                            Object argument;
                            argument.set_val_string("key", "argument");
                            argument.set_val_string("value", "10");

                            data.push_back_object(command);
                            data.push_back_object(argument);

                            log_info("sending custom command");
                            agent.send_custom_command(data);
                            break;
                        }
                        default: {
                            Array metadata;

                            Object map_object;
                            map_object.set_val_string("key", "map");
                            map_object.set_val_string("value", "fake map");
                            Object mode_object;
                            mode_object.set_val_string("key", "mode");
                            mode_object.set_val_string("value", "fake mode");
                            Object type_object;
                            type_object.set_val_string("key", "type");
                            type_object.set_val_string("value", "fake type");

                            metadata.push_back_object(map_object);
                            metadata.push_back_object(mode_object);
                            metadata.push_back_object(type_object);

                            log_info("sending metadata");
                            agent.send_metadata(metadata);

                            Array data;
                            Object command;
                            command.set_val_string("key", "command");
                            command.set_val_string("value", "command-1");
                            Object argument;
                            argument.set_val_string("key", "argument");
                            argument.set_val_string("value", "2");

                            data.push_back_object(command);
                            data.push_back_object(argument);

                            log_info("sending custom command");
                            agent.send_custom_command(data);

                            break;
                        }
                    }

                    ++messages_counter;
                }
            }
        } else {
            time_to_send_soft_stop = time_zero;
            did_send_soft_stop = false;
            messages_counter = 0;
        }

        err = agent.update();
        if (is_error(err)) {
            log_info(error_text(err));
            continue;
        }

        auto old_status = status;
        status = agent.client().status();
        if (status != old_status) {
            log_status(status);
        }
    }

    log_info("-----------------------");
    log_info("agent has been shutdown");
    return 0;
}
