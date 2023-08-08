from persistence import *
import sqlite3


def print_Activities():
    mylist = repo.get_All_Activities()
    print("\nActivities")
    for item in mylist:
        print(item)


def print_Branches():
    mylist = repo.get_All_Branches()
    print("Branches")
    for item in mylist:
        print(item)


def print_Employees():
    mylist = repo.get_All_Employees()
    print("Employees")
    for item in mylist:
        print(item)


def print_Products():
    mylist = repo.get_All_Products()
    print("Products")
    for item in mylist:
        print(item)


def print_Suppliers():
    mylist = repo.get_All_Suppliers()
    print('Suppliers')
    for item in mylist:
        print(item)


def print_Employees_report():
    mylist = repo.get_employees_reports()
    print("Employees report")
    for item in mylist:
        print(item)


def print_Activities_report():
    mylist = repo.get_activities_reports()
    print("Activities report")
    if mylist!=[]:
        for item in mylist:
            print(item)

def main():
    #TODO: implement
    print_Activities()
    print_Branches()
    print_Employees()
    print_Products()
    print_Suppliers()
    print()
    print_Employees_report()
    print()
    print_Activities_report()
    

if __name__ == '__main__':
    main()