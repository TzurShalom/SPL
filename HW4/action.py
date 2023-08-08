from persistence import *

import sys

def main(args : list):
    inputfilename : str = args[1]
    with open(inputfilename) as inputfile:
        for line in inputfile:
            splittedline : list[str] = line.strip().split(", ")
            #TODO: apply the action (and insert to the table) if possible

            if repo.products.getquantity(splittedline[0]) + int(splittedline[1]) >= 0:
                repo.activities.insert(Activitie(splittedline[0], splittedline[1], splittedline[2], splittedline[3]))
                repo.products.updatequantity(splittedline[0], splittedline[1])

if __name__ == '__main__':
    main(sys.argv)