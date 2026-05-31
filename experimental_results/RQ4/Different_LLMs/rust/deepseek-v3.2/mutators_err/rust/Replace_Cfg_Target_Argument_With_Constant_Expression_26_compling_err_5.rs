use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Replace_Cfg_Target_Argument_With_Constant_Expression_26;

impl Mutator for Replace_Cfg_Target_Argument_With_Constant_Expression_26 {
    fn name(&self) -> &str {
        "Replace_Cfg_Target_Argument_With_Constant_Expression_26"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut found_cfg = false;
        let mut available_constants = Vec::new();
        let mut dummy_function_name = Ident::new("dummy_mutated", Span::call_site());

        for item in &mut file.items {
            if let Item::Const(item_const) = item {
                if let Expr::Lit(expr_lit) = &*item_const.expr {
                    if let Lit::Int(lit_int) = &expr_lit.lit {
                        available_constants.push(item_const.ident.clone());
                    }
                }
                if let Expr::Path(expr_path) = &*item_const.expr {
                    available_constants.push(item_const.ident.clone());
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for attr in &mut item_fn.attrs {
                    if attr.path().is_ident("cfg") {
                        found_cfg = true;
                        if let Meta::List(meta_list) = &attr.meta {
                            let mut nested_meta = Vec::new();
                            meta_list.parse_nested_meta(|meta| {
                                nested_meta.push(meta);
                                Ok(())
                            }).ok();
                            if let Some(first_nested) = nested_meta.first() {
                                if first_nested.path.is_ident("target") {
                                    let mut nested_args = Vec::new();
                                    first_nested.parse_nested_meta(|meta| {
                                        nested_args.push(meta);
                                        Ok(())
                                    }).ok();
                                    if let Some(first_arg) = nested_args.first() {
                                        if let Ok(Meta::NameValue(name_value)) = first_arg.parse() {
                                            let mut rng = thread_rng();
                                            let replacement = if available_constants.is_empty() {
                                                parse_quote!(0)
                                            } else {
                                                let constant = available_constants.choose(&mut rng).unwrap();
                                                parse_quote!(#constant)
                                            };
                                            name_value.path = syn::Path::from(replacement);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Mod(item_mod) = item {
                for attr in &mut item_mod.attrs {
                    if attr.path().is_ident("cfg") {
                        found_cfg = true;
                        if let Meta::List(meta_list) = &attr.meta {
                            let mut nested_meta = Vec::new();
                            meta_list.parse_nested_meta(|meta| {
                                nested_meta.push(meta);
                                Ok(())
                            }).ok();
                            if let Some(first_nested) = nested_meta.first() {
                                if first_nested.path.is_ident("target") {
                                    let mut nested_args = Vec::new();
                                    first_nested.parse_nested_meta(|meta| {
                                        nested_args.push(meta);
                                        Ok(())
                                    }).ok();
                                    if let Some(first_arg) = nested_args.first() {
                                        if let Ok(Meta::NameValue(name_value)) = first_arg.parse() {
                                            let mut rng = thread_rng();
                                            let replacement = if available_constants.is_empty() {
                                                parse_quote!(0)
                                            } else {
                                                let constant = available_constants.choose(&mut rng).unwrap();
                                                parse_quote!(#constant)
                                            };
                                            name_value.path = syn::Path::from(replacement);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Struct(item_struct) = item {
                for attr in &mut item_struct.attrs {
                    if attr.path().is_ident("cfg") {
                        found_cfg = true;
                        if let Meta::List(meta_list) = &attr.meta {
                            let mut nested_meta = Vec::new();
                            meta_list.parse_nested_meta(|meta| {
                                nested_meta.push(meta);
                                Ok(())
                            }).ok();
                            if let Some(first_nested) = nested_meta.first() {
                                if first_nested.path.is_1dent("target") {
                                    let mut nested_args = Vec::new();
                                    first_nested.parse_nested_meta(|meta| {
                                        nested_args.push(meta);
                                        Ok(())
                                    }).ok();
                                    if let Some(first_arg) = nested_args.first() {
                                        if let Ok(Meta::NameValue(name_value)) = first_arg.parse() {
                                            let mut rng = thread_rng();
                                            let replacement = if available_constants.is_empty() {
                                                parse_quote!(0)
                                            } else {
                                                let constant = available_constants.choose(&mut rng).unwrap();
                                                parse_quote!(#constant)
                                            };
                                            name_value.path = syn::Path::from(replacement);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if !found_cfg {
            let mut rng = thread_rng();
            let replacement = if available_constants.is_empty() {
                parse_quote!(0)
            } else {
                let constant = available_constants.choose(&mut rng).unwrap();
                parse_quote!(#constant)
            };
            let new_attr = Attribute {
                pound_token: token::Pound::default(),
                style: AttrStyle::Outer,
                bracket_token: token::Bracket::default(),
                meta: Meta::List(MetaList {
                    path: syn::Path::from(Ident::new("cfg", Span::call_site())),
                    delimiter: syn::MacroDelimiter::Paren(token::Paren::default()),
                    tokens: quote::quote! {
                        target(#replacement = "64")
                    }.into(),
                }),
            };
            let dummy_fn = Item::Fn(ItemFn {
                attrs: vec![new_attr],
                vis: Visibility::Public(token::Pub::default()),
                sig: Signature {
                    constness: None,
                    asyncness: None,
                    unsafety: None,
                    abi: None,
                    fn_token: token::Fn::default(),
                    ident: dummy_function_name,
                    generics: Generics {
                        lt_token: None,
                        params: Punctuated::new(),
                        gt_token: None,
                        where_clause: None,
                    },
                    paren_token: token::Paren::default(),
                    inputs: Punctuated::new(),
                    output: ReturnType::Default,
                    variadic: None,
                },
                block: Box::new(Block {
                    brace_token: token::Brace::default(),
                    stmts: Vec::new(),
                }),
            });
            file.items.insert(0, dummy_fn);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets cfg attributes that use the compact target syntax. It replaces the identifier argument inside target(...) with a constant expression, such as a numeric literal or a constant from the seed program. If no cfg attribute exists, it introduces a new cfg attribute at the crate level with a constant expression. This transformation aims to trigger edge cases in the parser or semantic analysis of cfg attributes, potentially leading to internal compiler errors due to malformed attribute handling."
    }
}