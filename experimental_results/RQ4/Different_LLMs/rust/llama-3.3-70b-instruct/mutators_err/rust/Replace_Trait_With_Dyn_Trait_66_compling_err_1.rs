use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Replace_Trait_With_Dyn_Trait_66;

impl Mutator for Replace_Trait_With_Dyn_Trait_66 {
    fn name(&self) -> &str {
        "Replace_Trait_With_Dyn_Trait_66"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for param in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = param {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            for segment in &mut type_path.path.segments {
                                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                    for arg in &mut args.args {
                                        if let GenericArgument::Type(Type::Path(inner_type_path)) = arg {
                                            for inner_segment in &mut inner_type_path.path.segments {
                                                if let PathArguments::None = inner_segment.arguments {
                                                    inner_segment.arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                                        colon2_token: None,
                                                        lt_token: token::Lt::default(),
                                                        args: punctuated::Punctuated::from_iter(vec![GenericArgument::Type(Type::TraitObject(TypeTraitObject {
                                                            dyn_token: Some(token::Dyn::default()),
                                                            bounds: punctuated::Punctuated::new(),
                                                        }))]),
                                                        gt_token: token::Gt::default(),
                                                    });
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

            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if let syn::ReturnType::Type(_, ref mut ty) = method.sig.output {
                            if let Type::Path(type_path) = &**ty {
                                for segment in &mut type_path.path.segments {
                                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                        for arg in &mut args.args {
                                            if let GenericArgument::Type(Type::Path(inner_type_path)) = arg {
                                                for inner_segment in &mut inner_type_path.path.segments {
                                                    if let PathArguments::None = inner_segment.arguments {
                                                        inner_segment.arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                                            colon2_token: None,
                                                            lt_token: token::Lt::default(),
                                                            args: punctuated::Punctuated::from_iter(vec![GenericArgument::Type(Type::TraitObject(TypeTraitObject {
                                                                dyn_token: Some(token::Dyn::default()),
                                                                bounds: punctuated::Punctuated::new(),
                                                            }))]),
                                                            gt_token: token::Gt::default(),
                                                        });
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

            if let syn::Item::Struct(struct_item) = item {
                for field in &mut struct_item.fields {
                    if let Type::Path(type_path) = &*field.ty {
                        for segment in &mut type_path.path.segments {
                            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                for arg in &mut args.args {
                                    if let GenericArgument::Type(Type::Path(inner_type_path)) = arg {
                                        for inner_segment in &mut inner_type_path.path.segments {
                                            if let PathArguments::None = inner_segment.arguments {
                                                inner_segment.arguments = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                                    colon2_token: None,
                                                    lt_token: token::Lt::default(),
                                                    args: punctuated::Punctuated::from_iter(vec![GenericArgument::Type(Type::TraitObject(TypeTraitObject {
                                                        dyn_token: Some(token::Dyn::default()),
                                                        bounds: punctuated::Punctuated::new(),
                                                    }))]),
                                                    gt_token: token::Gt::default(),
                                                });
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
        "The mutation operator replaces trait bounds with dynamic trait objects, introducing dynamic dispatch and type erasure into the code. This transformation aims to test the compiler's handling of trait objects, lifetime elision, and closure compatibility, potentially leading to ICEs or inference failures in the trait resolution system."
    }
}