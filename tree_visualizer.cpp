#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

struct Node
{
	int val, height, width;
	vector <Node *> child;
};

void input (Node *&, int &);
void visualize_straight (Node *, int, long long &, bool);
void visualize_inclined (Node *, int, long long &, bool, int &);
void visualize_tree (Node *, int, int, int, char **);
void initTree (Node *);
void center (int, char **, int, int, int);
int getWidth (int);
void del (Node *&);

int main()
{
	Node * tree = new Node;
	int style = 0;
	input(tree, style);
	long long stat = 0;
	if (style == 1)
		visualize_straight(tree, 0, stat, 0);
	else if (style == 2)
	{
		int lines = 0;
		visualize_inclined(tree, 0, stat, 0, lines);
	}
	else if (style == 3)
	{
		//Init
		initTree(tree);
		char ** res = new char *[tree -> height];
		for (int i = 0; i < tree -> height; i++)
		{
			res[i] = new char [tree -> width + 1];
			for (int j = 0; j < tree -> width; j++)
				res[i][j] = ' ';
			res[i][tree -> width] = '\0';
		}

		visualize_tree(tree, 0, 0, tree -> width - 1, res);

		cout << "\n[INFO] Tree:\n\n";
		for (int i = 0; i < tree -> height; i++)
			cout << res[i] << "\n";
		cout << "\n";
	
		//Delete
		for (int i = 0; i < tree -> height; i++)
			delete [] res[i];
		delete [] res;
		cout << "[INFO] res[][] deleted\n";
	}
	del(tree);
	cout << "[INFO] Tree deleted\n";
}

void tab (int l)
{
	for (int i = 0; i < l; i++) cout << " ";
};

void visualize_straight (Node *tree, int depth, long long &stat, bool isLast)
{
	if (!tree)
		return;
	for (int i = 0; i <= depth; i++)
		if ((stat >> i) & 1)
			cout << "|  ";
		else
			cout << "   ";
	cout << "\n";

	for (int i = 0; i < depth - 1; i++)
		if ((stat >> i) & 1)
			cout << "|  ";
		else
			cout << "   ";

	if (depth)
		cout << "|__";
	cout << tree -> val << "\n";

	if (isLast)
		stat &= ~(1LL << (depth - 1));

	if ((tree -> child).size())
	{
		stat ^= 1LL << depth;
		for (int i = 0; i < (tree -> child).size(); i++)
			visualize_straight((tree -> child)[i], depth + 1, stat, i == (tree -> child).size() - 1);
	}
}
void visualize_inclined (Node *tree, int depth, long long &stat, bool isLast, int &lines)
{
	if (!tree)
		return;
	tab(lines);
	for (int i = 0; i <= depth; i++)
		if ((stat >> i) & 1)
			cout << "\\  ";
		else
			cout << "   ";
	cout << "\n";
	lines++;

	tab(lines);
	for (int i = 0; i < depth - 1; i++)
		if ((stat >> i) & 1)
			cout << "\\  ";
		else
			cout << "   ";

	if (depth)
		cout << "\\__";
	cout << tree -> val << "\n";
	lines++;

	if (isLast)
		stat &= ~(1LL << (depth - 1));

	if ((tree -> child).size())
	{
		stat ^= 1LL << depth;
		for (int i = 0; i < (tree -> child).size(); i++)
			visualize_inclined((tree -> child)[i], depth + 1, stat, i == (tree -> child).size() - 1, lines);
	}
}

void initTree (Node *tree)
{
	if (!tree) return;
	tree -> width = getWidth(tree -> val);
	int w = 0, h = 0;
	for (int i = 0; i < (tree -> child).size(); i++)
	{
		initTree((tree -> child)[i]);
		w += ((tree -> child)[i]) -> width + 2 * (i != 0);
		h = max(h, ((tree -> child)[i]) -> height);
	}
	tree -> width = max(tree -> width, w);
	tree -> height = h ? 4 + h : 1;

	//cerr << "[DEBUG] " << tree -> val << " " << tree -> width << " " << tree -> height << "\n";
	return;
}

int getWidth (int x)
{
	//cerr << "[DEBUG] width " << x << " ";
	int res = (x == 0);
	while (x)
	{
		res++;
		x /= 10;
	}
	//cerr << res << "\n";
	return res;
}

void center (int val, char **res, int depth, int l, int r)
{
	int w = getWidth(val), W = r - l + 1, margin = (W - w) / 2;
	for (int i = w; i > 0; i--)
	{
		res[depth][l + i + margin - 1] = (char)('0' + val % 10);
		val /= 10;
	}
}

void visualize_tree (Node * tree, int depth, int l, int r, char **res)
{
	if (!tree)
		return;
	
	//cerr << "[DEBUG] " << tree -> val << " " << depth << " " << l << " " << r << "\n";

	center(tree -> val, res, depth, l, r);
	int n = (tree -> child).size();
	if (n == 0)
		return;
	res[depth + 1][(l + r) / 2] = '|';
	if (n == 1)
	{
		res[depth + 2][(l + r) / 2] = '|';
		res[depth + 3][(l + r) / 2] = '|';
		visualize_tree((tree -> child)[0], depth + 4, l, r, res);
	}
	else if (n > 1)
	{
		int mL = (((tree -> child)[0]) -> width - 1) / 2,
	    	    mR = ((tree -> child)[n - 1] -> width) / 2;

		//cerr << "[DEBUG] mL = " << mL << " mR = " << mR << "\n";

    		for (int i = l + mL; i <= r - mR; i++)
			res[depth + 2][i] = '-';

		for (int i = 0; i < n; i++)
		{
			res[depth + 3][l + (((tree -> child)[i]) -> width - 1) / 2] = '|';
			//cerr << "[DEBUG] | at " << (((tree -> child)[i]) -> width - 1) / 2 << "\n";
			visualize_tree((tree -> child)[i], depth + 4, l, ((tree -> child)[i]) -> width + l - 1, res);
			l += ((tree -> child)[i]) -> width + 2;
		}
	}
}

void input (Node *&tree, int &style)
{
	cout << "[MESSAGE] Where do you want to import?\n";
	cout << "1. std\n";
	cout << "2. file\n";
	
	int prompt;
	
	do
	{
		cin >> prompt;
		if (prompt < 1 || prompt > 2)
		{
			cout << "[ERROR] Selection is in valid, please try again.\n";
			continue;
		}
		break;
	}
	while (1);

	if (prompt == 1)
	{
		int n, par;
		cout << "[INPUT] Type number of nodes: ";
		cin >> n;
		Node** nodes = new Node *[n];
		for (int i = 0; i < n; i++) nodes[i] = new Node;
		
		for (int i = 0; i < n; i++)
		{
			cout << "[INPUT] Type value of node " << i << ": ";
			cin >> ((nodes[i]) -> val);
			cout << "[INPUT] Type parent of node " << i << " (-1 means the root): ";
			cin >> par;
			if (par != -1)
				((nodes[par]) -> child).push_back(nodes[i]);
			else
				tree = nodes[i];
		}
	}
	else 
	{
		ifstream inp;
		inp.open("tree.txt");
		if (!inp.is_open())
		{
			cout << "[ERROR] File not found!\n";
			return;
		}

		int n, par;
		inp >> n;
		Node** nodes = new Node *[n];
		for (int i = 0; i < n; i++) nodes[i] = new Node;
		
		for (int i = 0; i < n; i++)
		{

			inp >> ((nodes[i]) -> val);
			inp >> par;
			if (par != -1)
				((nodes[par]) -> child).push_back(nodes[i]);
			else
				tree = nodes[i];
		}
		inp.close();
		cout << "[INFO] Input completed\n";
	}

	cout << "[MESSAGE] Which style of the tree do you want?\n";
	cout << "1. Vertical\n";
	cout << "2. Inclined\n";
	cout << "3. Tree\n";

	do
	{
		cin >> prompt;
		if (prompt < 1 || prompt > 3)
			cout << "[ERROR] SElection is invalid, please try again.\n";
		else
			break;
	}
	while (1);
	
	style = prompt;	
}

void del (Node *&tree)
{
	if (!tree)
		return;
	for (int i = 0; i < (tree -> child).size(); i++)
		del((tree -> child)[i]);
	//cerr << "[DEBUG] Removing node " << tree -> val << "\n";
	(tree -> child).clear();
	delete tree;
	tree = nullptr;
}
