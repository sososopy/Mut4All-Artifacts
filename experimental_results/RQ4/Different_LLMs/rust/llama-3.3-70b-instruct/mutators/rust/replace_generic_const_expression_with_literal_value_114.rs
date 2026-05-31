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

pub struct Replace_Generic_Const_Expression_With_Literal_Value_114;

impl Mutator for Replace_Generic_Const_Expression_With_Literal_Value_114 {
    fn name(&self) -> &str {
        "Replace_Generic_Const_Expression_With_Literal_Value_114"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        for statement in &mut func.block.stmts {
                            if let Stmt::Expr(expr, _) = statement {
                                if let Expr::Call(ExprCall { args, .. }) = expr {
                                    let args_clone = args.clone();
                                    for (index, arg) in args_clone.iter().enumerate() {
                                        if let Expr::Path(ExprPath { path, .. }) = arg {
                                            if path.segments.len() > 0 {
                                                let ident = &path.segments[0].ident;
                                                if ident == "min" || ident == "max" {
                                                    let literal_value = thread_rng().gen_range(0..10);
                                                    let new_arg = parse_quote! { #literal_value };
                                                    args[index] = new_arg;
                                                }
                                            }
                                        }
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
        "The mutation operator replaces generic const expressions with literal values. It targets const fn definitions, trait implementations, and struct definitions that use generic const expressions. By replacing these expressions with literal values, it tests the compiler's ability to handle const generics and may lead to bugs related to generic const expressions."
    }
}