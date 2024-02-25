# Stopwatch app
## Install the app
1. Download the .nwa file from the 'target' directory.
2. Go to [my.numworks.com/apps](https://my.numworks.com/apps) to upload this stopwatch.nwa file.
## Build you own app
Requirements :
1. Node.js from the [Node.js download page](https://nodejs.org/en/download/).
2. You need to install [nwlink](https://www.npmjs.com/package/nwlink).

Let's create your very own numworks app :

1. First, open your PowerShell terminal and run the following commands :
```shell
> Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
> Invoke-RestMethod -Uri https://get.scoop.sh | Invoke-Expression
```
2. Next run these commands to install 'make' to build your app !
```shell
scoop install git
scoop bucket add main
scoop install main/make
```
3. Run this to install the Pre-built [GNU Toolchain for the Arm Architecture](https://scoop.sh/#/apps?q=gcc-arm-none-eabi) :
```shell
scoop bucket add extras
scoop install extras/gcc-arm-none-eabi
```
4. And here it is ! You can build your app with :
```shell
cd C:\path\to\your\makefile\folder
make #or 'make run' if you want to upload your app on your calculator
```

##Create your app
Your project folder must contain :
1. A Makefile file ( like the one present in the repository above )
2. Your .cpp file
3. And other extras library like eadkpp

