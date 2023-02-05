#include "../include/calcDistance.h"

// 取三个数中的最小值
int triple_min(const int &a, const int &b, const int &c)
{
	return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

// 根据utf8编码解析当前字符占几个字节
size_t calcDistance::nBytesCode(const char ch)
{
    // 当前字节的最高位为1代表字节数超过1个
    if(ch & (1 << 7))
	{
		int nBytes = 1;
		for(int idx = 0; idx != 6; ++idx)
		{
			if(ch & (1 << (6 - idx)))
			{
				++nBytes;	
			}
			else
				break;
		}
		return nBytes;
	}
    // 当前字节的最高位为0
	return 1;
}

// 根据utf8编码解析单词每一位字节算出单词实际长度
size_t calcDistance::length(const string &str)
{
    std::size_t ilen = 0;
	for(std::size_t idx = 0; idx != str.size(); ++idx)
	{
		int nBytes = nBytesCode(str[idx]);
		idx += (nBytes - 1);
		++ilen;
	}
	return ilen;

}

int calcDistance::editDistance(const string &rhs)
{
    //计算最小编辑距离-包括处理中英文
	size_t lhs_len = length(_keyWord);
	size_t rhs_len = length(rhs);
	int editDist[lhs_len + 1][rhs_len + 1];
	
    // 先把两边的距离写出，相当于从无到有的编辑距离
    for(size_t idx = 0; idx <= lhs_len; ++idx)
	{
		editDist[idx][0] = idx;
	}

	for(size_t idx = 0; idx <= rhs_len; ++idx)
	{
		editDist[0][idx] = idx;
	}
	
    // 遍历最小距离图的每个结点，每个结点存的是对应位置左右单词的最小编辑距离
	std::string sublhs, subrhs;
	for(std::size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i, ++lhs_idx)
	{
		size_t nBytes = nBytesCode(_keyWord[lhs_idx]);
		sublhs = _keyWord.substr(lhs_idx, nBytes);
		lhs_idx += (nBytes - 1);

		for(std::size_t dist_j = 1, rhs_idx = 0; dist_j <= rhs_len; ++dist_j, ++rhs_idx)
		{
			nBytes = nBytesCode(rhs[rhs_idx]);
			subrhs = rhs.substr(rhs_idx, nBytes);
			rhs_idx += (nBytes - 1);
			
            // 相等说明不需要变化，相当于左右同时多了一个字符，所以与都减去一个字符的编辑距离相等
            if(sublhs == subrhs)
			{
				editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j - 1];
			}
            // 不相等说明需要变化，可以有三个方向，取三个方向变化的最小距离(相当于广度优先算法)
			else
			{
				editDist[dist_i][dist_j] = triple_min(
					editDist[dist_i][dist_j - 1] + 1,
					editDist[dist_i - 1][dist_j] + 1,
					editDist[dist_i - 1][dist_j - 1] + 1);
			}
		}
	}

	return editDist[lhs_len][rhs_len];
}

