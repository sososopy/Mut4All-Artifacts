use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemImpl, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Modify_Const_Generic_Arithmetic_50;

impl Mutator for Modify_Const_Generic_Arithmetic_50 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Arithmetic_50"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(ItemImpl {
                generics,
                items,
                ..
            }) = item
            {
                for generic in &generics.params {
                    if let GenericParam::Const(const_param) = generic {
                        let const_ident = &const_param.ident;
                        for impl_item in items {
                            if let syn::ImplItem::Fn(method) = impl_item {
                                if let ReturnType::Type(_, return_type) = &mut method.sig.output {
                                    if let Type::Path(TypePath { path, .. }) = return_type.as_ref() {
                                        if let Some(last_segment) = path.segments.last() {
                                            if let PathArguments::AngleBracketed(args) = &last_segment.arguments {
                                                let mut new_args: Punctuated<GenericArgument, Comma> = Punctuated::new();
                                                for generic_arg in &args.args {
                                                    if let GenericArgument::Const(Expr::Lit(expr_lit)) = generic_arg {
                                                        if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                                            let new_expr: Expr = parse_quote! { #const_ident * 2 + 1 };
                                                            new_args.push(GenericArgument::Const(new_expr));
                                                        }
                                                    }
                                                }
                                                let new_path: TypePath = parse_quote! {
                                                    Combination<{ #const_ident * 2 + 1 }>
                                                };
                                                *return_type = Box::new(Type::Path(new_path));
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
        "This mutation operator targets function implementations with constant generic parameters. It modifies the function's return type by introducing a complex arithmetic operation involving the constant generic parameter, such as multiplying by 2 and adding 1. This challenges the compiler's constant evaluation logic by requiring it to handle more intricate expressions during type checking."
    }
}