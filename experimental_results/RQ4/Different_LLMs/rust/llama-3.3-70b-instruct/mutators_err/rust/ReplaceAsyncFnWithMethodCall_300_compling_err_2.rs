use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct ReplaceAsyncFnWithMethodCall_300;

impl Mutator for ReplaceAsyncFnWithMethodCall_300 {
    fn name(&self) -> &str {
        "ReplaceAsyncFnWithMethodCall_300"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                for method in &mut trait_item.items {
                    if let syn::TraitItem::Method(method) = method {
                        if method.sig.asyncness.is_some() {
                            let method_name = method.sig.ident.clone();
                            let method_helper_name = Ident::new(
                                &format!("{}_helper", method_name),
                                method_name.span(),
                            );

                            let method_helper = syn::TraitItem::Method(syn::Method {
                                sig: syn::Signature {
                                    constness: method.sig.constness,
                                    asyncness: method.sig.asyncness,
                                    unsafety: method.sig.unsafety,
                                    abi: method.sig.abi,
                                    fn_token: method.sig.fn_token,
                                    ident: method_helper_name,
                                    generics: method.sig.generics.clone(),
                                    paren_token: method.sig.paren_token,
                                    inputs: method.sig.inputs.clone(),
                                    output: method.sig.output.clone(),
                                    variadic: method.sig.variadic,
                                },
                                block: method.block.clone(),
                                attributes: method.attributes.clone(),
                                is_placeholder: false,
                            });

                            let new_method = syn::TraitItem::Method(syn::Method {
                                sig: syn::Signature {
                                    constness: method.sig.constness,
                                    asyncness: method.sig.asyncness,
                                    unsafety: method.sig.unsafety,
                                    abi: method.sig.abi,
                                    fn_token: method.sig.fn_token,
                                    ident: method.sig.ident,
                                    generics: method.sig.generics.clone(),
                                    paren_token: method.sig.paren_token,
                                    inputs: method.sig.inputs.clone(),
                                    output: method.sig.output.clone(),
                                    variadic: method.sig.variadic,
                                },
                                block: {
                                    let mut new_block = syn::Block {
                                        brace_token: method.block.brace_token,
                                        stmts: vec![],
                                    };

                                    let call = syn::Expr::Call(syn::ExprCall {
                                        attrs: vec![],
                                        func: syn::Expr::Path(syn::ExprPath {
                                            attrs: vec![],
                                            qself: None,
                                            path: syn::Path {
                                                leading_colon: None,
                                                segments: syn::punctuated::Punctuated::from_iter(
                                                    vec![syn::PathSegment {
                                                        ident: method_helper_name,
                                                        arguments: syn::PathArguments::None,
                                                    }],
                                                ),
                                            },
                                        }),
                                        paren_token: Paren { span: method.block.brace_token.span },
                                        args: Punctuated::new(),
                                    });

                                    new_block.stmts.push(syn::Stmt::Expr(call, None));
                                    new_block
                                },
                                attributes: method.attributes.clone(),
                                is_placeholder: false,
                            });

                            trait_item.items.push(method_helper);
                            *method = new_method;
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces async functions in traits with calls to new methods with the same name and signature, introducing a new method with the same name and signature, and then call this new method from the original function definition."
    }
}