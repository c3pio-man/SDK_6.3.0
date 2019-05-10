// create by Denis Kuzko

#ifndef PBPOOL_H
#define PBPOOL_H

#include <map>
#include <string>
#include <inklog.h>

namespace pocketbook
{

namespace utilities
{

template <typename Key, typename T>
class PBPool
{
public:
	PBPool(){ limit_ = 10 * 1024 * 1024; }

	// key - unique key for pool map
	// owner - name of object, which try to get element from pool
	// wait - if true
	//				then wait element while it will be free
	//			  else
	//				return element or NULL (if it not free)
	//	If element is free
	//		then return pool element
	//	else
	//		wait it or return NULL
	// If no element with current "key", pool create it
	// IMPORTANT: if you get element, you should return it by function "put"
	T* get(Key key, const std::string &owner, bool wait = true);

	// function for return element
	bool put(T* value);

	// remove element from pool by key
	bool remove(Key key);

	// set max memory use by pools elements
	void set_limit(size_t limit) {limit_ = limit; optimize(); };

	size_t get_limit() { return limit_; }

	// optimize pool, remove most unused element in pool if pool size is more than limit
	void optimize();
private:
	std::map<Key, T*> pool_;
	size_t limit_;
};

template <typename Key, typename T>
T* PBPool<Key, T>::get(Key key, const std::string &owner, bool wait)
{
	T* res = NULL;
	typename std::map<Key, T*>::iterator it = pool_.find(key);
	if (it != pool_.end())
	{
		if ((*it).second->getOwner().empty())
		{
			res = (*it).second;
			res->setOwner(owner);
			res->access();
		}
		else if (wait)
		{
			unsigned int counter = 1;
			const unsigned int time_info = 1000;
			const unsigned int time_warn = 5 * time_info;
			const unsigned int time_error = 15 * time_info;
			while (!(*it).second->getOwner().empty())
			{
				usleep(1000);
				if (counter % time_error == 0)
					PB_ERROR("Wait object in pool %d ms: wait owner = [%s] current owner = [%s]", counter, owner.c_str(), (*it).second->getOwner().c_str());
				else if (counter % time_warn == 0)
					PB_WARN("Wait object in pool %d ms: wait owner = [%s] current owner = [%s]", counter, owner.c_str(), (*it).second->getOwner().c_str());
				else if (counter % time_info == 0)
					PB_INFO("Wait object in pool %d ms: wait owner = [%s] current owner = [%s]", counter, owner.c_str(), (*it).second->getOwner().c_str());
				counter++;
			}
			res = (*it).second;
			res->setOwner(owner);
			res->access();
		}
	}
	else
	{
		res = new T();
		res->setKey(key);
		res->setOwner(owner);
		res->init();
		pool_[key] = res;
		optimize();
	}
	return res;
}

template <typename Key, typename T>
bool PBPool<Key, T>::put(T *value)
{
	bool res = false;
	if (value)
	{
		typename std::map<Key, T*>::iterator it = pool_.find(value->getKey());
		if (it != pool_.end() && value == (*it).second)
		{
			(*it).second->setOwner("");
			res = true;
		}
	}
	return res;
}

template <typename Key, typename T>
bool PBPool<Key, T>::remove(Key key)
{
	bool res = false;
	if (get(key, "SELF", false))
	{
		pool_[key]->clear();
		delete pool_[key];
		pool_.erase(key);
	}
	return res;
}

template <typename Key, typename T>
void PBPool<Key, T>::optimize()
{
	size_t size = 0;
	time_t access;
	Key access_id;
	bool valid = false;

	while (size == 0 || size >= limit_)
	{
		size = 0;
		access = time(NULL);
		valid = false;
		for (typename std::map<Key, T*>::iterator it = pool_.begin(); it != pool_.end(); it++)
		{
			T *item = (*it).second;
			size += item->size();
			if (item->getOwner().empty() && access > item->lastAccess())
			{
				access = item->lastAccess();
				access_id = item->getKey();
				valid = true;
			}
		}
		if (size >= limit_)
		{
			if (valid)
			{
				if (get(access_id, "SELF", false))
				{
					size -= pool_[access_id]->size();
					pool_[access_id]->clear();
					delete pool_[access_id];
					pool_.erase(access_id);
				}
			}
		}
		if (size <= 0 || !valid)
			break;
	}
	PB_TRACE("use memory size = %d", size);
}

// Template class element for PBPool
// To use pool, inherit from it and implement abstract functions
template <typename Key>
class PBPoolElement
{
public:
	PBPoolElement() { access_ = time(NULL); owner_ = ""; }
	virtual ~PBPoolElement() {};

	// return unique key, that setted in pool when getted this object
	Key getKey() const { return key_; }

private:
	// init object. calling by pool after create this object
	virtual void init() = 0;
	// clear memory. calling by pool before delete this object
	virtual void clear() = 0;
	// memory use size
	virtual size_t size() const = 0;

	void setOwner(const std::string &owner){ owner_ = owner; }
	std::string getOwner() const { return owner_; }

	void setKey(Key key) { key_ = key; }
	void access() { access_ = time(NULL); }
	time_t lastAccess() const { return access_; }

	std::string owner_;
	time_t access_;
	Key key_;

	template<typename KeyPool, typename T>
	friend class PBPool;
};

/* Usage example
 *
 * class Element : public PBPoolElement<int>
 * {
 *	public:
 *		Element() { value_ = 0; };
 *		virtual void init() {};
 *		virtual void clear() {};
 *		virtual size_t size() const { return 0; }
 *		int value_;
 * }
 *
 * PBPool<int, Element<int> > pool;
 *
 * int main(void)
 * {
 *		Element<int> *element = pool.get(1, "main");
 *		element->value_ = 5;
 *		pool.put(element);
 *		return 0;
 * }
*/

} // utilities

} // pocketbook

#endif // PBPOOL_H
