#pragma once
#include <atomic>
#include <iostream>
#include "counter.h"
using namespace std;

template <typename T>

class SharedPtr {
	counter* count;
	T* data;
public:
	SharedPtr() {
		this->data = nullptr;
		this->count = nullptr;
	}
	SharedPtr(T* ptr) {
		this->data = ptr;
		this->count = new counter(1);
	}
	SharedPtr(const SharedPtr& r) {
		this->data = r.data;
		this->count = r.count;
		count->increase();
	}
	SharedPtr(SharedPtr&& r) {
		this->data = r.data;
		this->count = r.count;
		count->increase();
		r.data = nullptr;
		r.count = nullptr;
	}
	~SharedPtr() {
		if (*this) {
			count->decrease();
		}
		if (!count) {
			cout << "kfkfk";
			delete data;
			delete count;
		}
	}
	auto operator = (const SharedPtr & r)->SharedPtr<T> & {
		if (!count) {
			delete data;
			delete count;
		}
		data = r.data;
		count = r.count;
		if (*this) {
			count->increase();
		}
		return *this;
	}
	
	auto operator = (SharedPtr && r)->SharedPtr & {
		if (!count) {
			delete data;
			delete count;
		}
		data = r.data;
		count = r.count;
		r.data = nullptr;
		r.count = nullptr;
		return *this;

	}

	// проверяет, указывает ли указатель на объект
	operator bool() const {
		return !(data == nullptr);
	}
	auto operator*() const->T & {
		if (!*this) {
			throw logic_error("Attempt to get value of nullptr");
		}
		return *data;
	}
	auto operator->() const->T* {
		return data;
	}

	auto get()->T* {
		return data;
	}
	void reset() {
		if (!*this) return;
		count->decrease();
		if (count == nullptr) {
			delete count;
			delete data;
		}
		count = nullptr;
		data = nullptr;
	}
	void reset(T* ptr) {
		if (*this) {
			count->decrease();
			if (*count == 0) {
				delete count;
				delete data;
			}
		}
		count = new counter(1);
		data = ptr;
	}
	void swap(SharedPtr& r) {
		if (data == r.data) return;
		auto buf = r;
		r = *this;
		*this = buf;
	}
	// возвращает количество объектов SharedPtr, которые ссылаются на тот же управляемый объект
	auto use_count() const->size_t {
		if (!*this) return 0;
		return static_cast<size_t>(count->get());
	}
};
