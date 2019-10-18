#include "ImageNetDataSet.h"

#include <fstream>


#define message_assert(expr, message)      \
    if (!static_cast<bool>(expr))          \
    {                                      \
        std::cerr << message << std::endl; \
        assert(expr);                      \
    }

torch::Tensor read_data(std::string location) {
	/*
	 Function to return image read at location given as type torch::Tensor
	 Resizes image to (224, 224, 3)
	 Parameters
	 ===========
	 1. location (std::string type) - required to load image from the location

	 Returns
	 ===========
	 torch::Tensor type - image read as tensor
	*/
	cv::Mat img = cv::imread(location, 1);
	cv::resize(img, img, cv::Size(224, 224), cv::INTER_CUBIC);
	torch::Tensor img_tensor = torch::from_blob(img.data, { img.rows, img.cols, 3 }, torch::kByte);
	img_tensor = img_tensor.permute({ 2, 0, 1 });
	return img_tensor.clone();
}

torch::Tensor read_label(int label) {
	/*
	 Function to return label from int (0, 1 for binary and 0, 1, ..., n-1 for n-class classification) as type torch::Tensor
	 Parameters
	 ===========
	 1. label (int type) - required to convert int to tensor

	 Returns
	 ===========
	 torch::Tensor type - label read as tensor
	*/
	torch::Tensor label_tensor = torch::full({ 1 }, label);
	return label_tensor.clone();
}

std::tuple<std::vector<torch::Tensor>, std::vector<torch::Tensor>> read_map(std::string map_file)
{
	std::ifstream stream;
	stream.open(map_file);

	message_assert(stream.is_open(), "error: failed to read info file \""
		<< map_file << "\"");

	std::string path, id;
	std::vector<torch::Tensor> states , labels;

	while (true)
	{
		path.clear();
		stream >> path >> id;
		if (path.empty()) break;

		torch::Tensor img = read_data(path);
		states.push_back(img);

		torch::Tensor label = read_label(std::stoi(id));
		labels.push_back(label);


		if (stream.peek() == EOF || stream.eof()) break;
	}

	stream.close();

	return { states , labels };
}

