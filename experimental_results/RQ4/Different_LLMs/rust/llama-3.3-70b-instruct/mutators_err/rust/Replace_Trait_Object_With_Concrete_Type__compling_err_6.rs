use syn::{Item, ReturnType, Type, TypePath, PathSegment, PathArguments, FnArg, ImplItem, punctuated::Punctuated};
use rand::prelude::*;
use rand::thread_rng;

pub struct ReplaceTraitObjectWithConcreteType;

impl ReplaceTraitObjectWithConcreteType {
    pub fn new() -> Self {
        ReplaceTraitObjectWithConcreteType
    }

    pub fn name(&self) -> &str {
        "ReplaceTraitObjectWithConcreteType"
    }

    pub fn mutate(&self, file: &mut syn::File) {
        let mut concrete_types = Vec::new();

        for item in &file.items {
            if let Item::Struct(struct_item) = item {
                concrete_types.push(struct_item.ident.clone());
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        if let Some(concrete_type) = concrete_types.choose(&mut thread_rng()) {
                            *return_type = Box::new(Type::Path(TypePath {
                                qself: None,
                                path: syn::Path {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: concrete_type.clone(),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                            }));
                        }
                    }
                }

                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::TraitObject(trait_object) = &*pat_type.ty {
                            if let Some(concrete_type) = concrete_types.choose(&mut thread_rng()) {
                                pat_type.ty = Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: syn::Path {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: concrete_type.clone(),
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                }));
                            }
                        }
                    }
                }
            }

            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(method) = impl_item {
                        if let ReturnType::Type(_, return_type) = &mut method.sig.output {
                            if let Type::ImplTrait(type_impl_trait) = &**return_type {
                                if let Some(concrete_type) = concrete_types.choose(&mut thread_rng()) {
                                    *return_type = Box::new(Type::Path(TypePath {
                                        qself: None,
                                        path: syn::Path {
                                            leading_colon: None,
                                            segments: Punctuated::from_iter(vec![PathSegment {
                                                ident: concrete_type.clone(),
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    }));
                                }
                            }
                        }

                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::TraitObject(trait_object) = &*pat_type.ty {
                                    if let Some(concrete_type) = concrete_types.choose(&mut thread_rng()) {
                                        pat_type.ty = Box::new(Type::Path(TypePath {
                                            qself: None,
                                            path: syn::Path {
                                                leading_colon: None,
                                                segments: Punctuated::from_iter(vec![PathSegment {
                                                    ident: concrete_type.clone(),
                                                    arguments: PathArguments::None,
                                                }]),
                                            },
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

    pub fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces trait object types with concrete types in function and method signatures. This transformation tests the compiler's handling of trait objects and concrete types, potentially leading to type mismatches or inference failures."
    }
}