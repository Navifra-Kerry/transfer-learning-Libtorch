// transfer-learning.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "../Model/resnet.h"
#include "ImageNetDataSet.h"

float best_accuracy = 0.0;
size_t index = 0;
torch::DeviceType device_type;

template< typename Net,typename Dataloader>
void train(Net network, Dataloader& data_loader, torch::optim::Optimizer& optimizer, size_t dataset_size)
{
	/*
	 This function trains the network on our data loader using optimizer.

	 Also saves the model as model.pt after every epoch.
	 Parameters
	 ===========
	 1. network  - Pre-trained model
	 2. data_loader (DataLoader& type) - Training data loader
	 3. optimizer (torch::optim::Optimizer& type) - Optimizer like Adam, SGD etc.
	 4. size_t (dataset_size type) - Size of training dataset

	 Returns
	 ===========
	 Nothing (void)
	 */
	network->train();

	int batch_index = 0;

	float mse = 0;
	float Acc = 0.0;

	for (auto& batch : *data_loader) {
		auto data = batch.data;
		auto targets = batch.target.squeeze();

		// Should be of length: batch_size
		data = data.to(torch::kF32);
		targets = targets.to(torch::kInt64);

		data = data.to(device_type);
		targets = targets.to(device_type);

		optimizer.zero_grad();

		auto output = network->forward(data);

		auto loss = torch::nll_loss(torch::log_softmax(output, 1), targets);

		loss.backward();
		optimizer.step();

		auto acc = output.argmax(1).eq(targets).sum();

		Acc += acc.template item<float>();
		mse += loss.template item<float>();

		batch_index += 1;
	}

	mse = mse / float(batch_index); // Take mean of loss
	std::cout << "Epoch: " << index << ", " << "Accuracy: " << Acc / dataset_size << ", " << "MSE: " << mse << std::endl;

}

template<typename Net, typename Dataloader>
void test(Net network, Dataloader& loader, size_t data_size) {
	/*
	 Function to test the network on test data

	 Parameters
	 ===========
	 1. network  - Pre-trained model
	 2. loader (Dataloader& type) - test data loader
	 3. data_size (size_t type) - test data size

	 Returns
	 ===========
	 Nothing (void)
	 */
	torch::NoGradGuard no_grad;
	network->eval();

	float Loss = 0, Acc = 0;

	for (const auto& batch : *loader) {
		auto data = batch.data;
		auto targets = batch.target.squeeze();

		data = data.to(torch::kF32);
		targets = targets.to(torch::kInt64);

		data = data.to(device_type);
		targets = targets.to(device_type);

		auto output = network->forward(data);

		auto loss = torch::nll_loss(torch::log_softmax(output, 1), targets);
		auto acc = output.argmax(1).eq(targets).sum();
		Loss += loss.template item<float>();
		Acc += acc.template item<float>();
	}

	std::cout << "Test Loss: " << Loss / data_size << ", Acc:" << Acc / data_size << std::endl;

	if (Acc / data_size > best_accuracy) {
		best_accuracy = Acc / data_size;
		std::cout << "Saving model" << std::endl;
		torch::save(network, "model.pt");
	}
}

const int64_t kTrainBatchSize = 512;
const int64_t kTestBatchSize = 512;
const int64_t kNumberOfEpochs = 30;

int main()
{
	torch::manual_seed(1);
	
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
		ResNet18 network;
		torch::load(network, "../../Model/resnet18_Python.pt");

		network->unregister_module("fc");

		network->fc = torch::nn::Linear(torch::nn::LinearImpl(512, 2));	

		network->register_module("fc", network->fc);

		network->to(device_type);

		auto train_dataset = ImageNetDataSet("../../sample/train/train_map.txt") 
			.map(torch::data::transforms::Stack<>());
		const size_t train_dataset_size = train_dataset.size().value();

		auto train_loader = torch::data::make_data_loader<torch::data::samplers::RandomSampler>(std::move(train_dataset),
			torch::data::DataLoaderOptions().batch_size(kTrainBatchSize).workers(0));

		auto test_dataset = ImageNetDataSet("../../sample/test/test_map.txt")
			.map(torch::data::transforms::Stack<>());
		const size_t test_dataset_size = test_dataset.size().value();
		auto test_loader = torch::data::make_data_loader(std::move(test_dataset), kTestBatchSize);

		std::vector<torch::Tensor> trainable_params;
		auto params = network->named_parameters(true /*recurse*/);
		for (auto& param : params)
		{
			auto layer_name = param.key();
			if ("fc.weight" == layer_name || "fc.bias" == layer_name)
			{
				param.value().set_requires_grad(true);
				trainable_params.push_back(param.value());
			}
			else
			{
				param.value().set_requires_grad(false);
			}
		}

		torch::optim::Adam opt(trainable_params, torch::optim::AdamOptions(1e-3 /*learning rate*/));
			
		for (index = 0; index < kNumberOfEpochs; index++)
		{
			train(network, train_loader, opt, train_dataset_size);
			test(network, test_loader, test_dataset_size);
		}

	}
	catch (std::exception ex)
	{
		std::cout << ex.what();
	}
}

