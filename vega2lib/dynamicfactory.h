#ifndef DYNAMICFACTORY_H
#define DYNAMICFACTORY_H

#include <stdint.h>
#include <string>
#include <map>
#include <memory>

template<class Base>
class AbstractInstantiator
{
public:
	AbstractInstantiator()
	{

	}

	virtual ~AbstractInstantiator()
	{

	}

	virtual Base* createInstance() const = 0;

private:
	AbstractInstantiator(const AbstractInstantiator&);
	AbstractInstantiator& operator=(const AbstractInstantiator&);
};

template<class C, class Base>
class Instantiator : public AbstractInstantiator<Base>
{
public:
	Instantiator()
	{

	}

	virtual ~Instantiator()
	{

	}

	virtual Base* createInstance() const
	{
		return new C();
	}
};

template<class Base, class Type>
class DynamicFactoryIgnoreErrorPolicy
{
public:
	Base* classNotExist(const Type& /*type*/) const
	{
		return 0;
	}

	void classAlreadyRegistered(const Type& /*type*/)
	{

	}

	void classNotregistered(const Type& /*type*/)
	{

	}
};

template<class Base, class IdType = uint32_t,
	template<class, class> class DynamicFactoryErrorPolicy = DynamicFactoryIgnoreErrorPolicy >
class DynamicFactory : DynamicFactoryErrorPolicy<Base, IdType>
{
public:
	typedef AbstractInstantiator<Base> AbstractFactory;
	typedef std::map<IdType, AbstractFactory*> FactoryMap;

	DynamicFactory()
	{

	}

	~DynamicFactory()
	{
		for(typename FactoryMap::iterator iter = m_objectMap.begin(); iter != m_objectMap.end(); iter++)
		{
			delete iter->second;
		}
	}

	Base* create(const IdType& id)
	{
		typename FactoryMap::const_iterator iter = m_objectMap.find(id);
		if(iter != m_objectMap.end())
		{
			return iter->second->createInstance();
		}
		return this->classNotExist(id);
	}

	Base* create(typename FactoryMap::iterator& iter) const
	{
		return iter->second->createInstance();
	}

protected:
	template<class C>
	void add(const IdType& id)
	{
		registerClass(id, new Instantiator<C, Base>());
	}

	void remove(const IdType& id)
	{
		typename FactoryMap::iterator iter = m_objectMap.find(id);
		if(iter != m_objectMap.end())
		{
			delete iter->second;
			m_objectMap.erase(iter);
		}
		else
		{
			classNotRegistered(id);
		}
	}

	bool isExist(const IdType& id) const
	{
		return m_objectMap.find(id) != m_objectMap.end();
	}

	size_t size()
	{
		return m_objectMap.size();
	}

	typename FactoryMap::iterator begin()
	{
		return m_objectMap.begin();
	}

	typename FactoryMap::iterator end()
	{
		return m_objectMap.end();
	}

	typename FactoryMap::iterator find(const IdType& id)
	{
		return m_objectMap.find(id);
	}

	void registerClass(const IdType& id, AbstractFactory* abstractFactory)
	{
		typename FactoryMap::iterator iter = m_objectMap.find(id);
		if(iter == m_objectMap.end())
		{
			m_objectMap[id] = abstractFactory;
		}
		else
		{
			this->classAlreadyRegistered(id);
		}
	}
private:
	FactoryMap m_objectMap;
};

#endif // DYNAMICFACTORY_H
