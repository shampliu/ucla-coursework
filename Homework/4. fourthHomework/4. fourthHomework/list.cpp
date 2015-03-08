//
//  list.cpp
//  4. fourthHomework
//
//  Created by Chang Liu on 2/13/15.
//  Copyright (c) 2015 Chang Liu. All rights reserved.
//

void listAll(const MenuItem* m, string path) // two-parameter overload
{
    const vector<MenuItem*>* c = m->menuItems();
    if (c == nullptr) {
        return;
    }
    for (vector<MenuItem*>::const_iterator it = c->begin(); it != c->end(); it++) {
        cout << path + (*it)->name() << endl;
        listAll(*it, path + (*it)->name() + '/');
        
    }
}