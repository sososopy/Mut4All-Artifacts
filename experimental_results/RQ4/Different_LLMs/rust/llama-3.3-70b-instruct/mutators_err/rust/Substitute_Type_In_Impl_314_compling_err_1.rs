use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
};
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

pub struct Substitute_Type_In_Impl_314;

impl Mutator for Substitute_Type_In_Impl_314 {
    fn name(&self) -> &str {
        "Substitute_Type_In_Impl_314"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut types: Vec<Type> = Vec::new();
        for item in &file.items {
            if let Item::Struct(struct_item) = item {
                types.push(Type::Path(TypePath {
                    qself: None,
                    path: SynPath {
                        leading_colon: None,
                        segments: Punctuated::from_iter(vec![PathSegment {
                            ident: struct_item.ident.clone(),
                            arguments: PathArguments::None,
                        }]),
                    },
                }));
            }
        }

        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if let Some(type_path) = &impl_item.self_ty {
                    if let Type::Path(path) = type_path {
                        if let Some(new_type) = types.choose(&mut thread_rng()) {
                            impl_item.self_ty = Some(Box::new(Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: new_type
                                            .clone()
                                            .into_value()
                                            .as_path()
                                            .and_then(|p| p.segments.last())
                                            .and_then(|s| s.ident.clone()),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                            })));
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator substitutes the type in an impl block with another existing type in the same scope. This transformation aims to test the compiler's handling of type substitutions and their impact on trait resolution and method dispatch."
    }
}