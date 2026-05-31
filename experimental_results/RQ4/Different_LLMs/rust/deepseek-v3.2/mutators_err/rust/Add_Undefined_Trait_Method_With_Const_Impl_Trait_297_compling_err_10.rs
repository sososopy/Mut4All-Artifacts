use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Local, Pat, PatType, Path as SynPath,
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

pub struct Add_Undefined_Trait_Method_With_Const_Impl_Trait_297;

impl Mutator for Add_Undefined_Trait_Method_With_Const_Impl_Trait_297 {
    fn name(&self) -> &str {
        "Add_Undefined_Trait_Method_With_Const_Impl_Trait_297"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let is_const = item_impl.defaultness.is_some();
                let existing_method_names: Vec<String> = item_impl.items.iter()
                    .filter_map(|impl_item| match impl_item {
                        syn::ImplItem::Fn(func) => Some(func.sig.ident.to_string()),
                        _ => None,
                    })
                    .collect();
                if existing_method_names.is_empty() {
                    continue;
                }
                let base_name = existing_method_names.choose(&mut rng).unwrap();
                let suffix = rng.gen_range(1..10);
                let new_method_name = format!("{}{}", base_name, suffix);
                let new_method_ident = Ident::new(&new_method_name, Span::call_site());
                let const_token = if is_const { Some(token::Const::default()) } else { None };
                let mut bounds = Punctuated::new();
                bounds.push(TypeParamBound::Trait(TraitBound {
                    paren_token: None,
                    modifier: TraitBoundModifier::None,
                    lifetimes: None,
                    path: SynPath {
                        leading_colon: None,
                        segments: {
                            let mut segs = Punctuated::new();
                            segs.push(syn::PathSegment {
                                ident: Ident::new("PartialEq", Span::call_site()),
                                arguments: PathArguments::None,
                            });
                            segs
                        },
                    },
                }));
                bounds.push(TypeParamBound::Trait(TraitBound {
                    paren_token: None,
                    modifier: TraitBoundModifier::None,
                    lifetimes: None,
                    path: SynPath {
                        leading_colon: None,
                        segments: {
                            let mut segs = Punctuated::new();
                            segs.push(syn::PathSegment {
                                ident: Ident::new("Destruct", Span::call_site()),
                                arguments: PathArguments::None,
                            });
                            segs
                        },
                    },
                }));
                let return_type = TypeImplTrait {
                    impl_token: token::Impl { span: Span::call_site() },
                    bounds,
                };
                let new_method = syn::ImplItem::Fn(syn::ImplItemFn {
                    attrs: Vec::new(),
                    vis: syn::Visibility::Inherited,
                    defaultness: None,
                    sig: syn::Signature {
                        constness: const_token,
                        asyncness: None,
                        unsafety: None,
                        abi: None,
                        fn_token: token::Fn { span: Span::call_site() },
                        ident: new_method_ident,
                        generics: syn::Generics {
                            lt_token: None,
                            params: Punctuated::new(),
                            gt_token: None,
                            where_clause: None,
                        },
                        paren_token: token::Paren::default(),
                        inputs: Punctuated::new(),
                        output: ReturnType::Type(Default::default(), Box::new(Type::ImplTrait(return_type))),
                        variadic: None,
                    },
                    block: syn::Block {
                        brace_token: token::Brace::default(),
                        stmts: vec![Stmt::Expr(
                            Expr::Lit(syn::ExprLit {
                                attrs: Vec::new(),
                                lit: syn::Lit::Int(syn::LitInt::new("123", Span::call_site())),
                            }),
                            None,
                        )],
                    },
                });
                item_impl.items.push(new_method);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds a new method to trait impl blocks that is not defined in the trait. The method name is derived by appending a numeric suffix to an existing method name in the same impl block. The return type is an impl trait with const bounds (PartialEq and Destruct) and returns a literal integer. If the impl block is const, the new method is also marked const. This transformation aims to trigger query cycles and deadlocks in the compiler's const checking and trait solving logic, particularly when combined with associated type bounds and effect syntax."
    }
}