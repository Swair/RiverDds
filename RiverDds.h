#ifndef __RIVERDDS_H
#define __RIVERDDS_H

struct Topic
{
    Topic(){}
    
    Topic(const std::string& tName)
    {
        name = tName;
    }

    std::queue<std::string> mem;
    std::string name;
};

class Node 
{
    public:
        Node() {}

        Node(const std::string& nName)
        {
            name_ = nName;
        }

        Topic* open(const std::string& tName)
        {
            Topic* topic;
            auto&& item = topics_.find(tName);
            if(item == topics_.end())
            {
                topic = New2(Topic, tName);
                topics_[tName] = topic;
                return topic;
            }
            topic = topics_[tName];
            return topic;
        }

    private:
        std::map<std::string, Topic*> topics_; 
        std::string name_; 

};

class RiverDds
{
    public:
        Topic* open(const std::string& nName, const std::string& tName)
        {
            Node* db = nullptr;
            auto&& item = dbs_.find(nName);
            if(item == dbs_.end())
            {
                db = New2(Node, nName);
                dbs_[nName] = db;
                return db->open(tName);
            }
            db = dbs_[nName];
            return db->open(tName);
        }

    private:
        std::map<std::string, Node*> dbs_;
};

#endif



