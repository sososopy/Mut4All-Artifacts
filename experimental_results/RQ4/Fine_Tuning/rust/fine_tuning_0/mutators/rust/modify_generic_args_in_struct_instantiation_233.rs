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

pub struct Modify_Generic_Args_In_Struct_Instantiation_233;

impl Mutator for Modify_Generic_Args_In_Struct_Instantiation_233 {
    fn name(&self) -> &str {
        "Modify_Generic_Args_In_Struct_Instantiation_233"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut visitor = StructInstantiationVisitor;
                visitor.visit_item_fn_mut(func);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies struct instantiations with generic arguments and adds an additional generic argument. This transformation aims to test the compiler's handling of unexpected generic parameter counts, potentially leading to type resolution issues or ICEs."
    }
}

struct StructInstantiationVisitor;

impl VisitMut for StructInstantiationVisitor {
    fn visit_expr_path_mut(&mut self, expr_path: &mut ExprPath) {
        if let Some(last_segment) = expr_path.path.segments.last_mut() {
            if let PathArguments::AngleBracketed(ref mut args) = last_segment.arguments {
                args.args.push(GenericArgument::Type(parse_quote!(i32)));
            }
        }
        syn::visit_mut::visit_expr_path_mut(self, expr_path);
    }
}