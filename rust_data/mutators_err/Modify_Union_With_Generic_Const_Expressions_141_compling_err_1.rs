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

pub struct Modify_Union_With_Generic_Const_Expressions_141;

impl Mutator for Modify_Union_With_Generic_Const_Expressions_141 {
    fn name(&self) -> &str {
        "Modify_Union_With_Generic_Const_Expressions_141"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Union(item_union) = item {
                let mut has_generic_const_expr = false;
                for field in &item_union.fields.named {
                    if let Type::Array(type_array) = &field.ty {
                        if let Expr::Block(expr_block) = &*type_array.len {
                            if expr_block.block.stmts.len() == 1 {
                                if let Stmt::Expr(Expr::Call(expr_call)) = &expr_block.block.stmts[0] {
                                    if let Expr::Path(ExprPath { path, .. }) = &*expr_call.func {
                                        if path.segments.last().unwrap().ident == "size_of" {
                                            has_generic_const_expr = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if has_generic_const_expr {
                    let new_generic_param: GenericParam = parse_quote!(const N: usize);
                    item_union.generics.params.push(new_generic_param);
                    for field in &mut item_union.fields.named {
                        if let Type::Array(type_array) = &mut field.ty {
                            if let Expr::Block(expr_block) = &mut *type_array.len {
                                if expr_block.block.stmts.len() == 1 {
                                    if let Stmt::Expr(Expr::Call(expr_call)) = &mut expr_block.block.stmts[0] {
                                        if let Expr::Path(ExprPath { path, .. }) = &mut *expr_call.func {
                                            if path.segments.last().unwrap().ident == "size_of" {
                                                let new_expr: Expr = parse_quote!(N + mem::size_of::<T>());
                                                expr_block.block.stmts[0] = Stmt::Expr(new_expr);
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
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies union definitions with fields using generic const expressions, specifically targeting array lengths defined by such expressions. It introduces a new const generic parameter 'N' to the union, modifying the array length expression to include 'N', thus increasing complexity and testing the compiler's handling of const generics."
    }
}