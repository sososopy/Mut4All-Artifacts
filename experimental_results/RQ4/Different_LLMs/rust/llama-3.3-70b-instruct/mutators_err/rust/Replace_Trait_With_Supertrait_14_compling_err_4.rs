use syn::{Item, Type, TypePath, FnArg, ImplItem};
use crate::mutator::Mutator;

struct Replace_Trait_With_Supertrait_14;

impl Mutator for Replace_Trait_With_Supertrait_14 {
    fn name(&self) -> &str {
        "Replace_Trait_With_Supertrait_14"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                if let Some(supertraits) = &trait_item.supertraits {
                    for supertrait in supertraits.iter() {
                        if let Type::Path(TypePath {
                            qself: None,
                            path: supertrait_path,
                        }) = supertrait {
                            for item2 in &mut file.items {
                                if let Item::Fn(func) = item2 {
                                    for input in &mut func.sig.inputs {
                                        if let FnArg::Typed(pat_type) = input {
                                            if let Type::Path(TypePath {
                                                qself: None,
                                                path: path,
                                            }) = &*pat_type.ty {
                                                if path.segments.last().unwrap().ident == trait_item.ident {
                                                    pat_type.ty = Box::new(Type::Path(TypePath {
                                                        qself: None,
                                                        path: supertrait_path.clone(),
                                                    }));
                                                }
                                            }
                                        }
                                    }
                                }
                                if let Item::Struct(struct_item) = item2 {
                                    for field in &mut struct_item.fields {
                                        if let syn::Field::Named(field) = field {
                                            if let Type::Path(TypePath {
                                                qself: None,
                                                path: path,
                                            }) = &*field.ty {
                                                if path.segments.last().unwrap().ident == trait_item.ident {
                                                    field.ty = Box::new(Type::Path(TypePath {
                                                        qself: None,
                                                        path: supertrait_path.clone(),
                                                    }));
                                                }
                                            }
                                        } else if let syn::Field::Unnamed(field) = field {
                                            if let Type::Path(TypePath {
                                                qself: None,
                                                path: path,
                                            }) = &*field.ty {
                                                if path.segments.last().unwrap().ident == trait_item.ident {
                                                    field.ty = Box::new(Type::Path(TypePath {
                                                        qself: None,
                                                        path: supertrait_path.clone(),
                                                    }));
                                                }
                                            }
                                        }
                                    }
                                }
                                if let Item::Impl(impl_item) = item2 {
                                    for impl_item in &mut impl_item.items {
                                        if let ImplItem::Fn(func) = impl_item {
                                            for input in &mut func.sig.inputs {
                                                if let FnArg::Typed(pat_type) = input {
                                                    if let Type::Path(TypePath {
                                                        qself: None,
                                                        path: path,
                                                    }) = &*pat_type.ty {
                                                        if path.segments.last().unwrap().ident == trait_item.ident {
                                                            pat_type.ty = Box::new(Type::Path(TypePath {
                                                                qself: None,
                                                                path: supertrait_path.clone(),
                                                            }));
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces traits with their supertraits in function definitions, struct definitions, and trait definitions. This transformation aims to test the compiler's handling of trait hierarchies and supertrait relationships, potentially uncovering bugs in trait resolution and implementation."
    }
}