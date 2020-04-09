Name:       glacier-gallery
Summary:    Photo Gallery for Nemo
Version:    0.2.2
Release:    3
Group:      Applications/System
License:    BSD
URL:        https://github.com/nemomobile-ux/glacier-gallery
Source0:    %{name}-%{version}.tar.bz2

Requires:   glacier-gallery-qmlplugin
Requires:   mapplauncherd-booster-qtcomponents-qt5
Requires:   libglacierapp >= 0.1.1
Requires:   mapplauncherd-booster-nemomobile
Requires:   nemo-qml-plugin-dbus-qt5

BuildRequires:  cmake
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(qdeclarative5-boostable)
BuildRequires:  pkgconfig(libresourceqt5)
BuildRequires:  desktop-file-utils
BuildRequires:  pkgconfig(glacierapp)
BuildRequires:  qt5-qttools-linguist

Provides:   meego-handset-video > 0.2.5
Obsoletes:   meego-handset-video <= 0.2.5


%package qmlplugin
Summary: QML Plugin for gallery view
Requires:   qt-components-qt5
Requires:   qt5-qtquickcontrols
Requires:   qt5-qtquickcontrols-nemo
%if 0%{?fedora}
Requires:   qt5-qtmultimedia
%else
Requires:   qt5-qtdeclarative-import-multimedia
%endif
Requires:   nemo-qml-plugin-thumbnailer-qt5
Requires:   qt5-qtdocgallery

%description
Photo Gallery application using Qt Quick for Nemo Mobile.

%description qmlplugin
This pligin contants qml api for glacier gallery

%prep
%setup -q -n %{name}-%{version}

%build
mkdir build
cd build
cmake \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_INSTALL_PREFIX=%{_prefix} \
	-DCMAKE_INSTALL_LIBDIR=%{_lib} \
	-DCMAKE_VERBOSE_MAKEFILE=ON \
	..
cmake --build .

%install
cd build
rm -rf %{buildroot}
DESTDIR=%{buildroot} cmake --build . --target install

%files
%defattr(-,root,root,-)
%{_bindir}/glacier-gallery
%{_datadir}/applications/glacier-gallery.desktop
%{_datadir}/applications/glacier-gallery-openfile.desktop
%{_datadir}/glacier-gallery
%{_datadir}/dbus-1/services/org.nemomobile.gallery.service
%{_datadir}/dbus-1/interfaces/org.nemomobile.gallery.xml

%files qmlplugin
%{_libdir}/qt5/qml/org/nemomobile/gallery/*
