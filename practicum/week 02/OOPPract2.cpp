// OOPPract2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <fstream>
#include <iostream>

namespace constants
{
    const int maxItemsInArray = 20U;
    const int maxVectors = 50U;
}
namespace filenames
{
    const char fileNameOne[] = "file1.txt";
    const char fileNameTwo[] = "file2.txt";
    const char numbersFileName[] = "numbers.txt";
    const char oddFileName[] = "odd.txt";
    const char evenFileName[] = "even.txt";
    const char vectorFileName[] = "vectors.txt";
};


typedef struct vector
{
    int x = 0;
    int y = 0;
    int z = 0;
};

typedef struct vectorSpace
{
    size_t currentIndex = 0U;
    vector vectors[50] = { {0,0,0} };
};

enum DeviceType { TV, SMARTPHONE, LAPTOP, PC, CAMERA };

struct Device
{
    char name[32];
    int quantity;
    double price;
    DeviceType type;
    char supplier[32];
};

struct Warehouse
{
    char owner[32];
    size_t deviceCount = 0;
    Device devices[100];
};

Device createDevice(const char* name, int quantity, double price, DeviceType type, const char* supplier)
{
    Device device;
    strncpy(device.name, name, 32);
    device.quantity = quantity;
    device.price = price;
    device.type = type;
    strncpy(device.supplier, supplier, 32);
    return device;
}

Warehouse createWarehouse(const char* owner)
{
    Warehouse warehouse;
    strncpy(warehouse.owner, owner, 32);
    warehouse.deviceCount = 0;
    return warehouse;
}

void addDeviceToWarehouse(Warehouse& warehouse, const Device& device)
{
    if (warehouse.deviceCount < 100)
    {
        warehouse.devices[warehouse.deviceCount++] = device;
    }
}

void sortDevicesByPrice(Warehouse& warehouse)
{
    for (size_t i = 0; i < warehouse.deviceCount - 1; i++)
    {
        for (size_t j = 0; j < warehouse.deviceCount - i - 1; j++)
        {
            if ((warehouse.devices[j].price > warehouse.devices[j + 1].price) ||
                (warehouse.devices[j].price < warehouse.devices[j + 1].price))
            {
                Device temp = warehouse.devices[j];
                warehouse.devices[j] = warehouse.devices[j + 1];
                warehouse.devices[j + 1] = temp;
            }
        }
    }
}

void sortDevicesByQuantity(Warehouse& warehouse)
{
    for (size_t i = 0; i < warehouse.deviceCount - 1; i++)
    {
        for (size_t j = 0; j < warehouse.deviceCount - i - 1; j++)
        {
            if (warehouse.devices[j].quantity > warehouse.devices[j + 1].quantity)
            {
                Device temp = warehouse.devices[j];
                warehouse.devices[j] = warehouse.devices[j + 1];
                warehouse.devices[j + 1] = temp;
            }
        }
    }
}

Device findMostExpensiveDevice(const Warehouse& warehouse)
{
    Device mostExpensive = warehouse.devices[0];
    for (size_t i = 1; i < warehouse.deviceCount; i++)
    {
        if (warehouse.devices[i].price > mostExpensive.price)
        {
            mostExpensive = warehouse.devices[i];
        }
    }
    return mostExpensive;
}

Device findCheapestDevice(const Warehouse& warehouse)
{
    Device cheapest = warehouse.devices[0];
    for (size_t i = 1; i < warehouse.deviceCount; i++)
    {
        if (warehouse.devices[i].price < cheapest.price)
        {
            cheapest = warehouse.devices[i];
        }
    }
    return cheapest;
}
void printWarehouseInfo(const Warehouse& warehouse)
{
    std::cout << "Warehouse Owner: " << warehouse.owner << '\n';
    std::cout << "Devices in stock: " << warehouse.deviceCount << '\n';
    for (size_t i = 0; i < warehouse.deviceCount; i++)
    {
        std::cout << warehouse.devices[i].name << " - " << warehouse.devices[i].quantity << " units - $"
            << warehouse.devices[i].price << " - Supplier: " << warehouse.devices[i].supplier << '\n';
    }
}

void serializeVector(std::ofstream& ofs, const vector& vec)
{
    if (!ofs.is_open())
    {
        return;
    }
    ofs << "<" << vec.x << ", " << vec.y << ", " << vec.z << ">" << '\n';
}

vector deserializeVector(std::ifstream& ifs)
{
    vector vec = { 0,0,0 };
    if (ifs.is_open())
    {
        char discard;
        ifs >> discard >> vec.x >> discard >> vec.y >> discard >> vec.z >> discard;
    }
    return vec;
}

void serializeVectorSpace(const char* fileName, const vectorSpace& vectorSpace)
{
    std::ofstream ofs(fileName);
    if (!ofs.is_open()) return;
    ofs << vectorSpace.currentIndex << '\n';
    for (size_t i = 0; i < vectorSpace.currentIndex; i++)
    {
        serializeVector(ofs, vectorSpace.vectors[i]);
    }
}

vectorSpace deserializeVectorSpace(const char* fileName)
{
    vectorSpace vSpace;
    std::ifstream ifs(fileName);
    if (!ifs.is_open()) return vSpace;

    ifs >> vSpace.currentIndex;
    for (size_t i = 0; i < vSpace.currentIndex; i++)
    {
        vSpace.vectors[i] = deserializeVector(ifs);
    }
    return vSpace;
}


void makeVector(int x, int y, int z)
{
    vector newVector = { x,y,z };
}

int taskOne()
{
    std::ifstream readFromFirstFile(filenames::fileNameOne);
    if (!readFromFirstFile.is_open())
    {
        return -1;
    }
    int a = 0, b = 0;
    readFromFirstFile >> a >> b;
    readFromFirstFile.close();
    std::ofstream writeFromFirstFile(filenames::fileNameTwo);
    if (!writeFromFirstFile.is_open())
    {
        return -2;
    }
    writeFromFirstFile << a + b << ' ' << a * b;
    return 0;
}
int taskTwo(const char* numbersFileName)
{
    std::ifstream numberStream(numbersFileName);
    if (!numberStream.is_open())
    {
        return -1;
    }
    int arrayOfEvenNumbers[constants::maxItemsInArray];
    int arrayOfOddNumbers[constants::maxItemsInArray];
    size_t indexForEven = 0;
    size_t indexForOdd = 0;
    int tempNumber = 0;

    while (numberStream >> tempNumber)
    {
        if (tempNumber % 2 == 0 && indexForEven < constants::maxItemsInArray)
        {
            arrayOfEvenNumbers[indexForEven++] = tempNumber;
        }
        else if (indexForOdd < constants::maxItemsInArray)
        {
            arrayOfOddNumbers[indexForOdd++] = tempNumber;
        }
    }
    numberStream.close();

    std::ofstream ostreamForEven(filenames::evenFileName);
    if (!ostreamForEven.is_open())
    {
        return -2;
    }
    for (size_t i = 0; i < indexForEven; i++)
    {
        ostreamForEven << arrayOfEvenNumbers[i] << '\n';
    }
    ostreamForEven.close();

    std::ofstream ostreamForOdd(filenames::oddFileName);
    if (!ostreamForOdd.is_open())
    {
        return -3;
    }
    for (size_t i = 0; i < indexForOdd; i++)
    {
        ostreamForOdd << arrayOfOddNumbers[i] << '\n';
    }
    ostreamForOdd.close();

    return 0;
}

int task3()
{
    vectorSpace vSpace = vectorSpace();
    vector v1 = { 1, 2 ,3 };
    makeVector(1, 2, 3);
    std::ofstream vectorOstream(filenames::vectorFileName);
    if(!vectorOstream.is_open())
    {
        return -1;
    }

    serializeVector(vectorOstream, v1);
    serializeVector(vectorOstream, v1);

    vectorOstream.close();
    
    std::ifstream vectorIstream(filenames::vectorFileName);
    if (!vectorIstream.is_open())
    {
        return -1;
    }
    deserializeVector(vectorIstream);
    serializeVectorSpace(filenames::vectorFileName,vSpace);
    /*
    deserializeVectorSpace(filenames::vectorFileName);*/

    return 0;
}
int task4()
{
    Warehouse warehouse = createWarehouse("John Doe");

    addDeviceToWarehouse(warehouse, createDevice("Laptop", 10, 1500.0, LAPTOP, "Supplier A"));
    addDeviceToWarehouse(warehouse, createDevice("Smartphone", 20, 800.0, SMARTPHONE, "Supplier B"));
    addDeviceToWarehouse(warehouse, createDevice("Camera", 5, 1200.0, CAMERA, "Supplier C"));

    std::cout << "Before sorting:\n";
    printWarehouseInfo(warehouse);

    sortDevicesByPrice(warehouse);
    std::cout << "\nAfter sorting by price\n";
    printWarehouseInfo(warehouse);

    Device expensive = findMostExpensiveDevice(warehouse);
    Device cheap = findCheapestDevice(warehouse);

    std::cout << "\nMost Expensive Device: " << expensive.name << " - $" << expensive.price << '\n';
    std::cout << "Cheapest Device: " << cheap.name << " - $" << cheap.price << '\n';
}
int main()
{
    //task 1
    if (taskOne() == 0)
    {
        std::cout << "T1 works fine\n";
    };
    if (taskTwo(filenames::numbersFileName) == 0) 
    {
        std::cout << "T2 works fine\n";
    };
    if (task3() == 0)
    {
        std::cout << "T3 works fine\n";
    };
    //Couldn't test it cuz strncpy is unsafe
    if (task4() == 0)
    {
        std::cout << "T4 works fine\n";
    };
    return 0;
    std::cout << "\nHello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
