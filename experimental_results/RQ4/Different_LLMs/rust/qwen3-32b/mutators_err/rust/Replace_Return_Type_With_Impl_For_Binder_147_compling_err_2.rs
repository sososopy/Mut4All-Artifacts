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

pub struct Replace_Return_Type_With_Impl_For_Binder_147;

impl Mutator for Replace_Return_Type_With_Impl_For_Binder_147 {
    fn name(&self) -> &str {
        "Replace_Return_Type_With_Impl_For_Binder_147"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut traits = Vec::new();
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                traits.push(trait_item);
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let ReturnType::Type(_, _) = &func.sig.output {
                    if traits.is_empty() {
                        continue;
                    }
                    let selected_trait = traits.choose(&mut thread_rng()).unwrap();
                    let new_type = {
                        let mut bounds = Punctuated::new();
                        let for_binder = For {
                            for_token: token::For::default(),
                            generic_params: Punctuated::from_iter(vec![GenericParam::Type(parse_quote!(V))]),
                        };
                        let trait_bound = TraitBound {
                            modifier: TraitBoundModifier::For(for_binder),
                            path: parse_quote!(#selected_trait.ident),
                            paren_token: None,
                            lifetimes: None,
                        };
                        bounds.push(syn::TypeParamBound::Trait(trait_bound));
                        bounds.push(syn::TypeParamBound::Lifetime(Lifetime::new("'_", Span::call_site())));
                        TypeImplTrait {
                            impl_token: token::Impl::default(),
                            bounds,
                        }
                    };
                    let new_type = syn::Type::ImplTrait(new_type);
                    if let syn::ReturnType::Type(_, ty) = &mut func.sig.output {
                        let colon_token = token::Colon::default();
                        let colon_token: token::RArrow = unsafe { std::mem::transmute(colon_token) };
                        func.sig.output = syn::ReturnType::Type(colon_token, Box::new(new_type));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces a function's return type with a malformed `: (impl for<V> ExistingTrait<V> + '_>)` syntax. This exploits incorrect parsing of return types combined with generic binders, often triggering resolution errors or ICEs in compiler passes like HIR analysis."
    }
}