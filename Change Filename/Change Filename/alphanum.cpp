//#include "alphanum.h"
//#include <string>
//#include <sstream>
//
//using namespace std;
//
//bool isDigit(char ch)
//{
//	return (ch >= 48) && (ch <= 57);
//}
//
//string getChunk(string s, int slength, int marker)
//{
//	stringstream chunk; //StringBuilder
//	char c = s.at(marker);
//	chunk<<c; //Append the character
//	marker++;
//
//	if(isDigit(c))
//	{
//		while(marker < slength)
//		{
//			c = s.at(marker);
//			if(!isDigit(c))
//				break;
//			chunk<<c; //Append the character
//			marker++;
//		}
//	}
//	else
//	{
//		while(marker < slength)
//		{
//			c = s.at(marker);
//			if(isDigit(c))
//				break;
//			chunk<<c; //Append the character
//			marker++;
//		}
//	}
//
//	return chunk.str();
//}
//
//int compare(string s1, string s2)
//{
//	int thisMarker = 0;
//	int thatMarker = 0;
//	int s1Length = s1.length();
//	int s2Length = s2.length();
//
//	while((thisMarker < s1Length) && (thatMarker < s2Length))
//	{
//		string thisChunk = getChunk(s1, s1Length, thisMarker);
//		thisMarker += thisChunk.length();
//
//		string thatChunk = getChunk(s2, s2Length, thatMarker);
//		thatMarker += thatChunk.length();
//
//		//If both chunks contain numeric charactrs sort them numerically
//		int result = 0;
//		if(isDigit(thisChunk.at(0)) && isDigit(thatChunk.at(0)))
//		{
//			//Simple chunk comparison by length.
//			int thisChunkLength = thisChunk.length();
//			result = thisChunkLength - thatChunk.length();
//
//			//If equal, the first different number counts.
//			if(result == 0)
//			{
//				for(int i = 0; i < thisChunkLength; i++)
//				{
//					result = thisChunk.at(i) - thatChunk.at(i);
//					if(result != 0)
//						return result;
//				}
//			}
//			else
//				result = thisChunk.compare(thatChunk);
//
//			if(result != 0)
//				return result;
//		}
//	}
//
//	return s1Length - s2Length;
//}