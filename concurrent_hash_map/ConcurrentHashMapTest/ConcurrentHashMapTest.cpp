// ConcurrentHashMapTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <chrono>
#include <thread>
#include <mutex>
#include <unordered_map>

#define TOPIC_PREFIX            "TIT."
#define MARKETS_EXTENSIONS_QTY  100
#define TOPIC_GENERATOR_QTY     10000000
#define TOPIC_BODY_SIZE         10

#define HASH_MAP_POOL_SIZE      100

using namespace std;


struct concurrent_hash_map {
    mutex mtx;
    unordered_map<size_t, string>  hashMap;
};

unordered_map<unsigned int, concurrent_hash_map> hashMapPool;
vector<string> marketsExtensions;


char getRandomChar() {
    return char(rand() % 25 + 66);
}


void generateMarketsExtensions() {
    marketsExtensions.reserve(MARKETS_EXTENSIONS_QTY);

    for (int i = 0; i < MARKETS_EXTENSIONS_QTY; i++) {
        string mktExt;
        mktExt.push_back(getRandomChar());
        mktExt.push_back(getRandomChar());
        mktExt.push_back(getRandomChar());
        marketsExtensions.push_back(mktExt);
    }
}


void initStructures() {
    srand(time(NULL));
    generateMarketsExtensions();
    hashMapPool.reserve(HASH_MAP_POOL_SIZE);
}


string getNewTopic() {
    string topic = TOPIC_PREFIX;

    for (int i = 0; i < TOPIC_BODY_SIZE; i++)
        topic += getRandomChar();

    topic += "." + marketsExtensions[rand() % MARKETS_EXTENSIONS_QTY];

    return topic;
}


concurrent_hash_map& lockAndGetHashMap(const size_t key) {
    concurrent_hash_map& hashMap = hashMapPool[key % HASH_MAP_POOL_SIZE];
    hashMap.mtx.lock();
    return hashMap;
}


void populateCache(string name, int threads) {
    srand(unsigned int(&name));
    hash<string> string_hash;
    int failure = 0;

    for (int i = 0; i < TOPIC_GENERATOR_QTY/threads; i++) {
        string topic = getNewTopic();
        size_t key = string_hash(topic);

        concurrent_hash_map& hashMap = lockAndGetHashMap(key);
        hashMap.hashMap.insert(make_pair(key, topic));

        /*
        if (!hashMap.hashMap.insert(make_pair(key, topic)).second)
        {
            cout << "thread " << name << " failure on topic " << topic << endl;
            failure++;
        }
        */

        hashMap.mtx.unlock();
     
        /*
        if ((i + 1) % 100000 == 0)
            cout << "thread " << name << ": " << i + 1 << " (" << failure << ")" << endl;
        */
    }
}


int main()
{
    initStructures();

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    thread t1([&] {populateCache("1", 20); });
    thread t2([&] {populateCache("2", 20); });
    thread t3([&] {populateCache("3", 20); });
    thread t4([&] {populateCache("4", 20); });
    thread t5([&] {populateCache("5", 20); });
    thread t6([&] {populateCache("6", 20); });
    thread t7([&] {populateCache("7", 20); });
    thread t8([&] {populateCache("8", 20); });
    thread t9([&] {populateCache("9", 20); });
    thread t10([&] {populateCache("10", 20); });
    thread t11([&] {populateCache("11", 20); });
    thread t12([&] {populateCache("12", 20); });
    thread t13([&] {populateCache("13", 20); });
    thread t14([&] {populateCache("14", 20); });
    thread t15([&] {populateCache("15", 20); });
    thread t16([&] {populateCache("16", 20); });
    thread t17([&] {populateCache("17", 20); });
    thread t18([&] {populateCache("18", 20); });
    thread t19([&] {populateCache("19", 20); });
    thread t20([&] {populateCache("20", 20); });

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();
    t9.join();
    t10.join();
    t11.join();
    t12.join();
    t13.join();
    t14.join();
    t15.join();
    t16.join();
    t17.join();
    t18.join();
    t19.join();
    t20.join();

    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    int tot = 0;

    for (auto& m : hashMapPool)
    {
        cout << "map " << m.first << " --> " << m.second.hashMap.size() << endl;
        tot += m.second.hashMap.size();
        //for (auto& topic : m.second.hashMap)
        //    cout << topic.second << endl;
    }
    
    cout << endl << tot << " -> elapsed time: " << chrono::duration_cast<chrono::milliseconds> (end - begin).count() << " ms" << endl;

    system("pause");

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
