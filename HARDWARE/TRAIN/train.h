#ifndef _TRAIN_H
#define _TRAIN_H
#include "sys.h"

typedef struct 
{
    float LastP;//�ϴι���Э���� ��ʼ��ֵΪ0.02
    float Now_P;//��ǰ����Э���� ��ʼ��ֵΪ0
    float out;//�������˲������ ��ʼ��ֵΪ0
    float Kg;//���������� ��ʼ��ֵΪ0
    float Q;//��������Э���� ��ʼ��ֵΪ0.001
    float R;//�۲�����Э���� ��ʼ��ֵΪ0.543
}KFP;//Kalman Filter parameter
float kalmanFilter(KFP *kfp,float input);
void KFP_init(float LastP, float Now_P, float out, float Kg, float Q, float R, KFP* KFP);
void distortion_calculation(void);

#endif
