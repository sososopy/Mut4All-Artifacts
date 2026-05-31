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

pub struct Replace_Generic_Const_With_Closure_Binder_363;

impl Mutator for Replace_Generic_Const_With_Closure_Binder_363 {
    fn name(&self) -> &str {
        "Replace_Generic_Const_With_Closure_Binder_363"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let has_lifetimes = func.sig.generics.params.iter().any(|param| matches!(param, GenericParam::Lifetime(_)));
                let has_consts = func.sig.generics.params.iter().any(|param| matches!(param, GenericParam::Const(_)));
                
                if has_lifetimes && has_consts {
                    let lifetimes: Vec<_> = func.sig.generics.params.iter()
                        .filter_map(|param| match param {
                            GenericParam::Lifetime(lt) => Some(lt.lifetime.clone()),
                            _ => None,
                        })
                        .collect();
                    
                    if let Some(lifetime) = lifetimes.first().cloned() {
                        let closure_expr = parse_quote! { |x: & #lifetime u32| x };
                        let expr_block = parse_quote! { { let _ = #closure_expr; 4 } };
                        
                        if let syn::ReturnType::Type(_, ref mut return_type) = func.sig.output {
                            *return_type = Box::new(syn::Type::Array(syn::TypeArray {
                                bracket_token: Default::default(),
                                elem: Box::new(syn::Type::Path(syn::TypePath {
                                    qself: None,
                                    path: syn::Path::from(Ident::new("u8", proc_macro2::Span::call_site())),
                                })),
                                len: Box::new(expr_block),
                            }));
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