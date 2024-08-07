# Makita Solar Charger
320ah LiPeO<sup>4</sup> Solar storage battery charged from 200W pk panels connected to Makita DC chargers.\
This repo is really just for my documentation and backup but I suppose someone may find it useful.
## Hardware
> Fogstar 320ah LiPeO4 Battery assembled from individual B Grade cells.\
> JBD 100A BMS with Bluetooth monitoring.\
> Epever MPTT 20 Amp charge controller.\
> Victron Energy 12-24 Volt Orion 20 amp inverter.\
> Makita DC18SE x 4\ 
> Arduino custom controller, battery monitor and status display\
> Due to external constraints design is rather complicated.
## Design philosopy 

| Item 		| Reason	|
| ------------- | ------------- |
| Fogstar battery | Lithium best value - capacity / service life |
| JBD bms | Ensure battery is not overcharged or damaged by misuse|
| Epever Charge controller | RS485 comms for custom monitoring|
| 12-24 Volt Inverter | Overcome interconnect cable losses due to remote battery requirement |
| DC Chargers | 'Safety' requirement| 
| Custom controller | Useful information : simple UI|

## Code
**Coloumb counting:** Required for accurate monitoring of the solar battery. Not yet implemented.\
**Charge display:** Charge status, charge time, energy consumption.

Here you should say what actually happens when you execute the code above.

### Initial Configuration

Some projects require initial configuration (e.g. access tokens or keys, `npm i`).
This is the section where you would document those requirements.

## Developing

Here's a brief intro about what a developer must do in order to start developing
the project further:

```shell
git clone https://github.com/your/awesome-project.git
cd awesome-project/
packagemanager install
```

And state what happens step-by-step.

### Building

If your project needs some additional steps for the developer to build the
project after some code changes, state them here:

```shell
./configure
make
make install
```

Here again you should state what actually happens when the code above gets
executed.

### Deploying / Publishing

In case there's some step you have to take that publishes this project to a
server, this is the right time to state it.

```shell
packagemanager deploy awesome-project -s server.com -u username -p password
```

And again you'd need to tell what the previous code actually does.

## Features

What's all the bells and whistles this project can perform?

* What's the main functionality
* You can also do another thing
* If you get really randy, you can even do this

## Configuration

Here you should write what are all of the configurations a user can enter when
using the project.

### Argument 1

Type: `String`  
Default: `'default value'`

State what an argument does and how you can use it. If needed, you can provide
an example below.

Example:

```bash
awesome-project "Some other value"  # Prints "You're nailing this readme!"
```

#### Argument 2

Type: `Number|Boolean`  
Default: 100

Copy-paste as many of these as you need.

## Contributing

When you publish something open source, one of the greatest motivations is that
anyone can just jump in and start contributing to your project.

These paragraphs are meant to welcome those kind souls to feel that they are
needed. You should state something like:

"If you'd like to contribute, please fork the repository and use a feature
branch. Pull requests are warmly welcome."

If there's anything else the developer needs to know (e.g. the code style
guide), you should link it here. If there's a lot of things to take into
consideration, it is common to separate this section to its own file called
`CONTRIBUTING.md` (or similar). If so, you should say that it exists here.

## Links

Even though this information can be found inside the project on machine-readable
format like in a .json file, it's good to include a summary of most useful
links to humans using your project. You can include links like:

* Project homepage: <https://your.github.com/awesome-project/>
* Repository: <https://github.com/your/awesome-project/>
* Issue tracker: <https://github.com/your/awesome-project/issues>
  * In case of sensitive bugs like security vulnerabilities, please contact
    <my@email.com> directly instead of using issue tracker. We value your effort
    to improve the security and privacy of this project!
* Related projects:
  * Your other project: <https://github.com/your/other-project/>
  * Someone else's project: <https://github.com/someones/awesome-project/>

## Licensing

One really important part: Give your project a proper license. Here you should
state what the license is and how to find the text version of the license.
Something like:

"The code in this project is licensed under MIT license."



# Charge Makita batteries from solar panels
# Overview:
This project was dictated by a requirement 
In brief, it all got rather complicated. Given my time over I would have don
e it differently. 
However in justification much of the engineering decision making process was dictated by circumstances and others

LiFeO4 battery monitor and charge controller
# Features
monitor JBD BMS using (adapted) code from 

https://github.com/vagueDirector/ArduinoXiaoxiangSmartBMSDisplay

Display parameters on 20x4 LCD

Set Makita Battery Charge using pushbutton

24 VDC enabled until BMS current < 1 Amp or 2.5 Hours

Chargers switched off

Log charger current individual / total - Depends..

Calculate power.

Display charge duration and power in Watt-hours
This is the section where you would document those requirements.

## Developing

Here's a brief intro about what a developer must do in order to start developing
the project further:

```shell
git clone https://github.com/your/awesome-project.git
cd awesome-project/
packagemanager install
```

And state what happens step-by-step.

### Building

If your project needs some additional steps for the developer to build the
project after some code changes, state them here:

```shell
./configure
make
make install
```

Here again you should state what actually happens when the code above gets
executed.

### Deploying / Publishing

In case there's some step you have to take that publishes this project to a
server, this is the right time to state it.

```shell
packagemanager deploy awesome-project -s server.com -u username -p password
```

And again you'd need to tell what the previous code actually does.

## Features

What's all the bells and whistles this project can perform?

* What's the main functionality
* You can also do another thing
* If you get really randy, you can even do this

## Configuration

Here you should write what are all of the configurations a user can enter when
using the project.

### Argument 1

Type: `String`  
Default: `'default value'`

State what an argument does and how you can use it. If needed, you can provide
an example below.

Example:

```bash
awesome-project "Some other value"  # Prints "You're nailing this readme!"
```

#### Argument 2

Type: `Number|Boolean`  
Default: 100

Copy-paste as many of these as you need.

## Contributing

When you publish something open source, one of the greatest motivations is that
anyone can just jump in and start contributing to your project.

These paragraphs are meant to welcome those kind souls to feel that they are
needed. You should state something like:

"If you'd like to contribute, please fork the repository and use a feature
branch. Pull requests are warmly welcome."

If there's anything else the developer needs to know (e.g. the code style
guide), you should link it here. If there's a lot of things to take into
consideration, it is common to separate this section to its own file called
`CONTRIBUTING.md` (or similar). If so, you should say that it exists here.

## Links

Even though this information can be found inside the project on machine-readable
format like in a .json file, it's good to include a summary of most useful
links to humans using your project. You can include links like:

* Project homepage: <https://your.github.com/awesome-project/>
* Repository: <https://github.com/your/awesome-project/>
* Issue tracker: <https://github.com/your/awesome-project/issues>
  * In case of sensitive bugs like security vulnerabilities, please contact
    <my@email.com> directly instead of using issue tracker. We value your effort
    to improve the security and privacy of this project!
* Related projects:
  * Your other project: <https://github.com/your/other-project/>
  * Someone else's project: <https://github.com/someones/awesome-project/>

## Licensing

One really important part: Give your project a proper license. Here you should
state what the license is and how to find the text version of the license.
Something like:

"The code in this project is licensed under MIT license."





