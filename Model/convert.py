"""
This python script converts the network into Script Module
"""
import torch
from torchvision import models

# Download and load the pre-trained model
model = models.resnet18(pretrained=True)

# Set upgrading the gradients to False
for param in model.parameters():
	param.requires_grad = False

example_input = torch.rand(1, 3, 224, 224)
script_module = torch.jit.trace(model, example_input)
script_module.save('resnet18_Python.pt')