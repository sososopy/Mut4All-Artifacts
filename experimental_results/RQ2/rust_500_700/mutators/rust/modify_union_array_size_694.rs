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

pub struct Modify_Union_Array_Size_694;

impl Mutator for Modify_Union_Array_Size_694 {
    fn name(&self) -> &str {
        "Modify_Union_Array_Size_694"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Union(item_union) = item {
                for field in &mut item_union.fields.named {
                    if let Type::Array(type_array) = &mut field.ty {
                        if let Expr::Lit(expr_lit) = &type_array.len {
                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                if lit_int.base10_parse::<usize>().unwrap_or(1) == 0 {
                                    type_array.len = parse_quote!(1);
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