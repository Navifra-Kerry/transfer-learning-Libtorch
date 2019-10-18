#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <torch/torch.h>

// Function to return image read at location given as type torch::Tensor
// Resizes image to (224, 224, 3)
torch::Tensor read_data(std::string location);

// Function to return label from int (0, 1 for binary and 0, 1, ..., n-1 for n-class classification) as type torch::Tensor
torch::Tensor read_label(int label);

std::tuple<std::vector<torch::Tensor>, std::vector<torch::Tensor>> read_map(std::string map_file);

// Custom Dataset class
class ImageNetDataSet : public torch::data::Dataset<ImageNetDataSet> {
private:
	/* data */
	// Should be 2 tensors
	std::vector<torch::Tensor> states, labels;
	size_t ds_size;
public:
	ImageNetDataSet(std::string map_file)
	{
		std::tie(states, labels) = read_map(map_file);
		ds_size = states.size();
	};

	torch::data::Example<> get(size_t index) override {
		/* This should return {torch::Tensor, torch::Tensor} */
		torch::Tensor sample_img = states.at(index);
		torch::Tensor sample_label = labels.at(index);
		return { sample_img.clone(), sample_label.clone() };
	};

	torch::optional<size_t> size() const override {
		return ds_size;
	};
};