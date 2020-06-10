// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_W2ENT_MESH_FILE_LOADER_H_INCLUDED__
#define __C_W2ENT_MESH_FILE_LOADER_H_INCLUDED__

#include "IMeshLoader.h"
#include "irrString.h"
#include "SMesh.h"
#include "SAnimatedMesh.h"
#include "IMeshManipulator.h"
#include "ISkinnedMesh.h"

//#include "os.h"

#include "Log.h"



namespace irr
{

struct ChunkDescriptor
{
    int size;
    int adress;
};

struct PropertyHeader
{
    core::stringc propName;
    core::stringc propType;
    s32 propSize;
    u32 endPos;

    core::stringc toString()
    {
        return formatString("W2_PropertyHeader: propName = %s, propType = %s, propSize = %d, endPos = %d, startPos = %d", propName.c_str(), propType.c_str(), propSize, endPos, endPos-propSize);
    }
};

struct MeshData
{
    int nModel;
    core::array<int> nMat;
    ChunkDescriptor infos;
};

struct SubmeshData
{
    int vertexType;
    core::array<s32> dataI;
    core::array<u16> bonesId;
    u32 unk;
};

struct Material
{
    int id;
    video::SMaterial material;
};

struct BoneData
{
    core::stringc name;
    core::matrix4 matr;
};



namespace io
{
	class IFileSystem;
	class IReadFile;
} // end namespace io
namespace scene
{
class IMeshManipulator;

//! Meshloader capable of loading w2ent meshes.
class IO_MeshLoader_W2ENT : public IMeshLoader
{
public:

	//! Constructor
    IO_MeshLoader_W2ENT(scene::ISceneManager* smgr, io::IFileSystem* fs);

	//! returns true if the file maybe is able to be loaded by this class
	//! based on the file extension (e.g. ".cob")
	virtual bool isALoadableFileExtension(const io::path& filename) const;

	//! creates/loads an animated mesh from the file.
	//! \return Pointer to the created mesh. Returns 0 if loading failed.
	//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
	//! See IReferenceCounted::drop() for more information.
	virtual IAnimatedMesh* createMesh(io::IReadFile* file);


private:

    bool ReadPropertyHeader(io::IReadFile* file, PropertyHeader& propHeader);

    // Main function
	bool load(io::IReadFile* file);

    void CMesh(io::IReadFile* file, MeshData tmp);
    void loadStaticMesh(io::IReadFile* file, core::array<int> mats);
    void loadSubmeshes(io::IReadFile* file, core::array<int> meshData, core::array<SubmeshData> subMeshesData, core::array<int> mats);
    void vert_format(io::IReadFile* file);
    void loadSkinnedSubmeshes(io::IReadFile* file, core::array<int> meshData, core::array<SubmeshData> subMeshesData, core::array<int> mats, core::array<core::stringc> boneNames);

    void CMaterialInstance(io::IReadFile* file, ChunkDescriptor infos, int nMats);
    void XBM_CBitmapTexture(io::IReadFile* xbmFile, core::stringc xbm_file, ChunkDescriptor chunk, core::array<core::stringc> XbmStrings);
    void generateDDSFromXBM(core::stringc filepath, core::stringc ddsFilepath);

    void CSkeleton(io::IReadFile* file, ChunkDescriptor infos);

	bool find (core::array<core::stringc> stringVect, core::stringc name);

    void addVectorToLog(core::stringc name, core::vector3df vec);
    void addMatrixToLog(core::matrix4 mat);


    //void SkinSubmesh(SubmeshData dataSubMesh, core::array<core::array<unsigned char> > weighting);

    video::ITexture* getTexture(core::stringc textureFilepath);

    // Attributes
    scene::ISceneManager* SceneManager;
    io::IFileSystem* FileSystem;
    scene::ISkinnedMesh* AnimatedMesh;

    core::array<core::array<unsigned char> >IdLOD;

    s32 Version;
    core::array<core::stringc> Strings;
    core::array<core::stringc> Files;
    // Materials of the meshes
    core::array<Material> Materials;

    io::path ConfigGamePath;

    // Bones data
    //core::array<core::stringc> BonesName;
    //core::array<BoneData> BonesData;

    //DEBUG
    Log* log;


};

} // end namespace scene
} // end namespace irr

#endif
