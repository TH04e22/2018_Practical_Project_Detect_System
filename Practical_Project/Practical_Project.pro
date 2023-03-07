#-------------------------------------------------
#
# Project created by QtCreator 2019-01-23T12:35:01
#
#-------------------------------------------------

QT       += core gui multimedia sql xml charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Practical_Project
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS OPENCV CUDNN _TIMESPEC_DEFINED \
     _SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS _CRT_RAND_S \
     GPU WIN32 NDEBUG _CONSOLE _LIB
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += "E:\College\Junior\Practical_Project\Practical_Project_2018\Practical_Project\3rdparty\include"
INCLUDEPATH += "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v9.0\include"
INCLUDEPATH += E:\College\Junior\Practical_Project\Practical_Project_2018\Practical_Project\yolo_src
INCLUDEPATH += C:\opencv_3.0\opencv\build\include

CONFIG += c++11

LIBS += -LE:\College\Junior\Practical_Project\Practical_Project_2018\Practical_Project\3rdparty\lib\x64
LIBS += -L"C:\opencv_3.0\opencv\build\x64\vc15\lib"
LIBS += -lpthreadVC2
LIBS += -lkernel32 \
        -luser32 \
        -lgdi32 \
        -lwinspool \
        -lcomdlg32 \
        -ladvapi32 \
        -lshell32 \
        -lole32 \
        -loleaut32 \
        -luuid \
        -lodbc32 \
        -lodbccp32 \
        -lopencv_world340d

LIBS += -L"C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v9.0/lib/x64"
LIBS += -lcublas \
        -lcurand \
        -lcudart

LIBS += "E:\College\Junior\Practical_Project\Practical_Project_2018\Practical_Project\cuda\activation_kernels.cu.obj" \
    "E:\College\Junior\Practical_Project\Practical_Project_2018\Practical_Project\cuda\avgpool_layer_kernels.cu.obj" \
    "E:\College\Junior\Practical_Project\Practical_Project_2018\Practical_Project\cuda\blas_kernels.cu.obj" \
    "E:\College\Junior\Practical_Project\Practical_Project_2018\Practical_Project\cuda\col2im_kernels.cu.obj" \
    "E:\College\Junior\Practical_Project\Practical_Project_2018\Practical_Project\cuda\convolutional_kernels.cu.obj" \
    "E:\College\Junior\Practical_Project\Practical_Project_2018\Practical_Project\cuda\crop_layer_kernels.cu.obj" \
    "E:\College\Junior\Practical_Project\Practical_Project_2018\Practical_Project\cuda\deconvolutional_kernels.cu.obj" \
    "E:\College\Junior\Practical_Project\Practical_Project_2018\Practical_Project\cuda\dropout_layer_kernels.cu.obj" \
    "E:\College\Junior\Practical_Project\Practical_Project_2018\Practical_Project\cuda\im2col_kernels.cu.obj" \
    "E:\College\Junior\Practical_Project\Practical_Project_2018\Practical_Project\cuda\maxpool_layer_kernels.cu.obj" \
    "E:\College\Junior\Practical_Project\Practical_Project_2018\Practical_Project\cuda\network_kernels.cu.obj"

SOURCES += \
    dataWindow/database.cpp \
    dataWindow/databasesetdialog.cpp \
    dataWindow/datatablemodel.cpp \
    dataWindow/datawindow.cpp \
    dataWindow/defectwindow.cpp \
    dataWindow/imageviewer.cpp \
    detect.cpp \
        main.cpp \
        mainwindow.cpp \
    scaledchart.cpp \
    scaledchartview.cpp \
    settingdialog.cpp \
    yolo_src/activation_layer.c \
    yolo_src/activations.c \
    yolo_src/art.c \
    yolo_src/avgpool_layer.c \
    yolo_src/batchnorm_layer.c \
    yolo_src/blas.c \
    yolo_src/box.c \
    yolo_src/captcha.c \
    yolo_src/cifar.c \
    yolo_src/classifier.c \
    yolo_src/coco.c \
    yolo_src/col2im.c \
    yolo_src/compare.c \
    yolo_src/connected_layer.c \
    yolo_src/convolutional_layer.c \
    yolo_src/cost_layer.c \
    yolo_src/cpu_gemm.c \
    yolo_src/crnn_layer.c \
    yolo_src/crop_layer.c \
    yolo_src/cuda.c \
    yolo_src/data.c \
    yolo_src/deconvolutional_layer.c \
    yolo_src/demo.c \
    yolo_src/detection_layer.c \
    yolo_src/detector.c \
    yolo_src/dice.c \
    yolo_src/dropout_layer.c \
    yolo_src/gemm.c \
    yolo_src/getopt.c \
    yolo_src/gettimeofday.c \
    yolo_src/go.c \
    yolo_src/gru_layer.c \
    yolo_src/http_stream.cpp \
    yolo_src/im2col.c \
    yolo_src/image.c \
    yolo_src/layer.c \
    yolo_src/list.c \
    yolo_src/local_layer.c \
    yolo_src/matrix.c \
    yolo_src/maxpool_layer.c \
    yolo_src/network.c \
    yolo_src/nightmare.c \
    yolo_src/normalization_layer.c \
    yolo_src/option_list.c \
    yolo_src/parser.c \
    yolo_src/region_layer.c \
    yolo_src/reorg_layer.c \
    yolo_src/reorg_old_layer.c \
    yolo_src/rnn.c \
    yolo_src/rnn_layer.c \
    yolo_src/rnn_vid.c \
    yolo_src/route_layer.c \
    yolo_src/shortcut_layer.c \
    yolo_src/softmax_layer.c \
    yolo_src/super.c \
    yolo_src/swag.c \
    yolo_src/tag.c \
    yolo_src/tree.c \
    yolo_src/upsample_layer.c \
    yolo_src/utils.c \
    yolo_src/voxel.c \
    yolo_src/writing.c \
    yolo_src/yolo.c \
    yolo_src/yolo_layer.c \
    yolo_src/yolo_v2_class.cpp

HEADERS += \
    3rdparty/include/pthread.h \
    3rdparty/include/sched.h \
    3rdparty/include/semaphore.h \
    dataWindow/database.h \
    dataWindow/databasesetdialog.h \
    dataWindow/datatablemodel.h \
    dataWindow/datawindow.h \
    dataWindow/defectwindow.h \
    dataWindow/imageviewer.h \
    detect.h \
        mainwindow.h \
    product.h \
    scaledchart.h \
    scaledchartview.h \
    settingdialog.h \
    yolo_src/activation_layer.h \
    yolo_src/activations.h \
    yolo_src/avgpool_layer.h \
    yolo_src/batchnorm_layer.h \
    yolo_src/blas.h \
    yolo_src/box.h \
    yolo_src/classifier.h \
    yolo_src/col2im.h \
    yolo_src/connected_layer.h \
    yolo_src/convolutional_layer.h \
    yolo_src/cost_layer.h \
    yolo_src/crnn_layer.h \
    yolo_src/crop_layer.h \
    yolo_src/cuda.h \
    yolo_src/data.h \
    yolo_src/deconvolutional_layer.h \
    yolo_src/demo.h \
    yolo_src/detection_layer.h \
    yolo_src/dropout_layer.h \
    yolo_src/gemm.h \
    yolo_src/getopt.h \
    yolo_src/gettimeofday.h \
    yolo_src/gru_layer.h \
    yolo_src/http_stream.h \
    yolo_src/im2col.h \
    yolo_src/image.h \
    yolo_src/layer.h \
    yolo_src/list.h \
    yolo_src/local_layer.h \
    yolo_src/matrix.h \
    yolo_src/maxpool_layer.h \
    yolo_src/network.h \
    yolo_src/normalization_layer.h \
    yolo_src/option_list.h \
    yolo_src/parser.h \
    yolo_src/region_layer.h \
    yolo_src/reorg_layer.h \
    yolo_src/reorg_old_layer.h \
    yolo_src/rnn_layer.h \
    yolo_src/route_layer.h \
    yolo_src/shortcut_layer.h \
    yolo_src/softmax_layer.h \
    yolo_src/stb_image.h \
    yolo_src/stb_image_write.h \
    yolo_src/tree.h \
    yolo_src/unistd.h \
    yolo_src/upsample_layer.h \
    yolo_src/utils.h \
    yolo_src/yolo_layer.h \
    yolo_src/yolo_v2_class.hpp

FORMS += \
    dataWindow/datawindow.ui \
        mainwindow.ui \
    settingdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    dataWindow/resource.qrc
