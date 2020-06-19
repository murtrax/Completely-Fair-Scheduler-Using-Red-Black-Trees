/*
* C++ Program to Implement Red Black Tree
*/
#include <iostream>
#include <assert.h>
#include <Windows.h>
#include <conio.h>

#define INDENT_STEP  4
using namespace std;

int count = 0;
int time_slice = 16;
HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
enum color { RED, BLACK };
/*
* Node RBTree Declaration
*/
typedef struct rbtree_node
{
	char id;
	enum color color;
	int key, max_time = 0;
	rbtree_node *left, *right, *parent;
}*node;

typedef struct rbtree_t
{
	node root;
}*rbtree;

/*
* Class RBTree Declaration
*/
class RBTree
{
public:
	typedef int(*compare_func)(int left, int right);
	rbtree rbtree_create();

	int rbt_find(rbtree t, int, compare_func compare);
	void rbt_add(rbtree t,char id, int,int, compare_func compare);
	void rbt_addt(RBTree rbt,rbtree t, char id, int, int, compare_func compare,int check);
	void rbt_del(rbtree t, int, compare_func compare);

	node node_GP(node n);
	node node_SIB(node n);
	node node_UNC(node n);

	void check_P(rbtree t);
	void check_P1(node root);
	void check_P2(node root);
	
	color check_color(node n);

	void check_P3(node root);
	void check_P4(node root);
	void check_P4_helper(node n, int, int);


	node n_new(char id,int key,int max_time, color, node, node);
	node find_node(rbtree t, int, compare_func compare);

	void L_rotate(rbtree t, node n);
	void R_rotate(rbtree t, node n);

	void n_replace(rbtree t, node oldn, node newn);

	void add_C1(rbtree t, node n);
	void add_C2(rbtree t, node n);
	void add_C3(rbtree t, node n);
	void add_C4(rbtree t, node n);
	void add_C5(rbtree t, node n);

	node n_max(node root);
	node n_min(node root);

	void del_C1(rbtree t, node n);
	void del_C2(rbtree t, node n);
	void del_C3(rbtree t, node n);
	void del_C4(rbtree t, node n);
	void del_C5(rbtree t, node n);
	void del_C6(rbtree t, node n);
	void CFS(RBTree rbt,rbtree t);
};
/*
* Return node_GP of Node
*/
node RBTree::node_GP(node n)
{
	assert(n != NULL);
	assert(n->parent != NULL);
	assert(n->parent->parent != NULL);
	return n->parent->parent;
}

/*
* Return node_SIB of Node
*/
node RBTree::node_SIB(node n)
{
	assert(n != NULL);
	assert(n->parent != NULL);
	if (n == n->parent->left)
		return n->parent->right;
	else
		return n->parent->left;
}

/*
* Return node_UNC of Node
*/
node RBTree::node_UNC(node n)
{
	assert(n != NULL);
	assert(n->parent != NULL);
	assert(n->parent->parent != NULL); 
	return node_SIB(n->parent);
}

/*
* Verifying Properties of Red black Tree
*/
void RBTree::check_P(rbtree t)
{
	check_P1(t->root);
	check_P2(t->root);
	check_P3(t->root);
	check_P4(t->root);
}
/*
* Verifying Property 1
*/
void RBTree::check_P1(node n)
{
	assert(check_color(n) == RED || check_color(n) == BLACK);
	if (n == NULL)
		return;
	check_P1(n->left);
	check_P1(n->right);
}
/*
* Verifying Property 2
*/
void RBTree::check_P2(node root)
{
	assert(check_color(root) == BLACK);

}
/*
* Returns color of a node
*/
color RBTree::check_color(node n)
{
	return n == NULL ? BLACK : n->color;
}
/*
* Verifying Property 4
*/
void RBTree::check_P3(node n)
{
	if (check_color(n) == RED)
	{
		assert(check_color(n->left) == BLACK);
		assert(check_color(n->right) == BLACK);
		assert(check_color(n->parent) == BLACK);
	}
	if (n == NULL)
		return;
	check_P3(n->left);
	check_P3(n->right);
}
/*
* Verifying Property 5
*/
void RBTree::check_P4(node root)
{
	int black_count_path = -1;
	check_P4_helper(root, 0, black_count_path);
}

void RBTree::check_P4_helper(node n, int black_count, int path_black_count)
{
	if (check_color(n) == BLACK)
	{
		black_count++;
	}
	if (n == NULL)
	{
		if (path_black_count == -1)
		{
			path_black_count = black_count;
		}
		else
		{
			assert(black_count == path_black_count);
		}
		return;
	}
	check_P4_helper(n->left, black_count, path_black_count);
	check_P4_helper(n->right, black_count, path_black_count);
}

/*
* Create Red Black Tree
*/
rbtree RBTree::rbtree_create()
{
	rbtree t = new rbtree_t;
	t->root = NULL;
	check_P(t);
	return t;
}

/*
* Creating New Node of Reb Black Tree
*/
node RBTree::n_new(char id,int k,int m_t, color n_color, node left, node right)
{
	node result = new rbtree_node;
	result->id = id;
	result->key = k;
	result->max_time = m_t;
	result->color = n_color;
	result->left = left;
	result->right = right;
	if (left != NULL)
		left->parent = result;
	if (right != NULL)
		right->parent = result;
	result->parent = NULL;
	return result;
}
/*
* Look Up through Node
*/
node RBTree::find_node(rbtree t, int key, compare_func compare)
{
	node n = t->root;
	while (n != NULL)
	{
		int comp_result = compare(key, n->key);
		if (comp_result == 0)
		{
			return n;
		}
		else if (comp_result < 0)
		{
			n = n->left;
		}
		else
		{
			assert(comp_result > 0);
			n = n->right;
		}
	}
	return n;
}
/*
* RbTree Look Up
*/
int RBTree::rbt_find(rbtree t, int key, compare_func compare)
{
	node n = find_node(t, key, compare);
	return n == NULL ? NULL : n->key;
}

/*
* Rotate left
*/
void RBTree::L_rotate(rbtree t, node n)
{
	node R = n->right;
	n_replace(t, n, R);
	n->right = R->left;
	if (R->left != NULL)
	{
		R->left->parent = n;
	}
	R->left = n;
	n->parent = R;
}
/*
* Rotate right
*/
void RBTree::R_rotate(rbtree t, node n)
{
	node L = n->left;
	n_replace(t, n, L);
	n->left = L->right;
	if (L->right != NULL)
	{
		L->right->parent = n;
	}
	L->right = n;
	n->parent = L;
}
/*
* Replace a node
*/
void RBTree::n_replace(rbtree t, node oldn, node newn)
{
	if (oldn->parent == NULL)
	{
		t->root = newn;

		if (oldn->parent == NULL && newn->color == RED) {
			newn->color = BLACK;
		}

	}
	else
	{
		if (oldn == oldn->parent->left)
			oldn->parent->left = newn;
		else
			oldn->parent->right = newn;
	}
	if (newn != NULL)
	{
		newn->parent = oldn->parent;
	}
}
/*
* Insert node into RBTree
*/
void RBTree::rbt_add(rbtree t,char id, int key,int max_time, compare_func compare)
{
	node inserted_node = n_new(id, key, max_time, RED, NULL, NULL);

	if (t->root == NULL)
	{
		t->root = inserted_node;
	}
	else
	{
		node n = t->root;
		while (1)
		{
			int comp_result = compare(key, n->key);
			if (comp_result == 0)
			{
				return;
			}
			else if (comp_result < 0)
			{
				if (n->left == NULL)
				{
					n->left = inserted_node;
					break;
				}
				else
				{
					n = n->left;
				}
			}
			else
			{
				assert(comp_result > 0);
				if (n->right == NULL)
				{
					n->right = inserted_node;
					break;
				}
				else
				{
					n = n->right;
				}
			}
		}
		inserted_node->parent = n;
	}
	add_C1(t, inserted_node);
	check_P(t);
}

/*
* Inserting Case 1
*/
void RBTree::add_C1(rbtree t, node n)
{
	if (n->parent == NULL)
		n->color = BLACK;
	else
		add_C2(t, n);
}

/*
* Inserting Case 2
*/
void RBTree::add_C2(rbtree t, node n)
{
	if (check_color(n->parent) == BLACK)
		return;
	else
		add_C3(t, n);
}

/*
* Inserting Case 3
*/
void RBTree::add_C3(rbtree t, node n)
{
	if (check_color(node_UNC(n)) == RED)
	{
		n->parent->color = BLACK;
		node_UNC(n)->color = BLACK;
		node_GP(n)->color = RED;
		add_C1(t, node_GP(n));
	}
	else
	{
		add_C4(t, n);
	}
}

/*
* Inserting Case 4
*/
void RBTree::add_C4(rbtree t, node n)
{
	if (n == n->parent->right && n->parent == node_GP(n)->left)
	{
		L_rotate(t, n->parent);
		n = n->left;
	}
	else if (n == n->parent->left && n->parent == node_GP(n)->right)
	{
		R_rotate(t, n->parent);
		n = n->right;
	}
	add_C5(t, n);
}

/*
* Inserting Case 5
*/
void RBTree::add_C5(rbtree t, node n)
{
	n->parent->color = BLACK;
	node_GP(n)->color = RED;
	if (n == n->parent->left && n->parent == node_GP(n)->left)
	{
		R_rotate(t, node_GP(n));
	}
	else
	{
		assert(n == n->parent->right && n->parent == node_GP(n)->right);
		L_rotate(t, node_GP(n));
	}
}

/*
* Delete Node from RBTree
*/
void RBTree::rbt_del(rbtree t, int key, compare_func compare)
{
	node child;
	node n = find_node(t, key, compare);
	if (n == NULL)
		return;
	if (n->left != NULL && n->right != NULL)
	{
		node pred = n_max(n->left);
		n->key = pred->key;
		n = pred;
	}
	assert(n->left == NULL || n->right == NULL);
	child = n->right == NULL ? n->left : n->right;

	if (check_color(n) == BLACK)
	{
		n->color = check_color(child);
		del_C1(t, n);
	}

	

	n_replace(t, n, child);
	free(n);
	check_P(t);
	
}

/*
* Returns Maximum node
*/
node RBTree::n_max(node n)
{
	assert(n != NULL);
	while (n->right != NULL)
	{
		n = n->right;
	}
	return n;
}

node RBTree::n_min(node n) {

	assert(n != NULL);
	while (n->left != NULL) {
		n = n-> left;
	}
	return n;
}

/*
* Deleting Case 1
*/
void RBTree::del_C1(rbtree t, node n)
{
	if (n->parent == NULL)
		return;
	else
		del_C2(t, n);
}

/*
* Deleting Case 2
*/
void RBTree::del_C2(rbtree t, node n)
{
	if (check_color(node_SIB(n)) == RED)
	{
		n->parent->color = RED;
		node_SIB(n)->color = BLACK;
		if (n == n->parent->left)
			L_rotate(t, n->parent);
		else
			R_rotate(t, n->parent);
	}
	del_C3(t, n);
}

/*
* Deleting Case 3
*/
void RBTree::del_C3(rbtree t, node n)
{
	if (check_color(n->parent) == BLACK && check_color(node_SIB(n)) == BLACK &&
		check_color(node_SIB(n)->left) == BLACK && check_color(node_SIB(n)->right) == BLACK)
	{
		node_SIB(n)->color = RED;
		del_C1(t, n->parent);
	}
	else
		del_C4(t, n);
}

/*
* Deleting Case 4
*/
void RBTree::del_C4(rbtree t, node n)
{
	if (check_color(n->parent) == RED && check_color(node_SIB(n)) == BLACK &&
		check_color(node_SIB(n)->left) == BLACK && check_color(node_SIB(n)->right) == BLACK)
	{
		node_SIB(n)->color = RED;
		n->parent->color = BLACK;
	}
	else
		del_C5(t, n);
}

/*
* Deleting Case 5
*/
void RBTree::del_C5(rbtree t, node n)
{
	if (n == n->parent->left && check_color(node_SIB(n)) == BLACK &&
		check_color(node_SIB(n)->left) == RED && check_color(node_SIB(n)->right) == BLACK)
	{
		node_SIB(n)->color = RED;
		node_SIB(n)->left->color = BLACK;
		R_rotate(t, node_SIB(n));
	}
	else if (n == n->parent->right && check_color(node_SIB(n)) == BLACK &&
		check_color(node_SIB(n)->right) == RED && check_color(node_SIB(n)->left) == BLACK)
	{
		node_SIB(n)->color = RED;
		node_SIB(n)->right->color = BLACK;
		L_rotate(t, node_SIB(n));
	}
	del_C6(t, n);
}

/*
* Deleting Case 6
*/
void RBTree::del_C6(rbtree t, node n)
{
	node_SIB(n)->color = check_color(n->parent);
	n->parent->color = BLACK;
	if (n == n->parent->left)
	{
		assert(check_color(node_SIB(n)->right) == RED);
		node_SIB(n)->right->color = BLACK;
		L_rotate(t, n->parent);
	}
	else
	{
		assert(check_color(node_SIB(n)->left) == RED);
		node_SIB(n)->left->color = BLACK;
		R_rotate(t, n->parent);
	}
}

/*
* Compare two nodes
*/
int compare_int(int leftp, int rightp)
{
	int left = leftp;
	int right = rightp;
	if (left < right)
		return -1;
	else if (left > right)
		return 1;
	else
	{
		assert(left == right);
		return 0;
	}
}
/*
* Print RBTRee
*/
void print_tree_helper(node n, int indent)
{
	int i;
	if (n == NULL)
	{
		cout << "(E)" << endl;
		return;
	}

	if (n->right != NULL)
	{
		print_tree_helper(n->right, indent + INDENT_STEP);

	}


	for (i = 0; i < indent; i++)
		cout << " ";

	if (n->color == BLACK) {
		SetConsoleTextAttribute(hStdOut, 7);
		cout << "(" << n->key << "){"<< endl;
	}
	else {
		
		SetConsoleTextAttribute(hStdOut, 4);
		cout << "(" << n->key << "){" << endl;
		SetConsoleTextAttribute(hStdOut, 7);
}
	if (n->left != NULL)
	{
		print_tree_helper(n->left, indent + INDENT_STEP);
	}
	
}

void print_tree(rbtree t)
{
print_tree_helper(t->root, 0);
	puts("");
}



int choice;

void PrintMenu()
{
	cout << "\t\t\t\t----------R E D  B L A C K  T R E E----------" << endl << endl;
	cout << "\t\t\t\t\t\t   [1] INSERT " << endl;
	cout << "\t\t\t\t\t\t   [2] DELETE " << endl;
	cout << "\t\t\t\t\t\t   [3] DISPLAY " << endl;
	cout << "\t\t\t\t\t\t   [4] EXIT" << endl;
}

 void inorder( rbtree_node *ptr)
{
	//rbtree t;
	if (ptr != NULL) {
		inorder(ptr->left);
		cout << "\t\t" << ptr->id << ":  "<< ptr->key <<"  " <<ptr->max_time<< "\n\n";
		inorder(ptr->right);
		
	}
	
}



 int rbt_count(rbtree_node *root)
 {
	  int count = 1;
	 if (root->left != NULL) {
		 count += rbt_count(root->left);
	 }
	 if (root->right != NULL) {
		 count += rbt_count(root->right);
	 }
	 return count;
 }

/*
* Main Contains Menu
*/
 

 void CFS(RBTree rbt, rbtree t) {
 
	 cout << "\tCompletely Fair Scedular\n\t     Psuedo Example\n\n";

	
	
	 rbt.rbt_add(t, 'A', 1, 7, compare_int);
	 rbt.rbt_add(t, 'B', 2, 15, compare_int);
	 rbt.rbt_add(t, 'C', 3, 21, compare_int);
	 rbt.rbt_add(t, 'D', 4, 25, compare_int);
	 rbt.rbt_add(t, 'E', 6, 25, compare_int);

	 int total_process = 0;
	 total_process = rbt_count(t->root);
	 
	 //time slice defined as 16
	 int time = time_slice / total_process;
	
	 inorder(t->root);
	 cout << "\t   Processor Time: " << time_slice <<endl;

	 cout << "\n\nProcess: " << total_process << " " << "Time: " << time << endl;


	 int check = 0;

	 while (total_process != 0){
	
		
		 node nsmall = rbt.n_min(t->root);
		
		 cout << endl << nsmall->id << ": (" << nsmall->key << ") selected.\n\n";

		 if (nsmall == NULL) 
		 {
			 cout << "Queue empty";
			 break;
		 }

		 char nid = nsmall->id;
		 //HOW MUCH IT HAS RUN CURRENTLY
		 int max = nsmall->max_time;
		 int nkey = nsmall->key;

		 if (nkey < nsmall->max_time) {

			 print_tree(t);
			 nkey += time;
			 cout << nsmall->id << ": (" << nkey << ") executed-waiting.\n";

			 cout << "\n\n";
			 rbt.rbt_del(t, nsmall->key, compare_int);
			 rbt.rbt_add(t, nid, nkey, max, compare_int);

		 }
		
		 else if ( nkey >= nsmall->max_time) {

			 print_tree(t);
			 cout << nsmall->id << ": finished.\n";
			 cout << "Deleting: " << nsmall->id << endl;
			 
			 cout << "\n\n";
			 rbt.rbt_del(t, nsmall->key, compare_int);
			
			 /*Update time*/
			 total_process = rbt_count(t->root);
			 time = time_slice / total_process;
			 cout << "Process: " << total_process << " " << "Time: " << time << endl;
	
		 }

		 
		

		
	 }//end while
	 cout << "Finished...\n\n\n\n";
 }//end
 

int main()
{
	SetConsoleTextAttribute(hStdOut, 7);

	RBTree rbt;
	rbtree t = rbt.rbtree_create();


	CFS(rbt, t);
	print_tree(t);
	inorder(t->root);

}
