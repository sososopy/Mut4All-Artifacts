use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Mutator_MIR_NestedProjection_220;

impl Mutator for Mutator_MIR_NestedProjection_220 {
    fn name(&self) -> &str {
        "Mutator_MIR_NestedProjection_220"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, return_type) = &func.sig.output {
                    if let syn::Type::Path(type_path) = &**return_type {
                        if let Some(segment) = type_path.path.segments.iter().find(|s| s.ident == "Result") {
                            let mut insertions = Vec::new();
                            for (index, stmt) in func.block.stmts.iter_mut().enumerate() {
                                if let syn::Stmt::Expr(expr, None) = stmt {
                                    if let syn::Expr::Call(expr_call) = &*expr {
                                        if let syn::Expr::Path(path_expr) = &*expr_call.func {
                                            if path_expr.path.is_ident("Ok") {
                                                let arg = expr_call.args[0].clone();
                                                let new_statements = (0..3).map(|_| {
                                                    let inner_ok: syn::Expr = parse_quote!(Ok(#arg));
                                                    let outer_ok: syn::Expr = parse_quote!(Ok(#inner_ok));
                                                    let question_expr: syn::Expr = parse_quote!(#outer_ok?);
                                                    syn::Stmt::Expr(question_expr, None)
                                                }).collect::<Vec<_>>();
                                                insertions.push((index, new_statements));
                                            }
                                        }
                                    }
                                }
                            }
                            for (index, new_stmts) in insertions.into_iter().rev() {
                                for stmt in new_stmts {
                                    func.block.stmts.insert(index, stmt);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions returning Result<T, E> with Ok(...) return statements. It inserts multiple nested Ok(...) expressions followed by ? operators, increasing Result nesting depth and forcing MIR projection through multiple layers. This stresses the compiler's optimization passes by creating complex nested projections that may trigger assertion failures or incorrect layout handling during MIR processing."
    }
}