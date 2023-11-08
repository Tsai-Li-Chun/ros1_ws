set(catkin_richie_LIBRARIES
    /opt/ros/noetic/lib/libroscpp.so
    /opt/ros/noetic/lib/librosconsole.so
    /opt/PhotoneoSDK/Localization/lib/libPhoXi_API_gcc9.3.0_Release.so.1.9.5
    /opt/PhotoneoSDK/Localization/lib/libPhoLocalization_SDK_gcc9.3.0_Release.so.1.4.0
    # /opt/ros/noetic/lib/libcpp_common.so
    # /opt/ros/noetic/lib/librosconsole_backend_interface.so
    # /opt/ros/noetic/lib/librosconsole_log4cxx.so
    # /opt/ros/noetic/lib/libroscpp_serialization.so
    # /opt/ros/noetic/lib/librostime.so
    # /opt/ros/noetic/lib/libxmlrpcpp.so
    # /usr/lib/x86_64-linux-gnu/libboost_chrono.so.1.71.0
    # /usr/lib/x86_64-linux-gnu/libboost_date_time.so.1.71.0
    # /usr/lib/x86_64-linux-gnu/libboost_filesystem.so.1.71.0
    # /usr/lib/x86_64-linux-gnu/libboost_regex.so.1.71.0
    # /usr/lib/x86_64-linux-gnu/libboost_system.so.1.71.0
    # /usr/lib/x86_64-linux-gnu/libboost_thread.so.1.71.0
    # /usr/lib/x86_64-linux-gnu/libconsole_bridge.so.0.4
    # /usr/lib/x86_64-linux-gnu/liblog4cxx.so
    # /usr/lib/x86_64-linux-gnu/libpthread.so
)

set(PHOLOCALIZATION_LIBRARIES
    "/opt/PhotoneoSDK/Localization/lib/libPhoXi_API_gcc9.3.0_Release.so.1.9.5"
    # "/opt/PhotoneoSDK/Localization/lib/libPCLTools_ClientConfig.a"
    # "/opt/PhotoneoSDK/Localization/lib/libPhoLibrary_dynamic_Release.so"
    "/opt/PhotoneoSDK/Localization/lib/libPhoLocalization_SDK_gcc9.3.0_Release.so.1.4.0"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/CUrl/libcurl.so"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/gcc9.3.0/libgomp.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/gcc9.3.0/libstdc++.so.6"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Libpng/libpng16.so.16"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/LibSSH2/libssh2.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Libtiff/libtiff.so.5"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/OpenCV/libopencv_calib3d.so.3.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/OpenCV/libopencv_core.so.3.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/OpenCV/libopencv_features2d.so.3.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/OpenCV/libopencv_flann.so.3.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/OpenCV/libopencv_highgui.so.3.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/OpenCV/libopencv_imgcodecs.so.3.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/OpenCV/libopencv_imgproc.so.3.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/OpenCV/libopencv_photo.so.3.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/OpenSSL/libcrypto.so.1.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/OpenSSL/libssl.so.1.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Pcl/libpcl_common.so.1.8"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Pcl/libpcl_features.so.1.8"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Pcl/libpcl_filters.so.1.8"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Pcl/libpcl_io.so.1.8"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Pcl/libpcl_io_ply.so.1.8"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Pcl/libpcl_kdtree.so.1.8"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Pcl/libpcl_octree.so.1.8"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Pcl/libpcl_registration.so.1.8"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Pcl/libpcl_sample_consensus.so.1.8"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Pcl/libpcl_search.so.1.8"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Python/libpython2.7.so.1.0"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkalglib-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkCommonColor-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkCommonComputationalGeometry-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkCommonCore-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkCommonDataModel-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkCommonExecutionModel-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkCommonMath-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkCommonMisc-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkCommonSystem-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkCommonTransforms-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkDICOMParser-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkFiltersCore-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkFiltersExtraction-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkFiltersGeneral-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkFiltersGeometry-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkFiltersSources-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkFiltersStatistics-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkfreetype-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkglew-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkImagingCore-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkImagingFourier-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkImagingHybrid-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkInteractionStyle-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkIOCore-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkIOGeometry-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkIOImage-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkIOLegacy-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkIOPLY-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkjpeg-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkmetaio-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkRenderingCore-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkRenderingFreeType-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtkRenderingOpenGL2-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/Vtk/libvtksys-7.0.so.1"
    # "/opt/PhotoneoSDK/Localization/bin/3rdParty/YAML/libyaml-cpp.so.0.6"
)

set(PHOTONEO_INCLUDE_DIRS
    "/opt/PhotoneoSDK/Localization/include/PhoXi_API"
    # "/opt/PhotoneoSDK/Localization/include"
    # "/opt/PhotoneoSDK/Localization/include/Common"
    "/opt/PhotoneoSDK/Localization/include/PhoLocalization_SDK"
    # "/opt/PhotoneoSDK/Localization/include/3rdParty/eigen3"
    # "/opt/PhotoneoSDK/Localization/include/3rdParty/opencv"
    # "/opt/PhotoneoSDK/Localization/include/3rdParty/opencv/opencv"
    # "/opt/PhotoneoSDK/Localization/include/3rdParty/pcl"
    # "/opt/PhotoneoSDK/Localization/include/3rdParty/vtk"
)