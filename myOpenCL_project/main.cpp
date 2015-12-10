#define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.hpp>
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

int main() {
	ifstream f("helloworld.cl");
	const string helloStr(istreambuf_iterator<char>(f),
		(istreambuf_iterator<char>()));

	cl_int err=CL_SUCCESS;
	try {
		vector<cl::Platform> platforms;
		cl::Platform::get(&platforms);
		if (platforms.size() == 0) {
			cout << "Platform size 0" << endl;
			return -1;
		}
		cl_context_properties properties[] =
		{CL_CONTEXT_PLATFORM,
		(cl_context_properties)(platforms[0])(),
		0};
		cl::Context context(CL_DEVICE_TYPE_GPU, properties);
		vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
		//cout << devices.size() << endl;
		cl::Program::Sources sourse(1, make_pair(helloStr.c_str(),helloStr.length()));
		cl::Program program = cl::Program(context, sourse);
		program.build(devices);
		vector<size_t> work_item_size(3);
		err=devices[0].getInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES, &work_item_size);
		for (auto it = work_item_size.begin();
		it != work_item_size.end();
			it++)
			cout << *it<<'\t';
		cout << endl;
		cl::Kernel kernel(program, "hello", &err);
		cl::Event event;	
		cl::CommandQueue queue(context, devices[0], 0, &err);
		queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(301, 301,301), cl::NullRange, NULL, &event);
		event.wait();
	}
	catch (cl::Error err) {
		cerr
			<< "ERROR:"
			<< err.what()
			<< "("
			<< err.err()
			<< ")"
			<< endl;
	}
	
	
	return EXIT_SUCCESS;
	
}