#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <memory>
#include <unordered_map>
#include <fstream>

using namespace std;

struct Operation {
    int operationCounter;
    int amount;
    int source;
    int dest;
};

struct OperationCounter {
    int counter;
    mutex mtx;
    OperationCounter() : counter(0) {}
};

const int ACCOUNTS = 1000;
const int TRANSACTIONS = 1000000;
const int INITIAL_AMOUNT = 1000;

int threadsCount = 7;

class Account {
public:
    int id;
    long long balance;
    vector<Operation> logs;
    mutex mtx;

    Account(int id, int balance) : id(id), balance(balance) {}

    void transfer(Account &dest, int amount, OperationCounter &serial) {
        // Lock both accounts in increasing id order
        if (this->id < dest.id) {
            this->mtx.lock();
            dest.mtx.lock();
        } else {
            dest.mtx.lock();
            this->mtx.lock();
        }

        // Obtain a unique operationCounter
        serial.mtx.lock();
        int serialValue = serial.counter;
        serial.counter++;
        serial.mtx.unlock();

        // Create and apply the operation
        Operation op = {serialValue, amount, this->id, dest.id};

        balance -= amount;
        logs.push_back(op);

        dest.balance += amount;
        dest.logs.push_back(op);

        if (this->id < dest.id) {
            this->mtx.unlock();
            dest.mtx.unlock();
        } else {
            dest.mtx.unlock();
            this->mtx.unlock();
        }
    }

    bool check() {
        long long total = 0;
        for (const Operation &op : logs) {
            if (op.source == id)
                total -= op.amount;
            else
                total += op.amount;
        }
        return balance - total == INITIAL_AMOUNT;
    }
};

void performTransactions(vector<shared_ptr<Account>> &accounts, OperationCounter &ser) {
    for (int i = 0; i < TRANSACTIONS / threadsCount; ++i) {
        int source = rand() % ACCOUNTS;
        int dest = rand() % ACCOUNTS;
        while (source == dest)
            dest = rand() % ACCOUNTS;
        int amount = rand() % 1000;
        accounts[source]->transfer(*accounts[dest], amount, ser);
    }
}

bool continueCheck;

void check(vector<shared_ptr<Account>> &accounts, OperationCounter &ser) {
    try {
        while (continueCheck) {
            for (int i = 0; i < ACCOUNTS; ++i)
                accounts[i]->mtx.lock();

            cout << ser.counter << endl;
            bool ok = true;

            unordered_map<int, int> seen;
            for (int i = 0; i < ACCOUNTS; ++i) {
                ok &= accounts[i]->check();
                for (const Operation &op : accounts[i]->logs)
                    ++seen[op.operationCounter];
            }

            for (const auto &p : seen) {
                if (p.second != 2) {
                    ok = false;
                    cout << "OperationCounter " << p.first << " seen " << p.second
                         << " times (expected 2)" << endl;
                    break;
                }
            }

            if (ok)
                cout << "All accounts are correct" << endl;
            else
                cout << "Some accounts are incorrect" << endl;

            for (int i = 0; i < ACCOUNTS; ++i)
                accounts[i]->mtx.unlock();

            this_thread::sleep_for(chrono::seconds(1));
        }
    } catch (const std::exception &e) {
        cout << "Done checking" << endl;
    }
}

int main(int argc, char const *argv[]) {
    if (argc > 1)
        threadsCount = atoi(argv[1]);

    continueCheck = true;

    // cout << "Number of threads: " << threadsCount << endl;

    vector<shared_ptr<Account>> accounts;
    OperationCounter cnt;
    accounts.reserve(ACCOUNTS);

    for (int i = 1; i <= ACCOUNTS; ++i)
        accounts.push_back(make_shared<Account>(i, INITIAL_AMOUNT));

    vector<thread> threads(threadsCount);

    auto start = chrono::steady_clock::now();

    for (int i = 0; i < threadsCount; i++)
        threads[i] = thread(performTransactions, ref(accounts), ref(cnt));

    thread checkThread = thread(check, ref(accounts), ref(cnt));

    for (int i = 0; i < threadsCount; ++i)
        threads[i].join();

    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Total time taken: " << duration.count() << " milliseconds" << endl;

    continueCheck = false;
    cout << cnt.counter << endl;
    cout << "All threads have finished" << endl;

    checkThread.detach();

    ofstream outFile("/Users/maraielciu/Desktop/pdp/results.txt", ios::app);
    if (outFile.is_open()) {
        outFile << "TEST DATA\n";
        outFile << "Hardware & Software: MacOS, 8 cores\n";
        outFile << "Threads: " << threadsCount << "\n";
        outFile << "Accounts: " << ACCOUNTS << "\n";
        outFile << "Transactions: " << TRANSACTIONS << "\n";
        outFile << "Initial amount per account: " << INITIAL_AMOUNT << "\n";
        outFile << "Total time (ms): " << duration.count() << "\n";
        outFile << "Final operationCounter: " << cnt.counter << "\n";
        outFile << "-------------------------------------\n\n";
        outFile.close();
    } else {
        cerr << "Error: could not open results.txt for writing\n";
    }
    return 0;
}
