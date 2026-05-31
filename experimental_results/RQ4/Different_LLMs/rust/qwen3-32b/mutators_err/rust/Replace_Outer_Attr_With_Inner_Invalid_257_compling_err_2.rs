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

pub struct Replace_Outer_Attr_With_Inner_Invalid_257;

impl Mutator for Replace_Outer_Attr_With_Inner_Invalid_257 {
    fn name(&self) -> &str {
        "Replace_Outer_Attr_With_Inner_Invalid_257"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut functions: Vec<&mut ItemFn> = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                functions.push(func);
            }
        }

        for item in &mut file.items {
            match item {
                syn::Item::Fn(func) => {
                    let mut new_attrs = Vec::new();
                    for attr in &func.attrs {
                        if attr.style == AttrStyle::Outer {
                            let mut inner_attr = attr.clone();
                            inner_attr.style = AttrStyle::Inner;
                            
                            if let Some(first_func) = functions.get_mut(0) {
                                let expr = parse_quote! { 42 };
                                let mut expr_with_attr = expr;
                                expr_with_attr.attrs.push(inner_attr);
                                let stmt = Stmt::Expr(expr_with_attr, None);
                                if let Some(block) = first_func.block.as_mut() {
                                    block.stmts.insert(0, stmt);
                                }
                            }
                        } else {
                            new_attrs.push(attr.clone());
                        }
                    }
                    func.attrs = new_attrs;
                },
                syn::Item::Struct(struc) => {
                    let mut new_attrs = Vec::new();
                    for attr in &struc.attrs {
                        if attr.style == AttrStyle::Outer {
                            let mut inner_attr = attr.clone();
                            inner_attr.style = AttrStyle::Inner;
                            
                            if let Some(first_func) = functions.get_mut(0) {
                                let expr = parse_quote! { 42 };
                                let mut expr_with_attr = expr;
                                expr_with_attr.attrs.push(inner_attr);
                                let stmt = Stmt::Expr(expr_with_attr, None);
                                if let Some(block) = first_func.block.as_mut() {
                                    block.stmts.insert(0, stmt);
                                }
                            }
                        } else {
                            new_attrs.push(attr.clone());
                        }
                    }
                    struc.attrs = new_attrs;
                },
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}