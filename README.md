# Makita Solar Charger
<picture>
 <img src=https://github.com/hedley-a/MakitaSolarCharger/blob/main/pictures/solarPanels.jpg>
</picture>

320ah LiPeO<sup>4</sup> Solar storage battery charged from 200W pk panels connected to Makita DC chargers.\
This repo is really just for my documentation and backup but someone may find it useful.
## Hardware
> Fogstar 320ah LiPeO4 Battery assembled from individual B Grade cells.\
> JBD 100A BMS with Bluetooth monitoring.\
> Active battery balancer.\
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
**Charge display:** Charge status, charge time, energy consumption, temperature monitor.

|Arduino ProMini,| RS485-TTL converter,| 20x4 i2c LCD,| ACS 712 current sensors,| ADS1115 ADC.|\
Optically isolated remote switching of 12/24 Volt converter and active battery balancer.

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
<picture>

<img src="https://github.com/hedley-a/MakitaSolarCharger/blob/main/pictures/chargerPanel.jpg">

<img src="https://github.com/hedley-a/MakitaSolarCharger/blob/main/pictures/controller.jpg" width="200">

</picture>

What's all the bells and whistles this project can perform?

* What's the main functionality
* You can also do another thing
* If you get really randy, you

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

I still have the battery SOC columb counting to implement.\
Interesting that despite being a magpie coder I could find little on GitHub\
If you'd like to contribute, please fork the repository and use a feature
branch. Pull requests are warmly welcome."\

As a self taught coder I am open to style and structure advice.

## Links

Even though this information can be found inside the project on machine-readable
format like in a .json file, it's good to include a summary of most useful
links to humans using your project. You can include links like:

* Project homepage: <>
* Repository: <https://github.com/hedley-a/MakitaSolarCharger>
* Issue tracker: <https://github.com/hedley-a/MakitaSolarCharger>
* Related projects:
  * Your other project: <https://github.com/your/other-project/>
  * Someone else's project: <https://github.com/someones/awesome-project/>

## Licensing

The code in this project is licensed under GNU General Public License v3.0.



