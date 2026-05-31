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

pub struct Toggle_Const_Generic_Default_Block_350;

impl Mutator for Toggle_Const_Generic_Default_Block_350 {
    fn name(&self) -> &str {
        "Toggle_Const_Generic_Default_Block_350"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for param in &mut item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(default) = &const_param.default {
                            match default {
                                Expr::Block(expr_block) if expr_block.attrs.is_empty() => {
                                    if let Some(stmt) = expr_block.block.stmts.first() {
                                        if let Stmt::Expr(inner_expr, None) = stmt {
                                            if is_valid_const_generic_expr(inner_expr) {
                                                const_param.default = Some(inner_expr.clone());
                                            }
                                        }
                                    }
                                }
                                expr if is_valid_const_generic_expr(expr) => {
                                    let new_block: Expr = parse_quote! { const { #expr } };
                                    const_param.default = Some(new_block);
                                }
                                _ => {}
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct definitions with const generic parameters that have default values. It toggles between inline const block defaults and simple constant expression defaults. When the default is an inline const block containing a single expression, it extracts that expression as the new default. When the default is a simple constant expression, it wraps it in an inline const block. This transformation tests the compiler's handling of const generic defaults, particularly the interaction between inline const blocks and generic parameter defaults, potentially revealing edge cases in const evaluation and generic instantiation."
    }
}

fn is_valid_const_generic_expr(expr: &Expr) -> bool {
    match expr {
        Expr::Lit(_) => true,
        Expr::Path(ExprPath { path, .. }) if path.segments.len() == 1 => true,
        Expr::Unary(unary) => is_valid_const_generic_expr(&unary.expr),
        Expr::Binary(binary) => {
            is_valid_const_generic_expr(&binary.left) && is_valid_const_generic_expr(&binary.right)
        }
        Expr::Paren(paren) => is_valid_const_generic_expr(&paren.expr),
        _ => false,
    }
}