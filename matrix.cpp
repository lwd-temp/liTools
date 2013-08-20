//Matrix math functions for usability of XML
//Thanks to Mygod for providing!

#include "pakDataTypes.h"
#include <sstream>
#include <cmath>
#include <cfloat>

#define PI 3.1415926535
#define EPSILON	0.001

float DegreeToRadian(float fAngle)
{
	return fAngle * PI / 180.0;
}

float RadianToDegree(float fAngle)
{
	return fAngle * 180.0 / PI;
}

mtx23 RotateTransform(float theta)
{
	float radian = DegreeToRadian(theta);
	float cosine = cos(radian);
	float sine = sin(radian);
	mtx23 mat;
	mat._11 = cosine;
	mat._12 = -sine;
	mat._13 = 0;
	mat._21 = sine;
	mat._22 = cosine;
	mat._23 = 0;
	return mat;
}

mtx23 ScaleTransform(float scale_x, float scale_y)
{
	mtx23 mat;
	mat._11 = scale_x;
	mat._12 = mat._13 = 0;
	mat._21 = 0;
	mat._22 = scale_y;
	mat._23 = 0;
	return mat;
}

mtx23 SkewTransform(float angle_x, float angle_y)
{
	float skew_x = tan(DegreeToRadian(angle_x));
	float skew_y = tan(DegreeToRadian(angle_y));
	mtx23 mat;
	mat._11 = 1;
	mat._12 = skew_x;
	mat._13 = 0;
	mat._21 = skew_y;
	mat._22 = 1;
	mat._23 = 0;
	return mat;
}

mtx23 TranslateTransform(float offset_x, float offset_y)
{
	mtx23 mat;
	mat._11 = 1;
	mat._12 = 0;
	mat._13 = offset_x;
	mat._21 = 0;
	mat._22 = 1;
	mat._23 = offset_y;
	return mat;
}

int Sign(float f)
{
	if(f < 0)
		return -1;
	return 1;
}

bool FloatEquals(float a, float b)
{
	float diff = abs(a-b);
	if(diff < EPSILON)	//Very small #
		return true;
	return false;
}

void WriteTransforms(XMLElement* elemParent, XMLDocument* doc, mtx23 mat)
{
	XMLElement* elem = doc->NewElement("transforms");
	//var result = new StringBuilder("<transforms>\r\n");
	if ((abs(mat._11) < EPSILON || abs(mat._22) < EPSILON)    // the only kind of matrices that cannot be processed so far
		&& (abs(mat._11) > EPSILON || abs(mat._22) > EPSILON || Sign(mat._12) != -Sign(mat._21)))
	{
		// if this condition (after &&) is false, the transform is rotating �90� which is able to be processed
		//result.AppendLine(string.Format("  <matrix matrix='{0},{1},{2},{3},{4},{5}' />", _11, _12, _13, _21, _22, _23));
		XMLElement* elem2 = doc->NewElement("matrix");
		ostringstream oss;
		oss << mat._11 << ", " << mat._12 << ", " << mat._13 << ", " << mat._21 << ", " << mat._22 << ", " << mat._23;
		elem2->SetAttribute("matrix", oss.str().c_str());
		elem->InsertEndChild(elem2);
	}
	else
	{
		float ratio_a = mat._21 / mat._11, ratio_b = -(mat._12 / mat._22); // if it is rotation, ratio_a=ratio_b=sin/cos=tan
		if (FloatEquals(ratio_a, ratio_b)) // SCALE*ROTATE
		{
			float theta = atan(ratio_a), cosine = cos(theta), sine = sin(theta), 
				   scale_x = (abs(mat._11) > EPSILON) ? (mat._11 / cosine) : (mat._21 / sine),
				   scale_y = (abs(mat._22) > EPSILON) ? (mat._22 / cosine) : (-mat._12 / sine);
			if (abs(scale_x - 1.0) > EPSILON || abs(scale_y - 1.0) > EPSILON)
			{
				//result.AppendLine(string.Format("  <scale factor='{0},{1}' />", scale_x, scale_y));
				XMLElement* elem2 = doc->NewElement("scale");
				ostringstream oss;
				oss << scale_x << ", " << scale_y;
				elem2->SetAttribute("factor", oss.str().c_str());
				elem->InsertEndChild(elem2);
			}
			if (abs(theta) > EPSILON) 
			{
				//result.AppendLine(string.Format("  <rotate angle='{0}' />", RadianToDegree(theta)));
				XMLElement* elem2 = doc->NewElement("rotate");
				elem2->SetAttribute("angle", RadianToDegree(theta));
				elem->InsertEndChild(elem2);
			}
		}
		else                                                                // SCALE*SKEW
		{
			float scale_x = mat._11, scale_y = mat._22, skew_x = mat._12 / scale_y, skew_y = mat._21 / scale_x;
			if (abs(scale_x - 1.0) > EPSILON || abs(scale_y - 1.0) > EPSILON)
			{
				//result.AppendLine(string.Format("  <scale factor='{0},{1}' />", scale_x, scale_y));
				XMLElement* elem2 = doc->NewElement("scale");
				ostringstream oss;
				oss << scale_x << ", " << scale_y;
				elem2->SetAttribute("factor", oss.str().c_str());
				elem->InsertEndChild(elem2);
			}
			if (abs(skew_x) > EPSILON || abs(skew_y) > EPSILON) 
			{
				//result.AppendLine(string.Format("  <skew angle='{0},{1}' />", RadianToDegree(Math.Atan(skew_x)), RadianToDegree(Math.Atan(skew_y))));
				XMLElement* elem2 = doc->NewElement("skew");
				ostringstream oss;
				oss << RadianToDegree(atan(skew_x)) << ", " << RadianToDegree(atan(skew_y));
				elem2->SetAttribute("angle", oss.str().c_str());
				elem->InsertEndChild(elem2);
			}
		}
		if (abs(mat._13) > EPSILON || abs(mat._23) > EPSILON)
		{
			//result.AppendLine(string.Format("  <translate offset='{0},{1}' />", _13, _23));    // *TRANSLATE
			XMLElement* elem2 = doc->NewElement("translate");
			ostringstream oss;
			oss << mat._13 << ", " << mat._23;
			elem2->SetAttribute("offset", oss.str().c_str());
			elem->InsertEndChild(elem2);
		}
	}
	elemParent->InsertEndChild(elem);
	//result.AppendLine("</transforms>");
	//return result.ToString();
}

void ParseTransforms(/*TODO*/)
{
	/*var root = XDocument.Parse(xml).Root;
	var transform = DoNothingTransform;
	foreach (var child in root.Elements())
	{
		Matrix matrix;
		switch (child.Name.LocalName)
		{
			case "rotate":
				matrix = RotateTransform(double.Parse(child.Attribute("angle").Value));
				break;
			case "scale":
				var scales = child.Attribute("factor").Value.Split(',');
				matrix = ScaleTransform(double.Parse(scales[0]), double.Parse(scales[1]));
				break;
			case "skew":
				var angles = child.Attribute("angle").Value.Split(',');
				matrix = SkewTransform(double.Parse(angles[0]), double.Parse(angles[1]));
				break;
			case "translate":
				var offsets = child.Attribute("offset").Value.Split(',');
				matrix = TranslateTransform(double.Parse(offsets[0]), double.Parse(offsets[1]));
				break;
			case "matrix":
				var m = child.Attribute("matrix").Value.Split(',');
				matrix = new Matrix(double.Parse(m[0]), double.Parse(m[1]), double.Parse(m[2]),
									double.Parse(m[3]), double.Parse(m[4]), double.Parse(m[5]));
				break;
			default: throw new NotSupportedException("Unknown element name: " + child.Name.LocalName);
		}
		transform = matrix * transform;
	}
	return transform;*/
}

/* TODO

public static Matrix operator *(Matrix a, Matrix b)
        {
            // the simplest O(n^3) naive matrix multiplication, the _3y items has been replaced by the constants
            // more details here: http://en.wikipedia.org/wiki/Matrix_multiplication#Algorithms_for_efficient_matrix_multiplication
            return new Matrix(a._11 * b._11 + a._12 * b._21, a._11 * b._12 + a._12 * b._22, a._11 * b._13 + a._12 * b._23 + a._13,
                              a._21 * b._11 + a._22 * b._21, a._21 * b._12 + a._22 * b._22, a._21 * b._13 + a._22 * b._23 + a._23);
        }
		
*/