#pragma once

#include<vector>
#include<fstream>
#include<string>

#include<SFML\Graphics.hpp>

#include"Reader.hpp"

class Item
{
public:
	enum class Type : unsigned char
	{
		None,
		Food,
		Weapon,
		Armor,
		SpecialKey
	};
public:
	Item() {}
	Item(const TypeID ID)
	{
		LoadAtID(ID);
	}
	Item(const Package::ItemP& pac)
	{
		SetPackage(pac);
	}
	Item(const Item& other) :
		ID{other.ID},
		type{other.type},
		name{other.name},

		borodaPls{other.borodaPls},
		healthPls{other.healthPls},
		damagePls{other.damagePls},
		arrmorPls{other.arrmorPls},

		useInf{other.useInf},
		seeInf{other.seeInf},
		brkInf{other.brkInf}
	{}

	Item& operator=(const Item& other)
	{
		if (this == &other)
			return *this;
		ID = other.ID;
		type = other.type;
		name = other.name;

		borodaPls = other.borodaPls;
		healthPls = other.healthPls;
		damagePls = other.damagePls;
		arrmorPls = other.arrmorPls;

		useInf = other.useInf;
		seeInf = other.seeInf;
		brkInf = other.brkInf;
		return *this;
	}

	TypeID GetID() const
	{
		return ID;
	}
	Type GetType() const
	{
		return type;
	}
	const sf::String GenName() const
	{
		return name;
	}

	const sf::String GetSee() const
	{
		return seeInf;
	}
	const sf::String GetUse() const
	{
		return useInf;
	}
	const sf::String GetBrk() const
	{
		return brkInf;
	}

	const int& GetBorodaPlus() const
	{
		return borodaPls;
	}
	const int& GetHealtpPlus() const
	{
		return healthPls;
	}
	const int& GetDamagePlus() const
	{
		return damagePls;
	}
	const int& GetArrmorPlus() const
	{
		return arrmorPls;
	}

	void LoadAtID(const TypeID newID)
	{
		SetPackage(ItemReader::ReadItem(newID));
	}

	void SetPackage(const Package::ItemP& pac)
	{
		ID = pac.ID;
		type = static_cast<Type>(pac.type);
		name = pac.name;

		borodaPls = pac.Boroda;
		healthPls = pac.HP;
		damagePls = pac.Damage;
		arrmorPls = pac.Armor;

		useInf = pac.useInfo;
		seeInf = pac.info;
		brkInf = pac.breakInfo;
	}

private:
	TypeID ID = 0;
	Type type = Type::None;
	sf::String name;

	int borodaPls = 0;
	int healthPls = 0;
	int damagePls = 0;
	int arrmorPls = 0;

	sf::String useInf;
	sf::String seeInf;
	sf::String brkInf;
};

class Inventory
{
public:
	Inventory(unsigned char maxSize = 10)
		: maxItems{ maxSize }
	{}

	const Item& GetArmor() const
	{
		return currentArmor;
	}
	const Item& GetWeapon() const
	{
		return currentWeapon;
	}

	#pragma region Array methods

	unsigned char MaxElement() const
	{
		return maxItems;
	}
	unsigned char GetSize() const
	{
		return static_cast<unsigned char>(items.size());
	}

	const Item& GetAt(unsigned char ind) const
	{
		return items[ind];
	}

	bool Add(const Item& newItem)
	{
		if (items.size() >= maxItems)
			return false;
		items.push_back(newItem);
		return true;
	}

	void PopAt(unsigned char ind)
	{
		if (ind >= items.size())
			return;
		items.erase(items.begin() + ind, items.begin() + ind + 1);
	}

	#pragma endregion

	#pragma region Use See Break

	sf::String TrashAt(unsigned char ind)
	{
		if (ind >= items.size())
			return "";
		auto res = items[ind].GetBrk();
		items.erase(items.begin() + ind, items.begin() + ind + 1);
		return res;
	}

	sf::String UseAt(unsigned char ind, int& hp, int& boroda, int& damage, int& armor)
	{
		if (ind >= items.size())
			return "";
		sf::String res = items[ind].GetUse();
		
		hp += items[ind].GetHealtpPlus();
		boroda += items[ind].GetBorodaPlus();
		damage += items[ind].GetDamagePlus();
		armor += items[ind].GetArrmorPlus();

		if (items[ind].GetType() == Item::Type::Armor)
		{
			std::swap(currentArmor, items[ind]);
			if (items[ind].GenName().isEmpty())
				items.erase(items.begin() + ind, items.begin() + ind + 1);
		}
		else if (items[ind].GetType() == Item::Type::Weapon)
		{
			std::swap(currentWeapon, items[ind]);
			std::swap(currentArmor, items[ind]);
			if (items[ind].GenName().isEmpty())
				items.erase(items.begin() + ind, items.begin() + ind + 1);
		}
		else
			items.erase(items.begin() + ind, items.begin() + ind + 1);
		return res;
	}

	sf::String SeeAt(unsigned char ind) const
	{
		if (ind >= items.size())
			return "";
		return items[ind].GetSee();
	}

	#pragma endregion

	Inventory& operator=(const Inventory& other)
	{
		if (this == &other)
			return *this;
		maxItems = other.maxItems;
		items = other.items;
		currentArmor = other.currentArmor;
		currentWeapon = other.currentWeapon;
		return *this;
	}

private:
	unsigned char maxItems = 10;
	std::vector<Item> items;

	Item currentArmor;
	Item currentWeapon;

};

//class ItemBase
//{
//public:friend class Item;
//	ItemBase() {}
//	ItemBase(const sf::String& name, const sf::String& use, const sf::String& see, const sf::String& brk) :
//		name{ name }, txtAfterUse{ use }, txtAfterSee{ see }, txtAfterBrk{ brk } {}
//
//	const sf::String& GetName() const
//	{
//		return name;
//	}
//
//	const sf::String& InfoAfterUse() const
//	{
//		return txtAfterUse;
//	}
//	const sf::String& InfoAfterSee() const
//	{
//		return txtAfterSee;
//	}
//	const sf::String& InfoAfterBrk() const
//	{
//		return txtAfterBrk;
//	}
//
//protected:
//	unsigned short ID = 0;
//	sf::String name = L"����������� �������";
//
//	sf::String txtAfterUse = L"�� ������������ ����������� �������. ������ �� ���������.";
//	sf::String txtAfterSee = L"��� ����������� �������. �� �� ������ ��� ������.";
//	sf::String txtAfterBrk = L"�� ��������� ����������� �������. � ��������� �������.";
//};
//
//class Weapon : public ItemBase
//{
//public:friend class Item;
//	Weapon() : ItemBase{} {}
//	Weapon(const Weapon& v) : ItemBase{ v.name, v.txtAfterUse, v.txtAfterUse, v.txtAfterBrk }, damagePlus{ v.damagePlus } {}
//
//	int GetDamage() const
//	{
//		return damagePlus;
//	}
//
//private:
//	int damagePlus = 0;
//};
//
//class Armor : public ItemBase
//{
//public:friend class Item;
//	Armor() : ItemBase{} {}
//	Armor(const Armor& v) : ItemBase{ v.name, v.txtAfterUse, v.txtAfterUse, v.txtAfterBrk }, armorPlus{ v.armorPlus } {}
//
//	int GetArmor() const
//	{
//		return armorPlus;
//	}
//
//private:
//	int armorPlus = 0;
//};
//
//class Food : public ItemBase
//{
//public:friend class Item;
//	Food() : ItemBase{} {}
//	Food(const Food& v) : ItemBase{ v.name, v.txtAfterUse, v.txtAfterUse, v.txtAfterBrk }, hpPlus{ v.armohpPlusrPlus }, borodaPlus {}
//
//	int GetHP() const
//	{
//		return hpPlus;
//	}
//
//	int GetBorodaPlus() const
//	{
//		return borodaPlus;
//	}
//
//private:
//	int hpPlus = 0;
//	int borodaPlus = 0;
//};
//
//
//class Item
//{
//public:
//	enum class Type : unsigned char
//	{
//		None,
//		Food,
//		Armor,
//		Weapon
//	};
//public:
//	Item()
//	{
//		value = new ItemBase;
//	}
//	Item(const Food& val)
//	{
//		value = new Food{};
//	}
//
//	~Item()
//	{
//		if (value != nullptr)
//			delete value;
//		value = nullptr;
//	}
//
//private:
//	ItemBase* value = nullptr;
//	Type type = Type::None;
//};
