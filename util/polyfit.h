#ifndef POLYFIT_H
#define POLYFIT_H

//https://github.com/natedomin/polyfit/blob/master/test_polyfit.cpp

class PolyFitClz
{
public:
    PolyFitClz() = delete;

    static int plotfit(const double* const dependentValues,
                const double* const independentValues,
                unsigned int        countOfElements,
                unsigned int        order,
                double*             coefficients);

};

#endif // POLYFIT_H
