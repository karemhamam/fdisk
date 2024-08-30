/* *******************************************************
   * @File                 : fdisk.h
   * @Author               : mohamed bahget hamam
   * @email                : mahamedhamam15@gmial.com
   *******************************************************
*/
#ifndef _FDISK_H 
#define _FDISK_H

#include <stdint.h>        // to use uint8_t.  

/* **************** Macros section start *************** */

#define SECTOR_SIZE                (512)      // size of a sector in bytes.
#define PARTITION_ENTRY_SIZE       (16 )      // size of a partition entry in the MBR.
#define MBR_PARTITION_TABLE_OFFSET (446)      // offset from the start of the MBR to the partition table.
#define MBR_SIGNATURE_OFFSET       (510)      // offset of the MBR signature.
#define MBR_SIGNATURE              (0xAA55)   //the expected signature at the end of MBR.
#define GPT_HEADEER_SIGNATURE      "EFI PART" // the ASCII string identifing a GPT header.
#define GPT_HEADER_SIZE            (92)       // the size of GPT  header.
#define GPT_PARTITION_ENTRY_SIZE   (128)      //the size of a GPT partition entry.
#define GPT_PARTITION_ARRAY_SIZE   (128)      // max number of GPT partition entries to read.

/* **************** Macros section End *************** */





/* **************** Declaration section start *************** */
   
struct partition_entry     // struct. to represent a partition entry in the MBR.
{

    uint8_t status;        // status of the partition.
    uint8_t chs_first[3];  // cclinder-head-sector address.
    uint8_t type;          // type of the partition.
    uint8_t chs_last[3];   // CHS address of the last sector of the partition.
    uint8_t lba_first;     // logical block addressing of the first sector.
    uint8_t sectors;       // otal number of sectors.

};


void print_partition_entry (struct partition_entry *entry , int index); 
/* 
   * @brief              : function to print details of a partition entry.
   *
   * @functionality      : prints the partition number, type, starting LBA, and size in sectors using 'printf'.
   * 
   * @param=>(entry)     : pointer to a partition_entry struct.
   * 
   * @param=>(index)     : index of the partition entry.
   
*/


void parse_logical_partitions(int fd , uint32_t ebr_start , int32_t ebr_current);
/* 
   * @brief                 : recursively parses and prints loicl partitions within an extended patition.
   * 
   * @param=>(fd)           : file descriptor of the device.
   * 
   * @param=>(ebr_start)    : LBA of the start of the extended boot record.
   * 
   * @param=>(ebr_current)  : current LBA being read.
   
*/


void parse_gpt (int fd);
/* 
   * @brief                 : parses the GPT partition table.
   * 
   * @param=>(fd)           : file descriptor of the device.
   
*/


/* ************** Declaration section End *************** */


#endif

/* *******************************************************
    User               Date                 time
   *************************************** ***************
   kareem hamam       28 aug 2024           7:36 pm
*/

