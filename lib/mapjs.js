function KeyValuePair(a, b) {

    this._key = a;
    this._value = b;
}

KeyValuePair.prototype.key = function {

    return this._key;
};

KeyValuePair.prototype.value = function {

    return this._value;
};

/* 
 * Defines equality checking of two objects of the same type.
 * If objects are not of the same type, then throw an error.
 */
Object.prototype.equals = function(x) {

    for(p in this) {
        switch(typeof(this[p])) {
                case 'object':
                        if (!this[p].equals(x[p])) { return false }; break;
                case 'function':
                        if (typeof(x[p])=='undefined' || (p != 'equals' && this[p].toString() != x[p].toString())) { return false; }; break;
                default:
                        if (this[p] != x[p]) { return false; }
        }
    }

    for(p in x) {
        if(typeof(this[p])=='undefined') {return false;}
    }

    return true;
};


Array.prototype.hasNext = function {
    
    return this.length > 0;
};