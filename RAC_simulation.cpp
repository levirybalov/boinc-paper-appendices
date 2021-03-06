/*
This code is a simulation of RAC over time. 
This code was modified from the original RAC code, which can be found here: 
http://web.archive.org/web/20120418125739/http://www.boinc-wiki.info/Recent_Average_Credit
*/

#include <iostream>
#include <math.h>
#define SECONDS_PER_DAY 86400

using namespace std;

void update_average (
	double work_start_time,         // when new work was started // (or zero if no new work)
	double work,                    // amount of new work
	double half_life,
	double& avg,                    // average work per day (in and out)
	double& avg_time,               // when average was last computed
	double& fakeTime                // new, for simulations
) {
	//double now = dtime();
	double now = fakeTime;
	
	if (avg_time) {
	double diff, diff_days, weight;
	
	diff = now - avg_time;
	if (diff<0) diff=0;
	diff_days = diff/SECONDS_PER_DAY;
	
	weight = exp(-diff*M_LN2/half_life);
	
	avg *= weight;
	
	if ((1.0-weight) > 1.e-6) {
		avg += (1-weight)*(work/diff_days);
	} 
	else {
		avg += M_LN2*work*SECONDS_PER_DAY/half_life;
	}
}
else if (work) {
	// If first time, average is just work/duration
	//
	cout << "avg_time = " << avg_time << "\n";
	cout << "now = " << now << "\n";
	double dd = (now - work_start_time)/SECONDS_PER_DAY;
	avg = work/dd;
	}
	avg_time = now;
}

int main() {
	double RAC = 0;
	double timeOne = 1;
	double timeTwo = 1;
	double totalCredit = 0;
	double timeInterval = 3600; // new; time in seconds between each RAC update
	
	double work_start_time = 0;         // when new work was started // (or zero if no new work)
	double work = 200;                  // amount of new work
	double half_life = 604800;
	double& avg = RAC;                  // average work per day (in and out)
	double& avg_time = timeOne;         // when average was last computed
	double& fakeTime = timeTwo;         // new; for simulation
	
	for (int i=0; i<1500; i++) {
		if (1) {
			if (i % 24 == 0) {
              			cout<<"week " << i/168 + 1 << ", day " << (i/24)%7 + 1 << "; current hour = " << i << "; ";
				cout<<"totalCredit = " << totalCredit << "; ";
				cout<<"RAC = "<< RAC << "\n";
			}
		}
		fakeTime += timeInterval;
		update_average(work_start_time, work, half_life, avg, avg_time, fakeTime);
		totalCredit += work;
	}
	
	cout<<"Final totalCredit = " << totalCredit << "\n";
	cout<<"Final fakeTime = " << fakeTime << "\n";
	cout<<"Final RAC = "<< RAC;
	
	return 0;
}
