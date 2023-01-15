import torch.nn as nn
import torch

__all__ = ['resnet18_se', 'resnet34_se']


class SEBlock(nn.Module):

    def __init__(self, channel, reduction=16):
        super(SEBlock, self).__init__()
        self.avg_pool = nn.AdaptiveAvgPool2d(1)
        self.fc = nn.Sequential(
            nn.Linear(channel, channel // reduction, bias=False), nn.ReLU(),
            nn.Linear(channel // reduction, channel, bias=False), nn.Sigmoid())

    def forward(self, x):
        b, c, _, _ = x.size()
        y = self.avg_pool(x).view(b, c)
        y = self.fc(y).view(b, c, 1, 1)
        return x * y.expand_as(x)


class BasicBlock(nn.Module):
    expansion = 1  # 是否可以调用

    def __init__(self,
                 in_channel,
                 out_channel,
                 stride=1,
                 downsample=None,
                 **kwargs):
        super(BasicBlock, self).__init__()
        self.conv1 = nn.Conv2d(in_channels=in_channel,
                               out_channels=out_channel,
                               kernel_size=3,
                               stride=stride,
                               padding=1,
                               bias=False)
        self.bn1 = nn.BatchNorm2d(out_channel)
        self.relu = nn.ReLU()
        self.conv2 = nn.Conv2d(in_channels=out_channel,
                               out_channels=out_channel,
                               kernel_size=3,
                               stride=1,
                               padding=1,
                               bias=False)
        self.bn2 = nn.BatchNorm2d(out_channel)
        self.downsample = downsample
        self.se = SEBlock(out_channel)

    def forward(self, x):
        identity = x
        if self.downsample is not None:
            identity = self.downsample(x)

        out = self.conv1(x)
        out = self.bn1(out)
        out = self.relu(out)

        out = self.conv2(out)
        out = self.bn2(out)

        out += identity
        out = self.relu(out)

        out = self.se(out)

        return out


class ResNet(nn.Module):

    def __init__(
            self,
            block,  # 残差结构
            blocks_num,  # 残差结构数目
            num_classes=12,
            include_top=True,
            groups=1,
            width_per_group=64):
        super(ResNet, self).__init__()
        self.include_top = include_top
        self.in_channel = 64  # 卷积核个数

        self.groups = groups
        self.width_per_group = width_per_group

        self.conv1 = nn.Conv2d(3,
                               self.in_channel,
                               kernel_size=(7, 7),
                               stride=(2, 2),
                               padding=3,
                               bias=False)
        self.bn1 = nn.BatchNorm2d(self.in_channel)
        self.relu = nn.ReLU(inplace=True)
        self.maxpool = nn.MaxPool2d(kernel_size=(3, 3),
                                    stride=(2, 2),
                                    padding=1)
        self.layer1 = self._make_layer(block, 64, blocks_num[0])
        self.layer2 = self._make_layer(block, 128, blocks_num[1], stride=2)
        self.layer3 = self._make_layer(block, 256, blocks_num[2], stride=2)
        self.layer4 = self._make_layer(block, 512, blocks_num[3], stride=2)
        if self.include_top:
            self.avgpool = nn.AdaptiveAvgPool2d((1, 1))  # output_size=(1,1)
            self.fc = nn.Linear(512 * block.expansion, num_classes)

        for m in self.modules():
            if isinstance(m, nn.Conv2d):
                nn.init.kaiming_normal_(m.weight,
                                        mode='fan_out',
                                        nonlinearity='relu')

    def _make_layer(self, block, channel, block_num, stride=1):
        downsample = None
        if stride != 1 or self.in_channel != channel * block.expansion:
            downsample = nn.Sequential(
                nn.Conv2d(self.in_channel,
                          channel * block.expansion,
                          kernel_size=(1, 1),
                          stride=stride,
                          bias=False),
                nn.BatchNorm2d(channel * block.expansion))

        layers = []
        layers.append(
            block(
                self.in_channel,  # 第一层虚线残差结构
                channel,
                downsample=downsample,
                stride=stride,
                groups=self.groups,
                width_per_group=self.width_per_group))
        self.in_channel = channel * block.expansion

        for _ in range(1, block_num):  # 后面全是实线残差结构
            layers.append(
                block(self.in_channel,
                      channel,
                      groups=self.groups,
                      width_per_group=self.width_per_group))
        return nn.Sequential(*layers)

    def forward(self, x):
        x = self.conv1(x)
        x = self.bn1(x)
        x = self.relu(x)
        x = self.maxpool(x)

        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        x = self.layer4(x)

        if self.include_top:
            x = self.avgpool(x)
            x = torch.flatten(x, 1)
            x = self.fc(x)

        return x


def resnet18_se(num_classes=12, include_top=True):
    return ResNet(BasicBlock, [2, 2, 2, 2],
                  num_classes=num_classes,
                  include_top=include_top)


def resnet34_se(num_classes=12, include_top=True):
    return ResNet(BasicBlock, [3, 4, 6, 3],
                  num_classes=num_classes,
                  include_top=include_top)
