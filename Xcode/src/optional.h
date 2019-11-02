//
// Local version of std::<optional>
//	Apple-specific kludge/substitute
//
// Necessitated by Xcode's anemic support for c++17
//	and latest Standard Library features, namely:
//	<experimental/optional>
//	which seems forever-"experimental," doesn't
//	provide has_value(), etc.  C'mon Apple, really?
//
// Created 3/7/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//

template <typename T>
struct optional {
	T		theValue;
	bool	isAssigned = false;
	void operator = (const T &V) {
		theValue = V;
		isAssigned = true;
	}
	bool operator != (const optional &U) {
		return isAssigned && U.isAssigned && theValue != U.theValue;
	}
	bool has_value() {
		return isAssigned;
	}
	uint32_t value() {
		//if (!isAssigned) throw runtime_error("undefined access to unassigned optional");
		return isAssigned ? theValue : 0xDEADBEEF;
	}
};
