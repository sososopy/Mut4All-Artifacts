use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    parse_quote,
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

pub struct Replace_Enum_With_ImplTrait_483;

impl Mutator for Replace_Enum_With_ImplTrait_483 {
    fn name(&self) -> &str {
        "Replace_Enum_With_ImplTrait_483"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for (i, input) in func.sig.inputs.iter_mut().enumerate() {
                    if let FnArg::Typed(pat_type) = input {
                        if let Pat::Ident(pat_ident) = &*pat_type.pat {
                            let param_name = &pat_ident.ident;
                            if let Type::Path(_) = pat_type.ty.as_ref() {
                                let mut found_match = false;
                                for stmt in &mut func.block.stmts {
                                    if let Stmt::Expr(expr, _) = stmt {
                                        if let Expr::Closure(closure) = expr {
                                            if let Some(body) = closure.body.as_ref() {
                                                if let Expr::Match(match_expr) = body {
                                                    if let Expr::Path(path_expr) = &*match_expr.expr {
                                                        if let Some(ident) = path_expr.path.get_ident() {
                                                            if ident == param_name {
                                                                found_match = true;
                                                                break;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                if found_match {
                                    let alias_name = Ident::new("T", Span::call_site());
                                    let alias = parse_quote! {
                                        type #alias_name = impl Copy;
                                    };
                                    let alias_stmt = Stmt::Item(Item::Type(alias));
                                    func.block.stmts.insert(0, alias_stmt);
                                    let new_var_name = Ident::new("foo", Span::call_site());
                                    let new_local_stmt = parse_quote! {
                                        let #new_var_name: #alias_name = #param_name;
                                    };
                                    func.block.stmts.insert(1, new_local_stmt);
                                    for stmt in &mut func.block.stmts {
                                        if let Stmt::Expr(expr, _) = stmt {
                                            if let Expr::Closure(closure) = expr {
                                                if let Some(body) = closure.body.as_mut() {
                                                    if let Expr::Match(match_expr) = body {
                                                        match_expr.expr = Box::new(Expr::Path(ExprPath {
                                                            attrs: vec![],
                                                            qself: None,
                                                            path: SynPath::from(new_var_name.clone()),
                                                        }));
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies function parameters of enum type used in match expressions. It introduces an opaque type alias with `impl Copy`, assigns the parameter to a new variable of this alias, and modifies the match to use the alias. This transformation stresses the compiler's handling of opaque types and pattern matching, potentially exposing issues in type inference and match resolution for aliased enum types."
    }
}