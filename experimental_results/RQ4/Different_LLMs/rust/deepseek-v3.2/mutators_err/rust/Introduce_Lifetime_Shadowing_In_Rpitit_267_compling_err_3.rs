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

pub struct Introduce_Lifetime_Shadowing_In_Rpitit_267;

impl Mutator for Introduce_Lifetime_Shadowing_In_Rpitit_267 {
    fn name(&self) -> &str {
        "Introduce_Lifetime_Shadowing_In_Rpitit_267"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let mut trait_lifetime_param_name: Option<String> = None;
                if let Some((_, trait_ref, _)) = &item_impl.trait_ {
                    let trait_path = &trait_ref;
                    for trait_item in &file.items {
                        if let Item::Trait(item_trait) = trait_item {
                            if item_trait.ident == trait_path.segments.last().unwrap().ident {
                                for trait_item in &item_trait.items {
                                    if let TraitItem::Type(assoc_type) = trait_item {
                                        for generic_param in &assoc_type.generics.params {
                                            if let GenericParam::Lifetime(lifetime_param) = generic_param {
                                                trait_lifetime_param_name = Some(lifetime_param.lifetime.ident.to_string());
                                                break;
                                            }
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
                if let Some(lifetime_name) = trait_lifetime_param_name {
                    let mut has_rpitit_method = false;
                    for impl_item in &mut item_impl.items {
                        if let ImplItem::Fn(method) = impl_item {
                            if let ReturnType::Type(_, return_type) = &method.sig.output {
                                if let Type::ImplTrait(_) = &**return_type {
                                    has_rpitit_method = true;
                                    break;
                                }
                            }
                        }
                    }
                    if has_rpitit_method {
                        let new_lifetime_param = LifetimeParam {
                            attrs: Vec::new(),
                            lifetime: Lifetime::new(&format!("'{}", lifetime_name), Span::call_site()),
                            colon_token: None,
                            bounds: Punctuated::new(),
                        };
                        item_impl.generics.params.insert(0, GenericParam::Lifetime(new_lifetime_param));
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Fn(method) = impl_item {
                                if let ReturnType::Type(_, return_type) = &mut method.sig.output {
                                    if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                                        for bound in &mut type_impl_trait.bounds {
                                            if let TypeParamBound::Trait(trait_bound) = bound {
                                                let mut visitor = LifetimeReplacementVisitor {
                                                    target_lifetime: lifetime_name.clone(),
                                                };
                                                visitor.visit_trait_bound_mut(trait_bound);
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
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations that use RPITIT. It identifies traits with associated types containing lifetime parameters. When an impl block implements such a trait and contains a method returning impl Trait referencing the associated type, the mutation introduces a duplicate lifetime parameter in the impl block's generics that shadows the trait's lifetime parameter. Then, within the RPITIT return type, it replaces or adds references to the associated type using the shadowed lifetime. This creates lifetime shadowing conflicts that may trigger internal compiler errors related to region checking and RPITIT refinement."
    }
}

struct LifetimeReplacementVisitor {
    target_lifetime: String,
}

impl VisitMut for LifetimeReplacementVisitor {
    fn visit_path_mut(&mut self, path: &mut SynPath) {
        for segment in &mut path.segments {
            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                for arg in &mut args.args {
                    if let GenericArgument::Lifetime(lifetime) = arg {
                        lifetime.ident = Ident::new(&self.target_lifetime, Span::call_site());
                    }
                }
            }
        }
        syn::visit_mut::visit_path_mut(self, path);
    }
}