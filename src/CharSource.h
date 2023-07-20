#ifndef CHARSOURCE_H
#define CHARSOURCE_H

#include <cstdlib>
#include <deque>
#include <iostream>
#include <stack>

/**
 * A source of characters coming from standard input with
 * infinite push back capability.
 */
class CharSource {
private:
	std::stack<char> pushed_char;
	std::deque<char> returned_char;
	std::istream &in;
	int nchar;		// Number of characters read
	int newlines;		// Count encountered newlines
	/**
	 * Maximum number of characters that can be pushed back, with
	 * get_before() still returning a valid value (not 0).
	 * Without this we must maintain a returned_char queue with
	 * a size equal to the file read.
	 */
	static const size_t MAX_REWIND = 10;
public:
	CharSource(std::istream &s = std::cin) : in(s), nchar(0), newlines(0) {}

	/*
	 * Obtain the next valid character from the source.
	 * Ignore (silently consume without returning them) non-ASCII characters
	 * This is required, because these cannot be correctly classified
	 * as valid identifier parts (in languages that allow them)
	 * without knowledge of the character encoding scheme in effect.
	 * On EOF return false and set c to 0.
	 */
	bool get(char &c) {
		if (pushed_char.empty()) {
			// Read, ignoring non ASCII-characters
			do {
				if (!in.get(c)) {
					c = 0;
					return false;
				}
				nchar++;
			} while (c < 0 || c > 127);
		} else {
			c = pushed_char.top();
			pushed_char.pop();
		}
		if (c == '\n')
			newlines++;
		returned_char.push_back(c);
		while (returned_char.size() > MAX_REWIND)
			returned_char.pop_front();
		return true;
	}

	/**
	 * Return current line number
	 */
	int line_number() { return newlines + 1; }

	/**
	 * Return (peek) the next character from source without removing it
	 * Return 0 on EOF.
	 */
	char char_after() {
		char c;
		if (get(c)) {
			push(c);
			return c;
		} else
			return 0;
	}

	/**
	 * Return the nth character before the one returned
	 * Return 0 if no such character is available.
	 */
	char char_before(int n = 1) {
		int index = returned_char.size() - n - 1;

		if (index >= 0)
			return returned_char[index];
		else
			return 0;
	}

	/** Return number of characters read */
	int get_nchar() const { return nchar - pushed_char.size(); }

	/**
	 * Push the specified character back into the source
	 * In effect, this is an undo of the last get, and therefore
	 * also moves back one character in the returned character queue.
	 * */
	void push(char c) {
		if (c == '\n')
			newlines--;
		pushed_char.push(c);
		if (returned_char.size() > 0)
			returned_char.pop_back();
	}
};
#endif /* CHARSOURCE_H */
