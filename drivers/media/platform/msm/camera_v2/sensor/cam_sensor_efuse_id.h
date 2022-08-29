
#ifndef _CAM_SENSOR_EFUSE_ID_H_
#define _CAM_SENSOR_EFUSE_ID_H_

#include "msm_sensor.h"
#include "msm_sd.h"
#include "camera.h"
#include "msm_cci.h"
#include "msm_camera_io_util.h"
#include "msm_camera_i2c_mux.h"
#include <linux/regulator/rpm-smd-regulator.h>
#include <linux/regulator/consumer.h>




#ifdef USE_MY_DBG
#define CAM_DBG(x, fmt, args...)   pr_info(FILE_NAME"[%s](%d) " fmt ,  __FUNCTION__,__LINE__, ##args)
#else
#define CAM_DBG(fmt, args...)   do { } while (0)
#endif
 
#ifdef USE_MY_INFO
#define CAM_INFO(x, fmt, args...)   pr_info(FILE_NAME"[%s](%d) " fmt ,  __FUNCTION__,__LINE__, ##args)
#else
#define CAM_INFO(fmt, args...)   do { } while (0)
#endif

#ifdef USE_MY_ERR
#define CAM_ERR(x, fmt, args...)   pr_err(FILE_NAME"[%s](%d) " fmt ,  __FUNCTION__,__LINE__, ##args)
#else
#define CAM_ERR(fmt, args...)   do { } while (0)
#endif

/*                                       otp_sid    sid
#define SENSER_ID_ov16b10  0x1642        0xA0       0x20
#define SENSER_ID_s5k3p9   0x3109        0xA0       0x20
#define SENSER_ID_s5k5e9   0x559B        0x5A       0x5A
#define SENSER_ID_ov8856   0x885A        0xa2       0x6c
 */

#define EEPROM_SID_s5k3p9   0xA0

 
#define SENSER_ID_s5k3p9            0x3109  // front
#define SENSER_ID_ofilm_s5k3p9      0x3209 // front  ofilm_s5k3p9 
#define SENSER_ID_truly_s5k3p9      0x3309 // front  truly_s5k3p9 
#define SENSER_ID_ov16b10  			0x1642  // back
#define SENSER_ID_ov8856   			0x885A  // backaux
#define SENSER_ID_s5k5e9   			0x559B  // frontaux

#define SENSER_ID_s5kgm1sp          0x08D1 // htc  back  48
#define SENSER_ID_s5k2x5sp          0x2185 // htc  front 25
#define SENSER_ID_gc2375h           0x2375 // htc  back aux(ff) aux3(af) 2m
#define SENSER_ID_ov2180            0x2680 // htc  back aux(ff)


/*********      imx        ********/
#define SENSER_ID_imx350   0x02
#define SENSER_ID_imx486   0x486


/*********      ov        ********/
#define SENSER_ID_ov2281   0x56     
#define SENSER_ID_ov12a10  0x1241   

/*********      s5k        ********/
#define SENSER_ID_s5k2x7   0x2187

/* 
int32_t msm_camera_cci_i2c_write(struct msm_camera_i2c_client *client,
	uint32_t addr, uint16_t data,
	enum msm_camera_i2c_data_type data_type);
    
int32_t msm_camera_cci_i2c_read(struct msm_camera_i2c_client *client,
	uint32_t addr, uint16_t *data,
	enum msm_camera_i2c_data_type data_type);
 */
#define  WRITE_OTP_ADDR(reg_addr, reg_data)\
    msm_camera_cci_i2c_write(s_ctrl->sensor_i2c_client, \
    reg_addr, reg_data, MSM_CAMERA_I2C_BYTE_DATA);


    
    

#define  READ_OTP_ADDR(reg_addr, pu32_buf)\
    msm_camera_cci_i2c_read(s_ctrl->sensor_i2c_client, reg_addr, pu32_buf, \
    MSM_CAMERA_I2C_WORD_DATA);


    
    
#define  READ_EFUSE_ID(head_addr, cnt)\
for(i=0; i<cnt; i++)\
{\
    READ_OTP_ADDR(head_addr + i, (uint16_t *)&hex_efuseid)\
    sprintf(str_efuseid+2*i,"%02x", hex_efuseid);\
}



int32_t get_CameName_and_EfuseId(struct msm_sensor_ctrl_t *s_ctrl);

int8_t get_s5k3p9_supplier(struct msm_sensor_ctrl_t *s_ctrl);
#endif


