#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include "rb-tree.h"
using namespace std::chrono;

double random_0_1() {
	int rd = rand() * 10000 + rand() * 1000 + rand() * 100 + rand() * 10 + rand() * 1;
	int max = RAND_MAX * 10000 + RAND_MAX * 1000 + RAND_MAX * 100 + RAND_MAX * 10 + RAND_MAX * 1;
	double b = rd / (max + 0.0001);
	return b;
}


int main() {

	if (1) { // test insert
		system_clock::time_point start_time = system_clock::now();

		int insertTestCount = 100;
		int ranCount = 1000;
		int* rbarr = (int*)malloc(sizeof(int) * ranCount);

		for (int j = 0; j < insertTestCount; j++) {
			for (int i = 0; i < ranCount; i++) {
				rbarr[i] = random_0_1() * 1000000;
			}
			RBTree* rbTree = newRBTree();
			rb_init(rbTree, rbarr, ranCount);
			int succ = rb_check(rbTree);
			freeRBTree(rbTree);
			rbTree = NULL;
			if (!succ) {
				break;
			}
		}
		free(rbarr);

		system_clock::time_point end_time = system_clock::now();
		duration<double, std::milli> diff = end_time - start_time;
		printf("插入测试时间为 %lf ms \n", diff.count());
	}


	if (1) { // test delete
		system_clock::time_point start_time = system_clock::now();

		int insertTestCount = 100;
		int ranCount = 1000;
		int* rbarr = (int*)malloc(sizeof(int) * ranCount);

		for (int j = 0; j < insertTestCount; j++) {
			for (int i = 0; i < ranCount; i++) {
				rbarr[i] = random_0_1() * 1000000;
			}
			RBTree* rbTree = newRBTree();
			rb_init(rbTree, rbarr, ranCount);
			rb_check(rbTree);

			for (int i = 0; i < ranCount; i++) {
				rb_delete(rbTree, rbarr[i]);
				rb_check(rbTree);
			}

			freeRBTree(rbTree);
			rbTree = NULL;
		}
		free(rbarr);

		system_clock::time_point end_time = system_clock::now();
		duration<double, std::milli> diff = end_time - start_time;
		printf("插入删除时间为 %lf ms \n", diff.count());
	}

	return 0;
}