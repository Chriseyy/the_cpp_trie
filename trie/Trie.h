#ifndef SRC_TRIE_H_
#define SRC_TRIE_H_

#define	DEBUG_OFF

#include <iostream>
#include <stack>
#include <map>
#include <functional>
#include <iomanip>

using namespace std;

template <class T, class E=char>
class Trie {
public:
	typedef basic_string<E> key_type; //string=basic_string<char>
	typedef T mapped_type;
	typedef pair<const key_type, mapped_type> value_type;
	static const E end_zeichen;

private:
	class AbstrakterKnoten {
		public:
		virtual ~AbstrakterKnoten() = 0;
	};

	class InnererKnoten : public AbstrakterKnoten {
		public:
		InnererKnoten();
		virtual ~InnererKnoten();
		virtual AbstrakterKnoten* getKnoten(E key);
		virtual bool hasKnoten(E key);
		virtual void remove(E key);
		virtual bool empty();
		virtual typename map<E, AbstrakterKnoten*>::iterator begin();
		virtual typename map<E, AbstrakterKnoten*>::iterator end();

		private:
		map<E, AbstrakterKnoten*> knoten;
	};

	class BlattKnoten : public AbstrakterKnoten {
		public:
		BlattKnoten();
		virtual ~BlattKnoten();
		virtual void setValue(const value_type& value);
		virtual const value_type* getValue();

		private:
		value_type* wert;
	};

public:

	typedef pair<InnererKnoten*, typename map<E, AbstrakterKnoten*>::iterator > StackElement;
	class TrieIterator {
		typedef TrieIterator iterator;

	public:
		TrieIterator(AbstrakterKnoten* start);

		value_type operator *();
		bool operator !=(const iterator& rhs) const;
		bool operator ==(const iterator& rhs) const;
		iterator& operator ++();
		iterator operator ++(int);

	private:
		stack<StackElement> k_stack;
		BlattKnoten* blatt;
	};

	Trie();
	~Trie();

	typedef TrieIterator iterator;

	bool empty() const;
	void insert(const value_type& value);
	void erase(const key_type& value);
	void clear();

	iterator find(const key_type& testElement);
	iterator begin();
	iterator end();
	void printOn(ostream& ostr) const;

private:
	AbstrakterKnoten* start;
};

template <class T, class E>
const E Trie<T,E>::end_zeichen = '#'; // 0

template <class T, class E>
ostream& operator << (ostream& ostr, const Trie<T,E>& trie)
{
	trie.printOn(ostr);
	return ostr;
}

//**************************************************************************************************
// Implementierung Trie

template <class T, class E>
Trie<T,E>::Trie()
:	start{nullptr}
{}

template <class T, class E>
Trie<T,E>::~Trie()
{
	clear();
}

template <class T, class E>
bool Trie<T,E>::empty() const {
	return start == nullptr;
}

template <class T, class E>
void Trie<T,E>::insert(const value_type& value) {

        // Test ob key leer ist - keinen leeren Key einfügen
        if (value.first.length() < 1) {
                return;
        }

        // Trie ist leer - also erst einen Inneren Knoten anlegen
        if (start == nullptr) {
                start = new typename Trie<T,E>::InnererKnoten();
        }

        AbstrakterKnoten* akt_knoten = start;
        for (auto key_it = value.first.begin(); key_it != value.first.end(); ++key_it) {
                akt_knoten = dynamic_cast<InnererKnoten*>(akt_knoten)->getKnoten(*key_it);
        }
        akt_knoten = dynamic_cast<InnererKnoten*>(akt_knoten)->getKnoten(Trie<T,E>::end_zeichen);
        dynamic_cast<BlattKnoten*>(akt_knoten)->setValue(value);
}

template <class T, class E>
void Trie<T,E>::erase(const key_type& value) {

	// Ist der Trie oder value leer- kann nichts entfernt werden
	if (start == nullptr || value.size() < 1) {
		return;
	}

	// Suche Knoten für value und speichere diese in einem Stack
	stack<pair<E, AbstrakterKnoten*>> k_stack;
	AbstrakterKnoten* akt_knoten = start;
	for (auto key_it = value.begin(); key_it != value.end(); ++key_it) {
		bool key_exists = dynamic_cast<InnererKnoten*>(akt_knoten)->hasKnoten(*key_it);
		if (key_exists) {
			k_stack.push(pair<E, AbstrakterKnoten*>{*key_it, akt_knoten});
			akt_knoten = dynamic_cast<InnererKnoten*>(akt_knoten)->getKnoten(*key_it);
		}
		else {
			return; // Pfad nicht vorhanden
		}
	}
	bool key_exists = dynamic_cast<InnererKnoten*>(akt_knoten)->hasKnoten(Trie<T,E>::end_zeichen);
	if (!key_exists) {
		return; // Hier ist kein Blatt zum löschen
	}
	k_stack.push(pair<E, AbstrakterKnoten*>{Trie<T,E>::end_zeichen, akt_knoten});
	akt_knoten = dynamic_cast<InnererKnoten*>(akt_knoten)->getKnoten(Trie<T,E>::end_zeichen);

	// Das ist jetzt ein Blattknoten - der kann gelöscht werden
	delete akt_knoten;

	// Noch den Pfad aufräumen
	while (k_stack.size() > 0 ) {
		E key = k_stack.top().first;
		AbstrakterKnoten* knoten = k_stack.top().second;

		dynamic_cast<InnererKnoten*>(knoten)->remove(key);
		if (!dynamic_cast<InnererKnoten*>(knoten)->empty()) {
			return; // Pfad nicht weiter löschen - noch in Verwendung
		}
		delete knoten;
		k_stack.pop();
	}

	// Der ganze Trie wurde gelöscht
	start = nullptr;
}

template <class T, class E>
void Trie<T,E>::clear() {
	delete start;
	start = nullptr;
}

template <class T, class E>
typename Trie<T,E>::iterator Trie<T,E>::find(const key_type& testElement) {
        auto it = begin();
        while ( it != end()) {
                if ( (*it).first == testElement) {
                  return it;
                }
                ++it;
        }
        return it;
}

template <class T, class E>
typename Trie<T,E>::iterator Trie<T,E>::begin() {
        Trie<T,E>::iterator it(start);
        return it;
}

template <class T, class E>
typename Trie<T,E>::iterator Trie<T,E>::end() {
        Trie<T,E>::iterator it(nullptr);
        return it;
}

template <class T, class E>
void Trie<T,E>::printOn(ostream& ostr) const {

	std::function<void(ostream& ostr, Trie<T,E>::AbstrakterKnoten* start, int level)>
	printOnReku = [&printOnReku](ostream& ostr, Trie<T,E>::AbstrakterKnoten* start, int level) {
		InnererKnoten* akt_Knoten = dynamic_cast<InnererKnoten*>(start);
		for (auto key_it = akt_Knoten->begin(); key_it != akt_Knoten->end(); ++key_it){
			if (key_it->first == end_zeichen){
				auto wert = dynamic_cast<BlattKnoten*>(key_it->second)->getValue();
				ostr << setfill(' ') << setw(2*level+1) << ":" << wert->second << endl;
			}
			else{
				ostr << setfill(' ') << setw(2*level+1) << key_it->first << endl;
				printOnReku(ostr, key_it->second, level+1);
			}
		}
	};

	if (start == nullptr){
		return;
	}

	return printOnReku(ostr, start, 0);
}

//**************************************************************************************************
// Implementierung Trie::Abstrakter Knoten

template <class T, class E>
Trie<T,E>::AbstrakterKnoten::~AbstrakterKnoten()
{}

//**************************************************************************************************
// Implementierung Trie::Innerer Knoten
template <class T, class E>
Trie<T,E>::InnererKnoten::InnererKnoten()
:	knoten{}
{}

template <class T, class E>
Trie<T,E>::InnererKnoten::~InnererKnoten()
{
        for ( auto it = knoten.begin(); it != knoten.end(); ++it) {
                delete it->second;
        }
}

template <class T, class E>
typename Trie<T,E>::AbstrakterKnoten* Trie<T,E>::InnererKnoten::getKnoten(E key)
{
	AbstrakterKnoten* next = knoten[key];
	if (next == nullptr) {
		if (key == Trie<T,E>::end_zeichen) {
			next = new typename Trie<T,E>::BlattKnoten();
		}
		else {
			next = new typename Trie<T,E>::InnererKnoten();
		}
		knoten[key] = next;
	}
	return next;
}

template <class T, class E>
bool Trie<T,E>::InnererKnoten::hasKnoten(E key)
{
	return knoten.find(key) != knoten.end();
}

template <class T, class E>
void Trie<T,E>::InnererKnoten::remove(E key)
{
        knoten.erase(key);
}

template <class T, class E>
bool Trie<T,E>::InnererKnoten::empty()
{
        return knoten.empty();
}

template <class T, class E>
typename map<E, typename Trie<T,E>::AbstrakterKnoten*>::iterator Trie<T,E>::InnererKnoten::begin()
{
	return knoten.begin();
}

template <class T, class E>
typename map<E, typename Trie<T,E>::AbstrakterKnoten*>::iterator Trie<T,E>::InnererKnoten::end()
{
	return knoten.end();
}

//**************************************************************************************************
// Implementierung Trie::Blatt Knoten

template <class T, class E>
Trie<T,E>::BlattKnoten::BlattKnoten()
    :	wert{nullptr}
{}


template <class T, class E>
Trie<T,E>::BlattKnoten::~BlattKnoten()
{
	delete wert;
}

template <class T, class E>
void Trie<T,E>::BlattKnoten::setValue(const value_type& value)
{
	if (wert == nullptr) {
		wert = new typename Trie<T,E>::value_type{value};
	}
	else {
		wert->second = value.second;
	}
}

template <class T, class E>
const typename Trie<T,E>::value_type* Trie<T,E>::BlattKnoten::getValue()
{
	return wert;
}

//**************************************************************************************************
// Implementierung Trie::TreeIterator

template <class T, class E>
Trie<T,E>::TrieIterator::TrieIterator(AbstrakterKnoten* start)
:	k_stack{},
	blatt{nullptr}
{
	if (start == nullptr){
		return;  // Leerer Trie - dann ist der Blattknoten ein nullptr
	}

	InnererKnoten* akt_knoten = dynamic_cast<InnererKnoten*>(start);
	auto iterator = akt_knoten->begin();
	while(iterator->first != end_zeichen) {
		//cout << iterator->first << endl;
		k_stack.push(StackElement{akt_knoten, iterator});
		akt_knoten = dynamic_cast<InnererKnoten*>(iterator->second);
		iterator = akt_knoten->begin();
	}
	k_stack.push(StackElement{akt_knoten, iterator});
	blatt = dynamic_cast<BlattKnoten*>(iterator->second);
}

template <class T, class E>
typename Trie<T,E>::value_type Trie<T,E>::TrieIterator::operator *() {
	return *(blatt->getValue());
}

template <class T, class E>
bool Trie<T,E>::TrieIterator::operator !=(const iterator& rhs) const {
	return this->blatt != rhs.blatt;
}
template <class T, class E>
bool Trie<T,E>::TrieIterator::operator ==(const iterator& rhs) const {
        return this->blatt == rhs.blatt;
}

template <class T, class E>
typename Trie<T,E>::iterator& Trie<T,E>::TrieIterator::operator ++() {

        InnererKnoten* akt_knoten;

        do {
                akt_knoten = k_stack.top().first;
                auto iterator = ++(k_stack.top().second);
                if (iterator != akt_knoten->end() ) {
                        break;
                }
                k_stack.pop();
        } while (k_stack.size() > 0);

        if (k_stack.size() == 0) {
                blatt = nullptr;
                return *this;
        }

        auto iterator = k_stack.top().second;
        k_stack.pop();
        while(iterator->first != end_zeichen) {
                k_stack.push(StackElement{akt_knoten, iterator});
                akt_knoten = dynamic_cast<InnererKnoten*>(iterator->second);
                iterator = akt_knoten->begin();
        }
        k_stack.push(StackElement{akt_knoten, iterator});
        blatt = dynamic_cast<BlattKnoten*>(iterator->second);
        return *this;
}

template <class T, class E>
typename Trie<T,E>::iterator Trie<T,E>::TrieIterator::operator ++(int) {
        auto aktuell = *this;
        ++(*this);
        return aktuell;
}

#endif /* SRC_TRIE_H_ */

