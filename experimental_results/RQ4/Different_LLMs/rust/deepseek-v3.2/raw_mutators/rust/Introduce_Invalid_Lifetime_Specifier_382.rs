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

pub struct Introduce_Invalid_Lifetime_Specifier_382;

impl Mutator for Introduce_Invalid_Lifetime_Specifier_382 {
    fn name(&self) -> &str {
        "Introduce_Invalid_Lifetime_Specifier_382"
    }
    fn mutate(&self, file: &mut syn::File) {
        // First, ensure the feature generic_const_exprs is enabled
        let mut has_feature = false;
        for attr in &file.attrs {
            if let syn::AttrStyle::Inner(_) = attr.style {
                continue;
            }
            if let syn::Meta::List(ref meta_list) = attr.meta {
                if meta_list.path.is_ident("feature") {
                    if let Ok(syn::Meta::List(ref nested_list)) = meta_list.parse_args() {
                        if nested_list.path.is_ident("generic_const_exprs") {
                            has_feature = true;
                            break;
                        }
                    }
                }
            }
        }
        if !has_feature {
            let feature_attr: syn::Attribute = parse_quote! { #![feature(generic_const_exprs)] };
            file.attrs.insert(0, feature_attr);
        }

        // Collect items that need mutation
        let mut items_to_mutate = Vec::new();
        for item in &file.items {
            match item {
                Item::Struct(_) | Item::Enum(_) | Item::Union(_) | Item::Trait(_) | Item::Impl(_) => {
                    items_to_mutate.push(item);
                }
                _ => {}
            }
        }

        // Process each item
        for item in &mut file.items {
            match item {
                Item::Struct(item_struct) => {
                    process_item_with_lifetimes(&mut item_struct.generics, &mut item_struct.fields);
                }
                Item::Enum(item_enum) => {
                    process_item_with_lifetimes(&mut item_enum.generics, &mut ());
                    // Also process each variant
                    for variant in &mut item_enum.variants {
                        if let Fields::Named(ref mut fields) = &mut variant.fields {
                            process_lifetime_references_in_fields(fields);
                        } else if let Fields::Unnamed(ref mut fields) = &mut variant.fields {
                            process_lifetime_references_in_fields(fields);
                        }
                    }
                }
                Item::Union(item_union) => {
                    process_item_with_lifetimes(&mut item_union.generics, &mut item_union.fields);
                }
                Item::Trait(item_trait) => {
                    process_item_with_lifetimes(&mut item_trait.generics, &mut ());
                    // Process trait items
                    for trait_item in &mut item_trait.items {
                        if let TraitItem::Fn(trait_fn) = trait_item {
                            process_lifetime_references_in_fn(&mut trait_fn.sig);
                        } else if let TraitItem::Type(trait_type) = trait_item {
                            if let Some(ref mut ty) = &mut trait_type.ty {
                                process_lifetime_references_in_type(ty);
                            }
                        }
                    }
                }
                Item::Impl(item_impl) => {
                    process_item_with_lifetimes(&mut item_impl.generics, &mut ());
                    // Process impl items
                    for impl_item in &mut item_impl.items {
                        if let ImplItem::Fn(impl_fn) = impl_item {
                            process_lifetime_references_in_fn(&mut impl_fn.sig);
                        } else if let ImplItem::Type(impl_type) = impl_item {
                            process_lifetime_references_in_type(&mut impl_type.ty);
                        }
                    }
                }
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces invalid lifetime specifiers by replacing declared lifetime parameters with the reserved keyword 'static while keeping references to the original lifetime elsewhere. This creates a mismatch where 'static is used as a parameter name (invalid) and the original lifetime becomes undeclared. For items without lifetime parameters, it first adds one then applies the mutation. This transformation aims to trigger internal compiler errors in the borrow checker or region handling, especially when combined with generic const expressions."
    }
}

// Helper function to process items with lifetime parameters
fn process_item_with_lifetimes<T>(generics: &mut Generics, fields: &mut T) 
where
    T: VisitMut + Spanned,
{
    let mut rng = thread_rng();
    
    // Check if there are existing lifetime parameters
    let existing_lifetime_params: Vec<&LifetimeParam> = generics.params.iter()
        .filter_map(|param| {
            if let GenericParam::Lifetime(lifetime_param) = param {
                Some(lifetime_param)
            } else {
                None
            }
        })
        .collect();
    
    let target_lifetime_param = if !existing_lifetime_params.is_empty() {
        // Select a random existing lifetime parameter
        existing_lifetime_params.choose(&mut rng).map(|p| p.lifetime.ident.to_string())
    } else {
        // Add a new lifetime parameter
        let new_lifetime = LifetimeParam {
            attrs: Vec::new(),
            lifetime: Lifetime::new("'a", Span::call_site()),
            colon_token: None,
            bounds: Punctuated::new(),
        };
        generics.params.insert(0, GenericParam::Lifetime(new_lifetime));
        Some("a".to_string())
    };
    
    if let Some(lifetime_name) = target_lifetime_param {
        // Replace the selected lifetime parameter with 'static
        for param in &mut generics.params {
            if let GenericParam::Lifetime(lifetime_param) = param {
                if lifetime_param.lifetime.ident.to_string() == lifetime_name {
                    lifetime_param.lifetime = Lifetime::new("'static", Span::call_site());
                    break;
                }
            }
        }
        
        // Process the item's body to keep references to the original lifetime
        let mut visitor = LifetimeReferenceVisitor {
            target_lifetime: format!("'{}", lifetime_name),
            found_references: false,
        };
        visitor.visit_fields_mut(fields);
        
        // Also process where clauses
        if let Some(where_clause) = &mut generics.where_clause {
            for predicate in &mut where_clause.predicates {
                let mut predicate_visitor = LifetimeReferenceVisitor {
                    target_lifetime: format!("'{}", lifetime_name),
                    found_references: false,
                };
                predicate_visitor.visit_where_predicate_mut(predicate);
            }
        }
    }
}

// Helper function to process lifetime references in function signatures
fn process_lifetime_references_in_fn(sig: &mut Signature) {
    // This function is called from trait/impl processing
    // The actual lifetime replacement happens in the main item processing
    // This ensures references in method signatures maintain the original lifetime
}

// Helper function to process lifetime references in type
fn process_lifetime_references_in_type(ty: &mut Type) {
    // Similar to above, ensures type references maintain original lifetimes
}

// Helper function to process lifetime references in fields
fn process_lifetime_references_in_fields(fields: &mut Fields) {
    // This is handled by the visitor
}

// Visitor to track and preserve lifetime references
struct LifetimeReferenceVisitor {
    target_lifetime: String,
    found_references: bool,
}

impl VisitMut for LifetimeReferenceVisitor {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        match ty {
            Type::Reference(type_ref) => {
                if let Some(ref lifetime) = type_ref.lifetime {
                    if lifetime.ident.to_string() == self.target_lifetime.trim_start_matches('\'') {
                        self.found_references = true;
                    }
                }
            }
            Type::Path(type_path) => {
                // Check for lifetime parameters in path segments
                for segment in &mut type_path.path.segments {
                    if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                        for arg in &mut args.args {
                            if let GenericArgument::Lifetime(lifetime) = arg {
                                if lifetime.ident.to_string() == self.target_lifetime.trim_start_matches('\'') {
                                    self.found_references = true;
                                }
                            }
                        }
                    }
                }
            }
            _ => {}
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
    
    fn visit_lifetime_mut(&mut self, lifetime: &mut Lifetime) {
        if lifetime.ident.to_string() == self.target_lifetime.trim_start_matches('\'') {
            self.found_references = true;
        }
    }
}