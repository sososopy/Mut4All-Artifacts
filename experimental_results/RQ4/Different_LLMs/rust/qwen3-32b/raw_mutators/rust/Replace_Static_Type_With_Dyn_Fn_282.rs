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

pub struct Replace_Static_Type_With_Dyn_Fn_282;

impl Mutator for Replace_Static_Type_With_Dyn_Fn_282 {
    fn name(&self) -> &str {
        "Replace_Static_Type_With_Dyn_Fn_282"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Static(item_static) = item {
                // Skip if already a function-like type or trait object
                if let Type::TraitObject(_) = &*item_static.ty {
                    continue;
                }

                // Attempt to infer a function-like signature from the expression
                let mut return_type = parse_quote!(u32);
                let mut args = vec![];

                if let Expr::Closure(closure) = &*item_static.expr {
                    if let Some(body) = closure.body.as_ref().and_then(|b| b.as_expr()) {
                        if let Expr::Lit(lit) = body {
                            if let syn::Lit::Int(_) = &lit.lit {
                                return_type = parse_quote!(u32);
                            } else if let syn::Lit::Str(_) = &lit.lit {
                                return_type = parse_quote!(&'static str);
                            }
                        }
                    }
                }

                // Build a dyn Fn(...) -> T trait object
                let mut bounds = Punctuated::new();
                let fn_bound = TraitBound {
                    paren_token: None,
                    modifier: TraitBoundModifier::Dyn,
                    lifetimes: None,
                    path: SynPath {
                        leading_colon: None,
                        segments: {
                            let mut segs = Punctuated::new();
                            segs.push(syn::PathSegment {
                                ident: Ident::new("Fn", Span::call_site()),
                                arguments: PathArguments::AngleBracketed(
                                    parse_quote!(<#(#args),* -> #return_type>)
                                ),
                            });
                            segs
                        },
                    },
                };
                bounds.push(syn::TypeParamBound::Trait(fn_bound));

                let new_type = Type::TraitObject(TypeTraitObject {
                    dyn_token: Some(token::Dyn::default()),
                    bounds,
                    ..Default::default()
                });

                item_static.ty = Box::new(new_type);

                // Replace the static value with a closure expression
                let closure = parse_quote!(|| 0);
                item_static.expr = Box::new(closure);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}