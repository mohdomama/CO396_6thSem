#include <iostream>
#include <vector>

using namespace std;

void check_avail(int time_taken, int *arr_time, int n, bool *avail) {
	for (int i = 0; i < n; ++i) {
		if (time_taken >= arr_time[i]) {
			avail[i] = true;
		}
	}
}

int main() {
	int n;
	int pos;
	string dir;
	int n_processed = 0;
	int time_taken = 0;
	int dis_covered = 0;

	cout << "Enter number of processes:" << endl;
	cin >> n; 

	cout << "Enter initial pos:" << endl;
	cin >> pos;
	
	cout << "Enter initial dir:" << endl;
	cin >> dir;

	int cylinder[n];
	int arr_time[n];
	int ser_time[n];
	bool avail[n];
	bool processed[n];

	cout << "Enter cylinder, arr_time and ser_time of all processes:" << endl;
	for (int i = 0; i < n; ++i)	{
		cin >> cylinder[i] >> arr_time[i] >> ser_time[i];
		
	}

	for (int i = 0; i < n; ++i) {
		processed[i]  = false;
	}

	for (int i = 0; i < n; ++i) {
		avail[i]  = false;
	}
	cout << "Reached" << endl;
	while(n_processed < n) {
		check_avail(time_taken, arr_time, n, avail);

		for (int i = 0; i < n; ++i) {
			if (avail[i] && !processed[i] && pos == cylinder[i]) {
				cout << "Processing: " << i << endl;
				cout << "Start Time: " << time_taken << endl;
				cout << "End Time: " << time_taken + ser_time[i] << endl;
				cout << endl;
				time_taken += ser_time[i];
				n_processed++;
				processed[i] = true;

			}
		}
		
		if (dir == "left" && pos == 0) {
			dir = "right";
		}
		else if(dir == "right" && pos == 199) {
			dir = "left";
		}
		if (dir == "left") {
			pos--;
		}
		else{
			pos++;
		}
		time_taken += 1;
		dis_covered ++;
	}

	for (int i = 0; i < n; ++i) {
		cout << processed[i] << " ";
	}
	cout << endl;

	cout << "Total time: " << --time_taken << endl;
	cout << "dis_covered: " << --dis_covered << endl;

	return 0;
}