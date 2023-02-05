#include "../../include/RssParses.h"
#include <tinyxml2.h>

// 删除html标签
void  rm_html_flag(int type, string &text)
{
    regex reg2("<.*?>");
    text = regex_replace(text, reg2, "");
    regex reg3("\n");
    text = regex_replace(text, reg3, "");
}

void RssReader::parseRss()
{
    XMLDocument doc;
    int ret = doc.LoadFile(_filepath.c_str());
    if(ret != XML_SUCCESS)
    {
        perror("loadFile");
        return;
    }
    XMLElement * root = doc.RootElement();
    XMLElement * Item = root->FirstChildElement()->FirstChildElement("item");
    // 遍历文件找到所有item，将每个item中的内容存入vector容器
    int num = 1;
    _rss.clear();
    while(Item != nullptr)
    {
        RssItem itemNode;
        // 遍历每个item
        for(const XMLNode *tempNode = Item->FirstChild();tempNode != nullptr; tempNode = tempNode->NextSibling())
        {
            // title节点
            if(strcmp(tempNode->Value(), "title") == 0)
            {
                string text = tempNode->ToElement()->GetText();
                // 去除html标签
                rm_html_flag(TITLE, text);
                itemNode.title += text;
            }
            // link节点
            else if(strcmp(tempNode->Value(), "link") == 0)
            {
                string text = tempNode->ToElement()->GetText();
                rm_html_flag(LINK, text);
                itemNode.url += text;
            }
            // description
            else if(strcmp(tempNode->Value(), "description") == 0)
            {
                string text = tempNode->ToElement()->GetText();
                rm_html_flag(DESCRIPTION, text);
                itemNode.description += text;
            }
            // content
            else if(strcmp(tempNode->Value(), "content:encoded") == 0)
            {
                string text = tempNode->ToElement()->GetText();
                rm_html_flag(CONTENT, text);
                itemNode.content += text;
            }
        }
        _rss.push_back(itemNode);
        Item = Item->NextSiblingElement("item");
        ++num;
    }
}

vector<RssItem>& RssReader::getrss()
{
    return _rss;
}

