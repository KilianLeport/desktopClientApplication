# General project

The purpose of this project is to develop a network of sensors and actuators managed by a smartphone or a computer for real time monitoring. The sensors and actuators are designed to be plugged onto an electric line with short pulses of a maximum of 10KV (for example an energizer) and send updates back to the server on current continuity, allowing immediate detection of any breakage on the line. The project uses electronic devices for end user monitoring with both an Android and desktop application, a master device, and up to 255 slave devices. Here a server application is used to emulate the electronic aspect and the embedded server.

See below the project schematic. 
<img src="../master/image/schema.png" alt="General Project Schematic"/>

# List of repositories

* [Android Client application:](https://github.com/KilianLeport/androidClientApplication)
* [Desktop client application:](https://github.com/KilianLeport/desktopClientApplication)
* [Server Emulator application:](https://github.com/KilianLeport/serverEmulatorApplication)  

# Summary

* [General project](../master/README.md#General-project)
* [List of repositories](../master/README.md#List-of-repositories)
* [Summary](../master/README.md#Summary)
* [List of projects](../master/README.md#List-of-projects)
  * [Android Client application](../master/README.md#Android-client-application)
    * [MVP implementation example](../master/README.md#MVP-implementation-example)
  * [Server Emulator application](../master/README.md#Server-Emulator-application)
  * [Desktop client application](../master/README.md#Desktop-client-application)
* [Improvements](../master/README.md#Improvements)
* [Licence](../master/README.md#Licence)



# List of projects
### [Android Client application:](https://github.com/KilianLeport/androidClientApplication)
The application was developed in Java. The main pattern used the Model View Presenter design pattern. The Views are passive and their only purpose is to display information and transmits the interactions between the View and the user to the Presenter. Each View has a its own Presenter. The Model communicates with the network and stores data. The view doesn't access the model, only the Presenters communicate with the Model.
Each Presenter subscribes to the ModelManager when their respective Views run the "onResume()" life cycle and unsubscribe when their respective Views run the "onPause()" life cycle.

See below the general implementation of the application.
<img src="../master/image/android_client_application.png" alt="Android Client Application Schematic"/>

#### MVP implementation example:
1. The View 'MainActivity' holds an instance of 'MainPresenterInt' to communicate with its Presenter. The user clicks on the connection button in the View, the View then notifies the Presenter.
2. The Presenter 'MainPresenter' holds an instance of 'ModelManagerInt' to communicate with the Model. The 'MainPresenter' class asks the Model to connect to the server.
3. The 'ModelManager' holds a list of instances of 'PresentersInt'. It notifies all of them that a connection is established.
4. The 'MainPresenter' olds an instance of the 'MainActivityInt'. It notifies the 'MainActivity' to change the state of the connection button.
<img src="../master/image/android_client_application_MVP.png" alt="Android Client MVP pattern"/>



### [Desktop client application:](https://github.com/KilianLeport/desktopClientApplication)
The desktop application was developed in C++ with Qt. The application uses a Qt Model View Controller design pattern that is mainly a Model View design pattern [(see Qt website)](https://doc.qt.io/qt-5/model-view-programming.html).
The application is mainly composed of three Views:
* DeviceListWidget that implements a QTableView which lists the devices and allows the users to control them,
* FieldViewerWidget that implements a graphic view of the device network (To-do: see Improvements section),
* LogViewerWidget that implement a QPlainTextEdit to see debug, error, and other informations.

See below the general implementation of the application. 
<img src="../master/image/client_surveillance_schematic.png" alt="Desktop Client Application"/>



### [Server Emulator application:](https://github.com/KilianLeport/serverEmulatorApplication) 
The Server Emulator application was developed to test the client applications. This application emulates the server and the sensor network. 
This application can:
* simulate the TCP server
* simulate the UDP server to answer the IP and the PORT of the TCP server
* simulate a list of devices that are present on a virtual network
* simulate a loss of communication with those devices
* simulate a delay in communication with each device
* save a configuration for the sensor network 

See below the general implementation of the application. 
<img src="../master/image/server_surveillance_schematic.png" alt="Server Emulator Application"/>



# Improvements
- [ ] Change the emulation of the sensor network in the Server Emulator Application
- [ ] Add the possibility to draw links between devices in the graphic view of the different client applications



# Licence

    Copyright (C) 2018 Kilian Leport

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version
    

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
  
    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
