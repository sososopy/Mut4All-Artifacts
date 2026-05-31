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

pub struct Modify_Const_Generic_Parameters_401;

impl Mutator for Modify_Const_Generic_Parameters_401 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Parameters_401"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Struct(item_struct) => {
                    for param in &mut item_struct.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if let Type::Path(type_path) = const_param.ty {
                                if type_path.path.segments.last().unwrap().ident == "Dimension" {
                                    const_param.ty = Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath::from(Ident::new("i32", Span::call_site())),
                                    });
                                }
                            }
                        }
                    }
                }
                Item::Fn(item_fn) => {
                    for param in &mut item_fn.sig.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            if let Type::Path(type_path) = const_param.ty {
                                if type_path.path.segments.last().unwrap().ident == "Dimension" {
                                    const_param.ty = Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath::from(Ident::new("i32", Span::call_site())),
                                    });
                                }
                            }
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}