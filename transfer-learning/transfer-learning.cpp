// transfer-learning.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "../Model/resnet.h"
#include "ImageNetDataSet.h"


using namespace vision::models;

template <typename Model>
void convert_and_save_model(
	const std::string& input_path,
	const std::string& output_path) {
	Model network;
	torch::load(network, input_path);
	torch::save(network, output_path);

	auto index = input_path.find("_python");
	auto name = input_path.substr(0, index);
	std::cout << "finished loading and saving " << name << std::endl;
}

int main()
{
	torch::manual_seed(1);
	torch::DeviceType device_type;
	if (torch::cuda::is_available()) {
		std::cout << "CUDA available! Training on GPU." << std::endl;
		device_type = torch::kCUDA;
	}
	else {
		std::cout << "Training on CPU." << std::endl;
		device_type = torch::kCPU;
	}

	try
	{

		//convert_and_save_model<ResNet34>("script_module.pt","ResNet18.pt");
		ResNet34 network;
		torch::load(network, "script_module.pt");
	}
	catch (std::exception ex)
	{
		std::cout << ex.what();
	}

	//torch::load(resnet, "resnet18_without_last_layer.pt");
	//model->fc->pretty_print(std::cout);
}

