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

pub struct Replace_Generic_Type_In_FFI_595;

impl Mutator for Replace_Generic_Type_In_FFI_595 {
    fn name(&self) -> &str {
        "Replace_Generic_Type_In_FFI_595"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut concrete_type = None;

        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                if concrete_type.is_none() {
                    concrete_type = Some(Type::Path(TypePath {
                        qself: None,
                        path: SynPath::from(item_struct.ident.clone()),
                    }));
                }
            }
        }

        let default_type: Type = parse_quote!(i32);

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Item(Item::ForeignMod(foreign_mod)) = stmt {
                        for foreign_item in &mut foreign_mod.items {
                            if let ForeignItem::Static(foreign_static) = foreign_item {
                                if let Type::Path(type_path) = &*foreign_static.ty {
                                    if type_path.path.segments.len() == 1 {
                                        let segment = &type_path.path.segments[0];
                                        if segment.ident.to_string() == "T" {
                                            foreign_static.ty = Box::new(concrete_type.clone().unwrap_or(default_type.clone()));
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
        ""
    }
}