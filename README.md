# Makita Solar Charger
<picture>
 <img src=https://github.com/hedley-a/MakitaSolarCharger/blob/main/pictures/solarPanels.jpg>
</picture>

320ah LiFePO<sup>4</sup> Solar storage battery charged from 200W pk panels connected to Makita DC chargers.\
This repo is really just for my documentation and backup but someone may find it useful.
## Hardware
> Fogstar 320ah LiFePO<sup>4</sup> Battery assembled from individual B Grade cells.\
> JBD 100A BMS with Bluetooth monitoring.\
> Active battery balancer.\
> Epever MPTT 20 Amp charge controller.\
> Victron Energy 12-24 Volt Orion 20 amp inverter.\
> Makita DC18SE x 4\ 
> Arduino custom controller, battery monitor and status display\
> Due to external constraints design ended up being rather complicated.
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

## Developing

### Building

## Features
<picture>

<img src="https://github.com/hedley-a/MakitaSolarCharger/blob/main/pictures/chargerPanel.jpg">

<img src="https://github.com/hedley-a/MakitaSolarCharger/blob/main/pictures/controller.jpg" width="200">

</picture>

* Main functionality - manual start and timed automatic stop of battery charging
* Overtemperature charge delay
* Solar battery charge level monitoring

## Configuration

## Contributing

I still have the battery SOC columb counting to implement.\
Interesting that despite being a magpie coder I could find little on GitHub\
If you'd like to contribute, please fork the repository and use a feature
branch. Pull requests are warmly welcome."\

As a self taught coder I am open to style and structure advice.

## Links

* Project homepage: <>
* Repository: <https://github.com/hedley-a/MakitaSolarCharger>

## Licensing

The code in this project is licensed under GNU General Public License v3.0.



