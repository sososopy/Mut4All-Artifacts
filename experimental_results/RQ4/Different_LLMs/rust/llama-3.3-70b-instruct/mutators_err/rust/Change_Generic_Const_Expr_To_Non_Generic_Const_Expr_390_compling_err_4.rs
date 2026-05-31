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

pub struct Change_Generic_Const_Expr_To_Non_Generic_Const_Expr_390;

impl Mutator for Change_Generic_Const_Expr_To_Non_Generic_Const_Expr_390 {
    fn name(&self) -> &str {
        "Change_Generic_Const_Expr_To_Non_Generic_Const_Expr_390"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                for attr in &mut struct_item.attrs {
                    if let Meta::List(list) = &attr.meta {
                        if list.path.is_ident("const") {
                            let mut tokens = list.tokens.clone();
                            for (i, token) in tokens.into_iter().enumerate() {
                                if let TokenTree::Ident(ident) = token {
                                    if ident.to_string() == "generic" {
                                        // Replace generic type with non-generic type
                                        let non_generic_type = LitInt::new(42.to_string(), Span::call_site());
                                        tokens[i] = TokenTree::Literal(non_generic_type);
                                    }
                                }
                            }
                            list.tokens = tokens;
                        }
                    }
                }
            } else if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Path(path) = expr {
                            if path.path.is_ident("generic") {
                                // Replace generic type with non-generic type
                                let non_generic_type = Expr::Lit(ExprLit {
                                    attrs: vec![],
                                    lit: Lit::Int(LitInt::new(42.to_string(), Span::call_site())),
                                });
                                *expr = non_generic_type;
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets constant expressions that use generic types and replaces them with non-generic constant expressions. This transformation aims to test the compiler's ability to handle generic constant expressions and ensure that it can correctly replace them with non-generic constant expressions."
    }
}