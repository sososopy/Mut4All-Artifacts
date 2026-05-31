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

pub struct Add_Unused_Lifetime_Bounds_349;

impl Mutator for Add_Unused_Lifetime_Bounds_349 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetime_Bounds_349"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let lifetime_param: LifetimeParam = parse_quote!('a);
                func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param.clone()));
                
                let lifetime_bound = TypeParamBound::Lifetime(Lifetime::new("'a", Span::call_site()));
                if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::Path(type_path) = &mut **ty {
                        if let Some(last_segment) = type_path.path.segments.last_mut() {
                            last_segment.arguments = PathArguments::AngleBracketed(
                                syn::AngleBracketedGenericArguments {
                                    colon2_token: None,
                                    lt_token: token::Lt::default(),
                                    args: Punctuated::from_iter(vec![
                                        GenericArgument::Lifetime(Lifetime::new("'a", Span::call_site()))
                                    ]),
                                    gt_token: token::Gt::default(),
                                }
                            );
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds an unused lifetime parameter 'a to function signatures and attempts to bind it to the return type. This can create lifetime mismatches or unnecessary complexity in lifetime resolution, potentially triggering ICEs or other compiler errors related to lifetime inference and checking."
    }
}