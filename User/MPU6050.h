#ifndef __MPU6050_h__
#define __MPU6050_h__

#define MPU6050_AD0D_SLAVER_ARRD   0x68					//MPU6050姿态传感器AD0下拉时的地址
#define MPU6050_AD0U_SLAVER_ARRD   0x69					//MPU6050姿态传感器AD0上拉时的地址
#include "main.h"

#define MPU6050_INT_Pin           GPIO_PIN_8       // 中断引脚
#define MPU6050_INT_GPIO_Port     GPIOB            // 中断引脚所在端口
#define MPU6050_AD0_Pin           GPIO_PIN_9       // 从机地址 控制引脚
#define MPU6050_AD0_GPIO_Port     GPIOB            // 从机地址 控制引脚所在端口

#define MPU6050_WHO_AM_I              0x75        // MPU6050设备ID寄存器地址
#define MPU6050_SMPLRT_DIV            0           // 采样率分频，默认8000Hz
#define MPU6050_DLPF_CFG              0           // 数字低通滤波配置
#define MPU6050_GYRO_OUT              0x43        // 陀螺仪数据寄存器地址
#define MPU6050_ACC_OUT               0x3B        // 加速度数据寄存器地址

#define MPU6050_ID                   0x68         // MPU6050默认设备ID

// 偏移量及校准寄存器
#define MPU6050_RA_XG_OFFS_TC        0x00        // X轴陀螺仪偏移校准寄存器
#define MPU6050_RA_YG_OFFS_TC        0x01        // Y轴陀螺仪偏移校准寄存器
#define MPU6050_RA_ZG_OFFS_TC        0x02        // Z轴陀螺仪偏移校准寄存器
#define MPU6050_RA_X_FINE_GAIN       0x03        // X轴微调增益
#define MPU6050_RA_Y_FINE_GAIN       0x04        // Y轴微调增益
#define MPU6050_RA_Z_FINE_GAIN       0x05        // Z轴微调增益
#define MPU6050_RA_XA_OFFS_H         0x06        // X轴加速度偏移高字节
#define MPU6050_RA_XA_OFFS_L_TC      0x07        // X轴加速度偏移低字节
#define MPU6050_RA_YA_OFFS_H         0x08        // Y轴加速度偏移高字节
#define MPU6050_RA_YA_OFFS_L_TC      0x09        // Y轴加速度偏移低字节
#define MPU6050_RA_ZA_OFFS_H         0x0A        // Z轴加速度偏移高字节
#define MPU6050_RA_ZA_OFFS_L_TC      0x0B        // Z轴加速度偏移低字节

#define MPU6050_RA_XG_OFFS_USRH      0x13        // 用户X轴陀螺仪偏移高字节
#define MPU6050_RA_XG_OFFS_USRL      0x14        // 用户X轴陀螺仪偏移低字节
#define MPU6050_RA_YG_OFFS_USRH      0x15        // 用户Y轴陀螺仪偏移高字节
#define MPU6050_RA_YG_OFFS_USRL      0x16        // 用户Y轴陀螺仪偏移低字节
#define MPU6050_RA_ZG_OFFS_USRH      0x17        // 用户Z轴陀螺仪偏移高字节
#define MPU6050_RA_ZG_OFFS_USRL      0x18        // 用户Z轴陀螺仪偏移低字节

#define MPU6050_RA_SMPLRT_DIV        0x19        // 采样率分频寄存器
#define MPU6050_RA_CONFIG            0x1A        // 配置寄存器
#define MPU6050_RA_GYRO_CONFIG       0x1B        // 陀螺仪配置寄存器
#define MPU6050_RA_ACCEL_CONFIG      0x1C        // 加速度计配置寄存器
#define MPU6050_RA_FF_THR            0x1D        // 自由跌落阈值寄存器
#define MPU6050_RA_FF_DUR            0x1E        // 自由跌落持续时间寄存器
#define MPU6050_RA_MOT_THR           0x1F        // 运动检测阈值寄存器
#define MPU6050_RA_MOT_DUR           0x20        // 运动检测持续时间寄存器
#define MPU6050_RA_ZRMOT_THR         0x21        // 零运动阈值寄存器
#define MPU6050_RA_ZRMOT_DUR         0x22        // 零运动持续时间寄存器

#define MPU6050_RA_FIFO_EN           0x23        // FIFO使能寄存器
#define MPU6050_RA_I2C_MST_CTRL      0x24        // I2C主机控制寄存器
#define MPU6050_RA_I2C_SLV0_ADDR     0x25        // I2C从机0地址寄存器
#define MPU6050_RA_I2C_SLV0_REG      0x26        // I2C从机0寄存器寄存器
#define MPU6050_RA_I2C_SLV0_CTRL     0x27        // I2C从机0控制寄存器
#define MPU6050_RA_I2C_SLV1_ADDR     0x28        // I2C从机1地址寄存器
#define MPU6050_RA_I2C_SLV1_REG      0x29        // I2C从机1寄存器寄存器
#define MPU6050_RA_I2C_SLV1_CTRL     0x2A        // I2C从机1控制寄存器
#define MPU6050_RA_I2C_SLV2_ADDR     0x2B        // I2C从机2地址寄存器
#define MPU6050_RA_I2C_SLV2_REG      0x2C        // I2C从机2寄存器寄存器
#define MPU6050_RA_I2C_SLV2_CTRL     0x2D        // I2C从机2控制寄存器
#define MPU6050_RA_I2C_SLV3_ADDR     0x2E        // I2C从机3地址寄存器
#define MPU6050_RA_I2C_SLV3_REG      0x2F        // I2C从机3寄存器寄存器
#define MPU6050_RA_I2C_SLV3_CTRL     0x30        // I2C从机3控制寄存器
#define MPU6050_RA_I2C_SLV4_ADDR     0x31        // I2C从机4地址寄存器
#define MPU6050_RA_I2C_SLV4_REG      0x32        // I2C从机4寄存器寄存器
#define MPU6050_RA_I2C_SLV4_DO       0x33        // I2C从机4数据寄存器
#define MPU6050_RA_I2C_SLV4_CTRL     0x34        // I2C从机4控制寄存器
#define MPU6050_RA_I2C_SLV4_DI       0x35        // I2C从机4数据输入寄存器

#define MPU6050_RA_I2C_MST_STATUS    0x36        // I2C主机状态寄存器
#define MPU6050_RA_INT_PIN_CFG       0x37        // 中断引脚配置寄存器
#define MPU6050_RA_INT_ENABLE        0x38        // 中断使能寄存器
#define MPU6050_RA_DMP_INT_STATUS    0x39        // DMP中断状态寄存器
#define MPU6050_RA_INT_STATUS        0x3A        // 中断状态寄存器

#define MPU6050_RA_ACCEL_XOUT_H      0x3B        // 加速度X轴高字节寄存器
#define MPU6050_RA_ACCEL_XOUT_L      0x3C        // 加速度X轴低字节寄存器
#define MPU6050_RA_ACCEL_YOUT_H      0x3D        // 加速度Y轴高字节寄存器
#define MPU6050_RA_ACCEL_YOUT_L      0x3E        // 加速度Y轴低字节寄存器
#define MPU6050_RA_ACCEL_ZOUT_H      0x3F        // 加速度Z轴高字节寄存器
#define MPU6050_RA_ACCEL_ZOUT_L      0x40        // 加速度Z轴低字节寄存器

#define MPU6050_RA_TEMP_OUT_H        0x41        // 温度高字节寄存器
#define MPU6050_RA_TEMP_OUT_L        0x42        // 温度低字节寄存器

#define MPU6050_RA_GYRO_XOUT_H       0x43        // 陀螺仪X轴高字节寄存器
#define MPU6050_RA_GYRO_XOUT_L       0x44        // 陀螺仪X轴低字节寄存器
#define MPU6050_RA_GYRO_YOUT_H       0x45        // 陀螺仪Y轴高字节寄存器
#define MPU6050_RA_GYRO_YOUT_L       0x46        // 陀螺仪Y轴低字节寄存器
#define MPU6050_RA_GYRO_ZOUT_H       0x47        // 陀螺仪Z轴高字节寄存器
#define MPU6050_RA_GYRO_ZOUT_L       0x48        // 陀螺仪Z轴低字节寄存器

#define MPU6050_RA_EXT_SENS_DATA_00  0x49        // 外部传感器数据寄存器0
#define MPU6050_RA_EXT_SENS_DATA_01  0x4A
#define MPU6050_RA_EXT_SENS_DATA_02  0x4B
#define MPU6050_RA_EXT_SENS_DATA_03  0x4C
#define MPU6050_RA_EXT_SENS_DATA_04  0x4D
#define MPU6050_RA_EXT_SENS_DATA_05  0x4E
#define MPU6050_RA_EXT_SENS_DATA_06  0x4F
#define MPU6050_RA_EXT_SENS_DATA_07  0x50
#define MPU6050_RA_EXT_SENS_DATA_08  0x51
#define MPU6050_RA_EXT_SENS_DATA_09  0x52
#define MPU6050_RA_EXT_SENS_DATA_10  0x53
#define MPU6050_RA_EXT_SENS_DATA_11  0x54
#define MPU6050_RA_EXT_SENS_DATA_12  0x55
#define MPU6050_RA_EXT_SENS_DATA_13  0x56
#define MPU6050_RA_EXT_SENS_DATA_14  0x57
#define MPU6050_RA_EXT_SENS_DATA_15  0x58
#define MPU6050_RA_EXT_SENS_DATA_16  0x59
#define MPU6050_RA_EXT_SENS_DATA_17  0x5A
#define MPU6050_RA_EXT_SENS_DATA_18  0x5B
#define MPU6050_RA_EXT_SENS_DATA_19  0x5C
#define MPU6050_RA_EXT_SENS_DATA_20  0x5D
#define MPU6050_RA_EXT_SENS_DATA_21  0x5E
#define MPU6050_RA_EXT_SENS_DATA_22  0x5F
#define MPU6050_RA_EXT_SENS_DATA_23  0x60

#define MPU6050_RA_MOT_DETECT_STATUS 0x61        // 运动检测状态寄存器

#define MPU6050_RA_I2C_SLV0_DO       0x63        // I2C从机0数据寄存器
#define MPU6050_RA_I2C_SLV1_DO       0x64        // I2C从机1数据寄存器
#define MPU6050_RA_I2C_SLV2_DO       0x65        // I2C从机2数据寄存器
#define MPU6050_RA_I2C_SLV3_DO       0x66        // I2C从机3数据寄存器

#define MPU6050_RA_I2C_MST_DELAY_CTRL 0x67       // I2C主机延时控制寄存器
#define MPU6050_RA_SIGNAL_PATH_RESET  0x68       // 信号路径重置寄存器
#define MPU6050_RA_MOT_DETECT_CTRL    0x69       // 运动检测控制寄存器
#define MPU6050_RA_USER_CTRL          0x6A       // 用户控制寄存器
#define MPU6050_RA_PWR_MGMT_1         0x6B       // 电源管理1寄存器
#define MPU6050_RA_PWR_MGMT_2         0x6C       // 电源管理2寄存器
#define MPU6050_RA_BANK_SEL           0x6D       // 内存银行选择寄存器
#define MPU6050_RA_MEM_START_ADDR     0x6E       // 内存起始地址寄存器
#define MPU6050_RA_MEM_R_W            0x6F       // 内存读写寄存器
#define MPU6050_RA_DMP_CFG_1          0x70       // DMP配置1寄存器
#define MPU6050_RA_DMP_CFG_2          0x71       // DMP配置2寄存器
#define MPU6050_RA_FIFO_COUNTH        0x72       // FIFO计数高字节
#define MPU6050_RA_FIFO_COUNTL        0x73       // FIFO计数低字节
#define MPU6050_RA_FIFO_R_W           0x74       // FIFO读写寄存器
#define MPU6050_RA_WHO_AM_I           0x75       // 设备ID寄存器

// 位定义
#define MPU6050_TC_PWR_MODE_BIT       7           // 电源模式位
#define MPU6050_TC_OFFSET_BIT         6           // 偏移量起始位
#define MPU6050_TC_OFFSET_LENGTH      6           // 偏移量长度
#define MPU6050_TC_OTP_BNK_VLD_BIT    0           // OTP有效位

#define MPU6050_VDDIO_LEVEL_VLOGIC   0           // VLOGIC电平
#define MPU6050_VDDIO_LEVEL_VDD      1           // VDD电平

#define MPU6050_CFG_EXT_SYNC_SET_BIT       5       // 外部同步设置起始位
#define MPU6050_CFG_EXT_SYNC_SET_LENGTH    3       // 外部同步设置长度
#define MPU6050_CFG_DLPF_CFG_BIT            2       // DLPF配置起始位
#define MPU6050_CFG_DLPF_CFG_LENGTH         3       // DLPF配置长度

// 外部同步信号选择
#define MPU6050_EXT_SYNC_DISABLED          0x0       // 禁用外部同步
#define MPU6050_EXT_SYNC_TEMP_OUT_L        0x1       // 温度低字节同步
#define MPU6050_EXT_SYNC_GYRO_XOUT_L       0x2       // 陀螺仪X低字节同步
#define MPU6050_EXT_SYNC_GYRO_YOUT_L       0x3       // 陀螺仪Y低字节同步
#define MPU6050_EXT_SYNC_GYRO_ZOUT_L       0x4       // 陀螺仪Z低字节同步
#define MPU6050_EXT_SYNC_ACCEL_XOUT_L      0x5       // 加速度X低字节同步
#define MPU6050_EXT_SYNC_ACCEL_YOUT_L      0x6       // 加速度Y低字节同步
#define MPU6050_EXT_SYNC_ACCEL_ZOUT_L      0x7       // 加速度Z低字节同步

// 数字低通滤波器带宽配置
#define MPU6050_DLPF_BW_256         0x00        // 256Hz带宽
#define MPU6050_DLPF_BW_188         0x01        // 188Hz带宽
#define MPU6050_DLPF_BW_98          0x02        // 98Hz带宽
#define MPU6050_DLPF_BW_42          0x03        // 42Hz带宽
#define MPU6050_DLPF_BW_20          0x04        // 20Hz带宽
#define MPU6050_DLPF_BW_10          0x05        // 10Hz带宽
#define MPU6050_DLPF_BW_5           0x06        // 5Hz带宽

// 陀螺仪配置位
#define MPU6050_GCONFIG_FS_SEL_BIT       4           // 量程选择位
#define MPU6050_GCONFIG_FS_SEL_LENGTH    2           // 量程选择长度

// 陀螺仪量程配置
#define MPU6050_GYRO_FS_250          0x00        // ±250度/秒
#define MPU6050_GYRO_FS_500          0x01        // ±500度/秒
#define MPU6050_GYRO_FS_1000         0x02        // ±1000度/秒
#define MPU6050_GYRO_FS_2000         0x03        // ±2000度/秒

// 加速度计配置位
#define MPU6050_ACONFIG_XA_ST_BIT         7           // X轴自检启动位
#define MPU6050_ACONFIG_YA_ST_BIT         6           // Y轴自检启动位
#define MPU6050_ACONFIG_ZA_ST_BIT         5           // Z轴自检启动位
#define MPU6050_ACONFIG_AFS_SEL_BIT       4           // 加速度计量程选择起始位
#define MPU6050_ACONFIG_AFS_SEL_LENGTH    2           // 加速度计量程选择长度
#define MPU6050_ACONFIG_ACCEL_HPF_BIT     2           // 高通滤波起始位
#define MPU6050_ACONFIG_ACCEL_HPF_LENGTH  3           // 高通滤波长度

// 加速度计量程配置
#define MPU6050_ACCEL_FS_2          0x00        // ±2g
#define MPU6050_ACCEL_FS_4          0x01        // ±4g
#define MPU6050_ACCEL_FS_8          0x02        // ±8g
#define MPU6050_ACCEL_FS_16         0x03        // ±16g

// 数字高通滤波配置
#define MPU6050_DHPF_RESET          0x00        // 重置高通滤波
#define MPU6050_DHPF_5              0x01        // 5Hz高通滤波
#define MPU6050_DHPF_2P5            0x02        // 2.5Hz高通滤波
#define MPU6050_DHPF_1P25           0x03    // 高通滤波器截止频率 1.25 Hz
#define MPU6050_DHPF_0P63           0x04    // 高通滤波器截止频率 0.63 Hz
#define MPU6050_DHPF_HOLD           0x07    // 高通滤波器保持状态

// FIFO使能位定义
#define MPU6050_TEMP_FIFO_EN_BIT    7       // 温度传感器数据FIFO使能位
#define MPU6050_XG_FIFO_EN_BIT      6       // 陀螺仪X轴FIFO使能位
#define MPU6050_YG_FIFO_EN_BIT      5       // 陀螺仪Y轴FIFO使能位
#define MPU6050_ZG_FIFO_EN_BIT      4       // 陀螺仪Z轴FIFO使能位
#define MPU6050_ACCEL_FIFO_EN_BIT   3       // 加速度计FIFO使能位
#define MPU6050_SLV2_FIFO_EN_BIT    2       // 从设备2 FIFO使能位
#define MPU6050_SLV1_FIFO_EN_BIT    1       // 从设备1 FIFO使能位
#define MPU6050_SLV0_FIFO_EN_BIT    0       // 从设备0 FIFO使能位

// I2C主控配置位定义
#define MPU6050_MULT_MST_EN_BIT     7       // 多主机使能位
#define MPU6050_WAIT_FOR_ES_BIT     6       // 等待外部传感器使能位
#define MPU6050_SLV_3_FIFO_EN_BIT   5       // 从设备3 FIFO使能位
#define MPU6050_I2C_MST_P_NSR_BIT   4       // I2C主机读写模式位
#define MPU6050_I2C_MST_CLK_BIT     3       // I2C主机时钟位
#define MPU6050_I2C_MST_CLK_LENGTH  4       // I2C主机时钟长度

// 时钟分频定义（对应不同频率）
#define MPU6050_CLOCK_DIV_348       0x0
#define MPU6050_CLOCK_DIV_333       0x1
#define MPU6050_CLOCK_DIV_320       0x2
#define MPU6050_CLOCK_DIV_308       0x3
#define MPU6050_CLOCK_DIV_296       0x4
#define MPU6050_CLOCK_DIV_286       0x5
#define MPU6050_CLOCK_DIV_276       0x6
#define MPU6050_CLOCK_DIV_267       0x7
#define MPU6050_CLOCK_DIV_258       0x8
#define MPU6050_CLOCK_DIV_500       0x9
#define MPU6050_CLOCK_DIV_471       0xA
#define MPU6050_CLOCK_DIV_444       0xB
#define MPU6050_CLOCK_DIV_421       0xC
#define MPU6050_CLOCK_DIV_400       0xD
#define MPU6050_CLOCK_DIV_381       0xE
#define MPU6050_CLOCK_DIV_364       0xF

// I2C 从设备寄存器相关位定义
#define MPU6050_I2C_SLV_RW_BIT      7       // 读写位
#define MPU6050_I2C_SLV_ADDR_BIT    6       // 地址起始位
#define MPU6050_I2C_SLV_ADDR_LENGTH 7       // 地址长度
#define MPU6050_I2C_SLV_EN_BIT      7       // 从设备使能位
#define MPU6050_I2C_SLV_BYTE_SW_BIT 6       // 字节顺序切换位
#define MPU6050_I2C_SLV_REG_DIS_BIT 5       // 寄存器禁用位
#define MPU6050_I2C_SLV_GRP_BIT     4       // 分组操作位
#define MPU6050_I2C_SLV_LEN_BIT     3       // 数据长度起始位
#define MPU6050_I2C_SLV_LEN_LENGTH  4       // 数据长度长度

// I2C 从设备4寄存器相关位定义
#define MPU6050_I2C_SLV4_RW_BIT         7   // 读写位
#define MPU6050_I2C_SLV4_ADDR_BIT       6   // 地址起始位
#define MPU6050_I2C_SLV4_ADDR_LENGTH    7   // 地址长度
#define MPU6050_I2C_SLV4_EN_BIT         7   // 使能位
#define MPU6050_I2C_SLV4_INT_EN_BIT     6   // 中断使能位
#define MPU6050_I2C_SLV4_REG_DIS_BIT    5   // 寄存器禁用位
#define MPU6050_I2C_SLV4_MST_DLY_BIT    4   // 主机延迟起始位
#define MPU6050_I2C_SLV4_MST_DLY_LENGTH 5   // 主机延迟长度

// 主机状态相关位定义
#define MPU6050_MST_PASS_THROUGH_BIT    7   // 直通模式使能位
#define MPU6050_MST_I2C_SLV4_DONE_BIT   6   // 从设备4完成位
#define MPU6050_MST_I2C_LOST_ARB_BIT    5   // I2C仲裁丢失位
#define MPU6050_MST_I2C_SLV4_NACK_BIT   4   // 从设备4无响应位
#define MPU6050_MST_I2C_SLV3_NACK_BIT   3   // 从设备3无响应位
#define MPU6050_MST_I2C_SLV2_NACK_BIT   2   // 从设备2无响应位
#define MPU6050_MST_I2C_SLV1_NACK_BIT   1   // 从设备1无响应位
#define MPU6050_MST_I2C_SLV0_NACK_BIT   0   // 从设备0无响应位

// 中断配置寄存器位定义
#define MPU6050_INTCFG_INT_LEVEL_BIT        7   // 中断电平配置位
#define MPU6050_INTCFG_INT_OPEN_BIT         6   // 中断开漏/推挽配置位
#define MPU6050_INTCFG_LATCH_INT_EN_BIT     5   // 中断保持使能位
#define MPU6050_INTCFG_INT_RD_CLEAR_BIT     4   // 中断读取清除位
#define MPU6050_INTCFG_FSYNC_INT_LEVEL_BIT  3   // FSYNC中断电平位
#define MPU6050_INTCFG_FSYNC_INT_EN_BIT     2   // FSYNC中断使能位
#define MPU6050_INTCFG_I2C_BYPASS_EN_BIT    1   // I2C旁路使能位
#define MPU6050_INTCFG_CLKOUT_EN_BIT        0   // 时钟输出使能位

// 中断电平模式
#define MPU6050_INTMODE_ACTIVEHIGH  0x00    // 中断高电平有效
#define MPU6050_INTMODE_ACTIVELOW   0x01    // 中断低电平有效

// 中断驱动方式
#define MPU6050_INTDRV_PUSHPULL     0x00    // 推挽输出
#define MPU6050_INTDRV_OPENDRAIN    0x01    // 开漏输出

// 中断保持模式
#define MPU6050_INTLATCH_50USPULSE  0x00    // 50微秒脉冲中断
#define MPU6050_INTLATCH_WAITCLEAR  0x01    // 等待中断清除

// 中断清除方式
#define MPU6050_INTCLEAR_STATUSREAD 0x00    // 读状态寄存器清除
#define MPU6050_INTCLEAR_ANYREAD    0x01    // 读任意寄存器清除

// 各种中断标志位
#define MPU6050_INTERRUPT_FF_BIT            7   // 自由落体中断位
#define MPU6050_INTERRUPT_MOT_BIT           6   // 运动检测中断位
#define MPU6050_INTERRUPT_ZMOT_BIT          5   // 零运动检测中断位
#define MPU6050_INTERRUPT_FIFO_OFLOW_BIT    4   // FIFO溢出中断位
#define MPU6050_INTERRUPT_I2C_MST_INT_BIT   3   // I2C主机中断位
#define MPU6050_INTERRUPT_PLL_RDY_INT_BIT   2   // PLL准备就绪中断位
#define MPU6050_INTERRUPT_DMP_INT_BIT       1   // DMP中断位
#define MPU6050_INTERRUPT_DATA_RDY_BIT      0   // 数据准备中断位

// DMP中断位
#define MPU6050_DMPINT_5_BIT            5
#define MPU6050_DMPINT_4_BIT            4
#define MPU6050_DMPINT_3_BIT            3
#define MPU6050_DMPINT_2_BIT            2
#define MPU6050_DMPINT_1_BIT            1
#define MPU6050_DMPINT_0_BIT            0

// 运动检测状态位
#define MPU6050_MOTION_MOT_XNEG_BIT     7   // X轴负方向运动检测位
#define MPU6050_MOTION_MOT_XPOS_BIT     6   // X轴正方向运动检测位
#define MPU6050_MOTION_MOT_YNEG_BIT     5   // Y轴负方向运动检测位
#define MPU6050_MOTION_MOT_YPOS_BIT     4   // Y轴正方向运动检测位
#define MPU6050_MOTION_MOT_ZNEG_BIT     3   // Z轴负方向运动检测位
#define MPU6050_MOTION_MOT_ZPOS_BIT     2   // Z轴正方向运动检测位
#define MPU6050_MOTION_MOT_ZRMOT_BIT    0   // 零运动检测位

// 延时控制寄存器位定义
#define MPU6050_DELAYCTRL_DELAY_ES_SHADOW_BIT   7   // 延迟ES影子寄存器位
#define MPU6050_DELAYCTRL_I2C_SLV4_DLY_EN_BIT   4   // 从设备4延迟使能位
#define MPU6050_DELAYCTRL_I2C_SLV3_DLY_EN_BIT   3   // 从设备3延迟使能位
#define MPU6050_DELAYCTRL_I2C_SLV2_DLY_EN_BIT   2   // 从设备2延迟使能位
#define MPU6050_DELAYCTRL_I2C_SLV1_DLY_EN_BIT   1   // 从设备1延迟使能位
#define MPU6050_DELAYCTRL_I2C_SLV0_DLY_EN_BIT   0   // 从设备0延迟使能位

// 传感器路径重置位
#define MPU6050_PATHRESET_GYRO_RESET_BIT    2   // 陀螺仪路径重置位
#define MPU6050_PATHRESET_ACCEL_RESET_BIT   1   // 加速度路径重置位
#define MPU6050_PATHRESET_TEMP_RESET_BIT    0   // 温度传感器路径重置位

// 动作检测相关寄存器位及长度
#define MPU6050_DETECT_ACCEL_ON_DELAY_BIT       5   // 加速度动作延迟起始位
#define MPU6050_DETECT_ACCEL_ON_DELAY_LENGTH    2   // 加速度动作延迟长度
#define MPU6050_DETECT_FF_COUNT_BIT             3   // 自由落体计数起始位
#define MPU6050_DETECT_FF_COUNT_LENGTH          2   // 自由落体计数长度
#define MPU6050_DETECT_MOT_COUNT_BIT            1   // 运动检测计数起始位
#define MPU6050_DETECT_MOT_COUNT_LENGTH         2   // 运动检测计数长度

// 动作检测递减模式
#define MPU6050_DETECT_DECREMENT_RESET  0x0     // 递减重置
#define MPU6050_DETECT_DECREMENT_1      0x1     // 递减1
#define MPU6050_DETECT_DECREMENT_2      0x2     // 递减2
#define MPU6050_DETECT_DECREMENT_4      0x3     // 递减4

// 用户控制寄存器位定义
#define MPU6050_USERCTRL_DMP_EN_BIT             7   // DMP使能位
#define MPU6050_USERCTRL_FIFO_EN_BIT            6   // FIFO使能位
#define MPU6050_USERCTRL_I2C_MST_EN_BIT         5   // I2C主控使能位
#define MPU6050_USERCTRL_I2C_IF_DIS_BIT         4   // 禁用I2C接口位
#define MPU6050_USERCTRL_DMP_RESET_BIT          3   // DMP复位位
#define MPU6050_USERCTRL_FIFO_RESET_BIT         2   // FIFO复位位
#define MPU6050_USERCTRL_I2C_MST_RESET_BIT      1   // I2C主控复位位
#define MPU6050_USERCTRL_SIG_COND_RESET_BIT     0   // 信号条件复位位

// 电源管理1寄存器位定义
#define MPU6050_PWR1_DEVICE_RESET_BIT   7       // 设备复位位
#define MPU6050_PWR1_SLEEP_BIT          6       // 休眠模式位
#define MPU6050_PWR1_CYCLE_BIT          5       // 循环模式位
#define MPU6050_PWR1_TEMP_DIS_BIT       3       // 温度传感器关闭位
#define MPU6050_PWR1_CLKSEL_BIT         2       // 时钟选择起始位
#define MPU6050_PWR1_CLKSEL_LENGTH      3       // 时钟选择长度

// 时钟源定义
#define MPU6050_CLOCK_INTERNAL          0x00    // 内部时钟
#define MPU6050_CLOCK_PLL_XGYRO         0x01    // 陀螺仪X轴PLL时钟
#define MPU6050_CLOCK_PLL_YGYRO         0x02    // 陀螺仪Y轴PLL时钟
#define MPU6050_CLOCK_PLL_ZGYRO         0x03    // 陀螺仪Z轴PLL时钟
#define MPU6050_CLOCK_PLL_EXT32K        0x04    // 外部32kHz时钟
#define MPU6050_CLOCK_PLL_EXT19M        0x05    // 外部19.2MHz时钟
#define MPU6050_CLOCK_KEEP_RESET        0x07    // 保持复位状态

// 电源管理2寄存器位定义
#define MPU6050_PWR2_LP_WAKE_CTRL_BIT       7   // 低功耗唤醒控制起始位
#define MPU6050_PWR2_LP_WAKE_CTRL_LENGTH    2   // 低功耗唤醒控制长度
#define MPU6050_PWR2_STBY_XA_BIT            5   // 加速度计X轴待机位
#define MPU6050_PWR2_STBY_YA_BIT            4   // 加速度计Y轴待机位
#define MPU6050_PWR2_STBY_ZA_BIT            3   // 加速度计Z轴待机位
#define MPU6050_PWR2_STBY_XG_BIT            2   // 陀螺仪X轴待机位
#define MPU6050_PWR2_STBY_YG_BIT            1   // 陀螺仪Y轴待机位
#define MPU6050_PWR2_STBY_ZG_BIT            0   // 陀螺仪Z轴待机位

// 唤醒频率定义
#define MPU6050_WAKE_FREQ_1P25      0x0     // 唤醒频率 1.25 Hz
#define MPU6050_WAKE_FREQ_2P5       0x1     // 唤醒频率 2.5 Hz
#define MPU6050_WAKE_FREQ_5         0x2     // 唤醒频率 5 Hz
#define MPU6050_WAKE_FREQ_10        0x3     // 唤醒频率 10 Hz

// 存储器银行选择寄存器位定义
#define MPU6050_BANKSEL_PRFTCH_EN_BIT       6   // 预取使能位
#define MPU6050_BANKSEL_CFG_USER_BANK_BIT   5   // 用户银行配置位
#define MPU6050_BANKSEL_MEM_SEL_BIT         4   // 存储器选择起始位
#define MPU6050_BANKSEL_MEM_SEL_LENGTH      5   // 存储器选择长度

// WHO_AM_I寄存器位定义
#define MPU6050_WHO_AM_I_BIT        6       // ID起始位
#define MPU6050_WHO_AM_I_LENGTH     6       // ID长度

// DMP内存相关定义
#define MPU6050_DMP_MEMORY_BANKS        8   // DMP内存银行数量
#define MPU6050_DMP_MEMORY_BANK_SIZE    256 // DMP内存银行大小（字节）
#define MPU6050_DMP_MEMORY_CHUNK_SIZE   16  // DMP内存块大小（字节）


void MPU6050_WriteReg(uint8_t Address,uint8_t Data);
uint8_t MPU6050_ReadByte(uint8_t Address);
HAL_StatusTypeDef MPU6050_ReadBuffer(uint8_t Address,uint8_t *Buffer,uint8_t Size);
uint8_t MPU6050_GetID(void);
void MPU6050_WriteBuffer(uint8_t Address,uint8_t *Buffer,uint8_t Size);
HAL_StatusTypeDef MPU6050_Init(void);
HAL_StatusTypeDef MPU6050_ReadAcc(int16_t *Acc);
void MPU6050_GiveI2cMutex(void);
void MPU6050_GetI2cMutex(void);
void MPU6050_Task(void *Params);
#endif
