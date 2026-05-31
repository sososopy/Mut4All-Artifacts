use syn::{GenericParam, Type, TypeParamBound, TypePath, PathSegment, Ident, punctuated::Punctuated, TraitBound};
use proc_macro2::Span;

struct Substitute_Generic_Constraints_212;

impl Substitute_Generic_Constraints_212 {
    fn name(&self) -> &str {
        "Substitute_Generic_Constraints_212"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let syn::WherePredicate::Type(predicate_type) = predicate {
                            if let Some(trait_bound) = predicate_type.bounds.iter().find_map(|bound| {
                                if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                    Some(trait_bound)
                                } else {
                                    None
                                }
                            }) {
                                let new_trait_bound = TraitBound {
                                    paren_token: trait_bound.paren_token,
                                    modifier: trait_bound.modifier,
                                    lifetimes: trait_bound.lifetimes,
                                    path: syn::Path {
                                        leading_colon: trait_bound.path.leading_colon,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: Ident::new("Display", Span::call_site()),
                                            arguments: trait_bound.path.segments.last().unwrap().arguments.clone(),
                                        }]),
                                    },
                                };
                                predicate_type.bounds = Punctuated::from_iter(vec![TypeParamBound::Trait(new_trait_bound)]);
                            }
                        }
                    }
                }
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(const_type) = const_param.ty.as_ref() {
                            if let Type::Path(type_path) = const_type {
                                if let Some(segment) = type_path.path.segments.last() {
                                    if segment.ident == "usize" {
                                        let new_type_path = TypePath {
                                            qself: type_path.qself,
                                            path: syn::Path {
                                                leading_colon: type_path.path.leading_colon,
                                                segments: Punctuated::from_iter(vec![PathSegment {
                                                    ident: Ident::new("isize", Span::call_site()),
                                                    arguments: segment.arguments.clone(),
                                                }]),
                                            },
                                        };
                                        const_param.ty = Some(Box::new(new_type_path));
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let Some(where_clause) = &mut func.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let syn::WherePredicate::Type(predicate_type) = predicate {
                                    if let Some(trait_bound) = predicate_type.bounds.iter().find_map(|bound| {
                                        if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                            Some(trait_bound)
                                        } else {
                                            None
                                        }
                                    }) {
                                        let new_trait_bound = TraitBound {
                                            paren_token: trait_bound.paren_token,
                                            modifier: trait_bound.modifier,
                                            lifetimes: trait_bound.lifetimes,
                                            path: syn::Path {
                                                leading_colon: trait_bound.path.leading_colon,
                                                segments: Punctuated::from_iter(vec![PathSegment {
                                                    ident: Ident::new("Display", Span::call_site()),
                                                    arguments: trait_bound.path.segments.last().unwrap().arguments.clone(),
                                                }]),
                                            },
                                        };
                                        predicate_type.bounds = Punctuated::from_iter(vec![TypeParamBound::Trait(new_trait_bound)]);
                                    }
                                }
                            }
                        }
                        for param in &mut func.sig.generics.params {
                            if let GenericParam::Const(const_param) = param {
                                if let Some(const_type) = const_param.ty.as_ref() {
                                    if let Type::Path(type_path) = const_type {
                                        if let Some(segment) = type_path.path.segments.last() {
                                            if segment.ident == "usize" {
                                                let new_type_path = TypePath {
                                                    qself: type_path.qself,
                                                    path: syn::Path {
                                                        leading_colon: type_path.path.leading_colon,
                                                        segments: Punctuated::from_iter(vec![PathSegment {
                                                            ident: Ident::new("isize", Span::call_site()),
                                                            arguments: segment.arguments.clone(),
                                                        }]),
                                                    },
                                                };
                                                const_param.ty = Some(Box::new(new_type_path));
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
        "The mutation operator substitutes generic constraints in function and method signatures. It replaces trait bounds with new ones and modifies const generic parameters, aiming to test the compiler's ability to resolve generic constraints and handle const generics correctly."
    }
}