# brainCloud S2S example using Unreal Engine dedicated server

If you haven't signed up or you want to log into the brainCloud portal, you can do that here:

https://portalx.braincloudservers.com/

## How to set up your app on the brainCloud portal

1. Once logged in to the portal, start by creating a new App

2. Make an account on DockerHub and create a Docker Repo for your dedicated server container

3. In the brainCloud portal, select your app and go to Design-> Servers->My Servers create a new server, you can call this `dedicatedserver` and set the `Server Type` to `Room Server (hosted)` then provide the Docker Registry and Docker Repo you created in the previous step.

4. In Multiplayer->Lobbies set up a new lobby type and call it `DED_FPS` and set the Server of this lobby to the one you created in the previous step, `dedicatedserver`. Also make sure `Disband on start` is unchecked in the lobby rules

## How to build and run project in Unreal

1. [Follow these instructions to build Unreal Engine from source](https://docs.unrealengine.com/5.1/en-US/building-unreal-engine-from-source/) Make sure to get the proper version, this project is using UE `5.1.1-0+++UE5+Release-5.1`

2. Once Unreal Engine is built, right click on the `DedicatedDemo.uproject` file and click Generate Visual Studio files

3. Open the DedicatedDemo project in the built version of Unreal, then go to Tools->Open Visual Studio

4. In Visual Studio, build the project by selecting Build->Project Only->Build Only DedicatedDemo, do this for each of these Solution Configurations: Development Client, Development Editor and Development Server.

5. Open the brainCloud widget by going to Tools->brainCloud Settings - then you can input your AppID, AppSecret, Server URL and version in there. You can find this information in the brainCloud portal where you have created your App.

6. You should now be able to run the project in Unreal Engine

## How to build and deploy linux dedicated server

1. First make sure you have CLang - You can download the correct version required for this project [here](https://cdn.unrealengine.com/CrossToolchain_Linux/v20_clang-13.0.1-centos7.exe)

2. You can now build the linux dedicated server by selecting Platforms->Linux->Cook Content and then Package Project and make sure to select DedicatedDemoServer as your Target

3. In the DockerFiles folder, there are 2 files to help with creating a docker image of the built server and deploying it to your DockerHub repository. Copy these files into the folder where you have built your Linux server, and then modify the buildAndDeployDocker.bat file to input your DockerHub repository in the appropriate place. Then you can just run the buildAndDeployDocker.bat to build and deploy your server.

4. Once these steps are complete, you should be able to run the game and start a new server instance which will have your latest changes on the server side.