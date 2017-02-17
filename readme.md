# STM32F4 Xcode template
This is an **Xcode template for stm32f4 boards using gcc arm toolchain**, based on the macos template in the [STM32F4_Templates](https://github.com/TDAbboud/STM32F4_Templates) repository created by [Tony Abboud](https://github.com/TDAbboud). I would highly recommend you to check out his Youtube series: [STM32F4 Development Series](https://www.youtube.com/playlist?list=PL0wGKPc4_PKMqP_EkIwpvExM2rvi_-R3z)  
Forgive me when I call most of the information on programming with STM32F4 on internet garbage, but this guy really knows what he is doing and I enjoyed his tutorials, I suggest you take the time and watch his videos if you are not familiar with STM32 environment. 

##Getting started
###Clone this repository###
`git clone https://github.com/MuhsinFatih/stm32f4_Xcode`  
(I suggest not having spaces in your project directory. It might mess things up)  
###Open the project in Xcode###
I don't think this needs any explanation ^^  
###Set up the toolchain###
You can follow the steps here, or watch the first video in the series I mentioned: [STM32F4 Dev. | #1 - Setup the Toolchain / Templates (Barebones)](https://youtu.be/Frh-oPw7NjI?list=PL0wGKPc4_PKMqP_EkIwpvExM2rvi_-R3z)  
The two are slightly different, but both should work. 


##Setting up the toolchain
###Install homebrew
Go to brew's homepage and install: <https://brew.sh/>
###install the ARM toolchain###

	brew tap PX4/homebrew-px4  
	brew update  
	brew install gcc-arm-none-eabi-48  

###Install st-link###

	brew install st-link

##Building & Running##
###Build and run in Xcode###
straightforward: build and run as usual in Xcode. Activate the console if you don't see it: `view > debug area > activate console`  

You should see 'build succeeded' message show up

###Build and run with command line###
Go to project folder (where main.cpp is) and  

	make  
	make flash  


This will put build files in `build` folder in the project folder, whereas Xcode will store it in a temporary folder

##Troubleshooting
If you have any issues please report it. Feel free to submit a pull request if you have any fixes to issues or any improvements.

If you follow [STM32F4 Development Series](https://www.youtube.com/playlist?list=PL0wGKPc4_PKMqP_EkIwpvExM2rvi_-R3z) as I said on top of this page, make sure you wrap interrupt functions with ``` extern "C" {} ``` in case you want to define them inside a c++ file. Like so:
    
    extern "C" {
    interrupt function() { ... }
    }
Otherwise your program will crash. Here's a SO discussion with detailed info: [Why isn't my SysTick_Handler() being called in my LPCxpresso C++ Project?](http://electronics.stackexchange.com/a/93984)
