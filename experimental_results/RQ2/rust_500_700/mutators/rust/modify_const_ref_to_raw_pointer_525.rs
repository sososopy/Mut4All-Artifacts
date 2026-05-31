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

pub struct Modify_Const_Ref_To_Raw_Pointer_525;

impl Mutator for Modify_Const_Ref_To_Raw_Pointer_525 {
    fn name(&self) -> &str {
        "Modify_Const_Ref_To_Raw_Pointer_525"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Const(item_const) = item {
                if let Expr::Block(expr_block) = &*item_const.expr {
                    let mut found_ref = false;
                    let mut new_stmts = Vec::new();

                    for stmt in &expr_block.block.stmts {
                        new_stmts.push(stmt.clone());
                        if let Stmt::Expr(Expr::Reference(expr_ref), _) = stmt {
                            if let Expr::Path(ref expr_path) = *expr_ref.expr {
                                let ident = &expr_path.path.segments.last().unwrap().ident;
                                let new_stmt: Stmt = parse_quote! {
                                    let temp_ref = &*#ident;
                                };
                                new_stmts.push(new_stmt);
                                found_ref = true;
                            }
                        }
                    }

                    if found_ref {
                        let new_block = syn::Block {
                            brace_token: expr_block.block.brace_token,
                            stmts: new_stmts,
                        };
                        item_const.expr = Box::new(Expr::Block(ExprBlock {
                            attrs: expr_block.attrs.clone(),
                            label: expr_block.label.clone(),
                            block: new_block,
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}