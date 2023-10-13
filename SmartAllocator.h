#pragma once
#include <iostream>
using namespace std;

class SmartAllocator  
{
	// Innere Klasse Info dient zur Speicherung von Debug Informationen
	// im zugewiesenen Speicher als zusätzlicher Header
	// Dabei muss die angeforderte Speichergröße um die Größe
	// dieses Headers erhöht werden.
	// Es können natürlich auch andere Informationen als file und line benutzt werden,
	// z.B. eine Objekt-ID, eine Zeitangabe für eine Art "Verfallsdatum" etc.
	class Info
	{
	public:
		const char* _file;
		unsigned int _line;	
		Info* _prev;
		Info* _next;
	};

public:
	// Allokiert Speicher und korrigiert die pointer
	static void* alloc ( size_t size, const char* file, unsigned int line )
	{
		++Counter;

		// Speicher besorgen
		void* raw_chunk = ::operator new ( size + sizeof(Info) );

		// Info-Objekt in das raw memory mappen
		Info* chunk = (Info*) raw_chunk;
		chunk->_file = file;
		chunk->_line = line;

		//cout << "alloc @ ";
		//printInfo(chunk);

		// Einbinden des chunk in die verkettete Liste
		link ( chunk );
		
		// pointer korrigieren und zurückgeben
		return ++chunk;
	}

	// gibt den allokierten Speicher wieder frei und korrigiert dabei
	// die pointer
	static void free ( void* Object )
	{
		if ( Object == NULL ) return;

		Info* chunk = (Info*) Object;

		// pointer korrigieren
		--chunk;

		//cout << "free @ ";
		//printInfo ( chunk );

		// chunk aus der verketteten Liste entfernen
		unlink ( chunk );

		--Counter;
		::operator delete ( chunk );
	}

	// Gibt die Anzahl der memory leaks zurück
	static inline unsigned int getLeakNumber()
	{
		return Counter;
	}

	// Gibt memory leak Informationen aus
	static void report()
	{
		cout << "Anzahl Memory leaks: " << getLeakNumber() << endl;
		for ( Info* next = _head; next != NULL; next = next->_next )
		{
			cout << "Leak Info: ";
			printInfo ( next );
		}
	}

private:
	// bindet den Knoten in eine doppelt verkettete Liste ein
	static void link ( Info* current )
	{
		// Sicherstelen, dass unbenutzte Zeiger immer auf NULL zugewiesen werden
		current->_next = NULL;
		current->_prev = NULL;

		// Erstes Element in der verketteten Liste ?
		if ( _head == NULL )
		{
			_head = current;
			_tail = current;
		}
		else
		{
			_head->_prev = current; // Verweis auf voriges Element
			current->_next = _head; // Verweis auf nächstes Element
			_head = current;
		}
	}

	// entfernt den knoten aus der verketteten liste
	static void unlink ( Info* current )
	{
		// erstes Element ?
		if ( current->_prev == NULL )
		{
			_head = current->_next;
		}
		else
		{
			current->_prev->_next = current->_next;
		}

		// letztes Element ?
		if ( current->_next == NULL )
		{
			_tail = current->_prev;
		}
		else
		{
			current->_next->_prev = current->_prev;
		}
	}

	// gibt die im Info-Objekt enthaltene Debug-information aus
	static void printInfo ( const Info* chunk )
	{
		cout << endl << "Datei: " << chunk->_file;
		cout << endl << "Zeile: " << chunk->_line << endl;
	}
	
	static unsigned int Counter;	// chunk counter
	static Info* _head;				// Head pointer der verketteten Liste
	static Info* _tail;				// tail pointer der verketteten liste
};

unsigned int SmartAllocator::Counter = 0;
SmartAllocator::Info* SmartAllocator::_head = NULL;
SmartAllocator::Info* SmartAllocator::_tail = NULL;

