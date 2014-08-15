/*
Copyright (c) 2013 Daniele Bartolini, Michele Rossi
Copyright (c) 2012 Daniele Bartolini, Simone Boscaratto

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#include "material.h"
#include "material_resource.h"
#include "memory.h"
#include "random.h"
#include "device.h"
#include "resource_manager.h"
#include "texture_resource.h"
#include <bgfx.h>

static const uint8_t vs_h[358] =
{
	0x56, 0x53, 0x48, 0x02, 0x8c, 0xd4, 0x10, 0xbd, 0x01, 0x00, 0x0f, 0x75, 0x5f, 0x6d, 0x6f, 0x64, // VSH........u_mod
	0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x09, 0x01, 0x00, 0x00, 0x01, 0x00, // elViewProj......
	0x41, 0x01, 0x00, 0x00, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x20, 0x6d, 0x65, // A...attribute me
	0x64, 0x69, 0x75, 0x6d, 0x70, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, 0x61, 0x5f, 0x70, 0x6f, 0x73, // diump vec2 a_pos
	0x69, 0x74, 0x69, 0x6f, 0x6e, 0x3b, 0x0a, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, // ition;.attribute
	0x20, 0x6d, 0x65, 0x64, 0x69, 0x75, 0x6d, 0x70, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, 0x61, 0x5f, //  mediump vec2 a_
	0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x3b, 0x0a, 0x76, 0x61, 0x72, 0x79, 0x69, // texcoord0;.varyi
	0x6e, 0x67, 0x20, 0x6d, 0x65, 0x64, 0x69, 0x75, 0x6d, 0x70, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, // ng mediump vec2 
	0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x3b, 0x0a, 0x75, 0x6e, 0x69, // v_texcoord0;.uni
	0x66, 0x6f, 0x72, 0x6d, 0x20, 0x6d, 0x65, 0x64, 0x69, 0x75, 0x6d, 0x70, 0x20, 0x6d, 0x61, 0x74, // form mediump mat
	0x34, 0x20, 0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, // 4 u_modelViewPro
	0x6a, 0x3b, 0x0a, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x6d, 0x61, 0x69, 0x6e, 0x20, 0x28, 0x29, 0x0a, // j;.void main ().
	0x7b, 0x0a, 0x20, 0x20, 0x6d, 0x65, 0x64, 0x69, 0x75, 0x6d, 0x70, 0x20, 0x76, 0x65, 0x63, 0x34, // {.  mediump vec4
	0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, //  tmpvar_1;.  tmp
	0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x7a, 0x77, 0x20, 0x3d, 0x20, 0x76, 0x65, 0x63, 0x32, 0x28, // var_1.zw = vec2(
	0x30, 0x2e, 0x30, 0x2c, 0x20, 0x31, 0x2e, 0x30, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, // 0.0, 1.0);.  tmp
	0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x78, 0x79, 0x20, 0x3d, 0x20, 0x61, 0x5f, 0x70, 0x6f, 0x73, // var_1.xy = a_pos
	0x69, 0x74, 0x69, 0x6f, 0x6e, 0x3b, 0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x50, 0x6f, 0x73, 0x69, // ition;.  gl_Posi
	0x74, 0x69, 0x6f, 0x6e, 0x20, 0x3d, 0x20, 0x28, 0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, // tion = (u_modelV
	0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x20, 0x2a, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, // iewProj * tmpvar
	0x5f, 0x31, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, // _1);.  v_texcoor
	0x64, 0x30, 0x20, 0x3d, 0x20, 0x61, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, // d0 = a_texcoord0
	0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00,                                                             // ;.}...
};

static const uint8_t fs_h[207] =
{
	0x46, 0x53, 0x48, 0x02, 0x8c, 0xd4, 0x10, 0xbd, 0x01, 0x00, 0x08, 0x75, 0x5f, 0x61, 0x6c, 0x62, // FSH........u_alb
	0x65, 0x64, 0x6f, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0xb1, 0x00, 0x00, 0x00, 0x76, 0x61, 0x72, // edo..........var
	0x79, 0x69, 0x6e, 0x67, 0x20, 0x6d, 0x65, 0x64, 0x69, 0x75, 0x6d, 0x70, 0x20, 0x76, 0x65, 0x63, // ying mediump vec
	0x32, 0x20, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x3b, 0x0a, 0x75, // 2 v_texcoord0;.u
	0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72, 0x32, 0x44, // niform sampler2D
	0x20, 0x75, 0x5f, 0x61, 0x6c, 0x62, 0x65, 0x64, 0x6f, 0x3b, 0x0a, 0x76, 0x6f, 0x69, 0x64, 0x20, //  u_albedo;.void 
	0x6d, 0x61, 0x69, 0x6e, 0x20, 0x28, 0x29, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x6c, 0x6f, 0x77, 0x70, // main ().{.  lowp
	0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x3b, 0x0a, //  vec4 tmpvar_1;.
	0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x20, 0x3d, 0x20, 0x74, 0x65, 0x78, //   tmpvar_1 = tex
	0x74, 0x75, 0x72, 0x65, 0x32, 0x44, 0x20, 0x28, 0x75, 0x5f, 0x61, 0x6c, 0x62, 0x65, 0x64, 0x6f, // ture2D (u_albedo
	0x2c, 0x20, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x29, 0x3b, 0x0a, // , v_texcoord0);.
	0x20, 0x20, 0x67, 0x6c, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x20, 0x3d, //   gl_FragColor =
	0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00,       //  tmpvar_1;.}...
};

static const uint8_t vsc_h[358] =
{
	0x56, 0x53, 0x48, 0x02, 0x8c, 0xd4, 0x10, 0xbd, 0x01, 0x00, 0x0f, 0x75, 0x5f, 0x6d, 0x6f, 0x64, // VSH........u_mod
	0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x09, 0x01, 0x00, 0x00, 0x01, 0x00, // elViewProj......
	0x41, 0x01, 0x00, 0x00, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x20, 0x6d, 0x65, // A...attribute me
	0x64, 0x69, 0x75, 0x6d, 0x70, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, 0x61, 0x5f, 0x70, 0x6f, 0x73, // diump vec2 a_pos
	0x69, 0x74, 0x69, 0x6f, 0x6e, 0x3b, 0x0a, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, // ition;.attribute
	0x20, 0x6d, 0x65, 0x64, 0x69, 0x75, 0x6d, 0x70, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, 0x61, 0x5f, //  mediump vec2 a_
	0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x3b, 0x0a, 0x76, 0x61, 0x72, 0x79, 0x69, // texcoord0;.varyi
	0x6e, 0x67, 0x20, 0x6d, 0x65, 0x64, 0x69, 0x75, 0x6d, 0x70, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, // ng mediump vec2 
	0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x3b, 0x0a, 0x75, 0x6e, 0x69, // v_texcoord0;.uni
	0x66, 0x6f, 0x72, 0x6d, 0x20, 0x6d, 0x65, 0x64, 0x69, 0x75, 0x6d, 0x70, 0x20, 0x6d, 0x61, 0x74, // form mediump mat
	0x34, 0x20, 0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, // 4 u_modelViewPro
	0x6a, 0x3b, 0x0a, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x6d, 0x61, 0x69, 0x6e, 0x20, 0x28, 0x29, 0x0a, // j;.void main ().
	0x7b, 0x0a, 0x20, 0x20, 0x6d, 0x65, 0x64, 0x69, 0x75, 0x6d, 0x70, 0x20, 0x76, 0x65, 0x63, 0x34, // {.  mediump vec4
	0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, //  tmpvar_1;.  tmp
	0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x7a, 0x77, 0x20, 0x3d, 0x20, 0x76, 0x65, 0x63, 0x32, 0x28, // var_1.zw = vec2(
	0x30, 0x2e, 0x30, 0x2c, 0x20, 0x31, 0x2e, 0x30, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, // 0.0, 1.0);.  tmp
	0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x78, 0x79, 0x20, 0x3d, 0x20, 0x61, 0x5f, 0x70, 0x6f, 0x73, // var_1.xy = a_pos
	0x69, 0x74, 0x69, 0x6f, 0x6e, 0x3b, 0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x50, 0x6f, 0x73, 0x69, // ition;.  gl_Posi
	0x74, 0x69, 0x6f, 0x6e, 0x20, 0x3d, 0x20, 0x28, 0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, // tion = (u_modelV
	0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x20, 0x2a, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, // iewProj * tmpvar
	0x5f, 0x31, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, // _1);.  v_texcoor
	0x64, 0x30, 0x20, 0x3d, 0x20, 0x61, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, // d0 = a_texcoord0
	0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00,                                                             // ;.}...
};

static const uint8_t fsc_h[103] =
{
	0x46, 0x53, 0x48, 0x02, 0x8c, 0xd4, 0x10, 0xbd, 0x01, 0x00, 0x07, 0x75, 0x5f, 0x63, 0x6f, 0x6c, // FSH........u_col
	0x6f, 0x72, 0x07, 0x01, 0x00, 0x00, 0x01, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x75, 0x6e, 0x69, 0x66, // or......J...unif
	0x6f, 0x72, 0x6d, 0x20, 0x6d, 0x65, 0x64, 0x69, 0x75, 0x6d, 0x70, 0x20, 0x76, 0x65, 0x63, 0x34, // orm mediump vec4
	0x20, 0x75, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x3b, 0x0a, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x6d, //  u_color;.void m
	0x61, 0x69, 0x6e, 0x20, 0x28, 0x29, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x46, 0x72, // ain ().{.  gl_Fr
	0x61, 0x67, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x20, 0x3d, 0x20, 0x75, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, // agColor = u_colo
	0x72, 0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00,                                                       // r;.}...
};


namespace crown
{

void Material::create(const MaterialResource* mr)
{
	uint32_t size = mr->dynamic_data_size();
	uint32_t offt = mr->dynamic_data_offset();
	char* base = (char*) mr + offt;

	data = (char*) default_allocator().allocate(size);
	memcpy(data, base, size);

	bgfx::ShaderHandle vs = bgfx::createShader(bgfx::makeRef(vs_h, sizeof(vs_h)));
	bgfx::ShaderHandle fs = bgfx::createShader(bgfx::makeRef(fs_h, sizeof(fs_h)));
	program = bgfx::createProgram(vs, fs, true);

	for (uint32_t i = 0; i < mr->num_textures(); i++)
	{
		TextureData* ud = mr->get_texture_data(i);

		TextureHandle* th = mr->get_texture_handle(i, data);
		th->sampler_handle = bgfx::createUniform(ud->sampler_name, bgfx::UniformType::Uniform1iv).idx;

		ResourceId texid;
		texid.type = TEXTURE_TYPE;
		texid.name = ud->id;
		TextureResource* tex = (TextureResource*) device()->resource_manager()->get(texid);

		th->texture_handle = bgfx::createTexture(bgfx::copy(tex->data(), tex->size())).idx;
	}

	for (uint32_t i = 0; i < mr->num_uniforms(); i++)
	{
		UniformData* ud = mr->get_uniform_data(i);

		UniformHandle* uh = mr->get_uniform_handle(i, data);
		uh->uniform_handle = bgfx::createUniform(ud->name, bgfx::UniformType::Uniform4fv).idx;
	}

	resource = mr;
}

void Material::destroy() const
{
	bgfx::destroyProgram(program);

	for (uint32_t i = 0; i < resource->num_textures(); i++)
	{
		TextureHandle* th = resource->get_texture_handle(i, data);

		bgfx::UniformHandle sh;
		sh.idx = th->sampler_handle;
		bgfx::destroyUniform(sh);

		bgfx::TextureHandle bgfx_th;
		bgfx_th.idx = th->texture_handle;
		bgfx::destroyTexture(bgfx_th);
	}

	for (uint32_t i = 0; i < resource->num_uniforms(); i++)
	{
		UniformHandle* uh = resource->get_uniform_handle(i, data);

		bgfx::UniformHandle bgfx_uh;
		bgfx_uh.idx = uh->uniform_handle;
		bgfx::destroyUniform(bgfx_uh);
	}

	default_allocator().deallocate(data);
}

void Material::bind() const
{
	bgfx::setProgram(program);

	// Set samplers
	for (uint32_t i = 0; i < resource->num_textures(); i++)
	{
		TextureHandle* th = resource->get_texture_handle(i, data);

		bgfx::UniformHandle sampler;
		bgfx::TextureHandle texture;
		sampler.idx = th->sampler_handle;
		texture.idx = th->texture_handle;

		bgfx::setTexture(i, sampler, texture);
	}

	// Set uniforms
	for (uint32_t i = 0; i < resource->num_uniforms(); i++)
	{
		UniformHandle* uh = resource->get_uniform_handle(i, data);

		bgfx::UniformHandle buh;
		buh.idx = uh->uniform_handle;
		bgfx::setUniform(buh, (char*) uh + sizeof(uh->uniform_handle));
	}
}

} // namespace crown