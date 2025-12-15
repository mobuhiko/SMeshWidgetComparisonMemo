# SMeshWidgetComparisionMemo
Comparision of different methods that using SMeshWidget

## Method 1 - Particle(Original)

From [GitHub | dantreble/MeshWidgetExample](https://github.com/dantreble/MeshWidgetExample) mentioned in [Unreal Engine | Development | Rendering​ | SMeshWidget - Hardware Instanced Slate Meshes Thread](https://forums.unrealengine.com/t/smeshwidget-hardware-instanced-slate-meshes-thread/58020). 
(The MeshWidgetExample repository is also the base of this repository. )

### Files

ParticleWidget.h/cpp

## Method 2 - MyMesh

From [知乎 | 直接在UMG上渲染模型](https://zhuanlan.zhihu.com/p/393217635)

### Files

MyMeshWidget.h/cpp

## Method 3 - StaticMesh

From [知乎 | 虚幻5渲染编程(UI篇)第一卷: Slate渲染框架并通过为UMG拓展MeshUI了解Slate的图元阶段](https://zhuanlan.zhihu.com/p/387752531)

### Files

StaticMeshWidget.h/cpp

## Result

![Ret1](https://github.com/mobuhiko/SMeshWidgetComparision/blob/main/Image/SMeshWidgetComparision1.png)

It looks like Method 3 does not work with particle mesh, but sometimes the mesh is actually rendered in a weired way (that line, and it is rotating). 
It sometimes appears by changing the alignment of the slot. 
If you hide that widget, the line will also be hidden. 

![Ret2](https://github.com/mobuhiko/SMeshWidgetComparision/blob/main/Image/SMeshWidgetComparision2.png)

## Conclusion

All of the methods above have their own problems, so I don't think it is a good idea to use them in production. 
