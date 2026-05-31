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

pub struct Add_Explicit_Type_To_Const_Item_111;

impl Mutator for Add_Explicit_Type_To_Const_Item_111 {
    fn name(&self) -> &str {
        "Add_Explicit_Type_To_Const_Item_111"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut new_stmts = Vec::new();
                for stmt in &func.block.stmts {
                    if let syn::Stmt::Item(syn::Item::Const(item_const)) = stmt {
                        if item_const.ty.is_empty() {
                            let new_stmt = syn::Stmt::Item(syn::Item::Const(syn::ItemConst {
                                attrs: item_const.attrs.clone(),
                                vis: item_const.vis.clone(),
                                const_token: item_const.const_token,
                                ident: item_const.ident.clone(),
                                generics: item_const.generics.clone(),
                                colon_token: token::Colon::default(),
                                ty: Box::new(syn::Type::Path(syn::TypePath {
                                    qself: None,
                                    path: syn::Path::from(Ident::new("i32", Span::call_site())),
                                })),
                                eq_token: item_const.eq_token,
                                expr: item_const.expr.clone(),
                                semi_token: item_const.semi_token,
                            }));
                            new_stmts.push(new_stmt);
                            continue;
                        }
                    }
                    new_stmts.push(stmt.clone());
                }
                func.block.stmts = new_stmts;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets const items within functions that lack an explicit type annotation. It transforms such const items by adding a type annotation, specifically using `i32` as the default type. This ensures that the const item has an explicit type, reducing the likelihood of triggering type-related compiler errors or ICEs (Internal Compiler Errors) related to omitted types in const generics."
    }
}