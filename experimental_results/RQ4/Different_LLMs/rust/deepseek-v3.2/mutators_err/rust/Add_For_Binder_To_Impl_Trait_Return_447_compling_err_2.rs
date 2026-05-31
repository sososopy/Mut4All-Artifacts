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

pub struct Add_For_Binder_To_Impl_Trait_Return_447;

impl Mutator for Add_For_Binder_To_Impl_Trait_Return_447 {
    fn name(&self) -> &str {
        "Add_For_Binder_To_Impl_Trait_Return_447"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut traits_with_assoc_types = HashSet::new();
        let mut functions_with_impl_trait_return = Vec::new();

        struct TraitCollector<'a> {
            traits: &'a mut HashSet<String>,
        }
        impl<'ast> Visit<'ast> for TraitCollector<'ast> {
            fn visit_item_trait(&mut self, i: &'ast syn::ItemTrait) {
                for item in &i.items {
                    if let syn::TraitItem::Type(_) = item {
                        self.traits.insert(i.ident.to_string());
                        break;
                    }
                }
                syn::visit::visit_item_trait(self, i);
            }
        }

        struct FunctionCollector<'a> {
            functions: &'a mut Vec<(usize, usize)>,
        }
        impl<'ast> Visit<'ast> for FunctionCollector<'ast> {
            fn visit_item_fn(&mut self, i: &'ast syn::ItemFn) {
                if let ReturnType::Type(_, ty) = &i.sig.output {
                    if let Type::ImplTrait(_) = &**ty {
                        self.functions.push((i.sig.ident.span().start().line, i.sig.ident.span().start().column));
                    }
                }
                syn::visit::visit_item_fn(self, i);
            }
            fn visit_impl_item_fn(&mut self, i: &'ast syn::ImplItemFn) {
                if let ReturnType::Type(_, ty) = &i.sig.output {
                    if let Type::ImplTrait(_) = &**ty {
                        self.functions.push((i.sig.ident.span().start().line, i.sig.ident.span().start().column));
                    }
                }
                syn::visit::visit_impl_item_fn(self, i);
            }
        }

        {
            let mut collector = TraitCollector { traits: &mut traits_with_assoc_types };
            collector.visit_file(file);
        }
        {
            let mut collector = FunctionCollector { functions: &mut functions_with_impl_trait_return };
            collector.visit_file(file);
        }

        if traits_with_assoc_types.is_empty() || functions_with_impl_trait_return.is_empty() {
            return;
        }

        let mut rng = thread_rng();
        let target_trait = traits_with_assoc_types.iter().next().unwrap().clone();

        struct MutatorVisitor {
            target_trait: String,
            rng: rand::rngs::ThreadRng,
            applied: bool,
        }
        impl VisitMut for MutatorVisitor {
            fn visit_item_fn_mut(&mut self, i: &mut syn::ItemFn) {
                if self.applied {
                    return;
                }
                if let ReturnType::Type(_, ty) = &mut i.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **ty {
                        if !type_impl_trait.bounds.is_empty() {
                            let first_bound = type_impl_trait.bounds.first().unwrap();
                            if let TypeParamBound::Trait(trait_bound) = first_bound {
                                let path_segments: Vec<String> = trait_bound.path.segments.iter().map(|seg| seg.ident.to_string()).collect();
                                if path_segments.contains(&self.target_trait) {
                                    let mut new_bounds = Punctuated::new();
                                    let fresh_param = Ident::new(&format!("T{}", self.rng.gen::<u32>()), Span::call_site());
                                    let mut new_trait_bound = trait_bound.clone();
                                    new_trait_bound.lifetimes = Some(BoundLifetimes {
                                        for_token: token::For::default(),
                                        lt_token: token::Lt::default(),
                                        lifetimes: {
                                            let mut lifetimes = Punctuated::new();
                                            lifetimes.push(GenericParam::Lifetime(LifetimeParam {
                                                attrs: Vec::new(),
                                                lifetime: Lifetime::new(&format!("'{}", fresh_param), Span::call_site()),
                                                colon_token: None,
                                                bounds: Punctuated::new(),
                                            }));
                                            lifetimes
                                        },
                                        gt_token: token::Gt::default(),
                                    });
                                    let mut new_path = new_trait_bound.path.clone();
                                    if let Some(last_seg) = new_path.segments.last_mut() {
                                        if let PathArguments::AngleBracketed(args) = &mut last_seg.arguments {
                                            for arg in &mut args.args {
                                                if let GenericArgument::AssocType(assoc) = arg {
                                                    assoc.ty = Type::ImplTrait(TypeImplTrait {
                                                        impl_token: token::Impl::default(),
                                                        bounds: {
                                                            let mut inner_bounds = Punctuated::new();
                                                            inner_bounds.push(TypeParamBound::Trait(TraitBound {
                                                                paren_token: None,
                                                                modifier: TraitBoundModifier::None,
                                                                lifetimes: None,
                                                                path: SynPath::from(Ident::new(&self.target_trait, Span::call_site())),
                                                            }));
                                                            inner_bounds
                                                        },
                                                    });
                                                }
                                            }
                                        }
                                    }
                                    new_trait_bound.path = new_path;
                                    new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                                    type_impl_trait.bounds = new_bounds;
                                    self.applied = true;
                                }
                            }
                        } else {
                            let mut new_bounds = Punctuated::new();
                            let fresh_param = Ident::new(&format!("T{}", self.rng.gen::<u32>()), Span::call_site());
                            let new_trait_bound = TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: Some(BoundLifetimes {
                                    for_token: token::For::default(),
                                    lt_token: token::Lt::default(),
                                    lifetimes: {
                                        let mut lifetimes = Punctuated::new();
                                        lifetimes.push(GenericParam::Lifetime(LifetimeParam {
                                            attrs: Vec::new(),
                                            lifetime: Lifetime::new(&format!("'{}", fresh_param), Span::call_site()),
                                            colon_token: None,
                                            bounds: Punctuated::new(),
                                        }));
                                        lifetimes
                                    },
                                    gt_token: token::Gt::default(),
                                }),
                                path: SynPath::from(Ident::new(&self.target_trait, Span::call_site())),
                            };
                            new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                            type_impl_trait.bounds = new_bounds;
                            self.applied = true;
                        }
                    }
                }
                syn::visit_mut::visit_item_fn_mut(self, i);
            }
            fn visit_impl_item_fn_mut(&mut self, i: &mut syn::ImplItemFn) {
                if self.applied {
                    return;
                }
                if let ReturnType::Type(_, ty) = &mut i.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **ty {
                        if !type_impl_trait.bounds.is_empty() {
                            let first_bound = type_impl_trait.bounds.first().unwrap();
                            if let TypeParamBound::Trait(trait_bound) = first_bound {
                                let path_segments: Vec<String> = trait_bound.path.segments.iter().map(|seg| seg.ident.to_string()).collect();
                                if path_segments.contains(&self.target_trait) {
                                    let mut new_bounds = Punctuated::new();
                                    let fresh_param = Ident::new(&format!("T{}", self.rng.gen::<u32>()), Span::call_site());
                                    let mut new_trait_bound = trait_bound.clone();
                                    new_trait_bound.lifetimes = Some(BoundLifetimes {
                                        for_token: token::For::default(),
                                        lt_token: token::Lt::default(),
                                        lifetimes: {
                                            let mut lifetimes = Punctuated::new();
                                            lifetimes.push(GenericParam::Lifetime(LifetimeParam {
                                                attrs: Vec::new(),
                                                lifetime: Lifetime::new(&format!("'{}", fresh_param), Span::call_site()),
                                                colon_token: None,
                                                bounds: Punctuated::new(),
                                            }));
                                            lifetimes
                                        },
                                        gt_token: token::Gt::default(),
                                    });
                                    let mut new_path = new_trait_bound.path.clone();
                                    if let Some(last_seg) = new_path.segments.last_mut() {
                                        if let PathArguments::AngleBracketed(args) = &mut last_seg.arguments {
                                            for arg in &mut args.args {
                                                if let GenericArgument::AssocType(assoc) = arg {
                                                    assoc.ty = Type::ImplTrait(TypeImplTrait {
                                                        impl_token: token::Impl::default(),
                                                        bounds: {
                                                            let mut inner_bounds = Punctuated::new();
                                                            inner_bounds.push(TypeParamBound::Trait(TraitBound {
                                                                paren_token: None,
                                                                modifier: TraitBoundModifier::None,
                                                                lifetimes: None,
                                                                path: SynPath::from(Ident::new(&self.target_trait, Span::call_site())),
                                                            }));
                                                            inner_bounds
                                                        },
                                                    });
                                                }
                                            }
                                        }
                                    }
                                    new_trait_bound.path = new_path;
                                    new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                                    type_impl_trait.bounds = new_bounds;
                                    self.applied = true;
                                }
                            }
                        } else {
                            let mut new_bounds = Punctuated::new();
                            let fresh_param = Ident::new(&format!("T{}", self.rng.gen::<u32>()), Span::call_site());
                            let new_trait_bound = TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: Some(BoundLifetimes {
                                    for_token: token::For::default(),
                                    lt_token: token::Lt::default(),
                                    lifetimes: {
                                        let mut lifetimes = Punctuated::new();
                                        lifetimes.push(GenericParam::Lifetime(LifetimeParam {
                                            attrs: Vec::new(),
                                            lifetime: Lifetime::new(&format!("'{}", fresh_param), Span::call_site()),
                                            colon_token: None,
                                            bounds: Punctuated::new(),
                                        }));
                                        lifetimes
                                    },
                                    gt_token: token::Gt::default(),
                                }),
                                path: SynPath::from(Ident::new(&self.target_trait, Span::call_site())),
                            };
                            new_bounds.push(TypeParamBound::Trait(new_trait_bound));
                            type_impl_trait.bounds = new_bounds;
                            self.applied = true;
                        }
                    }
                }
                syn::visit_mut::visit_impl_item_fn_mut(self, i);
            }
        }

        let mut visitor = MutatorVisitor {
            target_trait,
            rng,
            applied: false,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions returning `impl Trait` and adds a higher-ranked binder (`for<T>`) to the trait bound, or nests an `impl Trait` within an associated type projection if a binder already exists. It reuses existing traits with associated types from the seed program, introducing fresh generic parameters. This creates complex canonicalization scenarios involving higher-ranked trait bounds, associated type defaults, and nested opaque types, mimicking the bug report's conditions to stress the compiler's internal logic."
    }
}