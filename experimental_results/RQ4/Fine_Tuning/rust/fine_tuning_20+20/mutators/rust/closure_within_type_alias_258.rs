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

pub struct Closure_Within_Type_Alias_258;

impl Mutator for Closure_Within_Type_Alias_258 {
    fn name(&self) -> &str {
        "Closure_Within_Type_Alias_258"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut alias_names = HashSet::new();
        for item in &file.items {
            if let Item::Type(item_type) = item {
                if let Type::ImplTrait(type_impl_trait) = &*item_type.ty {
                    alias_names.insert(item_type.ident.to_string());
                }
            }
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if alias_names.contains(&item_fn.sig.ident.to_string()) {
                    let mut has_closure = false;
                    for stmt in &item_fn.block.stmts {
                        if let Stmt::Expr(expr, None) = stmt {
                            if matches!(expr, Expr::Closure(_)) {
                                has_closure = true;
                            }
                        }
                    }
                    if has_closure {
                        let captured_var: Stmt = parse_quote! {
                            let captured_var = 10;
                        };
                        item_fn.block.stmts.insert(0, captured_var);
                        for stmt in &mut item_fn.block.stmts {
                            if let Stmt::Expr(expr, None) = stmt {
                                if let Expr::Closure(expr_closure) = expr {
                                    let new_closure: ExprClosure = parse_quote! {
                                        || { let _ = captured_var; #expr_closure }
                                    };
                                    *expr = Expr::Closure(new_closure);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets type aliases defined with `impl Trait` and specifically modifies functions that return these aliases by enhancing any contained closure to capture an additional variable from the surrounding scope, thereby increasing the complexity of type inference and testing the compiler's handling of closure captures within opaque type contexts."
    }
}