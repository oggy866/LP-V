#include <iostream>
#include <vector>
#include <omp.h>
#include <limits>
using namespace std;
int main() {
int n;
cout << "Enter the size of the array: ";
cin >> n;
vector<int> arr(n);
cout << "Enter " << n << " elements:\n";
for (int i = 0; i < n; i++)
cin >> arr[i];
int min_val = numeric_limits<int>::max();
int max_val = numeric_limits<int>::min();
long long sum = 0;
#pragma omp parallel for reduction(min:min_val) reduction(max:max_val) reduction(+:sum)
for (int i = 0; i < n; i++) {
if (arr[i] < min_val) min_val = arr[i];
if (arr[i] > max_val) max_val = arr[i];
sum += arr[i];
}
double average = static_cast<double>(sum) / n;
cout << "\nMinimum value: " << min_val << endl;
cout << "Maximum value: " << max_val << endl;
cout << "Sum: " << sum << endl;
cout << "Average: " << average << endl;
return 0;
}