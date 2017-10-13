#include <string>
#include <cstdlib>
#include <stdexcept>

#include <SIRlib.h>
#include <CSVExport.h>
#include <PyramidTimeSeries.h>
#include <TimeStatistic.h>
#include <TimeSeries.h>
#include <RNG.h>
#include <Normal.h>

#include <Calibrate.h>
#include <PolyRegCal.hpp>

#include "SIRSimRunner.h"

using namespace SIRlib;
using namespace ComputationalLib;
using namespace SimulationLib;

using uint = unsigned int;

// Parameters:
// 1: fileName:
//      Prefix of .csv file name (do not specify extension). Three files will
//        be created: [fileName]-births.csv, [fileName]-deaths.csv,
//        [filename]-population.csv
// 2. λ:
//      transmission parameter (double | > 0) unit: [cases/day]
// 3. Ɣ:
//      duration of infectiousness. (double | > 0) double, unit: [day]
// 4. nPeople:
//      number of people in the population (uint | > 0)
// 5. ageMin:
//      minimum age of an individual (uint) unit: [years]
// 6. ageMax:
//      maximum age of an individual (uint | >= ageMin) unit: [years]
// 7. ageBreak:
//      interval between age breaks of population (uint | > 1, < (ageMax - ageMin)) unit: [years]
// 8. tMax:
//      maximum length of time to run simulation to (uint | >= 1) unit: [days]
// 9. Δt:
//      timestep (uint | >= 1, <= tMax) unit: [days]
//10. pLength:
//      length of one data-aggregation period (uint | > 0, < tMax) unit: [days]
using RunType = SIRSimRunner::RunType;

int main(int argc, char const *argv[])
{
    using Params = std::vector<SimulationLib::TimeSeries<int>::query_type>;
    if (argc < 12) {
        printf("Error: too few arguments\n");
        exit(1);
    }

    // Grab params from command line ////////////////////
    size_t i {0};
    auto fileName      = string(argv[++i]);
    auto λ             = (double)stof(argv[++i], NULL);
    auto Ɣ             = (double)stof(argv[++i], NULL);
    auto nPeople       = atol(argv[++i]);
    auto ageMin        = atoi(argv[++i]);
    auto ageMax        = atoi(argv[++i]);
    auto ageBreak      = atoi(argv[++i]);
    auto tMax          = atoi(argv[++i]);
    auto Δt            = atoi(argv[++i]);
    auto pLength       = atoi(argv[++i]);
    // End grab from command line ////////////////////////


    // Create historical data to compare against model data
    auto InfectedData = new PrevalenceTimeSeries<int>("Historical data", tMax, pLength, 1, nullptr);
    InfectedData->Record(0.0, 1);
    InfectedData->Record(1.0, 2);
    InfectedData->Record(2.0, 8);
    InfectedData->Record(3.0, 3);
    InfectedData->Record(4.0, 2);

    // Calculate likelihood on t=0,1,2,3,4
    Params Ps {0, 1, 2, 3, 4};

    // Create a normal distribution with stDev=1 for each model point
    auto DistributionGenerator = [] (auto v, auto t) {
        double standardDeviation {1.0};
        return StatisticalDistributions::Normal(v, standardDeviation);
    };

    // Create a lambda function for use with PolyRegCal routine. The
    // only parameters which can be varied by the iterative method
    // are λ and Ɣ.
    auto f = [&] (double λ, double Ɣ) {
        bool succ {true};
        
        auto S = SIRSimRunner(fileName, 1, λ, Ɣ, nPeople, ageMin, \
                              ageMax, ageBreak, tMax, Δt, pLength);
        
        succ &= S.Run<RunType::Serial>();

        auto Data = S.GetTrajectoryResult(0);
        auto InfectedModel = Data.Infected;
        auto Likelihood = CalculateLikelihood(InfectedModel, InfectedData, Ps, DistributionGenerator)

        return Likelihood;
    };

    auto CalibrationResult = PolyRegCal({1.0, 1.0}, f);

    return 0;
}
