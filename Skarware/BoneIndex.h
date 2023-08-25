Vector3 GetBoneWithRotation(DWORD_PTR mesh, int index)
{
	uintptr_t bonearray = 0;
	bonearray = read<uintptr_t>(mesh + 0x600);
	if (!bonearray) bonearray = read<uintptr_t>(mesh + 0x600 + 0x10);
	FTransform ComponentToWorld = read<FTransform>(mesh + 0x240);
	FTransform bone = read<FTransform>(bonearray + (index * 0x600));
	D3DMATRIX Matrix;
	Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}

