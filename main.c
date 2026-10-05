#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

// Capacitor structure (from assignment)

struct Capacitor {
    double* time;      // time array
    double* voltage;   // voltage array
    double* current;   // current array
    double C;          // capacitance value
};

void initializeArrays(Capacitor& cap, int timesteps, double dt); // allocate and initialize arrays
void simulateConstantCurrent(Capacitor& cap, int timesteps, double dt, double I); // simulate constant current charging
void simulateConstantVoltage(Capacitor& cap, int timesteps, double dt, double R, double V0); // simulate constant voltage charging
void printResults(const Capacitor& cap, int timesteps); // print results every 200 timesteps

int main() {

    const double dt = 1e-10; // timestep
    const double finalTime = 5e-6; // final time  
    const int timesteps = finalTime / dt; // number of timesteps
    const double R = 1000.0; // 1 kOhm resistor for constant voltage case
    const double C = 100e-12; // 100 pF capacitor
    const double I = 1e-2; // constant current of 10 mA for constant current case
    const double V0 = 10.0; // constant voltage of 10 V for constant voltage case

    // Create capacitor structure and set capacitance
    Capacitor cap;
    cap.C = C;

    // Allocate and initialize arrays
    initializeArrays(cap, timesteps, dt);

    // Case 1: Constant current charging
    cout << "\n=== Constant Current Charging ===\n"; 
    simulateConstantCurrent(cap, timesteps, dt, I); 
    printResults(cap, timesteps);

    // Case 2: Constant voltage charging
    cout << "\n=== Constant Voltage Charging ===\n";
    simulateConstantVoltage(cap, timesteps, dt, R, V0);
    printResults(cap, timesteps);

    // Free memory
    delete[] cap.time;
    delete[] cap.voltage;
    delete[] cap.current;

    return 0;
}

// Allocate and initialize arrays
void initializeArrays(Capacitor& cap, int timesteps, double dt) {
    cap.time = new double[timesteps];
    cap.voltage = new double[timesteps];
    cap.current = new double[timesteps];

    // Fill time array with time values
    for (int i = 0; i < timesteps; i++) {
        cap.time[i] = i * dt;
    }

    // Initial conditions (assignment rules)
    cap.voltage[0] = 0.0;   // constant current case: V(0) = 0 V, constant voltage case: V(0) = 0 V
    // current[0] will be set inside each simulation function based on the case (constant current or constant voltage)
}

// Constant current simulation
void simulateConstantCurrent(Capacitor& cap, int timesteps, double dt, double I) { 

    cap.current[0] = I;

    for (int t = 1; t < timesteps; t++) {

        // Voltage update requires integrating current:
        // V(t+1) = V(t) + I(t)*dt*(1/C)
        cap.voltage[t] = cap.voltage[t-1] + I * dt * (1/cap.C); 

        // Current stays constant for this case:
        cap.current[t] = I;
    }
}

// Constant voltage simulation
void simulateConstantVoltage(Capacitor& cap, int timesteps, double dt, double R, double V0) { // given by assignment

    // Initial current: I(0) = V0 / R 
    cap.current[0] = V0 / R; 

    for (int t = 1; t < timesteps; t++) { 

        // Current update requires solving the differential equation for an RC circuit:
        // I(t+1) = I(t) - (I(t)/(R*C))*dt
        cap.current[t] = cap.current[t-1] - (cap.current[t-1] / (R * cap.C)) * dt;

        // Voltage update requires integrating current:
        // V(t+1) = V(t) + I(t)*dt*(1/C)
        cap.voltage[t] = cap.voltage[t-1] + cap.current[t] * dt * (1 / cap.C);
    }
}

// Print every 200 timesteps 
void printResults(const Capacitor& cap, int timesteps) { 

    for (int t = 0; t < timesteps; t += 200) { // print every 200 timesteps
        cout << "t = " << cap.time[t] 
             << "   V = " << cap.voltage[t]
             << "   I = " << cap.current[t]
             << endl;
    }
}

