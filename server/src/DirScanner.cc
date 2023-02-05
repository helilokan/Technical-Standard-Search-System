#include "../include/DirScanner.h"
#include <sys/types.h>
#include <dirent.h>

DirScanner::DirScanner()
{

}

void DirScanner::operator()(const string &dirname)
{
    if(!_files.empty())
    {
        _files.clear();
    }
    traverse(dirname);
}

vector<string> &DirScanner::files()
{
    return _files;
}

void DirScanner::traverse(const string &dirname)
{
    DIR *dirp = opendir(dirname.c_str());
    if(dirp == nullptr)
    {
        perror("opendir");
        return;
    }
    struct dirent *pdirent;
    while((pdirent = readdir(dirp)) != nullptr)
    {
        if(*(pdirent->d_name) == '.')
        {
            continue;
        }
        string newPath = dirname + string("/") + pdirent->d_name;
        /* printf("newPath = %s\n", newPath.c_str()); */
        if(pdirent->d_type == DT_DIR)
        {
            
            traverse(newPath);
        }
        else
        {
            _files.push_back(newPath);
        }
    }
    closedir(dirp);
}
