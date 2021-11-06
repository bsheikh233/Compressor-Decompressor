#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <cmath>
#include <bitset>
using namespace std;

struct minHeapEntry
{
    unsigned int ch;
    unsigned int freq;
    minHeapEntry* left;
    minHeapEntry* right;

    minHeapEntry()
    {

    }

    minHeapEntry(const minHeapEntry& obj)
    {
        ch = obj.ch;
        freq = obj.freq;
        left = obj.left;
        right = obj.right;
    }

    minHeapEntry(int character = 97, unsigned int frequency = 0, minHeapEntry* lc = nullptr, minHeapEntry* rc = nullptr)
    {
        ch = character;
        freq = frequency;
        left = lc;
        right = rc;
    }

    bool operator < (const minHeapEntry& obj)
    {
        return (freq < obj.freq);
    }
};
class bMinHeap {

    vector<minHeapEntry*> h;

    inline int parent(int i) {
        return i / 2;
    }

    inline int lchild(int i) {
        return 2 * i;
    }

    inline int rchild(int i) {
        return 2 * i + 1;
    }


    inline bool isLeaf(int i) {
        return lchild(i) > size();

    }

    void percolateUp(int i) {
        //O(lgn)
        while (parent(i) > 0 && (*(h[i])) < (*(h[parent(i)]))) {
            swap(h[i], h[parent(i)]);
            i = parent(i);
        }
    }


    void heapify(int i) {//percolate down
        //O(lgn)
        while (!isLeaf(i)) {
            int mini = i;//node itself is min

            //update if lchild has smaller key
            if ((*(h[lchild(i)])) < (*(h[mini])))
                mini = lchild(i);

            //update if rchild has even smaller key
            if (rchild(i) <= size() && (*(h[rchild(i)])) < (*(h[mini])))
                mini = rchild(i);

            if (i != mini) {
                swap(h[i], h[mini]);//swap ith node with the minimum of i, 2*i and 2*i+1
                i = mini;//move to one of the children
            }
            else
                break;//no violation
        }
    }
public:
    bMinHeap() {
        h.resize(1);
    }

    int size() {
        return h.size() - 1;
    }

    bool empty() {
        return size() == 0;
    }

    minHeapEntry* getMin() {
        //O(1)
        return h[1];
    }


    void insert(minHeapEntry* key) {

        h.push_back(key);//key becomes the next leaf the last level

        //now percolate up
        int i = size(); //index of the newly added node
        percolateUp(i);
    }


    void deteleMin() {
        if (size() > 0) {
            h[1] = h[size()];//overwrite root with last leaf
            h.pop_back();//remove last leaf
            heapify(1);//percolateDown(1)
        }
    }

    void Delete(minHeapEntry* ptr)
    {
        if (ptr != nullptr)
        {
            Delete(ptr->left);
            Delete(ptr->right);
            delete ptr;
        }
    }

    ~bMinHeap()
    {
        Delete(getMin());
    }
};
bool isLeaf(minHeapEntry* ptr)
{
    return ((ptr->left == nullptr) && (ptr->right == nullptr));
}
void codeGenerator(minHeapEntry* ptr, char arr[], int top, unordered_map <int, string>& umap)
{
    if (ptr->left)
    {
        arr[top] = '0';
        codeGenerator(ptr->left, arr, top + 1, umap);
    }
    if (ptr->right)
    {
        arr[top] = '1';
        codeGenerator(ptr->right, arr, top + 1, umap);
    }
    if (isLeaf(ptr))
    {
        arr[top] = '\0';
        string str(arr);
        umap.insert(make_pair(ptr->ch, str));
    }
}
void codeGenerator(minHeapEntry* ptr, unordered_map <int, string>& umap)
{
    int top = 0;
    char temp[20];
    codeGenerator(ptr, temp, top, umap);
}
void inputArray(int arr[], string fileName)
{
    ifstream fin(fileName);
    char ch[10];
    int n;

    while (true)
    {
        fin.get(ch, 9);
        if (fin.eof())
            break;
        n = stoi(ch, nullptr, 2);
        (arr[n])++;
    }
    fin.clear();
    fin.close();
}
void buildMinHeapWithHuffman(bMinHeap& minHeap, string fileName)
{
    int arr[256] = { 0 };
    inputArray(arr, fileName);
    for (int i = 0; i < 256; ++i)
    {
        if (arr[i] > 0)
        {
            minHeapEntry* temp = new minHeapEntry(i, arr[i]);
            minHeap.insert(temp);
        }
    }

    while (minHeap.size() != 1)
    {
        minHeapEntry* temp1 = minHeap.getMin();
        minHeap.deteleMin();
        minHeapEntry* temp2 = minHeap.getMin();
        minHeap.deteleMin();
        minHeapEntry* temp = new minHeapEntry(97, (temp1->freq + temp2->freq));
        temp->left = temp1;
        temp->right = temp2;
        minHeap.insert(temp);
    }
}
void storeTree(minHeapEntry* ptr, ofstream& fout)
{
    if (isLeaf(ptr))
    {
        fout << "1 " << ptr->ch << " ";
    }
    else
    {
        fout << "0 ";
        storeTree(ptr->left, fout);
        storeTree(ptr->right, fout);
    }
}
float fileCompressor(string inputFile, string outputFile)
{
    ifstream fin(inputFile);
    if (!(fin.is_open()))
    {
        return -1;
    }
    ofstream fout(outputFile);

    bMinHeap minHeap;
    buildMinHeapWithHuffman(minHeap, inputFile);
    minHeapEntry* ptr = minHeap.getMin();
    unordered_map <int, string> umap;
    int original = 0, compressed = 0;
    char ch[10];
    int n;

    codeGenerator(ptr, umap);
    storeTree(ptr, fout);
    fout << endl;
    while (true)
    {
        fin.get(ch, 9);
        original += 9;
        if (fin.eof())
            break;
        n = stoi(ch, nullptr, 2);
        fout << umap.at(n);
        compressed += (umap.at(n)).length();
    }
    fin.clear();
    fin.close();
    fout.close();
    return ((1 - (compressed / (float)original)) * 100);
}
minHeapEntry* readTree(ifstream& fin, int num)
{
    fin >> num;
    if (num == 1)
    {
        fin >> num;
        return (new minHeapEntry(num));
    }
    else
    {
        minHeapEntry* leftChild = readTree(fin, num);
        minHeapEntry* rightChild = readTree(fin, num);
        return (new minHeapEntry(97, 0, leftChild, rightChild));
    }
}
bool fileDecompressor(string inputFile, string outputFile)
{
    char ch[1000];
    unsigned char character;
    ifstream fin(inputFile);
    if (!(fin.is_open()))
    {
        return false;
    }
    ofstream fout(outputFile);
    int num = 0;

    unordered_map <int, string> umap;
    minHeapEntry* ptr = readTree(fin, num);
    num = 0;
    minHeapEntry* temp = ptr;;
    while (true)
    {
        fin >> character;
        if (fin.eof())
        {
            break;
        }
        num = character - '0';
        if (num == 0)
        {
            temp = temp->left;
        }
        else if (num == 1)
        {
            temp = temp->right;
        }
        if (isLeaf(temp))
        {
            bitset<8> bin_x(temp->ch);
            temp = ptr;
            fout << bin_x;
        }

    }
    fin.clear();
    fin.close();
    fout.close();
}

/*int main()
{
    int num;
    string s;
    while (true)
    {
        system("CLS");
        cout << ">Press 1 for Compression.\n>Press 2 for decompression.\n>Press 3 to exit." << endl;
        cin >> num;
        if (num == 3)
            break;
        cout << "Enter File Name!" << endl;
        cin >> s;
        if (num == 1)
        {
            float temp = fileCompressor(s, "abc.cmp");
            if (temp == -1)
            {
                cout << "File not found :(" << endl;
                system("pause");
            }
            else
            {
                cout << "Compression completed :)\np = " << temp << "%" << endl;
                system("pause");
            }
        }
        else if (num == 2)
        {
            if (fileDecompressor(s, "abc.8b"))
            {
                cout << "Decompression Completed :)" << endl;
                system("pause");
            }
            else
            {
                cout << "File not found :(" << endl;
                system("pause");
            }
        }
    }
    return 0;
}*/

int main()
{
    string str;
    system("CLS");
    cout << "> ";
    getline(cin, str, ' ');
    if (str == "compress" || str == "Compress")
    {
        getline(cin, str);
        float temp = fileCompressor(str, "abc.cmp");
        if (temp == -1)
        {
            cout << "File not found :(" << endl;
            system("pause");
        }
        else
        {
            cout << "Compression completed :)\np = " << temp << "%" << endl;
            system("pause");
        }
    }
    else if (str == "decompress" || str == "Decompress")
    {
        getline(cin, str);
        if (fileDecompressor(str, "abc.8b"))
        {
            cout << "Decompression Completed :)" << endl;
            system("pause");
        }
        else
        {
            cout << "File not found :(" << endl;
            system("pause");
        }
    }
    else
    {
        cout << "Invalid Command" << endl;
        system("pause");
    }

    return 0;
}