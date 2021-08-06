#ifndef Model_H
#define Model_H
#pragma once

#include<GLFW/glfw3.h>
#include<vector>
#include<set>

#include <GL/glew.h>

#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<iostream>
#include"../include/shader_s.hpp"


using namespace glm;
using namespace std;

class Triangle;
class Tlevel;

struct Point
{
    float x;
    float y;
};

class Node
{
    public :

        Node(float a, float b);         

        float x, y;
        vector <Node*> connected_list; 
        int degree;
        vector <Triangle*> triangle_list;

        bool marked; //If adjacent in this iteration.
        bool in_IS;          
        bool is_deleted; //If candidate for IS
        bool is_hullpoint;
        pair <Node*, Node*> current_polygon; 

        int to_pair;   //not used till now?

        void printNode()
        {
            cout<<x<<" "<<y<<endl;
        }

};

class Edge
{
    public :

        Edge(Node* a, Node* b);

        Node *v1;
        Node *v2;

        bool cutting_edge;

        void printEdge()
        {
            v1->printNode();
            v2->printNode();

        }
};

class Triangle
{
    public :

        Triangle(Edge* l1, Edge* l2, Edge *l3, Node *a1, Node *a2, Node *a3);
        Triangle(Node *a1, Node* a2, Node* a3, int);
        Node *v1;
        Node *v2;
        Node *v3;

        vector <Edge*> edges;

        vector <Triangle*> parents;
        vector <Triangle*> children;

        bool to_remove; //If one of its points are in IS.
        bool removed; //Do not consider it, not present in this level.
        bool in_polygon;
        bool does_contain;



        void printTriangle()
        {
            cout<<"Node 1: ";
            v1->printNode();
            cout<<"Node 2: ";
            v2->printNode();
            cout<<"Node 3: ";
            v3->printNode();
        }

};

class Model
{
    public :

    Model();

    vector <vec3> vertices;
    vector <Triangle*> triangles; 

    Triangle *rootTriangle;

    vector <Tlevel*> levels; 
    int current_level;
    vector <Node*> all_nodes;
    vector <Node*> remaining_nodes;
    vector <Node*> IS;
    vector <Node*> IS_count;
    vector <Node*> makeHole(Node*);
    vector <Triangle*> fixHole(vector <Node*> points, Node *v);
    
    Node* createNode(float, float);
    void createTriangle(Node*, Node*, Node*);
    
    void getTriangle(Point points[], int clicks[]);
    Node * nodes[3];
    int allow_error, cur_error;
    bool next_it;
    
    void startbuild();
    int startitr;

    vector<Node*> createIS();
    void deleteIS(Tlevel*);
    void countIntersection(Node* v, vector<Triangle*>);
    vector <Node*>::iterator itr;

    void searchForpoint(float, float);

    void DrawLevel(Shader ourshader);
    void setBuffers(int x);

    int curLevel;
    bool indicator;
    
};

class Tlevel
{
    public :

        vector <Triangle*> triangles; 
        
        vector<vec3> vertices;
        vector<vec3> colours;
        GLuint VAO, VBO, VCO;

        void setBuffer(int x);
        void Draw(Shader ourShader);
        
        void print_level()
        {
            for(int i=0;i<triangles.size();i++)
            {
                cout<<i<<endl;
                triangles[i]->printTriangle();
            }
        }

    glm::mat4 modelMatrix;
        glm::mat4 modelInitial;
        glm::mat4 scaleMatrix;
        glm::mat4 translateMatrix;
        glm::mat4 rotateMatrix;
        
};

#endif