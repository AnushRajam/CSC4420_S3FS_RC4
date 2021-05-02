# CSC4420 Final Project

## Overview
The goal of this project (repository) is to attach an S3 Bucket as the backend for a filesystem using FUSE and S3FS. Along with that, automatically encrypt and decrypt any files being uploaded or downloaded (respectively) using OpenSSL's implementation of RC4.

## Features
Once downloaded you will be able to:
- sync a specificed folder with an S3 bucket of your choosing
- once you insert any type of file into the folder it will automatically sync with S3
- the file uploaded will be encrypted and can only decrypted using your key (this process happens automatically if accessed via Linux)

## Installation (Ubuntu Instructions)
There are 3 stages to set up the environment and project:
1. Clone project files onto your Linux OS.
2. Installing FUSE and S3FS onto your Linux OS.
3. Connecting and mouting AWS S3 to your Linux environment.

### 0. Preconditions
- Open a terminal
- Update all packages before starting
	- `sudo apt-get update`
- In case an older version exists on your system
	- `sudo apt-get remove fuse` 
- Verify necessary packages are installed: 
	- `sudo apt-get install build-essential libcurl4-openssl-dev libxml2-dev mime-support` 
- Install Git
	- `sudo apt-get install git`

### 1. Clone project files onto your Linux OS
- Clone the repository using git:
	- (via Terminal) `sudo git clone https://github.com/AnushRajam/CSC4420_S3FS_RC4.git` 

### 2. Installing FUSE and S3FS onto your Linux OS
- Finished via the previous steps

### 3. Connecting and Mounting AWS S3 to Linux environment
- Navigate to the root folder (/) of your OS
	- `echo <your-access-key>:<your-secret-key> > /etc/passwd-s3fs`
	- `chmod 777 /etc/passwd-s3fs`
- sudo mkdir /path/to/mountpoint
- Access root user privledges 
	- `sudo su`
- Update fstab file
	- `echo s3fs# <your-s3-bucket> fuse _netdev,rw,nosuid,nodev,allow_other,nonempty 0 0 >> /etc/fstab`
- Mount newly added bucket
	- `mount -a` 
- Locate the s3fs encryption upload source code (CSC4420_S3FS_RC4/src/fdcache_entity.cpp)
- Go to line 52 and 53, change the location names to where you desire them to be
- (Optional) Go to line 102 and update this key to one unique to you

## Running the project
### RC4 Standalone Script
- open the directory that contains the standalone files
 - either by:
   - utilizing the file system
   - using the terminal and the "cd" function to navigate
   - using your preferred text editor if it has these capabilities (such as vscode)
- To compile: `gcc rc4Standalone.c -lcrypto -o rc4`
- To execute: `./rc4 [-e | -d] [-salt | -no-salt] [key] [inputFile] [outputFile]`

### S3FS Implementation
- Once the installation instructions are followed, everything will work automatically. 
 - This can be tested by inserting a file into the mounted folder. Then, check your S3 bucket to reflect the changes. Additionally, you may download the file to see if it has been encrypted.

## Dependencies
- fuse >= 2.8.4
- automake
- gcc-c++
- make
- libcurl
- libxml2
- openssl
- mime.types (the package providing depends on the OS)
- pkg-config (or your OS equivalent)
