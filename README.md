# A Clique-based Approach for Co-location Pattern Mining

Implementation của thuật toán **Instances Driven Schema (IDS)** bằng C++ cho bài toán khai phá co-location pattern trong dữ liệu không gian.

## 📖 Giới thiệu

Co-location pattern mining là kỹ thuật phát hiện các tập đặc trưng không gian (spatial features) thường xuyên xuất hiện gần nhau trong không gian địa lý. Thuật toán IDS sử dụng cách tiếp cận dựa trên clique để tìm kiếm các pattern này một cách hiệu quả.

### Tham khảo
> Yoo, J. S., & Shekhar, S. (2006). A joinless approach for mining spatial colocation patterns. *IEEE Transactions on Knowledge and Data Engineering*, 18(10), 1323-1337.

## 🏗️ Cấu trúc dự án

```
├── data/                    # Dữ liệu đầu vào
│   ├── LasVegas_x_y_alphabet_version_03_2.csv
│   └── sample_data.csv
├── src/
│   ├── config.txt           # File cấu hình
│   ├── include/             # Header files
│   │   ├── config.h
│   │   ├── csv.hpp
│   │   ├── data_loader.h
│   │   ├── miner.h
│   │   ├── neighborhood_mgr.h
│   │   ├── spatial_index.h
│   │   ├── types.h
│   │   └── utils.h
│   └── src/                 # Source files
│       ├── main.cpp
│       ├── config.cpp
│       ├── data_loader.cpp
│       ├── miner.cpp
│       ├── neighborhood_mgr.cpp
│       ├── spatial_index.cpp
│       └── utils.cpp
└── results/                 # Kết quả đầu ra
```

## ⚙️ Cấu hình

Chỉnh sửa file `src/config.txt`:

```ini
# I/O Paths
dataset_path=data/LasVegas_x_y_alphabet_version_03_2.csv
output_path=results/colocation_rules.txt

# Algorithm Thresholds
neighbor_distance=160        # Khoảng cách láng giềng
min_prevalence=0.2           # Ngưỡng prevalence tối thiểu
min_cond_prob=0.5            # Xác suất điều kiện tối thiểu

# Debug
debug_mode=true
```

## 🚀 Biên dịch và chạy

### Yêu cầu
- C++11 hoặc cao hơn
- CMake (tùy chọn)

### Biên dịch

```bash
cd src
g++ -std=c++11 -O2 -o colocation_miner src/*.cpp -I include/
```

### Chạy

```bash
./colocation_miner
```

## 📊 Định dạng dữ liệu đầu vào

File CSV với các cột:
- `instance_id`: ID của instance
- `feature_type`: Loại đặc trưng (A, B, C, ...)
- `x`: Tọa độ x
- `y`: Tọa độ y

Ví dụ:
```csv
instance_id,feature_type,x,y
1,A,10.5,20.3
2,B,11.2,21.0
3,A,50.0,60.0
```

## 📝 Thuật toán IDS

1. **Xây dựng Neighborhood Graph**: Tạo đồ thị láng giềng dựa trên khoảng cách
2. **Tìm Clique Instances**: Phát hiện các row instances dựa trên clique
3. **Tính Participation Index**: Đo lường mức độ phổ biến của pattern
4. **Lọc Pattern**: Chỉ giữ lại các pattern có prevalence >= ngưỡng

## 👥 Thành viên

1. Nguyễn Văn Thức
2. Phạm Xuân Khang
3. Dương Văn Hiệp
4. Phạm Thành Duy

## 📄 License

MIT License

