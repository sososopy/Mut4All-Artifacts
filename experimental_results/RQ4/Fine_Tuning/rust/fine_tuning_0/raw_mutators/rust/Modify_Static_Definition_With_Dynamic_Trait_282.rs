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

pub struct Modify_Static_Definition_With_Dynamic_Trait_282;

impl Mutator for Modify_Static_Definition_With_Dynamic_Trait_282 {
    fn name(&self) -> &str {
        "Modify_Static_Definition_With_Dynamic_Trait_282"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let mut new_items = Vec::new();
                for stmt in &func.block.stmts {
                    if let Stmt::Item(Item::Static(item_static)) = stmt {
                        if let Type::TraitObject(type_trait_object) = &*item_static.ty {
                            let fn_name = &item_static.ident;
                            let fn_return_type: Type = parse_quote! {
                                Box<dyn #type_trait_object>
                            };
                            let fn_body = &item_static.expr;
                            let new_fn: ItemFn = parse_quote! {
                                fn #fn_name() -> #fn_return_type {
                                    Box::new(#fn_body)
                                }
                            };
                            new_items.push(Stmt::Item(Item::Fn(new_fn)));
                        } else {
                            new_items.push(stmt.clone());
                        }
                    } else {
                        new_items.push(stmt.clone());
                    }
                }
                func.block.stmts = new_items;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}