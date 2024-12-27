#include "PrimitiveDrawer.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Game/System/GameSystem.h>

//============================================================================*/
//	PrimitiveDrawer classMethods
//============================================================================*/

PrimitiveDrawer* PrimitiveDrawer::GetInstance() {

	static PrimitiveDrawer instance;
	return &instance;
}

void PrimitiveDrawer::Init(PipelineManager* pipeline) {

	pipeline_ = pipeline;

	vertexBuffer_.CreateVertexBuffer(GraphicsEngine::GetDevice()->Get(), kMaxLineCount_ * kVertexCountLine_);

	lineMaterials_[LineColor::White].Init();
	lineMaterials_[LineColor::White].color = Color::White();

	lineMaterials_[LineColor::Black].Init();
	lineMaterials_[LineColor::Black].color = Color::Black();

	lineMaterials_[LineColor::Red].Init();
	lineMaterials_[LineColor::Red].color = Color::Red();

	lineMaterials_[LineColor::Green].Init();
	lineMaterials_[LineColor::Green].color = Color::Green();

	lineMaterials_[LineColor::Blue].Init();
	lineMaterials_[LineColor::Blue].color = Color::Blue();

	// 転送用の頂点数でresize
	vertexBuffer_.pos.resize(kMaxLineCount_ * kVertexCountLine_);

}

void PrimitiveDrawer::Update() {

	vertexBuffer_.TransferVectorData(vertexBuffer_.pos);

	for (auto& material : lineMaterials_) {

		material.second.Update();
	}

}

void PrimitiveDrawer::DrawLine3D(const Vector3& pointA, const Vector3& pointB, const LineColor& color) {

	assert(indexLine_ < kMaxLineCount_);

	auto commandList = GraphicsEngine::GetCommand()->GetCommandList();
	PrimitiveMaterial& material = lineMaterials_[color];

	vertexBuffer_.pos[indexLine_] = { pointA.x,pointA.y,pointA.z,1.0f };
	indexLine_++;
	vertexBuffer_.pos[indexLine_] = { pointB.x,pointB.y,pointB.z,1.0f };
	indexLine_++;

	pipeline_->SetRendererPipeline(commandList, PrimitiveLine, kBlendModeNormal);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	commandList->IASetVertexBuffers(0, 1, &vertexBuffer_.GetVertexBuffer());
	material.SetCommand(commandList);
	GameSystem::GameCamera()->GetCamera3D()->GetViewProBuffer().SetCommand(commandList);
	commandList->DrawInstanced(kVertexCountLine_, 1, indexLine_ - kVertexCountLine_, 0);
}

void PrimitiveDrawer::DrawGrid() {
#ifdef _DEBUG

	const uint32_t kSubdivision = 24;
	const float kGridHalfWidth = 32.0f;
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);

	for (uint32_t index = 0; index <= kSubdivision; ++index) {

		float offset = -kGridHalfWidth + index * kGridEvery;

		// 横
		Vector3 verticalStart(offset, 0.0f, kGridHalfWidth);
		Vector3 verticalEnd(offset, 0.0f, -kGridHalfWidth);
		DrawLine3D(verticalStart, verticalEnd, LineColor::White);

		// 縦
		Vector3 horizontalStart(-kGridHalfWidth, 0.0f, offset);
		Vector3 horizontalEnd(kGridHalfWidth, 0.0f, offset);
		DrawLine3D(horizontalStart, horizontalEnd, LineColor::White);
	}

#endif // _DEBUG
}

void PrimitiveDrawer::DrawSphere(float radius, const Vector3& centerPos, const LineColor& color) {
#ifdef _DEBUG

	const uint32_t kSubdivision = 8;                                         //* 分割数
	const float kLatEvery = std::numbers::pi_v<float> / kSubdivision;        //* 緯度
	const float kLonEvery = 2.0f * std::numbers::pi_v<float> / kSubdivision; //* 経度

	auto calculatePoint = [&](float lat, float lon) -> Vector3 {
		return {
			radius * std::cos(lat) * std::cos(lon),
			radius * std::sin(lat),
			radius * std::cos(lat) * std::sin(lon)
		};
		};

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -std::numbers::pi_v<float> / 2.0f + kLatEvery * latIndex;
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;

			Vector3 pointA = calculatePoint(lat, lon);
			Vector3 pointB = calculatePoint(lat + kLatEvery, lon);
			Vector3 pointC = calculatePoint(lat, lon + kLonEvery);

			DrawLine3D(pointA + centerPos, pointB + centerPos, color);
			DrawLine3D(pointA + centerPos, pointC + centerPos, color);
		}
	}

#endif // _DEBUG
}

void PrimitiveDrawer::DrawOBB(
	const Vector3& size, const Quaternion& rotate, const Vector3& centerPos, const LineColor& color) {
#ifdef _DEBUG

	const uint32_t vertexNum = 8;

	Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(rotate);

	Vector3 vertices[vertexNum];
	Vector3 halfSizeX = Vector3::Transform(Direction::Right(), rotateMatrix) * size.x;
	Vector3 halfSizeY = Vector3::Transform(Direction::Up(), rotateMatrix) * size.y;
	Vector3 halfSizeZ = Vector3::Transform(Direction::Forward(), rotateMatrix) * size.z;

	Vector3 offsets[vertexNum] = {
		{-1, -1, -1}, {-1,  1, -1}, {1, -1, -1}, {1,  1, -1},
		{-1, -1,  1}, {-1,  1,  1}, {1, -1,  1}, {1,  1,  1}
	};

	for (int i = 0; i < vertexNum; ++i) {

		Vector3 localVertex = offsets[i].x * halfSizeX +
			offsets[i].y * halfSizeY +
			offsets[i].z * halfSizeZ;
		vertices[i] = centerPos + localVertex;
	}

	int edges[12][2] = {
		{0, 1}, {1, 3}, {3, 2}, {2, 0},
		{4, 5}, {5, 7}, {7, 6}, {6, 4},
		{0, 4}, {1, 5}, {2, 6}, {3, 7}
	};

	for (int i = 0; i < 12; ++i) {

		int start = edges[i][0];
		int end = edges[i][1];

		DrawLine3D(vertices[start], vertices[end], color);
	}

#endif // _DEBUG
}

void PrimitiveDrawer::DrawAABB(const AABB& aabb, const LineColor& color) {
#ifdef _DEBUG

	// AABBの各頂点
	Vector3 min = aabb.GetMin();
	Vector3 max = aabb.GetMax();

	// AABBの各頂点
	std::vector<Vector3> vertices = {
		{min.x, min.y, min.z},
		{max.x, min.y, min.z},
		{min.x, max.y, min.z},
		{max.x, max.y, min.z},
		{min.x, min.y, max.z},
		{max.x, min.y, max.z},
		{min.x, max.y, max.z},
		{max.x, max.y, max.z} };

	// 各辺
	std::vector<std::pair<int, int>> edges = {
		{0, 1}, {1, 3}, {3, 2}, {2, 0}, // 前面
		{4, 5}, {5, 7}, {7, 6}, {6, 4}, // 背面
		{0, 4}, {1, 5}, {2, 6}, {3, 7}  // 前面と背面を繋ぐ辺
	};

	for (const auto& edge : edges) {

		const Vector3& start = vertices[edge.first];
		const Vector3& end = vertices[edge.second];

		// 各辺の描画
		DrawLine3D(start, end, color);
	}

#endif // _DEBUG
}

void PrimitiveDrawer::Reset() {

	indexLine_ = 0;
}