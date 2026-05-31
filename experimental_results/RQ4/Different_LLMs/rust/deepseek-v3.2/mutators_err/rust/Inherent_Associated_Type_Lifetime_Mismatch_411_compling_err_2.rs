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

pub struct Inherent_Associated_Type_Lifetime_Mismatch_411;

impl Mutator for Inherent_Associated_Type_Lifetime_Mismatch_411 {
    fn name(&self) -> &str {
        "Inherent_Associated_Type_Lifetime_Mismatch_411"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut struct_assoc_types = HashSet::new();
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                for attr in &item_struct.attrs {
                    if attr.path().is_ident("allow") {
                        continue;
                    }
                }
                if let syn::Fields::Named(fields) = &item_struct.fields {
                    for field in &fields.named {
                        if let Some(ident) = &field.ident {
                            struct_assoc_types.insert((item_struct.ident.to_string(), ident.to_string()));
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for trait_item in &mut item_trait.items {
                    if let syn::TraitItem::Fn(trait_method) = trait_item {
                        if let ReturnType::Type(_, return_type) = &mut trait_method.sig.output {
                            let mut visitor = AssocTypeVisitor {
                                struct_assoc_types: &struct_assoc_types,
                                changed: false,
                            };
                            visitor.visit_type_mut(&mut return_type);
                            if visitor.changed {
                                let mut rng = thread_rng();
                                let new_lifetime = Lifetime::new(&format!("'invalid{}", rng.gen_range(0..100)), Span::call_site());
                                let mut replacer = LifetimeReplacer {
                                    new_lifetime,
                                };
                                replacer.visit_type_mut(&mut return_type);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait method signatures that return inherent associated types from structs with lifetime parameters. It identifies such return types and replaces their lifetime specifiers with fresh undeclared lifetime identifiers (e.g., 'invalid42). This creates a mismatch between the method's referenced lifetime and the associated type's defined lifetime parameters, potentially triggering region constraint errors and internal compiler errors similar to the reported bug."
    }
}

struct AssocTypeVisitor<'a> {
    struct_assoc_types: &'a HashSet<(String, String)>,
    changed: bool,
}

impl<'a> VisitMut for AssocTypeVisitor<'a> {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Path(type_path) = ty {
            if let Some(qself) = &type_path.qself {
                if let Type::Path(qself_type) = &*qself.ty {
                    if let Some(first_seg) = qself_type.path.segments.first() {
                        let struct_name = first_seg.ident.to_string();
                        if let PathArguments::AngleBracketed(args) = &first_seg.arguments {
                            for arg in &args.args {
                                if let GenericArgument::Lifetime(_) = arg {
                                    self.changed = true;
                                    return;
                                }
                            }
                        }
                        for seg in &type_path.path.segments {
                            let assoc_name = seg.ident.to_string();
                            if self.struct_assoc_types.contains(&(struct_name.clone(), assoc_name)) {
                                if let PathArguments::AngleBracketed(args) = &seg.arguments {
                                    for arg in &args.args {
                                        if let GenericArgument::Lifetime(_) = arg {
                                            self.changed = true;
                                            return;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            } else {
                if let Some(first_seg) = type_path.path.segments.first() {
                    let struct_name = first_seg.ident.to_string();
                    if let PathArguments::AngleBracketed(args) = &first_seg.arguments {
                        for arg in &args.args {
                            if let GenericArgument::Lifetime(_) = arg {
                                self.changed = true;
                                return;
                            }
                        }
                    }
                    for seg in &type_path.path.segments {
                        let assoc_name = seg.ident.to_string();
                        if self.struct_assoc_types.contains(&(struct_name.clone(), assoc_name)) {
                            if let PathArguments::AngleBracketed(args) = &seg.arguments {
                                for arg in &args.args {
                                    if let GenericArgument::Lifetime(_) = arg {
                                        self.changed = true;
                                        return;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
}

struct LifetimeReplacer {
    new_lifetime: Lifetime,
}

impl VisitMut for LifetimeReplacer {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Path(type_path) = ty {
            if let Some(qself) = &mut type_path.qself {
                if let Type::Path(qself_type) = &mut *qself.ty {
                    if let Some(first_seg) = qself_type.path.segments.first_mut() {
                        if let PathArguments::AngleBracketed(args) = &mut first_seg.arguments {
                            for arg in &mut args.args {
                                if let GenericArgument::Lifetime(lifetime) = arg {
                                    *lifetime = self.new_lifetime.clone();
                                }
                            }
                        }
                    }
                }
            } else {
                if let Some(first_seg) = type_path.path.segments.first_mut() {
                    if let PathArguments::AngleBracketed(args) = &mut first_seg.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::Lifetime(lifetime) = arg {
                                *lifetime = self.new_lifetime.clone();
                            }
                        }
                    }
                }
                for seg in &mut type_path.path.segments {
                    if let PathArguments::AngleBracketed(args) = &mut seg.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::Lifetime(lifetime) = arg {
                                *lifetime = self.new_lifetime.clone();
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
}