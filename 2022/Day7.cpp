#include "stdafx.h"
namespace Day7
{
	struct Element
	{
		string name;

		virtual size_t GetSize() const = 0;
	};

	struct File : Element
	{
		size_t size;
		size_t GetSize() const { return size; }
	};

	struct Directory : Element
	{
		Directory *		  pParent = nullptr;
		vector<Element *> children;

		size_t GetSize() const
		{
			size_t sum = 0;
			for (const auto pChild : children)
			{
				sum += pChild->GetSize();
			}

			return sum;
		}
	};

	vector<Directory *> allDirs;


	void Solve()
	{
		cout << "--- Day 7: No Space Left On Device ---" << endl;
		std::ifstream is("Data7.txt");

		Directory  root;
		Directory *pCurrent = &root;
		allDirs.push_back(&root);
		string line;
		while (getline(is, line, '\n'))
		{
			stringstream ss(line);
			if (line[0] == '$')
			{
				// command
				string word;
				ss >> word; // $
				ss >> word;
				if (word.compare("cd") == 0)
				{
					ss >> word;
					if (word.compare("..") == 0)
					{
						pCurrent = pCurrent->pParent;
					}
					else if (word.compare("/") == 0)
					{
						pCurrent = &root;
					}
					else
					{
						// subdir
						bool found = false;
						for (auto pElmt : pCurrent->children)
						{
							if (word.compare(pElmt->name) == 0)
							{
								pCurrent = dynamic_cast<Directory *>(pElmt);
								assert(pCurrent);
								found = true;
								break;
							}
						}
						assert(found);
					}
				}
				// dont care about ls
			}
			else if (line.find_first_of("dir") == 0)
			{
				Directory *pDir = new Directory;
				pDir->pParent	= pCurrent;
				ss >> pDir->name; // dir
				ss >> pDir->name;
				pCurrent->children.push_back(pDir);
				allDirs.push_back(pDir);
			}
			else // file
			{
				File *pFile = new File;
				ss >> pFile->size;
				ss >> pFile->name;
				pCurrent->children.push_back(pFile);
			}
		}


		size_t sum = 0;
		for (auto child : allDirs)
		{
			if (child->GetSize() <= 100000)
				sum += child->GetSize();
		}
		cout << "Part1:" << sum << endl;


		size_t remaining = 70000000 - root.GetSize();
		size_t needed	 = 30000000 - remaining;

		std::sort(allDirs.begin(), allDirs.end(), [](const Directory *lhs, const Directory *rhs) { return lhs->GetSize() < rhs->GetSize(); });

		for (auto child : allDirs)
		{
			if (child->GetSize() >= needed)
			{
				sum = child->GetSize();
				break;
			}
		}

		cout << "Part2:" << sum << endl;
	}
} // namespace Day7