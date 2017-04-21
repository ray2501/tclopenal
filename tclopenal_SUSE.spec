%{!?directory:%define directory /usr}

%define buildroot %{_tmppath}/%{name}

Name:          tclopenal
Summary:       Tcl bindings for OpenAL (open audio library)
Version:       0.3
Release:       1
License:       LGPL v2.1
Group:         Development/Libraries/Tcl
Source:        https://github.com/ray2501/tclopenal/tclopenal_0.3.zip
URL:           https://github.com/ray2501/tclopenal
BuildRequires: autoconf
BuildRequires: make
BuildRequires: tcl-devel >= 8.4
BuildRequires: openal-soft-devel
Requires:      tcl >= 8.4
Requires:      libopenal1
BuildRoot:     %{buildroot}

%description
Tcl bindings for OpenAL (open audio library).

%prep
%setup -q -n %{name}

%build
./configure \
	--prefix=%{directory} \
	--exec-prefix=%{directory} \
	--libdir=%{directory}/%{_lib}
make 

%install
make DESTDIR=%{buildroot} pkglibdir=%{directory}/%{_lib}/tcl/%{name}%{version} install

%clean
rm -rf %buildroot

%files
%defattr(-,root,root)
%{directory}/%{_lib}/tcl
