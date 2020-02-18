#ifndef UTILITY_H
#define UTILITY_H

#include <QCoreApplication>
#include <QVector>
#include <string>
#include <fstream>
#include <iostream>
#include <string>

namespace utility {

/* File lines formats */

static const char* FormatKAT_OBS_R = "%c%d%d%lld%lld%lld%d%2s%8s%d%lf%lf%d%lf%lf%lf%d%lf";
static const char* FormatKAT_OBS_W = "e%3d%3d%6lld%6lld%8lld%5d%3s%9s%6d%6.1lf%7.1lf%3d%8.1lf%10.2lf%6.3lf%3d%7.1lf";

static const char* FormatKAT_KBO_R = "%c%lld%lld%d%3s%lld%lld%lf%lf%d%d%lf%d%d%d%8s%lf";
static const char* FormatKAT_KBO_W = "e%7lld%8lld%5d%3s%6lld%6lld%6.1lf%7.1lf%3d%6d%6.3lf%3d%4d%4d% 9s%7.1lf";

static const char* FormatOGA_R = "%lf%lf%lf%lf%d";
static const char* FormatCGA_W = " %20.12lf%10.4lf%8.4lf%14.3lf%15.7lf%5d";
static const char* FormatCGA_R = " %lf%lf%lf%lf%lf%d";
static const char* FormatEGA_R = "%c%lld%lf%lf%lf";
static const char* FormatPGA_W = " %20.12lf%12.4lf%12.4lf%12.4lf";
static const char* FormatPGA_R = "%lf%lf%lf%lf";

static const char* FormatKAT_KAL_R = "%c%d%2s%2s%lld%lld%d%lf%lf%lf%lf%lf%d%d%d%d%lld%d%lf%lf%lf%12s";
static const char* FormatKAT_KAL_W = "e %5d %2s %2s%6lld%6lld%4d%9.3lf%6.3lf%9.3lf%6.1lf%7.1lf%3d%5d%4d%3d%7lld%3d%7.3lf%7.3lf%12.4lf % 12s";

static const char* FormatSYS_CONF_R = "%lf%s%s%s%s%s";
static const char* FormatKAT_TAR_R = "%*d%*2s%2s%lf%*lf%*lf%*20s";
static const char* FormatEPA_AUTO_W = "%4d%6lld%4d%4d%8lld %10s";
static const char* FormatEPA_AUTO_R = "%d%lld%d%d%lld";

static const char* FormatCOPY_W = "%20.12lf%9.4lf%8.4lf%14.3lf%15.7lf%5d";

static const char* FormatSATELLIT_R = "%10s%lld%d%lld%d%d%d%d%s";


/* date & time functions */

void mjd_dat(int &d, int &m, int &y, int64_t mjd);
int64_t dat_mjd(int d, int m, int y);
int day_year(int d, int m, int y);
void sgms(int &hh, int &mm, double &ss, double &secd);


/* math functions */

double pod(double x, double y);
double pot(double x, int l);
double inter_parabola(double x0, double y0,
                      double x1, double y1,
                      double x2, double y2, double x);


void filter_omc(QVector<double> &x, QVector<double> &y,
                const QVector<double> &et, const QVector<double> &er,
                int bandLength, double bandWidth, double snr, double sigmaRate);

/* utility classes and structures */

struct plot_data {
    double plo0,plo1;
    int    ind0,ind1,ind2;
};

class File {
public:
    File()=delete;
    File(const std::string& name, std::ios_base::openmode mode)
        : _mode(mode),
          _name(name)
    {
        f.open(name, mode);
        if (!f.is_open()) {
            std::cout << "Unable to open file " << name << std::endl;
            throw std::runtime_error("Coulld not open file!");
        }
    }

    ~File()=default;

    std::string getLine(int nr)
    {
        if (_mode != std::ios_base::in)
            throw std::runtime_error("File not opened for reading");

        f.seekg(0, std::ios_base::beg);
        std::string temp;
        for (int i=0; i<nr; i++) {
            std::getline(f,temp);
        }
        f.seekg(0, std::ios_base::beg);
        return temp;
    }

    bool writeSingleNumber(int n)
    {
        if (_mode != std::ios_base::out)
            throw std::runtime_error("File not opened for writing");
        f << n << std::endl;
        return f.good();
    }

    bool readSingleNumber(int& n)
    {
        if (_mode != std::ios_base::in)
            throw std::runtime_error("File not opened for reading");
        f >> n;
        return f.good();
    }

private:
    std::fstream f;
    std::ios_base::openmode _mode;
    std::string _name;
};

class FanFilter {
    void minmax();
    void sigmaFilter();
    void gaus();
    void filtr();

    float ck[325][5],cd[325][6],s;
    double cl[8],cobr[8][8],d[8],cob9[8][8],c[8][8];
    float mx,my,tn,tk,dmax,dmin,mx1,my1,tn1,dmin1;
    int npoint,k;

public:
    Filter();
    float t[2700],r[2700],r1[2700],t1[2700];
    void run(const QVector<double> &at, const QVector<double> &ar);
    int mpoint,n,pmin;
};

}
#endif // UTILITY_H
