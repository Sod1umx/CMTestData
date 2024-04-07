#include <windows.h>
#include <cstdio>
#include <cmath>
#include <iostream>
using namespace std;

typedef unsigned int uint8;

struct octNode
{
	long long cnt = 1;
	long long rSum = 0, gSum = 0, bSum = 0;//下面所有结点值的和
	bool isLeaf = 0;//是否是叶子结点
	int depth = 0;//本节点的深度
	octNode* child[8] = { NULL };//8个子结点的指针数组
};

class octTree
{
public:
	octTree() {};
	octTree(int maxColorNum) { maxColors = maxColorNum; }
	~octTree();

	void insertColor(uint8 r, uint8 g, uint8 b);						//插入一个颜色
	//uint8 generatePalette(RGBQUAD* pal);						//生成调色板
private:
	octNode* root;														//八叉树的根
	int colors;															//当前的颜色总数
	int maxColors;														//最大颜色数
	//vector<vector<octNode*>> allNode;

	//void init_allNode(); //初始化
	void D_to_B(int* res, int a);								//十进制转二进制，0-255
	void reduceNode(octNode* _root, int depth, int max_colors);
};

//释放八叉树的内存空间
octTree::~octTree()
{
	//To do....

}

void octTree::D_to_B(int* res, int a)
{
	int divisor = 128;
	int ptr = 0;
	while (divisor != 0)
	{
		int tmp = a / divisor;
		a = a % divisor;
		divisor = divisor / 2;
		res[ptr++] = tmp;
	}
}

void octTree::insertColor(uint8 r, uint8 g, uint8 b)
{
	/*转int和二进制*/
	int _r = r, _g = g, _b = b;
	int r_2[10], g_2[10], b_2[10];
	D_to_B(r_2, _r);
	D_to_B(g_2, _g);
	D_to_B(b_2, _b);
	/*建立索引值数组*/
	int key[10];
	for (int i = 0; i < 8; i++)
	{
		key[i] = r_2[i] * 4 + g_2[i] * 2 + b_2[i];
	}
	/*若树空，建立根*/
	if (this->root == NULL)
	{
		octNode* newroot = new octNode;
		newroot->cnt = 0;
		root = newroot;
	}
	/*寻找插入位置*/
	octNode* _root = this->root;
	int layer = 0;
	while (layer < 8 && _root->child[key[layer]] != NULL)
	{
		_root->cnt++;
		_root->rSum += _r; _root->gSum += _g; _root->bSum += _b;
		_root = _root->child[key[layer++]];
	}
	_root->cnt++;
	_root->rSum += _r; _root->gSum += _g; _root->bSum += _b;
	if (layer == 8) //遍历到leaf，已经存在这种颜色
	{

	}
	/*若不存在，建立新分支*/
	else
	{
		if (_root->isLeaf) //遇到了不在8层的叶子结点
		{
			_root->isLeaf = false;
		}
		for (layer; layer < 7; layer++)
		{
			octNode* newnode = new octNode;
			newnode->rSum = _r; newnode->gSum = _g; newnode->bSum = _b;
			newnode->depth = layer + 1;
			_root->child[key[layer]] = newnode;
			_root = newnode;
		}
		/*建立叶子结点*/
		octNode* leafnode = new octNode;
		leafnode->isLeaf = 1;
		leafnode->rSum = _r; leafnode->gSum = _g; leafnode->bSum = _b;
		leafnode->depth = layer + 1;
		_root->child[key[layer]] = leafnode;
		this->colors++; //颜色数增加
	}
	_root = root;
	reduceNode(root, 7, 1);
}

void octTree::reduceNode(octNode* _root, int depth, int max_colors)
{
	if (this->colors <= max_colors)return;
	if (_root == NULL)return;
	if (_root->depth < depth)
	{
		for (int i = 0; i < 8; i++)
		{
			reduceNode(_root->child[i], depth, max_colors);
			if (this->colors <= max_colors)break;
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			octNode* n = _root->child[i];
			if (n == NULL)continue;
			colors--;
			delete _root->child[i];
			_root->child[i] = NULL;
		}
		_root->isLeaf = 1;
		colors++;
	}
}

int main()
{
	octTree* octt = new octTree;
	octt->insertColor(1, 1, 0);
	octt->insertColor(1, 1, 1);
	octt->insertColor(1, 1, 1);
}
