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

pub struct Alter_Lifetime_Annotations_In_HRTB_40;

impl Mutator for Alter_Lifetime_Annotations_In_HRTB_40 {
    fn name(&self) -> &str {
        "Alter_Lifetime_Annotations_In_HRTB_40"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct LifetimeReplacer {
            old_lifetime: Lifetime,
            new_lifetime: Lifetime,
        }
        
        impl VisitMut for LifetimeReplacer {
            fn visit_lifetime_mut(&mut self, i: &mut Lifetime) {
                if i.ident == self.old_lifetime.ident {
                    *i = self.new_lifetime.clone();
                }
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                let mut replacer = None;
                
                for input in &func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            for segment in &type_path.path.segments {
                                if let PathArguments::AngleBracketed(angle_bracketed) = &segment.arguments {
                                    for arg in &angle_bracketed.args {
                                        if let GenericArgument::Binding(binding) = arg {
                                            if let Type::Path(type_path) = &binding.ty {
                                                for segment in &type_path.path.segments {
                                                    if segment.ident == "for" {
                                                        for arg in &angle_bracketed.args {
                                                            if let GenericArgument::Lifetime(lifetime) = arg {
                                                                let old_lifetime = lifetime.clone();
                                                                let new_lifetime: Lifetime = parse_quote!('b);
                                                                replacer = Some(LifetimeReplacer {
                                                                    old_lifetime,
                                                                    new_lifetime,
                                                                });
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

                if let Some(mut replacer) = replacer {
                    replacer.visit_item_fn_mut(func);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}