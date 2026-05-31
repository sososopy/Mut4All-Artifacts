use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Replace_Const_Default_With_Type_381;

impl Mutator for Replace_Const_Default_With_Type_381 {
    fn name(&self) -> &str {
        "Replace_Const_Default_With_Type_381"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                self.process_generics(&mut item_struct.generics);
            } else if let syn::Item::Enum(item_enum) = item {
                self.process_generics(&mut item_enum.generics);
            } else if let syn::Item::Impl(item_impl) = item {
                self.process_generics(&mut item_impl.generics);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

impl Replace_Const_Default_With_Type_381 {
    fn process_generics(&self, generics: &mut syn::Generics) {
        for param in &mut generics.params {
            if let GenericParam::Const(const_param) = param {
                if let Some(default) = &mut const_param.default {
                    let types = ["u8", "i32", "String", "bool", "usize"];
                    let selected = types.choose(&mut thread_rng()).unwrap();
                    let new_expr: Expr = parse_quote!(#selected);
                    *default = Box::new(new_expr);
                }
            }
        }
    }
}