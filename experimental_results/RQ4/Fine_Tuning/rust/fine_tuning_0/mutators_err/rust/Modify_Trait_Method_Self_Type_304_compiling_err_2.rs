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
    TraitItemMethod,
};

use crate::mutator::Mutator;

pub struct Modify_Trait_Method_Self_Type_304;

impl Mutator for Modify_Trait_Method_Self_Type_304 {
    fn name(&self) -> &str {
        "Modify_Trait_Method_Self_Type_304"
    }
    fn mutate(&self, file: &mut syn::File) {
        let additional_trait = parse_quote!(Trait3);
        let mut trait_defined = false;

        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_method in &mut trait_item.items {
                    if let TraitItem::Method(method) = trait_method {
                        if method.sig.asyncness.is_some() {
                            if let Some(FnArg::Receiver(receiver)) = method.sig.inputs.first() {
                                if let Type::ImplTrait(type_impl_trait) = &*receiver.ty {
                                    let bounds = &mut type_impl_trait.bounds;
                                    if bounds.iter().any(|bound| match bound {
                                        TypeParamBound::Trait(trait_bound) => {
                                            trait_bound.path.is_ident("Trait3")
                                        }
                                        _ => false,
                                    }) {
                                        trait_defined = true;
                                    } else {
                                        bounds.push(TypeParamBound::Trait(TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: None,
                                            path: SynPath::from(additional_trait.clone()),
                                        }));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if !trait_defined {
            file.items.push(Item::Trait(parse_quote! {
                pub trait Trait3 {}
            }));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies async trait methods with self type bounds containing multiple traits and adds an additional trait constraint 'Trait3'. If 'Trait3' is not defined, it introduces a new trait declaration. This transformation tests the compiler's handling of trait bounds in async contexts, potentially revealing issues in trait resolution and async trait method handling."
    }
}