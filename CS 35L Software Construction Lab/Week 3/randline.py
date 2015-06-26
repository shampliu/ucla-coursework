#!/usr/bin/python

"""
Output lines selected randomly from a file

Copyright 2005, 2007 Paul Eggert.
Copyright 2010 Darrell Benjamin Carbajal.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Please see <http://www.gnu.org/licenses/> for a copy of the license.

$Id: randline.py,v 1.4 2010/04/05 20:04:43 eggert Exp $
"""

import random, sys
from optparse import OptionParser

class randline:
    def __init__(self, args):
        self.lines = [ ]
        for arg in args:
            f = open(arg, 'r')
            temp = f.readlines()
            if len(temp) == 0:
                if len(args) == 1:
                    #sys.stdout.write("Error! Argument passed in was an empty" 
                    #"file")
                    parser.error("Error! Argument was an empty file")
                else:
                    continue
            #newline check
            if temp[-1][-1] != '\n':
                temp[-1] = temp[-1] + '\n'
            self.lines.extend(temp)
            f.close()

    def chooseline(self, arg):
        if (arg == True):
            tempChoice = random.choice(self.lines)
            self.lines.remove(tempChoice)
            return tempChoice
        else:
            return random.choice(self.lines)

    def unique(self):
        uniqueLines = list(set(self.lines))
        del self.lines[:]
        self.lines = list(uniqueLines)

def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE

Output randomly selected lines from FILE."""

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-n", "--numlines",
                      action="store", dest="numlines", default=1,
                      help="output NUMLINES lines (default 1)")
    parser.add_option("-u", "--unique", 
                      action="store_true", dest="unique", default=False,
                      help="ignore duplicates when randomizing lines (default"
                      "False)")
    parser.add_option("-w", "--without-replacement",
                      action="store_true", dest="withoutReplacement", 
                      default=False, help="output lines without replacement" 
                      "(default False)")
    options, args = parser.parse_args(sys.argv[1:])

    try:
        numlines = int(options.numlines)
    except:
        parser.error("invalid NUMLINES: {0}".
                     format(options.numlines))
    if numlines < 0:
        parser.error("negative count: {0}".
                     format(numlines))
    if len(args) < 1:
        parser.error("wrong number of operands")

    try:
        generator = randline(args)
    except IOError as err:
        (errno, strerror) = err.args
        parser.error("I/O error({0}): {1}".
                     format(errno, strerror))

    if options.unique:
        generator.unique()
    
    noReplace = False
    
    if options.withoutReplacement:
        noReplace = True
        for index in range(numlines):
            output = str(generator.chooseline(noReplace))
            sys.stdout.write(output)
    else:
        for index in range(numlines):
            output = str(generator.chooseline(noReplace))
            sys.stdout.write(output)

if __name__ == "__main__":
    main()
