# Fake Agent

The Agent is the component that connects to and communicates to the Game Server to send and receive Arcus API messages. In production, it lives alongside the deployed Game Server process.

The Fake Agent provided here uses the arcus library to simulate all of the agent behavior that is possible on a production ONE deployment. It is used to connect to and test a Server of the ONE Game Hosting SDK.

It is *not* intended to be included in the game server's build nor is it required to build the server or accompanying arcus folder libraries.

## Configuring Fake Agent

If present in the same folder the Fake Agent will load the agent.json configuration file. If the configuration file is not present, the Fake Agent will create a new agent.json configuration file automatically.

The following settings can be configured for the Fake Agent:

```json
{
    "port": 19001,
    "address": "127.0.0.1",
    "stressTest": false,
    "sendPragmaData": false,
    "pragmaInstanceId": "",
    "PragmaBackendAddress": "",
    "pragmaGameToken": "",
    "pragmaSocialToken": ""
}
```

port:                   Port number for the server to listen on.
address:                Address for the server to bind to.
stressTest:             Whether to run in stress test mode.
sendPragmaData:         Whether to send Pragma tokens as metadata payload with the allocated command.
pragmaInstanceId:       Id of the Pragma Instance.
PragmaBackendAddress:   Network address of the Pragma Engine
pragmaGameToken:        The pragma game token.
pragmaSocialToken:      he pragma social token.

## Connecting against your game server with the fake agent

1. Build the repository.
2. Run run_fake_agent_release.sh (or debug, as needed).
3. The agent will run, and connect to the game server immediately.

## Using the fake game

The fake game can be run instead of your game server to ensure things are working as expected in your environment.

1. Build the repository.
2. Run run_fake_game_release.sh (or debug, as needed).
3. The server will listen and accept an incoming agent connection.
