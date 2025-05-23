#pragma once

struct Sort
{
	int index;
	float y;
};

typedef bool (*MethodSort)(float f0, float f1);

struct iSort
{
	iSort(int max = 100);
	virtual ~iSort();

	Sort* _s;
	Sort** s;
	int num, max;

	void init(int max = 100);
	void add(float y);
	static bool compare(float f0, float f1)
	{
		return f0 > f1;
	}

	void update(MethodSort method = compare);

	int get(int i) { return s[i]->index; }
};


//class xSort
//{
//private:
//	xSort(){}
//	xSort(int max){}
//	~xSort(){}
//public:
//	static void init(int max = 100);
//
//	static void add(float y);
//
//	typedef bool(*MethodSort)(float f0, float f1);
//
//	void update(MethodSort method);
//
//	int get(int i);
//};
//
//
//
//struct HSort { int unused; };
//#define HANDLE_MICHIN(a) (a)
//#define HSort HANDLE_MICHIN(_HSort);	
//
//void SortInit(int max = 100);
//void SortAdd(float y);
//void SortUpdate(MethodSort method);
//int SortGet(int i);

