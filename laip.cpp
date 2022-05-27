/*
*Name: natizgr8
*Description: This program implements the concept of parallell
*             programming by using the std::thread c++ library
*             to add two large numbers.
*Input : a filename of a file containing two large numbers.
*Output: the sum of the two numbers.
*/

#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

// prototypes
void add(const vector<int>&, const vector<int>&, vector<int>&, vector<int>&, int, int);

/*
* Main entry-point for this program.
* @returns Exit-code for the process - 0 for success, else an error code.
*/
int main()
{
    auto max_threads = thread::hardware_concurrency();  //< maximum possible threads

    string filename;                  //< input file
    ifstream file;                    //< input file stream object
    string str;          			  //< a string of one line of input
    vector<int> num1;                 //< a vector to store first num
    vector<int> num2;                 //< a vector to store second num
    vector<thread> threads;           //< a container of all threads
    auto id = 0;                      //< num1/num2 identifier


    // Accept a valid filename from user & open file
    while (true) {
        cout << "Enter file name: ";
        cin >> filename;
        file.open(filename);
        if(file.is_open()) {
            break;
        }
    }

    // read one line of data from input file; store it in a vector
    while (getline(file, str)) {
        ++id;
        if(id == 1) {
            // initialize num1
            for(auto ch : str) {
                num1.push_back(ch - '0');
            }
        }
        else {
            // initialize num2
            for(auto ch : str) {
                num2.push_back(ch - '0');
            }
        }
    }

    file.close();

    // Determine the maximum possible number of power-of-2 threads to run
    while(ceil(log2(max_threads)) != floor(log2(max_threads))) {
        --max_threads;
    }

    auto thread_size = num1.size()/max_threads;   //< num of digits in a thread
    vector<int> sum(num1.size());                 //< container of sum
    vector<int> carry(num1.size()/thread_size);   //< container of carry over

    // create the threads
    for(int i = 0, j = thread_size-1; j < num1.size();) {
        threads.push_back(thread(add, num1, num2, ref(sum), ref(carry), i, j));
        i += thread_size;
        j += thread_size;
    }

    // wait until all threads are done
    for(int i = 0; i < threads.size(); ++i) {
        threads[i].thread::join();
    }

    if(carry[0] != 0) {
        cout << carry[0];
    }

    auto i = sum.size()-thread_size-1;  //< index of sum to add last carry to

    // add carry-overs to sum
    for(auto j = carry.size()-1; j >= 1; j--) {
        int s = sum[i] + carry[j];  //< sum of carry & ith digit of sum

        if(s - 9 <= 0) {
            sum[i] = s;
            i -= thread_size;
            continue;
        }
        else {
            int k = thread_size;  //< distance from current i to the next
            while(s - 9 > 0) {
                int c = s / 10;  //< most sig digit of s(carry)
                sum[i] = s % 10;
                i -= 1;
                k -= 1;
                s = sum[i] + c;
            }
            sum[i] = s;
            i -= k;
            continue;
        }
        i -= thread_size;
    }

    // display sum
    for(auto& n : sum) {
        cout << n;
    }

    cout << endl;
    return 0;
}

/*
* add: given a range of integer values of two vectors,
*      this function sums up the corresponding integers.
* parameters: n1: vector holding first range of numbers
*			  n2: vector holding second range of numbers
*              S: a vector to hold the sum of each digit
*              C: a vector to hold the last carry-over
*           left: index of last number to add
*          right: index of first number to add
*/
void add(const vector<int>& n1, const vector<int>& n2,
         vector<int>& S, vector<int>& C, int left, int right) {
    auto single_dig_sum = 0;            //< sum of single digit addends
    auto single_dig_carry = 0;          //< carry-over of sum
    auto thread_length = right-left+1;  //< total number of digits

    // add the corresponding digits of n1 & n2 except the leftmost digits
    for(; right != left; --right) {
        single_dig_sum = n1[right] + n2[right] + single_dig_carry;

        // if 'single_dig_sum' is a two-digit number
        if(single_dig_sum - 9 > 0) {

            // store least significant digit in S
            S[right] = single_dig_sum % 10;

            // retain most significant digit as a carry-over
            single_dig_carry = single_dig_sum / 10;
        }

        // if 'single_dig_sum' ia a single-digit number
        else {
            S[right] = single_dig_sum;
            single_dig_carry = 0;
        }
    }

    // add the leftmost digits of n1 and n2; store out-of-bound carry to in C
    single_dig_sum = n1[left] + n2[left] + single_dig_carry;
    S[left] = single_dig_sum % 10;
    C[left/thread_length] = single_dig_sum / 10;

    return;
}

/* EOF */
