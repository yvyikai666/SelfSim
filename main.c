#include "SelfSim.h"


void rK5_dynamics(double t, double *x, double *fx)
{
    // electromagnetic model
    fx[2] = IM.rreq * x[0] - IM.alpha * x[2] - x[4] * x[3];
    fx[3] = IM.rreq * x[1] - IM.alpha * x[3] + x[4] * x[2];
    fx[0] = (IM.ual - IM.rs * x[0] - fx[2]) / IM.Lsigma;
    fx[1] = (IM.ube - IM.rs * x[1] - fx[3]) / IM.Lsigma;

    // mechanical model
    IM.Tem = IM.npp * (x[1] * x[2] - x[0] * x[3]);
    fx[4] = (IM.Tem - IM.Tload) * IM.mu_m;
}


void rk555_Lin(double t, double *x, double hs)
{
    double k1[5], k2[5], k3[5], k4[5], xk[5];
    double fx[5];
    int i;

    rK5_dynamics(t, x, fx);
    for (i = 0; i < 5; ++i)
    {
        k1[i] = fx[i] * hs;
        xk[i] = x[i] + k1[i] * 0.5;
    }

    rK5_dynamics(t, xk, fx);
    for (i = 0; i < 5; ++i)
    {
        k2[i] = fx[i] * hs;
        xk[i] = x[i] + k2[i] * 0.5;
    }

    rK5_dynamics(t, xk, fx);
    for (i = 0; i < 5; ++i)
    {
        k2[i] = fx[i] * hs;
        xk[i] = x[i] + k3[i];
    }

    rK5_dynamics(t, xk, fx); // timer.t+hs,
    for (i = 0; i < 5; ++i)
    {
        k4[i] = fx[i] * hs;
        x[i] = x[i] + (k1[i] + 2 * (k2[i] + k3[i]) + k4[i]) / 6.0;
    }
}


int machine_simulation()
{
    rk555_Lin(CTRL.timebase, IM.x, IM.Ts);

    IM.ids = IM.x[0];
    IM.iqs = IM.x[1];
    IM.rpm = IM.x[4] * 60 / (2 * M_PI * IM.npp);

    if (isNumber(IM.rpm))
        return false;
    else
        return true;
}


int main()
{
    printf("NUMBER_OF_LINES: %d\n\n", NUMBER_OF_LINES);


    IM_init();
    CTRL_init();

    FILE *fw = fopen("algorithm.dat", "w");

    /* main loop */
    clock_t begin, end;
    begin = clock();
    int _;
    int dfe = 0;
    for (_ = 0; _ < NUMBER_OF_LINES; ++_)
    {
        IM.rpm = 50;
        IM.Tload = 10;

        if (machine_simulation())
        {
            printf("Break the loop.\n");
            break;
        }

        if (++dfe == DOWN_FREQ_EXE)
        {
            dfe = 0;

            CTRL.timebase += TS;

            write_data_to_file(fw);


            double freq = 2; // 0.15 ~ 0.5 ~ 2 （0.1时电压李萨茹就变成一个圆了）
            double volt = VF_RATIO * freq;
            IM.ual = volt * cos(2 * M_PI * freq * CTRL.timebase);
            IM.ube = volt * sin(2 * M_PI * freq * CTRL.timebase);
        }
    }
    end = clock();
    printf("The simulation in C cost %g sec.\n", (double) (end - begin) / CLOCKS_PER_SEC);
    fclose(fw);

    system("D:/Desktop/SelfSim/venv/Scripts/python.exe ../plot.py");

    return 0;
}


void write_data_to_file(FILE *fw)
{
    static int j = 0, jj = 0;

    {
        if (++j == 10)
        {
            j = 0;
            fprintf(fw, "%g,%g,%g,%g,%g\n",
                    IM.x[0], IM.x[1], IM.x[2], IM.x[3], IM.x[4] / IM.npp / 2 / M_PI * 60);
        }
    }
}


bool isNumber(double x)  // 判断是不是NaN
{
    return (x == x);
}


struct InductionMachineSimulated IM;
void IM_init() // 初始化感应电机（其实就和一个只有属性而没有方法的类一样）
{
    int i;
    for (i = 0; i < 5; ++i)
    {
        IM.x[i] = 0.0;
    }
    IM.rpm = 0.0;  // 转速 r/min

    IM.iqs = 0.0;
    IM.ids = 0.0;

    IM.Tload = 0.0;  // 负载转矩
    IM.rpm_cmd = 0.0;
    IM.rpm_deriv_cmd = 0.0;

    IM.Lmu = 0.4482;
    IM.Lsigma = 0.0126;

    IM.rreq = 1.69;
    IM.rs = 3.04;

    IM.alpha = IM.rreq / (IM.Lmu);
    IM.Lmu_inv = 1.0 / IM.Lmu;

    IM.Js = 0.0636; // Awaya92 using im.omg
    IM.npp = 2;
    IM.mu_m = IM.npp / IM.Js;

    IM.Ts = IM_TS;

    IM.ual = 0.0;
    IM.ube = 0.0;
}


struct ControllerForExperiment CTRL;

/* Initialization */
void CTRL_init(){
    int i=0,j=0;

    CTRL.timebase = 0.0;

    /* Parameter (including speed) Adaptation */
    CTRL.rs     = IM.rs;
    CTRL.rreq   = IM.rreq;
    CTRL.Lsigma = IM.Lsigma;
    CTRL.alpha  = IM.alpha;
    CTRL.Lmu    = IM.Lmu;
    CTRL.Lmu_inv = 1.0/IM.Lmu;
    CTRL.Js     = IM.Js;
    CTRL.Js_inv = 1.0/IM.Js;

    CTRL.ual = 0.0;
    CTRL.ube = 0.0;

    CTRL.rpm_cmd = 0.0;
}