#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
#include <bits/stdc++.h> 

using namespace std;

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>


#include "../include/model.hpp"
//#include"../include/parser.hpp"

bool my_compare(Node* v1, Node* v2)
{
    return (v1->degree <v2->degree);
}

Model::Model()
{
    //create nodes from vec3. 

    rootTriangle = nullptr;
    Tlevel *first_level = new Tlevel();
    levels.push_back(first_level);
    curLevel = 0;
    allow_error = 10000;
    next_it = false;
    current_level = 0;
    startitr = 0;
    indicator = false;
    //sort all nodes in the start. 
}

Node* Model::createNode(float x, float y)
{
    Node * my_node = new Node(x, y);
    all_nodes.push_back(my_node);
    remaining_nodes.push_back(my_node);
    if(all_nodes.size() <4) //3 for hull.
    {
        my_node->is_hullpoint = true;
    }
    return my_node;
}

void Model::createTriangle(Node* a, Node*b, Node*c)
{
    Triangle *my_triangle = new Triangle(a, b, c, 0);
    triangles.push_back(my_triangle);
    int count = triangles.size();
    if(count > 1)
    {
        levels[0]->triangles.push_back(my_triangle);
    }
    //cout<<"done"<<endl;
}

void Model::getTriangle(Point points[], int clicks[])
{
    for(int i=0; i<3; i++)
    {
        if(clicks[i] == 0)
        {
            nodes[i] = createNode(points[i].x, points[i].y);
             
        }
        else
        {
            allow_error = INT_MAX;
            for(int j=0; j<all_nodes.size(); j++)
            {
                cur_error = abs((all_nodes[j]->x)*(all_nodes[j]->x) - (points[i].x * points[i].x)
                                - ((all_nodes[j]->y)*(all_nodes[j]->y) - (points[i].y * points[i].y)));
                if(cur_error < allow_error)
                {
                    allow_error = cur_error;
                    nodes[i] = all_nodes[j];
                }
            }

        }
    }
    createTriangle(nodes[0], nodes[1], nodes[2]);
    
    next_it = true;
}

Node::Node(float a, float b)
{
    x = a;
    y = b;
    degree = 0;
    marked = false;
    in_IS = false;
    is_deleted = false;
    to_pair = 0;
    is_hullpoint = false;

    current_polygon.first = nullptr;
    current_polygon.second = nullptr;
}

Edge::Edge(Node *a, Node *b)
{
    v1 = a;
    v2 = b;

    cutting_edge = false;
}

Triangle::Triangle(Edge* l1, Edge* l2, Edge *l3, Node *a1, Node *a2, Node *a3)
{
    v1 = a1;
    v2 = a2;
    v3 = a3;

    edges.push_back(l1);
    edges.push_back(l2);
    edges.push_back(l3);



    in_polygon = false;
    to_remove = false;
    removed = false; 
}

Triangle::Triangle(Node* a1, Node *a2, Node *a3, int existing)
{
    v1 = a1;
    v2 = a2;
    v3 = a3;

    edges.push_back(new Edge(a1, a2));    
    edges.push_back(new Edge(a3, a1));
    edges.push_back(new Edge(a2, a3));    

    v1->triangle_list.push_back(this);
    v2->triangle_list.push_back(this);
    v3->triangle_list.push_back(this);

    if(existing == 0)
    {   
        v1->degree++;
        v2->degree++;
        v3->degree++;

        v1->connected_list.push_back(v2);
        v1->connected_list.push_back(v3);
        v2->connected_list.push_back(v1);
        v2->connected_list.push_back(v3);
        v3->connected_list.push_back(v2);
        v3->connected_list.push_back(v1);

    }
    else
    {
        v1->degree++;
        v3->degree++;

        v1->connected_list.push_back(v3);
        v3->connected_list.push_back(v1);
    }

    

    in_polygon = false;
    to_remove = false;
    removed = false;    
    does_contain = false;
}

void Model::startbuild()
{
    while(startitr == 0)
    {
        IS_count = createIS();
        if(!IS_count.empty())
        {
            deleteIS(levels[current_level]);
            current_level++;
        }
        else 
        {
            //rootTriangle = new Triangle(remaining_nodes[0], remaining_nodes[1], remaining_nodes[2]);
            setBuffers(0);
            startitr = 1;
            break;
        }
    }
}

vector<Node*> Model::createIS()
{
    IS.clear();  //Sort all nodes in the start.
    remaining_nodes.clear();
    for(itr = all_nodes.begin(); itr < all_nodes.end(); itr++)
    {
        if((*itr)->is_deleted == false)
        {
            remaining_nodes.push_back((*itr));        
        }
    }
    if(remaining_nodes.size() == 3)
    {
        return IS; //will be empty.
    }
    sort(remaining_nodes.begin(), remaining_nodes.end(), my_compare);
    for(itr = remaining_nodes.begin(); itr < remaining_nodes.end(); itr++)
    {
        if((*itr)->is_hullpoint == false && (*itr)->degree < 9)  //degree 9 means we dont go through all the points.
        {
            if((*itr)->marked == false)
            {
                IS.push_back(*itr);
                for(int i=0; i<(*itr)->connected_list.size(); i++)
                {
                    (*itr)->connected_list[i]->marked = true;
                      
                }
                for(int i=0; i<(*itr)->triangle_list.size(); i++)
                {
                    (*itr)->triangle_list[i]->to_remove = true;
                }

                (*itr)->in_IS = true;
                (*itr)->is_deleted = true;  //can have one variable for both. 
            }
            else
            {
                (*itr)->marked = false; //for next iteration. 
            }
               
        }

    }

    return IS;  
}

void Model::deleteIS(Tlevel* tlevel) //assume this level has all of previous level.
{
    Tlevel* new_level = new Tlevel();  //might have to pass below. 
    levels.push_back(new_level);

    for(int i=0; i<tlevel->triangles.size(); i++)
    {
        if(tlevel->triangles[i]->to_remove == false)
        {
            new_level->triangles.push_back(tlevel->triangles[i]);
        }        
    }
    
    vector<Triangle*> new_triangles;
    
    for(int i=0; i<IS.size(); i++)
    {
        new_triangles = fixHole(makeHole(IS[i]), IS[i]);
        countIntersection(IS[i], new_triangles);
    }
    for(int i=0; i<new_triangles.size(); i++)
    {
        new_level->triangles.push_back(new_triangles[i]);
    }
}

vector <Node*> Model::makeHole(Node* v)
{
    vector <Node*> points;
    set <Node*> pvisited;
    vector <Edge*> edges;
    Node *temp1;
    Node *temp2;
    
    for(int i=0; i< v->triangle_list.size(); i++)
    {

        if(v->triangle_list[i]->removed == false)
        {

            for(int j=0; j<3; j++)
            {

                temp1 = v->triangle_list[i]->edges[j]->v1 ;
                temp2 = v->triangle_list[i]->edges[j]->v2 ;
                                
                if(temp1 != v && temp2 !=v)
                {
                    
                    edges.push_back(v->triangle_list[i]->edges[j]);            
                   
                    temp1->current_polygon.second = temp2;    //second is what comes after, first is what comes before.
                    temp2->current_polygon.first = temp1;
                    
                    temp1->to_pair += 1;
                    temp2->to_pair += -1;
                    
                    temp1->degree += -1;
                    temp2->degree += -1;

                    pvisited.insert(temp1);
                    pvisited.insert(temp2);

                    break;
                }
                else
                {
                    v->triangle_list[i]->edges[j]->cutting_edge = true;  //for intersections.
                }
                
            }
            v->triangle_list[i]->removed = true;

        }
        //delete triangles from temp1 and temp2 and remove v from connected list . 
    }

//iterate trhough set to form final edge if needed. and moke topair 0.  
//make the polygon in points and clear current polygon
    set<Node*>::iterator it = pvisited.begin();

    while(it != pvisited.end())  //can use to_pair here. 
    {
        if((*it)->current_polygon.first == nullptr)
        {
            temp2 = (*it);
        }
        else if((*it)->current_polygon.second == nullptr)
        {
            temp1 = (*it);
        }
        it++;
    }

    if(temp1->to_pair !=0)
    {
        
        temp1->current_polygon.second = temp2;
        temp2->current_polygon.first = temp1;

        temp1->to_pair = 0;
        temp2->to_pair = 0;
    }
    points.push_back(temp1);
    points.push_back(temp2);

    while(temp2->current_polygon.second != points[0])
    {
        temp2 = temp2->current_polygon.second;
        points.push_back(temp2);        
    }

    return points;
}


vector<Triangle*> Model::fixHole(vector <Node*> points, Node* v)
{
    vector <Triangle*> new_triangles;

    Node* it_left;
    Node* it_right;
    it_left = points[0]->current_polygon.first;
    it_right = points[0]->current_polygon.second;

    new_triangles.push_back(new Triangle(it_left, points[0], it_right, 0));
    
    //add to degree
    bool triangulated = false;
    int count = points.size() - 3; //-2 -1 as one triangle already added.


    while(triangulated == false)
    {
        if(count == 0)       //(*it_left)->current_polygon.first == (*it_right) logical check.
        {
            triangulated = true;
        }
        else
        {
            count --;
            new_triangles.push_back(new Triangle(it_left->current_polygon.first, it_left, it_right, 1));
            
            it_left = it_left->current_polygon.first;
        
        }

        if(count == 0)
        {
            triangulated = true;
        }
        else
        {
            count --;
            new_triangles.push_back(new Triangle(it_right, (it_right)->current_polygon.second, (it_left),1));
            it_right = (it_right)->current_polygon.second;
        }
            
    }

    for(int i=0; i<points.size();i++)
    {
        points[i]->current_polygon.first = nullptr;
        points[i]->current_polygon.second = nullptr;
    }

    return new_triangles;

}

bool onSegment(Point p, Point q, Point r) 
{ 
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && 
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y)) 
       return true; 
  
    return false; 
} 
  
 
int orientation(Point p, Point q, Point r) 
{ 
    float val = (q.y - p.y) * (r.x - q.x) - 
              (q.x - p.x) * (r.y - q.y); 
  
    if (val == 0) return 0;  // colinear 
  
    return (val > 0)? 1: 2; // clock or counterclock wise 
} 


bool doIntersect(Point p1, Point q1, Point p2, Point q2) 
{ 

    int o1 = orientation(p1, q1, p2); 
    int o2 = orientation(p1, q1, q2); 
    int o3 = orientation(p2, q2, p1); 
    int o4 = orientation(p2, q2, q1); 
 
    // General case 
    if (o1 != o2 && o3 != o4) 
        return true; 
  
    // Special Cases 
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1 
    if (o1 == 0 && onSegment(p1, p2, q1)) return true; 
  
    // p1, q1 and q2 are colinear and q2 lies on segment p1q1 
    if (o2 == 0 && onSegment(p1, q2, q1)) return true; 
  
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2 
    if (o3 == 0 && onSegment(p2, p1, q2)) return true; 
  
     // p2, q2 and q1 are colinear and q1 lies on segment p2q2 
    if (o4 == 0 && onSegment(p2, q1, q2)) return true; 
  
    return false; // Doesn't fall in any of the above cases 
} 

bool intersecting(Edge *e1, Edge *e2)
{
    Point e11, e12, e21, e22;
    e11.x = e1->v1->x;
    e11.y = e1->v1->y;
    e12.x = e1->v2->x;
    e12.y = e1->v2->y;
    e21.x = e2->v1->x;
    e21.y = e2->v1->y;
    e22.x = e2->v2->x;
    e22.y = e2->v2->y;

    return doIntersect(e11, e12, e21, e22);
}

void Model::countIntersection(Node* v, vector<Triangle*> triangles)
{
    vector<Triangle*>::iterator vit;
    vector<Triangle*>::iterator newit;
    for(newit = triangles.begin(); newit != triangles.end(); newit += 1 )
    {
        for(vit = v->triangle_list.begin(); vit!=v->triangle_list.end(); vit += 1)
        {
            for(int i=0; i<3;i++)
            {
                if((*vit)->edges[i]->cutting_edge == true)
                {
                    if(intersecting((*vit)->edges[i], (*newit)->edges[2]) == true && (*vit) != (*newit))
                    {
                        (*vit)->parents.push_back((*newit));
                        (*newit)->children.push_back((*vit));
                        break;
                        
                    }
                }
            }

        }
    }
}



void Model::searchForpoint(float x, float y)
{
    Node *temp_node = new Node(x, y);
    Node *inf_node = new Node(10000.0f, y);
    Edge *temp_edge = new Edge(temp_node, inf_node);
    Triangle *container; 
    Triangle* child;
    int count = 0;


    Tlevel* current_level = levels[levels.size()-1];
    int rang = current_level->triangles.size();
    for(int i=0; i<rang ; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(intersecting(current_level->triangles[i]->edges[j], temp_edge) == true)
            {
                count++;
            }
        }
        if(count == 1)
        {
            container = current_level->triangles[i];
            container->does_contain = true;
            container->printTriangle();
            count = 0;
            break;
        }
        else
        {
            count = 0;
        }
    }

    bool runner = true;
    while(!(container->children.empty()) && runner == true) //assuming previous loop has found the container. 
    {
        for(int i=0; i<container->children.size(); i++) 
        {

            child = container->children[i];
            
            for(int j=0; j<3; j++)
            {
                if(intersecting(child->edges[j], temp_edge) == true)
                {
                    count++;
                }
            

            }
            if(count == 1 && child != container)
            {
                container = child;
                container->does_contain = true;
                container->printTriangle();
                count = 0;
                break;
            }
            else
            {
                count = 0;
            }
        }

        for(int i =0; i<levels[0]->triangles.size(); i++)
        {
            if(levels[0]->triangles[i] == container)
            {
                runner = false;
                break;
            }
        }
        
    }

}

void Model::setBuffers(int x)
{
    for(int i = 0; i<levels.size(); i++)
    {
        levels[i]->setBuffer(x);
    }
}

void Model::DrawLevel(Shader ourshader)
{
    if(curLevel> levels.size()-1)
    {
        curLevel = levels.size() -1;
    }
    else if(curLevel < 0)
    {
        curLevel = 0;
    }

    levels[curLevel]->Draw(ourshader);
}


glm::vec3 normalised(vec3 val)
{
    vec3 temp1, temp2;
    temp1.x = (-(1400 - 2*(val.x))/1400);
    temp1.y = ((1400 - 2*(val.y))/1400);
    temp1.z = 0.0f;
    //temp1 = normalize(val);
    return temp1;
} 

void Tlevel::setBuffer(int setter)
{
    float x, y;
    vec3 vertex;

    if(setter == 1)
    {
        colours.clear();
    }
    //add triangle nodes to vertices.
    for(auto it= triangles.begin(); it != triangles.end(); it++)
    {
        x = (*it)->v1->x;
        y = (*it)->v1->y;
        vertex = vec3(x, y, 0.0f);
       //normalize(vertex);
        vertex = normalised(vertex);
        vertices.push_back(vertex);

        x = (*it)->v2->x;
        y = (*it)->v2->y;
       
        vertex = vec3(x, y, 0.0f);
        vertex = normalised(vertex);
        
 
        vertices.push_back(vertex);
        x = (*it)->v3->x;
        y = (*it)->v3->y;
        vertex = vec3(x, y, 0.0f);
        vertex = normalised(vertex);
        
        vertices.push_back(vertex);

        if((*it)->does_contain == true)
        {   
            colours.push_back(vec3(0.0f, 1.0f, 0.3f));
            colours.push_back(vec3(0.0f, 1.0f, 0.3f));
            colours.push_back(vec3(0.0f, 1.0f, 0.3f));
        }
        else
        {
            colours.push_back(vec3(1.0f, 0.5f, 0.2f));
            colours.push_back(vec3(1.0f, 0.5f, 0.2f));
            colours.push_back(vec3(1.0f, 0.5f, 0.2f));
        }
        
    }

    //setting up the array object.
    glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    glEnableVertexAttribArray(0);

    glGenBuffers(1, &VCO);
    glBindBuffer(GL_ARRAY_BUFFER, VCO);
    glBufferData(GL_ARRAY_BUFFER, colours.size()*sizeof(glm::vec3), &colours[0], GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    glEnableVertexAttribArray(1);

}

void Tlevel::Draw(Shader ourshader)
{
    modelMatrix = mat4(1.0f);
    ourshader.use();
    ourshader.setMat4("model", modelMatrix);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0 ,vertices.size());
}



