/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TORUS_H
#define TORUS_H

//Torus model made out of triangles
//Contains arrays:
//vertices - vertex positions in homogenous coordinates
//normals - vertex normals in homogenous coordinates
//texCoords - texturing coordinates


#include "model.h"


namespace Models {
	
	using namespace std;
	using namespace glm;
	
	class Torus: public Model {
	
		public:
			Torus();
			Torus(float R,float r,float mainDivs,float tubeDivs);
			virtual ~Torus();
			virtual void drawSolid(bool smooth=true);
			
		private:
			vector<vec4> internalVertices;
			vector<vec4> internalFaceNormals;
			vector<vec4> internalVertexNormals;

			inline float d2r(float deg);
			vec4 generateTorusPoint(float R,float r,float alpha,float beta);
			vec4 computeVertexNormal(float alpha,float beta);
			vec4 computeFaceNormal(vector<vec4> &face);
			void generateTorusFace(vector<vec4> &vertices, vector<vec4> &vertexNormals, vec4 &faceNormal,float R,float r,float alpha,float beta,float step_alpha,float step_beta);
			void buildTorus(float R,float r,float mainDivs,float tubeDivs);
			
	};
	
	extern Torus torus;

}

#endif