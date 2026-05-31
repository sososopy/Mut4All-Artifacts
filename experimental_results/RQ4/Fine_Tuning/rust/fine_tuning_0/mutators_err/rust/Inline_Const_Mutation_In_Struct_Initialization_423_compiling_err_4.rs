use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit::Visit, visit_mut::VisitMut,
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, *,
};

use crate::mutator::Mutator;

pub struct Inline_Const_Mutation_In_Struct_Initialization_423;

impl Mutator for Inline_Const_Mutation_In_Struct_Initialization_423 {
    fn name(&self) -> &str {
        "Inline_Const_Mutation_In_Struct_Initialization_423"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if let Some(block) = func.block.stmts.first_mut() {
                            if let Stmt::Expr(Expr::Block(expr_block), _) = block {
                                if let Some(Stmt::Expr(Expr::Struct(expr_struct), _)) = expr_block.block.stmts.first_mut() {
                                    if let Some(field) = expr_struct.fields.first_mut() {
                                        if let Expr::Call(expr_call) = &mut field.expr {
                                            let temp_ident = Ident::new("temp_value", Span::call_site());
                                            let temp_stmt: Stmt = parse_quote! {
                                                let #temp_ident = #expr_call;
                                            };
                                            expr_block.block.stmts.insert(0, temp_stmt);
                                            field.expr = parse_quote! { #temp_ident };
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