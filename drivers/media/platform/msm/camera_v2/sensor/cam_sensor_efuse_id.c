
#include <linux/module.h>
//#include <cam_sensor_cmn_header.h>
//#include "cam_sensor_core.h"
//#include "cam_sensor_util.h"
//#include "cam_soc_util.h"
//#include "cam_trace.h"


#define USE_MY_DBG
#define USE_MY_INFO
#define USE_MY_ERR

#undef FILE_NAME
#undef CAM_DBG
#undef CAM_INFO
#undef CAM_ERR

#include "cam_sensor_efuse_id.h"
#define  FILE_NAME  "cam_sensor_efuse_id.c"



/*  
extern char front_cam_name[64];// s5k3p9
extern char frontaux_cam_name[64];//  s5k5e9
extern char back_cam_name[64];//  ov16e10
extern char backaux_cam_name[64];// ov8856
extern char backaux2_cam_name[64];// ov8856


extern char front_cam_efuse_id[17];// s5k3p9
extern char frontaux_cam_efuse_id[17];// s5k5e9
extern char back_cam_efuse_id[33];//  ov16e10
extern char backaux_cam_efuse_id[33];// ov8856
extern char backaux2_cam_efuse_id[33];// ov8856
 */

extern char back_cam_name[64];//  s5kgm1sp
extern char front_cam_name[64];// s5k2x5sp
extern char backaux_cam_name[64];// 2m ff gc2375h
extern char backaux2_cam_name[64];// ov8856
extern char backaux3_cam_name[64];// 2m af gc2375h


extern char back_cam_efuse_id[64];//  s5kgm1sp
extern char front_cam_efuse_id[64];// s5k2x5sp
extern char backaux_cam_efuse_id[64];// 2m ff gc2375h
extern char backaux2_cam_efuse_id[64];// ov8856
extern char backaux3_cam_efuse_id[64];// 2m af gc2375h



static int8_t get_efuse_id_s5k3p9_ByEeprom(struct msm_sensor_ctrl_t *s_ctrl, char * pstr_efuse_id) 
{
    uint16_t dummy_sid = 0;
    uint8_t i = 0;
    char hex_efuseid;
    char str_efuseid[17]={0};
    
    uint32_t head_addr = 0x0015;
    uint8_t efuse_id_size = 6;
    
    dummy_sid = s_ctrl->sensor_i2c_client->cci_client->sid;
    s_ctrl->sensor_i2c_client->cci_client->sid = EEPROM_SID_s5k3p9 >> 1;

    READ_EFUSE_ID(head_addr, efuse_id_size)
    strcpy(pstr_efuse_id, str_efuseid);
    CAM_INFO(CAM_INFO, "pstr_efuse_id=%s strlen=%lu", pstr_efuse_id, strlen(pstr_efuse_id));
    
    s_ctrl->sensor_i2c_client->cci_client->sid = dummy_sid;
    
    return 0;
}

static int8_t get_efuse_id_s5k(struct msm_sensor_ctrl_t *s_ctrl, uint32_t SensorId, char * pstr_efuse_id) 
{

    char hex_efuseid;
    char str_efuseid[17]={0};
    uint8_t i;
    
    uint32_t page_number = 0;
    uint32_t head_addr = 0;
    uint8_t efuse_id_size = 0;
    
    // 8B use for 5E9
    if(SensorId == SENSER_ID_s5k5e9)
    {
        head_addr = 0x0A04;
        efuse_id_size = 8;
    }
    // 6B use for 3p9 2x7 
    else if(SensorId == SENSER_ID_s5k2x7)
    {
        head_addr = 0x0A24;
        efuse_id_size = 6;
    }
    else if(SensorId == SENSER_ID_ofilm_s5k3p9)
    {
        get_efuse_id_s5k3p9_ByEeprom(s_ctrl, pstr_efuse_id);

        return 0;
    }
    else if(SensorId == SENSER_ID_truly_s5k3p9)
    {
        //get_efuse_id_truly_s5k3p9_ByEeprom(s_ctrl, pstr_efuse_id);

        return 0;
    }
    else if(SensorId == SENSER_ID_s5kgm1sp)
    {
/*         head_addr = 0x0A04;// error
        efuse_id_size = 8; */
        head_addr = 0x0A24;
        efuse_id_size = 6;
        
    }
    else
    {
        CAM_INFO(CAM_INFO, "sensor id error   0x%x", SensorId);
        return -1;
    }

    WRITE_OTP_ADDR(0x0100, 0x01)// Streaming ON
    msleep(50);
    WRITE_OTP_ADDR(0x0A02, page_number)  // Write OTP Page
    

    WRITE_OTP_ADDR(0x0A00, 0x01)//  Read CMD
    msleep(1);

    READ_EFUSE_ID(head_addr, efuse_id_size)
    
    strcpy(pstr_efuse_id, str_efuseid);
    CAM_INFO(CAM_INFO," pstr_efuse_id=%s  strlen=%lu", pstr_efuse_id, strlen(pstr_efuse_id));
    
    WRITE_OTP_ADDR(0x0A00, 0x04)// Clear error bits
    WRITE_OTP_ADDR(0x0A00, 0x00)// Initial command

    CAM_DBG(CAM_INFO," return 0 ");
    return 0;
}



static int32_t get_efuse_id_ov(struct msm_sensor_ctrl_t *s_ctrl, char * pstr_efuse_id) 
{

    char hex_efuseid;
    char str_efuseid[33]={0};

    uint8_t i;
    

    
    WRITE_OTP_ADDR(0x0100, 0x01)
    WRITE_OTP_ADDR(0x3D84, 0x40)  
/*     WRITE_OTP_ADDR(0x3D88, 0x7000)
    WRITE_OTP_ADDR(0x3D8A, 0x700F) */
    WRITE_OTP_ADDR(0x3D88, 0x00)
    WRITE_OTP_ADDR(0x3D89, 0x70)
    WRITE_OTP_ADDR(0x3D8A, 0x0F)
    WRITE_OTP_ADDR(0x3D8B, 0x70)
    
    WRITE_OTP_ADDR(0x0100, 0x01)

    // Read Chip ID
    for(i=0; i<16; i++)
    {
        READ_OTP_ADDR(0x7000 + i, (uint16_t *)&hex_efuseid)
        //CAM_DBG(CAM_INFO, "i=%d   hex_efuseid=0x%x ", i, hex_efuseid);
        sprintf(str_efuseid+2*i,"%02x", hex_efuseid);
		msleep(1);
    }
    //CAM_DBG(CAM_INFO, "str_efuseid=%s", str_efuseid);

    strcpy(pstr_efuse_id, str_efuseid); 
    CAM_INFO(CAM_INFO,"pstr_efuse_id=%s strlen=%lu", pstr_efuse_id, strlen(pstr_efuse_id));
    
    return 0;
}

#if 0 // use it  if you need
static int8_t get_efuse_id_imx(struct msm_sensor_ctrl_t *s_ctrl, uint32_t SensorId, char * pstr_efuse_id) 
{

    char hex_efuseid;
    char str_efuseid[23]={0};

    uint8_t i;
    
    uint32_t page_number=0;
    uint32_t head_addr=0;
    
    struct cam_sensor_i2c_reg_setting  i2c_reg_settings;
    struct cam_sensor_i2c_reg_array    i2c_reg_array;
    
    if(SensorId == SENSER_ID_imx350)
    {
        page_number=39;
        head_addr=0x0A20;
    }
    else if(SensorId == SENSER_ID_imx486)
    {
        page_number=11;
        head_addr=0x0A27;
    }
    else
    {
        CAM_INFO(CAM_INFO, "sensor id error   0x%x", SensorId);
        return -1;
    }
    
    i2c_reg_settings.reg_setting = &i2c_reg_array;
    
    WRITE_OTP_ADDR(0x0136, 0x18)
    WRITE_OTP_ADDR(0x0137, 0x00)// Set up real INCK frequency EXCK_FREQ
    
    WRITE_OTP_ADDR(0x0A02, page_number)// Set the OTP page number
    WRITE_OTP_ADDR(0x0A00, 0x01)// Set up for OTP read transfer mode

/* Read OTP status register.
0d: = OTP access is in prog
1d: = OTP read has complet
5d: = OTP read has failed */
    READ_OTP_ADDR(0x0A01, (uint32_t *)&hex_efuseid)
    CAM_DBG(CAM_INFO, " Read OTP status=%d ", hex_efuseid);
    
    // Read Chip ID
    for(i=0; i<11; i++)
    {
        READ_OTP_ADDR(head_addr + i, (uint32_t *)&hex_efuseid)
        //CAM_DBG(CAM_INFO, "i=%d   hex_efuseid=0x%x ", i, hex_efuseid);
        sprintf(str_efuseid+2*i,"%02x", hex_efuseid);
		msleep(1);
    }
    //CAM_DBG(CAM_INFO, "str_efuseid=%s", str_efuseid);

    strcpy(pstr_efuse_id, str_efuseid); 
    CAM_INFO(CAM_INFO,"pstr_efuse_id=%s strlen=%lu", pstr_efuse_id, strlen(pstr_efuse_id));
    
    return 0;
}

#endif

int32_t get_CameName_and_EfuseId(struct msm_sensor_ctrl_t *s_ctrl)
{
    int32_t rc = 0;
    uint32_t sensor_id = 0;
    
    sensor_id = s_ctrl->sensordata->slave_info->sensor_id;

    switch(sensor_id){
        
        case SENSER_ID_ofilm_s5k3p9: {
            CAM_DBG(CAM_INFO," case SENSER_ID_ofilm_s5k3p9 ");
            strcpy(front_cam_name,"1_oof_s5k3p9");
            rc = get_efuse_id_s5k(s_ctrl, sensor_id, front_cam_efuse_id);
        }
        break;
        
        case SENSER_ID_truly_s5k3p9: {
            CAM_DBG(CAM_INFO," case SENSER_ID_truly_s5k3p9 ");
            strcpy(front_cam_name,"1_truly_s5k3p9");
            //rc = get_efuse_id_s5k(s_ctrl, sensor_id, front_cam_efuse_id);
        }
        break;
        
        case SENSER_ID_s5k5e9: {
            CAM_DBG(CAM_INFO," case SENSER_ID_s5k5e9 ");
            strcpy(backaux_cam_name,"2_qtech_s5k5e9");
            get_efuse_id_s5k(s_ctrl, sensor_id, backaux_cam_efuse_id);
        }
        break;
        
        case SENSER_ID_ov16b10: {
            CAM_DBG(CAM_INFO," case SENSER_ID_ov16b10 ");
            strcpy(back_cam_name,"0_qtech_ov16b10");
            get_efuse_id_ov(s_ctrl, back_cam_efuse_id);
        }
        break;
    
        case SENSER_ID_ov8856: {
            CAM_DBG(CAM_INFO," case SENSER_ID_ov8856 ");
            strcpy(backaux2_cam_name,"3_qtech_ov8856");
            get_efuse_id_ov(s_ctrl, backaux2_cam_efuse_id);
        }
        break;
    
        case SENSER_ID_s5kgm1sp: {
            CAM_DBG(CAM_INFO," case SENSER_ID_s5kgm1sp ");
            strcpy(back_cam_name,"0_ofilm_s5kgm1sp");
            get_efuse_id_s5k(s_ctrl, sensor_id, back_cam_efuse_id);
        }
        break;
        case SENSER_ID_s5k2x5sp: {
            CAM_DBG(CAM_INFO," case SENSER_ID_s5k2x5sp ");
            strcpy(front_cam_name,"1_qtech_s5k2x5sp");
            //get_efuse_id_s5k(s_ctrl, sensor_id, back_cam_efuse_id);
        }
        break;
        case SENSER_ID_ov2180: {
            CAM_DBG(CAM_INFO," case SENSER_ID_ov2180 ");
            strcpy(backaux_cam_name,"2_jsl_ov2180");
        }
        break;
        case SENSER_ID_gc2375h: {
            CAM_DBG(CAM_INFO," case SENSER_ID_gc2375h ");
            pr_info("[%s](%d)  yzm_id   sensor_id=0x%x  sensor_name=%s \n",
                __func__, __LINE__, 
                s_ctrl->sensordata->slave_info->sensor_id,
                s_ctrl->sensordata->sensor_name);
            
            if( 0 == strcmp(s_ctrl->sensordata->sensor_name, "gc2375h_ff") )
            {
                strcpy(backaux_cam_name,"2_ts_gc2375h_ff");
            //get_efuse_id_s5k(s_ctrl, sensor_id, back_cam_efuse_id);
            }
            else if(0 == strcmp(s_ctrl->sensordata->sensor_name, "gc2375h_af"))
            {
                strcpy(backaux3_cam_name,"4_ts_gc2375h_af");
            //get_efuse_id_s5k(s_ctrl, sensor_id, back_cam_efuse_id);
            }
            else
            {
                CAM_ERR(CAM_INFO," error  can not find the  sensor_name ");
            }

        }
        break;
        
        
        default:
        {
            CAM_ERR(CAM_INFO, "sensor id error   %d", sensor_id);
            rc = -1;
        }
    }

    return rc;
    
}


int8_t get_s5k3p9_supplier(struct msm_sensor_ctrl_t *s_ctrl) 
{
    uint16_t dummy_sid = 0;
    uint8_t  rc = 1;
    uint16_t eep_data = 0;
    
    dummy_sid = s_ctrl->sensor_i2c_client->cci_client->sid;
    s_ctrl->sensor_i2c_client->cci_client->sid = EEPROM_SID_s5k3p9 >> 1;

	msm_camera_cci_i2c_read(s_ctrl->sensor_i2c_client, 
	6, 
	&eep_data,
	MSM_CAMERA_I2C_WORD_DATA);
	CAM_INFO(CAM_INFO," yzm_did  eep_data=%d ",  eep_data);
	if(eep_data == 2)
	{
		msm_camera_cci_i2c_read(s_ctrl->sensor_i2c_client, 
		7, 
		&eep_data,
		MSM_CAMERA_I2C_WORD_DATA);
		CAM_INFO(CAM_INFO,"  yzm_did eep_data=%d ",  eep_data);
		if(eep_data == 0)
		{
			rc = 2;
		}
	}

    s_ctrl->sensor_i2c_client->cci_client->sid = dummy_sid;
    return rc;
}




