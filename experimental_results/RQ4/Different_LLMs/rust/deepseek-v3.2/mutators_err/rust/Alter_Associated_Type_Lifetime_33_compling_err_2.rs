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

pub struct Alter_Associated_Type_Lifetime_33;

impl Mutator for Alter_Associated_Type_Lifetime_33 {
    fn name(&self) -> &str {
        "Alter_Associated_Type_Lifetime_33"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        let mut visitor = LifetimeProjectionVisitor::new();
                        visitor.visit_type_mut(&mut method.sig.output);
                        for input in &mut method.sig.inputs {
                            visitor.visit_fn_arg_mut(input);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait definitions containing methods where the return type or parameter type includes an associated type projection with an anonymous lifetime (e.g., Self::Assoc<'_>). It changes the lifetime to 'static or removes it entirely, creating a mismatch between the trait definition and its implementations. This mutation tests the compiler's handling of lifetime mismatches in associated type projections, particularly focusing on borrow checking diagnostics and internal slicing operations."
    }
}

struct LifetimeProjectionVisitor {
    rng: rand::rngs::ThreadRng,
}

impl LifetimeProjectionVisitor {
    fn new() -> Self {
        Self { rng: thread_rng() }
    }
}

impl VisitMut for LifetimeProjectionVisitor {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        match ty {
            Type::Path(type_path) => {
                for segment in &mut type_path.path.segments {
                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        let mut new_args = Punctuated::new();
                        for arg in &mut args.args {
                            if let GenericArgument::Lifetime(lifetime) = arg {
                                if lifetime.ident == "_" {
                                    let choice = self.rng.gen_range(0..2);
                                    if choice == 0 {
                                        new_args.push(GenericArgument::Lifetime(Lifetime::new("'static", Span::call_site())));
                                    } else {
                                        continue;
                                    }
                                } else {
                                    new_args.push(arg.clone());
                                }
                            } else if let GenericArgument::Type(inner_type) = arg {
                                self.visit_type_mut(inner_type);
                                new_args.push(arg.clone());
                            } else {
                                new_args.push(arg.clone());
                            }
                        }
                        args.args = new_args;
                    }
                }
            }
            Type::ImplTrait(type_impl_trait) => {
                for bound in &mut type_impl_trait.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        self.visit_path_mut(&mut trait_bound.path);
                    }
                }
            }
            Type::TraitObject(type_trait_object) => {
                for bound in &mut type_trait_object.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        self.visit_path_mut(&mut trait_bound.path);
                    }
                }
            }
            _ => {}
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }

    fn visit_fn_arg_mut(&mut self, arg: &mut FnArg) {
        match arg {
            FnArg::Typed(pat_type) => {
                self.visit_type_mut(&mut pat_type.ty);
            }
            _ => {}
        }
        syn::visit_mut::visit_fn_arg_mut(self, arg);
    }

    fn visit_path_mut(&mut self, path: &mut SynPath) {
        for segment in &mut path.segments {
            if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                let mut new_args = Punctuated::new();
                for arg in &mut args.args {
                    if let GenericArgument::Lifetime(lifetime) = arg {
                        if lifetime.ident == "_" {
                            let choice = self.rng.gen_range(0..2);
                            if choice == 0 {
                                new_args.push(GenericArgument::Lifetime(Lifetime::new("'static", Span::call_site())));
                            } else {
                                continue;
                            }
                        } else {
                            new_args.push(arg.clone());
                        }
                    } else {
                        new_args.push(arg.clone());
                    }
                }
                args.args = new_args;
            }
        }
        syn::visit_mut::visit_path_mut(self, path);
    }
}