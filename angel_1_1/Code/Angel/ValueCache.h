#ifndef VALUECACHE__H
#define VALUECACHE__H

#define CREATE_VALUECACHE_FUNCTOR( type, objClass, instance, method ) \
	new ValueCache<type, objClass>::Functor<type,objClass>(instance, &objClass::method)

template<class T, class C>
class ValueCache
{
public:

	template<class T, class C>
	class Functor
	{
	public:

		Functor( C* obj, void(C::*fpt)( T& ) )
		{
			_obj = obj;
			_delegate = fpt;
		}

		void operator()(T& val)
		{
			if( _obj != NULL)
				(*_obj.*_delegate)(val);
		}
	private:
		C* _obj;
		void (C::*_delegate)( T& );
	};

	ValueCache(Functor<T,C>* del)
	{
		_dirty = true;
		_delegate = del;
	}

	~ValueCache()
	{
		if( _delegate != NULL)
			delete _delegate;
	}

	T& Value()
	{
		if (_dirty)
		{
			(*_delegate)( _cachedValue);
			_dirty = false;
		}
		return _cachedValue;
	}

	void Dirty()
	{
		_dirty = true;
	}

private:
	T _cachedValue;
	bool _dirty;
	Functor<T,C>* _delegate;
};

#endif // VALUECACHE__H