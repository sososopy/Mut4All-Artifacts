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

pub struct Modify_Union_In_Function_13;

impl Mutator for Modify_Union_In_Function_13 {
    fn name(&self) -> &str {
        "Modify_Union_In_Function_13"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut union_def_opt = find_union_definition(&mut func.block.stmts);
                        if let Some(union_def) = union_def_opt.as_mut() {
                            modify_union_and_function(func, union_def);
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

fn find_union_definition(stmts: &mut [Stmt]) -> Option<&mut ItemUnion> {
    for stmt in stmts {
        if let Stmt::Item(Item::Union(union_def)) = stmt {
            return Some(union_def);
        }
    }
    None
}

fn modify_union_and_function(func: &mut ImplItemFn, union_def: &mut ItemUnion) {
    let first_field_type = {
        let first_field = union_def.fields.named.first().unwrap();
        first_field.ty.clone()
    };
    let new_field: syn::Field = parse_quote! {
        b: #first_field_type
    };
    union_def.fields.named.push(new_field);

    if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
        *ty = Box::new(parse_quote!((Option<#first_field_type>, Option<#union_def.ident>)));
    }

    if let Some(last_stmt) = func.block.stmts.last_mut() {
        if let Stmt::Expr(expr, _) = last_stmt {
            *expr = parse_quote!((None, None));
        }
    }
}