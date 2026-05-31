use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Modify_Closure_Return_Type_27;

impl Mutator for Modify_Closure_Return_Type_27 {
    fn name(&self) -> &str {
        "Modify_Closure_Return_Type_27"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        for stmt in &mut item_fn.block.stmts {
                            if let Stmt::Expr(Expr::Closure(closure), _) = stmt {
                                if let Expr::Lit(expr_lit) = &*closure.body {
                                    if let syn::Lit::Int(_) = expr_lit.lit {
                                        closure.body = Box::new(Expr::Lit(parse_quote!(1.0)));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets closures within functions returning `impl Trait`. It modifies the return type of the closure from an integer literal to a floating-point literal. This transformation tests the compiler's handling of type inference and trait specialization by introducing a structurally different but trait-compatible return type."
    }
}