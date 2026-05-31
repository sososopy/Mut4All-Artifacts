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

pub struct Convert_Higher_Ranked_Assoc_To_Nested_Opaque_167;

impl Mutator for Convert_Higher_Ranked_Assoc_To_Nested_Opaque_167 {
    fn name(&self) -> &str {
        "Convert_Higher_Ranked_Assoc_To_Nested_Opaque_167"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                self.mutate_fn_signature(&mut item_fn.sig);
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        self.mutate_fn_signature(&mut func.sig);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function signatures with higher-ranked trait bounds (for<'a>) that have associated types. It transforms the return type by replacing the concrete associated type binding with a nested opaque impl Trait (e.g., impl Sized). This creates complex nested opaque type scenarios under higher-ranked lifetimes, potentially triggering compiler bugs in opaque type handling, associated type projection, and lifetime resolution."
    }
}

impl Convert_Higher_Ranked_Assoc_To_Nested_Opaque_167 {
    fn mutate_fn_signature(&self, sig: &mut Signature) {
        if let ReturnType::Type(_, ref mut return_type) = sig.output {
            if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                let bounds = &mut type_impl_trait.bounds;
                for bound in bounds.iter_mut() {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        if let Some(ref mut lifetimes) = trait_bound.lifetimes {
                            if lifetimes.lifetimes.len() > 0 {
                                if let Some(last_segment) = trait_bound.path.segments.last_mut() {
                                    if let PathArguments::AngleBracketed(ref mut args) = last_segment.arguments {
                                        for arg in args.args.iter_mut() {
                                            if let GenericArgument::AssocType(assoc_type) = arg {
                                                let new_type = Type::ImplTrait(TypeImplTrait {
                                                    impl_token: token::Impl { span: Span::call_site() },
                                                    bounds: {
                                                        let mut bounds = Punctuated::new();
                                                        bounds.push(TypeParamBound::Trait(TraitBound {
                                                            paren_token: None,
                                                            modifier: TraitBoundModifier::None,
                                                            lifetimes: None,
                                                            path: SynPath::from(Ident::new("Sized", Span::call_site())),
                                                        }));
                                                        bounds
                                                    },
                                                });
                                                assoc_type.ty = new_type;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}