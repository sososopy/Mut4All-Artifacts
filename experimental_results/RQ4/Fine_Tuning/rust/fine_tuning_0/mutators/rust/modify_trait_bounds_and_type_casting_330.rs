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

pub struct Modify_Trait_Bounds_And_Type_Casting_330;

impl Mutator for Modify_Trait_Bounds_And_Type_Casting_330 {
    fn name(&self) -> &str {
        "Modify_Trait_Bounds_And_Type_Casting_330"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                
                // Modify trait bounds in function signature
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::ImplTrait(type_impl_trait) = &mut *pat_type.ty {
                            type_impl_trait.bounds.push(parse_quote!(Send));
                        }
                    }
                }
                
                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        type_impl_trait.bounds.push(parse_quote!(Send));
                    }
                }
                
                // Modify type casting inside function body
                let mut new_stmts = Vec::new();
                for stmt in &func.block.stmts {
                    if let Stmt::Expr(Expr::Cast(expr_cast), _) = stmt {
                        let casted_expr: Expr = parse_quote! {
                            {
                                let casted: &dyn Send = &expr_cast.expr;
                                casted as _
                            }
                        };
                        new_stmts.push(Stmt::Expr(casted_expr, None));
                    } else {
                        new_stmts.push(stmt.clone());
                    }
                }
                func.block.stmts = new_stmts;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies function signatures by adding the `Send` trait to existing trait bounds and alters type casting within function bodies to use `&dyn Send`. This transformation aims to stress test the Rust compiler's handling of trait bounds and type casting, potentially uncovering bugs related to trait object interactions and trait resolution."
    }
}