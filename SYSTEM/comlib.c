#include "stmlib.h"


#define CR1_SPE_Set          ((u16)0x0040)
#define CR1_SPE_Reset        ((u16)0xFFBF)

/* SPI CRCNext mask */
#define CR1_CRCNext_Set      ((u16)0x1000)

/* SPI CRCEN mask */
#define CR1_CRCEN_Set        ((u16)0x2000)
#define CR1_CRCEN_Reset      ((u16)0xDFFF)

/* SPI SSOE mask */
#define CR2_SSOE_Set        ((u16)0x0004)
#define CR2_SSOE_Reset      ((u16)0xFFFB)

/* SPI registers Masks */
#define CR1_CLEAR_Mask1       ((u16)0x3040)




#define PWR_OFFSET               (PWR_BASE - PERIPH_BASE)

/* --- CR Register ---*/
/* Alias word address of DBP bit */
#define CR_OFFSET                (PWR_OFFSET + 0x00)
#define DBP_BitNumber            0x08
#define CR_DBP_BB                (PERIPH_BB_BASE + (CR_OFFSET * 32) + (DBP_BitNumber * 4))

/* Alias word address of PVDE bit */
#define PVDE_BitNumber           0x04
#define CR_PVDE_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PVDE_BitNumber * 4))

/* --- CSR Register ---*/
/* Alias word address of EWUP bit */
#define CSR_OFFSET               (PWR_OFFSET + 0x04)
#define EWUP_BitNumber           0x08
#define CSR_EWUP_BB              (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (EWUP_BitNumber * 4))

/* ------------------ PWR registers bit mask ------------------------ */
/* CR register bit mask */
#define CR_PDDS_Set              ((u32)0x00000002)
#define CR_DS_Mask               ((u32)0xFFFFFFFC)
#define CR_CWUF_Set              ((u32)0x00000004)
#define CR_PLS_Mask              ((u32)0xFFFFFF1F)

/* --------- Cortex System Control register bit mask ---------------- */
/* Cortex System Control register address */
#define SCB_SysCtrl              ((u32)0xE000ED10)
/* SLEEPDEEP bit mask */
#define SysCtrl_SLEEPDEEP_Set    ((u32)0x00000004)

//void PWR_BackupAccessCmd(FunctionalState NewState)
//{
//  /* Check the parameters */
//  assert(IS_FUNCTIONAL_STATE(NewState));

//  *(vu32 *) CR_DBP_BB = (u32)NewState;
//}

#define CR1_CEN_Set                 ((u16)0x0001)
#define CR1_CEN_Reset               ((u16)0x03FE)
#define CR1_UDIS_Set                ((u16)0x0002)
#define CR1_UDIS_Reset              ((u16)0x03FD)
#define CR1_URS_Set                 ((u16)0x0004)
#define CR1_URS_Reset               ((u16)0x03FB)
#define CR1_OPM_Mask                ((u16)0x03F7)
#define CR1_CounterMode_Mask        ((u16)0x039F)
#define CR1_ARPE_Set                ((u16)0x0080)
#define CR1_ARPE_Reset              ((u16)0x037F)
#define CR1_CKD_Mask                ((u16)0x00FF)

#define CR2_CCDS_Set                ((u16)0x0008)
#define CR2_CCDS_Reset              ((u16)0x0007)
#define CR2_MMS_Mask                ((u16)0x0080)
#define CR2_TI1S_Set                ((u16)0x0080)
#define CR2_TI1S_Reset              ((u16)0xFF70)

#define SMCR_SMS_Mask               ((u16)0xFFF0)
#define SMCR_ETR_Mask               ((u16)0x00F7)
#define SMCR_TS_Mask                ((u16)0xFF87)
#define SMCR_MSM_Mask               ((u16)0xFF77)
#define SMCR_ECE_Set                ((u16)0x4000)

#define CCMR_CC13S_Mask             ((u16)0x7F7C)
#define CCMR_CC24S_Mask             ((u16)0x7C7F)
#define CCMR_TI13Direct_Set         ((u16)0x0001)
#define CCMR_TI24Direct_Set         ((u16)0x0100)
#define CCMR_OC13FE_Mask            ((u16)0x7F7B)
#define CCMR_OC24FE_Mask            ((u16)0x7B7F)
#define CCMR_OC13PE_Mask            ((u16)0x7F77)
#define CCMR_OC24PE_Mask            ((u16)0x777F)
#define CCMR_OCM13_Mask             ((u16)0x7F0F)
#define CCMR_OCM24_Mask             ((u16)0x0F7F)

#define CCMR_OC13CE_Mask            ((u16)0xFF7F)
#define CCMR_OC24CE_Mask            ((u16)0x7FFF)

#define CCMR_IC13PSC_Mask           ((u16)0xFFF3)
#define CCMR_IC24PSC_Mask           ((u16)0xF3FF)
#define CCMR_IC13F_Mask             ((u16)0xFF0F)
#define CCMR_IC24F_Mask             ((u16)0x0FFF)

#define CCER_CC1P_Mask              ((u16)0xFFFD)
#define CCER_CC2P_Mask              ((u16)0xFFDF)
#define CCER_CC3P_Mask              ((u16)0xFDFF)
#define CCER_CC4P_Mask              ((u16)0xDFFF)

#define CCER_CC1E_Set               ((u16)0x0001)
#define CCER_CC1E_Reset             ((u16)0xFFFE)
#define CCER_CC1E_Mask              ((u16)0xFFFE)

#define CCER_CC2E_Set               ((u16)0x0010)
#define CCER_CC2E_Reset             ((u16)0xFFEF)
#define CCER_CC2E_Mask              ((u16)0xFFEF)

#define CCER_CC3E_Set               ((u16)0x0100)
#define CCER_CC3E_Reset             ((u16)0xFEFF)

#define CCER_CC4E_Set               ((u16)0x1000)
#define CCER_CC4E_Reset             ((u16)0xEFFF)
#define CCER_CC4E_Mask              ((u16)0xEFFF)

#define DCR_DMA_Mask                ((u16)0x0000)

/* TIM private Masks */
#define TIM_Period_Reset_Mask       ((u16)0x0000)
#define TIM_Prescaler_Reset_Mask    ((u16)0x0000)
#define TIM_Pulse_Reset_Mask        ((u16)0x0000)
#define TIM_ICFilter_Mask           ((u8)0x00)


#define CRL_CNF_Set      ((u16)0x0010)      /* Configuration Flag Enable Mask */
#define CRL_CNF_Reset    ((u16)0xFFEF)      /* Configuration Flag Disable Mask */
#define RTC_LSB_Mask     ((u32)0x0000FFFF)  /* RTC LSB Mask */
#define RTC_MSB_Mask     ((u32)0xFFFF0000)  /* RTC MSB Mask */
#define PRLH_MSB_Mask    ((u32)0x000F0000)  /* RTC Prescaler MSB Mask */


#define ACR_LATENCY_Mask         ((u32)0x00000038)
#define ACR_HLFCYA_Mask          ((u32)0xFFFFFFF7)
#define ACR_PRFTBE_Mask          ((u32)0xFFFFFFEF)

#ifdef _FLASH_PROG
/* Flash Access Control Register bits */
#define ACR_PRFTBS_Mask          ((u32)0x00000020) 

/* Flash Control Register bits */
#define CR_PG_Set                ((u32)0x00000001)
#define CR_PG_Reset              ((u32)0x00001FFE) 

#define CR_PER_Set               ((u32)0x00000002)
#define CR_PER_Reset             ((u32)0x00001FFD)

#define CR_MER_Set               ((u32)0x00000004)
#define CR_MER_Reset             ((u32)0x00001FFB)

#define CR_OPTPG_Set             ((u32)0x00000010)
#define CR_OPTPG_Reset           ((u32)0x00001FEF)

#define CR_OPTER_Set             ((u32)0x00000020)
#define CR_OPTER_Reset           ((u32)0x00001FDF)

#define CR_STRT_Set              ((u32)0x00000040)
							 
#define CR_LOCK_Set              ((u32)0x00000080)

/* FLASH Mask */
#define RDPRT_Mask               ((u32)0x00000002)
#define WRP0_Mask                ((u32)0x000000FF)
#define WRP1_Mask                ((u32)0x0000FF00)
#define WRP2_Mask                ((u32)0x00FF0000)
#define WRP3_Mask                ((u32)0xFF000000)

/* FLASH Keys */
#define RDP_Key                  ((u16)0x00A5)
#define FLASH_KEY1               ((u32)0x45670123)
#define FLASH_KEY2               ((u32)0xCDEF89AB)

/* Delay definition */   
#define EraseTimeout             ((u32)0x00000FFF)
#define ProgramTimeout           ((u32)0x0000000F)
#endif

//void FLASH_ClearFlag(u16 FLASH_FLAG)
//{
//  /* Clear the flags */
//  FLASH->SR = FLASH_FLAG;
//}

////得到FLASH状态
//u8 STMFLASH_GetStatus(void)
//{	
//	u32 res;		
//	res=FLASH->SR; 
//	if(res&(1<<0))return 1;		    //忙
//	else if(res&(1<<2))return 2;	//编程错误
//	else if(res&(1<<4))return 3;	//写保护错误
//	return 0;						//操作完成
//}
////等待操作完成
////time:要延时的长短
////返回值:状态.
//u8 STMFLASH_WaitDone(u16 time)
//{
//	u8 res;
//	do
//	{
//		res=STMFLASH_GetStatus();
//		if(res!=1)break;//非忙,无需等待了,直接退出.
//		time--;
//	 }while(time);
//	 if(time==0)res=0xff;//TIMEOUT
//	 return res;
//}
////擦除页
////paddr:页地址
////返回值:执行情况
//u8 FLASH_ErasePage(u32 paddr)
//{
//	u8 res=0;
//	res=STMFLASH_WaitDone(0X5FFF);//等待上次操作结束,>20ms    
//	if(res==0)
//	{ 
//		FLASH->CR|=1<<1;//页擦除
//		FLASH->AR=paddr;//设置页地址 
//		FLASH->CR|=1<<6;//开始擦除		  
//		res=STMFLASH_WaitDone(0X5FFF);//等待操作结束,>20ms  
//		if(res!=1)//非忙
//		{
//			FLASH->CR&=~(1<<1);//清除页擦除标志.
//		}
//	}
//	return res;
//}
//u8 FLASH_ProgramWord(u32 Address, u32 Data)
//{
//  u8 status = FLASH_COMPLETE;


//  /* Wait for last operation to be completed */
//  status = STMFLASH_WaitDone(ProgramTimeout);
//  
//  if(status == FLASH_COMPLETE)
//  {
//    /* if the previous operation is completed, proceed to program the new first 
//    half word */
//    FLASH->CR |= CR_PG_Set;
//  
//    *(vu16*)Address = (u16)Data;

//    /* Wait for last operation to be completed */
//    status = STMFLASH_WaitDone(ProgramTimeout);
// 
//    if(status == FLASH_COMPLETE)
//    {
//      /* if the previous operation is completed, proceed to program the new second 
//      half word */
//      *(vu16*)(Address + 2) = Data >> 16;
//    
//      /* Wait for last operation to be completed */
//      status = STMFLASH_WaitDone(ProgramTimeout);
//        
//      if(status != FLASH_BUSY)
//      {
//        /* Disable the PG Bit */
//        FLASH->CR &= CR_PG_Reset;
//      }
//    }
//    else
//    {
//      if (status != FLASH_BUSY)
//      {
//        /* Disable the PG Bit */
//        FLASH->CR &= CR_PG_Reset;
//      }
//     }
//  }
//  /* Return the Program Status */
//  return status;
//}

//void FLASH_Unlock(void)
//{
//  /* Authorize the FPEC Access */
//  FLASH->KEYR = FLASH_KEY1;
//  FLASH->KEYR = FLASH_KEY2;
//}





#define CR1_RUN_Set               ((u16)0x2000)  /* USART Enable Mask */
#define CR1_RUN_Reset             ((u16)0xDFFF)  /* USART Disable Mask */

#define CR2_Address_Mask          ((u16)0xFFF0)  /* USART address Mask */

/* USART RWU Mask */
#define CR1_RWU_Set               ((u16)0x0002)  /* USART mute mode Enable Mask */
#define CR1_RWU_Reset             ((u16)0xFFFD)  /* USART mute mode Enable Mask */

#define USART_IT_Mask             ((u16)0x001F)  /* USART Interrupt Mask */

/* USART LIN Mask */
#define CR2_LINE_Set              ((u16)0x4000)  /* USART LIN Enable Mask */
#define CR2_LINE_Reset            ((u16)0xBFFF)  /* USART LIN Disable Mask */

#define CR1_SBK_Set               ((u16)0x0001)  /* USART Break Character send Mask */

/* USART SC Mask */
#define CR3_SCEN_Set              ((u16)0x0020)  /* USART SC Enable Mask */
#define CR3_SCEN_Reset            ((u16)0xFFDF)  /* USART SC Disable Mask */

/* USART SC NACK Mask */
#define CR3_NACK_Set              ((u16)0x0010)  /* USART SC NACK Enable Mask */
#define CR3_NACK_Reset            ((u16)0xFFEF)  /* USART SC NACK Disable Mask */

/* USART Half-Duplex Mask */
#define CR3_HDSEL_Set             ((u16)0x0008)  /* USART Half-Duplex Enable Mask */
#define CR3_HDSEL_Reset           ((u16)0xFFF7)  /* USART Half-Duplex Disable Mask */

/* USART IrDA Mask */
#define CR3_IRLP_Mask             ((u16)0xFFFB)  /* USART IrDA LowPower mode Mask */

/* USART LIN Break detection */
#define CR3_LBDL_Mask             ((u16)0xFFDF)  /* USART LIN Break detection Mask */

/* USART WakeUp Method  */
#define CR3_WAKE_Mask             ((u16)0xF7FF)  /* USART WakeUp Method Mask */

/* USART IrDA Mask */
#define CR3_IREN_Set              ((u16)0x0002)  /* USART IrDA Enable Mask */
#define CR3_IREN_Reset            ((u16)0xFFFD)  /* USART IrDA Disable Mask */

#define GTPR_LSB_Mask             ((u16)0x00FF)  /* Guard Time Register LSB Mask */
#define GTPR_MSB_Mask             ((u16)0xFF00)  /* Guard Time Register MSB Mask */

#define CR1_CLEAR_Mask            ((u16)0xE9F3)  /* USART CR1 Mask */
#define CR2_CLEAR_Mask            ((u16)0xC0FF)  /* USART CR2 Mask */
#define CR3_CLEAR_Mask            ((u16)0xFCFF)  /* USART CR3 Mask */




//#define AIRC_VECTKEY_MASK    ((u32)0x05FA0000)
//void NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset)
//{ 
//  /* Check the parameters */
//  assert(IS_NVIC_VECTTAB(NVIC_VectTab));
//  assert(IS_NVIC_OFFSET(Offset));  
//   
//  SCB->ExceptionTableOffset = NVIC_VectTab | (Offset & (u32)0x1FFFFF80);
//}
//void NVIC_PriorityGroupConfig(u32 NVIC_PriorityGroup)
//{
//  /* Check the parameters */
//  assert(IS_NVIC_PRIORITY_GROUP(NVIC_PriorityGroup));
//  
//  /* Set the PRIGROUP[10:8] bits according to NVIC_PriorityGroup value */
//  SCB->AIRC = AIRC_VECTKEY_MASK | NVIC_PriorityGroup;
//}

//void NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct)
//{
//  u32 tmppriority = 0x00, tmpreg = 0x00, tmpmask = 0x00;
//  u32 tmppre = 0, tmpsub = 0x0F;
// 
//  if (NVIC_InitStruct->NVIC_IRQChannelCmd != DISABLE)
//  {
//    /* Compute the Corresponding IRQ Priority --------------------------------*/    
//    tmppriority = (0x700 - (SCB->AIRC & (u32)0x700))>> 0x08;
//    tmppre = (0x4 - tmppriority);
//    tmpsub = tmpsub >> tmppriority;
//    
//    tmppriority = (u32)NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority << tmppre;
//    tmppriority |=  NVIC_InitStruct->NVIC_IRQChannelSubPriority & tmpsub;

//    tmppriority = tmppriority << 0x04;
//    tmppriority = ((u32)tmppriority) << ((NVIC_InitStruct->NVIC_IRQChannel & (u8)0x03) * 0x08);
//    
//    tmpreg = NVIC->Priority[(NVIC_InitStruct->NVIC_IRQChannel >> 0x02)];
//    tmpmask = (u32)0xFF << ((NVIC_InitStruct->NVIC_IRQChannel & (u8)0x03) * 0x08);
//    tmpreg &= ~tmpmask;
//    tmppriority &= tmpmask;  
//    tmpreg |= tmppriority;

//    NVIC->Priority[(NVIC_InitStruct->NVIC_IRQChannel >> 0x02)] = tmpreg;
//    
//    /* Enable the Selected IRQ Channels --------------------------------------*/
//    NVIC->Enable[(NVIC_InitStruct->NVIC_IRQChannel >> 0x05)] =
//      (u32)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (u8)0x1F);
//  }
//  else
//  {
//    /* Disable the Selected IRQ Channels -------------------------------------*/
//    NVIC->Disable[(NVIC_InitStruct->NVIC_IRQChannel >> 0x05)] =
//      (u32)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (u8)0x1F);
//  }
//}

