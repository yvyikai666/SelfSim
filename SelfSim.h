//
// Created by YVYIKAI on 2022/10/3.
//

#ifndef SELFSIM_SELFSIM_H
#define SELFSIM_SELFSIM_H


#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include "stdio.h"
//#include "controller.h"


#define bool _Bool

#define IM_TS 1.25e-4  // 采样时间间隔
//#define M_PI 3.1415
#define NUMBER_OF_LINES 200000
#define DOWN_FREQ_EXE 2
#define TS (IM_TS*DOWN_FREQ_EXE)
#define VF_RATIO 18


struct InductionMachineSimulated
{
    double x[13]; // x[0]是id, x[1]是iq, x[4]是omega角速度
    double rpm;
    double rpm_cmd;
    double rpm_deriv_cmd;
    double Tload;
    double Tem;

    double Lsigma;
    double rs;
    double rreq;
    double Lmu;
    double Lmu_inv;
    double alpha;

    double Js;
    double npp;
    double mu_m;
    double Ts;

    double iqs;
    double ids;

    double ual;
    double ube;
};
extern struct InductionMachineSimulated IM;

bool isNumber(double x);
void write_data_to_file(FILE *fw);

struct ControllerForExperiment
{
    double timebase;

    double ual;
    double ube;

    double rs;
    double rreq;
    double Lsigma;
    double alpha;
    double Lmu;
    double Lmu_inv;

    double Tload;
    double rpm_cmd;

    double Js;
    double Js_inv;
};

extern struct ControllerForExperiment CTRL;
void CTRL_init();
void IM_init();

#endif //SELFSIM_SELFSIM_H