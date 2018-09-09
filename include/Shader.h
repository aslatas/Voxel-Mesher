/**
 * @file Shader.h
 * Wrapper for nanogui GLShader class, in order to support initialization from
 * multiple files as well as uploading attributes for instanced drawing.
 * @author Matthew McLaurin
 */

#pragma once

#include <nanogui/glutil.h>

#include <fstream>
#include <streambuf>

 /**
  * @class Shader
  * Wrapper for nanogui::GLShader. Directs most function calls to an internal
  * GLShader, but implements custom functionality for initialization from
  * several source files, as well as for uploading attributes needed for
  * instanced drawing.
  */
class Shader : nanogui::GLShader
{
public:
    /**
     * Initializes a shader program from files. Accepts one or more paths for
     * vertex, fragment, and optionally geometry shaders. Files are read in as
     * buffered strings, and concatenated in order. Internally calls init()
     * with the resultant strings, which will provide error output if shader
     * compilation fails.
     * @param name Name of the shader program, can be used later to access
     * attributes.
     * @param vertex_fnames Array of one or more file paths containing vertex
     * shader code.
     * @param fragment_fnames Array of one or more file paths containing
     * fragment shader code.
     * @param geometry_fnames Array of zero or more file paths containing
     * geometry shader code.
     * @param numVert Length of the vertex file name array.
     * @param numFrag Length of the fragment file name array.
     * @param numGeom Length of the geometry file name array.
     * @return True if shaders compile and link successfully, false otherwise.
     */
    bool Shader::initFromFiles(
        const std::string &name,
        const std::string vertex_fnames[],
        const std::string fragment_fnames[],
        const std::string geometry_fnames[],
        int numVert, int numFrag, int numGeom)
    {
        std::string vName = "";
        std::string fName = "";
        std::string gName = "";

        for (int i = 0; i < numVert; i++) {
            vName += readFile(vertex_fnames[i]);
        }
        for (int i = 0; i < numFrag; i++) {
            fName += readFile(fragment_fnames[i]);
        }
        for (int i = 0; i < numGeom; i++) {
            gName += readFile(geometry_fnames[i]);
        }

        return init(name, vName, fName, gName);
    }

    /**
     * Draws instanced objects without an element buffer.
     * @param type Type of primitives to draw.
     * @param offset Width of one vertex in the buffer.
     * @param vertices ID of the vertex buffer to draw.
     * @param count Number of objects to draw.
     */
    void Shader::drawArraysInstanced(int type, uint32_t offset, uint32_t vertices, uint32_t count)
    {
        if (count == 0)
            return;

        glDrawArraysInstanced(type, offset, vertices, count);
    }

    /**
    * Draws instanced objects with an element buffer.
    * @param type Type of primitives to draw.
    * @param offset_ Width of one vertex in the buffer.
    * @param vertices ID of the vertex buffer to draw.
    * @param count_ Number of objects to draw.
    */
    void Shader::drawElementsInstanced(int type, uint32_t offset_, uint32_t vertices, uint32_t count_)
    {
        if (count_ == 0)
            return;
        size_t offset = offset_;
        size_t count = vertices;

        switch (type) {
        case GL_TRIANGLES: offset *= 3; count *= 3; break;
        case GL_LINES: offset *= 2; count *= 2; break;
        }

        glDrawElementsInstanced(type, (GLsizei)count, GL_UNSIGNED_INT,
            (const void *)(offset * sizeof(uint32_t)), count_);
    }

    bool Shader::init(const std::string &name,
        const std::string &vertex_str,
        const std::string &fragment_str,
        const std::string &geometry_str)
    {
        return mShader.init(name, vertex_str, fragment_str, geometry_str);
    }

    void Shader::bind()
    {
        mShader.bind();
    }

    GLint Shader::attrib(const std::string &name, bool warn) const
    {
        return mShader.attrib(name, warn);
    }

    void Shader::setUniform(const std::string &name, const nanogui::GLUniformBuffer &buf, bool warn)
    {
        mShader.setUniform(name, buf, warn);
    }

    GLint Shader::uniform(const std::string &name, bool warn) const
    {
        return mShader.uniform(name, warn);
    }

    void Shader::shareAttrib(const GLShader &otherShader, const std::string &name, const std::string &_as)
    {
        mShader.shareAttrib(otherShader, name, _as);
    }

    void Shader::invalidateAttribs()
    {
        mShader.invalidateAttribs();
    }

    void Shader::freeAttrib(const std::string &name)
    {
        mShader.freeAttrib(name);
    }

    void Shader::drawIndexed(int type, uint32_t offset, uint32_t count)
    {
        mShader.drawIndexed(type, offset, count);
    }

    void Shader::drawArray(int type, uint32_t offset, uint32_t count)
    {
        mShader.drawArray(type, offset, count);
    }

    void Shader::free()
    {
        mShader.free();
    }

    const nanogui::GLShader::Buffer &Shader::attribBuffer(const std::string &name)
    {
        return mShader.attribBuffer(name);
    }

    void Shader::uploadAttrib(const std::string &name, size_t size, int dim,
        uint32_t compSize, GLuint glType, bool integral,
        const void *data, int version)
    {
        mShader.uploadAttrib(name, size, dim, compSize, glType, integral, data, version);
    }

    void Shader::downloadAttrib(const std::string &name, size_t size,
        uint32_t compSize, void *data)
    {
        mShader.downloadAttrib(name, size, 0, compSize, 0, data);
    }

    const std::string &name() const
    {
        return mShader.name();
    }

    void define(const std::string &key, const std::string &value)
    {
        mShader.define(key, value);
    }

    template <typename Matrix> void uploadAttrib(const std::string &name, const Matrix &M, int version = -1)
    {
        mShader.uploadAttrib(name, M, version);
    }

    template <typename Matrix> void downloadAttrib(const std::string &name, Matrix &M)
    {
        mShader.downloadAttrib(name, M);
    }

    template <typename Matrix> void uploadIndices(const Matrix &M, int version = -1)
    {
        mShader.uploadIndices(M, version);
    }

    bool hasAttrib(const std::string &name) const
    {
        return mShader.hasAttrib(name);
    }

    int attribVersion(const std::string &name) const
    {
        return mShader.attribVersion(name);
    }

    void resetAttribVersion(const std::string &name)
    {
        mShader.resetAttribVersion(name);
    }

    template <typename T>
    void setUniform(const std::string &name, const Eigen::Matrix<T, 4, 4> &mat, bool warn = true)
    {
        mShader.setUniform(name, mat, warn);
    }

    template <typename T>
    void setUniform(const std::string &name, const Eigen::Transform<T, 3, 3> &affine, bool warn = true)
    {
        mShader.setUniform(name, mat, warn);
    }

    template <typename T>
    void setUniform(const std::string &name, const Eigen::Matrix<T, 3, 3> &mat, bool warn = true)
    {
        mShader.setUniform(name, mat, warn);
    }

    template <typename T>
    void setUniform(const std::string &name, const Eigen::Transform<T, 2, 2> &affine, bool warn = true)
    {
        mShader.setUniform(name, affine, warn);
    }

    void setUniform(const std::string &name, bool value, bool warn = true)
    {
        mShader.setUniform(name, value, warn);
    }

    template <typename T, typename std::enable_if<nanogui::detail::type_traits<T>::integral == 1, int>::type = 0>
    void setUniform(const std::string &name, T value, bool warn = true)
    {
        mShader.setUniform(name, value, warn);
    }

    template <typename T, typename std::enable_if<nanogui::detail::type_traits<T>::integral == 0, int>::type = 0>
    void setUniform(const std::string &name, T value, bool warn = true)
    {
        mShader.setUniform(name, value, warn);
    }

    template <typename T, typename std::enable_if<nanogui::detail::type_traits<T>::integral == 1, int>::type = 0>
    void setUniform(const std::string &name, const Eigen::Matrix<T, 2, 1>  &v, bool warn = true)
    {
        mShader.setUniform(name, v, warn);
    }

    template <typename T, typename std::enable_if<nanogui::detail::type_traits<T>::integral == 0, int>::type = 0>
    void setUniform(const std::string &name, const Eigen::Matrix<T, 2, 1>  &v, bool warn = true)
    {
        mShader.setUniform(name, v, warn);
    }

    template <typename T, typename std::enable_if<nanogui::detail::type_traits<T>::integral == 1, int>::type = 0>
    void setUniform(const std::string &name, const Eigen::Matrix<T, 3, 1>  &v, bool warn = true)
    {
        mShader.setUniform(name, v, warn);
    }

    template <typename T, typename std::enable_if<nanogui::detail::type_traits<T>::integral == 0, int>::type = 0>
    void setUniform(const std::string &name, const Eigen::Matrix<T, 3, 1>  &v, bool warn = true)
    {
        mShader.setUniform(name, v, warn);
    }

    template <typename T, typename std::enable_if<nanogui::detail::type_traits<T>::integral == 1, int>::type = 0>
    void setUniform(const std::string &name, const Eigen::Matrix<T, 4, 1>  &v, bool warn = true)
    {
        mShader.setUniform(name, v, warn);
    }

    template <typename T, typename std::enable_if<nanogui::detail::type_traits<T>::integral == 0, int>::type = 0>
    void setUniform(const std::string &name, const Eigen::Matrix<T, 4, 1>  &v, bool warn = true)
    {
        mShader.setUniform(name, v, warn);
    }

    size_t bufferSize() const {
        size_t size = 0;
        for (auto const &buf : mBufferObjects)
            size += buf.second.size;
        return size;
    }

private:
    /** Internal nanogui shader for which the Shader class is a wrapper. */
    nanogui::GLShader mShader;

    /**
     * Reads in a file path to a buffered stream, and generates a string of
     * file contents. 
     * @param filename Path to the file.
     * @return A string containing file contents.
     */
    std::string Shader::readFile(const std::string &filename)
    {
        std::ifstream t(filename);
        return std::string(std::istreambuf_iterator<char>(t), std::istreambuf_iterator<char>());
    }
};