#ifndef OBJECT_H_
#define OBJECT_H_

#include <string>
#include <list>

class Object
{
	public:
		Object();
		virtual ~Object();

		virtual void setName(std::string name);
		virtual void setID(unsigned int id);
		virtual void setActive(bool value);
		virtual bool isActive();
		virtual std::string& getName();
		virtual unsigned int& getID();

	protected:
		bool active;
		unsigned int id;
		std::string name;
};

#endif /*OBJECT_H_*/
