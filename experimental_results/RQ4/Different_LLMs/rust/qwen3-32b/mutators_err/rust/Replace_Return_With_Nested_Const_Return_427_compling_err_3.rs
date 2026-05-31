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

pub struct Replace_Return_With_Nested_Const_Return_427;

impl Mutator for Replace_Return_With_Nested_Const_Return_427 {
    fn name(&self) -> &str {
        "Replace_Return_With_Nested_Const_Return_427"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, None) = stmt {
                        if let Expr::Return(ret) = &*expr {
                            if let Some(expr_box) = &ret.expr {
                                if let Expr::Const(const_expr) = &**expr_box {
                                    let new_const_body = parse_quote! {{
                                        { return; }
                                    }};
                                    let new_const_expr = Expr::Const(ExprConst {
                                        const_token: const_expr.const_token.clone(),
                                        body: Box::new(ExprBlock {
                                            attrs: Vec::new(),
                                            label: None,
                                            block: new_const_body,
                                        }),
                                    });
                                    let new_return_expr = Expr::Return(ExprReturn {
                                        attrs: Vec::new(),
                                        return_token: ret.return_token.clone(),
                                        expr: Some(Box::new(new_const_expr)),
                                    });
                                    *stmt = Stmt::Expr(new_return_expr, None);
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