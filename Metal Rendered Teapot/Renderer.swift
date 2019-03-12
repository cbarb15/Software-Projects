////
////  Renderer.swift
////  Model - Part7
////
////  Created by Charlie Barber on 2/23/19.
////  Copyright © 2019 Charlie Barber. All rights reserved.
////
//
//import MetalKit
//import ModelIO
//import simd
//
//class Renderer: NSObject, MTKViewDelegate {
//
//    var device: MTLDevice!
//    var mtkView: MTKView!
//    var commandQueue: MTLCommandQueue!
//    var commandBuffer: MTLCommandBuffer!
//    var pipelineState: MTLRenderPipelineState!
//    var vertexBuffer: MTKMeshBuffer!
//    var indexBuffer: MTKMeshBuffer!
//    var meshes: [MTKMesh] = []
//    var modelViewMatrix: float4x4!
//    var projectionMatrix: float4x4!
//    var modelVertexDescriptor: MTLVertexDescriptor!
//    var depthStencilState: MTLDepthStencilState!
//
//    struct Uniforms {
//        var modelMatrix: float4x4
//        var viewProjectionMatrix: float4x4
//        var normalMatrix: float3x3
//    }
//
//    init(with device: MTLDevice, and view:MTKView) {
//        self.device = device
//        self.mtkView = view
//        depthStencilState = Renderer.buildDepthStencilState(device: self.device)
//        self.commandQueue = device.makeCommandQueue()
//        super.init()
//        loadModel()
//        buildPipelineState()
//    }
//
//    func loadModel() {
//
//        let modelURL = Bundle.main.url(forResource: "teapot", withExtension: "obj")!
//
//        let vertexDescriptor = MDLVertexDescriptor()
//        vertexDescriptor.attributes[0] = MDLVertexAttribute(name: MDLVertexAttributePosition, format: .float3, offset: 0, bufferIndex: 0)
//        vertexDescriptor.attributes[1] = MDLVertexAttribute(name: MDLVertexAttributeNormal, format: .float3, offset: MemoryLayout<Float>.size * 3, bufferIndex: 0)
//        vertexDescriptor.attributes[2] = MDLVertexAttribute(name: MDLVertexAttributeTextureCoordinate, format: .float2, offset: MemoryLayout<Float>.size * 6, bufferIndex: 0)
//        vertexDescriptor.layouts[0] = MDLVertexBufferLayout(stride: MemoryLayout<Float>.size * 8)
//
//        modelVertexDescriptor = MTKMetalVertexDescriptorFromModelIO(vertexDescriptor)
//
//        let bufferAllocator = MTKMeshBufferAllocator(device: device)
//
//        let asset = MDLAsset(url: modelURL, vertexDescriptor: vertexDescriptor, bufferAllocator: bufferAllocator)
//
//        do {
//            (_, meshes) = try MTKMesh.newMeshes(asset: asset, device: device)
//        } catch {
//            fatalError("Could not extract meshes from Model I/O asset")
//        }
//
//    }
//
//    func buildPipelineState() {
//
//        guard let library = device.makeDefaultLibrary() else {
//            fatalError("Could not load default library from main bundle")
//        }
//
//        let vertexFunction = library.makeFunction(name: "vertex_main")
//        let fragmentFunction = library.makeFunction(name: "fragment_main")
//
//        let pipelineDescriptor = MTLRenderPipelineDescriptor()
//        pipelineDescriptor.vertexFunction = vertexFunction
//        pipelineDescriptor.fragmentFunction = fragmentFunction
//
//        pipelineDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat
//        pipelineDescriptor.depthAttachmentPixelFormat = mtkView.depthStencilPixelFormat
//        pipelineDescriptor.vertexDescriptor = modelVertexDescriptor
//
//        do {
//            pipelineState = try device.makeRenderPipelineState(descriptor: pipelineDescriptor)
//        } catch {
//            fatalError("Could not create render pipeline state object: \(error)")
//        }
//    }
//
//    static func buildDepthStencilState(device: MTLDevice) -> MTLDepthStencilState {
//        let depthStencilDescriptor = MTLDepthStencilDescriptor()
//        depthStencilDescriptor.depthCompareFunction = .less
//        depthStencilDescriptor.isDepthWriteEnabled = true
//        return device.makeDepthStencilState(descriptor: depthStencilDescriptor)!
//    }
//
//    func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
//    }
//
//    func draw(in view: MTKView) {
//        let commandBuffer = commandQueue.makeCommandBuffer()!
//
//        if let renderPassDescriptor = view.currentRenderPassDescriptor, let drawable = view.currentDrawable {
//            let commandEncoder = commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor)!
//
////            let modelMatrix = float4x4(rotationAbout: float3(1, 0, 0), by: 90) *  float4x4(scaleBy: 1)
//            let modelMatrix = float4x4(rotationAbout: float3(1, 0, 0), by: 0) *  float4x4(scaleBy: 1)
////            let viewMatrix = float4x4(translationBy: float3(-75, -50, -400))
////            let viewMatrix = float4x4(translationBy: float3(0, -10, -50))
//            let viewMatrix = float4x4(translationBy: float3(0, 0, -2))
//            let aspectRatio = Float(view.drawableSize.width / view.drawableSize.height)
//            let projectionMatrix = float4x4(perspectiveProjectionFov: Float.pi / 3, aspectRatio: aspectRatio, nearZ: 0.1, farZ: 500)
//            let viewProjectionMatrix = viewMatrix * projectionMatrix
//
////            let url: URL = Bundle.main.url(forResource: "texture", withExtension: "png")!
////            let texLoader: MTKTextureLoader = MTKTextureLoader(device: device)
////
////            do {
////                let texture: MTLTexture = try texLoader.newTexture(URL: url, options: nil)
////                commandEncoder.setFragmentTexture(texture, index: 0)
////            } catch {
////                print(error)
////            }
//
////            var uniforms = Uniforms(modelViewMatrix: modelViewMatrix, projectionMatrix: projectionMatrix)
//            var uniforms = Uniforms(modelMatrix: modelMatrix, viewProjectionMatrix: viewProjectionMatrix, normalMatrix: modelMatrix.normalMatrix)
//
//            commandEncoder.setVertexBytes(&uniforms, length: MemoryLayout<Uniforms>.size, index: 1)
//            commandEncoder.setDepthStencilState(depthStencilState)
//            commandEncoder.setRenderPipelineState(pipelineState)
//
//            for mesh in meshes {
//                let vertexBuffer = mesh.vertexBuffers.first!
//                commandEncoder.setVertexBuffer(vertexBuffer.buffer, offset: vertexBuffer.offset, index: 0)
//
//                for submesh in mesh.submeshes {
//                    let indexBuffer = submesh.indexBuffer
//                    commandEncoder.drawIndexedPrimitives(type: submesh.primitiveType,
//                                                         indexCount: submesh.indexCount,
//                                                         indexType: submesh.indexType,
//                                                         indexBuffer: indexBuffer.buffer,
//                                                         indexBufferOffset: indexBuffer.offset)
//                }
//            }
//
//            commandEncoder.endEncoding()
//            commandBuffer.present(drawable)
//            commandBuffer.commit()
//        }
//    }
//}

import Foundation
import MetalKit
import simd

struct Uniforms {
    var modelMatrix: float4x4
    var viewProjectionMatrix: float4x4
    var normalMatrix: float3x3
}

class Renderer: NSObject, MTKViewDelegate {
    let device: MTLDevice
    let commandQueue: MTLCommandQueue
    var renderPipeline: MTLRenderPipelineState
    let depthStencilState: MTLDepthStencilState
    var vertexDescriptor: MDLVertexDescriptor
    var meshes: [MTKMesh] = []
    var time: Float = 0
    
    init(view: MTKView, device: MTLDevice) {
        self.device = device
        commandQueue = device.makeCommandQueue()!
        vertexDescriptor = Renderer.buildVertexDescriptor()
        renderPipeline = Renderer.buildPipeline(device: device, view: view, vertexDescriptor: vertexDescriptor)
        depthStencilState = Renderer.buildDepthStencilState(device: device)
        super.init()
        loadResources()
    }
    
    func loadResources() {
        let modelURL = Bundle.main.url(forResource: "teapot", withExtension: "obj")!
        
        let bufferAllocator = MTKMeshBufferAllocator(device: device)
        
        let asset = MDLAsset(url: modelURL, vertexDescriptor: vertexDescriptor, bufferAllocator: bufferAllocator)
        
        do {
            (_, meshes) = try MTKMesh.newMeshes(asset: asset, device: device)
        } catch {
            fatalError("Could not extract meshes from Model I/O asset")
        }
    }
    
    static func buildVertexDescriptor() -> MDLVertexDescriptor {
        let vertexDescriptor = MDLVertexDescriptor()
        vertexDescriptor.attributes[0] = MDLVertexAttribute(name: MDLVertexAttributePosition,
                                                            format: .float3,
                                                            offset: 0,
                                                            bufferIndex: 0)
        vertexDescriptor.attributes[1] = MDLVertexAttribute(name: MDLVertexAttributeNormal,
                                                            format: .float3,
                                                            offset: MemoryLayout<Float>.size * 3,
                                                            bufferIndex: 0)
        vertexDescriptor.attributes[2] = MDLVertexAttribute(name: MDLVertexAttributeTextureCoordinate,
                                                            format: .float2,
                                                            offset: MemoryLayout<Float>.size * 6,
                                                            bufferIndex: 0)
        vertexDescriptor.layouts[0] = MDLVertexBufferLayout(stride: MemoryLayout<Float>.size * 8)
        return vertexDescriptor
    }
    
    static func buildDepthStencilState(device: MTLDevice) -> MTLDepthStencilState {
        let depthStencilDescriptor = MTLDepthStencilDescriptor()
        depthStencilDescriptor.depthCompareFunction = .less
        depthStencilDescriptor.isDepthWriteEnabled = true
        return device.makeDepthStencilState(descriptor: depthStencilDescriptor)!
    }
    
    static func buildPipeline(device: MTLDevice, view: MTKView, vertexDescriptor: MDLVertexDescriptor) -> MTLRenderPipelineState {
        guard let library = device.makeDefaultLibrary() else {
            fatalError("Could not load default library from main bundle")
        }
        
        let vertexFunction = library.makeFunction(name: "vertex_main")
        let fragmentFunction = library.makeFunction(name: "fragment_main")
        
        let pipelineDescriptor = MTLRenderPipelineDescriptor()
        pipelineDescriptor.vertexFunction = vertexFunction
        pipelineDescriptor.fragmentFunction = fragmentFunction
        
        pipelineDescriptor.colorAttachments[0].pixelFormat = view.colorPixelFormat
        pipelineDescriptor.depthAttachmentPixelFormat = view.depthStencilPixelFormat
        
        let mtlVertexDescriptor = MTKMetalVertexDescriptorFromModelIO(vertexDescriptor)
        pipelineDescriptor.vertexDescriptor = mtlVertexDescriptor
        
        do {
            return try device.makeRenderPipelineState(descriptor: pipelineDescriptor)
        } catch {
            fatalError("Could not create render pipeline state object: \(error)")
        }
    }
    
    func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
    }
    
    func draw(in view: MTKView) {
        let commandBuffer = commandQueue.makeCommandBuffer()!
        
        if let renderPassDescriptor = view.currentRenderPassDescriptor, let drawable = view.currentDrawable {
            let commandEncoder = commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor)!
            commandEncoder.setDepthStencilState(depthStencilState)
            
            let modelMatrix = float4x4(rotationAbout: float3(0, 1, 0), by: 5) *  float4x4(scaleBy: 1)
            let viewMatrix = float4x4(translationBy: float3(0, 0, -2))
            let aspectRatio = Float(view.drawableSize.width / view.drawableSize.height)
            let projectionMatrix = float4x4(perspectiveProjectionFov: Float.pi / 3, aspectRatio: aspectRatio, nearZ: 0.1, farZ: 100)
            let viewProjectionMatrix = projectionMatrix * viewMatrix
            
            var uniforms = Uniforms(modelMatrix: modelMatrix, viewProjectionMatrix: viewProjectionMatrix, normalMatrix: modelMatrix.normalMatrix)
            
            commandEncoder.setVertexBytes(&uniforms, length: MemoryLayout<Uniforms>.size, index: 1)
            commandEncoder.setRenderPipelineState(renderPipeline)
            
            let url: URL = Bundle.main.url(forResource: "tiles_baseColor", withExtension: "jpg")!
            let loader: MTKTextureLoader = MTKTextureLoader(device: device)
            do {
                let texture: MTLTexture = try loader.newTexture(URL: url, options: nil)
                commandEncoder.setFragmentTexture(texture, index: 0)
            } catch {
                print(error)
            }
            
            for mesh in meshes {
                let vertexBuffer = mesh.vertexBuffers.first!
                commandEncoder.setVertexBuffer(vertexBuffer.buffer, offset: vertexBuffer.offset, index: 0)
                
                for submesh in mesh.submeshes {
                    let indexBuffer = submesh.indexBuffer
                    commandEncoder.drawIndexedPrimitives(type: submesh.primitiveType,
                                                         indexCount: submesh.indexCount,
                                                         indexType: submesh.indexType,
                                                         indexBuffer: indexBuffer.buffer,
                                                         indexBufferOffset: indexBuffer.offset)
                }
            }
            
            commandEncoder.endEncoding()
            
            commandBuffer.present(drawable)
            
            commandBuffer.commit()
        }
    }
}
