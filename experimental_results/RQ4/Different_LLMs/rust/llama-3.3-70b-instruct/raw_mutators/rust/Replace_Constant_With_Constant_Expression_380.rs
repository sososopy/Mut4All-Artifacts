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

pub struct Replace_Constant_With_Constant_Expression_380;

impl Mutator for Replace_Constant_With_Constant_Expression_380 {
    fn name(&self) -> &str {
        "Replace_Constant_With_Constant_Expression_380"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(const_item) = item {
                if let Expr::Lit(lit) = &*const_item.expr {
                    if let Lit::Int(int) = &lit.lit {
                        let new_value = thread_rng().gen_range(1..100);
                        const_item.expr = parse_quote! {
                            #new_value + #int.base10_digits()
                        };
                    }
                }
            }
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Lit(lit) = &**expr.expr {
                            if let Lit::Int(int) = &lit.lit {
                                let new_value = thread_rng().gen_range(1..100);
                                expr.expr = parse_quote! {
                                    #new_value + #int.base10_digits()
                                };
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constant expressions in the code with different constant expressions to test how the Rust compiler handles various constant values and expressions."
    }
}