/* ------   Application For EverBright   --------------------------------------------------------------
P0, T4, T3, T2, T1
P1, T8, T7, T6, T5
------------------------------------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "nse.h"
#include "npu_platform.h"
#include "TcError.h"
#include <time.h>

void str2binary(char* str, int width);
void binary2str(char* in, char* out, int width);
void displayresponse(NseLookupResponse* response);

#define TABLE1_SIZE 25*1024
#define TABLE2_SIZE 25*1024
#define TABLE3_SIZE 25*1024
#define TABLE4_SIZE 25*1024

int main(int argc, const char* argv[])
{
    NseError nseStatus = Nse_OK;
    NseManagerDp* MyStargate = NULL;
    NseTable* MyTableBuff = NULL;
    NseTable* MyTable = NULL;
    NseTableRecord* MyRecord = NULL;
    NseTableRecord* MyRecordBuff = NULL;
    NseProfile* MyProfile = NULL;
    NseProfile* MyProfileBuff = NULL;
    NseProfileTableInfo* MyTableInfo = NULL;
    NseProfileKguSegment* MyKguSegment = NULL;
    NseLookupResponse* MyResponse = NULL;
    NseAdBlock* MyAD = NULL;
    NseAdBlock* MyADBuff = NULL;

    char strBuff[1000] = "";
    char data[1000] = "";
    char data_tmp[1000] = "";
    char rules[1000] = "";
    char mask[1000] = "";
    char bitmask[1000] = "";
    char search[1000] = "";
    char searchIn[1000] = "";
    int i,j,k;
    int online_md;
    int tbl_size;
    int unmask_bit;

    time_t t;
    srand((unsigned)time(NULL));
	
    printf("Enter 1 for online mode or 0 for offline md\n");
    scanf("%d", &online_md);
	
    printf("Enter table size for T5-T8, based on K:\n");
    scanf("%d", &tbl_size);
    tbl_size = tbl_size * 1024; 	

    const char* v = NseManagerDp_GetVersion();
    printf("STARGATE SDK Version: %s\n", v);

    nseStatus = platform_init(argc, argv);  // bring up the board, replace implementation according to board setting
    if (nseStatus)
    {
        printf("Platform initialization failed\n");
        return nseStatus;
    }

    // Create NSE Handler
    nseStatus = NseManagerDp_Create(&MyStargate, NSE_MODE_SINGLE_PORT);  // required in the beginning, single port mode
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to create manager\n");
        goto done;
    }

    nseStatus = NseAdBlock_Create(&MyADBuff);
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to create AD table\n");
        goto done;
    }
    // ------------------------       Add AD ----------------------------------------------------------
    MyADBuff->baseAddr = 0;   //  lowest priority for this AD table is 0
    MyADBuff->size = 512*1024;
    MyADBuff->width = 32;    
    nseStatus = NseManagerDp_AdBlock_Add(MyStargate, MyADBuff, 0, 0);  // add this AD block to port 0, id of this AD is 0
    if (nseStatus != Nse_OK) 
    {
        printf("Error: fail to add AD table\n");
        goto done;
    }
  
    MyADBuff->size = 512*1024;
    nseStatus = NseManagerDp_AdBlock_Add(MyStargate, MyADBuff, 0, 1);  
    if (nseStatus != Nse_OK) 
    {
        printf("Error: fail to add AD table\n");
        goto done;
    }
	   
	MyADBuff->size = 512*1024;
    nseStatus = NseManagerDp_AdBlock_Add(MyStargate, MyADBuff, 0, 2);  
    if (nseStatus != Nse_OK) 
    {
        printf("Error: fail to add AD table\n");
        goto done;
    }
   
    MyADBuff->size = 512*1024;
    nseStatus = NseManagerDp_AdBlock_Add(MyStargate, MyADBuff, 0, 3);  
    if (nseStatus != Nse_OK) 
    {
        printf("Error: fail to add AD table\n");
        goto done;
    }
	
	MyADBuff->size = 1024*1024;
    nseStatus = NseManagerDp_AdBlock_Add(MyStargate, MyADBuff, 0, 4);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add AD table\n");
        goto done;
    }

	MyADBuff->size = 1024*1024;
    nseStatus = NseManagerDp_AdBlock_Add(MyStargate, MyADBuff, 0, 5);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add AD table\n");
        goto done;
    }

    MyADBuff->size = 1024*1024;
    nseStatus = NseManagerDp_AdBlock_Add(MyStargate, MyADBuff, 0, 6);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add AD table\n");
        goto done;
    }

	MyADBuff->size = 1024*1024;
    nseStatus = NseManagerDp_AdBlock_Add(MyStargate, MyADBuff, 0, 7);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add AD table\n");
        goto done;
    }
    // ---------------------------------------------------------------------------------------------------------

    // Add Table
    nseStatus = NseTable_Create(&MyTableBuff);  // allocate memory for table handler
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to create table\n");
        goto done;
    }
	
    MyTableBuff->name = "T1";
    MyTableBuff->depth = TABLE1_SIZE;  
    MyTableBuff->width = 160; 
    MyTableBuff->adBlock = NseManagerDp_AdBlock_Get(MyStargate, 0, 0);
    nseStatus = NseManagerDp_Table_Add(MyStargate, MyTableBuff, 0, 0);  // add table 0 to port 0
    if (nseStatus != Nse_OK) 
    {
        printf("Error: fail to add table\n");
        goto done;
    }
	
    MyTableBuff->name = "T2";
    MyTableBuff->depth = TABLE2_SIZE;   
    MyTableBuff->width = 160;   
    MyTableBuff->adBlock = NseManagerDp_AdBlock_Get(MyStargate, 0, 1);
    nseStatus = NseManagerDp_Table_Add(MyStargate, MyTableBuff, 0, 1);  // add table 1 to port 0
    if (nseStatus != Nse_OK) 
    {
        printf("Error: fail to add table\n");
        goto done;
    }

    MyTableBuff->name = "T3";
    MyTableBuff->depth = TABLE3_SIZE;  
    MyTableBuff->width = 160;     
    MyTableBuff->adBlock = NseManagerDp_AdBlock_Get(MyStargate, 0, 2);
    nseStatus = NseManagerDp_Table_Add(MyStargate, MyTableBuff, 0, 2);  
    if (nseStatus != Nse_OK) 
    {
        printf("Error: fail to add table\n");
        goto done;
    }

    MyTableBuff->name = "T4";
    MyTableBuff->depth = TABLE4_SIZE;  
    MyTableBuff->width = 160;     
    MyTableBuff->adBlock = NseManagerDp_AdBlock_Get(MyStargate, 0, 3);
    nseStatus = NseManagerDp_Table_Add(MyStargate, MyTableBuff, 0, 3);  
    if (nseStatus != Nse_OK) 
    {
        printf("Error: fail to add table\n");
        goto done;
    }
	
    MyTableBuff->name = "T5";
    MyTableBuff->depth = tbl_size;  
    MyTableBuff->width = 320;     
    MyTableBuff->adBlock = NseManagerDp_AdBlock_Get(MyStargate, 0, 4);
    nseStatus = NseManagerDp_Table_Add(MyStargate, MyTableBuff, 0, 4);  
    if (nseStatus != Nse_OK) 
    {
        printf("Error: fail to add table\n");
        goto done;
    }

    MyTableBuff->name = "T6";
    MyTableBuff->depth = tbl_size; 
    MyTableBuff->width = 320;   
    MyTableBuff->adBlock = NseManagerDp_AdBlock_Get(MyStargate, 0, 5);
    nseStatus = NseManagerDp_Table_Add(MyStargate, MyTableBuff, 0, 5); 
    if (nseStatus != Nse_OK) 
    {
        printf("Error: fail to add table\n");
        goto done;
    }
	
    MyTableBuff->name = "T7";
    MyTableBuff->depth = tbl_size;
    MyTableBuff->width = 320;  
    MyTableBuff->adBlock = NseManagerDp_AdBlock_Get(MyStargate, 0, 6);
    nseStatus = NseManagerDp_Table_Add(MyStargate, MyTableBuff, 0, 6);  
    if (nseStatus != Nse_OK) 
    {
        printf("Error: fail to add table\n");
        goto done;
    }
	
    MyTableBuff->name = "T8";
    MyTableBuff->depth = tbl_size; 
    MyTableBuff->width = 320;     
    MyTableBuff->adBlock = NseManagerDp_AdBlock_Get(MyStargate, 0, 7);
    nseStatus = NseManagerDp_Table_Add(MyStargate, MyTableBuff, 0, 7);  
    if (nseStatus != Nse_OK) 
    {
        printf("Error: fail to add table\n");
        goto done;
    }

    // Add Profile
    nseStatus = NseProfile_Create(&MyProfileBuff);  // allocate memory for profile handler
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to create profile\n");
        goto done;
    }

    nseStatus = NseProfileTableInfo_Create(&MyTableInfo);
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to create tableInfo\n");
        goto done;
    }
    nseStatus = NseProfileKguSegment_Create(&MyKguSegment);  // allocate memory for kgu segment handler; used to construct key
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to create KGU\n");
        goto done;
    }

    // Profile-0 includes T3,T2,T1,T0
    nseStatus = NseManagerDp_Profile_Add(MyStargate, MyProfileBuff, 0, 0); 
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add profile\n");
        goto done;
    }
    MyProfile = NseManagerDp_Profile_Get(MyStargate, 0, 0);  // get profile handler from port 0 to add tableInfo and KguSegments in
	
    // -------------------    Add table info and kgu segment   -------------------------------------------------
	// T1 
    MyTableInfo->table = NseManagerDp_Table_Get(MyStargate, 0, 0);  // set search request on T1
    MyTableInfo->chanMask = 1;   
    MyTableInfo->keySelect = 0;   
    strcpy(bitmask, "11111111111111111111111111111111111111111111111111111111111111111111111111111111");  
    str2binary(bitmask, 80);  
    MyTableInfo->bitmask = bitmask;
    nseStatus = NseProfile_TableInfo_Add(MyProfile, MyTableInfo, 0);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add search table\n");
        goto done;
    }
    MyKguSegment->offset = 0;
    MyKguSegment->byteCount = 15;
    nseStatus = NseProfile_KguSegment_Add(MyProfile, 0, MyKguSegment, 0);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add kgu segment\n");
        goto done;
    }
    MyKguSegment->offset = 16;
    MyKguSegment->byteCount = 3;
    nseStatus = NseProfile_KguSegment_Add(MyProfile, 0, MyKguSegment, 1);
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add kgu segment\n");
        goto done;
    }
	
   	// T2 
    MyTableInfo->table = NseManagerDp_Table_Get(MyStargate, 0, 1);  // set search request on T1
    MyTableInfo->chanMask = 2;    // search T2 in channel-1, 0010
    MyTableInfo->keySelect = 1;   // use key from 2nd KGU to search
    strcpy(bitmask, "11111111111111111111111111111111111111111111111111111111111111111111111111111111");  
    str2binary(bitmask, 80);  // convert bitmask string to binary memory
    MyTableInfo->bitmask = bitmask;
    nseStatus = NseProfile_TableInfo_Add(MyProfile, MyTableInfo, 1);  // add T1 to profile 0 so that profile 0 will search on T1
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add search table\n");
        goto done;
    }
    MyKguSegment->offset = 20;
    MyKguSegment->byteCount = 15;
    nseStatus = NseProfile_KguSegment_Add(MyProfile, 1, MyKguSegment, 0);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add kgu segment\n");
        goto done;
    }
    MyKguSegment->offset = 36;
    MyKguSegment->byteCount = 3;
    nseStatus = NseProfile_KguSegment_Add(MyProfile, 1, MyKguSegment, 1);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add kgu segment\n");
        goto done;
    }

	// T3
    MyTableInfo->table = NseManagerDp_Table_Get(MyStargate, 0, 2);  // set search request on T1
    MyTableInfo->chanMask = 4;    // search T3 in channel-2, 0100
    MyTableInfo->keySelect = 2;   // use key from 3rd KGU to search
    strcpy(bitmask, "11111111111111111111111111111111111111111111111111111111111111111111111111111111");  
    str2binary(bitmask, 80);  // convert bitmask string to binary memory
    MyTableInfo->bitmask = bitmask;
    nseStatus = NseProfile_TableInfo_Add(MyProfile, MyTableInfo, 2);  // add T1 to profile 0 so that profile 0 will search on T1
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add search table\n");
        goto done;
    }
    MyKguSegment->offset = 40;
    MyKguSegment->byteCount = 15;
    nseStatus = NseProfile_KguSegment_Add(MyProfile, 2, MyKguSegment, 0);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add kgu segment\n");
        goto done;
    }
    MyKguSegment->offset = 56;
    MyKguSegment->byteCount = 3;
    nseStatus = NseProfile_KguSegment_Add(MyProfile, 2, MyKguSegment, 1);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add kgu segment\n");
        goto done;
    }
	
	// T4
    MyTableInfo->table = NseManagerDp_Table_Get(MyStargate, 0, 3);  // set search request on T1
    MyTableInfo->chanMask = 8;    // search T4 in channel-3, 1000
    MyTableInfo->keySelect = 3;   // use key from 4th KGU to search
    strcpy(bitmask, "11111111111111111111111111111111111111111111111111111111111111111111111111111111");  
    str2binary(bitmask, 80);  // convert bitmask string to binary memory
    MyTableInfo->bitmask = bitmask;
    nseStatus = NseProfile_TableInfo_Add(MyProfile, MyTableInfo, 3);  // add T1 to profile 0 so that profile 0 will search on T1
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add search table\n");
        goto done;
    }
    MyKguSegment->offset = 60;
    MyKguSegment->byteCount = 15;
    nseStatus = NseProfile_KguSegment_Add(MyProfile, 3, MyKguSegment, 0); 
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add kgu segment\n");
        goto done;
    }
    MyKguSegment->offset = 76;
    MyKguSegment->byteCount = 3;
    nseStatus = NseProfile_KguSegment_Add(MyProfile, 3, MyKguSegment, 1);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add kgu segment\n");
        goto done;
    }

	// ----------------------------------------------------------------------------------------------------------------------------------------------------
	
	nseStatus = NseManagerDp_Profile_Add(MyStargate, MyProfileBuff, 0, 1);  // add one profile to port 0
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add profile\n");
        goto done;
    }
    MyProfile = NseManagerDp_Profile_Get(MyStargate, 0, 1); 
	
    // -------------------    Add table info and kgu segment   -------------------------------------------------
	// T5
    MyTableInfo->table = NseManagerDp_Table_Get(MyStargate, 0, 4);  
    MyTableInfo->chanMask = 1;    // search T1 in channel-0, 0001
    MyTableInfo->keySelect = 0;   // use key from 1st KGU to search
    strcpy(bitmask, "11111111111111111111111111111111111111111111111111111111111111111111111111111111"
                    "11111111111111111111111111111111111111111111111111111111111111111111111111111111");  
    str2binary(bitmask, 160);  
    MyTableInfo->bitmask = bitmask;
    nseStatus = NseProfile_TableInfo_Add(MyProfile, MyTableInfo, 4);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add search table\n");
        goto done;
    }
    MyKguSegment->offset = 0;
    MyKguSegment->byteCount = 15;
    nseStatus = NseProfile_KguSegment_Add(MyProfile, 0, MyKguSegment, 0); 
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add kgu segment\n");
        goto done;
    }
    MyKguSegment->offset = 16;
    MyKguSegment->byteCount = 15;
    nseStatus = NseProfile_KguSegment_Add(MyProfile, 0, MyKguSegment, 1);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add kgu segment\n");
        goto done;
    }
    MyKguSegment->offset = 32;
    MyKguSegment->byteCount = 7;
    nseStatus = NseProfile_KguSegment_Add(MyProfile, 0, MyKguSegment, 2);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add kgu segment\n");
        goto done;
    }
	
   	// T6
    MyTableInfo->table = NseManagerDp_Table_Get(MyStargate, 0, 5);  
    MyTableInfo->chanMask = 2;    // search T2 in channel-1, 0010
    MyTableInfo->keySelect = 1;   // use key from 2nd KGU to search
    MyTableInfo->bitmask = bitmask;
    nseStatus = NseProfile_TableInfo_Add(MyProfile, MyTableInfo, 5);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add search table\n");
        goto done;
    }
    MyKguSegment->offset = 0;
    MyKguSegment->byteCount = 15;
    nseStatus = NseProfile_KguSegment_Add(MyProfile, 1, MyKguSegment, 0); 
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add kgu segment\n");
        goto done;
    }
    MyKguSegment->offset = 16;
    MyKguSegment->byteCount = 15;
    nseStatus = NseProfile_KguSegment_Add(MyProfile, 1, MyKguSegment, 1);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add kgu segment\n");
        goto done;
    }
    MyKguSegment->offset = 32;
    MyKguSegment->byteCount = 7;
    nseStatus = NseProfile_KguSegment_Add(MyProfile, 1, MyKguSegment, 2);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add kgu segment\n");
        goto done;
    }

	// T7
    MyTableInfo->table = NseManagerDp_Table_Get(MyStargate, 0, 6); 
    MyTableInfo->chanMask = 4;    // search in channel-2, 0100
    MyTableInfo->keySelect = 2;   // use key from 3rd KGU to search
    MyTableInfo->bitmask = bitmask;
    nseStatus = NseProfile_TableInfo_Add(MyProfile, MyTableInfo, 6); 
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add search table\n");
        goto done;
    }
    MyKguSegment->offset = 0;
    MyKguSegment->byteCount = 15;
    nseStatus = NseProfile_KguSegment_Add(MyProfile, 2, MyKguSegment, 0); 
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add kgu segment\n");
        goto done;
    }
    MyKguSegment->offset = 16;
    MyKguSegment->byteCount = 15;
    nseStatus = NseProfile_KguSegment_Add(MyProfile, 2, MyKguSegment, 1);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add kgu segment\n");
        goto done;
    }
    MyKguSegment->offset = 32;
    MyKguSegment->byteCount = 7;
    nseStatus = NseProfile_KguSegment_Add(MyProfile, 2, MyKguSegment, 2);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add kgu segment\n");
        goto done;
    }
	
	// T8
    MyTableInfo->table = NseManagerDp_Table_Get(MyStargate, 0, 7);  
    MyTableInfo->chanMask = 8;    // search in channel-3, 1000
    MyTableInfo->keySelect = 3;   // use key from 4th KGU to search
    MyTableInfo->bitmask = bitmask;
    nseStatus = NseProfile_TableInfo_Add(MyProfile, MyTableInfo, 7);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add search table\n");
        goto done;
    }
    MyKguSegment->offset = 0;
    MyKguSegment->byteCount = 15;
    nseStatus = NseProfile_KguSegment_Add(MyProfile, 3, MyKguSegment, 0); 
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add kgu segment\n");
        goto done;
    }
    MyKguSegment->offset = 16;
    MyKguSegment->byteCount = 15;
    nseStatus = NseProfile_KguSegment_Add(MyProfile, 3, MyKguSegment, 1);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add kgu segment\n");
        goto done;
    }
    MyKguSegment->offset = 32;
    MyKguSegment->byteCount = 7;
    nseStatus = NseProfile_KguSegment_Add(MyProfile, 3, MyKguSegment, 2);  
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to add kgu segment\n");
        goto done;
    }
	// ----------------------------------------------------------------------------------------------------------------------------------------------------
	
	// Offline part rules adding
    nseStatus = NseTableRecord_Create(&MyRecordBuff);
    if (nseStatus != Nse_OK) {
        printf("Error: fail to create record\n");
        goto done;
    }
	
    // T1~T4
    char filename[100] = "";
    char maskname[100] = "";
    FILE *fp;
    FILE *fp_mask;
	
    if(online_md == 0) {
        for(k = 0; k < 4; k++) {
            sprintf(filename, "rules_incre%d.txt", k+1);
            sprintf(maskname, "mask_incre%d.txt", k+1);
            if((fp = fopen(filename, "w")) == NULL) printf("%s cann't be opened\n", filename);
            if((fp_mask = fopen(maskname, "w")) == NULL) printf("%s cann't be opened\n", maskname);
            //fp_mask = fopen(maskname, "w");
	        
            MyTable = NseManagerDp_Table_Get(MyStargate, 0, k);  
            MyRecordBuff->format = NSE_TABLERECORD_FORMAT_DATA_MASK;  
            MyRecordBuff->width = 160; 
            MyRecordBuff->range1Lower = NSE_TABLERECORD_RANGE_LOWER_NONE;  
            MyRecordBuff->range1Upper = NSE_TABLERECORD_RANGE_UPPER_NONE;  
            MyRecordBuff->range2Lower = NSE_TABLERECORD_RANGE_LOWER_NONE;  
            MyRecordBuff->range2Upper = NSE_TABLERECORD_RANGE_UPPER_NONE;
	        
            MyAD = NseManagerDp_AdBlock_Get(MyStargate, 0, k);
            for(i = 0; i < TABLE1_SIZE; i++) {
                // Add rules for T1
                // -----------------------------------------------------------------------------------
                strcat(data_tmp, "0000101000000001");
                for(j = 0; j < 16; j++) {
                    if( ((i+258)>>(15-j))%2 == 1) {
                        strcat(data_tmp, "1");
                    }
                    else {
                        strcat(data_tmp, "0");
                    }
                }
                strcat(data, data_tmp);
                strcat(data, "00001010111111100000000100000001000010010010100100000000011001000001000100000001000000000000000000000000000000000000000000000000");
                strcpy(rules, data);
                strcpy(mask, "00000000000000000000000000000000000000001111111111111111111111110000000000000000"
                             "00000000000000000000000000000000111111111111111111111111111111111111111111111111");   
                fprintf(fp, "%s\n", rules);
                fprintf(fp_mask, "%s\n", mask);
                str2binary(data, 160);  
                str2binary(mask, 160);  
                MyRecordBuff->data = data;  
                MyRecordBuff->mask = mask;  
                MyRecordBuff->priority = i;
            
                str2binary(data_tmp, 32);
                nseStatus = NseAdBlock_AD_Add(MyAD, MyRecordBuff->priority, data_tmp); 
                if (nseStatus != Nse_OK)
                {
                    printf("Error: fail to add AD record\n");
                    goto done;
                }
            
                nseStatus = NseTable_Record_Add(MyTable, MyRecordBuff, MyRecordBuff->priority); 
                if (nseStatus != Nse_OK)
                {
                    printf("Error: fail to add record at %d in T1\n", i);
                    goto done;
                }
	        	
                memset(data, 0, 1000);
                memset(data_tmp, 0, 1000);
                memset(rules, 0, 1000);
                memset(mask, 0, 1000);
                if((i+1)%1024 == 0) printf("1K rules have been added for %s, group number %d\n", MyTable->name, i/1024);
                //--------------------------------------------------------------------------------------------------
            }
            fclose(fp);
            fclose(fp_mask);
        }
	
        // Add rules for T5~T8
        for(k = 0; k < 4; k++) {
            sprintf(filename, "rules_incre%d.txt", k+5);
            sprintf(maskname, "mask_incre%d.txt", k+5);
            fp = fopen(filename, "w");
            fp_mask = fopen(maskname, "w");
            MyTable = NseManagerDp_Table_Get(MyStargate, 0, 4+k);  
            
            MyRecordBuff->format = NSE_TABLERECORD_FORMAT_DATA_MASK;  
            MyRecordBuff->width = 320; 
            MyRecordBuff->range1Lower = NSE_TABLERECORD_RANGE_LOWER_NONE;  
            MyRecordBuff->range1Upper = NSE_TABLERECORD_RANGE_UPPER_NONE;  
            MyRecordBuff->range2Lower = NSE_TABLERECORD_RANGE_LOWER_NONE;  
            MyRecordBuff->range2Upper = NSE_TABLERECORD_RANGE_UPPER_NONE;
	        
            MyAD = NseManagerDp_AdBlock_Get(MyStargate, 0, 4+k);
            for(i = 0; i < tbl_size; i++) {
                if(i < 7504) {
                    for(j = 0; j < 8; j++) {
                        if( ((i%256)>>(7-j)) %2 == 1 )
                            strcat(data_tmp, "1");
                        else strcat(data_tmp, "0");
                    }
                    strcat(data_tmp, "00110000001100010011001000110011001101000011010100110110001101110011100000111001"
                                     "00111010001110110011110000111101001111100011111101000000010000010100001001000011"
                                     "0100010001000101010001100100011101001000010010010100101001001011010011000100110101001110");
                    for(j = 0; j < 8; j++) {
                        if( (((i/256) + 79)>>(7-j))%2 == 1) 
                            strcat(data_tmp, "1");
                        else  strcat(data_tmp, "0");
                    }
                    strcat(data_tmp, "00000000000000000000000000000000000000000000000000000000");
                    strcat(data, data_tmp);
                    strcpy(rules, data);
                    strcpy(mask, "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
                                 "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
                                 "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
                                 "00000000000000000000000011111111111111111111111111111111111111111111111111111111");
                    fprintf(fp, "%s\n", rules);
	                fprintf(fp_mask, "%s\n", mask);
                } else {
                    unmask_bit = 24 + 8*(rand()%((256-24)/8+1));
                    for(j = 0; j < unmask_bit; j++) {
                        strcat(data_tmp, rand()%2 == 1 ? "1" : "0"); 
                        strcat(mask,"0");
                    }
            
                    for(j = 0; j < (320 - unmask_bit); j++) {           
                        strcat(data_tmp, "0");
	                    strcat(mask, "1");
                    }
                    strcat(data, data_tmp);
                    strcpy(rules, data);
                    fprintf(fp, "%s\n", rules);
	                fprintf(fp_mask, "%s\n", mask);
                }
	    	
                str2binary(data, 320); 		
                str2binary(mask, 320);  
                MyRecordBuff->data = data;  
                MyRecordBuff->mask = mask;  
                MyRecordBuff->priority = i;
                
                str2binary(data_tmp, 32);
                nseStatus = NseAdBlock_AD_Add(MyAD, MyRecordBuff->priority, data_tmp); 
                if (nseStatus != Nse_OK)
                {
                    printf("Error: fail to add AD record\n");
                    goto done;
                }
        
                nseStatus = NseTable_Record_Add(MyTable, MyRecordBuff, i); 
                if (nseStatus != Nse_OK)
                {
                    printf("Error: fail to add record\n");
                    goto done;
                }
	        	
                memset(data, 0, 1000);
                memset(data_tmp, 0, 1000);
                memset(rules, 0, 1000);
                memset(mask, 0, 1000);
                if((i+1)%1024 == 0) printf("1K rules have been added for %s, group number %d\n", MyTable->name, i/1024);
            }
            fclose(fp);
            fclose(fp_mask);
        }
	
        time(&t);
        printf("All offline rules added done %s\n", ctime(&t));
    }
    // Offline part done
	
    // Start allocation and configure the TCAM database
    time(&t);
    printf("Before LockConfig at %s\n", ctime(&t));
    nseStatus = NseManagerDp_LockConfig(MyStargate);  // configure TCAM
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to configure TCAM\n");
        goto done;
    }
    time(&t);
    printf("After LockConfig at %s\n", ctime(&t));

    nseStatus = NseTableRecord_Create(&MyRecordBuff);
    if (nseStatus != Nse_OK) {
        printf("Error: fail to create record\n");
        goto done;
    }
	
    if(online_md == 1) {
        for(k = 0; k < 4; k++) {
            sprintf(filename, "rules_incre%d.txt", k+1);
            sprintf(maskname, "mask_incre%d.txt", k+1);
            if((fp = fopen(filename, "w")) == NULL) printf("%s cann't be opened\n", filename);
            if((fp_mask = fopen(maskname, "w")) == NULL) printf("%s cann't be opened\n", maskname);
            //fp_mask = fopen(maskname, "w");
	        
            MyTable = NseManagerDp_Table_Get(MyStargate, 0, k);  
            MyRecordBuff->format = NSE_TABLERECORD_FORMAT_DATA_MASK;  
            MyRecordBuff->width = 160; 
            MyRecordBuff->range1Lower = NSE_TABLERECORD_RANGE_LOWER_NONE;  
            MyRecordBuff->range1Upper = NSE_TABLERECORD_RANGE_UPPER_NONE;  
            MyRecordBuff->range2Lower = NSE_TABLERECORD_RANGE_LOWER_NONE;  
            MyRecordBuff->range2Upper = NSE_TABLERECORD_RANGE_UPPER_NONE;
	        
            MyAD = NseManagerDp_AdBlock_Get(MyStargate, 0, k);
            for(i = 0; i < TABLE1_SIZE; i++) {
                // Add rules for T1
                // -----------------------------------------------------------------------------------
                strcat(data_tmp, "0000101000000001");
                for(j = 0; j < 16; j++) {
                    if( ((i+258)>>(15-j))%2 == 1) {
                        strcat(data_tmp, "1");
                    }
                    else {
                        strcat(data_tmp, "0");
                    }
                }
                strcat(data, data_tmp);
                strcat(data, "00001010111111100000000100000001000010010010100100000000011001000001000100000001000000000000000000000000000000000000000000000000");
                strcpy(rules, data);
                strcpy(mask, "00000000000000000000000000000000000000001111111111111111111111110000000000000000"
                             "00000000000000000000000000000000111111111111111111111111111111111111111111111111");   
                fprintf(fp, "%s\n", rules);
                fprintf(fp_mask, "%s\n", mask);
                str2binary(data, 160);  
                str2binary(mask, 160);  
                MyRecordBuff->data = data;  
                MyRecordBuff->mask = mask;  
                MyRecordBuff->priority = i;
            
                str2binary(data_tmp, 32);
                nseStatus = NseAdBlock_AD_Add(MyAD, MyRecordBuff->priority, data_tmp); 
                if (nseStatus != Nse_OK)
                {
                    printf("Error: fail to add AD record\n");
                    goto done;
                }
            
                nseStatus = NseTable_Record_Add(MyTable, MyRecordBuff, MyRecordBuff->priority); 
                if (nseStatus != Nse_OK)
                {
                    printf("Error: fail to add record at %d in T1\n", i);
                    goto done;
                }
	        	
                memset(data, 0, 1000);
                memset(data_tmp, 0, 1000);
                memset(rules, 0, 1000);
                memset(mask, 0, 1000);
                if((i+1)%1024 == 0) printf("1K rules have been added for %s, group number %d\n", MyTable->name, i/1024);
                //--------------------------------------------------------------------------------------------------
            }
            fclose(fp);
            fclose(fp_mask);
        }
	
        // Add rules for T5~T8
        for(k = 0; k < 4; k++) {
            sprintf(filename, "rules_incre%d.txt", k+5);
            sprintf(maskname, "mask_incre%d.txt", k+5);
            fp = fopen(filename, "w");
            fp_mask = fopen(maskname, "w");
            MyTable = NseManagerDp_Table_Get(MyStargate, 0, 4+k);  
            
            MyRecordBuff->format = NSE_TABLERECORD_FORMAT_DATA_MASK;  
            MyRecordBuff->width = 320; 
            MyRecordBuff->range1Lower = NSE_TABLERECORD_RANGE_LOWER_NONE;  
            MyRecordBuff->range1Upper = NSE_TABLERECORD_RANGE_UPPER_NONE;  
            MyRecordBuff->range2Lower = NSE_TABLERECORD_RANGE_LOWER_NONE;  
            MyRecordBuff->range2Upper = NSE_TABLERECORD_RANGE_UPPER_NONE;
	        
            MyAD = NseManagerDp_AdBlock_Get(MyStargate, 0, 4+k);
            for(i = 0; i < tbl_size; i++) {
                if(i < 7504) {
                    for(j = 0; j < 8; j++) {
                        if( ((i%256)>>(7-j)) %2 == 1 )
                            strcat(data_tmp, "1");
                        else strcat(data_tmp, "0");
                    }
                    strcat(data_tmp, "00110000001100010011001000110011001101000011010100110110001101110011100000111001"
                                     "00111010001110110011110000111101001111100011111101000000010000010100001001000011"
                                     "0100010001000101010001100100011101001000010010010100101001001011010011000100110101001110");
                    for(j = 0; j < 8; j++) {
                        if( (((i/256) + 79)>>(7-j))%2 == 1) 
                            strcat(data_tmp, "1");
                        else  strcat(data_tmp, "0");
                    }
                    strcat(data_tmp, "00000000000000000000000000000000000000000000000000000000");
                    strcat(data, data_tmp);
                    strcpy(rules, data);
                    strcpy(mask, "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
                                 "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
                                 "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
                                 "00000000000000000000000011111111111111111111111111111111111111111111111111111111");
                    fprintf(fp, "%s\n", rules);
	                fprintf(fp_mask, "%s\n", mask);
                } else {
                    unmask_bit = 24 + 8*(rand()%((256-24)/8+1));
                    for(j = 0; j < unmask_bit; j++) {
                        strcat(data_tmp, rand()%2 == 1 ? "1" : "0"); 
                        strcat(mask,"0");
                    }
            
                    for(j = 0; j < (320 - unmask_bit); j++) {           
                        strcat(data_tmp, "0");
	                    strcat(mask, "1");
                    }
                    strcat(data, data_tmp);
                    strcpy(rules, data);
                    fprintf(fp, "%s\n", rules);
	                fprintf(fp_mask, "%s\n", mask);
                }
	    	
                str2binary(data, 320); 		
                str2binary(mask, 320);  
                MyRecordBuff->data = data;  
                MyRecordBuff->mask = mask;  
                MyRecordBuff->priority = i;
                
                str2binary(data_tmp, 32);
                nseStatus = NseAdBlock_AD_Add(MyAD, MyRecordBuff->priority, data_tmp); 
                if (nseStatus != Nse_OK)
                {
                    printf("Error: fail to add AD record\n");
                    goto done;
                }
        
                nseStatus = NseTable_Record_Add(MyTable, MyRecordBuff, i); 
                if (nseStatus != Nse_OK)
                {
                    printf("Error: fail to add record\n");
                    goto done;
                }
	        	
                memset(data, 0, 1000);
                memset(data_tmp, 0, 1000);
                memset(rules, 0, 1000);
                memset(mask, 0, 1000);
                if((i+1)%1024 == 0) printf("1K rules have been added for %s, group number %d\n", MyTable->name, i/1024);
            }
            fclose(fp);
            fclose(fp_mask);
        }
	
        time(&t);
        printf("All rules added online done %s\n", ctime(&t));   	
    }
	
#if 1
    // Add search on this record
    nseStatus = NseLookupResponse_Create(&MyResponse);
    if (nseStatus != Nse_OK)
    {
        printf("Error: fail to create response buffer\n");
        goto done;
    }

    fp = fopen("rules_incre1.txt", "r");
    FILE *fp_res = fopen("rules_incre1_result.txt", "w");
    for(i = 0; i < TABLE1_SIZE; i++) {
        fgets(search, sizeof(search), fp);
        //printf("search key is %s\n", search);
        fprintf(fp_res, "SearchKey %s", search);
        str2binary(search, 160);  // convert string to binary
        nseStatus = NseManagerDp_Lookup(MyStargate, 0, 0, 0, 1, search, 20, MyResponse);  
        if (nseStatus != Nse_OK)
        {
            printf("Error: fail to perform search\n");
            goto done;
        }

        fprintf(fp_res, "Channel    Ready    Match     Priority\n");
        memset(strBuff, 0, sizeof(strBuff));
        binary2str(MyResponse->adValues[0], strBuff, 4);
        fprintf(fp_res, "%7d    %5d    %5d     %d    %s\n\n", 0, MyResponse->readies[0], MyResponse->matches[0], MyResponse->priorities[0], strBuff);
        if(MyResponse->matches[0] == 0)
            printf("%7d    %5d    %5d     %d\n\n", 0, MyResponse->readies[0], MyResponse->matches[0], MyResponse->priorities[0]);
        //displayResults(MyResponse->readies, MyResponse->matches, MyResponse->priorities);
    }
    printf("All Table-1 search done\n");
	
    FILE *fp2 = fopen("rules_incre2.txt", "r");
    FILE *fp_res2 = fopen("rules_incre_result2.txt", "w");
    for(i = 0; i < TABLE2_SIZE; i++) {
        fgets(searchIn, sizeof(searchIn), fp2);
        //printf("search key is %s\n", search);
        fprintf(fp_res2, "SearchKey %s", searchIn);
        strcpy(search, "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
                       "00000000000000000000000000000000000000000000000000000000000000000000000000000000");
        strcat(search, searchIn);
        str2binary(search, 320);  // convert string to binary
        nseStatus = NseManagerDp_Lookup(MyStargate, 0, 0, 0, 3, search, 40, MyResponse);  
        if (nseStatus != Nse_OK)
        {
            printf("Error: fail to perform search\n");
            goto done;
        }

        memset(searchIn, 0, 1000);
        fprintf(fp_res2, "Channel    Ready    Match     Priority\n");
        memset(strBuff, 0, sizeof(strBuff));
        
        binary2str(MyResponse->adValues[1], strBuff, 4);
        fprintf(fp_res2, "%7d    %5d    %5d     %d    %s\n\n", 0, MyResponse->readies[1], MyResponse->matches[1], MyResponse->priorities[1], strBuff);
        //if(MyResponse->matches[1] == 0)
        //    printf("%7d    %5d    %5d     %d\n\n", 0, MyResponse->readies[1], MyResponse->matches[1], MyResponse->priorities[1]);
        //displayResults(MyResponse->readies, MyResponse->matches, MyResponse->priorities);
    }
    printf("All Table-2 search done\n");
	
    FILE *fp3 = fopen("rules_incre3.txt", "r");
    FILE *fp_res3 = fopen("rules_incre_result3.txt", "w");
    for(i = 0; i < TABLE3_SIZE; i++) {
        fgets(searchIn, sizeof(searchIn), fp3);
        //printf("search key is %s\n", search);
        fprintf(fp_res3, "SearchKey %s", searchIn);
        strcpy(search, "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
                       "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
                       "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
                       "00000000000000000000000000000000000000000000000000000000000000000000000000000000");
        strcat(search, searchIn);
        str2binary(search, 480);  // convert string to binary
        nseStatus = NseManagerDp_Lookup(MyStargate, 0, 0, 0, 5, search, 60, MyResponse);  
        if (nseStatus != Nse_OK)
        {
            printf("Error: fail to perform search\n");
            goto done;
        }

        memset(searchIn, 0, 1000);
        fprintf(fp_res3, "Channel    Ready    Match     Priority     AD\n");
        memset(strBuff, 0, sizeof(strBuff));
        binary2str(MyResponse->adValues[2], strBuff, 4);
        fprintf(fp_res3, "%7d    %5d    %5d     %d    %s\n\n", 0, MyResponse->readies[2], MyResponse->matches[2], MyResponse->priorities[2], strBuff);
        //if(MyResponse->matches[0] == 0)
        //    printf("%7d    %5d    %5d     %d\n\n", 0, MyResponse->readies[0], MyResponse->matches[0], MyResponse->priorities[0]);
        //displayResults(MyResponse->readies, MyResponse->matches, MyResponse->priorities);
    }
    printf("All Table-3 search done\n");
	
    FILE *fp4 = fopen("rules_incre4.txt", "r");
    FILE *fp_res4 = fopen("rules_incre_result4.txt", "w");
    for(i = 0; i < TABLE4_SIZE; i++) {
        fgets(searchIn, sizeof(searchIn), fp4);
        //printf("search key is %s\n", search);
        fprintf(fp_res4, "SearchKey %s", searchIn);
        strcpy(search, "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
                       "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
                       "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
                       "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
                       "00000000000000000000000000000000000000000000000000000000000000000000000000000000"
                       "00000000000000000000000000000000000000000000000000000000000000000000000000000000");
        strcat(search, searchIn);
        str2binary(search, 640);  // convert string to binary
        nseStatus = NseManagerDp_Lookup(MyStargate, 0, 0, 0, 7, search, 80, MyResponse);  
        if (nseStatus != Nse_OK)
        {
            printf("Error: fail to perform search\n");
            goto done;
        }

        fprintf(fp_res4, "Channel    Ready    Match     Priority     AD\n");
        memset(strBuff, 0, sizeof(strBuff));
        binary2str(MyResponse->adValues[3], strBuff, 4);
        fprintf(fp_res4, "%7d    %5d    %5d     %d    %s\n\n", 0, MyResponse->readies[3], MyResponse->matches[3], MyResponse->priorities[3], strBuff);
        //if(MyResponse->matches[0] == 0)
        //    printf("%7d    %5d    %5d     %d\n\n", 0, MyResponse->readies[0], MyResponse->matches[0], MyResponse->priorities[0]);
        //displayResults(MyResponse->readies, MyResponse->matches, MyResponse->priorities);
    }
    printf("All Table-4 search done\n");
	
    FILE *fp5 = fopen("rules_incre5.txt", "r");
    FILE *fp_res5 = fopen("rules_incre_result5.txt", "w");
    for(i = 0; i < tbl_size; i++) {
        fgets(search, sizeof(search), fp5);
        //printf("search key is %s\n", search);
        fprintf(fp_res5, "SearchKey %s", search);
        str2binary(search, 320);  // convert string to binary
        nseStatus = NseManagerDp_Lookup(MyStargate, 0, 1, 0, 3, search, 40, MyResponse);  
        if (nseStatus != Nse_OK)
        {
            printf("Error: fail to perform search\n");
            goto done;
        }

        fprintf(fp_res5, "Channel    Ready    Match     Priority     AD\n");
        memset(strBuff, 0, sizeof(strBuff));
        binary2str(MyResponse->adValues[0], strBuff, 4);
        fprintf(fp_res5, "%7d    %5d    %5d     %d    %s\n\n", 0, MyResponse->readies[0], MyResponse->matches[0], MyResponse->priorities[0], strBuff);
        //if(MyResponse->matches[0] == 0)
        //    printf("%7d    %5d    %5d     %d\n\n", 0, MyResponse->readies[0], MyResponse->matches[0], MyResponse->priorities[0]);
        //displayResults(MyResponse->readies, MyResponse->matches, MyResponse->priorities);
    }
    printf("All Table-5 search done\n");
	
    FILE *fp6 = fopen("rules_incre6.txt", "r");
    FILE *fp_res6 = fopen("rules_incre_result6.txt", "w");
    for(i = 0; i < tbl_size; i++) {
        fgets(search, sizeof(search), fp6);
        //printf("search key is %s\n", search);
        fprintf(fp_res6, "SearchKey %s", search);
        str2binary(search, 320);  // convert string to binary
        nseStatus = NseManagerDp_Lookup(MyStargate, 0, 1, 0, 3, search, 40, MyResponse);  
        if (nseStatus != Nse_OK)
        {
            printf("Error: fail to perform search\n");
            goto done;
        }

        fprintf(fp_res6, "Channel    Ready    Match     Priority     AD\n");
        memset(strBuff, 0, sizeof(strBuff));
        binary2str(MyResponse->adValues[1], strBuff, 4);
        fprintf(fp_res6, "%7d    %5d    %5d     %d    %s\n\n", 0, MyResponse->readies[1], MyResponse->matches[1], MyResponse->priorities[1], strBuff);
        //if(MyResponse->matches[0] == 0)
        //    printf("%7d    %5d    %5d     %d\n\n", 0, MyResponse->readies[0], MyResponse->matches[0], MyResponse->priorities[0]);
        //displayResults(MyResponse->readies, MyResponse->matches, MyResponse->priorities);
    }
    printf("All Table-6 search done\n");
	
    FILE *fp7 = fopen("rules_incre7.txt", "r");
    FILE *fp_res7 = fopen("rules_incre_result7.txt", "w");
    for(i = 0; i < tbl_size; i++) {
        fgets(search, sizeof(search), fp7);
        //printf("search key is %s\n", search);
        fprintf(fp_res7, "SearchKey %s", search);
        str2binary(search, 320);  // convert string to binary
        nseStatus = NseManagerDp_Lookup(MyStargate, 0, 1, 0, 3, search, 40, MyResponse);  
        if (nseStatus != Nse_OK)
        {
            printf("Error: fail to perform search\n");
            goto done;
        }

        fprintf(fp_res7, "Channel    Ready    Match     Priority     AD\n");
        memset(strBuff, 0, sizeof(strBuff));
        binary2str(MyResponse->adValues[2], strBuff, 4);
        fprintf(fp_res7, "%7d    %5d    %5d     %d    %s\n\n", 0, MyResponse->readies[2], MyResponse->matches[2], MyResponse->priorities[2], strBuff);
        //if(MyResponse->matches[0] == 0)
        //    printf("%7d    %5d    %5d     %d\n\n", 0, MyResponse->readies[0], MyResponse->matches[0], MyResponse->priorities[0]);
        //displayResults(MyResponse->readies, MyResponse->matches, MyResponse->priorities);
    }
    printf("All Table-7 search done\n");
	
    FILE *fp8 = fopen("rules_incre8.txt", "r");
    FILE *fp_res8 = fopen("rules_incre_result8.txt", "w");
    for(i = 0; i < tbl_size; i++) {
        fgets(search, sizeof(search), fp8);
        //printf("search key is %s\n", search);
        fprintf(fp_res8, "SearchKey %s", search);
        str2binary(search, 320);  // convert string to binary
        nseStatus = NseManagerDp_Lookup(MyStargate, 0, 1, 0, 3, search, 40, MyResponse);  
        if (nseStatus != Nse_OK)
        {
            printf("Error: fail to perform search\n");
            goto done;
        }

        fprintf(fp_res8, "Channel    Ready    Match     Priority     AD\n");
        memset(strBuff, 0, sizeof(strBuff));
        binary2str(MyResponse->adValues[3], strBuff, 4);
        fprintf(fp_res8, "%7d    %5d    %5d     %d    %s\n\n", 0, MyResponse->readies[3], MyResponse->matches[3], MyResponse->priorities[3], strBuff);
        //if(MyResponse->matches[0] == 0)
        //    printf("%7d    %5d    %5d     %d\n\n", 0, MyResponse->readies[0], MyResponse->matches[0], MyResponse->priorities[0]);
        //displayResults(MyResponse->readies, MyResponse->matches, MyResponse->priorities);
    }
    printf("All Table-8 search done\n");
	
    time(&t);
    printf("All search at %s\n", ctime(&t));
#endif

done:
    if (MyStargate) NseManagerDp_Destroy(MyStargate);
    if (MyTableBuff) NseTable_Destroy(MyTableBuff);
    if (MyRecordBuff) NseTableRecord_Destroy(MyRecordBuff);
    if (MyProfileBuff) NseProfile_Destroy(MyProfileBuff);
    if (MyTableInfo) NseProfileTableInfo_Destroy(MyTableInfo);
    if (MyKguSegment) NseProfileKguSegment_Destroy(MyKguSegment);
    if (MyResponse) NseLookupResponse_Destroy(MyResponse);

    return nseStatus;
}


void str2binary(char* str, int width)
{
    int i, j;
    for (i=0; i<width/8; i++)
    {
        char byte = 0;
        for (j=0; j<8; j++)
        {
            int idx = 8*i + j;
            char value = (idx < width) 
                            ? str[idx] - '0' 
                            : 0;
            byte = byte<<1 | value;
        }
        str[i] = byte;
    }
    str[i] = '\0';
}

void binary2str(char* in, char* out, int width)
{
    int i, j;
    char buff[1024] = "";
    memcpy(buff, in, width);
    for (i=0; i<width; i++)
    {
        for (j=7; j>=0; j--)
        {
            out[8*i+j] = '0' + ((int)buff[i]&0x01);
            buff[i] = buff[i]>>1;
        }
        out[width*8] = 0;
    }
}

void displayresponse(NseLookupResponse* response)
{
    int i;
    char strBuff[300];

    printf("Channel\tReady\tMatch\tPriority\tAD\n");
    for(i = 0; i < 4; i++)
    {
        memset(strBuff, 0, sizeof(strBuff));
        binary2str(response->adValues[i], strBuff, 4);
        printf("%7d\t%5d\t%5d\t%8d\t%s\n", i, response->readies[i], response->matches[i], response->priorities[i], strBuff);

    }
}

