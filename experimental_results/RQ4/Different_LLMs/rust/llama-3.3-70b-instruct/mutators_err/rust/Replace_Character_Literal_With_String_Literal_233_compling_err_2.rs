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

pub struct Replace_Character_Literal_With_String_Literal_233;

impl Mutator for Replace_Character_Literal_With_String_Literal_233 {
    fn name(&self) -> &str {
        "Replace_Character_Literal_With_String_Literal_233"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr) = stmt {
                        if let Expr::Lit(lit) = expr {
                            if let Lit::Char(c) = &lit.lit {
                                let string_literal = Lit::Str(LitStr::new(&c.to_string(), lit.span()));
                                *expr = parse_quote! { #string_literal };
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for stmt in &mut func.block.stmts {
                            if let Stmt::Expr(expr) = stmt {
                                if let Expr::Lit(lit) = expr {
                                    if let Lit::Char(c) = &lit.lit {
                                        let string_literal = Lit::Str(LitStr::new(&c.to_string(), lit.span()));
                                        *expr = parse_quote! { #string_literal };
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
        "The mutation operator replaces character literals with string literals of the same character. This transformation tests the compiler's ability to handle Unicode and generic types, potentially triggering type mismatches or inference failures."
    }
}