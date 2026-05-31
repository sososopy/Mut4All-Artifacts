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

pub struct Mutator_Invalid_Const_Generic_Type_Assignment_375;

impl Mutator for Mutator_Invalid_Const_Generic_Type_Assignment_375 {
    fn name(&self) -> &str {
        "Invalid_Const_Generic_Type_Assignment_375"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for param in &mut item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        const_param.ty = Box::new(Type::Path(TypePath {
                            qself: None,
                            path: {
                                let mut segments = Punctuated::new();
                                segments.push(PathSegment {
                                    ident: Ident::new("str", Span::call_site()),
                                    arguments: PathArguments::None,
                                });
                                SynPath {
                                    leading_colon: None,
                                    segments,
                                }
                            },
                        }));
                        const_param.default = Some(Box::new(parse_quote!(10)));
                    }
                }
            } else if let Item::Enum(item_enum) = item {
                for param in &mut item_enum.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        const_param.ty = Box::new(Type::Path(TypePath {
                            qself: None,
                            path: {
                                let mut segments = Punctuated::new();
                                segments.push(PathSegment {
                                    ident: Ident::new("str", Span::call_site()),
                                    arguments: PathArguments::None,
                                });
                                SynPath {
                                    leading_colon: None,
                                    segments,
                                }
                            },
                        }));
                        const_param.default = Some(Box::new(parse_quote!(10)));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}