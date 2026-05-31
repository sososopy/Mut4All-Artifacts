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

pub struct Struct_With_Impl_Trait_In_Field_409;

impl Mutator for Struct_With_Impl_Trait_In_Field_409 {
    fn name(&self) -> &str {
        "Struct_With_Impl_Trait_In_Field_409"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_names = HashSet::new();
        for item in &file.items {
            match item {
                Item::Trait(item_trait) => {
                    trait_names.insert(item_trait.ident.to_string());
                }
                Item::Impl(item_impl) => {
                    if let Some((_, path, _)) = &item_impl.trait_ {
                        if let Some(segment) = path.segments.last() {
                            trait_names.insert(segment.ident.to_string());
                        }
                    }
                }
                _ => {}
            }
        }
        let mut rng = thread_rng();
        let mut struct_indices = Vec::new();
        for (i, item) in file.items.iter().enumerate() {
            if matches!(item, Item::Struct(_)) {
                struct_indices.push(i);
            }
        }
        let target_struct_index = struct_indices.choose(&mut rng).copied();
        if let Some(index) = target_struct_index {
            if let Item::Struct(item_struct) = &mut file.items[index] {
                let mut target_field_index = None;
                for (i, field) in item_struct.fields.iter().enumerate() {
                    if let Type::Path(_) = &field.ty {
                        target_field_index = Some(i);
                        break;
                    }
                }
                if let Some(field_index) = target_field_index {
                    if let Type::Path(type_path) = &item_struct.fields[field_index].ty {
                        if let Some(segment) = type_path.path.segments.last() {
                            if trait_names.contains(&segment.ident.to_string()) {
                                return;
                            }
                        }
                    }
                    let trait_name = trait_names.iter().choose(&mut rng).cloned();
                    if let Some(trait_name) = trait_name {
                        item_struct.fields[field_index].ty =
                            Type::ImplTrait(TypeImplTrait {
                                impl_token: token::Impl::default(),
                                bounds: {
                                    let mut bounds = Punctuated::new();
                                    bounds.push(TypeParamBound::Trait(TraitBound {
                                        paren_token: None,
                                        modifier: TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: SynPath::from(Ident::new(
                                            &trait_name,
                                            Span::call_site(),
                                        )),
                                    }));
                                    bounds
                                },
                            });
                    }
                } else {
                    let trait_name = trait_names.iter().choose(&mut rng).cloned();
                    if let Some(trait_name) = trait_name {
                        let new_field: syn::Field = parse_quote! {
                            new_field: impl #trait_name
                        };
                        if let syn::Fields::Named(fields_named) = &mut item_struct.fields {
                            fields_named.named.push(new_field);
                        } else if let syn::Fields::Unnamed(fields_unnamed) = &mut item_struct.fields
                        {
                            fields_unnamed.unnamed.push(new_field);
                        }
                    }
                }
            }
        } else {
            let struct_name = Ident::new("MutatedStruct", Span::call_site());
            let trait_name = trait_names.iter().choose(&mut rng).cloned();
            if let Some(trait_name) = trait_name {
                let new_struct: ItemStruct = parse_quote! {
                    struct #struct_name {
                        new_field: impl #trait_name,
                    }
                };
                file.items.push(Item::Struct(new_struct));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets struct definitions by introducing or modifying a field to use `impl Trait`, where `Trait` is a trait already present in the seed program. It ensures no explicit implementation of the trait for types within the struct, aiming to stress-test the compiler's handling of trait bounds and associated types in opaque contexts."
    }
}