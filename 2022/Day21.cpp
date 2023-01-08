#include "stdafx.h"

namespace Day21
{
	bool printOperations = false;

	struct Operation
	{
		S64		   num		= 0;
		bool	   computed = false;
		string	   name;
		Operation *left	 = nullptr;
		Operation *right = nullptr;
		enum Operator
		{
			PLUS,
			MINUS,
			DIVIDE,
			MULTIPLY
		} oper;

		bool Compute()
		{
			if (!computed)
			{
				if (left && left->computed && right->computed)
				{
					switch (oper)
					{
						case PLUS:
							num = left->num + right->num;
							if (printOperations)
								cout << fixed << left->num << " + " << right->num << " -> " << num << endl;
							break;
						case MINUS:
							num = left->num - right->num;
							if (printOperations)
								cout << fixed << left->num << " - " << right->num << " -> " << num << endl;
							break;
						case DIVIDE:
							num = left->num / right->num;
							if (printOperations)
								cout << fixed << left->num << " / " << right->num << " -> " << num << endl;
							break;
						case MULTIPLY:
							num = left->num * right->num;
							if (printOperations)
								cout << fixed << left->num << " * " << right->num << " -> " << num << endl;
							break;
					}
					computed = true;
				}
			}

			return computed;
		}

		Operator InverseOper()
		{
			switch (oper)
			{
				case PLUS: return MINUS;
				case MINUS: return PLUS;
				case DIVIDE: return MULTIPLY;
				case MULTIPLY: return DIVIDE;
			}
		}
	};

	void Solve()
	{
		cout << "--- Day 21: Monkey Math ---" << endl;
		unordered_map<string, Operation *> operations;
		ifstream						   is("Data21.txt");
		string							   line;
		while (getline(is, line))
		{
			stringstream ss(line);
			string		 name;
			ss >> name;

			name.pop_back();
			auto &op = operations[name];
			if (op == nullptr)
				op = new Operation;
			op->name = name;

			string nameLeft;
			ss >> nameLeft;
			char c = nameLeft[0];
			if (c >= '0' && c <= '9')
			{
				op->num		 = atoi(nameLeft.c_str());
				op->computed = true;
			}
			else
			{
				string oper, nameRight;
				ss >> oper >> nameRight;
				switch (oper[0])
				{
					case '+': op->oper = Operation::PLUS; break;
					case '-': op->oper = Operation::MINUS; break;
					case '*': op->oper = Operation::MULTIPLY; break;
					case '/': op->oper = Operation::DIVIDE; break;
				}

				auto &left = operations[nameLeft];
				if (left == nullptr)
					left = new Operation;
				left->name	= nameLeft;
				auto &right = operations[nameRight];
				if (right == nullptr)
					right = new Operation;
				right->name = nameRight;
				op->left	= left;
				op->right	= right;
			}
		}


		Operation *root = operations["root"];

		unordered_set<Operation *> visited;
		list<Operation *>		   todo;


		std::function<void(Operation *)> Unroll;
		Unroll = [&](Operation *op) {
			if (visited.insert(op).second)
			{
				if (!op->computed)
				{
					if (!op->Compute())
					{
						todo.push_back(op);
						if (op->left)
						{
							Unroll(op->left);
							Unroll(op->right);
						}
					}
				}
			}
			else
			{
				// we need that sooner so move to the end
				auto it = std::find(todo.begin(), todo.end(), op);
				std::swap(*it, *todo.end());
			}
		};

		Unroll(root);


		while (!todo.empty())
		{
			Operation *op	= todo.back();
			bool	   done = op->Compute();
			Assert(done);
			todo.pop_back();
		}
		cout << "Part1:" << root->num << endl;


		// reset for part2
		visited.clear();
		for (auto &op : operations)
		{
			op.second->computed = (op.second->left == nullptr && op.second->right == nullptr);
		}
		operations["humn"]->computed = false;

		Unroll(root);

		while (!todo.empty())
		{
			Operation *op	= todo.back();
			bool	   done = op->Compute();
			todo.pop_back();
		}

		// now we should only have the operations dependent on humn that are not computed
		// we need to reverse the process from root

		Operation *oldMissing = !root->left->computed ? root->left : root->right;
		Operation *oldGood	  = root->left->computed ? root->left : root->right;

		// copy operations to form a new tree that should end with humn as root and root as leaf
		// build form leaf to root
		// left leaf is previous new op
		// right leaf is old sibling operand
		// new op will be old child with !computed
		Operation *pNewOp = new Operation;
		pNewOp->computed  = true;
		pNewOp->num		  = oldGood->num;
		pNewOp->name	  = oldMissing->name;

		Operation *lastNewOp = pNewOp;
		while (oldMissing->name.compare("humn") != 0)
		{
			Operation *lastOldMissing = oldMissing;
			oldGood					  = oldMissing->left->computed ? oldMissing->left : oldMissing->right;
			oldMissing				  = !oldMissing->left->computed ? oldMissing->left : oldMissing->right;

			Operation *newGood = new Operation;
			newGood->computed  = true;
			newGood->num	   = oldGood->num;
			newGood->name	   = oldGood->name;

			Operation *newMissing = new Operation;
			newMissing->name	  = oldMissing->name;
			if (oldMissing == lastOldMissing->right && (lastOldMissing->oper == Operation::MINUS || lastOldMissing->oper == Operation::DIVIDE))
			{
				newMissing->oper  = lastOldMissing->oper;
				newMissing->left  = newGood;
				newMissing->right = lastNewOp;
			}
			else
			{
				newMissing->oper  = lastOldMissing->InverseOper();
				newMissing->left  = lastNewOp;
				newMissing->right = newGood;
			}

			lastNewOp = newMissing;
		}

		// printOperations = true;
		visited.clear();
		Unroll(lastNewOp);

		while (!todo.empty())
		{
			Operation *op	= todo.back();
			bool	   done = op->Compute();
			Assert(done);
			todo.pop_back();
		}

		cout << "Part2:" << fixed << lastNewOp->num << endl;
	}
} // namespace Day21