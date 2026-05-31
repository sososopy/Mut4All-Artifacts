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

pub struct Modify_Iterator_With_Const_Generics_35;

impl Mutator for Modify_Iterator_With_Const_Generics_35 {
    fn name(&self) -> &str {
        "Modify_Iterator_With_Const_Generics_35"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut has_iterator = false;
                        for bound in &type_impl_trait.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.path.segments.last().unwrap().ident == "Iterator" {
                                    has_iterator = true;
                                    break;
                                }
                            }
                        }
                        if has_iterator {
                            let mut new_block = func.block.clone();
                            let const_ident = Ident::new("HashesEntryLEN", Span::call_site());
                            let size_expr: Expr = parse_quote! { #const_ident * 2 };
                            let take_expr: Expr = parse_quote! { .take(#size_expr) };
                            for stmt in &mut new_block.stmts {
                                if let Stmt::Expr(expr) = stmt {
                                    if let Expr::MethodCall(method_call) = expr {
                                        if method_call.method == "iter_mut" {
                                            method_call.args.push(take_expr.clone());
                                        }
                                    }
                                }
                            }
                            func.block = new_block;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies methods returning `impl Iterator` by incorporating arithmetic expressions using generic constants into iterator operations. This tests the compiler's handling of const generics in iterator contexts and aims to expose potential issues in the interaction between const generics and iterator logic."
    }
}