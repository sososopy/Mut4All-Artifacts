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

pub struct Append_Zero_To_Negated_Literal_200;

impl Mutator for Append_Zero_To_Negated_Literal_200 {
    fn name(&self) -> &str {
        "Append_Zero_To_Negated_Literal_200"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = AppendZeroVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets unary negation operations applied to numeric literals. It modifies the literal by appending a zero digit to its representation, effectively scaling its value by the base. For decimal literals, append a '0'. For hexadecimal literals (starting with '0x'), append a '0' after the existing hex digits. For binary literals (starting with '0b'), append a '0' after the existing binary digits. For octal literals (starting with '0o'), append a '0' after the existing octal digits. This change can create literals that exceed the default inferred integer type (e.g., i32/i64) range, potentially triggering overflow checks during constant evaluation or linting phases."
    }
}

struct AppendZeroVisitor;

impl VisitMut for AppendZeroVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Unary(syn::ExprUnary { op: syn::UnOp::Neg(_), expr: inner_expr, .. }) = expr {
            if let Expr::Lit(syn::ExprLit { lit: syn::Lit::Int(int_lit), .. }) = &**inner_expr {
                let token = int_lit.token();
                let mut new_token = token.to_string();
                
                if new_token.starts_with("0x") {
                    new_token.push('0');
                } else if new_token.starts_with("0b") {
                    new_token.push('0');
                } else if new_token.starts_with("0o") {
                    new_token.push('0');
                } else {
                    new_token.push('0');
                }
                
                let new_lit = syn::LitInt::new(&new_token, int_lit.span());
                **inner_expr = Expr::Lit(syn::ExprLit {
                    attrs: Vec::new(),
                    lit: syn::Lit::Int(new_lit),
                });
            }
        }
        
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}