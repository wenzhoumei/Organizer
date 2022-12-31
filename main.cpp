#include "tables.hpp"
int main() {
    Database db("database.db");

    db.insertAction("hello.link", "yo.com");
    struct Action a;
    if (db.getAction("hello.link", a)) {
	a.print();
    }
    return 0;
}
