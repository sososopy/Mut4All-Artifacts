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

pub struct Recursive_Trait_Tuple_Return_269;

struct FunctionReferenceChecker<'a> {
    target_ident: &'a Ident,
    found: bool,
}

impl<'a> Visit<'_> for FunctionReferenceChecker<'a> {
    fn visit_expr_path(&mut self, expr_path: &ExprPath) {
        if expr_path.path.is_ident(self.target_ident) {
            self.found = true;
        }
    }
}

impl Mutator for Recursive_Trait_Tuple_Return_269 {
    fn name(&self) -> &str {
        "Recursive_Trait_Tuple_Return_269"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::ImplTrait(_) = **return_type {
                        let ident = &func.sig.ident;
                        let mut checker = FunctionReferenceChecker {
                            target_ident: ident,
                            found: false,
                        };
                        for stmt in &func.block.stmts {
                            checker.visit_stmt(stmt);
                        }
                        if checker.found {
                            let new_expr: Expr = parse_quote! { (#ident,) };
                            let new_block = parse_quote! { { #new_expr } };
                            func.block = Box::new(new_block);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions returning `impl Trait` that reference themselves in their bodies. It replaces the function's body with a tuple containing the function's name, creating a recursive dependency in the return type. This may trigger infinite trait resolution cycles or ICEs in the compiler's type checker by forcing the return type to depend on itself."
    }
}