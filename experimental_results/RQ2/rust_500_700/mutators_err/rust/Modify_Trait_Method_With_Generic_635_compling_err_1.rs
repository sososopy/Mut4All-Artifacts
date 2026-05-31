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

pub struct Modify_Trait_Method_With_Generic_635;

impl Mutator for Modify_Trait_Method_With_Generic_635 {
    fn name(&self) -> &str {
        "Modify_Trait_Method_With_Generic_635"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for trait_item in &mut item_trait.items {
                    if let syn::TraitItem::Method(method) = trait_item {
                        if method.sig.inputs.iter().any(|arg| matches!(arg, FnArg::Receiver(_))) {
                            let generic_param: GenericParam = parse_quote!(T);
                            method.sig.generics.params.push(generic_param);

                            for input in &mut method.sig.inputs {
                                if let FnArg::Receiver(receiver) = input {
                                    let new_arg: FnArg = parse_quote! { extra: T };
                                    method.sig.inputs.push(new_arg);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Method(method) = impl_item {
                        if method.sig.inputs.iter().any(|arg| matches!(arg, FnArg::Receiver(_))) {
                            let generic_param: GenericParam = parse_quote!(T);
                            method.sig.generics.params.push(generic_param);

                            for input in &mut method.sig.inputs {
                                if let FnArg::Receiver(receiver) = input {
                                    let new_arg: FnArg = parse_quote! { extra: T };
                                    method.sig.inputs.push(new_arg);
                                    break;
                                }
                            }

                            for stmt in &mut method.block.stmts {
                                if let Stmt::Expr(Expr::Call(expr_call)) = stmt {
                                    if let Expr::Path(ExprPath { path, .. }) = &*expr_call.func {
                                        if path.segments.len() == 1 && path.segments[0].ident == "self" {
                                            expr_call.args.push(parse_quote!(extra));
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
        ""
    }
}