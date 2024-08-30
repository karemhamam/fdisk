# fdisk -l Implementation

This project implements a simplified version of the `fdisk -l` command. The program reads the partition table from a specified disk and displays information about primary and logical partitions if present. Additionally, it includes support for parsing GPT (GUID Partition Table) partition tables as a bonus feature.

## Project Description

### Features
- **MBR Partition Table Parsing**: Displays primary partitions and their details.
- **Logical Partitions**: Extended to show logical partitions within extended partitions.
- **GPT Support** : Parses and displays GPT partition tables.

### Important Notes
1. Ensure that the return values of system calls and library routines are checked for proper error handling.
2. The program should be compiled using the provided commands, and examples of output are included below.

## Compilation Instructions

To compile the program, use the following command:

gcc -o fdisk fdisk.c


## Usage

Run the compiled program with root privileges to list the partitions of a specified disk:

sudo ./fdisk /dev/sda

## Installation

1. Clone the repository and compile the code: git clone ()

## Contributing

Contributions are welcome! 
If you have suggestions for new features or improvements, feel free to open an issue or submit a pull request.

## License

mahamedhamam15@gmail.com



