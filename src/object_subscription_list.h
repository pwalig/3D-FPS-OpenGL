#pragma once
#include <vector>
#include <functional>

namespace engine {
	template<typename T>
	class object_subscription_list {
	private:
		volatile bool _running = false; // volatile to prevent compiler from optimizing
		std::vector<T*> _deletions;

		void _unsubscribe(T* obj);

	public:
		std::vector<T*> _objects; // try not to use

		void subscribe(T* obj);
		void perform_on_all(const std::function<void(T*)>& func_);
		bool on_list(T* obj);
		void unsubscribe(T* obj);
		void clear();
	};

	template<typename T>
	inline void object_subscription_list<T>::_unsubscribe(T* obj)
	{
		typename std::vector<T*>::iterator id = std::find(this->_objects.begin(), this->_objects.end(), obj);
		if (id != this->_objects.end()) this->_objects.erase(id);
	}

	template<typename T>
	inline void object_subscription_list<T>::subscribe(T* obj)
	{
		this->_objects.push_back(obj);
	}

	template<typename T>
	inline void object_subscription_list<T>::perform_on_all(const std::function<void(T*)>& func_)
	{
		this->_running = true; // event calling started

		int siz = this->_objects.size();
		for (typename std::vector<T*>::iterator it = this->_objects.begin(); it != this->_objects.begin() + siz; ++it) {
			const auto d = std::distance(this->_objects.begin(), it);
			func_(*it);
			it = this->_objects.begin();
			std::advance(it, d);
		}

		// perform scheduled deletions
		for (T* id : _deletions) {
			this->_unsubscribe(id);
		}
		this->_deletions.clear();

		this->_running = false; // event calling ended
	}
	template<typename T>
	inline bool object_subscription_list<T>::on_list(T* obj)
	{
		if (std::find(this->_objects.begin(), this->_objects.end(), obj) == this->_objects.end()) return false; // if not on list
		if (std::find(this->_deletions.begin(), this->_deletions.end(), obj) != this->_deletions.end()) return false; // if deleted
		return true;
	}
	template<typename T>
	inline void object_subscription_list<T>::unsubscribe(T* obj)
	{
		if (this->_running) this->_deletions.push_back(obj);
		else this->_unsubscribe(obj);
	}

	template<typename T>
	inline void object_subscription_list<T>::clear()
	{
		this->_objects.clear();
		this->_deletions.clear();
	}
}