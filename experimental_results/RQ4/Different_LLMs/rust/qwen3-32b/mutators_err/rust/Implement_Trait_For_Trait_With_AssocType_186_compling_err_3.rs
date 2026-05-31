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

pub struct Implement_Trait_For_Trait_With_AssocType_186;

impl Mutator for Implement_Trait_For_Trait_With_AssocType_186 {
    fn name(&self) -> &str {
        "Implement_Trait_For_Trait_With_AssocType_186"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some(trait_) = &item_impl.trait_ {
                    let trait_path = &trait_.1;
                    let dyn_type = syn::Type::TraitObject(syn::TypeTraitObject {
                        dyn_token: Some(syn::token::Dyn::default()),
                        bounds: {
                            let mut bounds = syn::punctuated::Punctuated::new();
                            let bound = syn::TypeParamBound::Trait(syn::TraitBound {
                                paren_token: None,
                                modifier: syn::TraitBoundModifier::None,
                                lifetimes: None,
                                path: trait_path.clone(),
                            });
                            bounds.push(bound);
                            bounds
                        },
                    });
                    item_impl.self_ty = Box::new(dyn_type);
                    item_impl.generics.params.push(parse_quote!(T));
                    item_impl.items.push(parse_quote! {
                        type Assoc = T;
                    });
                    let static_item: syn::Item = parse_quote! {
                        static __mutator_static: <dyn #trait_path as #trait_path>::Assoc = ();
                    };
                    file.items.push(static_item);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}