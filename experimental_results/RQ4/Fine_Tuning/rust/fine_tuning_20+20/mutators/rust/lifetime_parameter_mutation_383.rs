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

pub struct Lifetime_Parameter_Mutation_383;

impl Mutator for Lifetime_Parameter_Mutation_383 {
    fn name(&self) -> &str {
        "Lifetime_Parameter_Mutation_383"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let mut has_lifetime = false;
                for param in &item_struct.generics.params {
                    if let syn::GenericParam::Lifetime(_) = param {
                        has_lifetime = true;
                        break;
                    }
                }
                if has_lifetime {
                    let mut rng = thread_rng();
                    let lifetime_idx = rng.gen_range(0..item_struct.generics.params.len());
                    if let syn::GenericParam::Lifetime(lifetime_param) =
                        &mut item_struct.generics.params[lifetime_idx]
                    {
                        lifetime_param.lifetime = syn::Lifetime::new("'static", Span::call_site());
                    }
                } else {
                    let new_lifetime_param: syn::LifetimeParam = parse_quote!('static);
                    item_struct
                        .generics
                        .params
                        .push(syn::GenericParam::Lifetime(new_lifetime_param));
                    if let syn::Fields::Named(ref mut fields) = item_struct.fields {
                        if !fields.named.is_empty() {
                            let mut rng = thread_rng();
                            let field_idx = rng.gen_range(0..fields.named.len());
                            let field = &mut fields.named[field_idx];
                            let mut ty = field.ty.clone();
                            let mut lifetime_replaced = false;
                            syn::visit_mut::visit_type_mut(
                                &mut LifetimeReplacer {
                                    target_lifetime: syn::Lifetime::new(
                                        "'static",
                                        Span::call_site(),
                                    ),
                                    replaced: &mut lifetime_replaced,
                                },
                                &mut ty,
                            );
                            if lifetime_replaced {
                                field.ty = ty;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets struct declarations with lifetime parameters. It either replaces an existing lifetime with `'static` or introduces a new `'static` lifetime if none are present, ensuring the new lifetime is used in a field type. This stresses the compiler's lifetime resolution and checking mechanisms by leveraging the reserved `'static` lifetime."
    }
}

struct LifetimeReplacer {
    target_lifetime: syn::Lifetime,
    replaced: *mut bool,
}

impl syn::visit_mut::VisitMut for LifetimeReplacer {
    fn visit_lifetime_mut(&mut self, node: &mut syn::Lifetime) {
        if !node.ident.to_string().starts_with('\'') {
            node.ident = self.target_lifetime.ident.clone();
            unsafe {
                *self.replaced = true;
            }
        }
    }
}