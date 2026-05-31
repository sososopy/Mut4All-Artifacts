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

pub struct Modify_Const_Expr_In_Struct_Impl_542;

impl Mutator for Modify_Const_Expr_In_Struct_Impl_542 {
    fn name(&self) -> &str {
        "Modify_Const_Expr_In_Struct_Impl_542"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if path.segments.last().unwrap().ident == "From" {
                        for impl_item in &mut item_impl.items {
                            if let syn::ImplItem::Fn(func) = impl_item {
                                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                                    if let Type::Path(type_path) = &**ty {
                                        if let Some(last_segment) = type_path.path.segments.last() {
                                            if let PathArguments::AngleBracketed(ref mut args) =
                                                last_segment.arguments
                                            {
                                                for arg in &mut args.args {
                                                    if let GenericArgument::Const(expr) = arg {
                                                        if let Expr::Block(expr_block) = expr {
                                                            if let Some(stmt) =
                                                                expr_block.block.stmts.first_mut()
                                                            {
                                                                if let Stmt::Expr(Expr::Binary(
                                                                    expr_binary,
                                                                )) = stmt
                                                                {
                                                                    let mut rng = thread_rng();
                                                                    let new_op = match rng
                                                                        .gen_range(0..4)
                                                                    {
                                                                        0 => syn::BinOp::Add(
                                                                            token::Add::default(),
                                                                        ),
                                                                        1 => syn::BinOp::Sub(
                                                                            token::Sub::default(),
                                                                        ),
                                                                        2 => syn::BinOp::Mul(
                                                                            token::Mul::default(),
                                                                        ),
                                                                        _ => syn::BinOp::Div(
                                                                            token::Div::default(),
                                                                        ),
                                                                    };
                                                                    expr_binary.op = new_op;
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
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}