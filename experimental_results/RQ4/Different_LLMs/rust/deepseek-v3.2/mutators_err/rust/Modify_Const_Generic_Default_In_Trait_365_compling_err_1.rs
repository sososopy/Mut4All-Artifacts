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

pub struct Modify_Const_Generic_Default_In_Trait_365;

impl Mutator for Modify_Const_Generic_Default_In_Trait_365 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Default_In_Trait_365"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for param in &mut item_trait.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(default) = &mut const_param.default {
                            let new_expr = self.mutate_const_expr(default, &item_trait.generics, &mut rng);
                            if new_expr.is_some() {
                                *default = new_expr.unwrap();
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait definitions with generic const parameters that have default values. It modifies the default constant expression by altering its structure while maintaining type correctness and using only in-scope elements. Transformations include swapping binary operations, wrapping single parameters in blocks, or adjusting literal values. This stresses the compiler's const generic evaluation, type inference, and default value substitution in trait contexts."
    }
}

impl Modify_Const_Generic_Default_In_Trait_365 {
    fn mutate_const_expr(&self, expr: &Expr, generics: &Generics, rng: &mut impl Rng) -> Option<Expr> {
        match expr {
            Expr::Binary(bin_expr) => {
                let op = &bin_expr.op;
                let left = &bin_expr.left;
                let right = &bin_expr.right;
                let new_op = match op {
                    BinOp::Add(_) => Some(BinOp::Sub(token::Sub::default())),
                    BinOp::Sub(_) => Some(BinOp::Add(token::Add::default())),
                    BinOp::Mul(_) => Some(BinOp::Div(token::Div::default())),
                    BinOp::Div(_) => Some(BinOp::Mul(token::Mul::default())),
                    _ => None,
                };
                if let Some(new_op) = new_op {
                    let mut new_expr = bin_expr.clone();
                    new_expr.op = new_op;
                    Some(Expr::Binary(new_expr))
                } else if rng.gen_bool(0.5) {
                    let mut new_expr = bin_expr.clone();
                    std::mem::swap(&mut new_expr.left, &mut new_expr.right);
                    Some(Expr::Binary(new_expr))
                } else {
                    None
                }
            }
            Expr::Path(path_expr) if path_expr.path.segments.len() == 1 => {
                let ident = &path_expr.path.segments[0].ident;
                let is_generic_param = generics.params.iter().any(|p| match p {
                    GenericParam::Const(c) => c.ident == *ident,
                    GenericParam::Type(t) => t.ident == *ident,
                    GenericParam::Lifetime(l) => l.lifetime.ident == *ident,
                });
                if is_generic_param {
                    let block_expr: Expr = parse_quote!({ #ident });
                    Some(block_expr)
                } else {
                    None
                }
            }
            Expr::Lit(lit_expr) => {
                match &lit_expr.lit {
                    Lit::Int(int_lit) => {
                        let value = int_lit.base10_parse::<u64>().ok()?;
                        let new_value = if rng.gen_bool(0.5) { value + 1 } else { value.saturating_sub(1) };
                        let new_lit = Lit::Int(LitInt::new(&new_value.to_string(), int_lit.span()));
                        Some(Expr::Lit(ExprLit { attrs: lit_expr.attrs.clone(), lit: new_lit }))
                    }
                    Lit::Bool(bool_lit) => {
                        let new_value = !bool_lit.value;
                        let new_lit = Lit::Bool(LitBool::new(new_value, bool_lit.span()));
                        Some(Expr::Lit(ExprLit { attrs: lit_expr.attrs.clone(), lit: new_lit }))
                    }
                    Lit::Char(char_lit) => {
                        let c = char_lit.value();
                        let new_c = if c == 'a' { 'b' } else { 'a' };
                        let new_lit = Lit::Char(LitChar::new(new_c, char_lit.span()));
                        Some(Expr::Lit(ExprLit { attrs: lit_expr.attrs.clone(), lit: new_lit }))
                    }
                    _ => None,
                }
            }
            Expr::Block(block_expr) if block_expr.block.stmts.len() ==,1 => {
                if let Stmt::Expr(inner_expr, _) = &block_expr.block.stmts[0] {
                    if let Expr::Path(path_expr) = &**inner_expr {
                        if path_expr.path.segments.len() == 1 {
                            let ident = &path_expr.path.segments[0].ident;
                            let is_generic_param = generics.params.iter().any(|p| match p {
                                GenericParam::Const(c) => c.ident == *ident,
                                GenericParam::Type(t) => t.ident == *ident,
                                GenericParam::Lifetime(l) => l.lifetime.ident == *ident,
                            });
                            if is_generic_param {
                                let path_expr: Expr = parse_quote!(#ident);
                                Some(path_expr)
                            } else {
                                None
                            }
                        } else {
                            None
                        }
                    } else {
                        None
                    }
                } else {
                    None
                }
            }
            _ => None,
        }
    }
}