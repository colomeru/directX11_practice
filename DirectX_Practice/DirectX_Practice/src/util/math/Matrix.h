#ifndef MATRIX_H_
#define MATRIX_H_

struct Vector3;
struct Quaternion;

struct Matrix {
	float m[4][4];

	// コンストラクタ
	Matrix(
		float m11 = 1.0f, float m12 = 0.0f, float m13 = 0.0f, float m14 = 0.0f,
		float m21 = 0.0f, float m22 = 1.0f, float m23 = 0.0f, float m24 = 0.0f,
		float m31 = 0.0f, float m32 = 0.0f, float m33 = 1.0f, float m34 = 0.0f,
		float m41 = 0.0f, float m42 = 0.0f, float m43 = 0.0f, float m44 = 1.0f);

	// スケーリング Matrix を作成します。
	static Matrix CreateScale(const Vector3& scale);
	// 移動 Matrix を作成します。
	static Matrix CreateTranslation(const Vector3& translation);
	// 任意のベクトルの周囲を回転する新しい Matrix を作成します
	static Matrix CreateFromAxisAngle(const Vector3& axis, float degree);
	// Quaternion から回転 Matrix を作成します
	static Matrix CreateFromQuaternion(const Quaternion& rotate);
	// 指定したヨー、ピッチ、ロールで行列を作成します
	static Matrix CreateFromYawPitchRoll(float yaw, float pitch, float roll);
	// x 軸を回転軸として頂点のセットを回転させるために使用できる行列を作成します
	static Matrix CreateRotationX(float degree);
	// y 軸を回転軸として頂点のセットを回転させるために使用できる行列を作成します
	static Matrix CreateRotationY(float degree);
	// z 軸を回転軸として頂点のセットを回転させるために使用できる行列を作成します
	static Matrix CreateRotationZ(float degree);
	// ワールド行列を作成します。
	static Matrix CreateWorld(const Vector3& scale, const Quaternion& rotation, const Vector3& translation);
	// ワールド行列を作成します。
	static Matrix CreateWorld(const Vector3& scale, const Matrix& rotate, const Vector3& translation);
	// ワールド行列を作成します。
	static Matrix CreateWorld(const Vector3& position, const Vector3& forward, const Vector3& up);
	// 視野に基づいて、パースペクティブ射影行列を作成します
	static Matrix CreatePerspectiveFieldOfView(float fov, float aspect, float near, float far);
	// ビュー行列を作成します
	static Matrix CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& up);
	// スクリーン行列を作成します
	static Matrix CreateScreen(float x, float y, float width, float height);

	// 逆行列を計算します。
	static Matrix Invert(const Matrix& matrix);
	// 
	static Matrix InvertFast(const Matrix& matrix);
	// 行列の行と列を入れ替えます。
	static Matrix Transpose(const Matrix& matrix);
	// 2つの行列の対応する値の間を線形補間します。
	static Matrix Lerp(const Matrix& value1, const Matrix& value2, float amount);

	// 3D スケール / 回転 / 変換(SRT) Matrix からスカラー、変換、回転のコンポーネントを取得します
	void Decompose(Vector3& scale, Quaternion& rotation, Vector3& translation) const;
	// 回転行列を正規化します。
	static Matrix NormalizeRotationMatrix(const Matrix& matrix);
	// 回転行列を正規化します。
	Matrix& NormalizeRotationMatrix();

	// 前方ベクトルを取得
	Vector3 Forward() const;
	// 前方ベクトルを設定
	Matrix& Forward(const Vector3& forward);
	// 後方ベクトルを取得
	Vector3 Backward() const;
	// 後方ベクトルを設定
	Matrix& Backward(const Vector3& backward);
	// 上方ベクトルを取得
	Vector3 Up() const;
	// 上方ベクトルを設定
	Matrix& Up(const Vector3& up);
	// 下方ベクトルを取得
	Vector3 Down() const;
	// 下方ベクトルを設定
	Matrix& Down(const Vector3& down);
	// 左ベクトルを取得
	Vector3 Left() const;
	// 左ベクトルを設定
	Matrix& Left(const Vector3& left);
	// 右ベクトルを取得
	Vector3 Right() const;
	// 右ベクトルを設定
	Matrix& Right(const Vector3& rignt);
	// 平行移動ベクトルを取得または設定します。
	Vector3 Translation() const;
	// 平行移動ベクトルを取得または設定します。
	Matrix& Translation(const Vector3& translation);
	// スケール値を取得
	Vector3 Scale() const;
	// 回転の取得
	Quaternion Rotation() const;
	// 回転行列の取得
	Matrix RotationMatrix() const;

	// 単位行列
	static const Matrix Identity;
	// ゼロ行列
	static const Matrix Zero;

#ifdef __DXLIB
	// Dxlib用変換関数
	explicit Matrix(const DxLib::MATRIX& mat) {
		memcpy(this, &mat, sizeof(Matrix));
	}
	operator const DxLib::MATRIX() const {
		DxLib::MATRIX result;
		memcpy(&result, this, sizeof(DxLib::MATRIX));
		return result;
	}
#endif
};

// 単項演算子オーバーロード
Matrix operator - (const Matrix& m);

// 代入演算子オーバーロード
Matrix& operator += (Matrix& m1, const Matrix& m2);
Matrix& operator -= (Matrix& m1, const Matrix& m2);
Matrix& operator *= (Matrix& m1, const Matrix& m2);
Matrix& operator *= (Matrix& m, float s);
Matrix& operator /= (Matrix& m, float s);

// ２項演算子オーバーロード
Matrix  operator * (Matrix m1, const Matrix& m2);
Matrix  operator * (Matrix m, float s);
Matrix  operator * (Matrix s, float m);
Vector3 operator * (const Vector3& v, const Matrix& m);
Vector3 operator * (const Matrix& m, const Vector3& v);

// ２項演算子オーバーロード
Matrix operator + (Matrix m1, const Matrix& m2);
Matrix operator - (Matrix m1, const Matrix& m2);
Matrix operator * (Matrix m, float s);
Matrix operator * (float s, Matrix m);
Matrix operator / (Matrix m, float s);

#endif
