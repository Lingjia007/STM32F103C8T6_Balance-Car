#include "encoder.h"
/***********************************************
��˾����Ȥ�Ƽ�����ݸ�����޹�˾
Ʒ�ƣ�WHEELTEC
������wheeltec.net
�Ա����̣�shop114407458.taobao.com
����ͨ: https://minibalance.aliexpress.com/store/4455017
�汾��V1.0
�޸�ʱ�䣺2023-01-04

Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version: V1.0
Update��2023-01-04

All rights reserved
***********************************************/

/**************************************************************************
�������ܣ���λʱ���ȡ����������
��ڲ�������ʱ��
����  ֵ���ٶ�ֵ
**************************************************************************/
int Read_Encoder(TIM_HandleTypeDef *htim)
{
	int Encoder_TIM;

	// ��ȡ��������ǰֵ������
	Encoder_TIM = (short)__HAL_TIM_GET_COUNTER(htim);
	__HAL_TIM_SET_COUNTER(htim, 0);

	return Encoder_TIM;
}
///**************************************************************************
// �������ܣ�TIM3�жϷ�����
// ��ڲ�������
// ����  ֵ����
//**************************************************************************/
// void TIM4_IRQHandler(void)//�жϴ�����Ϊ�գ�����жϱ�־λ������ж�
//{
//  if(TIM_GetFlagStatus(TIM4,TIM_FLAG_Update)==SET)//����ж�
//  {
//
//  }
//  TIM_ClearITPendingBit(TIM4,TIM_IT_Update); //����жϱ�־λ
// }
///**************************************************************************
// �������ܣ�TIM2�жϷ�����
// ��ڲ�������
// ����  ֵ����
//**************************************************************************/
// void TIM2_IRQHandler(void)//�жϴ�����Ϊ�գ�����жϱ�־λ������ж�
//{
//  if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update)==SET)//����ж�
//  {
//
//  }
//  TIM_ClearITPendingBit(TIM2,TIM_IT_Update); //����жϱ�־λ
// }
