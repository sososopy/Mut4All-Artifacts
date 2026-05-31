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

pub struct Modify_Generator_Yield_Points_118;

impl Mutator for Modify_Generator_Yield_Points_118 {
    fn name(&self) -> &str {
        "Modify_Generator_Yield_Points_118"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                if let Expr::Closure(closure) = &*item_static.expr {
                    if closure.body.stmts.iter().any(|stmt| {
                        if let Stmt::Expr(expr, _) = stmt {
                            matches!(expr, Expr::Yield(_))
                        } else {
                            false
                        }
                    }) {
                        let mut new_body = closure.body.clone();
                        let mut has_non_sized = false;
                        for stmt in &mut new_body.stmts {
                            if let Stmt::Expr(expr, _) = stmt {
                                if let Expr::Yield(yield_expr) = expr {
                                    if has_non_sized {
                                        *expr = parse_quote! { yield non_sized.len() };
                                    } else {
                                        *expr = parse_quote! { yield };
                                    }
                                }
                            }
                        }
                        if !has_non_sized {
                            new_body.stmts.insert(0, parse_quote! { let non_sized = *""; });
                        }
                        closure.body = new_body;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets generator functions by modifying their yield points. It transforms simple `yield;` statements into more complex expressions involving non-sized types, such as yielding the length of a string slice. This increases the likelihood of triggering compiler bugs related to generator state management and type layout."
    }
}