#include "MemoryOutputModule.h"
#include <decoder.h>

int memory_output_module_probe(const char *name)
{
	ByteVector* aByteVector = (ByteVector*) atoi(name);

	if (aByteVector == NULL)
		return XA_ERROR_INVALID_PARAMETERS;

	return XA_SUCCESS;
}

int memory_output_module_query(XA_OutputModuleQuery *query, 
                                 unsigned long query_mask)
{
	return XA_SUCCESS;
}

int   memory_output_new(XA_OutputInstance **output, const char *name,
                        XA_OutputModuleClassInfo *class_info,
                        XA_DecoderInfo *decoder)
{
	
	*output = (XA_OutputInstance*) atoi(name);
	MemoryOutput* aMemoryOutput = (MemoryOutput*) *output;

	return XA_SUCCESS;
}

int   memory_output_delete(XA_OutputInstance *output)
{	
	return XA_SUCCESS;
}

int memory_output_open(XA_OutputInstance *output)
{
	return XA_SUCCESS;
}

int memory_output_close(XA_OutputInstance *output)
{
	return XA_SUCCESS;
}

int   memory_output_write(XA_OutputInstance *output, const void *buffer, 
                      unsigned long size, unsigned int bytes_per_sample,
                      unsigned int nb_channels, unsigned int sample_rate)
{
	MemoryOutput* aMemoryOutput = (MemoryOutput*) output;

	aMemoryOutput->mBitsPerSample = bytes_per_sample*8;
	aMemoryOutput->mNumChannels = nb_channels;
	aMemoryOutput->mSamplingRate = sample_rate;

	aMemoryOutput->mByteVector.insert(aMemoryOutput->mByteVector.end(), 
		(const unsigned char*) buffer, 
		(const unsigned char*) buffer + size);

	return XA_SUCCESS;
}

int memory_output_module_register(XA_OutputModule *module)
{
	module->api_version_id = XA_SYNC_API_VERSION;
	module->output_module_probe = memory_output_module_probe;
	module->output_module_query = memory_output_module_query;
	module->output_new = memory_output_new;
	module->output_delete = memory_output_delete;
	module->output_open = memory_output_open;
	module->output_close = memory_output_close;
	module->output_write = memory_output_write;
	module->output_get_buffer = NULL;
	module->output_set_control = NULL;
	module->output_get_control = NULL;
	module->output_get_status = NULL;
	module->output_get_caps = NULL;	
	module->output_send_message = NULL;	

	return XA_SUCCESS;
}