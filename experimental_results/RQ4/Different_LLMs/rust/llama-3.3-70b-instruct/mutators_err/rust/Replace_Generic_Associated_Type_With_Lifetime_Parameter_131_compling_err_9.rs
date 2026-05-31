use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
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

pub struct Replace_Generic_Associated_Type_With_Lifetime_Parameter_131;

impl Mutator for Replace_Generic_Associated_Type_With_Lifetime_Parameter_131 {
    fn name(&self) -> &str {
        "Replace_Generic_Associated_Type_With_Lifetime_Parameter_131"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for method in &mut trait_item.items {
                    if let syn::TraitItem::Method(method) = method {
                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments,
                                    },
                                }) = &*pat_type.ty
                                {
                                    if let Some(segment) = segments.first() {
                                        if segment.ident == "Self" {
                                            let lifetime_param = LifetimeParam {
                                                attrs: vec![],
                                                colon_token: Some(token::Colon::default()),
                                                bounds: Punctuated::new(),
                                                lifetime: Lifetime::new(
                                                    &format!("_{}", thread_rng().gen::<u32>()),
                                                    Span::call_site(),
                                                ),
                                            };
                                            pat_type.ty = Box::new(Type::Reference(TypeReference {
                                                and_token: token::And::default(),
                                                lifetime: Some(lifetime_param.lifetime.clone()),
                                                mutability: None,
                                                elem: Box::new(Type::Path(TypePath {
                                                    qself: None,
                                                    path: SynPath {
                                                        leading_colon: None,
                                                        segments: Punctuated::from_iter(vec![PathSegment {
                                                            ident: Ident::new("i32", Span::call_site()),
                                                            arguments: PathArguments::None,
                                                        }]),
                                                    },
                                                })),
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

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces generic associated types in trait method signatures with lifetime parameters. This transformation checks if the compiler correctly handles the difference between generic associated types and lifetime parameters, which is a crucial aspect of the Rust type system."
    }
}