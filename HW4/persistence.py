import sqlite3
import atexit
from dbtools import Dao
 
# Data Transfer Objects:
class Employee(object):
    #TODO: implement
    def __init__(self, id, name, salary, branche):
        self.id = id
        self.name = name
        self.salary = salary
        self.branche = branche
    
    def __str__(self):
        return str((self.id, self.name.decode('utf-8'), self.salary, self.branche))
    
 
class Supplier(object):
    #TODO: implement
    def __init__(self, id, name, contact_information):
        self.id = id
        self.name = name
        self.contact_information = contact_information
    
    def __str__(self):
        return str((self.id, self.name.decode('utf-8'), self.contact_information.decode('utf-8')))
    

class Product(object):
    #TODO: implement
    def __init__(self, id, description, price, quantity):
        self.id = id
        self.description = description
        self.price = price
        self.quantity = quantity
    
    def __str__(self):
        return str((self.id, self.description.decode('utf-8'), self.price, self.quantity))
    

class Branche(object):
    #TODO: implement
    def __init__(self, id, location, number_of_employees):
        self.id = id
        self.location = location
        self.number_of_employees = number_of_employees
    
    def __str__(self):
        return str((self.id, self.location.decode('utf-8'), self.number_of_employees))
    

class Activitie(object):
    #TODO: implement
    def __init__(self, product_id, quantity, activator_id, date):
        self.product_id = product_id
        self.quantity = quantity
        self.activator_id = activator_id
        self.date = date
    
    def __str__(self):
        return str((self.product_id, self.quantity, self.activator_id, self.date.decode('utf-8')))
    


class employees_report:
    def __init__(self, Employees_name, Employees_salary, Brnache_location, Total_sales_income):
        self.Employees_name = Employees_name
        self.Employees_salary = Employees_salary
        self.Brnache_location = Brnache_location
        self.Total_sales_income = Total_sales_income

    def __str__(self):
        return str("{} {} {} {}".format(self.Employees_name.decode('utf-8'), self.Employees_salary, self.Brnache_location.decode('utf-8'),
                                        self.Total_sales_income))


class activities_report:
    def __init__(self, Activities_date, Products_description, Activities_quantity, Employees_name, Suppliers_name):
        self.Activities_date = Activities_date
        self.Products_description = Products_description
        self.Activities_quantity = Activities_quantity
        self.Employees_name = Employees_name
        self.Suppliers_name = Suppliers_name

    def __str__(self):
        if self.Suppliers_name==None:
            return str((self.Activities_date.decode('utf-8'), self.Products_description.decode('utf-8'), self.Activities_quantity,
                    self.Employees_name.decode('utf-8'), self.Suppliers_name))
        if self.Employees_name==None:
            return str((self.Activities_date.decode('utf-8'), self.Products_description.decode('utf-8'), self.Activities_quantity,
                    self.Employees_name, self.Suppliers_name.decode('utf-8')))
 
#Repository
class Repository(object):
    def __init__(self):
        self._conn = sqlite3.connect('bgumart.db')
        self._conn.text_factory = bytes
        #TODO: complete
        self.employees = Dao(Employee,self._conn)
        self.suppliers = Dao(Supplier,self._conn)
        self.products = Dao(Product,self._conn)
        self.branches = Dao(Branche,self._conn)
        self.activities = Dao(Activitie,self._conn)
 
    def _close(self):
        self._conn.commit()
        self._conn.close()
 
    def create_tables(self):
        self._conn.executescript("""
            CREATE TABLE employees (
                id              INT         PRIMARY KEY,
                name            TEXT        NOT NULL,
                salary          REAL        NOT NULL,
                branche    INT REFERENCES branches(id)
            );
    
            CREATE TABLE suppliers (
                id                   INTEGER    PRIMARY KEY,
                name                 TEXT       NOT NULL,
                contact_information  TEXT
            );

            CREATE TABLE products (
                id          INTEGER PRIMARY KEY,
                description TEXT    NOT NULL,
                price       REAL NOT NULL,
                quantity    INTEGER NOT NULL
            );

            CREATE TABLE branches (
                id                  INTEGER     PRIMARY KEY,
                location            TEXT        NOT NULL,
                number_of_employees INTEGER
            );
    
            CREATE TABLE activities (
                product_id      INTEGER REFERENCES products(id),
                quantity        INTEGER NOT NULL,
                activator_id    INTEGER NOT NULL,
                date            TEXT    NOT NULL
            );
        """)

    
    def get_All_Activities(self):
        c = self._conn.cursor()
        all = c.execute("SELECT * FROM activities ORDER BY date").fetchall()
        return [Activitie(*row) for row in all]
    
    def get_All_Branches(self):
        c = self._conn.cursor()
        all = c.execute("SELECT * FROM branches ORDER BY id").fetchall()
        return [Branche(*row) for row in all]
    
    def get_All_Employees(self):
        c = self._conn.cursor()
        all = c.execute("SELECT * FROM employees ORDER BY id").fetchall()
        return [Employee(*row) for row in all]
    
    def get_All_Products(self):
        c = self._conn.cursor()
        all = c.execute("SELECT * FROM products ORDER BY id").fetchall()
        return [Product(*row) for row in all]
    
    def get_All_Suppliers(self):
        c = self._conn.cursor()
        all = c.execute("SELECT * FROM suppliers ORDER BY id").fetchall()
        return [Supplier(*row) for row in all]
    
    def get_employees_reports(self):
        c = self._conn.cursor()
        all = c.execute("""
                SELECT es.name, es.salary, b.location ,ifnull((SELECT ABS(SUM(a.quantity * price)) 
                FROM employees as e 
                INNER JOIN
                (products as p INNER JOIN Activities as a ON id = product_id AND a.quantity < 0) as sub 
                ON e.id = sub.activator_id and e.id = es.id),0) 
                FROM employees as es LEFT JOIN branches as b ON es.branche = b.id
                group by name
               """).fetchall()
        return [employees_report(*row) for row in all]

    def get_activities_reports(self):
        c = self._conn.cursor()
        all = c.execute("""
               SELECT Activities.date, Products.description, Activities.quantity, Employees.name, Suppliers.name 
               FROM ((Activities LEFT OUTER JOIN Products on Activities.product_id=Products.id) 
               LEFT OUTER JOIN Employees on Activities.activator_id=Employees.id) 
               LEFT OUTER JOIN Suppliers on Activities.activator_id=Suppliers.id
               ORDER BY Activities.date ASC
           """).fetchall()
        return [activities_report(*row) for row in all]

    def execute_command(self, script: str) -> list:
        return self._conn.cursor().execute(script).fetchall()
 

    def return_conn(self):
        return self._conn

                    
# singleton
repo = Repository()
atexit.register(repo._close)