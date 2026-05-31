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

pub struct Replace_Concrete_Return_With_ImplTrait_Lifetimes_Recursive_195;

impl Mutator for Replace_Concrete_Return_With_ImplTrait_Lifetimes_Recursive_195 {
    fn name(&self) -> &str {
        "Replace_Concrete_Return_With_ImplTrait_Lifetimes_Recursive_195"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let syn::ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let syn::Type::ImplTrait(_) = **return_type {
                        continue;
                    }
                    let a_lifetime = syn::Lifetime::new("'a", proc_macro2::Span::call_site());
                    let b_lifetime = syn::Lifetime::new("'b", proc_macro2::Span::call_site());
                    let a_param = syn::LifetimeParam {
                        attrs: vec![],
                        lifetime: a_lifetime.clone(),
                        colon_token: Some(Default::default()),
                        bounds: syn::punctuated::Punctuated::from_iter(vec![a_lifetime.clone()]),
                    };
                    let b_param = syn::LifetimeParam {
                        attrs: vec![],
                        lifetime: b_lifetime.clone(),
                        colon_token: Some(Default::default()),
                        bounds: syn::punctuated::Punctuated::from_iter(vec![a_lifetime.clone()]),
                    };
                    func.sig.generics.params.push(syn::GenericParam::Lifetime(a_param));
                    func.sig.generics.params.push(syn::GenericParam::Lifetime(b_param));
                    let trait_path = parse_quote!(Trait<'a>);
                    let mut bounds = syn::punctuated::Punctuated::new();
                    bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                        paren_token: None,
                        modifier: syn::TraitBoundModifier::None,
                        lifetimes: None,
                        path: trait_path,
                    }));
                    *return_type = Box::new(syn::Type::ImplTrait(syn::TypeImplTrait {
                        impl_token: token::Impl {
                            span: proc_macro2::Span::call_site(),
                        },
                        bounds,
                    }));
                    let block = &mut func.block;
                    let func_name = &func.sig.ident;
                    block.stmts.clear();
                    let ty = Type::Reference(syn::TypeReference {
                        and_token: Default::default(),
                        mutability: syn::token::Mut::default(),
                        lifetime: Some(a_lifetime.clone()),
                        elem: Box::new(syn::Type::Tuple(syn::TypeTuple {
                            paren_token: Default::default(),
                            elems: Punctuated::new(),
                        })),
                    });
                    let expr = Expr::Path(ExprPath {
                        attrs: vec![],
                        qself: None,
                        path: {
                            let mut path = syn::Path::from(func_name.clone());
                            let args = syn::AngleBracketedGenericArguments {
                                colon2_token: None,
                                lt_token: Default::default(),
                                args: {
                                    let mut args = Punctuated::new();
                                    args.push(GenericArgument::Lifetime(a_lifetime.clone()));
                                    args.push(GenericArgument::Lifetime(a_lifetime.clone()));
                                    args
                                },
                                gt_token: Default::default(),
                            };
                            path.segments.last_mut().unwrap().arguments = PathArguments::AngleBracketed(args);
                            path
                        },
                    });
                    let pat = Pat::Type(PatType {
                        attrs: vec![],
                        colon_token: Default::default(),
                        ty: Box::new(ty),
                        pat: Box::new(Pat::Wild(syn::PatWild {
                            attrs: vec![],
                            underscore_token: Default::default(),
                        })),
                    });
                    let local = Local {
                        attrs: vec![],
                        let_token: Default::default(),
                        pat,
                        init: Some(LocalInit {
                            eq_token: syn::token::Eq::default(),
                            expr: Box::new(expr),
                            diverge: None,
                        }),
                        semi_token: Default::default(),
                    };
                    block.stmts.push(Stmt::Local(local));
                    block.stmts.push(Stmt::Expr(parse_quote! { loop {} }, None));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces concrete return types with `impl Trait` having multiple lifetimes, adds a recursive call inside the function, and assigns the result to a reference with a specific lifetime. This transformation introduces complex lifetime relationships and opaque return types, potentially exposing bugs in region inference and opaque type processing during compilation."
    }
}