/*
 * todo:
 * コンストラクタでパースというおマヌケ仕様をやめる
 * .mtlファイルの読み込み
 * v, vn, vt, f等の有効行を数えるだけの関数追加
 * まともなサンプルとまともなテスト
*/

#include <iostream>
#include <vector>

namespace Nano {
  class ObjParser {
    private:

    protected:
      virtual void vertex(float x, float y, float z) {
        std::cout << "vertex " << x << " " << y << " " << z << std::endl;
      }

      virtual void texture_vertex(float x, float y) {
        std::cout << "texture " << x << " " << y << std::endl;
      }

      virtual void normal_vertex(float x, float y, float z) {
        std::cout << "normal " << x << " " << y << " " << z << std::endl;
      }

      virtual void face(const std::vector<int>& vs, const std::vector<int>& vts, const std::vector<int>& vns) {
        std::cout << "face ";
        for(int i = 0; i < vs.size(); i++) {
          std::cout << vs[i]<< "/" << vts[i] << "/" << vns[i] << " ";
        }
        std::cout << " g:" << group;
        std::cout << " o:" << object;
        std::cout << " mtl:" << mtl << std::endl;
      }
      char object[1024];
      char group[1024];
      char mtl[1024];

    public:
      ObjParser(std::istream& is) {
        object[0] = 0;
        group[0] = 0;
        mtl[0] = 0;
        const int BUF_SIZE = 1024;
        char buf[BUF_SIZE], *s, *t;
        std::vector<char*> faces;
        std::vector<int> vs, vts, vns;
        while(!is.eof() && is.getline(buf, BUF_SIZE)) {
          switch(buf[0]) {
            case 'v':
              float x, y, z;
              switch(buf[1]) {
                case ' ':
                  sscanf(buf + 2, "%f%f%f", &x, &y, &z);
                  vertex(x, y, z);
                  break;
                case 'n':
                  sscanf(buf + 3, "%f%f%f", &x, &y, &z);
                  normal_vertex(x, y, z);
                  break;
                case 't':
                  sscanf(buf + 3, "%f%f", &x, &y);
                  texture_vertex(x, y);
                  break;
              }
              break;
            case 'f':
              s = strtok(buf + 2, " ");
              faces.clear();
              vs.clear();
              vts.clear();
              vns.clear();
              while(s) {
                faces.push_back(s);
                s = strtok(NULL, " ");
              }
              for(std::vector<char*>::iterator it = faces.begin(); it != faces.end(); it++) {
                int v, vn, vt;
                if(sscanf(*it, "%d/%d/%d", &v, &vt, &vn) != 3) {
                  if(sscanf(*it, "%d//%d", &v, &vn) != 2) {
                    vn = 0;
                    if(sscanf(*it, "%d/%d", &v, &vt) != 2) {
                      sscanf(*it, "%d", &v);
                      vt = 0;
                    }
                  } else {
                    vt = 0;
                  }
                }
                vs.push_back(v);
                vts.push_back(vt);
                vns.push_back(vn);
              }
              face(vs, vts, vns);
              break;
            case 'g':
              sscanf(buf + 2, "%1023s", group);
              break;
            case 'o':
              sscanf(buf + 2, "%1023s", object);
              break;
            case 'u':
              sscanf(buf, "usemtl %1023s", mtl);
              break;
            case 'm':
              // マテリアルの読み込み
              // sscanf(buf, "mtllib %1023s", mtllib);
              break;
          }
        }
      }

      virtual ~ObjParser() {
      }
  };
};
