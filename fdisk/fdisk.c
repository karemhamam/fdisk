/* *******************************************************
   * @File                 : fdisk.c
   * @Author               : mohamed bahget hamam
   * @email                : mahamedhamam15@gmial.com
   *******************************************************
*/


/* **************** Include section start *************** */

#include <stdio.h>         // includes the standard I/O
#include <stdlib.h>        // includes the standard library.
#include <unistd.h>        // includes unix standard library.
#include <fcntl.h>         // includes file control options
#include <string.h>        // includes string handling func.
#include <errno.h>         // includes error number definitions.
#include "fdisk.h"         // includes the declaration of functions.

/* **************** Include section End *************** */


/* **************** main section start *************** */

int main (int argc , char *argv[])    
{
    int fd ;
    uint8_t buffer[SECTOR_SIZE];

    if ( argc != 2 )
    {
        fprintf(stderr,"Usage: %s <device>\n",argv[0]);
        return 1 ;
    }

    fd = open(argv[1] , O_RDONLY); // open the specified device  in read-only mode.
    if (fd == -1)
    {
        perror("Failed to open device."); // error handling.
        return 1;
    }

    if ( read (fd , buffer , SECTOR_SIZE) != SECTOR_SIZE) // read the first sector of the device.
    {
        perror("Failed to read MBR."); // error handling
        close(fd);
        return 1;
    }

    if ( buffer[MBR_SIGNATURE_OFFSET] == 0x55 && buffer[MBR_SIGNATURE_OFFSET + 1] == 0xAA) // check if the lst two bytes match the MBR signature (0xAA55)
    {
        /*
         * if MBR detected ?
         * it reads and prints primary partitions and checks for extended partitions.
        */
        printf("MBR deected.\n");
        struct partition_entry *entry = (struct partition_entry *)(buffer + MBR_PARTITION_TABLE_OFFSET);
        int i = 0 ;
        for ( i = 0 ; i < 4 ; i++)
        { 
            print_partition_entry(&entry[i] , i);
            if (entry[i].type == 0x05 || entry[i].type == 0x0F)
            {

                printf("Extended partition detected, checking for logical partitions.\n");
                parse_logical_partitions(fd , entry[i].lba_first , entry[i].lba_first);

            }
        }

    }else if (memcmp(buffer + 1 , GPT_HEADEER_SIGNATURE , 8 ) == 0 )
    {
        /*
         * if GPT detected ?
         * it parses the GPT partition table.
        */
        printf("GPT deteted.\n");
        parse_gpt(fd);
    }else
    {
        /*
         * if neither MBR nor GPT detected ?
         * it prints an error message.
        */
        fprintf(stderr, "Unknown partition table format.\n");
    }

    close(fd); //close the file descriptor.

    return 0 ;
}

/* **************** main section End *************** */


/* **************** Definition section start *************** */

void print_partition_entry (struct partition_entry *entry , int index)
{
    printf("Partiton %d: Type: ox%X, start LBA: %u Size in sectors: %u\n",index + 1 , entry->type , entry->lba_first,entry->sectors);
}


void parse_logical_partitions(int fd , uint32_t ebr_start , int32_t ebr_current)
{
    uint8_t buffer[SECTOR_SIZE];

    lseek(fd, ebr_current * SECTOR_SIZE , SEEK_SET); // moves the file offset to the start of the current EBR.

    if (read(fd , buffer , SECTOR_SIZE ) != SECTOR_SIZE) // reads a sector from the current EBR.
    {
        perror("Failed to read EBR"); // error handling.
        return;
    }

    struct partition_entry *logical_entry = ( struct partition_entry *)(buffer +MBR_PARTITION_TABLE_OFFSET); // points to the partition entry within the EBR.
    struct partition_entry *next_entry =(struct partition_entry *)(buffer + MBR_PARTITION_TABLE_OFFSET + PARTITION_ENTRY_SIZE); //point to the next partition entry within the EBR.

    if (logical_entry->type != 0)
    {
        printf("Logical Partition: Type: ox%X, Start LBA: %u, Size in sectors: %u\n", logical_entry->type,ebr_current + logical_entry->lba_first,logical_entry->sectors); // print details of the logical partition.
    }

    if (next_entry->type != 0)
    {
        /*
         * if there is another entry?
         * the function will call itself recursively.
        */
        parse_logical_partitions(fd, ebr_start, ebr_start + next_entry->lba_first);
    }
}

void parse_gpt (int fd)
{
    uint8_t buffer[SECTOR_SIZE];

    lseek(fd, SECTOR_SIZE,SEEK_SET); // moves the file offset to the start of the GPT header at LBA 1.

    if (read(fd,buffer,SECTOR_SIZE) != SECTOR_SIZE) // reads a sector from LBA 1.
    {
        perror("Failed to read GPT header"); // error handling.
        return;
    }
    
    if (memcmp(buffer,GPT_HEADEER_SIGNATURE,8) != 0) // compare the read buffer the the GPT header signature.
    {
        fprintf(stderr, "Invalid GPT signature.\n"); // error handling.
        return;
    }

    int i = 0 ; 

    for ( i = 0 ; i < GPT_PARTITION_ARRAY_SIZE ; i++) // iterates over GPT partition entries starting at LBA 2.
    {
        lseek(fd ,(2 + i) * SECTOR_SIZE, SEEK_SET);

        if (read(fd , buffer , SECTOR_SIZE) != SECTOR_SIZE)
        {
            perror("Failed to read GPT entry");
            return;
        }
        // print the type GUID and starting LBA of each GPT partition entry.
        printf("GPT Partition %d: Type GUID: %02X%02X%02X%02X, Start LBA: %u\n", i + 1, buffer[0], buffer[1], buffer[2], buffer[3], *((uint32_t *)(buffer + 32)));
    }
}

/* ************** Definition section End *************** */



/* *******************************************************
    User               Date                 time
   *************************************** ***************
   kareem hamam       28 aug 2024           7:36 pm
*/

