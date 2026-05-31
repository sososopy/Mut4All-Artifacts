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

pub struct Replace_Type_Alias_With_Underlying_Type_480;

impl Mutator for Replace_Type_Alias_With_Underlying_Type_480 {
    fn name(&self) -> &str {
        "Replace_Type_Alias_With_Underlying_Type_480"
    }

    fn mutate(&self, file: &mut syn::File) {
        // Collect type aliases
        let mut type_aliases = Vec::new();
        for item in &file.items {
            if let Item::Type(item_type) = item {
                if let Type::Path(TypePath {
                    qself: None,
                    path: SynPath {
                        leading_colon: None,
                        segments,
                    },
                }) = &*item_type.ty
                {
                    if let Some(segment) = segments.last() {
                        type_aliases.push((segment.ident.clone(), item_type.ty.clone()));
                    }
                }
            }
        }

        // Replace type aliases with their underlying types
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for arg in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        for (alias, underlying_type) in &type_aliases {
                            if let Type::Path(TypePath {
                                qself: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments,
                                },
                            }) = &*pat_type.ty
                            {
                                if let Some(segment) = segments.last() {
                                    if segment.ident == *alias {
                                        pat_type.ty = underlying_type.clone();
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        for arg in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = arg {
                                for (alias, underlying_type) in &type_aliases {
                                    if let Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments,
                                        },
                                    }) = &*pat_type.ty
                                    {
                                        if let Some(segment) = segments.last() {
                                            if segment.ident == *alias {
                                                pat_type.ty = underlying_type.clone();
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
        "This mutation operator replaces type aliases with their underlying types, testing the compiler's ability to handle complex type relationships and aliasing."
    }
}