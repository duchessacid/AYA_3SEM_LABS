#pragma once
#include <iostream>
#include <memory>
#include <utility>
#include <exception>
#include "pch.h"
using namespace std;

template <typename T>
struct node
{
	T value;
	node* prev = nullptr;
};


template <typename T>
class stack
{
	node<T>* headnode = nullptr;
public:
	stack() = default;

	stack(const stack<T>& obj) = delete;

	stack(stack<T>&& obj) = default;

	~stack() {
		while (headnode) {
			auto* currnode = headnode;
			headnode = currnode->prev;
			delete currnode;
		}
	}

	void push(T&& value) {
		auto* prevnode = headnode;
		headnode = new node<T>{
			forward<T>(value),
			prevnode,
		};
	}

	void push(const T& value) {
		auto* prevnode = stack<T>::headnode;
		stack<T>::headnode = new node<T>{
			value,
			prevnode,
		};
	}

	const T& head() const {
		if (!headnode) {
			throw runtime_error("stack is empty");
		}
		return headnode->value;
	}

	template <typename... Args>
	void push_emplace(Args&& ... value) {
		auto* prevnode = stack<T>::headnode;
		stack<T>::headnode = new node<T>{
			{forward<Args>(value)...},
			prevnode,
		};
	}

	T pop() {
		if (!stack<T>::headnode) {
			throw runtime_error("stack is empty");
		}
		auto* prevnode = stack<T>::headnode;
		stack<T>::headnode = stack<T>::headnode->prev;
		T value = move(prevnode->value);
		delete prevnode;
		return value;
	}
};



