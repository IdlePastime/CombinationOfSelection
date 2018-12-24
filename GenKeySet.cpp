// GenKeySet.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#define ChangeBits 10
#define KeyLength  32
#define MaxBuffer  1000
using namespace std;
struct Key
{
	string probkey = " ";
	int lastchangedbit = 0;
	int changedbits = 0;
};

class Queue {
public:
	Key keyqueue[MaxBuffer];
	int rest = MaxBuffer-1;
	void inqueue(Key _key) {
		if (!this->isfull()) {
			keyqueue[rear] = _key;
			rear = (rear + 1) % MaxBuffer;
			rest--;
		}
		return;
	}
	Key dequeue() {
		if (!this->isempty()){
			Key resultKey=keyqueue[front];
			front = (front + 1) % MaxBuffer;
			rest++;
			return resultKey;
		}
		return keyqueue[front];
	}

	bool isempty() {
		if (front == rear)
			return true;
		else
			return false;
	}
	bool isfull() {
		if (front == (rear + 1)% MaxBuffer)
			return true;
		else
			return false;
	}
private:
	int front = 0;
	int rear = 0;
};

void findnext(Queue &KeyQueue);
vector<string> split(const string &str, const string &pattern);
static string cachefilename = "D:/cachefile.txt";
static int combinationcounter=0;
static string resultfile = "D:/result.txt";
int main()
{
	//writer.open("result.txt");
	string Rb = "10011110001111011111001110000110";

	streampos lastreadpos = 0;
	ofstream cleaner;
	cleaner.open(cachefilename);
	cleaner.clear();
	cleaner.close();
	cleaner.open(resultfile);
	cleaner.clear();
	cleaner.close();
	Key sourcekey;
	sourcekey.probkey = Rb;
	sourcekey.lastchangedbit = -1;
	sourcekey.changedbits = 0;

	Queue KeyQueue;
	KeyQueue.inqueue(sourcekey);
	bool isallfinished = false;
	//KeyQueue.initial();


	//clock_t start;
	//clock_t end;

	//start = clock();

	time_t start, end;
	start = time(NULL);
	while (!isallfinished) {
		while (!KeyQueue.isempty()) {
			findnext(KeyQueue);
		}

		//如果队列中的key被搜索完
		//检查缓存文件中的key有没有被搜索完
		//如果都搜索完，则整个循环结束

		//计算缓存文件末尾偏移量
		int counter = 0;
		ifstream offsetreader;
		offsetreader.open(cachefilename, ios::in);
		offsetreader.seekg(0, ios::end);
		streampos offset = offsetreader.tellg();

		//如果缓存文件里有key没被搜索，就将文件中key读到队列中，最多读MaxBuffer-1个
		if (offset != lastreadpos) {
			ifstream cachereader;
			cachereader.open(cachefilename,ios::app);
			cachereader.seekg(lastreadpos);
			while (lastreadpos<offset && counter<MaxBuffer-1) {
				Key tempKey;
				string regforKey;
				getline(cachereader, regforKey);
				// << regforKey;
				vector<string> spliter = split(regforKey, " ");
				tempKey.probkey = spliter[0];
				tempKey.lastchangedbit = stoi(spliter[1]);
				tempKey.changedbits = stoi(spliter[2]);
				KeyQueue.inqueue(tempKey);
				counter++;
				//返回上次指针位置
				lastreadpos = cachereader.tellg();
			}

			isallfinished = false;
		}

		//如果缓存文件中KEY被搜索完，结束整个循环
		else {
			//reader.clear();
			isallfinished = true;
		}
	}
	//end = clock();
	end = time(NULL);
	cout << (end - start);
    return 0;
}

void findnext(Queue & KeyQueue)
{
	int storemodel = 0;
	Key tempKey = KeyQueue.dequeue();
	if (tempKey.changedbits != 0) {
		ofstream resultwriter;
		resultwriter.open(resultfile, ios::app);
		resultwriter << tempKey.probkey<<endl;
		resultwriter.close();
	}
	//如果已经改变的bit数小于应该改变的bit数，则在剩余位中选一个改变
	//否则直接返回
	if (tempKey.changedbits<ChangeBits) {
		ofstream writer;
		//判断队列剩余空间是否足够放入将生成的可能密钥
		if ((KeyLength - tempKey.lastchangedbit+1)>=KeyQueue.rest) {
			storemodel = 1;
			writer.open(cachefilename,ios::app);
		}
		for (int i = tempKey.lastchangedbit+1;i < KeyLength;i++) {
			Key nextKey;
			nextKey.probkey = tempKey.probkey;
			nextKey.probkey.at(i) = (1 - (int)(nextKey.probkey.at(i) - '0')) + '0';
			//.changenum = temKey.changenum;
			//cout << nextKey.probkey << endl;
			nextKey.lastchangedbit = i;
			nextKey.changedbits = tempKey.changedbits + 1;

			//如果队列中剩余空间足够，就放入队列
			if (storemodel == 0) {
				KeyQueue.inqueue(nextKey);
			}
			//否则写到tempfile文件末尾
			if (storemodel == 1) {
				writer << nextKey.probkey << " " << to_string(nextKey.lastchangedbit) << " " << to_string(nextKey.changedbits) << endl;
				writer.flush();
			}
		}
		writer.close();
	}
	else {
		return;
	}
	return;
}

vector<string> split(const string &str, const string &pattern) {
	char * strc = new char[strlen(str.c_str()) + 1];
	strcpy(strc, str.c_str());
	vector<string> resultVec;
	char* tmpStr = strtok(strc, pattern.c_str());
	while (tmpStr != NULL)
	{
		resultVec.push_back(string(tmpStr));
		tmpStr = strtok(NULL, pattern.c_str());
	}

	delete[] strc;
	return resultVec;
};
