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

pub struct Modify_Generator_To_Use_Undefined_Variable_116;

impl Mutator for Modify_Generator_To_Use_Undefined_Variable_116 {
    fn name(&self) -> &str {
        "Modify_Generator_To_Use_Undefined_Variable_116"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut found_yield = false;
                for stmt in &func.block.stmts {
                    if let syn::Stmt::Semi(expr, _) = stmt {
                        if let syn::Expr::Yield(_) = expr {
                            found_yield = true;
                            break;
                        }
                    }
                }
                if found_yield {
                    let mut found_assignment = false;
                    for stmt in &mut func.block.stmts {
                        if let syn::Stmt::Local(local) = stmt {
                            if let Some((_, init_expr)) = &mut local.init {
                                if !found_assignment {
                                    *init_expr = parse_quote!(undefined_var);
                                    found_assignment = true;
                                }
                            }
                        }
                        if let syn::Stmt::Semi(expr, _) = stmt {
                            if let syn::Expr::Assign(assign_expr) = expr {
                                if !found_assignment {
                                    *assign_expr.right = parse_quote!(undefined_var);
                                    found_assignment = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets generator functions by identifying the presence of a `yield` expression. It then modifies the first assignment within the generator's body to reference an undefined variable, `undefined_var`. This transformation is intended to induce compilation errors or ICEs by exploiting the compiler's handling of unresolved variable references within the unique context of generator functions."
    }
}