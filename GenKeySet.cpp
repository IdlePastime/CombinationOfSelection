// GenKeySet.cpp : �������̨Ӧ�ó������ڵ㡣
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

		//��������е�key��������
		//��黺���ļ��е�key��û�б�������
		//����������꣬������ѭ������

		//���㻺���ļ�ĩβƫ����
		int counter = 0;
		ifstream offsetreader;
		offsetreader.open(cachefilename, ios::in);
		offsetreader.seekg(0, ios::end);
		streampos offset = offsetreader.tellg();

		//��������ļ�����keyû���������ͽ��ļ���key���������У�����MaxBuffer-1��
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
				//�����ϴ�ָ��λ��
				lastreadpos = cachereader.tellg();
			}

			isallfinished = false;
		}

		//��������ļ���KEY�������꣬��������ѭ��
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
	//����Ѿ��ı��bit��С��Ӧ�øı��bit��������ʣ��λ��ѡһ���ı�
	//����ֱ�ӷ���
	if (tempKey.changedbits<ChangeBits) {
		ofstream writer;
		//�ж϶���ʣ��ռ��Ƿ��㹻���뽫���ɵĿ�����Կ
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

			//���������ʣ��ռ��㹻���ͷ������
			if (storemodel == 0) {
				KeyQueue.inqueue(nextKey);
			}
			//����д��tempfile�ļ�ĩβ
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
