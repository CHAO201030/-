#include"bst.h"

BST* bst_create()
{
	BST* tree = (BST*)calloc(1, sizeof(BST));
	if (tree == NULL)
	{
		printf("BST create failed, calloc failed\n");
		exit(-1);
	}

	return tree;
}

TreeNode* bst_search(BST* tree, K key)
{
	if (tree == NULL || tree->root == NULL)
	{
		printf("BST search failed, BST is empty or not initialized\n");
		return NULL;
	}
	else
	{
		TreeNode* pCur = tree->root;
		while (pCur)
		{
			if (pCur->key == key)
			{
				return pCur;
			}
			pCur = key > pCur->key ? pCur->right : pCur->left;
		}
		return NULL;
	}
}

void bst_destroy(BST* tree)
{

}

void bst_insert(BST* tree, K key)
{
	if (tree == NULL)
	{
		printf("BST insert failed, BST is NULL\n");
		exit(-1);
	}
	else
	{
		TreeNode* pNew = (TreeNode*)calloc(1, sizeof(TreeNode));
		if (pNew == NULL)
		{
			printf("BST insert failed, malloc failed\n");
			exit(-1);
		}

		pNew->key = key;

		if (tree->root == NULL)
		{
			tree->root = pNew;
			return;
		}
		else
		{
			// ���� key ֵ��λ�� new_key > cur_key,���������ߣ���Ȼ����������
			TreeNode* pCur = tree->root;
			TreeNode* pPre = NULL;
			while (pCur)
			{
				if (pCur->key == key)
				{
					printf("BST insert failed, data already in BST\n\n");
					free(pNew);
					return;
				}
				pPre = pCur;
				// key�������ߣ�keyС������
				pCur = key > pCur->key ? pCur->right : pCur->left;
			}

			// pNewС���뵽���ӣ�pNew����뵽�Һ���
			if (pPre->key > pNew->key)
			{
				pPre->left = pNew;
				return;
			}
			pPre->right = pNew;
		}
	}
}

void bst_delete(BST* tree, K key)
{
	if (tree == NULL || tree->root == NULL)
	{
		printf("BST search failed, BST is empty or not initialized\n");
		return;
	}
	else
	{
		TreeNode* pCur = tree->root;
		TreeNode* pParent = NULL;

		// �ҵ�Ҫɾ���Ľڵ�
		while (pCur)
		{
			if (pCur->key == key)break;
			pParent = pCur;
			pCur = key > pCur->key ? pCur->right : pCur->left;

		}

		if (pCur == NULL)
		{
			printf("BST delete node failed, no such node\n");
			return;
		}

		// case a : ��ɾ���ڵ�û�к��ӣ�ֱ��ɾ�������ڵ�ָ���ΪNULL
		if (pCur->left == NULL && pCur->right == NULL)
		{

		}

		// case b : ��ɾ���ڵ����������ӣ��������������ֱ��ǰ�����̴�������ת��Ϊcase a
		if (pCur->left && pCur->right)
		{
			// ������ֱ��ǰ���ڵ�

			// ������ֱ�Ӻ�̽ڵ�

		}

		// case c : ��ɾ���ڵ���һ�����ӣ��ø��ڵ�ָ��ָ���亢��
		if (pCur->left || pCur->right)
		{

		}
	}
}

void pre_order(TreeNode* node)
{
	if (node == NULL)
	{
		return;
	}
	else
	{
		printf("%3d ", node->key);
		pre_order(node->left);
		pre_order(node->right);
	}
}

void bst_preorder(BST* tree)
{
	pre_order(tree->root);
	printf("\n");
}

void in_order(TreeNode* node)
{
	if (node == NULL)
	{
		return;
	}
	else
	{
		in_order(node->left);
		printf("%3d ", node->key);
		in_order(node->right);
	}
}

void bst_inorder(BST* tree)
{
	in_order(tree->root);
	printf("\n");
}

void post_order(TreeNode* node)
{
	if (node == NULL)
	{
		return;
	}
	else
	{
		post_order(node->left);
		post_order(node->right);
		printf("%3d ", node->key);
	}

}

void bst_postorder(BST* tree)
{
	post_order(tree->root);
	printf("\n");
}

void bst_levelorder(BST* tree)
{

}